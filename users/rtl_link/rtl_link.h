#include "apmib.h"
#include "rtl_link_list.h"



#define MAX_AP_NUM NUM_WLAN_INTERFACE*(1+NUM_VWLAN) // 1:rootap 4:vap
#define RTL_LINK_RESTART_BLOCK "/var/rtl_link_restart_block"
#define RTL_LINK_FW_DIR "/var/rtl_link_fw_dir"
#define ETH_PORT_INFO  "/var/eth_port_info"
#define LAN_DEV_LINK_INFO	"/var/lan_dev_link_info"
#define UPGRADE_STATUS_PATH_NAME 	"/var/upgrade_status"    // for download fw
#define FWCHECK_PATH_NAME 	"/var/fwcheck" 					 // for upgrade fw
#define	RTL_LINK_DEL_ROUTE	"/val/rtl_link_del_route"

#ifdef WIFI_SIMPLE_CONFIG
#define _WSC_DAEMON_PROG 	"wscd"
#endif

#define RTL_LINK_FAILED 		-1
#define RTL_LINK_SUCCESS 	0

#ifndef __RTL_API_H__
typedef enum {BAND2G=1, BAND5G, DUALBAND} WIFI_BAND_T;
#endif

typedef enum {ETHERNET, WIRELESS, PLC, CABLE} UPLINK_TYPE_T;
//typedef enum {DHCP, STATIC} IP_TYPE_T;
typedef enum {RTL_LINK_BRIDGE, RTL_LINK_REPEATER, RTL_LINK_GW, RTL_LINK_WISP} RTL_LINK_MODE_T;
enum recover_bit {OPMODE_BIT, DHCP_BIT, REPEATER1_BIT, REPEATER2_BIT, MODE1_BIT, MODE2_BIT, REPEATER_MODE_BIT};

enum led_type {
	LEDTYPE_HW_TX_RX,
	LEDTYPE_HW_LINKACT_INFRA,
	LEDTYPE_SW_LINK_TXRX,
	LEDTYPE_SW_LED2_GPIO10_LINKTXRX_92D = 52,		// 52 for 92D, LED Control = 92C LED_TYPE 13
	LEDTYPE_SW_MAX,
};

#define ANDLINK_IF5_EXIT   			"/var/tmp/andlink_if5_exit"
#define ANDLINK_RECEIVE_NETINFO   	"/var/tmp/andlink_receive_netinfo"

//#define RTL_LINK_PID_FILE 		"/var/run/rtl_link.pid"
//#define RTL_LINK_IF5_PID_FILE 	"/var/run/rtl_link_if5.pid"
//#define ANDLINK_IF6_STATUS   		"/var/tmp/andlink_if6_status"
#define RTL_LINK_CHK_PING_FILE		"/var/tmp/tmp_ping"
#define RTL_LINK_SWITCH_PPPOE		"/var/tmp/switch_pppoe"

#define TYPE_IF6_NEED_START 1
#define TYPE_IF6_STARTED	2
#define TYPE_IF3_SEARCH_GW	4
#define TYPE_IF3_STARTED	8

#define SECONDS_OF_A_DAY    86400

typedef enum {FW_TYPE_INVALID=1, FW_UPGRADE_FAIL} FW_CHECK_T;

#define APPLY_IMMEDIATE
#ifdef APPLY_IMMEDIATE
typedef enum 
{
	MIB_BYTE_T=1,
	MIB_INT_T,
	MIB_SSID_STRING_T,
	MIB_BYTE_ARRAY_T,
	MIB_ACL_T,	
	MIB_STRING_T
} MIB_TYPE_T;
#endif

#define RTL8192CD_IOCTL_DEL_STA	0x89f7
//#ifdef RTK_SMART_ROAMING
#define RTL8192CD_IOCTL_SILENT_STA      0x8BF7
#define RTL8192CD_IOCTL_UNSILENT_STA      0x8BFB
//#endif

#ifdef CONFIG_RTL_NFJROM_MP
    #define NUM_STAT		8
#elif defined(__ECOS)
    #define NUM_STAT        16
#elif defined(__OSK__)
    #define NUM_STAT		16
#elif defined(MULTI_MAC_CLONE) && defined(CONFIG_WLAN_HAL_8192EE)
    #define NUM_STAT        63
#elif defined(CONFIG_RTL_88E_SUPPORT)
    #define NUM_STAT		(RTL8188E_NUM_STAT - 1)
#elif defined(__ECOS)
    #define NUM_STAT		(32 - 1)    	
#elif defined(CONFIG_RTL_SHRINK_MEMORY_SIZE)
	#define NUM_STAT		16	// for reduce ram size 
#else
    #define NUM_STAT		64 //127
#endif

#ifndef MAX_SSID_LEN
#define MAX_SSID_LEN 33
#endif

#define BR_IF       "br0"
#define WAN_IF      "eth1" 
#define WLAN_IF     "wlan%d"
#define VXD_IF      "wlan%d-vxd"
#define PPPOE_IF	"ppp0"

