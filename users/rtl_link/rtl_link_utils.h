extern int debug_mode;
extern int roaming_debug_on;

#define RDEBUG(x...) \
    do{ \
        if(roaming_debug_on) { \
            printf(x); \
        }\
    } while(0)


#define DEBUG(x...) \
    do{ \
        if(debug_mode >= 2) { \
            printf(x); \
        }\
    } while(0)
    
#define ERR_PRINT(x...) \
    do{ \
        if(debug_mode >= 1) { \
            printf(x); \
        }\
    } while(0)

#define IP_ADDR_T 0x02
#define NET_MASK_T 0x04
#define HW_ADDR_T 0x08

#define RTF_UP			0x0001          /* route usable                 */
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */
#define _PATH_PROCNET_ROUTE	"/proc/net/route"
#define DHCPD_CONF_FILE "/var/udhcpd.conf"
#define NULL_FILE 0
#define NULL_STR ""
typedef enum { LAN_NETWORK=0, WAN_NETWORK } DHCPC_NETWORK_TYPE_T;

#define SSID_LEN 32
#if defined(CONFIG_RTK_MESH) ||  defined(CONFIG_RTL_819X)  /*add for RTL819X since wlan driver default include mesh data*/
//by GANTOE for site survey 2008/12/26
#define MESHID_LEN 32 
#endif 
#define	MAX_BSS_DESC	64

/*** added by elaine, for andlink_if5, for common use in fwupgrade.c and rtl_link.c****/
#define DOWNLOAD_LOG "/var/download.log"
#define DOWNLOAD_LOG_REPORT "/var/downloadreport.log"

typedef struct _bss_info {
    unsigned char state;
    unsigned char channel;
    unsigned char txRate;
    unsigned char bssid[6];
    unsigned char rssi, sq;	// RSSI  and signal strength
    unsigned char ssid[SSID_LEN+1];
} bss_info;

typedef struct _OCTET_STRING {
    unsigned char *Octet;
    unsigned short Length;
} OCTET_STRING;

typedef	struct _IbssParms {
    unsigned short	atimWin;
} IbssParms;

typedef enum _BssType {
    infrastructure = 1,
    independent = 2,
} BssType;

typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[SSID_LEN];
    OCTET_STRING  bdSsId;

#if defined(CONFIG_RTK_MESH) || defined(CONFIG_RTL_819X) 
	//by GANTOE for site survey 2008/12/26
	unsigned char bdMeshIdBuf[MESHID_LEN]; 
	OCTET_STRING bdMeshId; 
#endif 
    BssType bdType;
    unsigned short bdBcnPer;			// beacon period in Time Units
    unsigned char bdDtimPer;			// DTIM period in beacon periods
    unsigned long bdTstamp[2];			// 8 Octets from ProbeRsp/Beacon
    IbssParms bdIbssParms;			// empty if infrastructure BSS
    unsigned short bdCap;				// capability information
    unsigned char ChannelNumber;			// channel number
    unsigned long bdBrates;
    unsigned long bdSupportRates;		
    unsigned char bdsa[6];			// SA address
    unsigned char rssi, sq;			// RSSI and signal strength
    unsigned char network;			// 1: 11B, 2: 11G, 4:11G
	// P2P_SUPPORT
	unsigned char	p2pdevname[33];		
	unsigned char	p2prole;	
	unsigned short	p2pwscconfig;		
	unsigned char	p2paddress[6];	
	unsigned char   stage;
	
	#if defined(CONFIG_RTL_COMAPI_WLTOOLS)
    unsigned char	    wpa_ie_len;
    unsigned char	    wpa_ie[256];
    unsigned char	    rsn_ie_len;    
    unsigned char	    rsn_ie[256];
    #endif       
} BssDscr, *pBssDscr;

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    BssDscr bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;

extern int pidfile_acquire(char *pidfile);
extern void pidfile_write_release(int pid_fd);
extern int getDefaultRoute(char *interface, struct in_addr *route);
extern int getInAddr(char *interface, int type, void *pAddr);
extern int _string_to_hex(char *string, unsigned char *key, int len);
extern int equal_mac(unsigned char mac1[6],unsigned char mac2[6]);
extern void convert_bin_to_str(unsigned char *bin, int len, char *out);
extern int getWlBssInfo_andlink(char *interface, bss_info *pInfo);
extern int getWlSiteSurveyRequest(char *interface, int *pStatus);
extern int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus );
extern void set_lan_dhcpc(char *iface);
extern pid_t find_pid_by_name( char* pidName);
extern void start_dhcpd_with_ip(char *ifname, char *ip, char *ip_start, char *ip_end);
extern int write_line_to_file(char *filename, int mode, char *line_data);

