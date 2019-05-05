#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <linux/wireless.h>
#include "../boa/apmib/apmib.h"

static char *tmp_file = "/tmp/tmp.txt";
#if defined(TRAFFIC_METER_SUPPORT)	
#define _PATH_PROCNET_DEV "/proc/net/dev"
#endif

//#define CBN_SPEC

#define _DHCPD_PID_PATH "/var/run"
#define _DHCPD_PROG_NAME "udhcpd"
#define _CONFIG_SCRIPT_PROG "init.sh"
#define WAN_DHCP_CONNECTED "/var/dhcp_connect"

#ifdef TIMER_TASK_SUPPORT
#define MAX_TASK_ACTION_LEN  		20
#define TIMER_TASK_STR_REBOOT 		"ToReboot"
#define TIMER_TASK_STR_HEALTHMODE   "ToSetHealthMode"
#define WRITE_FLASH_PROG			("flash")
#define PARAM_TEMP_FILE				("/tmp/task_timer_file")
#define MAX_TASK_SCHEDULE_NUM 		(10*7) 
#endif

#define SLEEP_TIME	3

#ifdef CONFIG_RTL8186_KB
#include <time.h>
#if 0
#define DEBUG_PRINT printf

#else
#define DEBUG_PRINT
#endif

typedef struct ps_info {
	unsigned int resv:2;
	unsigned int flag1:1;
	unsigned int flag2:1;
	unsigned int flag3:1;
	unsigned int flag4:1;
	unsigned int fTime:9;
	unsigned int tTime:9;
	unsigned int date:8;
}ps_info_T;

static int pre_enable_state = -1;

#else /*Main Trunk*/ 


#if 0
#define DEBUG_PRINT printf

#else
#define DEBUG_PRINT
#endif

#include <time.h>
typedef struct ps_info {
	unsigned int fTime;
	unsigned int tTime;
	unsigned int date;
}ps_info_T;

#if defined(TIMER_TASK_SUPPORT)
typedef struct task_info {
	unsigned int fTime;	//unit:second
	unsigned int taskid;
	unsigned int date;  //0:Sun, 1:Mon, 2:Tue, ..., 6:Sat, 7:everyday
	unsigned char action[MAX_TASK_ACTION_LEN];
	unsigned int tTime;
	unsigned int index;
	unsigned char loop;	
}task_info_T, *task_info_Tp;
#endif

#if defined(TRAFFIC_METER_SUPPORT)	
typedef struct traffic{
	int pos;
	int minute;
	unsigned long long total_traffic;
	unsigned long long stats[24];
}traffic_info_T;
struct user_net_device_stats {
	unsigned long long rx_packets;	/* total packets received       */
	unsigned long long tx_packets;	/* total packets transmitted    */
	unsigned long long rx_bytes;	/* total bytes received         */
	unsigned long long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long rx_multicast;	/* multicast packets received   */
	unsigned long tx_multicast;	/* multicast packets transmitted   */
	unsigned long rx_unicast;	/* unicast packets received   */
	unsigned long tx_unicast;	/* unicast packets transmitted   */
	unsigned long rx_broadcast;	/* broadcast packets received   */
	unsigned long tx_broadcast;	/* broadcast packets transmitted   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;
	unsigned long rx_length_errors;
	unsigned long rx_over_errors;	/* receiver ring buff overflow  */
	unsigned long rx_crc_errors;	/* recved pkt with crc error    */
	unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
	unsigned long rx_missed_errors;	/* receiver missed packet     */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
};
unsigned char g_wan_if[16];
#if defined(CONFIG_APP_ADAPTER_API)||defined(CONFIG_ANDLINK_SUPPORT)||defined(CONFIG_ELINK_SUPPORT)
time_t tm_start;
typedef struct load_statics{
	unsigned long long rx_speed_max;
	unsigned long long rx_speed_min;
	unsigned long long rx_speed_avg;
	unsigned long long rx_speed_cur;
	unsigned long long rx_bytes;
	unsigned long long tx_speed_max;
	unsigned long long tx_speed_min;
	unsigned long long tx_speed_avg;
	unsigned long long tx_speed_cur;
	unsigned long long tx_bytes;
	time_t tm;
}load_statics_T;

typedef struct rxtx_rate_stats{
	unsigned int rx_speed;
	unsigned int  rx_bytes;
	unsigned int  tx_speed;
	unsigned int  tx_bytes;
	time_t tm;
	unsigned char valid;
	unsigned char linkFlag;  /*0: ethernet , 1: wlan*/
	unsigned char port; /*for ethernet*/
	unsigned char macAddr[6]; /*for wlan*/
	
}rxtx_rate_stats_T;

typedef struct rtk_wlan_sta_info {
 unsigned short aid;
 unsigned char  addr[6];
 unsigned long  tx_packets;
 unsigned long  rx_packets;
 unsigned long  expired_time; // 10 msec unit
 unsigned short flag;
 unsigned char  txOperaRates;
 unsigned char  rssi;
 unsigned long  link_time;  // 1 sec unit
 unsigned long  tx_fail;
 unsigned long  tx_bytes;
 unsigned long  rx_bytes;
 unsigned char  network;
 unsigned char  ht_info; // bit0: 0=20M mode, 1=40M mode; bit1: 0=longGI, 1=shortGI
 unsigned char  RxOperaRate;
 unsigned char  resv[5];
} RTK_WLAN_STA_INFO_T, *RTK_WLAN_STA_INFO_Tp;

#define MAX_TERM_NUMBER 64
#define TERMINAL_RATE_INFO "/var/terminal_rate_info"
#define MAX_STA_NUM			64	// max support sta number

rxtx_rate_stats_T rate_stats_tbl[MAX_TERM_NUMBER];



#endif
#endif

enum _WLAN_INTERFACE_ {
	WLAN_IF_ROOT,
	WLAN_IF_VXD,
	WLAN_IF_VA0,
	WLAN_IF_VA1,
	WLAN_IF_VA2,
	WLAN_IF_VA3,
	WLAN_IF_TOTAL
};

enum _ETH_INTERFACE_ {
	ETH_IF_0,
	ETH_IF_2,
	ETH_IF_3,
	ETH_IF_4,	
	ETH_IF_TOTAL
};

#define MAX_SCHEDULE_NUM 10 /* everyday, Mon~Sun */
#define MAX_WLAN_INT_NUM 2 /*we assume Max 2 WLAN interface */
static int pre_enable_state = -1;
static int pre_enable_state_new[MAX_WLAN_INT_NUM] = {-1,-1};

#endif // CONFIG_RTL8186_KB

#if 0
static int test_gpio(void)
{	
	FILE *fp;
	int i;
	fp=fopen("/proc/rf_switch","r");
	fscanf(fp,"%d",&i);
	fclose(fp); 
	return i;	
}
#endif

#if defined(CONFIG_APP_ADAPTER_API)||defined(CONFIG_ANDLINK_SUPPORT)||defined(CONFIG_ELINK_SUPPORT)
static inline int iw_get_ext(int skfd, char *ifname, int request, struct iwreq *pwrq)
{
	/* Set device name */
	strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
	/* Do the request */
	return(ioctl(skfd, request, pwrq));
}

static int equal_mac(unsigned char mac1[6],unsigned char mac2[6])
{
	if((mac1[0]==mac2[0])&& (mac1[1]==mac2[1])&&(mac1[2]==mac2[2])&&
	   (mac1[3]==mac2[3])&& (mac1[4]==mac2[4])&&(mac1[5]==mac2[5]))
	   return 1;
	return 0;
}

static int getWlStaNum( char *interface, int *num )
{
#ifndef NO_ACTION
	int skfd=0;
	unsigned short staNum;
	struct iwreq wrq;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
	/* Get wireless name */
	if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
	{
	/* If no wireless name : no wireless extensions */
		close( skfd );
		return -1;
	}
	wrq.u.data.pointer = (caddr_t)&staNum;
	wrq.u.data.length = sizeof(staNum);

	if (iw_get_ext(skfd, interface, SIOCGIWRTLSTANUM, &wrq) < 0)
	{
		close( skfd );
		return -1;
	}
	*num  = (int)staNum;

	close( skfd );
#else
	*num = 0 ;
#endif

	return 0;
}

static int rtk_get_wlan_sta( unsigned char *interface,  RTK_WLAN_STA_INFO_Tp pInfo)
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
    }
    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(RTK_WLAN_STA_INFO_T) * (MAX_STA_NUM+1);
    memset(pInfo, 0, sizeof(RTK_WLAN_STA_INFO_T) * (MAX_STA_NUM+1));

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSTAINFO, &wrq) < 0){
        close( skfd );
        return -1;
    }
    close( skfd );
#else
    return -1;
#endif
    return 1;

}
#endif

static int get_flash_int_value(char *keyword, int *pVal)
{
	char tmpbuf[100], *ptr;
	FILE *fp;
	
	sprintf(tmpbuf, "flash get %s > %s", keyword, tmp_file);
	system(tmpbuf);
		
	fp = fopen(tmp_file, "r");
	if (fp == NULL) {
		printf("read tmp file [%s] failed!\n", tmp_file);
		return 0;
	}
	fgets(tmpbuf, 100, fp);
	fclose(fp);	
	
	ptr = strchr(tmpbuf, '=');
	if (ptr == NULL) {
		printf("get %s failed!!!!!!\n",keyword);
		return 0;			
	}else
	
	*pVal = atoi(ptr+1);
	return 1;	
}

