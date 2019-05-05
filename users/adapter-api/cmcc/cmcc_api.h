#ifndef __CMCC_API_H
#define __CMCC_API_H

#define  UPDATE_IMAGE_FILE "/var/tmp/fw.bin"
#define REBOOT_WAIT_COMMAND(time) {\
	if(fork()==0) {\
		sleep(time);\
		system("reboot");\
		}\
}

struct initinfo_req{
    unsigned char devID[32];
    unsigned char deviceType[4];
    unsigned char IP[32];
};

struct initinfo_rsp{
    unsigned char cycle[4];
    unsigned char time[32];
    unsigned char timeZone[16];
    unsigned char platformVersion[32];
    unsigned char password[128];
};

struct deviceCapacity{
    unsigned char enable5GAccess[4];
    unsigned char enable2GAccess[4];
};

struct ability_notify_req{
    unsigned char devID[32];
    unsigned char devName[128];
    unsigned char fac[32];
    unsigned char type[32];
    unsigned char softVersion[256];
    struct deviceCapacity capacitySet;
};

struct route_base_info_rsp{
    unsigned char deviceID[32];
    unsigned char wanMAC[32];
    unsigned char wanIP[32];
    unsigned char lanIP[32];
};

struct cmcc_sta_info{
    unsigned char clientDownlinkRate[32];
    unsigned char clientUplinkRate[32];
    unsigned char deviceStatus[8];
    unsigned char MAC[32];
    unsigned char IP[20];
    unsigned char name[256];
    unsigned char onlineDuration[32];
    unsigned char accessInternet[4];
    unsigned char qosSwitch[4];
    unsigned char maxUplinkRate[32];
    unsigned char maxDownlinkRate[32];
    unsigned char SSID[256];
};

struct route_overview_rsp{
    unsigned char type[32];
    unsigned char MAC[32];
    unsigned char IP[20];
    unsigned char channel[8];
    unsigned char softwareVersion[256];
    unsigned char onlineDuration[32];
    unsigned char upLineSignal[32];
    unsigned char upLinkSSID[256];
    unsigned char firewallStatus[4];
    unsigned char WifiStatus[4];
    unsigned char WifiStatus5G[4];
    unsigned char downlinkRate[32];
    unsigned char uplinkRate[32];
    struct cmcc_sta_info connectDevices[256];
    int connect_num;
    struct cmcc_sta_info connect5GDevices[256];
    int connect5G_num;
};

struct device_operate_req{
    unsigned char opType[32];
    unsigned char opPara[128];
};

struct device_status_change{
	unsigned char	deviceID[32];
	unsigned char	Status[32];
};

/*===================== access control start ============================*/
#ifndef MAX_STA_NUM
#define MAX_STA_NUM			64
#endif

typedef enum {DENY = 1, ALLOW} ACCESS_CONTROL_T;

typedef struct client_access_control_info{
	unsigned char clientMAC[32];
	unsigned char clientIP[32];
	unsigned char clientName[256];
	unsigned char accessRight[8];
}CLIENT_ACCESS_CONTROL_INFO_T, *CLIENT_ACCESS_CONTROL_INFO_Tp;


typedef struct client_access_control_info_rsp{
	CLIENT_ACCESS_CONTROL_INFO_T caInfo[MAX_STA_NUM];
}CLIENT_ACCESS_CONTROL_INFO_RSP_T, *CLIENT_ACCESS_CONTROL_INFO_RSP_Tp;


/*
typedef struct client_access_control_req{
	unsigned char clientMAC[32];
	unsigned char accessRight[8];
}CLIENT_ACCESS_CONTROL_REQ_T, *CLIENT_ACCESS_CONTROL_REQ_Tp;
*/
/*===================== access control end ============================*/


/*===================== shaping control start ============================*/

#define QOS_DISABLE						0
#define QOS_ENABLE						1

#define QOS_RATE_OUT_RANGE				-2
#define NO_RATE_LIMIT					-1
#if defined (CONFIG_RTL_8198)|| defined (CONFIG_RTL_8198C) || defined (CONFIG_RTL_8367R_SUPPORT)
#define MAX_LINK_RATE					1024000
#else
#define MAX_LINK_RATE					102400
#endif

