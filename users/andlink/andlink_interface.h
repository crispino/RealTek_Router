#ifndef __ANDLINK_INTERFACE_H
#define __ANDLINK_INTERFACE_H
#include "apmib.h"
#define PATH_GWSEARCHACK    "qlink/searchack"
#define PATH_QLINKINFO      "qlink/netinfo"
#define PATH_WIFISYNC       "apdevice/control/WiFiParameterSync"
#define PATH_WIFISWITCH     "apdevice/control/WiFiSwitch"
#define PATH_RADIOCONFIG    "apdevice/control/RadioConfig"
#define PATH_WPS            "apdevice/control/WPS"
#define PATH_MACFILTER      "apdevice/control/MacFilter"
#define PATH_REBOOT         "apdevice/control/Reboot"
#define PATH_LED            "apdevice/control/LEDControl"
#define PATH_ROAMING        "apdevice/control/RoamingConfig"
#define PATH_ROAMING_REQ    "apdevice/control/RoamingRequest"
#define PATH_APCONFIG       "apdevice/query/ApConfigInfo"
#define PATH_UPLINK         "apdevice/query/UplinkStatus"
#define PATH_STAINFO        "apdevice/query/STAInfo"
#define PATH_WIFISTATS      "apdevice/query/WiFiStats"
#define PATH_ROAMING_QUERY  "apdevice/query/RoamingConfig"
#define PATH_SEARCHAP 		"apdevice/searchap"
#define PATH_ADDGW          "qlink/addgw"
#define PATH_APPSEARCHDEV	"qlink/searchdevice"
#define PATH_QLINKQUERYGW	"qlink/querygw"

#define PATH_UPGRADE        "device/command/file"
#define PATH_UNBIND         "device/command/unbind"

#define PATH_SEARCHGW       "qlink/searchgw"
#define PATH_QLINKREQ       "qlink/request"
#define PATH_QLINKSUCCESS   "qlink/success"
#define PATH_BOOTSTRAP      "device/inform/bootstrap"
#define PATH_BOOT           "device/inform/boot"
#define PATH_HEARTBEAT      "device/inform/heartbeat"
#define PATH_INFORMDATA     "device/inform/data"
#define PATH_REGIST         "qlink/regist"
#define PATH_AUTOBOOT       "espapi/m2m/rest/devices/cgw"

#define ANDLINK_LOG_DIR     "/var/andlink_log.txt"
#define ANDLINK_SET_MIB_DIR "/var/andlink_set_mib.txt"
#define ANDLINK_IF5_EXIT   "/var/tmp/andlink_if5_exit"
#define ANDLINK_RECEIVE_NETINFO   "/var/tmp/andlink_receive_netinfo"

#define CONTENT_TYPE "Content-type: application/json"
#define ACCEPT_CHARSET "Accept-Charset: utf-8"
#define MAX_MESSAGE_LEN 2048
//#define IF5_DEFAULT_USER_KEY      "CMHI-AN1202L"

typedef enum err_num {
    ERROR=-1, 
    RECEIVED_SUCCESS=0,
    SUCCESS=1,
    AUTHORIZATION_FAILED=1000,
    INVALID_ARGUMENT=1001,
    LACK_OF_ARGUMENT=1002,
    INVALID_VALUE=1003,
    INVALID_COMMAND=1004,
    FILE_TRANSFER_COMPLETE=2000,
    FILE_UPGRADE_SUCCESS=2001,
    FILE_TRANSFERRING=2002,
    FILE_TRANSFER_INTERRUPT=2003,
    DEVICE_UNREGIST=3001,  //for andlink if5, added by elaine
    UNKNOW_ERROR=9999,
} ERR_NUM_T;

typedef enum event_type{
    AP_STAONLINE,
    AP_STAOFFLINE,
    AP_STAREFUSED,
} EVENT_TYPE_T;

#define OTHERS              		-1
#define GETIP_TIMEOUT               -2
#define SEARCH_GW_TIMEOUT       	-3
#define SCAN_DEFSSID_TIMEOUT    	-4
#define CONNECT_DEFSSID_TIMEOUT 	-5	
#define CONNECT_WIRELESS_TIMEOUT    -6
#define COAP_RSP_TIMEOUT    		-7	
#define ANDLINK_ERROR       		-8	
#define ANDLINK_TOKEN_MISSMATCH  	-9
#define ANDLINK_UNBIND				-10
#define WAIT_APP_NETINFO_TIMEOUT    -11
#define CLOUD_ERR_RSP_TIMEOUT       -12
#define CLOUD_ERR_FAIL              -13
#define CLOUD_ERR_REGISTER_ILLEGAL  -14
#define CLOUD_ERR_NEED_REREGISTER   -15
#define CLOUD_ERR_RECONNECT_SERVER  -16
#define CLOUD_ERR_DISCONNECT        -17
#define COAP_RSP_TIMEOUT_HB			-18
#define	INTERNET_ACCESS_FAIL_AFTER_SEARCH_GW_TO		-19
#define INTERNET_ACCESS_SUCCESS_AFTER_SEARCH_GW_TO		-20
#define PPPOE_CONNECT_FAIL			-21
#define	PPPOE_CONNECT_SUCCESS		-22
#define IF5_AUTO_BOOT_SUCCESS		-23
#define IF5_GET_NETINFO_SUCCESS		-24
#define IF5_START_DIRECT			-25
#define START_IF_AFTER_SWITCH_BRIDGE	-26

