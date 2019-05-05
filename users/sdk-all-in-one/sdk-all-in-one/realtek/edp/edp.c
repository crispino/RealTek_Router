#include <sys/time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <ont/edp.h>
#include <ont/log.h>
#include <ont/device.h>
#ifndef ONT_DECREASE_SIZE
#include "Openssl.h"
#endif
#include "common.h"
#include "message.h"
#include "cmcc_api.h"

ont_device_t *dev = NULL;
static int test_mode=0;

/************************************************************************/
#include "cJSON.h"

static char* reg_test_TBL[]={
	MSG_WTP_ONLINE_NOTIFICATION_REQ,
};

typedef int	(*ont_req_Send)(void **data,int *datalen);

static int ont_MSG_AP_GET_WIFI_SWITCH_STATUS_REQ_send(void **data,int *datalen);
static int ont_MSG_WTP_GET_DEVICE_WIFI_REQ_send(void **data,int *datalen);
static int ont_MSG_WTP_SET_DEVICE_WIFI_REQ_send(void **data,int *datalen);
static int ont_MSG_WTP_SCAN_DEVICES_REQ_send(void **data,int *datalen);


typedef struct ReqRspInfo
{	
	char* req_cmd;
	char* rsp_cmd;
	ont_req_Send	*ops;
}ReqRspInfo_T,*ReqRspInfo_Tp;

static  ReqRspInfo_T rsp_test_TBL[] = {
	{MSG_AP_GET_WIFI_SWITCH_STATUS_REQ,MSG_AP_GET_WIFI_SWITCH_STATUS_RSP,ont_MSG_AP_GET_WIFI_SWITCH_STATUS_REQ_send},
	{MSG_WTP_GET_DEVICE_WIFI_INFO_REQ,MSG_WTP_GET_DEVICE_WIFI_INFO_RSP,ont_MSG_WTP_GET_DEVICE_WIFI_REQ_send},
	{MSG_WTP_SET_DEVICE_WIFI_REQ,MSG_WTP_SET_DEVICE_WIFI_RSP,ont_MSG_WTP_SET_DEVICE_WIFI_REQ_send},
	/* app */
	{MSG_WTP_SCAN_DEVICES_REQ,NULL,ont_MSG_WTP_SCAN_DEVICES_REQ_send}
};


/*MSG_AP_GET_WIFI_SWITCH_STATUS */
static int ont_MSG_AP_GET_WIFI_SWITCH_STATUS_REQ_send(void **data,int *datalen)
{
	cJSON *json = NULL;
	char* szJsonData = NULL;
	int ret = ONT_ERR_OK;

	json = cJSON_CreateObject();

	ont_req_CreateReqHeader(json,MSG_AP_GET_WIFI_SWITCH_STATUS_REQ);
	szJsonData = cJSON_PrintUnformatted(json);

	*datalen= strlen(szJsonData);
	*data=(unsigned char*)malloc(strlen(szJsonData)+1);
	if(*data == NULL){
		ont_platform_free(szJsonData);
		cJSON_Delete(json);
		return -1;
	}
	memcpy(*data,szJsonData,strlen(szJsonData)+1);
	
	ret = ont_req_send(szJsonData,strlen(szJsonData));
	ont_platform_free(szJsonData);
	cJSON_Delete(json);	
	return ret;
}

/*MSG_WTP_GET_DEVICE_WIFI_INFO */
typedef struct device_wifi_info
{
	char	SSID[257];
	char	CODE[1025];
	char	encryptType[33];
}DEVICE_WIFI_INFO_T,*DEVICE_WIFI_INFO_Tp;


static int ont_MSG_WTP_GET_DEVICE_WIFI_REQ_send(void **data,int *datalen)
{
	DEVICE_WIFI_INFO_T info;
	cJSON *json = NULL;
	char* szJsonData = NULL;
	int ret = ONT_ERR_OK;
	memset(&info,0,sizeof(DEVICE_WIFI_INFO_T));

	json = cJSON_CreateObject();

	ont_req_CreateReqHeader(json,MSG_WTP_GET_DEVICE_WIFI_INFO_REQ);
	szJsonData = cJSON_PrintUnformatted(json);

	*datalen= strlen(szJsonData);
	*data=(unsigned char*)malloc(strlen(szJsonData)+1);
	if(*data == NULL){
		ont_platform_free(szJsonData);
		cJSON_Delete(json);
		return -1;
	}
	memcpy(*data,szJsonData,strlen(szJsonData)+1);

	ret = ont_req_send(szJsonData,strlen(szJsonData));
	ont_platform_free(szJsonData);
	cJSON_Delete(json);	
	return ret;
}