/*
typedef struct qos_req{
	unsigned char clientMAC[32];
	unsigned char qosSwitch[4];
	unsigned char maxUplinkRate[32];
	unsigned char maxDownlinkRate[32];
}QOS_REQ_T, *QOS_REQ_Tp;
*/

/*===================== shaping control end ==============================*/


/* for wlan */
#define CUST_WIFI_STATUS_LEN 		4
#define CUST_WIFI_SSID_LEN			256
#define CUST_WIFI_CODE_LEN			1024
#define CUST_WIFI_ENCRPTY_TYPE_LEN	32
#define CUST_WIFI_FREQUENCY_LEN		1024
#define	CUST_SSID_NUM				256

#ifdef FOR_DUAL_BAND
#ifdef CONFIG_BAND_5G_ON_WLAN0
#define WLAN_5G_IFNAME		 	"wlan0"
#define WLAN_2G_IFNAME 			"wlan1"
#else
#define WLAN_5G_IFNAME 			"wlan1"
#define WLAN_2G_IFNAME 			"wlan0"
#endif
#else
#ifdef CONFIG_BAND_5G_ON_WLAN0
#define WLAN_5G_IFNAME		 	"wlan0"
#define WLAN_2G_IFNAME 			""
#else
#define WLAN_2G_IFNAME 			"wlan0"
#define WLAN_5G_IFNAME 			""
#endif
#endif

enum CUST_WIFI_STATUS {STATUS_NOT_SUPPORT, STATUS_ENABLE, STATUS_DISABLE, STATUS_REINIT=4};
enum CUST_WIFI_ENC_TYPE {TYPE_NONE=1, TYPE_WEP, TYPE_WPA_PSK, TYPE_WPA_TKIP, 
						 TYPE_WPA2_PSK, TYPE_WPA2_AES, TYPE_WPA_PSK_WPA2_PSK, TYPE_WPA_TKIP_WPA2_PSK, 
						 TYPE_WPA_PSK_WPA2_AES, TYPE_WPA_TKIP_WPA2_AES, TYPE_WAPI=16};
enum CUST_WIFI_FREQUENCY {FREQUENCY_BAND_2G, FREQUENCY_BAND_5G};
enum CUST_WIFI_ENC_TYPE_SETTING {ENC_TYPE_GET, ENC_TYPE_SET}; 

typedef struct wlan_ssid_info
{
	unsigned char SSID[CUST_WIFI_SSID_LEN];				/* ssid */
	unsigned char CODE[CUST_WIFI_CODE_LEN];				/* encrypt phrase */
	unsigned char encryptType[CUST_WIFI_ENCRPTY_TYPE_LEN];	/* 1:none; 2:wep; 3: */
	unsigned char frequency[CUST_WIFI_FREQUENCY_LEN];	
}WLAN_SSID_INFO, *WLAN_SSID_INFOp;

typedef struct cust_wlan_info {
	unsigned char WifiStatus[CUST_WIFI_STATUS_LEN];		/* 0:not support 2.4G,  1:enable, 2:disable */
	unsigned char WifiStatus_5G[CUST_WIFI_STATUS_LEN];	/* 0:not support 5G, 	1:enable, 2:disable */
	WLAN_SSID_INFO SSIDList[CUST_SSID_NUM];	
	unsigned char SSIDNum[CUST_SSID_NUM];
}CUST_WLAN_INFO, *CUST_WLAN_INFOp;