/***for andlink_if5******/
#define ANDLINK_IF5_ERROR        -18
#define ANDLINK_IF5_UNREGIST                -19
//#define ANDLINK_IF5_TOKEN_MISSMATCH         -21
#define ANDLINK_IF5_MQTT_DISCONNECT         -20
#define ANDLINK_IF5_MQTT_SESSION_FAILED     -21
#define ANDLINK_IF5_INFORMFWDOWNLOADSTATUS_FAILED  -22
#define ANDLINK_IF5_INFORMUPGRADE_FAILED    -23
#define ANDLINK_IF5_MOSQ_ERR                -24
#define ANDLINK_IF5_AUTOBOOT_DISCONNECT     -25


#define ANDLINK_GETIP_STEADY        999
#define	ANDLINK_PPPOE_CONNECT_STEADY	99999

//#define RTL_LINK_RESTART_BLOCK "/var/rtl_link_restart_block"

#define	ANDLINK_IF5_RUNNING		1
#define	ANDLINK_IF6_RUNNING		2
#define	ANDLINK_IF3_RUNNING		4
#define	ANDLINK_IF6_POLL_RUNNING	8

#define ANDLINK_SUPPORT_IF5 	0x1
#define ANDLINK_SUPPORT_IF6 	0x2

typedef struct Andlink_endpoint_s{
    int method;               /* (i.e. POST, PUT or GET) */
    char *path;   /* path towards a resource (i.e. foo/bar/) */
    ERR_NUM_T (*resouce_handler)(cJSON *);
} Andlink_endpoint;

typedef struct Andlink_basicInfo_s{
    int basic_size;
    char lan_mac[18];
    char wlan_mac_2g[18];
    char wlan_mac_5g[18];
    char vendor[RTL_LINK_MAX_INFO_LEN];
    char model[RTL_LINK_MAX_INFO_LEN];
    char hwversion[RTL_LINK_MAX_INFO_LEN];
    char swversion[RTL_LINK_MAX_INFO_LEN];
    char sn[RTL_LINK_MAX_INFO_LEN];
    char deviceType[RTL_LINK_MAX_INFO_LEN];
    char productToken[RTL_LINK_MAX_INFO_LEN];
    char deviceId[RTL_LINK_MAX_INFO_LEN];
    char deviceToken[RTL_LINK_MAX_INFO_LEN];
    char andlinkToken[RTL_LINK_MAX_INFO_LEN];
    char synccode[RTL_LINK_MAX_INFO_LEN];
    char andlinkVer[RTL_LINK_MAX_INFO_LEN];
    char HBPort[RTL_LINK_MAX_INFO_LEN];
    int rtl_link_mode;
    int roaming_switch;
    int roaming_qos;
    int rssi_th_2g;
    int rssi_th_5g;
    int idx_2g;
    int idx_5g;
    int retry_ratio;
    int fail_ratio;
    int detect_period;
    int rssi_delta;
    int lowquality_check_time;
    int neighbor_check_time;
    int roaming_rate_th;
    int dismiss_time;
    int HBTime;
    int need_recover;
    int connect_gw_timeout;
    int getip_timeout;
    int search_gw_max_retry;
    int search_ack;
    int search_async;
    int notify_upgrade;
    int vxd_up;
    int wisp_idx;
    int version;
    int auto_dhcp;
    int simulate_gw;
    OPMODE_T opmode;
    char staticIp[16];
//#ifdef CONFIG_ANDLINK_V3_SUPPORT   
    char gwIp_host[16];
    char gwToken_host[RTL_LINK_MAX_INFO_LEN];
    char gwIp_cloud[16];
	int port_cloud;
    //char user_key[RTL_LINK_MAX_INFO_LEN_EXTRA];
    char gwToken_cloud[RTL_LINK_MAX_INFO_LEN];
//#else  
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    int poweron;
	int default_setting_poweron;
    int udp_port;
    int tcp_port;
    char gwIp_cloud_udp[16];
    char gwIp_cloud_tcp[16];
	char gwDomain_udp[RTL_LINK_MAX_INFO_LEN_EXTRA];
    char user_key[RTL_LINK_MAX_INFO_LEN_EXTRA]; 
//#endif 
    char gwIp[16];
    char gwToken[RTL_LINK_MAX_INFO_LEN];
//#endif
    int standingby; //1:for if5; 2:for if6
    int restart_flag;
    int skip_coapping;
    int skip_online;
    int skip_inform_success;
    int skip_check_devId;
	int skip_hb_rspto;
	int skip_encrypt_reg;
	char area_code[RTL_LINK_MAX_INFO_LEN];
	
/***for andlink if5, added by elaine***/
	char gwToken_if5[RTL_LINK_MAX_INFO_LEN];
	char gwIp_if5[16];
	int port_if5;
	int fwdownload_percent;
	char mqttUser[RTL_LINK_MAX_INFO_LEN];
	char mqttClientId[RTL_LINK_MAX_INFO_LEN];
	//char mqttUrl[256];
	char mqtt_ip[RTL_LINK_MAX_INFO_LEN];
	int mqtt_port;
	char mqttPassword[RTL_LINK_MAX_INFO_LEN_EXTRA];
	int mqttKeepalive;
	char autoBootUrl[RTL_LINK_MAX_INFO_LEN_EXTRA];
	int autoBootFlag;
/********************************/	
}Andlink_basicInfo;