/*MSG_WTP_SET_DEVICE_WIFI */
static int ont_MSG_WTP_SET_DEVICE_WIFI_REQ_send(void **data,int *datalen)
{
	char *endata = NULL;
	char encrypt_data[1024] = {0};
	cJSON *json = NULL;
	char* szJsonData = NULL;
	int ret = ONT_ERR_OK;
	
	json = cJSON_CreateObject();

	ont_req_CreateReqHeader(json,MSG_WTP_SET_DEVICE_WIFI_REQ);

	cJSON_AddItemToObject(json, "SSID", cJSON_CreateString("wifi_test_ssid"));
	endata = ONT_ENCRYPT("11223");

	cJSON_AddItemToObject(json, "CODE", cJSON_CreateString(endata));
	if(endata){
		free(endata);
		endata = NULL;
	}


	cJSON_AddItemToObject(json, "encryptType", cJSON_CreateString("2"));	/*wep*/
	
	szJsonData = cJSON_PrintUnformatted(json);

	*datalen = strlen(szJsonData);
	*data=(unsigned char*)malloc(strlen(szJsonData)+1);
	if(*data == NULL){
		ont_platform_free(szJsonData);
		cJSON_Delete(json);
		return -1;
	}
	memcpy(*data,szJsonData,strlen(szJsonData)+1);

	ret = ont_req_send(szJsonData,strlen(szJsonData));
	ont_platform_free(szJsonData);
	cJSON_Delete(json);	
	return ret;
}


/* app */
typedef struct app_devices
{
	char 	IP[21];
	char	port[5];
}APP_DEVICES_T,*APP_DEVICES_Tp;
#define MIB_IP_ADDR			170

static int ont_MSG_WTP_SCAN_DEVICES_REQ_send(void **data,int *datalen)
{
	cJSON *json = NULL;
	char* szJsonData = NULL;
	int ret = ONT_ERR_OK;
	
	json = cJSON_CreateObject();
	
	unsigned char buffer1[64]={0};
	unsigned char lanip[64]={0};
	char lanip_0[64]={0};			
	char lanip0[8]={0},lanip1[8]={0},lanip2[8]={0},lanip3[8]={0};
	struct in_addr inIp1={0};

	if(!apmib_get( MIB_IP_ADDR,  (void *)&buffer1))
	{
		printf("Get ip addr fail\n");
	}
	
	sprintf(lanip,"%s",inet_ntoa(*((struct in_addr *)buffer1)));
	sscanf(lanip,"%[^.].%[^.].%[^.].%s",lanip0,lanip1,lanip2,lanip3);
	sprintf(lanip_0,"%s.%s.%s.0",lanip0,lanip1,lanip2);

	ont_req_CreateReqHeader(json,MSG_WTP_SCAN_DEVICES_REQ);

	cJSON_AddItemToObject(json, "IP", cJSON_CreateString(lanip_0));
	cJSON_AddItemToObject(json, "port", cJSON_CreateString("2017"));	
	
	szJsonData = cJSON_PrintUnformatted(json);

	*datalen = strlen(szJsonData);
	*data=(unsigned char*)malloc(strlen(szJsonData) + 1);
	if(*data == NULL){
		ont_platform_free(szJsonData);
		cJSON_Delete(json);
		return -1;
	}
	memcpy(*data,szJsonData,strlen(szJsonData)+1);

	ret = ont_req_send(szJsonData,strlen(szJsonData));
	ont_platform_free(szJsonData);
	cJSON_Delete(json);	
	return ret;
}



static void ont_req_send_test(void)
{	
	int i,err = 0;
	
	for(i = 0;i< sizeof(reg_test_TBL)/sizeof(char*);i++){
		err = ont_req_ops(reg_test_TBL[i]);
	    if (ONT_ERR_OK != err)
	    	fprintf(stderr,"req=%s send fail\n",reg_test_TBL[i]);
	}
	return;
}

static void ont_rsp_rcv_test(void)
{
	int i = 0,datalen = 0,ret = 0;
	unsigned char* data = NULL;
	int ;
	int	(*ops)(void **data,int datalen);	

	for(i = 0;i < sizeof(rsp_test_TBL)/sizeof(ReqRspInfo_T);i++){
		/* send req */
		ops=rsp_test_TBL[i].ops;
		if(ops != NULL){
			ret = ops(&data,&datalen);
			if(ret != ONT_ERR_OK)
				fprintf(stderr,"req=%s,rsp=%s,req create ops fail\n",rsp_test_TBL[i].req_cmd,rsp_test_TBL[i].rsp_cmd);
		}else
			fprintf(stderr,"req=%s,rsp=%s,req create ops NULL\n",rsp_test_TBL[i].req_cmd,rsp_test_TBL[i].rsp_cmd);

		
		/*rsp */
		if(ont_rsp_ops(data,datalen) != ONT_ERR_OK)
			fprintf(stderr,"req=%s,rsp=%s,rsp fail\n",rsp_test_TBL[i].req_cmd,rsp_test_TBL[i].rsp_cmd);

		if(data){
			free(data);
			data = NULL;
		}
	}
	return;
}