#define CMCC_DEBUG(fmt, args...)\
	do{\	
		if(debug_on)\
			printf("[%s %d]:"fmt,__FUNCTION__ , __LINE__ , ## args);\
	}while(0)
/*===================== upgrade firmware start ==============================*/
struct devUpdate_info_req{
	unsigned char devID[32];
	unsigned char type[4];
	unsigned char currentVersion[32];
};
struct devUpdate_info_rsp{
	unsigned char needUpdate[2];
	unsigned char updateUrl[256];
	unsigned char updateDescription[256];
};
struct devUpdate_req{
	unsigned char updateUrl[256];
	unsigned char updateDescription[256];
};
/*===================== upgrade firmware end ==============================*/
/*===================== get network info start ============================*/
#define MAX_NAME_LEN_LONG		129
struct boardBand_setting{
	unsigned char type[4];
	unsigned char PPPoEName[MAX_NAME_LEN_LONG];
	unsigned char PPPoECode[MAX_NAME_LEN_LONG];
	unsigned char IP[32];
	unsigned char gatewayIP[32];
	unsigned char submask[32];
	unsigned char DNS1[32];
	unsigned char DNS2[32];
};
/*===================== get network info end ===============================*/
#define SYSLOG_FILE "/var/log/messages"
struct log_upload_req{  
    unsigned char deviceID[33];
	unsigned char *log;
};

/*===================================================================*/
struct device_password
{
	unsigned char 	password[129];
};

struct device_info_rsp
{
	unsigned char 	name[257];
	unsigned char	MAC[21];
	unsigned char	deviceID[33];
};

struct online_notification_rsp
{
	unsigned char 	name[257];
	unsigned char	MAC[21];
	unsigned char	IP[21];
	unsigned char	deviceID[33];
};


extern int cmcc_api_init(void);  
extern int cmcc_api_update(void);
extern void cmcc_validate_access_settings();
extern void cmcc_validate_qos_settings();
extern void cmcc_validate_wifi_settings();
extern void cmcc_validate_wan_settings();
extern void cmcc_validate_settings();
extern int cmcc_get_wan_status(int *status);
extern int cmcc_set_wan_status(int status);
extern int cmcc_get_INITINFO_REQ(struct initinfo_req *req);
extern int cmcc_set_INITINFO_RSP(struct initinfo_rsp rsp);
extern int cmcc_get_ABLITITY_NOTIFY_REQ(struct ability_notify_req *req);
extern int cmcc_get_ABLITITY_NOTIFY_REQ_init(char* devId,char* fac,char* softVersion);
extern int cmcc_get_ROUTE_BASE_INFO_RSP(struct route_base_info_rsp *rsp);
extern int cmcc_get_ROUTE_OVERVIEW_RSP(struct route_overview_rsp *rsp); 
extern int cmcc_set_DEVICE_OPERATE_REQ(struct device_operate_req req);
extern int cmcc_get_DEVICE_WIFI_INFO_NEW_RSP(CUST_WLAN_INFOp pwlan_info);
extern int cmcc_set_DEVICE_WIFI_NEW_RSP(CUST_WLAN_INFO wlan_info);
extern int cmcc_WIFI_ACCELERATE(unsigned int accelerate);
extern int cmcc_set_QOS_REQ(QOS_REQ_Tp req);
extern int cmcc_get_CLIENT_ACCESS_CONTROL_INFO_RSP(int *deviceNum, CLIENT_ACCESS_CONTROL_INFO_RSP_Tp rsp);
extern int cmcc_set_CLIENT_ACCESS_CONTROL_REQ(CLIENT_ACCESS_CONTROL_REQ_Tp req);
/*===================== upgrade firmware start ==============================*/
extern int cmcc_get_DEVUPDATE_INFO_REQ(struct devUpdate_info_req *req);
extern int cmcc_set_DEVUPDATE_INFO_RSP(struct devUpdate_info_rsp *rsp);
extern int cmcc_set_DEVUPDATE_REQ(struct devUpdate_req *req);
extern int cmcc_get_DEVUPDATE_RSP();
extern int cmcc_get_DEVUPDATE_STATUS(int * status);
extern int cmcc_set_DEVUPDATE_STATUS(int        status);
extern void cmcc_get_DEVICE_STATUS_CHANG_REQ(struct device_status_change *req);
/*===================== upgrade firmware end ==============================*/
/*===================== get network info start ============================*/
extern int cmcc_get_BROADBAND_SETTING_REQ();
extern int cmcc_get_BROADBAND_SETTING_RSP(struct boardBand_setting* rsp);
extern int cmcc_set_BROADBAND_SETTING_REQ(struct boardBand_setting* req);
extern int cmcc_set_BROADBAND_SETTING_RSP();
/*===================== get network info end ===============================*/
extern void cmcc_get_LOG_UPLOAD_REQ(struct log_upload_req *req);
/*===================== get network info end ===============================*/
extern int cmcc_GET_DEVICE_INFO_RSP(struct device_info_rsp *rsp);
extern int cmcc_GET_DEVICE_PASSWORD_RSP(struct device_password* rsp);
extern int cmcc_WTP_ONLINE_NOTIFICATION_REQ(struct online_notification_rsp* req);

#endif
