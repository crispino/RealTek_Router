#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <ont/mqtt.h>
#include <ont/log.h>
#include <ont/video.h>
#include <ont/video_rvod.h>
#include "device_onvif.h"
#include "cJSON.h"
#include "onvif_config.h"
#include "device_config.h"
#include "status_sync.h"
#include "message.h"
#include "reboot_task.h"
#include "isp_info.h"

#ifdef ONT_OS_POSIX
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#ifdef WIN32
#include "windows.h"
#include "Dbghelp.h"
#include "tchar.h"
#include "assert.h"
#pragma comment(lib, "Dbghelp.lib")
#endif

#define M_ONVIF_SLEEPTIME           500
#define M_ONVIF_TICK_REBOOT_CHECK   7200

static void _log(void *ctx, ont_log_level_t ll , const char *format, ...)
{
    static const char *level[] = {
        "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
    };
    va_list ap;
    time_t timer;
    struct tm *timeinfo;
    char buffer [64];
    timer = time(NULL);
    timeinfo = localtime(&timer);
    strftime (buffer,sizeof(buffer),"%Y/%m/%d %H:%M:%S",timeinfo);

    printf("[%s] [%s]", buffer, level[ll]);

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    printf("\n");
}


void log_redirect()
{
    char name[128];
    struct tm *timeinfo;
    time_t timer;
    timer = time(NULL);
    timeinfo = localtime(&timer);
    strftime (name,sizeof(name),"log%Y-%m-%d.txt",timeinfo);

    fflush(stdout);
    setvbuf(stdout,NULL,_IONBF,0);
    int save_fd = dup(STDOUT_FILENO);
    int fd = open(name,(O_RDWR | O_CREAT|O_APPEND), 0644);
    dup2(fd,STDOUT_FILENO);
}


int ont_video_playlist_singlestep(void *dev, ont_list_t *list);

int ont_add_onvif_one_device(const char*url, const char*usr, const char*passwd);

int ont_isp_publish(ont_device_t *dev, int ispid);


int ont_add_onvif_devices()
{
    int i;
    int j = cfg_get_channel_num();
    struct _onvif_channel * channels = cfg_get_channels();
    for (i = 0; i < j; i++)
    {
        if (ont_onvifdevice_adddevice(channels[i].channelid, channels[i].url, channels[i].user, channels[i].pass) != 0)
        {
            ONT_LOG0(ONTLL_ERROR, "Failed to login the onvif server");
        }
    }
    return j;
}

void sync_channel_rvod(ont_device_t *dev)
{

}

#ifdef WIN32
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{

    TCHAR szFull[260];
    TCHAR szDrive[3];
    TCHAR szDir[256];
    GetModuleFileName(NULL, szFull, sizeof(szFull) / sizeof(char));
    _tsplitpath(szFull, szDrive, szDir, NULL, NULL);
    strcpy(szFull, szDrive);
    strcat(szFull, szDir);
    strcat(szFull, "//");
    strcat(szFull, lpstrDumpFilePathName);

    // 创建Dump文件
    //
    HANDLE hDumpFile = CreateFile(szFull, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Dump信息
    //
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    // 写入Dump文件内容
    //
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    //FatalAppExit(-1, "*** Unhandled Exception! ***");

    CreateDumpFile("video_sample.dmp", pException);

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void* device_connect()
{
    static ont_device_t *dev=NULL;
    char macaddr[32];
    char devname[64];
    int err;
    int product_id = devcfg_get_profile()->productid;

    status_gethwaddr(macaddr);

    snprintf(devname, sizeof(devname), "video-%s",macaddr);
    if (!dev)
    {
        err = ont_device_create(product_id, ONTDEV_MQTT, devname, &dev);
        if (ONT_ERR_OK != err) {
            ONT_LOG1(ONTLL_ERROR, "Failed to create device instance, error=%d", err);
            return NULL;
        }
    }
    //http://172.19.3.6:9090
    //183.230.40.33:80
    dev->device_id = 0;
    err = ont_device_connect(dev,
            &devcfg_get_profile()->svraddr[0],
            devcfg_get_profile()->svrport,
            &devcfg_get_profile()->regcode[0],
                            devname,
                            120);
    if (ONT_ERR_OK != err)
    {
        ont_device_destroy(dev);
        dev = NULL;
        ONT_LOG1(ONTLL_ERROR, "Failed to connect to the server, error=%d", err);
        return NULL;
    }
    return dev;
}


int main( int argc, char *argv[] )
{
    log_redirect();

#ifdef WIN32
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif

    //ont_onvif_device_discovery();
    ont_device_t *dev=NULL;
    int err;
    int delta = 0;
    int needreconnect = 0;
    int ispid = ONTISP_CM;

    ont_device_cmd_t *cmd = NULL;
    int chnnum = 0;

    if (devcfg_initilize("device_config.json")<0)
    {
        ONT_LOG0(ONTLL_ERROR, "device config file error");
    }

    /*if config file error, need connect to the platform and receive command*/
    if (cfg_initilize("config.json")<0)
    {
        ONT_LOG0(ONTLL_ERROR, "config file error");
    }

#ifdef ONT_OS_POSIX
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;//设定接受到指定信号后的动作为忽略
    sa.sa_flags = 0;
    if (sigemptyset(&sa.sa_mask) == -1 || //初始化信号集为空
        sigaction(SIGPIPE, &sa, 0) == -1)
    {
        //屏蔽SIGPIPE信号
        exit(1);
    }
#endif
    ont_log_set_logger(NULL, _log);
    ont_log_set_log_level(ONTLL_DEBUG);

    ispid = isp_parse_info();

    ont_add_onvif_devices();
    chnnum = cfg_get_channel_num();
    while (1)
    {
        if (!dev)
        {
            dev = device_connect();
        }
        if (!dev)
        {
            ONT_LOG1(ONTLL_ERROR, "device is not alive, error=%d", errno);
            ont_platform_sleep(1000);
            continue;
        }
        else
        {
            break;
        }
    }

    if ( !status_synced(chnnum))
    {
        ont_video_dev_set_channels(dev, chnnum);
    }

	err = ont_isp_publish(dev, ispid);
	if (ONT_ERR_OK != err)
	{
		ONT_LOG1(ONTLL_ERROR, "Failed to publish ISP type, error=%d", err);
	}


    messagehandle_init();
    subconnect_wait();
    int running_tick=0;
    while (1)
    {
        running_tick++;
        if (!dev)
        {
            dev = device_connect();
        }
        if (!dev)
        {
            ONT_LOG1(ONTLL_ERROR, "device is not alive, error=%d", errno);
            ont_platform_sleep(5000);
            continue;
        }

        if (dev)
        {

            err = ont_device_keepalive(dev);
            if (ONT_ERR_OK != err)
            {
                ONT_LOG1(ONTLL_ERROR, "device is not alive, error=%d", err);
                dev = NULL;
            }
        }

        if (dev)
        {
            /*GET PLATFORM CMD*/
            cmd = ont_device_get_cmd(dev);
        }

        if(NULL != cmd)
        {
            chnnum = ont_get_channleid(dev, cmd->req);
            messagesend(dev, chnnum, cmd, 0/*not used*/);

        }
        else
        {
            ont_platform_sleep(500);
        }
        if (running_tick > M_ONVIF_TICK_REBOOT_CHECK){
            sys_check_and_reboot_at(23,0,0);
        }

    }

    ont_device_destroy(dev);
    return 0;
}
