#ifndef __ANDLINK_CORE_H
#define __ANDLINK_CORE_H

#include "andlink_interface.h"
#include "coap-client.h"
typedef enum {
ANDLINK_START,
ANDLINK_SEARCHGW,
ANDLINK_WAIT_SEARCHACK,
//ANDLINK_WAIT_APP_SEARCH_DEV,
QLINK_START,
QLINK_SCAN_DEFSSID, 
QLINK_SET_CMCCQLINK,
QLINK_CONNECT_CMCCQLINK,
QLINK_CLOSE_CMCCQLINK,
QLINK_OPEN_DEFSSID,
QLINK_SENDREQ,
QLINK_WAIT_NETINFO, 
QLINK_WAIT_APP_NETINFO,
QLINK_COMPLETE,
CONNECT_WIRELESS,
ANDLINK_LOGIN,
ANDLINK_SEARCHGW_SUCCESS,
ANDLINK_BOOTSTRAP,  //×¢²á
ANDLINK_BOOTSTRAPBC,
ANDLINK_SEARCHAP,
ANDLINK_SEARCHAP_DONE,
ANDLINK_BOOT,       //ÉÏÏß
ANDLINK_POLLING,
ANDLINK_WAIT_RESTART,
//ANDLINK_EXIT
} ANDLINK_STATE_T;

typedef enum {
ANDLINK_CLOUD_START,
ANDLINK_CLOUD_BOOT,
ANDLINK_CLOUD_REGISTER,
ANDLINK_CLOUD_POLLING,
} ANDLINK_CLOUD_STATE_T;

#define ANDLINK_BOOT_FAILED -2
#define RSP_TIMEOUT -2
#define MAX_OUTPUT_LEN 4096

#define PORT 5683

#define SEARCHACK_TIMEOUT 5
#define SEARCHAP_ACK_TIMEOUT 10
#define HB_TIMEOUT		  2
#define SEARCH_GW_WAIT_TIME 30
#define QLINK_SUCCESS_RSP_TIMEOUT 3
#define REGIST_RSP_TIMEOUT  3
#define NETLINK_USER 31
#define MAX_STR_BUFFER_SIZE 2048
#define CMCCQLINK_DURATION 4*60
#define QLINK_CONNECT_TIMEOUT 10
#define COAPPING_TIMEOUT 2

#define COAP_OPTION_SecrectKey  128
#define COAP_OPTION_UserKey     0

typedef struct ADNLINK_S 
{
    char localIp[16];
    char br0Ip[16];
    int err;
    int search_gw_retry;
	int bc_retry;
	int search_ap_bc_retry;
    //int restart_flag;
    int encrypt;
	int doing_qlink;
	int reg_respCode;
    int is_cloud;
    int switch_time;
    char *gwToken;
    char *gwIp;
    int netlinkfd;
    int new_roaming_info;
    int HBCount;
    int HBFail;
	int Cloud_HBFail;
    int noneed_update_log;
    int respCode;
    int netlink_running;
    List neighbor_list;
    List lowquality_list;
    List roaming_white_list;
    List roaming_black_list;
    ANDLINK_STATE_T stat;
    ANDLINK_STATE_T next_stat;
    Andlink_basicInfo *basic_info;
//#ifdef CONFIG_ANDLINK_V25_SUPPORT
	int send_packet_type;
    int sockfd_tcp;
    char ID[64];
    char DevRND[33];
    char ChallangeCode[17];
    char MD5Key[33];
    char MD5Key_last[33];
    char MD5Challenge[33];
    char MD5Sn[33];
    char MD5Sn_16[17];
    int plugin_port;
    char plugin_gwIp[16];
    ANDLINK_CLOUD_STATE_T cloud_stat;
    char TCP_SESSION_RUNNING;
//#endif
}ANDLINK;

//#ifdef CONFIG_ANDLINK_V25_SUPPORT
#define DEF_TCP_PORT 9005
#define DEF_UDP_PORT 9007
#define DEF_LOCAL_UDP_PORT 9007
#define DEF_HBTIME 60
#define RETRY_INTERVAL 60
#define CHECKSN_FAIL_SILENTTIME 10*60

#define CLOUD_DevRND_RECV_SUCCESS   1
#define CLOUD_REGISTER_SUCCESS      2
#define CLOUD_SUCCESS               0
#define CLOUD_FAIL                  -1
#define CLOUD_REGISTER_ILLEGAL      -2
#define CLOUD_NEED_REREGISTER       -3
#define CLOUD_RECONNECT_SERVER      -5

typedef struct ANDLINK_CLOUD_REQ_S
{
    char *input;
    int input_len;
    char **output;
    int *output_len;
    int wait_time;
}ANDLINK_CLOUD_REQ;

typedef enum {
    T_WIFIPARAMSYNC, 
    T_WIFISWITCH, 
    T_RADIOCONFIG, 
    T_WPS, 
    T_MACFILTER, 
    T_REBOOT, 
    T_LEDCONTROL, 
    T_ROAMINGCONFIG, 
    T_GET_APCONFIGINFO, 
    T_GET_UPLINKSTATUS, 
    T_GET_STAINFO, 
    T_GET_WIFISTATS, 
    T_COMMAND_UNBIND, 
    T_COMMAND_FILE,
    T_DISCONNECT,
} ANDLINK_CLOUD_TYPE_T;
typedef enum {
	ANDLINK_SEND_NULL,
	ANDLINK_SEND_BOOT,
	ANDLINK_SEND_REGISTER,
	ANDLINK_SEND_HB,
}ANDLINK_SEND_PACKET_TYPE; 
//#endif

#ifdef RTK_SMART_ROAMING
enum _ROAM_TYPE {
    WLAN_STA_INFO      = 1,
    ASSOC_NEW_STA      = 2,
    ASSOC_OVER_THE_DS  = 3,
    ASSOC_OVER_THE_AIR = 4,
#ifdef STA_ROAMING_CHECK
    NEIGHBOR_INFO      = 5,
    ROAMING_INFO       = 6,
    RTL_LINK_CONNECT_FAIL  = 7,
#endif
    STACTRL_HIDDEN_AP = 8,
};
#endif

extern void Andlink_getLocalTime(char *buf);
extern void Andlink_syncLocalTime(char *time);
extern int Andlink_server_sendQlinkInfo(void);
extern int Andlink_handle_roaming_info(cJSON *json, char *peerIp, ANDLINK *p);
extern int Andlink_handle_roaming_result(cJSON *json, ANDLINK *p);
extern ERR_NUM_T Andlink_handle_roaming_req(cJSON *json, ANDLINK *p);
extern char *Stat2str(ANDLINK_STATE_T stat);
extern char *Err2str(int err);
extern char *Result2Str(ERR_NUM_T res);

#endif