typedef struct ANDLINK_CURL_REQ_IF5_S
{
    char url_path[256];
	char *input;
//	int input_len;
    char *output;
//	int *output_len;
	char user_key[RTL_LINK_MAX_INFO_LEN_EXTRA+10];
}ANDLINK_CURL_REQ_IF5;

struct MemoryStruct{
	char *memory;
	size_t size;
};

typedef struct ANDLINK_CLOUD_GW_INFO_S
{
	char domain[RTL_LINK_MAX_INFO_LEN_EXTRA];
	char gwip[16];
	int  port;
}ANDLINK_CLOUD_GW_INFO;

typedef struct Andlink_neighborSta_s{
    unsigned char wlanid;
    unsigned char rssi;
    unsigned char mac[6];
    unsigned char channel;
    int bandwith;
    int duration;
    int last_appertime;
}Andlink_neighborSta;

typedef struct Andlink_lowqualitySta_s{
    unsigned char wlanid;
    unsigned char mac[6];
    unsigned char rssi;
    unsigned char channel;
    int bandwith;
    int retry_ratio;
    int fail_ratio;
    int traffic_pkts;
    int duration;
    int last_appertime;
}Andlink_lowqualitySta;

typedef struct Andlink_roamingSta_s{
    unsigned char mac[6];
    int start_time;
}Andlink_roamingSta;

typedef struct Andlink_assoc_s{
    unsigned char wlanid;
    unsigned char rssi;
    unsigned char channel;
    unsigned char channel_utilization;
    unsigned char mac[6];
}Andlink_assocSta;

typedef struct Andlink_refusedSta_s{
    unsigned char wlanid;
    unsigned char mac[6];
}Andlink_refusedSta;

extern int Andlink_startClient(Andlink_basicInfo *info);
extern void Andlink_register_endpoints(Andlink_endpoint ep);
extern void Andlink_unregister_endpoints(void);
extern int Andlink_scanQlinkDefBssid(void);
extern int Andlink_connectWireless(void);
extern void Andlink_setQlinkVXD(void);
extern void Andlink_closeQlinkVXD(void);
extern void Andlink_openQlinkVAP(void);
extern void Andlink_closeQlinkVAP(void);
extern int Andlink_formBootstrap(cJSON * root, Andlink_basicInfo *info);
extern int Andlink_formBoot(cJSON * root, Andlink_basicInfo *info);
extern int Andlink_storeLoginInfo(cJSON *root, Andlink_basicInfo *info);
extern int Andlink_formQlinkReq(cJSON * data, Andlink_basicInfo *info);
extern int Andlink_formQlinkSuccess(cJSON * data, Andlink_basicInfo *info);
extern int Andlink_formSearchGW(cJSON *root);
extern int Andlink_formSearchAP(cJSON *root);
extern int Andlink_formRoamingReq(cJSON *root, Andlink_neighborSta *sta, Andlink_basicInfo *info);
extern int Andlink_formRoamingInfo(cJSON *root, Andlink_basicInfo *info, List *lowquality_list);
extern int Andlink_formRoamingResult(cJSON *root, unsigned char *mac, Andlink_basicInfo *info, int result);
extern void Andlink_storeGWIp(struct in_addr ip, Andlink_basicInfo *info);
#ifdef CONFIG_ANDLINK_V3_SUPPORT
extern void Andlink_storeRemoteGwInfo(cJSON *root, Andlink_basicInfo *info);
#endif
extern void Andlink_set_restart_flag(int restart_flag);
extern int Andlink_switchMode(Andlink_basicInfo *info);
extern int Andlink_switchDHCP(Andlink_basicInfo *info);
extern int Andlink_switchRepeater(Andlink_basicInfo *info);
extern int Andlink_isCloudGW(void);
extern void Andlink_setCloudGW(void);
extern void Andlink_setHostGW(void);
extern void Andlink_macFormat_add_delim(char *dest, char *src);
#endif