typedef struct LoginInfo_s{
#ifdef CONFIG_ELINK_SUPPORT
    char vendor[RTL_LINK_MAX_INFO_LEN];
    char model[RTL_LINK_MAX_INFO_LEN];
    char hwversion[RTL_LINK_MAX_INFO_LEN];
    char swversion[RTL_LINK_MAX_INFO_LEN];
    char url[RTL_LINK_MAX_INFO_LEN];
    int wireless;
#endif
#ifdef CONFIG_ANDLINK_SUPPORT
    char localIp[16];
    UPLINK_TYPE_T uptype;
    WIFI_BAND_T band;
#endif
}LoginInfo;

typedef struct APConfig_S {
    PHYBAND_TYPE_T band;
    int channel;
    int idx;
    int enable;
    char ssid[MAX_SSID_LEN];
    ENCRYPT_T encrypt;
    WPA_CIPHER_T wpa_cypher;
    WPA_AUTH_T wpa_auth;
    KEY_TYPE_T wpa_key_fmt;
    WEP_T wep_type;
    char key[128];
#ifdef CONFIG_ELINK_SUPPORT
    int txpower;
	int disable_cfg; //上海电信测试: 一键组网可能被关闭，置1时，不再同步ssid/key
#endif
#ifdef CONFIG_ANDLINK_SUPPORT
    int max_sta_num;
    int broadcast_ssid;
    int root_disabled;
#endif
}APConfig;

typedef struct WiFiConfig_s{
    char synccode[RTL_LINK_MAX_INFO_LEN];
    int num;
    APConfig ap[MAX_AP_NUM];
}WiFiConfig;

typedef struct WiFiSwitch_s{
    WIFI_BAND_T band;
    int enable;
}WiFiSwitch;

typedef struct RadioConfig_s{
    WIFI_BAND_T band;
    int power;
}RadioConfig;

typedef struct WPSSwitch_s{
    WIFI_BAND_T band;
    int enable;
}WPSSwitch;

typedef struct MacNode_s{
    char mac[13];
    char name[RTL_LINK_MAX_INFO_LEN];
	int is_lan_sta;
}MacNode;

typedef struct MacFilter_s{
    int enable;
    int policy;
	int v25;
	int contain_entries;
    List mac_list;
	int lan_sta_num;
	int wlan_sta_num;
}MacFilter;

typedef struct Reboot_s{
    WIFI_BAND_T band;
}Reboot;

typedef struct LEDStatus_s{
    int on;
}LEDStatus;

typedef struct DeviceLimit_s{
	List limit_list;
}DeviceLimit;

typedef struct NodeLimit_s{
	char mac[18];
	char limit_rxrate[10];
    char limit_txrate[10];	
	int is_lan_sta;
}NodeLimit;

typedef struct TimerTask_s{
	char operation[10];
	char taskid[10];
	long int timeoffset;
	int week;
	int enable;
	char action[20];
	long int timeoffset2;
	int index;
}TimerTask;

typedef struct FWUpgrade_s{
    char url[1024];
    int reboot;
}FWUpgrade;

typedef struct Schedule_s{
    int weekday;
    int from_time;
    int to_time;
}Schedule;

typedef struct WiFiSchedule_s{
    Schedule item[MAX_SCHEDULE_NUM];
    int num;
}WiFiSchedule;

typedef struct APStatus_s{
    WIFI_BAND_T band;
    int cpurate;
    int memoryuserate;
    int uploadspeed_2G;
    int uploadspeed_5G;
    int downloadspeed_2G;
    int downloadspeed_5G;
    unsigned long uploadstat_2G;
    unsigned long uploadstat_5G;
    unsigned long downloadstat_2G;
    unsigned long downloadstat_5G;
    unsigned long onlinetime;
    int wired_terminalNum;
    int terminalNum_2G;
    int terminalNum_5G;
    int channel_2G;
    int channel_5G;
    int channelload_2G;
    int channelload_5G;
}APStatus;

typedef struct RoamingConfig_s{
    int enable;
    int rssi_2g;
    int rssi_5g;
    int qos;
    int fail_ratio;
    int retry_ratio;
    int detect_period;
    int rssi_delta;
    int dismiss_time;
}RoamingConfig;

typedef struct Rf_s{
    PHYBAND_TYPE_T band;
    int enable;
    int power;
    int channel;
}Rf;

typedef struct RfStatus_s{
    int num;
    Rf rf[NUM_WLAN_INTERFACE];
}RfStatus;

typedef struct SysMemInfo_s{
	unsigned int flashSize;		//MByte
	unsigned int ramSize;		//MByte
	unsigned int ramUtility;	//percentage (eg:60)
	unsigned int cpuUtility;	//percentage (eg:60)
}SysMemInfo;

typedef struct PppoeInfo_s{
	char username[MAX_NAME_LEN_LONG];
	char password[MAX_NAME_LEN_LONG];
}PppoeInfo;

typedef struct UplinkStatus_s{
    UPLINK_TYPE_T type;
    unsigned char mac[18];
    PHYBAND_TYPE_T band;
    int wan_port_num;
    char ssid[MAX_SSID_LEN];
    int channel;
    char noise[5];
    char snr[5];
    char rssi[10];
    char rxrate[10];
    char txrate[10];
    unsigned int cur_rxrate;
    unsigned int cur_txrate;
    char ip[16];
}UplinkStatus;

