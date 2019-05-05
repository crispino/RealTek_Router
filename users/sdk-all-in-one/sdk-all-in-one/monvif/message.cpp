#include "video.h"
#include "ont_onvif_if.h"
#include "onvif_config.h"

#include "device.h"
#include "message.h"


#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"


threadinfo_t *g_threads=NULL;
static volatile int _g_SockInit=0;

    

int messagehandle_init()
{
    int s = 0;
    long i =0;
    int threadnum = cfg_get_thread_num();   
    g_threads = ont_platform_malloc(threadnum*sizeof(threadinfo_t));
    memset(g_threads, 0x00, threadnum*sizeof(threadinfo_t));

    for (i=0; i<threadnum; i++)
    {
        g_threads[i].index=i;
        g_threads[i].list = ont_list_create();
        
        g_threads[i].play_env= ont_onvifdevice_create_playenv();
        
        s = pthread_create(&g_threads[i].thread_id, &g_threads[i].attr,
                           &messagehandle_proc, &g_threads[i]);
        if (s != 0)
        {
            printf("create thread err\n");
            abort();
        }
    }
    return 0;
}


int subconnect_wait()
{
    struct sockaddr_in sin;   
    struct sockaddr_in cin;     
    int fd;
    int c_fd;
    int i = 0;
    int index=0;
    socklen_t len;
    
    char addr_p[36];
    short port = cfg_get_port();
    int maxwait = cfg_get_thread_num();;
    int n;
    bzero((void *)&sin,sizeof(sin));
    sin.sin_family = AF_INET;   
    sin.sin_addr.s_addr = INADDR_ANY;  
    sin.sin_port = htons(port); 
    fd = socket(AF_INET,SOCK_STREAM,0);
    int option = 1;
    if (setsockopt ( fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) ) < 0)
    {
        printf("set sock opt err\n");
        return 1 ;       
    }

    if (bind(fd,(struct sockaddr *)&sin,sizeof(sin))<0)
    {
        printf("bind err\n");
        return 2 ;
    }
    if (listen(fd, 10)<0)
    {
        printf("listen err\n");
        return 3;
    }
    
    _g_SockInit = 1;
    while(1)
    {
        c_fd = accept(fd, (struct sockaddr *)&cin,&len);
        if (c_fd)
        {
            printf("wait %d ...\n", i);
            ont_platform_sleep(500);
            n = read(c_fd, &index, sizeof(index));
            g_threads[index].sock_srv = c_fd;
            i++;   
        }
        else
        {
            abort();
        }
        
        if (i>=maxwait)
        {
            break;
        }
    }
    printf("wait done\n");
    return 0;
}


static void _handle_proc(void* cliendData, int mask)
{
    int cnt=0;
    message me;
	ont_device_cmd_t *cmd;
    if (mask&SOCKET_READABLE)
    {
        threadinfo_t *info = cliendData;
        cnt = read(info->sock_cli, &me, sizeof(me));
        if (cnt < sizeof(me))
        {
            printf("read err, %d, errno %d\n", cnt, errno);
            abort();            
        }
        cmd = me.data;
        ont_videocmd_handle(me.dev, info->index, cmd->req);
        ont_device_cmd_destroy(cmd);
    }
    return ;
}

static int ont_setmessage_handle(void *penv, int sock, HandlerProc proc, void *arg)
{
	BasicUsageEnvironment* env = (BasicUsageEnvironment*)penv;
	TaskScheduler &scheduler = env->taskScheduler();   
    scheduler.setBackgroundHandling(sock,  SOCKET_READABLE | SOCKET_EXCEPTION, proc, arg);
    return 0;
}


int subconnect(int index)
{
    short port = cfg_get_port();
    struct sockaddr_in sin;
    int fd;
    int ret = 0;
    const char *serverIP = "127.0.0.1";
    bzero((void *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;   
     
    inet_pton(AF_INET,serverIP,(void *)&(sin.sin_addr));
    sin.sin_port = htons(port);
    fd=socket(AF_INET,SOCK_STREAM,0);
    ret = connect(fd,(struct sockaddr *)&(sin),sizeof(sin));
    if (ret ==0)
    {  
        write(fd, &index, sizeof(index));
        g_threads[index].sock_cli=fd;
    }
    ont_setmessage_handle(g_threads[index].play_env, fd, _handle_proc,  &g_threads[index]);
    return fd;    
}

int messagesend(void *dev, int channel, void* cmd, int size)
{
    int threadnum = cfg_get_thread_num();
    threadinfo_t *info =  &g_threads[channel%threadnum];
    char *ptr =0;
    int send=0;
    message me;
    me.dev = dev;
    me.data=cmd;
    me.type=0;
    me.size = sizeof(message);
    ptr = (char*)&me;
    while (send<sizeof(me))
    {
        send += write(info->sock_srv, ptr+send, sizeof(me));
    }
    return 0;
}

    

void *messagehandle_proc(void *arg)
{
    threadinfo_t *info =  arg;
    int index = info->index;
    while(!_g_SockInit)
    {
        ont_platform_sleep(300);
    }
    
    int sock = subconnect(info->index);
    if (sock<=0)
    {
        printf("sub connect err\n");
        abort();        
    }
    sock = g_threads[index].sock_cli;   
    while (1)
    {
        ont_onvifdevice_live_stream_singlestep(g_threads[index].play_env, 1000);
        checkneedclose(g_threads[index].list);
    }
}