static is_wlan_exist(int index)
{
	char tmpbuf[100], *ptr;
	char wlanIntname[10];
	FILE *fp;
	
	sprintf(tmpbuf, "ifconfig > %s", tmp_file);
	system(tmpbuf);
	
	fp = fopen(tmp_file, "r");
	if (fp == NULL)
		return 0;
	sprintf(wlanIntname,"wlan%d",index);
	while (fgets(tmpbuf, 100, fp)) {
		ptr = strstr(tmpbuf, wlanIntname);	
		if (ptr) {
			if (strlen(ptr) <= 5)
				break;
			if (*(ptr+5) != '-')
				break;
		}
	}
	fclose(fp);
	
	return (ptr ? 1 : 0);
}

static void enable_wlan(int index) 
{
	int wlan_disabled;
	int wlan_wds_enabled=0;
	int wlan_wds_num=0;
	int i;
	char cmdBuffer[128];
#ifdef CONFIG_RTL8186_KB
	int guest_disabled;
#else
	int repeader_enabled;
	int guest0_disabled;
	int guest1_disabled;
	int guest2_disabled;
	int guest3_disabled;
#endif

//	DEBUG_PRINT("Enable wlan!\n");
	sprintf(cmdBuffer,"wlan%d WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &wlan_disabled);
	
	sprintf(cmdBuffer,"wlan%d WDS_ENABLED",index);
	get_flash_int_value(cmdBuffer, &wlan_wds_enabled);

	sprintf(cmdBuffer,"wlan%d WDS_NUM",index);
	get_flash_int_value(cmdBuffer, &wlan_wds_num);
	
#ifdef CONFIG_RTL8186_KB
	sprintf(cmdBuffer,"wlan%d-va0 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest_disabled);
#else
	sprintf(cmdBuffer,"wlan%d-va0 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest0_disabled);

	sprintf(cmdBuffer,"wlan%d-va1 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest1_disabled);
	
	sprintf(cmdBuffer,"wlan%d-va2 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest2_disabled);
	
	sprintf(cmdBuffer,"wlan%d-va3 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest3_disabled);
	
	sprintf(cmdBuffer,"REPEATER_ENABLED%d",index+1);
	get_flash_int_value(cmdBuffer, &repeader_enabled);
#endif

	if (!wlan_disabled){
		sprintf(cmdBuffer,"ifconfig wlan%d up",index);
		system(cmdBuffer);
		if(wlan_wds_enabled==1){
				for(i=0;i<wlan_wds_num;i++){
					sprintf(cmdBuffer, "ifconfig wlan%d-wds%d up",index, i);
					system(cmdBuffer);
				}
		}
	}
#ifdef CONFIG_RTL8186_KB
	if (!guest_disabled) 
	{
		sprintf(cmdBuffer,"ifconfig wlan%d-va0 up",index);
		system(cmdBuffer);
	}
#else	

	if (!guest0_disabled) 
	{
		sprintf(cmdBuffer,"ifconfig wlan%d-va0 up",index);
		system(cmdBuffer); 
	}
	if (!guest1_disabled) 
	{
		sprintf(cmdBuffer,"ifconfig wlan%d-va1 up",index);
		system(cmdBuffer); 
	}
	if (!guest2_disabled) 
	{
		sprintf(cmdBuffer,"ifconfig wlan%d-va2 up",index);
		system(cmdBuffer); 
	}
	if (!guest3_disabled) 
	{
		sprintf(cmdBuffer,"ifconfig wlan%d-va3 up",index);
		system(cmdBuffer); 
	}
	if (repeader_enabled) 
	{
		sprintf(cmdBuffer,"ifconfig wlan%d-vxd up",index);
		system(cmdBuffer); 
	}
#endif	
}

static void disable_wlan(int index)
{
	int wlan_disabled;
	int wlan_wds_enabled=0;
	int wlan_wds_num=0;
	int i;
	char cmdBuffer[128];
#ifdef CONFIG_RTL8186_KB
	int guest_disabled;
#else
	int guest0_disabled;
	int guest1_disabled;
	int guest2_disabled;
	int guest3_disabled;
	int repeader_enabled;
#endif

//	DEBUG_PRINT("Disable wlan!\n");
	sprintf(cmdBuffer,"wlan%d WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &wlan_disabled);
	
	sprintf(cmdBuffer,"wlan%d WDS_ENABLED",index);
	get_flash_int_value(cmdBuffer, &wlan_wds_enabled);
	
	sprintf(cmdBuffer,"wlan%d WDS_NUM",index);
	get_flash_int_value(cmdBuffer, &wlan_wds_num);
	
#ifdef CONFIG_RTL8186_KB
	sprintf(cmdBuffer,"wlan%d-va0 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest_disabled);
#else
	sprintf(cmdBuffer,"wlan%d-va0 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest0_disabled);

	sprintf(cmdBuffer,"wlan%d-va1 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest1_disabled);

	sprintf(cmdBuffer,"wlan%d-va2 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest2_disabled);

	sprintf(cmdBuffer,"wlan%d-va3 WLAN_DISABLED",index);
	get_flash_int_value(cmdBuffer, &guest3_disabled);

	sprintf(cmdBuffer,"REPEATER_ENABLED%d",index+1);
	get_flash_int_value(cmdBuffer, &repeader_enabled);
#endif

	if (!wlan_disabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d set_mib keep_rsnie=1",index);
		system(cmdBuffer);

		sprintf(cmdBuffer,"ifconfig wlan%d down",index);
		system(cmdBuffer);

		if(wlan_wds_enabled==1){
				for(i=0;i<wlan_wds_num;i++){
					sprintf(cmdBuffer, "ifconfig wlan%d-wds%d down",index, i);
					system(cmdBuffer);
				}
		}
	}

#ifdef CONFIG_RTL8186_KB
	if (!guest_disabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d-va0 set_mib keep_rsnie=1",index);
		system(cmdBuffer);

		sprintf(cmdBuffer,"ifconfig wlan%d-va0 down",index);
		system(cmdBuffer);
	}
#else
	if (!guest0_disabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d-va0 set_mib keep_rsnie=1",index);		
		system(cmdBuffer);
		
		sprintf(cmdBuffer,"ifconfig wlan%d-va0 down",index);
		system(cmdBuffer);
	}

	if (!guest1_disabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d-va1 set_mib keep_rsnie=1",index);
		system(cmdBuffer);

		sprintf(cmdBuffer,"ifconfig wlan%d-va1 down",index);
		system(cmdBuffer);
	}

	if (!guest2_disabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d-va2 set_mib keep_rsnie=1",index);		
		system(cmdBuffer);

		sprintf(cmdBuffer,"ifconfig wlan%d-va2 down",index);
		system(cmdBuffer);
	}

	if (!guest3_disabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d-va3 set_mib keep_rsnie=1",index);
		system(cmdBuffer);

		sprintf(cmdBuffer,"ifconfig wlan%d-va3 down",index);
		system(cmdBuffer);
	}

	if (repeader_enabled) {
		sprintf(cmdBuffer,"iwpriv wlan%d-vxd set_mib keep_rsnie=1",index);
		system(cmdBuffer);

		sprintf(cmdBuffer,"ifconfig wlan%d-vxd down",index);
		system(cmdBuffer);
	}
#endif	
}

#if defined(CONFIG_RTL_8196B) || defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198) || defined(CONFIG_RTL_819XD) || defined(CONFIG_RTL_8196E)|| defined(CONFIG_RTL_8198B) || defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8197F)
static int test_gpio(void)
{
	FILE *fp;
	int i;
	fp=fopen("/proc/rf_switch","r");
	if (fp==NULL) return -1;
	fscanf(fp,"%d",&i);
	fclose(fp);
	return i;
}

// Read RF h/w switch (GPIO6) to see if need to disable/enable wlan interface
static int poll_rf_switch(void)
{
	static int rf_enabled=2;
	static int is_wlan_enabled=-1;
	int temp, wlan_disabled;
	temp = test_gpio();

	if  (temp==-1) return -1;
	if (rf_enabled == 2) { // first time
		rf_enabled = temp;
		if(rf_enabled ==1)
			return 1;
		if(rf_enabled ==0)
			return 2;  
	}

	if (temp != rf_enabled) {
		rf_enabled = temp;

		if (!get_flash_int_value("WLAN_DISABLED", &wlan_disabled))
			return -1;

		if (rf_enabled && !wlan_disabled){
				//	enable_wlan();
				return 1; //WLAN is enabled, but we should check the time match in advance
		}else{
			pre_enable_state = 0;
			disable_wlan(WLAN_IF_ROOT);
			return 2; //WLAN disabled 
		}
	}
#if 0 ///GPIO state no change, we donot modify wlan0 interface state, let schedule to modify the state
	else {
		if (is_wlan_enabled < 0) { // first time
			is_wlan_enabled = is_wlan0_exist();
			get_flash_int_value("WLAN_DISABLED", &wlan_disabled);
			printf("is wlan_enabled==%d\n", is_wlan_enabled);
			if (!wlan_disabled && is_wlan_enabled && temp == 0)
				disable_wlan();
			if(is_wlan_enabled==0)	
			return 0;	//first time check gpio status, but we check wlan by scheduld 
			if(is_wlan_enabled ==1)
				return 1;
		}
		//gpio status is not changed
		if(temp ==1)
			return 1;
		else if(temp ==0)
			return 2;
	}
#endif

	//gpio status is not changed
		if(temp ==1)
			return 1;
		else if(temp ==0)
			return 2;
		else
			return 0; //for compile warning only
}
#endif

#define ECO_SUNDAY_MASK		0x00000001
#define ECO_MONDAY_MASK		0x00000002
#define ECO_TUESDAY_MASK	0x00000004
#define ECO_WEDNESDAY_MASK	0x00000008
#define ECO_THURSDAY_MASK	0x00000010
#define ECO_FRIDAY_MASK		0x00000020
#define ECO_SATURDAY_MASK	0x00000040

#ifdef CONFIG_RTL8186_KB
static void dump_ps(struct ps_info *ps)
{
	char tmpbuf[200];


	DEBUG_PRINT("\nps_info=0x%lx\n", *((unsigned long *)ps));
		
	sprintf(tmpbuf, "date (%x): ", ps->date);
	if (ps->date & ECO_SUNDAY_MASK)
		strcat(tmpbuf, "Sunday ");	
	if (ps->date & ECO_MONDAY_MASK)
		strcat(tmpbuf, "Monday ");
	if (ps->date & ECO_TUESDAY_MASK)
		strcat(tmpbuf, "Tuesday ");
	if (ps->date & ECO_WEDNESDAY_MASK)
		strcat(tmpbuf, "Wednesday ");	
	if (ps->date & ECO_THURSDAY_MASK)
		strcat(tmpbuf, "Thursday ");		
	if (ps->date & ECO_FRIDAY_MASK)
		strcat(tmpbuf, "Friday ");
	if (ps->date & ECO_SATURDAY_MASK)
		strcat(tmpbuf, "Saturday ");
	strcat(tmpbuf, "\n");
	DEBUG_PRINT(tmpbuf);

	DEBUG_PRINT("From (%x): %dh %dm\n", ps->fTime, (ps->fTime >> 4)&0x1f, (ps->fTime & 0xf)*5);
	DEBUG_PRINT("To (%x): %dh %dm\n",  ps->tTime, (ps->tTime >> 4)&0x1f, (ps->tTime & 0xf)*5);
	DEBUG_PRINT("flag1: %x\n", ps->flag1);
	DEBUG_PRINT("flag2: %x\n", ps->flag2);
	DEBUG_PRINT("flag3: %x\n", ps->flag3);
	DEBUG_PRINT("flag4: %x\n\n", ps->flag4);
}


static void check_time_and_control_RF(struct ps_info *ps)
{
	int hit_date=0, hit_time=0;
	unsigned long start, end, current;		
	time_t tm;	
	struct tm tm_time;	

	time(&tm);	
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));

	DEBUG_PRINT(" tm_wday=%d, tm_hour=%d, tm_min=%d, tm_sec=%d\n", tm_time.tm_wday, 
					tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	
	switch(tm_time.tm_wday) {
		case 0:
			if (ps->date & ECO_SUNDAY_MASK)
				hit_date = 1;
			break;
		case 1:
			if (ps->date & ECO_MONDAY_MASK)
				hit_date = 1;
			break;
		case 2:
			if (ps->date & ECO_TUESDAY_MASK)
				hit_date = 1;
			break;
		case 3:
			if (ps->date & ECO_WEDNESDAY_MASK)
				hit_date = 1;
			break;
		case 4:
			if (ps->date & ECO_THURSDAY_MASK)
				hit_date = 1;
			break;
		case 5:
			if (ps->date & ECO_FRIDAY_MASK)
				hit_date = 1;
			break;
		case 6:
			if (ps->date & ECO_SATURDAY_MASK)
				hit_date = 1;
			break;
	}

	if (ps->flag4)	 // 24 hr
		hit_time = 1;
	else {
		start =  ((ps->fTime >> 4)&0x1f)*3600 + ((ps->fTime & 0xf)*5)*60;
		end =  ((ps->tTime >> 4)&0x1f)*3600 + ((ps->tTime & 0xf)*5)*60;
		current = tm_time.tm_hour*3600 +  tm_time.tm_min*60 +   tm_time.tm_sec; 
		if (current > start && current < end)
			hit_time = 1;	

		DEBUG_PRINT("start=%d, end=%d, current=%d\n", start, end, current); // for debug
	}

	DEBUG_PRINT("pre_enable_state=%d, hit_date=%d, hit_time=%d\n", pre_enable_state, hit_date, hit_time);

	if (pre_enable_state < 0) { // first time
		if (hit_date && hit_time) {
			disable_wlan();
			pre_enable_state = 0;
		}
		else
			pre_enable_state = 1;	
	}
	else {
		if (pre_enable_state && hit_date && hit_time) {
			disable_wlan();
			pre_enable_state = 0;
		}
		else if (!pre_enable_state && (!hit_date || !hit_time)) {
			enable_wlan();		
			pre_enable_state = 1;
		}
	}
}
#else /*main trunk*/

static void parse_schl(char *argv, struct ps_info *ps)
{
	int i, head, tail, value[4];
	char tmpbuf[8];

	head = 0;
	for (i=0; i<4; i++) {
		tail = head + 1;
		while (argv[tail] != ',')
			tail++;
		memset(tmpbuf, 0, sizeof(tmpbuf));
		strncpy(tmpbuf, &(argv[head]), tail-head);
		value[i] = atoi(tmpbuf);
		head = tail + 1;
	}

	ps->fTime = value[1];
	ps->tTime = value[2];
	ps->date  = value[3];
}

static void dump_ps(struct ps_info *ps)
{
	char tmpbuf[200];

	sprintf(tmpbuf, "date (%x): ", ps->date);
	if (ps->date & ECO_SUNDAY_MASK)
		strcat(tmpbuf, "Sunday ");
	if (ps->date & ECO_MONDAY_MASK)
		strcat(tmpbuf, "Monday ");
	if (ps->date & ECO_TUESDAY_MASK)
		strcat(tmpbuf, "Tuesday ");
	if (ps->date & ECO_WEDNESDAY_MASK)
		strcat(tmpbuf, "Wednesday ");
	if (ps->date & ECO_THURSDAY_MASK)
		strcat(tmpbuf, "Thursday ");
	if (ps->date & ECO_FRIDAY_MASK)
		strcat(tmpbuf, "Friday ");
	if (ps->date & ECO_SATURDAY_MASK)
		strcat(tmpbuf, "Saturday ");
	strcat(tmpbuf, "\n");
	DEBUG_PRINT(tmpbuf);

	DEBUG_PRINT("From (%x): %dh %dm\n", ps->fTime, (ps->fTime / 60), (ps->fTime % 60));
	DEBUG_PRINT("To (%x): %dh %dm\n",  ps->tTime, (ps->tTime /60), (ps->tTime % 60));
}


static void check_time_and_control_RF(struct ps_info *ps, int wlan_if, int force_on)
{
	int hit_date=0, hit_time=0;
	unsigned int start, end, current;
	time_t tm;
	struct tm tm_time;

	if (force_on) {
		hit_date = 1;		
		hit_time = 1;			
	}	

	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));

	DEBUG_PRINT(" tm_wday=%d, tm_hour=%d, tm_min=%d\n",
		tm_time.tm_wday, tm_time.tm_hour, tm_time.tm_min);

	switch(tm_time.tm_wday) {
		case 0:
			if (ps->date & ECO_SUNDAY_MASK)
				hit_date = 1;
			break;
		case 1:
			if (ps->date & ECO_MONDAY_MASK)
				hit_date = 1;
			break;
		case 2:
			if (ps->date & ECO_TUESDAY_MASK)
				hit_date = 1;
			break;
		case 3:
			if (ps->date & ECO_WEDNESDAY_MASK)
				hit_date = 1;
			break;
		case 4:
			if (ps->date & ECO_THURSDAY_MASK)
				hit_date = 1;
			break;
		case 5:
			if (ps->date & ECO_FRIDAY_MASK)
				hit_date = 1;
			break;
		case 6:
			if (ps->date & ECO_SATURDAY_MASK)
				hit_date = 1;
			break;
	}

	start = ps->fTime;
	end   = ps->tTime;
	current = tm_time.tm_hour * 60 + tm_time.tm_min;
	DEBUG_PRINT("start=%d, end=%d, current=%d\n", start, end, current);

	if (end >= start) {
		if ((current >= start) && (current < end))
			hit_time = 1;
	}
	else { 
		if ((current >= start) || (current < end))
			hit_time = 1;
	}

	DEBUG_PRINT("pre_enable_state=%d, hit_date=%d, hit_time=%d\n", pre_enable_state, hit_date, hit_time);

	if (pre_enable_state < 0) { // first time
		if (hit_date && hit_time) {
			pre_enable_state = 1;
			if (!is_wlan_exist(wlan_if))
				enable_wlan(wlan_if);			
		}
		else {
			disable_wlan(wlan_if);
			pre_enable_state = 0;
		}
	}
	else {
		if (!pre_enable_state && hit_date && hit_time) {
			enable_wlan(wlan_if);
			pre_enable_state = 1;
		}
		else if (pre_enable_state && (!hit_date || !hit_time)) {
			disable_wlan(wlan_if);
			pre_enable_state = 0;
		}
	}
}
#endif // CONFIG_RTL8186_KB

#define MAX_SCHEDULE_NUM 10 /* everyday, Mon~Sun */
#define MAX_WLAN_INT_NUM 2 /*we assume Max 2 WLAN interface */

void chk_wlanSch_ksRule(struct ps_info *ks, int index)
{
	int i=0;
	time_t tm;
	struct tm tm_time;
	struct ps_info *ps;
	int hit_date = 0;
	int hit_time = 0;
		
	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
	
	DEBUG_PRINT(" tm_wday=%d, tm_hour=%d, tm_min=%d\n", tm_time.tm_wday, tm_time.tm_hour, tm_time.tm_min);	

	for(i=0 ; i<MAX_SCHEDULE_NUM ; i++)
	{

		
		if(ks[i].date > 7) // this item is null
		{
			continue;
		}
		else
		{	
			hit_time = 0;
			hit_date = 0;
			ps = &ks[i];
			if(ps->date == 7) // 7:everyday
			{
				hit_date = 1;
			}
			else if(ps->date == tm_time.tm_wday)
			{
				hit_date = 1;
			}
			else
				continue;
			
			DEBUG_PRINT("\r\n ps.date = [%u], ps.fTime = [%u], ps.tTime = [%u]__[%s-%u]\r\n",ps->date,ps->fTime,ps->tTime,__FILE__,__LINE__);

			int start = ps->fTime;
			int end = ps->tTime;
			int current = tm_time.tm_hour * 60 + tm_time.tm_min;
			DEBUG_PRINT("start=%d, end=%d, current=%d\n", start, end, current);
		
			if (end >= start) {
				if ((current >= start) && (current < end))
					hit_time = 1;
			}
			else {
				if ((current >= start) || (current < end))
					hit_time = 1;
			}			

			if(hit_date && hit_time)
				break;
			
		}								
	}
	
	DEBUG_PRINT("pre_enable_state=%d, hit_date=%d, hit_time=%d\n", pre_enable_state_new[index], hit_date, hit_time);											
	if (pre_enable_state_new[index] < 0) { // first time
		if (hit_date && hit_time) {
			pre_enable_state_new[index] = 1;
			if (!is_wlan_exist(index))
				enable_wlan(index);
		}
		else {
			disable_wlan(index);
			pre_enable_state_new[index] = 0;
		}
	}
	else {
		if (!pre_enable_state_new[index] && hit_date && hit_time) {
			enable_wlan(index);
			pre_enable_state_new[index] = 1;
		}
		else if (pre_enable_state_new[index] && (!hit_date || !hit_time)) {
			disable_wlan(index);
			pre_enable_state_new[index] = 0;
		}
	}
	DEBUG_PRINT("%s %d\n",__FUNCTION__,__LINE__);
}

#ifdef TIMER_TASK_SUPPORT
void chk_taskSch_ksRule(task_info_Tp ks)
{
	int i=0;
	time_t tm;
	struct tm tm_time;
	task_info_Tp ps;
	int hit_date = 0,hit_time = 0, start, end, current, hit_idx=0;
	unsigned char cmd[MAX_TASK_ACTION_LEN+10]={0}, tmpbuf[100]={0};	
	int max_sec_offset = SLEEP_TIME + 1;
	FILE *fp;
		
	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
	
	DEBUG_PRINT(" tm_wday=%d, tm_hour=%d, tm_min=%d, tm_sec=%d\n", tm_time.tm_wday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);	

	for(i=0 ; i<MAX_TASK_SCHEDULE_NUM ; i++)
	{	
		if(ks[i].date > 7) // this item is null
		{
			continue;
		}
		else
		{	
			hit_time = 0;
			hit_date = 0;
			ps = &ks[i];

			if(ps->date == tm_time.tm_wday)
			{
				hit_date = 1;
			}
			else
				continue;
			
			
			DEBUG_PRINT("\n ps.date = [%u], ps.fTime = [%u], ps.tTime = [%u], ps.action = [%s], ps.index = [%u], ps.loop = [%d]__[%s-%u]\n",
			ps->date, ps->fTime, ps->tTime, ps->action, ps->index, ps->loop, __FILE__,__LINE__);
 
			start = ps->fTime;
			end = ps->tTime;
			current = (tm_time.tm_hour * 60 + tm_time.tm_min) * 60 + tm_time.tm_sec;
			
			DEBUG_PRINT("start=%d, end=%d, current=%d\n", start, end, current);

			//if(current==target_time)
			if(!strncmp(ps->action, TIMER_TASK_STR_REBOOT, sizeof(TIMER_TASK_STR_REBOOT)-1)){
				if(((start>=current) && (start-current<=max_sec_offset)) ||
					((start<current) && (current-start<=max_sec_offset)))
					hit_time = 1;	
			}else if(!strncmp(ps->action, TIMER_TASK_STR_HEALTHMODE, sizeof(TIMER_TASK_STR_HEALTHMODE)-1)){
				if(end >= start){
					if ((current >= start) && (current < end))
						hit_time = 1;
				}else {
				if ((current >= start) || (current < end))
					hit_time = 1;
				}	
				
				if(!hit_date || !hit_time){
					if(ps->index == 0){ //index:0 indicate all band
						if (!is_wlan_exist(0)){
							printf("[%s %d] set ifconfig wlan%d up\n",__FUNCTION__,__LINE__,0);
							enable_wlan(0);
						}
						if (!is_wlan_exist(1)){
							printf("[%s %d] set ifconfig wlan%d up\n",__FUNCTION__,__LINE__,1);
							enable_wlan(1);
						}
					}else if(ps->index >= 1 && ps->index<= 4){ //index:1-4 indicate 2.4G
						if (!is_wlan_exist(1)){
							printf("[%s %d] set ifconfig wlan%d up\n",__FUNCTION__,__LINE__,1);
							enable_wlan(1);
						}
					}else if(ps->index >= 5 && ps->index <= 8){ //index:5-8 indicate 5G
						if (!is_wlan_exist(0)){
							printf("[%s %d] set ifconfig wlan%d up\n",__FUNCTION__,__LINE__,0);
							enable_wlan(0);
						}
					}

					//action:ToSetHealthMode, no hit_date and hit_time, current > end, need hit_idx
					if((ps->loop == 0) && (current > end)){
						hit_idx = ps->taskid;
					}
					
					fp = fopen(PARAM_TEMP_FILE, "w");
					if(fp){
						//action, hit_idx, index, disable
						snprintf(cmd, sizeof(cmd), "%s,%d,%d,%d", TIMER_TASK_STR_HEALTHMODE, hit_idx, ps->index,0);
						fputs(cmd,fp);
						fclose(fp);
					}else{
						printf("create file %s fail\n", PARAM_TEMP_FILE);
						return;
					}

					snprintf(tmpbuf, sizeof(tmpbuf), "flash -file_task_timer %s", PARAM_TEMP_FILE);				
					system(tmpbuf);	
					
				}
			}

			if(hit_date && hit_time){
				//for action To Reboot
				hit_idx = ps->taskid;
				break;		
			}	
		}
	}
	
	DEBUG_PRINT("hit_date=%d, hit_time=%d, action:%s, loop:%d\n", hit_date, hit_time, ps->action, ps->loop);	
	if(hit_date && hit_time){		
		if(!strncmp(ps->action, TIMER_TASK_STR_REBOOT, sizeof(TIMER_TASK_STR_REBOOT)-1)){
			// timer done just one time
			if(ps->loop==0){
				fp = fopen(PARAM_TEMP_FILE, "w");
				if(fp){
					snprintf(cmd, sizeof(cmd), "%s,%d,%d,%d", TIMER_TASK_STR_REBOOT, hit_idx, ps->index, 0);
					fputs(cmd,fp);
					fclose(fp);
				}else{
					printf("create file %s fail\n", PARAM_TEMP_FILE);
					return;
				}
				
				snprintf(tmpbuf, sizeof(tmpbuf), "flash -file_task_timer %s", PARAM_TEMP_FILE);				
				system(tmpbuf);		
			}
			
			system("reboot");
		}else if(!strncmp(ps->action, TIMER_TASK_STR_HEALTHMODE, sizeof(TIMER_TASK_STR_HEALTHMODE)-1)){
			if(ps->index == 0){    //index:0 indicate all band
				if (is_wlan_exist(0)){
					printf("[%s %d] set ifconfig wlan%d and vap down\n",__FUNCTION__,__LINE__,0);
					disable_wlan(0);
				}
				if (is_wlan_exist(1)){
					printf("[%s %d] set ifconfig wlan%d and vap down\n",__FUNCTION__,__LINE__,1);
					disable_wlan(1);
				}
			}else if(ps->index >= 1 && ps->index <= 4){  //index:1-4 indicate 2.4G
				if (is_wlan_exist(1)){
					printf("[%s %d] set ifconfig wlan%d and vap down\n",__FUNCTION__,__LINE__,1);
					disable_wlan(1);
				}
			}else if(ps->index >= 5 && ps->index <= 8){ //index:5-8 indicate 5G
				if (is_wlan_exist(0)){
					printf("[%s %d] set ifconfig wlan%d and vap down\n",__FUNCTION__,__LINE__,0);
					disable_wlan(0);
				}
			}
			
			hit_idx = 0;
		
			fp = fopen(PARAM_TEMP_FILE, "w");
			if(fp){
				//action, hit_idx, index, disable
				snprintf(cmd, sizeof(cmd), "%s,%d,%d,%d", TIMER_TASK_STR_HEALTHMODE, hit_idx, ps->index,1);
				fputs(cmd,fp);
				fclose(fp);
			}else{
				printf("create file %s fail\n", PARAM_TEMP_FILE);
				return;
			}

			snprintf(tmpbuf, sizeof(tmpbuf), "flash -file_task_timer %s", PARAM_TEMP_FILE);				
			system(tmpbuf);		

		}
	}
	
	return;
}
#endif

#ifdef CBN_SPEC
int getPid(char *filename)
{
        struct stat status;
        char buff[100];
        FILE *fp;

        if ( stat(filename, &status) < 0)
                return -1;
        fp = fopen(filename, "r");
        if (!fp) {
                return -1;
        }
        fgets(buff, 100, fp);
        fclose(fp);

        return (atoi(buff));
}

void killSomeDaemon(void)
{
	system("killall -9 sleep 2> /dev/null");
	system("killall -9 routed 2> /dev/null");
//	system("killall -9 pppoe 2> /dev/null");
//	system("killall -9 pppd 2> /dev/null");
//	system("killall -9 pptp 2> /dev/null");
	system("killall -9 dnrd 2> /dev/null");
	system("killall -9 ntpclient 2> /dev/null");
//	system("killall -9 miniigd 2> /dev/null");	//comment for miniigd iptables rule recovery
	system("killall -9 lld2d 2> /dev/null");
//	system("killall -9 l2tpd 2> /dev/null");	
//	system("killall -9 udhcpc 2> /dev/null");	
//	system("killall -9 udhcpd 2> /dev/null");	
//	system("killall -9 reload 2> /dev/null");		
	system("killall -9 iapp 2> /dev/null");	
	system("killall -9 wscd 2> /dev/null");
	system("killall -9 mini_upnpd 2> /dev/null");
	system("killall -9 iwcontrol 2> /dev/null");
	system("killall -9 auth 2> /dev/null");
	system("killall -9 disc_server 2> /dev/null");
	system("killall -9 igmpproxy 2> /dev/null");
	system("killall -9 syslogd 2> /dev/null");
	system("killall -9 klogd 2> /dev/null");
	
	system("killall -9 ppp_inet 2> /dev/null");
	
#ifdef VOIP_SUPPORT
	system("killall -9 snmpd 2> /dev/null");
	system("killall -9 solar_monitor 2> /dev/null");
	system("killall -9 solar 2> /dev/null");
	system("killall -9 dns_task 2> /dev/null");
	system("killall -9 ivrserver 2> /dev/null");
#endif

#ifdef CONFIG_SNMP
	system("killall -9 snmpd 2> /dev/null");
#endif
}

void run_init_script(char *arg)
{
	int pid=0;
	char tmpBuf[100];
	
#ifdef RTK_USB3G
	system("killall -9 mnet 2> /dev/null");
	system("killall -9 hub-ctrl 2> /dev/null");
	system("killall -9 usb_modeswitch 2> /dev/null");
	system("killall -9 ppp_inet 2> /dev/null");
	system("killall -9 pppd 2> /dev/null");
	system("rm /etc/ppp/connectfile >/dev/null 2>&1");
#endif /* #ifdef RTK_USB3G */

#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
	Stop_Domain_Query_Process();
	Reset_Domain_Query_Setting();
#endif

	snprintf(tmpBuf, 100, "%s/%s.pid", _DHCPD_PID_PATH, _DHCPD_PROG_NAME);
	pid = getPid(tmpBuf);
	if ( pid > 0)
		kill(pid, SIGUSR1);
		
	usleep(1000);
	
	if ( pid > 0){
		system("killall -9 udhcpd 2> /dev/null");
		system("rm -f /var/run/udhcpd.pid 2> /dev/null");
	}
	//Patch: kill some daemons to free some RAM in order to call "init.sh gw all" more quickly
	//which need more tests especially for 8196c 2m/16m
	killSomeDaemon();
	system("killsh.sh");	// kill all running script	

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
	web_restart_solar();
#endif

	pid = fork();
/*	
       	if (pid)
               	waitpid(pid, NULL, 0);
   	else 
*/ 
	if (pid == 0) {
#ifdef HOME_GATEWAY
		sprintf(tmpBuf, "%s gw %s", _CONFIG_SCRIPT_PROG, arg);
#elif defined(VOIP_SUPPORT) && defined(ATA867x)
		sprintf(tmpBuf, "%s ATA867x %s", _CONFIG_SCRIPT_PROG, arg);
#else
		sprintf(tmpBuf, "%s ap %s", _CONFIG_SCRIPT_PROG, arg);
#endif
		sleep(1);
		system(tmpBuf);
		exit(1);
	}
}

void sigHandler_swreinit(int signo)
{
	#define REINIT_WEB_FILE		"/tmp/reinit_web"
	struct stat status;
	int reinit=1;

	if (stat(REINIT_WEB_FILE, &status) == 0) { // file existed
        unlink(REINIT_WEB_FILE);
		reinit = 0;		
	}	
	if (reinit) { // re-init system
#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
	   Start_Domain_Query_Process=0;
#endif
#ifndef NO_ACTION
		run_init_script("all");
#endif		
	}
}
#endif	//CBN_SPEC

int wisp_wan_connected()
{
    FILE *pfile = NULL;
    char tmpBuf[128];
    int wan_type;
    
	if((pfile = fopen(WAN_DHCP_CONNECTED,"r+"))!= NULL)
    {
        fgets(tmpBuf, sizeof(tmpBuf), pfile);
        system(tmpBuf);
        unlink(WAN_DHCP_CONNECTED);
        fclose(pfile);
    }
    else
    {
        return -1;
    }
    return 0;
}

#if defined(TRAFFIC_METER_SUPPORT)	
char *get_name(char *name, char *p)
{
    while (isspace(*p))
	p++;
    while (*p) {
	if (isspace(*p))
	    break;
	if (*p == ':') {	/* could be an alias */
	    char *dot = p, *dotname = name;
	    *name++ = *p++;
	    while (isdigit(*p))
		*name++ = *p++;
	    if (*p != ':') {	/* it wasn't, backup */
		p = dot;
		name = dotname;
	    }
	    if (*p == '\0')
		return NULL;
	    p++;
	    break;
	}
	*name++ = *p++;
    }
    *name++ = '\0';
    return p;
}
int getStats(char *interface, struct user_net_device_stats *pStats)
{
 	FILE *fh;
  	char buf[512];
	int type;
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		printf("Warning: cannot open %s\n",_PATH_PROCNET_DEV);
		return -1;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);
  	if (strstr(buf, "compressed"))
		type = 3;
	else if (strstr(buf, "bytes"))
		type = 2;
	else
		type = 1;
	while (fgets(buf, sizeof buf, fh)) {
		char *s, name[40];
		s = get_name(name, buf);
		if ( strcmp(interface, name))
			continue;
		get_dev_fields(type, s, pStats);
		fclose(fh);
		return 0;
    	}
	fclose(fh);
	return -1;
}
int get_dev_fields(int type, char *bp, struct user_net_device_stats *pStats)
{
    switch (type) {
    case 3:
	sscanf(bp,
	"%Lu %Lu %lu %lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu %lu",
	       &pStats->rx_bytes,
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,
	       &pStats->rx_compressed,
	       &pStats->rx_multicast,
	       &pStats->tx_bytes,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors,
	       &pStats->tx_compressed);
	break;
    case 2:
	sscanf(bp, "%Lu %Lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu",
	       &pStats->rx_bytes,
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,
	       &pStats->tx_bytes,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors);
	pStats->rx_multicast = 0;
	break;
    case 1:
	sscanf(bp, "%Lu %lu %lu %lu %lu %Lu %lu %lu %lu %lu %lu",
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors);
	pStats->rx_bytes = 0;
	pStats->tx_bytes = 0;
	pStats->rx_multicast = 0;
	break;
    }
    return 0;
}
int get_wan_if(unsigned char *wan_if)
{
	int op_mode, wisp_id, wan_type, wlan_mode;
	unsigned char value[64], cmd[128];
	get_flash_int_value("WAN_DHCP", &wan_type);
	if(wan_type == 3 || wan_type == 4 ||wan_type == 6 ||wan_type == 16 )
		strcpy(wan_if, "ppp0");
	else
	{
		get_flash_int_value("OP_MODE", &op_mode);
		if(op_mode == 0)
		{
			strcpy(wan_if, "eth1");
		}
		else if(op_mode == 2)
		{
			get_flash_int_value("WISP_WAN_ID", &wisp_id);
			sprintf(value, "WLAN%d_WLAN_MODE", wisp_id);
			get_flash_int_value(value, &wlan_mode);
			if(wlan_mode == 0)
				sprintf(wan_if, "wlan%d-vxd", wisp_id);
			else
				sprintf(wan_if, "wlan%d", wisp_id);
		}
	}
	return 0;
}
#if 0
int get_traffic(traffic_info_T *traffic)
{
	time_t tm;
	struct tm tm_time;
	unsigned long long current_traffic;
	struct user_net_device_stats pStats;
	unsigned char bp;
	int i=0;
	unsigned char cmd[256], temp[64];
	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
	if((tm_time.tm_min > traffic->minute)  || (tm_time.tm_min==0 && traffic->minute==59) || (tm_time.tm_min < traffic->minute))
	{
		getStats(g_wan_if, &pStats);
		current_traffic = pStats.rx_bytes + pStats.tx_bytes;
		traffic->minute = tm_time.tm_min;
		if(current_traffic >= traffic->total_traffic)
		{
			if(tm_time.tm_hour == 0)
			{
				if(traffic->pos != -1)
				{
					if(tm_time.tm_min==0)
						traffic->stats[23] = current_traffic - traffic->total_traffic;
					else
						traffic->stats[23] += current_traffic - traffic->total_traffic;
				}
				traffic->pos = 0;
			}
			else
			{
				if(traffic->pos != -1)
				{
					if(tm_time.tm_min==0)
						traffic->stats[tm_time.tm_hour] = current_traffic - traffic->total_traffic;
					else
						traffic->stats[tm_time.tm_hour] += current_traffic - traffic->total_traffic;
				}
				traffic->pos = tm_time.tm_hour;
			}
			traffic->total_traffic = current_traffic;
			sprintf(cmd, "echo \"");
			for(i=0; i<24; i++)
			{
				sprintf(temp, "%Lu:", traffic->stats[i]);
				strcat(cmd, temp);
			}
			strcat(cmd, " \" > /var/trafic_by_hour");
			system(cmd);
		}
		else
		{
		}
	}
}
#endif
#if defined(CONFIG_APP_ADAPTER_API)||defined(CONFIG_ANDLINK_SUPPORT)||defined(CONFIG_ELINK_SUPPORT)
int get_load_statics(load_statics_T *load)
{
	unsigned long long current_speed;
	unsigned int interval_tm;
	time_t tm;
	struct user_net_device_stats pStats;
	unsigned char cmd[256];
	time(&tm);
	getStats(g_wan_if, &pStats);
	interval_tm = tm - load->tm;
	if(interval_tm > 0)
	{
		load->tm = tm;
		current_speed = (pStats.rx_bytes - load->rx_bytes)/interval_tm;
		load->rx_speed_cur = current_speed;
		if(current_speed > load->rx_speed_max)
			load->rx_speed_max = current_speed;
		if(current_speed < load->rx_speed_min)
			load->rx_speed_min = current_speed;
		load->rx_speed_avg = pStats.rx_bytes/(tm - tm_start);
		current_speed = (pStats.tx_bytes - load->tx_bytes)/interval_tm;
		load->tx_speed_cur = current_speed;
		if(current_speed > load->tx_speed_max)
			load->tx_speed_max = current_speed;
		if(current_speed < load->tx_speed_min)
			load->tx_speed_min = current_speed;
		load->tx_speed_avg = pStats.tx_bytes/(tm - tm_start);
		load->rx_bytes = pStats.rx_bytes;
		load->tx_bytes = pStats.tx_bytes;
	}
	sprintf(cmd, "echo \"%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu\" > /var/load_statics",
		load->rx_speed_max, load->rx_speed_min, load->rx_speed_avg, load->rx_speed_cur, pStats.rx_bytes,
		load->tx_speed_max, load->tx_speed_min, load->tx_speed_avg, load->tx_speed_cur, pStats.tx_bytes);
	system(cmd);
	return 0;
}


rxtx_rate_stats_T * find_rate_entry_from_tbl( int linkFlag, int portId, unsigned char *macAddr)
{
	int i;
	rxtx_rate_stats_T *valid_entry=NULL;
	
	for(i=0;i<MAX_TERM_NUMBER;i++)
	{
		valid_entry = &rate_stats_tbl[i];
		if(valid_entry->valid){
			if(
				(valid_entry->linkFlag==linkFlag )&&(valid_entry->port==portId)
				&&(equal_mac(valid_entry->macAddr,macAddr)==1)
			)
			return valid_entry;
		}
	}
	return NULL;
}


rxtx_rate_stats_T * find_emptyRate_entry_from_tbl( void)
{
	int i;
	rxtx_rate_stats_T *valid_entry=NULL;
	
	for(i=0;i<MAX_TERM_NUMBER;i++)
	{
		valid_entry = &rate_stats_tbl[i];
		if(valid_entry->valid==0){
			valid_entry->valid=1;
			return valid_entry;
		}
	}
	return NULL;
}

void rtl_GetPortRate(time_t time)
{
	/*fill cur_rx /cur_tx parememter */
	FILE *fp=NULL;
	int  line_cnt =0;
	unsigned char buffer[128];
	int port_id;
	unsigned char cmd[256];
	rxtx_rate_stats_T *stats_entry=NULL;
	rxtx_rate_stats_T temp_entry;
	rxtx_rate_stats_T *info=&temp_entry;
	memset(info,0,sizeof(rxtx_rate_stats_T));
	if((fp = fopen("/proc/rtl865x/asicCounter","r+")) != NULL)
	{
		{
			port_id=0;
			while(fgets(buffer, 128, fp))
			{
				if(port_id>4)
					goto out;
				line_cnt++;
				
				info->port =port_id;
				info->linkFlag =0;
				if(line_cnt == 12*port_id+3)	//update receive bytes
				{
					info->rx_bytes =0;
					sscanf(buffer," Rcv %lu ",&(info->rx_bytes));
					
					//printf("%d,%d,%lu,%lu,[%s]:[%d].\n",info->port,info->linkFlag,info->rx_bytes,info->tx_bytes,__FUNCTION__ ,__LINE__);
				}
				
				if(line_cnt == 12*port_id+10)	//update send bytes
				{
					info->tx_bytes =0;
					sscanf(buffer," Snd %lu ",&(info->tx_bytes));
					port_id ++;
					if(info->rx_bytes||info->tx_bytes)
					{
						stats_entry=find_rate_entry_from_tbl(info->linkFlag,info->port,info->macAddr);
						if(stats_entry)
						{
							unsigned int interval_tm;
							interval_tm =time -stats_entry->tm;
							
							//printf("%d,%d,%u,%u,%x [%s]:[%d].\n",info->port,info->linkFlag,info->rx_bytes,info->tx_bytes,interval_tm,__FUNCTION__ ,__LINE__);
							stats_entry->linkFlag =info->linkFlag;
							stats_entry->port=info->port;
							memcpy(stats_entry->macAddr,info->macAddr,6);
							if(info->rx_bytes&&interval_tm)
								stats_entry->rx_speed =( info->rx_bytes-stats_entry->rx_bytes)/interval_tm;
							if(info->tx_bytes&&interval_tm)
								stats_entry->tx_speed =( info->tx_bytes-stats_entry->tx_bytes)/interval_tm;
							
							stats_entry->rx_bytes=info->rx_bytes; 
							stats_entry->tx_bytes =info->tx_bytes;
							stats_entry->tm =time;
						}
						else
						{
							stats_entry=find_emptyRate_entry_from_tbl( );
							if(stats_entry)
							{
								stats_entry->linkFlag =info->linkFlag;
								stats_entry->port=info->port;
								memcpy(stats_entry->macAddr,info->macAddr,6);
								stats_entry->rx_bytes=info->rx_bytes; 
								stats_entry->tx_bytes =info->tx_bytes;
								stats_entry->tm =time;
							}	
							else
							{
								printf("can't find empty rate stats entry![%s]:[%d]\n",__FUNCTION__,__LINE__);
							}
						}
					}
					memset(info,0,sizeof(rxtx_rate_stats_T));
					//printf("%d,%d,%lu,%lu,[%s]:[%d].\n",info->port,info->linkFlag,info->rx_bytes,info->tx_bytes,__FUNCTION__,__LINE__);
				}
				
				
			}
			
		}
	out:	
		fclose(fp);
	}
	return;
}




int check_wifi(char *wifi_interface, time_t time)
{
	int sta_num = 0 ;
	RTK_WLAN_STA_INFO_Tp pInfo;
	int i;
	char *buff;
	rxtx_rate_stats_T temp_entry;
	rxtx_rate_stats_T *info=&temp_entry;
	rxtx_rate_stats_T *stats_entry=NULL;
	int ret =0;

	if(!getWlStaNum(wifi_interface,&sta_num))
	{
		if(sta_num <= 0)
			goto out;
	}
	else		
		goto out;

	buff = malloc(sizeof(RTK_WLAN_STA_INFO_T) * (MAX_STA_NUM+1));
	if(!buff)
		goto out;
	
	memset(buff,0,sizeof(RTK_WLAN_STA_INFO_T) * (MAX_STA_NUM +1));	
	if(rtk_get_wlan_sta(wifi_interface,(RTK_WLAN_STA_INFO_Tp)buff) )
	{	
	    if(strstr(wifi_interface, "vxd")){      //added by david_zhang: for vxd multiple macclone case
	        memset(info,0,sizeof(rxtx_rate_stats_T));
	        info->port =0xF;
    		info->linkFlag =1;
	        for(i = 1; i <= sta_num; ++i)
	        {	
	            pInfo = (RTK_WLAN_STA_INFO_Tp)&buff[i*sizeof(RTK_WLAN_STA_INFO_T)];
	            info->rx_bytes+=pInfo->rx_bytes;
    			info->tx_bytes+=pInfo->tx_bytes;
	        }
	        memcpy(info->macAddr,pInfo->addr,6);
	        #if 0
			printf("[%x:%x:%x:%x:%x:%x]: rx: %u tx:%u[%s]:[%d].\n",info->macAddr[0],info->macAddr[1],info->macAddr[2],
			info->macAddr[3],info->macAddr[4],info->macAddr[5],info->rx_bytes,info->tx_bytes,__FUNCTION__,__LINE__);
    		#endif
	        if(info->rx_bytes ||info->tx_bytes)
			{
				stats_entry=find_rate_entry_from_tbl(info->linkFlag,info->port,info->macAddr);
				if(stats_entry)
				{
					unsigned int interval_tm;
					interval_tm =time -stats_entry->tm;
					stats_entry->linkFlag =info->linkFlag;
					stats_entry->port=info->port;
					//memcpy(stats_entry->macAddr,info->macAddr,6);
					
					if(info->rx_bytes&&interval_tm)
						stats_entry->rx_speed =( info->rx_bytes-stats_entry->rx_bytes)/interval_tm;
					if(info->tx_bytes&&interval_tm)
						stats_entry->tx_speed =( info->tx_bytes-stats_entry->tx_bytes)/interval_tm;
					
					stats_entry->rx_bytes=info->rx_bytes; 
					stats_entry->tx_bytes =info->tx_bytes;
					stats_entry->tm =time;
				}
				else
				{
					stats_entry=find_emptyRate_entry_from_tbl( );
					if(stats_entry)
					{
						stats_entry->linkFlag =info->linkFlag;
						stats_entry->port=info->port;
						memcpy(stats_entry->macAddr,info->macAddr,6);
						stats_entry->rx_bytes=info->rx_bytes; 
						stats_entry->tx_bytes =info->tx_bytes;
						stats_entry->tm =time;
						
					}	
					else
					{
						printf("can't find empty rate stats entry![%s]:[%d]\n",__FUNCTION__,__LINE__);
					}
				}
			}
	    }else{
    		for(i = 1; i <= sta_num; ++i)
    		{
    			memset(info,0,sizeof(rxtx_rate_stats_T));
    			
    			pInfo = (RTK_WLAN_STA_INFO_Tp)&buff[i*sizeof(RTK_WLAN_STA_INFO_T)];
    			info->port =0xF;
    			info->linkFlag =1;
    			info->rx_bytes=pInfo->rx_bytes;
    			info->tx_bytes=pInfo->tx_bytes;
    			memcpy(info->macAddr,pInfo->addr,6);
    			
    			#if 0
    			printf("[%x:%x:%x:%x:%x:%x]: rx: %u tx:%u[%s]:[%d].\n",info->macAddr[0],info->macAddr[1],info->macAddr[2],
    			info->macAddr[3],info->macAddr[4],info->macAddr[5],info->rx_bytes,info->tx_bytes,__FUNCTION__,__LINE__);
    			#endif			
    			if(info->rx_bytes ||info->tx_bytes)
    			{
    				stats_entry=find_rate_entry_from_tbl(info->linkFlag,info->port,info->macAddr);
    				if(stats_entry)
    				{
    					unsigned int interval_tm;
    					interval_tm =time -stats_entry->tm;
    					stats_entry->linkFlag =info->linkFlag;
    					stats_entry->port=info->port;
    					//memcpy(stats_entry->macAddr,info->macAddr,6);
    					
    					if(info->rx_bytes&&interval_tm)
    						stats_entry->rx_speed =( info->rx_bytes-stats_entry->rx_bytes)/interval_tm;
    					if(info->tx_bytes&&interval_tm)
    						stats_entry->tx_speed =( info->tx_bytes-stats_entry->tx_bytes)/interval_tm;
    					
    					stats_entry->rx_bytes=info->rx_bytes; 
    					stats_entry->tx_bytes =info->tx_bytes;
    					stats_entry->tm =time;
    				}
    				else
    				{
    					stats_entry=find_emptyRate_entry_from_tbl( );
    					if(stats_entry)
    					{
    						stats_entry->linkFlag =info->linkFlag;
    						stats_entry->port=info->port;
    						memcpy(stats_entry->macAddr,info->macAddr,6);
    						stats_entry->rx_bytes=info->rx_bytes; 
    						stats_entry->tx_bytes =info->tx_bytes;
    						stats_entry->tm =time;
    						
    					}	
    					else
    					{
    						printf("can't find empty rate stats entry![%s]:[%d]\n",__FUNCTION__,__LINE__);
    					}
    				}
    			}
    			
    		}
		}
	}	
	free(buff);

	ret =1;
out :	
	return ret;	

	
}

int rtl_getWifiRate(time_t tm)
{
#if 1
    int i, j;
    char ifname[16];

    for(i = 0; i<NUM_WLAN_INTERFACE; i++)
    {
        for(j=0; j<=NUM_VWLAN; j++){
            if(j == 0)
                snprintf(ifname, sizeof(ifname), "wlan%d", i);
            else
                snprintf(ifname, sizeof(ifname), "wlan%d-va%d", i, j-1);
            check_wifi(ifname, tm);
        }
        snprintf(ifname, sizeof(ifname), "wlan%d-vxd", i);
        check_wifi(ifname, tm);
    }
#else	
	 check_wifi("wlan0",tm);
	 check_wifi("wlan1",tm);
	 return 0;
#endif
}
int  rtl_get_terminal_rxtx_rate(time_t tm )
{
	rtl_GetPortRate(tm);
	rtl_getWifiRate( tm);
	return  0;
}

int rtl_get_rate_stats(void)
{
	int i;
	time_t tm;
	rxtx_rate_stats_T *stats_entry=NULL;
	unsigned char line_buffer[100]={0};
	int flag=1;
	int fh=0; 

	
	time(&tm);
	rtl_get_terminal_rxtx_rate(tm );
	
	fh = open(TERMINAL_RATE_INFO, O_RDWR|O_CREAT|O_TRUNC);
	if(fh<0)
		return;

	flock(fh, LOCK_EX);
	for(i=0;i<MAX_TERM_NUMBER;i++)
	{
		stats_entry =&rate_stats_tbl[i];
		if(stats_entry->valid)
		{
			#if 0
			printf("[%d]: %u %u %x:%x:%x:%x:%x:%x %u %u %u %u\n",
			i,stats_entry->linkFlag,stats_entry->port,
			stats_entry->macAddr[0],stats_entry->macAddr[1],stats_entry->macAddr[2],
			stats_entry->macAddr[3],stats_entry->macAddr[4],stats_entry->macAddr[5],
			stats_entry->rx_speed,stats_entry->tx_speed,stats_entry->rx_bytes,stats_entry->tx_bytes);
			#endif
			if((tm-stats_entry->tm) >= (10*3)){
				memset(stats_entry,0,sizeof(rxtx_rate_stats_T));
			}
			else
			{
				sprintf(line_buffer,"%u %u %02x%02x%02x%02x%02x%02x %u %u %u %u\n",stats_entry->linkFlag,stats_entry->port,
				stats_entry->macAddr[0],stats_entry->macAddr[1],stats_entry->macAddr[2],
				stats_entry->macAddr[3],stats_entry->macAddr[4],stats_entry->macAddr[5],
				stats_entry->rx_speed,stats_entry->tx_speed,stats_entry->rx_bytes,stats_entry->tx_bytes);
				write(fh, line_buffer, strlen(line_buffer));
			}
			//write_line_to_file(TERMINAL_RATE_INFO, flag, line_buffer); 
			//flag =2;
		}
	}
	flock(fh, LOCK_UN);
	close(fh);
	return 0;
	
}

#endif
#endif


void reset_led_off(){
	//¹Ø±ÕWPS LEDµÆ
	system("echo 17  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio17/direction"); 
	system("echo 0  > /sys/class/gpio/gpio17/value");  //¹Ø±Õ
	system("echo 0  > /sys/class/gpio/gpio17/value");  //¹Ø±
	//system("echo 1   > /sys/class/gpio/gpio17/value");  //´ò¿ª

	//¹Ø±ÕLAN LEDµÆ
	system("echo 55  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio55/direction"); 
	system("echo 1  > /sys/class/gpio/gpio55/value");  //¹Ø±Õ
	system("echo 1  > /sys/class/gpio/gpio55/value");  //¹Ø±Õ
	//system("echo 0   > /sys/class/gpio/gpio55/value");   //´ò¿ª
}



int main(int argc, char *argv[])
{
	FILE *fp;
	FILE *fp1;
	int gpio_state=-1;
	int last_gpio_state=-1;
	unsigned char keepalive[5];
	keepalive[0]='8';
	keepalive[1]='8';
	keepalive[2]='8';
	int i;
	unsigned char line[20];
	int sleep_time = 3;
	
#if defined(TRAFFIC_METER_SUPPORT)	
	get_wan_if(g_wan_if);
#if defined(CONFIG_APP_ADAPTER_API)||defined(CONFIG_ANDLINK_SUPPORT)||defined(CONFIG_ELINK_SUPPORT)
	load_statics_T g_load;
	memset(&g_load, 0, sizeof(load_statics_T));
	time(&tm_start);
	g_load.tm = tm_start;
	g_load.rx_speed_min = 0xffffffff;
	g_load.tx_speed_min = 0xffffffff;

	memset(rate_stats_tbl, 0, sizeof(rxtx_rate_stats_T)*MAX_TERM_NUMBER);

#endif
#endif

#ifdef CONFIG_RTL8186_KB
	struct ps_info ps;
	unsigned long val;

	static int ntp_success=0;

	memset(&ps, '\0', sizeof(ps));
	if (argc > 2 && !strcmp(argv[1], "-e")) {		
		sscanf(argv[2], "%lu", &val);		
		memcpy(&ps, &val,  4);
		dump_ps(&ps);
	}
	if (ps.flag1)
		system("echo u > /proc/gpio");
	else
		system("echo y > /proc/gpio");
#else /*main trunk*/ 
	struct ps_info ps;
	struct ps_info ks[MAX_WLAN_INT_NUM][MAX_SCHEDULE_NUM];
	
#if defined(TIMER_TASK_SUPPORT) 
	task_info_T tasks[MAX_TASK_SCHEDULE_NUM];
	static int task_schk = 0;
#endif

	static int wlan_schl=0, wlan_schk=0, ntp_enable=0, ntp_success=0;
	static int wlan_schkmulti[MAX_WLAN_INT_NUM]={0};
	
	memset(&ps, '\0', sizeof(ps));
	if (argc > 2) {
		for (i=0; i<argc; i++) {
			if (!strcmp(argv[i], "-e")) {
				parse_schl(argv[i+1], &ps);
				dump_ps(&ps);
				wlan_schl = 1;
			}
			if (!strcmp(argv[i], "-k")) 
			{
				FILE *fp;
				char line[200];
				unsigned char filename[20];
				unsigned char tmpfilename[50];

				
				memset(&ks, 0xff, sizeof(ps_info_T)*MAX_SCHEDULE_NUM*MAX_WLAN_INT_NUM);
				memset(filename, 0x00, sizeof(filename));
				sprintf(filename,"%s",argv[++i]);

				if(strlen(filename) == 0)
				{
					DEBUG_PRINT("get wlan schedule file failed!\n");										
				}
				else
				{
#if defined(TIMER_TASK_SUPPORT)
					snprintf(tmpfilename, sizeof(tmpfilename), "%s.task",filename);					
					fp = fopen(tmpfilename, "r");
					if (fp == NULL)
					{
						printf("file %s not exist!\n", tmpfilename);										
					}
					else
					{
						int idx1=0;
						char *str1, *lineptr1;	
						memset(&tasks, 0, sizeof(task_info_T)*MAX_TASK_SCHEDULE_NUM);
						while ( fgets(line, sizeof(line), fp) )
						{
							//entry idx, taskid, week, offset, action, loop
							if(strlen(line) != 0)
							{
								lineptr1 = line;
								
								str1 = strsep(&lineptr1,",");																
								idx1 = atoi(str1);
								idx1--;

								tasks[idx1].taskid = atoi(strsep(&lineptr1,","));
								tasks[idx1].date = atoi(strsep(&lineptr1,","));
								tasks[idx1].fTime = atoi(strsep(&lineptr1,","));
								tasks[idx1].tTime = atoi(strsep(&lineptr1,","));
								strncpy(tasks[idx1].action, strsep(&lineptr1,","), MAX_TASK_ACTION_LEN);
								tasks[idx1].index = atoi(strsep(&lineptr1,","));
								tasks[idx1].loop = atoi(strsep(&lineptr1,","));
							
								DEBUG_PRINT("	idx=[%u],taskid=[%d],date=[%u],fTime=[%u],tTime=[%u],action=[%s],index=[%u],loop=[%d]__[%s-%u]\n",
									idx1,tasks[idx1].taskid, tasks[idx1].date,tasks[idx1].fTime,tasks[idx1].tTime,tasks[idx1].action,tasks[idx1].index,tasks[idx1].loop,
									__FILE__,__LINE__);
								
								task_schk = 1;
							}
						}
						fclose(fp);
					}
#endif				
					for(i=0;i<MAX_WLAN_INT_NUM;i++)
					{
						sprintf(tmpfilename,"%s%d",filename,i);
						DEBUG_PRINT("filename (%s)\n",tmpfilename);
						fp = fopen(tmpfilename, "r");
						if (fp == NULL)
						{
							DEBUG_PRINT("open wlan schedule file failed!\n");										
						}
						else
						{
							while ( fgets(line, 200, fp) )
							{
								int idx;
								char *str;

								if(strlen(line) != 0)
								{
									char *lineptr = line;
									//strcat(line,",");
									
									str = strsep(&lineptr,",");
																	
									idx = atoi(str);

									idx--;
									
									ks[i][idx].date = atoi(strsep(&lineptr,","));
									ks[i][idx].fTime = atoi(strsep(&lineptr,","));
									ks[i][idx].tTime  = atoi(strsep(&lineptr,","));
									DEBUG_PRINT(" idx=[%u],date=[%u],fTime=[%u],tTime=[%u]__[%s-%u]\r\n",idx,ks[i][idx].date,ks[i][idx].fTime,ks[i][idx].tTime,__FILE__,__LINE__);
									wlan_schk = 1;
									wlan_schkmulti[i]=1;
								}
							}
							fclose(fp);
						}
					}
				}
			}
#if defined(CONFIG_RTL865X_KLD)
			if (!strcmp(argv[i], "-f")) {
				parse_schl(argv[i+1], &ps_va0);
				dump_ps(&ps_va0);
				wlan_va0_schl = 1;
			}
			if (!strcmp(argv[i], "-g")) {
				parse_schl(argv[i+1], &ps_eth);
				dump_ps(&ps_eth);			
			}
			if (!strcmp(argv[i], "-p")) {
				sscanf(argv[i+1], "%d", &eth_port);	
			}			
#endif
		}
	}	get_flash_int_value("NTP_ENABLED", &ntp_enable);
#endif
#ifdef CBN_SPEC
	sprintf(line, "%d\n", getpid());
	if ((fp = fopen("/var/run/rc.pid", "w")) == NULL) {
		printf("Can't create PID file!");
		return -1;
	}
	fwrite(line, strlen(line), 1, fp);
	fclose(fp);

	signal(SIGUSR1, sigHandler_swreinit);
#endif	//CBN_SPEC	
	while(1)
	{  	  
#if defined(TRAFFIC_METER_SUPPORT)
#if defined(CONFIG_APP_ADAPTER_API)||defined(CONFIG_ANDLINK_SUPPORT)||defined(CONFIG_ELINK_SUPPORT)
		get_load_statics(&g_load);
		
		rtl_get_rate_stats();
#else
#endif
#endif
#if defined(CONFIG_RTL_8196B)
#else
		fp=fopen("/proc/load_default","r");
		if(fp)	//add by LZQ to check if file be opened success
		{
			fscanf(fp,"%d",&i);
			if (i==1) 
			{   
		    	printf("Going to Reload Default\n");
				
				//after reset button pressed, turn off reset led immediately.
				reset_led_off();
				
				system("flash reset");
				system("reboot");
			}
			else if (i==2)
			{   
				system("reboot");
			}
			fclose(fp); 
		}
		else
		{
			//printf("<%s>LZQ: Open  /proc/load_default file error!\n", __FUNCTION__);
		}
#endif
		
		fp=fopen("/tmp/wscd_status","r");
		if(fp)	//add by wscd_status to check if file be WPS success
		{
			fscanf(fp,"%d",&i);
			if (i==3) 
			{   
				system("echo 1 > /proc/gpio");		
			}
			else
			{
					
			}
			
			fclose(fp); 
		}
		else
		{
			//printf("<%s>LZQ: Open  /proc/load_default file error!\n", __FUNCTION__);
		}
		
#ifdef CONFIG_RTL8186_KB
		if (!ntp_success)
		{
			fp=fopen("/etc/TZ","r");
			if (fp) 
			{
				fclose(fp);
				ntp_success = 1;
			}
		}
		if (ntp_success && ps.flag2) // enable power-saving
			check_time_and_control_RF(&ps);
#else	/*main trunk*/
		if (ntp_enable) {
			if (!ntp_success) {
				fp = fopen("/etc/TZ", "r");
				if (fp) {
					fclose(fp);
					ntp_success = 1;
				}
			}
		}
		if (wlan_schl && (!ntp_enable || ntp_success)) { // enable wlan scheduling
			check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
		}
#endif
  		//#else //LZQ
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL865X_SC)
		fp1 = fopen("/proc/gpio","w");
		fwrite(keepalive,1, 3, fp1);
		fclose(fp1);
#endif  

#if defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198) || defined(CONFIG_RTL_819XD) || defined(CONFIG_RTL_8196E)|| defined(CONFIG_RTL_8198B) || defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
		/* rtl8196C or rtl8198 do not need to check rf pin status*/
		gpio_state = 1;
		last_gpio_state =gpio_state; 
#else
		gpio_state = poll_rf_switch();
#endif
		DEBUG_PRINT("wlan_schk (%d) wlan_schkmulti (%d) (%d)\n",wlan_schk,wlan_schkmulti[0],wlan_schkmulti[1]);
		//printf("the gpio state =%d\n", gpio_state);
		if(last_gpio_state != gpio_state)
		{
			if(last_gpio_state == -1){ ///This is first state, we should check in advance for schedule, if the gpio is enabled
				if(gpio_state ==1 )
				{
#if !defined(CONFIG_RTL865X_AP)	
					if (ntp_enable) {
						if (!ntp_success) {
							fp = fopen("/etc/TZ", "r");
							if (fp) {
								fclose(fp);
								ntp_success = 1;
							}
						}
					}
#endif
						// enable wlan scheduling
					if ((wlan_schl || wlan_schk) && (!ntp_enable || ntp_success))
					{
						if(wlan_schk == 1)
						{
							for(i=0;i<MAX_WLAN_INT_NUM;i++)
								if(wlan_schkmulti[i])
									chk_wlanSch_ksRule(&ks[i],i);						
						}
						else
						check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
					}
					else if ((wlan_schl || wlan_schk) && ntp_enable && !ntp_success)
					{
						if(wlan_schk == 1)
						{
							for(i=0;i<MAX_WLAN_INT_NUM;i++)
								if(wlan_schkmulti[i])
									chk_wlanSch_ksRule(&ks[i],i);
						}
						else
						check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
				        }
				}
			}else {  ///It is not first state, we get the result from gpio state
					if(gpio_state ==1 ){ //gpio state is available for time schedule match
						#if !defined(CONFIG_RTL865X_AP)	
								if (ntp_enable) {
									if (!ntp_success) {
										fp = fopen("/etc/TZ", "r");
										if (fp) {
											fclose(fp);
											ntp_success = 1;
										}
									}
								}
						#endif
                                        // enable wlan scheduling
					if ( (wlan_schl || wlan_schk) && (!ntp_enable || ntp_success))
					{						
						if(wlan_schk == 1)
						{
							for(i=0;i<MAX_WLAN_INT_NUM;i++)
								if(wlan_schkmulti[i])
									chk_wlanSch_ksRule(&ks[i],i);
						}
						else
						check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
					}
					else if ( (wlan_schl || wlan_schk) && ntp_enable && !ntp_success)
					{						
						if(wlan_schk == 1)
						{
							for(i=0;i<MAX_WLAN_INT_NUM;i++)
								if(wlan_schkmulti[i])					
									chk_wlanSch_ksRule(&ks[i],i);
						}
						else
						check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
					}
					else
						enable_wlan(WLAN_IF_ROOT);
						}
			}
			last_gpio_state = gpio_state;
		}
		else
		{
				if(gpio_state ==1 ){
				#if !defined(CONFIG_RTL865X_AP)	
						if (ntp_enable) {
							if (!ntp_success) {
								fp = fopen("/etc/TZ", "r");
								if (fp) {
									fclose(fp);
									ntp_success = 1;
								}
							}
						}
				#endif
				
				// enable if5 timer task				
#ifdef TIMER_TASK_SUPPORT				
				if((task_schk==1) && ((!ntp_enable || ntp_success) || (ntp_enable && !ntp_success)))
				{
					chk_taskSch_ksRule(&tasks);
				}
#endif				
				// enable wlan scheduling
				if ( (wlan_schl || wlan_schk) && (!ntp_enable || ntp_success))
				{
					if(wlan_schk == 1)
					{
						for(i=0;i<MAX_WLAN_INT_NUM;i++)
								if(wlan_schkmulti[i])
									chk_wlanSch_ksRule(&ks[i],i);
					}
					else
					check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
				}
				else if ((wlan_schl || wlan_schk) && ntp_enable && !ntp_success)
				{
					if(wlan_schk == 1)
					{
						for(i=0;i<MAX_WLAN_INT_NUM;i++)
								if(wlan_schkmulti[i])
									chk_wlanSch_ksRule(&ks[i],i);
					}
					else
						check_time_and_control_RF(&ps, WLAN_IF_ROOT, 0);
			}
				}
		}

		wisp_wan_connected();
		//sleep(3);
		sleep(SLEEP_TIME);
	}
}
	