typedef struct STAInfo_s{
    char mac[18];
    char clone_mac[18];
    char uptime[10];
    char port[10];
    PHYBAND_TYPE_T band;
    char ssid[MAX_SSID_LEN];
    char rssi[10];
	char ip_addr[16];
    char rxrate[10];
    char txrate[10];
    unsigned int cur_rxrate;
    unsigned int cur_txrate;
    UPLINK_TYPE_T uptype;
	char hostname[64];
}STAInfo;

typedef struct APStats_s{
    PHYBAND_TYPE_T band;
    int idx;
    char ssid[MAX_SSID_LEN];
    long long send_bytes;
    long long recv_bytes;
    long long send_packets;
    long long recv_packets;
    int err_send;
    int err_recv;
    int discard_send;
    int discard_recv;
}APTrafficStats;

typedef struct WiFiTrafficStats_s{
    int num;
    APTrafficStats ap[MAX_AP_NUM];
}WiFiTrafficStats;

typedef struct LanTrafficStats_s{
    int num;
    APTrafficStats ap[MAX_AP_NUM];
}LanTrafficStats;

#define MAX_TERM_NUMBER 64
#define TERMINAL_RATE_INFO "/var/terminal_rate_info"

#ifndef __RTL_API_H__
struct rtk_terminal_rate_info {
	unsigned char  mac[6];		//terminal mac
	unsigned int download_speed;//terminal download speed(bytes/s)
	unsigned int upload_speed;	//terminal upload speed(bytes/s)
	unsigned int rx_bytes;
	unsigned int tx_bytes;  
	unsigned int all_bytes;		//all traffic(bytes)	
	int port_number;			//terminal phy port number
	unsigned int link_flag;
};
#endif

extern int rtl_link_getGatewayIp(char *gwip);
extern int rtl_link_getLanMacAddr(char *lan_mac);
extern int rtl_link_getWlanMacAddr(char *wlan_mac, char *ifname);
extern void rtl_link_setRecoverMib(int mib, int value, int bit);
extern void rtl_link_recoverSettings(void);
extern void rtl_link_restart(int ret);
extern int rtl_link_switchOpMode(RTL_LINK_MODE_T work_mode, char *addr_s, int wisp_idx);
extern int rtl_link_getLoginInfo(LoginInfo *login);
extern int rtl_link_syncRepeaterConfig(WiFiConfig wifi);
extern int rtl_link_syncSearchAPWiFiConfig(WiFiConfig wifi);
extern int rtl_link_syncWiFiConfig(WiFiConfig wifi);
extern int rtl_link_syncWiFiSwitch(WiFiSwitch wifiswitch);
extern int rtl_link_syncRadioConfig(RadioConfig radio);
extern int rtl_link_syncWPSSwitch(WPSSwitch wps);
extern int rtl_link_syncMacFilterSetting(MacFilter macfilter);
extern int rtl_link_syncReboot(Reboot reboot);
extern int rtl_link_syncLEDOnOff(LEDStatus led);
extern int rtl_link_syncWiFiSchedule(WiFiSchedule schedule);
extern int rtl_link_upgradeFirmware(FWUpgrade fw);
extern int rtl_link_syncRoamingConfig(RoamingConfig roaming);
extern int rtl_link_getRFInfo(RfStatus *RF);
extern void rtl_link_getWiFiConfig(WiFiConfig *wifi);
extern void rtl_link_getLEDStatus(LEDStatus *led);
extern void rtl_link_getRoamingStatus(RoamingConfig *roaming);
extern void rtl_link_getWiFiSwitchStatus(WiFiSwitch *wifi);
extern void rtl_link_getWiFiTimerSchedule(WiFiSchedule *schedule);
extern int rtl_link_getUplinkType(int *idx);
extern int rtl_link_getUplinkStatus(UplinkStatus *uplink);
extern int rtl_link_getWlanSTAInfo(List *sta_list);
extern int rtl_link_getLanDevInfo(List *sta_list);
extern int rtl_link_getWiFiTrafficStats(WiFiTrafficStats *wifi);
extern void rtl_link_getMacFilterSetting(MacFilter *macfilter);
extern void rtl_link_getRoamingConfig(RoamingConfig *roaming);
extern void rtl_link_getAPStatus(APStatus *status);
extern int rtl_link_get_terminal_rate_by_mac(char *mac_s, int *download_rate, int *upload_rate);
extern int rtl_link_get_terminal_rate_by_port(char *port, int *download_rate, int *upload_rate);
extern int rtl_link_delete_sta(char *if_name, unsigned char *mac_s);
extern int rtl_link_silent_sta(unsigned char *mac_s);
extern int rtl_link_get_port_speed(int port_num, unsigned int *up_speed, unsigned int *down_speed);
extern void rtl_link_getWorkMode(int *workmode);
extern int rtl_link_getLanTrafficStats(LanTrafficStats *lan);