/************************************************************************/


static void sample_log(void *ctx, ont_log_level_t ll , const char *format, ...)
{
    static const char *level[] = {
	"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
    };
    va_list ap;
    struct timeval tv;
    time_t t;
    struct tm *st;

    gettimeofday(&tv, NULL);

    t = tv.tv_sec;
    st = localtime(&t);

    printf("[%02d:%02d:%02d.%03d] [%s] ", st->tm_hour, st->tm_min,
	    st->tm_sec, (int)tv.tv_usec/1000, level[ll]);

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    printf("\n");
}

#define PRODUCT_RELEASE
#ifdef PRODUCT_RELEASE
#define SVR_NAME		"networkingsrv"
#else
#define SVR_NAME		"devenv"
#endif
/* send function */
int _ont_send_rsp(char* data,int datalen)
{
	int err;
	{
		if(!test_mode){
			err = ont_edp_send_transdata(dev, SVR_NAME, data, datalen);
			if (ONT_ERR_OK != err)
			{
				ONT_LOG0(ONTLL_ERROR, "Failed to send  req rsp");
				return err;
			}
		}
	}

	return ONT_ERR_OK;
}

int _ont_req_send(char* data,int datalen)
{
	int err;
	if(!test_mode){
		err = ont_edp_send_transdata(dev, SVR_NAME, data, datalen);
		if (ONT_ERR_OK != err)
		{
			ONT_LOG0(ONTLL_ERROR, "Failed to send req");
			return err;
		}
	}

	return ONT_ERR_OK;
}

void edpc_client_sigusr1_process(int signum)
{
	int err;

	/* send request */
	err = ont_req_ops(MSG_WTP_DEVICE_STATUS_CHANG_REQ);
	if (ONT_ERR_OK != err)
    {
	    ONT_LOG1(ONTLL_ERROR, "Failed to send MSG_WTP_DEVICE_STATUS_CHANG_REQ  msg to the server, error=%d", err);;
    } 
		
}

void ont_sys_disconnect(void)
{
	const ont_device_rt_t *rt;
	rt = ont_device_get_rt(dev->type);
	ont_platform_sleep(500); 
	rt->destroy(dev);
	ont_platform_sleep(1000);
	
	return;
}

void trans_data_cb_func(const char*svr_name,size_t svr_len,
	const char* data,size_t data_len)
{
	unsigned char* tmp = data;
	*(tmp+data_len) = '\0';
	ont_rsp_ops(data,data_len);
}

