#ifndef __ANDLINK_H
#define __ANDLINK_H


/***for Andlink_sendBootstrap_BC***/
#define PORT 5683
#define REGIST_RSP_TIMEOUT 3
#define RSP_TIMEOUT -2

//#define DEBUG_WITH_PC

#define NO_RATE_LIMIT					0
#if defined (CONFIG_RTL_8198)|| defined (CONFIG_RTL_8198C) || defined (CONFIG_RTL_8367R_SUPPORT)
#define MAX_LINK_RATE					1024000
#else
#define MAX_LINK_RATE					102400
#endif


typedef enum {
ANDLINK_IF5_START,      //开始
ANDLINK_IF5_BOOTSTRAP,  //注册
ANDLINK_IF5_BOOTSTRAP_BC,  //注册成功广播
ANDLINK_IF5_BOOT,       //上线
ANDLINK_IF5_MQTTCONNECT, //MQTT长连线
ANDLINK_IF5_POLLING,    //轮询状态
} ANDLINK_IF5_STATE_T;

typedef struct ADNLINK_IF5_S 
{
    int err;
	int disconnect_flag;
	int respCode;
	int port;

	char *pub_buf;  //存放需要pub的json数据
	char *will_payload; //存放遗嘱消息

/***for Andlink_sendBootstrap_BC***/
	char localIp[16];
	char br0Ip[16];
	int reg_respCode;
	int bc_retry;
	int is_cloud;

/***for restart mqtt connection thread***/
	int is_thread1_alive;
	//int is_thread2_alive;

    Andlink_basicInfo *basic_info;
    ANDLINK_IF5_STATE_T IF5_stat;
}ANDLINK_IF5;


typedef enum { 
	T_WIFIPARAMSYNC,
    T_WIFISWITCH, 
    T_RADIOCONFIG, 
    T_WPS, 
    T_MACFILTER, 
    T_REBOOT, 
    T_LEDCONTROL, 
    T_ROAMINGCONFIG, 
    T_DEVICELIMIT,
    T_TIMERTASK,
    T_GET_APCONFIGINFO, 
    T_GET_UPLINKSTATUS, 
    T_GET_STAINFO, 
    T_GET_ALLDEVICELIMIT,
    T_GET_AllTIMERTASK,
    T_GET_WIFISTATS, 
    T_COMMAND_UNBIND, 
    T_GET_FILE,
   // T_DISCONNECT,
   //IF5新增出错下发事件type
    T_RESPONSE_APCONFIGINGO,
    T_RESPONSE_UPLINKSTATUS,
    T_RESPONSE_STAINFO,
    T_RESPONSE_WIFISTATS,
    T_RESPONSE_FILE,
} ANDLINK_IF5_TYPE_T;

typedef struct ANDLINK_MQTT_REQ_IF5_S
{
	int port;
	int keepalive;
	char host[256];
	char id[RTL_LINK_MAX_INFO_LEN];
	char username[RTL_LINK_MAX_INFO_LEN];
	char password[RTL_LINK_MAX_INFO_LEN_EXTRA];
	
	char sub_topic[128];
	char pub_topic[128];
	//char *sub_buf; //用来承接sub的topic返回的数据
	char *pub_buf; //用来存放需要pub出去的数据
	
	char willtopic[128];  //遗嘱topic：等于pub_topic
	char *willpayload; //遗嘱消息：设备离线消息
	bool willretain;
	int willqos;

//#ifdef WITH_TLS
	char cafile[128];
	//char capath[128];
	//char certfile[128];
	//char keyfile[128];
	//char ciphers[128];
	//bool insecure;
	//char tls_version[128];
//#ifdef WITH_TLS_PSK
	//char psk[128];
	//char psk_identity[128];
//#endif
//#endif
	
}ANDLINK_MQTT_REQ_IF5;


int Andlink_if5_form_disconnect_message(ANDLINK_IF5 *p);
int Andlink_if5_mqtt_session(ANDLINK_IF5 *p_if5, cJSON *json);
static int Andlink_if5_curlPost(char *type, ANDLINK_CURL_REQ_IF5 *req);
//int Andlink_sendBootstrap_BC(ANDLINK_IF5 *p_if5);


ANDLINK_IF5_TYPE_T Andlink_if5_getType(char *s);
char *Result2Str(ERR_NUM_T errno);
//int Andlink_mqttsub(ANDLINK_MQTT_REQ_IF5 mqtt_req);
int Andlink_mqttsub(ANDLINK_MQTT_REQ_IF5 *mqtt_req);

int Andlink_mqttpub(ANDLINK_MQTT_REQ_IF5 *mqtt_req);


int Andlink_if5_form_post_rsp(ANDLINK_IF5 *p, cJSON *data_control, ERR_NUM_T errno, char *function);
int Andlink_if5_form_get_rsp(ANDLINK_IF5 *p, ANDLINK_IF5_TYPE_T type, cJSON *function);


ERR_NUM_T Andlink_if5_postRoamingConfig(cJSON *data);
ERR_NUM_T Andlink_if5_postUpgrade(cJSON *data);
ERR_NUM_T Andlink_if5_postWiFiSwitch(cJSON *data);
ERR_NUM_T Andlink_if5_postRadioConfig(cJSON *data);
ERR_NUM_T Andlink_if5_postWPS(cJSON *data);
ERR_NUM_T Andlink_if5_postMacFilter(cJSON *data);
ERR_NUM_T Andlink_if5_postReboot(cJSON *data);
ERR_NUM_T Andlink_if5_postLEDControl(cJSON *data);
ERR_NUM_T Andlink_if5_postUnbind(cJSON *data);

ERR_NUM_T Andlink_if5_getApConfigInfo(cJSON *data);
ERR_NUM_T Andlink_if5_getUplinkStatus(cJSON *data);
ERR_NUM_T Andlink_if5_getSTAInfo(cJSON *data);
ERR_NUM_T Andlink_if5_getWiFiStats(cJSON *data);

#endif