int start_edp(unsigned char *sn, unsigned char *auth, unsigned char *softVersion, unsigned char *fac)
{
	int err,status = 0,i;
    struct hostent *hosts;
    int32_t next_dp_time;

	/* apmib */
	if ( !apmib_init()) {
		printf("Initialize AP MIB failed in setSystemTime_flash!\n");
		return -1;
	}

/*
	if(cmcc_get_ABLITITY_NOTIFY_REQ_init(sn,fac,softVersion) != RTK_SUCCESS){
		printf("ABLITITY_NOTIFY_REQ init fail!\n");
		return -1;
	}
*/
	cmcc_api_update();

	if(test_mode){
		printf("%s:%d,req cmd send test start\n",__func__,__LINE__);
		ont_req_send_test();
		printf("%s:%d,rsp cmd rcv test start\n",__func__,__LINE__);
		ont_rsp_rcv_test();
		return 0;
	}

#ifdef PRODUCT_RELEASE
	err = ont_device_create(91867, ONTDEV_EDP, "auto_register", &dev);
#else
	err = ont_device_create(85, ONTDEV_EDP, "auto_register", &dev);
#endif

	if (ONT_ERR_OK != err) {
		ONT_LOG1(ONTLL_ERROR, "Failed to create device instance, error=%d", err);
		return -1;
	}

	hosts = gethostbyname(ONT_SERVER_ADDRESS);
	if (NULL == hosts)
	{
		ONT_LOG2(ONTLL_ERROR, "Failed to get the IP of the server('%s'), "
			"for: %d", ONT_SERVER_ADDRESS, h_errno);
		return -1;
	}

	for (i = 0; hosts->h_addr_list[i]; ++i)
	{
		if (AF_INET == hosts->h_addrtype)
			break;
	}

	if (NULL == hosts->h_addr_list[i])
	{
		ONT_LOG0(ONTLL_ERROR, "No invalide address of the server.");
		return -1;
	}
	ont_edp_set_transdata_cb(dev,trans_data_cb_func);
	err = ont_device_connect_without_autoReg(dev,
		inet_ntoa(*(struct in_addr*)hosts->h_addr_list[i]),
		ONT_SERVER_PORT,
#ifdef PRODUCT_RELEASE
		"zuwang",
#else
		"omq_test",
#endif
		auth,
		30);
	if (ONT_ERR_OK != err)
	{
		ont_device_destroy(dev);

		ONT_LOG1(ONTLL_ERROR, "Failed to connect to the server, error=%d", err);
		return -1;
	}

	/* register signal*/
	signal(SIGUSR1,edpc_client_sigusr1_process);

	/* MSG_GET_INITINFO_REQ*/
	err = ont_req_ops(MSG_GET_INITINFO_REQ);
	if (ONT_ERR_OK != err)
	{
		ont_device_destroy(dev);
		ONT_LOG1(ONTLL_ERROR, "Failed to get init info from server, error=%d", err);
		return -1;
	}
	/* MSG_WTP_ABILITY_NOTIFY_REQ */
	err = ont_req_ops(MSG_WTP_ABILITY_NOTIFY_REQ);
	if (ONT_ERR_OK != err)
	{
		ont_device_destroy(dev);
		ONT_LOG1(ONTLL_ERROR, "Failed to notify ont ability to the server, error=%d", err);
		return -1;
	} 

	/* check if upgrade ok */
	if(cmcc_get_DEVUPDATE_STATUS(&status) == RTK_SUCCESS){
		if(status == 1){
			cmcc_set_DEVUPDATE_STATUS(0);
			cmcc_api_update();

			/* send request */
			err = ont_req_ops(MSG_WTP_DEVICE_STATUS_CHANG_REQ);
			if (ONT_ERR_OK != err)
			{
				ont_device_destroy(dev);
				ONT_LOG1(ONTLL_ERROR, "Failed to send MSG_WTP_DEVICE_STATUS_CHANG_REQ  msg to the server, error=%d", err);
				return -1;
			} 
		}
	}

	next_dp_time = (int32_t)time(NULL) + 3;
	while (1)
	{
		err = ont_device_keepalive(dev);
		if (ONT_ERR_OK != err)
		{
			ONT_LOG1(ONTLL_ERROR, "device is not alive, error=%d", err);
			break;
		}
		
		if (next_dp_time <= time(NULL))
		{
			next_dp_time = (int32_t)time(NULL) + 10;

#if 1			
			err = ont_req_ops(MSG_GET_DEVUPDATE_INFO_REQ);
			if (ONT_ERR_OK != err)
			{
				ONT_LOG1(ONTLL_ERROR, "Failed to send MSG_GET_DEVUPDATE_INFO_REQ  msg to the server, error=%d", err);
				break;
			}

			err = ont_req_ops(MSG_WTP_LOG_UPLOAD_REQ);
			if (ONT_ERR_OK != err)
			{
				ONT_LOG1(ONTLL_ERROR, "Failed to send MSG_WTP_LOG_UPLOAD_REQ  msg to the server, error=%d", err);
				break;
			}
#endif
		}
		ont_platform_sleep(50);/* 50 ms*/
	}

	ont_device_destroy(dev);
	return 0;

}

int main( int argc, char *argv[] )
{
	int ch;
    unsigned char key_md5[16];
	unsigned char mac[21] = {0},sn[33] = {0},auth[256] = {0};
	unsigned char fac[33]  = {0},softVersion[257] = {0};

	ont_log_set_logger(NULL, sample_log);
#ifndef ONT_DECREASE_SIZE
    /*zuwang en|decrypt demo start*/
    MD5String((const unsigned char*)"app@2017",key_md5);
    InitAes(key_md5);
#endif

	while((ch = getopt(argc,argv,"tl:n:m:s:v:"))!= -1){
		switch(ch){
		case 'l':
			ont_rtk_set_debugLevel(atoi(optarg));
			break;
		case 'n':
			strcpy(sn,optarg);
			break;
		case 'm':
			strcpy(mac,optarg);
			break;
		case 's':
			strcpy(softVersion,optarg);
			break;
		case 'v':
			strcpy(fac,optarg);
			break;
		case 't':
			test_mode = 1;
			break;
		default: 
			;
		}
	}

	ont_platform_snprintf(auth,sizeof(auth),"{\"SN\":\"%s\",\"MAC\":\"%s\"}",sn,mac);

	while(1)
	{
		start_edp(sn, auth, softVersion, fac);
		ont_platform_sleep(5*1000);
	}

	return 0;
}

