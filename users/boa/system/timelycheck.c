#include "apmib.h"
#include "mibtbl.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h> 

#define RTL_LINK_PID_FILE "/var/run/rtl_link.pid"


#if defined(CONFIG_APP_BOA_AUTO_RECOVER)
#define CHECK_BOA_TIMEOUT 5
#endif

#if defined(CONFIG_ANDLINK_SUPPORT)
#define CHECK_ANDLINK_TIMEOUT 5
#endif


#define WIFI_SWITCH_GPIO "/proc/wifi_gpio" 
static unsigned int time_count;

static int wan_status=1;//0: can connect internet 1: cannot connect internet
static int wan_last_status = 0;
char *website="www.baidu.com";
#ifdef CONFIG_CMCC
char telnet_disable[5]={0};
#endif

//#define DEBUG 1
#ifdef DEBUG
#define DEBUG_TRACE(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG_TRACE(fmt, args...)
#endif
#ifdef CALC_CPU_OCCUPY
#define CHECK_CPU_INTERVAL	5
#endif
#ifdef CONFIG_APP_RTK_ADAPTER
#include <fcntl.h>
#include "../../adapter-api/rtk_api/rtk_api.h"
#include "../../adapter-api/rtk_api/rtk_ethernet_adapter.h"
#include "../../adapter-api/rtk_api/rtk_common_utility.h"

#define ETH_PORT_INFO  "/var/eth_port_info"
#define LAN_IFNAME "br0"
#define SEND_ARP_NUM 1
#define CHECK_INTERVAL 3
#define MAX_PORT_NUMBER 5

#define MAX_TERM_NUMBER 64

static RTK_ETH_PORT_INFO last_port_info[MAX_PORT_NUMBER];

static LAN_DEV_BASE_INFO last_dev_info[MAX_TERM_NUMBER];
static int last_wired_dev_num=0;
static unsigned long ntp_difference;

#ifdef CALC_CPU_OCCUPY
typedef struct cpu_info
{
	unsigned int cpu_total;
	unsigned int cpu_load;
	unsigned int cpu_idle;
}CPU_INFO_T, *CPU_INFO_Tp;
#endif

int rtk_output_port_info_to_file(RTK_ETH_PORT_INFO *pEthPortInfo, int array_len)
{
	int i;
	int port_num;
	unsigned int up_speed=0;
	unsigned int down_speed=0;
	char line_buffer[64]={0};

	int fd=0; 
	
	if((fd = open(ETH_PORT_INFO, O_RDWR|O_CREAT|O_TRUNC))<0)
		return -1;
	
	rtk_set_lock(fd, F_WRLCK);
	
	for(i=0;i<array_len;i++)
	{
		port_num=pEthPortInfo[i].port_num;
		up_speed=pEthPortInfo[i].up_speed;
		down_speed=pEthPortInfo[i].down_speed;

		//port_num up_speed down_speed
		snprintf(line_buffer, sizeof(line_buffer), "port_num=%d,up_speed=%u,down_speed=%u\n",port_num,up_speed,down_speed);

		write(fd, line_buffer, strlen(line_buffer));		
	}	
	
	rtk_set_lock(fd, F_UNLCK);	
	
	close(fd);	
	return 0;	
}

int rtk_output_dev_to_file(LAN_DEV_BASE_INFO *pDevInfo, int array_len)
{
	int i;
	char line_buffer[512]={0};
	int fd=0; 

	
	if((fd = open(LAN_DEV_LINK_INFO, O_RDWR|O_CREAT|O_TRUNC))<0)
		return -1;
	
	rtk_set_lock(fd, F_WRLCK);
	
	for(i=0;i<array_len; i++)
	{		
		memset(line_buffer, 0, sizeof(line_buffer));		
		
		snprintf(line_buffer, sizeof(line_buffer), "ip=%s,mac=%02X:%02X:%02X:%02X:%02X:%02X,dev_name=%s,conn_type=%d,link_time=%u,up_speed=%u,down_speed=%u,conn_port=%d,expires=%u,is_alive=%d\n",
		inet_ntoa(*((struct in_addr *)&pDevInfo[i].ip)),pDevInfo[i].mac[0],pDevInfo[i].mac[1],pDevInfo[i].mac[2],pDevInfo[i].mac[3],pDevInfo[i].mac[4],pDevInfo[i].mac[5],
		pDevInfo[i].dev_name,pDevInfo[i].conn_type,pDevInfo[i].link_time,pDevInfo[i].up_speed,pDevInfo[i].down_speed,pDevInfo[i].conn_port,pDevInfo[i].expires,pDevInfo[i].is_alive);  
		
		write(fd, line_buffer, strlen(line_buffer));
	}	
	
	rtk_set_lock(fd, F_UNLCK);	
	
	close(fd);
	return 0;
}

int rtk_check_port_speed()
{
	int i;
	
	unsigned int tx_bytes=0;
	unsigned int rx_bytes=0;	
	RTK_ETH_PORT_INFO cur_port_info[MAX_PORT_NUMBER];

	memset(cur_port_info, 0, sizeof(cur_port_info));
	
	for(i=0;i<MAX_PORT_NUMBER;i++)
	{		
		cur_port_info[i].port_num=i;
		rtk_ethernet_get_port_statistics(i, &tx_bytes, &rx_bytes);
		cur_port_info[i].rx_bytes=rx_bytes;
		cur_port_info[i].tx_bytes=tx_bytes;

		cur_port_info[i].down_speed=(cur_port_info[i].rx_bytes-last_port_info[i].rx_bytes)/CHECK_INTERVAL;		
		cur_port_info[i].up_speed=(cur_port_info[i].tx_bytes-last_port_info[i].tx_bytes)/CHECK_INTERVAL;

		//save cur info
		last_port_info[i].port_num=cur_port_info[i].port_num;
		last_port_info[i].rx_bytes=cur_port_info[i].rx_bytes;
		last_port_info[i].tx_bytes=cur_port_info[i].tx_bytes;
		last_port_info[i].down_speed=cur_port_info[i].down_speed;
		last_port_info[i].up_speed=cur_port_info[i].up_speed;		
	}
	
	//save info to file 
	rtk_output_port_info_to_file(last_port_info, MAX_PORT_NUMBER);

	return 0;
}

int rtk_check_dev_is_alive()
{
	int i, j;
	struct in_addr lan_addr;
	struct sockaddr hwaddr;
	unsigned char lan_mac[6];
	
	LAN_DEV_BASE_INFO cur_dev_info[MAX_TERM_NUMBER];	
	int cur_wired_dev_num=0;

	int port_num=0;
	
	memset(cur_dev_info, 0, sizeof(cur_dev_info));
	
	getInAddr(LAN_IFNAME, IP_ADDR_T, (void *)&lan_addr);
	getInAddr(LAN_IFNAME, HW_ADDR_T, (void *)&hwaddr);	
	memcpy(lan_mac, hwaddr.sa_data, 6);
	
	rtk_lan_get_wired_client_list(cur_dev_info, MAX_TERM_NUMBER, &cur_wired_dev_num);

	//printf("\ncur_wired_dev_num=%d\n",cur_wired_dev_num);
	
	for(i=0; i<cur_wired_dev_num; i++)
	{			
		for(j=0;j<SEND_ARP_NUM;j++)
		{				
			if(rtk_send_arp(cur_dev_info[i].ip, lan_addr.s_addr, lan_mac, LAN_IFNAME)==0)
				break;
		}
		if(j<SEND_ARP_NUM)
		{
			cur_dev_info[i].link_time=CHECK_INTERVAL;
			cur_dev_info[i].is_alive=1;
		}
		else
			cur_dev_info[i].is_alive=0;
		
		port_num=cur_dev_info[i].conn_port;
		
		cur_dev_info[i].down_speed=last_port_info[port_num].down_speed;
		cur_dev_info[i].up_speed=last_port_info[port_num].up_speed;
	}	
	
	for(i=0;i<cur_wired_dev_num;i++)
	{
		for(j=0;j<last_wired_dev_num;j++)
		{
			if(cur_dev_info[i].is_alive==1 && memcmp(cur_dev_info[i].mac, last_dev_info[j].mac, 6)==0 && cur_dev_info[i].ip==last_dev_info[j].ip)
			{
				cur_dev_info[i].link_time+=last_dev_info[j].link_time;
				break;
			}
		}
		if(j==last_wired_dev_num)
			cur_dev_info[i].link_time=0;
	}	
	memcpy(last_dev_info, cur_dev_info, sizeof(LAN_DEV_BASE_INFO)*cur_wired_dev_num);		
	last_wired_dev_num=cur_wired_dev_num;	

	rtk_output_dev_to_file(cur_dev_info, cur_wired_dev_num);

#if 0
	LAN_DEV_BASE_INFO wifiClient[128];
	int wifi_client_num=0;
	rtk_lan_get_wifi_client_info(wifiClient, 128, &wifi_client_num);

	LAN_DEV_BASE_INFO WiredClient[64];
	int wired_client_num=0;
	rtk_lan_get_wired_client_info(WiredClient, 64, &wired_client_num);
#endif
	
	return 0;
}

#endif



int get_eth1_link_status()
{
	char	tmpbuf;
	int		pressed = 0;
	FILE *fp;
	char line[20];

	if ((fp = fopen("/proc/eth1/link_status", "r")) != NULL) {
		fgets(line, sizeof(line), fp);
		if (sscanf(line, "%c", &tmpbuf))
			pressed = (tmpbuf == '0')? 0 : 1;
		else
			pressed = 0;
		fclose(fp);
	} 

	return pressed;
}

int is_vxd_connected()
{
	FILE *stream;
	char *strtmp;
	char line[100];
	int result=0, i = 0;
	unsigned char *p;
	unsigned char str[10];

	for(i = 0; i < 2; ++i){
		if(i == 0)
			stream = fopen ( "/proc/wlan0-vxd/sta_info", "r" );
		else
			stream = fopen ( "/proc/wlan1-vxd/sta_info", "r" );
		if ( stream != NULL ) {		
			while (fgets(line, sizeof(line), stream))
			{
				strtmp = line;
				while(*strtmp == ' ')
					strtmp++;
					
				if(strstr(strtmp,"active") != 0){
					sscanf(strtmp, "%*[^:]:%[^)]",str);
					p = str;
					while(*p == ' ')
						p++;										
					if(strcmp(p,"0") == 0){
						result=0;
					}else{
						result=1;	
						break;
					}										
				}
			}
			fclose(stream );
		}
		if(result == 1)
			return result;
	}
	return result;
}

int checkConnect(char *dst, int cnt)   
{      
    FILE *stream;   
    char recvBuf[16] = {0};   
    char cmdBuf[256] = {0};   
  
    if (NULL == dst || cnt <= 0)   
        return -1;   
  
    sprintf(cmdBuf, "ping %s -c %d | grep time= | wc -l", dst, cnt);   
    stream = popen(cmdBuf, "r");   
    fread(recvBuf, sizeof(char), sizeof(recvBuf)-1, stream);   
    pclose(stream);   
  
    if (atoi(recvBuf) > 0)   
    	return 0;   
  
    return -1;   
}   

int internet_status_led_new(void)
{
	if (wan_status == wan_last_status) return 0;
	wan_last_status = wan_status;
	printf("wan_status:%d\n",wan_status);
	if (!wan_status)  
		{
#if defined(CONFIG_MODEL_HN120_341BA)
		//wan led blue on
		system("echo 1 > /proc/gpio");
		
		//wan led Red off
		system("echo 34 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio34/direction"); 
		system("echo 1 > /sys/class/gpio/gpio34/value");		
#endif
#if defined(CONFIG_MODEL_HN120_341FA)
			//wan led blue on
			system("echo 1 > /proc/gpio");
			
			//wan led Red off
			system("echo 34 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio56/direction"); 
			system("echo 1 > /sys/class/gpio/gpio56/value");		
#endif

	}
	else
		{
#if defined(CONFIG_MODEL_HN120_341BA)
		//wan led blue off
		system("echo 0 > /proc/gpio");
		
		//wan led Red on
		system("echo 34 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio34/direction"); 
		system("echo 0 > /sys/class/gpio/gpio34/value");		
#endif
#if defined(CONFIG_MODEL_HN120_341FA)
			//wan led blue off
			system("echo 0 > /proc/gpio");
			
			//wan led Red on
			system("echo 34 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio56/direction"); 
			system("echo 0 > /sys/class/gpio/gpio56/value");		
#endif

	}

	wlan_idx=0; 
	int wlan_disable=0;
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);

	if(wlan_disable) {
#if defined(CONFIG_MODEL_HN120_341BA)
		//5G	Blue off
		system("echo 35 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio35/direction");
		system("echo 1 > /sys/class/gpio/gpio35/value");	
#endif
#if defined(CONFIG_MODEL_HN120_341FA)
		//5G	Blue off
		system("echo 35 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio35/direction");
		system("echo 1 > /sys/class/gpio/gpio35/value");	
#endif

	} else {
#if defined(CONFIG_MODEL_HN120_341BA)
		//5G	Blue on
		system("echo 35 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio35/direction");
		system("echo 0 > /sys/class/gpio/gpio35/value");	
#endif
#if defined(CONFIG_MODEL_HN120_341FA)
			//5G	Blue on
			system("echo 35 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio35/direction");
			system("echo 0 > /sys/class/gpio/gpio35/value");	
#endif
	}

	
#if defined(CONFIG_MODEL_HN120_341FA) || defined(CONFIG_MODEL_HN120_341BA)
	if(NUM_WLAN_INTERFACE==2)
		wlan_idx=1;
	else
		wlan_idx=0; 
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);

	if(wlan_disable) {
#if defined(CONFIG_MODEL_HN120_341BA)		
		//2.4G	Blue off
		system("echo 33 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio33/direction");
		system("echo 1 > /sys/class/gpio/gpio33/value");		
#endif
#if defined(CONFIG_MODEL_HN120_341FA)
			//5G	Blue on
			system("echo 35 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio35/direction");
			system("echo 0 > /sys/class/gpio/gpio35/value");	
#endif

	} else {
#if defined(CONFIG_MODEL_HN120_341BA)
		//2.4G	Blue on
		system("echo 33 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio33/direction");
		system("echo 0 > /sys/class/gpio/gpio33/value");	
#endif
#if defined(CONFIG_MODEL_HN120_341FA)
			//5G	Blue on
			system("echo 35 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio35/direction");
			system("echo 0 > /sys/class/gpio/gpio35/value");	
#endif
	}
#endif

	return 0;
}


int internet_status_led(void)
{
	if (wan_status == wan_last_status) return 0;
	wan_last_status = wan_status;

	wlan_idx=0; 
	vwlan_idx = 0;

	if (wan_status)  
	{
		int wlan_disable=0;
		apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);

	#if defined(CONFIG_MODEL_HN120_341FA)
		if(wlan_disable) {
			//5G Blue off
			system("echo 57 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio57/direction");
			system("echo 1 > /sys/class/gpio/gpio57/value");
	
			//5G Red off
			system("echo 58 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio58/direction"); 
			system("echo 1 > /sys/class/gpio/gpio58/value");
		} else {
			//5G Blue off
			system("echo 57 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio57/direction");
			system("echo 1 > /sys/class/gpio/gpio57/value");
	
			//5G Red on
			system("echo 58 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio58/direction"); 
			system("echo 0 > /sys/class/gpio/gpio58/value");
		}	
	#else		
		if(wlan_disable)
			system("echo 0 > /proc/wlan0/led");
		else
			system("echo 100 > /proc/wlan0/led");
	#endif

#if defined(CONFIG_MODEL_HN120_341FA) || defined(CONFIG_MODEL_HN120_341BA)  || defined (CONFIG_MODEL_HN140_361EA)
		if(NUM_WLAN_INTERFACE==2)
			wlan_idx=1;
		else
			wlan_idx=0; 
		apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);

	#if defined(CONFIG_MODEL_HN120_341FA)
		if(wlan_disable) {
			//2.4G	Blue off
			system("echo 55 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio55/direction");
			system("echo 1 > /sys/class/gpio/gpio55/value");
	
			//2.4G Red off
			system("echo 56 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio56/direction"); 
			system("echo 1 > /sys/class/gpio/gpio56/value");			
		} else {
			//2.4G	Blue off
			system("echo 55 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio55/direction");
			system("echo 1 > /sys/class/gpio/gpio55/value");
			
			//2.4G Red on
			system("echo 56 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio56/direction"); 
			system("echo 0 > /sys/class/gpio/gpio56/value");			
		}
	#else

		if(wlan_disable)
			system("echo 0 > /proc/wlan1/led");
		else
			system("echo 100 > /proc/wlan1/led");
	#endif
#endif
		DEBUG_TRACE("%s(%d):Network is not up to %s\n", __FUNCTION__, __LINE__, website);  
	} 
	else  
	{
		int wlan_disable=0;
		apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);
	#if defined(CONFIG_MODEL_HN120_341FA)
		if(wlan_disable) {
			//5G Blue off
			system("echo 57 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio57/direction");
			system("echo 1 > /sys/class/gpio/gpio57/value");
	
			//5G Red off
			system("echo 58 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio58/direction"); 
			system("echo 1 > /sys/class/gpio/gpio58/value");
		} else {
			//5G Blue on
			system("echo 57 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio57/direction");
			system("echo 0 > /sys/class/gpio/gpio57/value");
	
			//5G Red off
			system("echo 58 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio58/direction"); 
			system("echo 1 > /sys/class/gpio/gpio58/value");
		}	
	#else
		if(wlan_disable)
			system("echo 0 > /proc/wlan0/led");
		else
			system("echo 101 > /proc/wlan0/led");
	#endif
				
#if defined(CONFIG_MODEL_HN120_341FA) || defined(CONFIG_MODEL_HN120_341BA)||  defined (CONFIG_MODEL_HN140_361EA)
		if(NUM_WLAN_INTERFACE==2)
			wlan_idx=1;
		else
			wlan_idx=0; 
		//int wlan_disable=0;
		apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);

	#if defined(CONFIG_MODEL_HN120_341FA)
		if(wlan_disable) {
			//2.4G	Blue off
			system("echo 55 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio55/direction");
			system("echo 1 > /sys/class/gpio/gpio55/value");
	
			//2.4G Red off
			system("echo 56 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio56/direction"); 
			system("echo 1 > /sys/class/gpio/gpio56/value");			
		} else {
			//2.4G	Blue on
			system("echo 55 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio55/direction");
			system("echo 0 > /sys/class/gpio/gpio55/value");
			
			//2.4G Red off
			system("echo 56 > /sys/class/gpio/export");
			system("echo out > /sys/class/gpio/gpio56/direction"); 
			system("echo 1 > /sys/class/gpio/gpio56/value");			
		}
	#else
		if(wlan_disable)
			system("echo 0 > /proc/wlan1/led");
		else
			system("echo 101 > /proc/wlan1/led");
	#endif
#endif
		DEBUG_TRACE("%s(%d):Network is up to %s\n", __FUNCTION__, __LINE__, website);  
	}

	return 0;
}

#ifdef CALC_CPU_OCCUPY
static CPU_INFO_T cpu_info1, cpu_info2;	
#endif

#if defined(APP_WATCHDOG)
static int is_watchdog_alive(void)
{
	int is_alive = 0;
	int pid = -1;
	pid = find_pid_by_name("watchdog");
	if(pid > 0)
		is_alive = 1;
	return is_alive;
}

#endif

#if defined(CONFIG_APP_BOA_AUTO_RECOVER)
static int is_boa_alive(void)
{
	int pid = -1;
	pid = find_pid_by_name("boa");
	if(pid > 0)
		return 1;
	else
		return 0;
}
#endif


void wlan_get_button_state(int *pState)
{
   char tmpbuf;
   FILE *fp;
   char line[10];

   if((fp = fopen(WIFI_SWITCH_GPIO, "r")) != NULL){
      fgets(line, sizeof(line), fp);
	  if (sscanf(line, "%c", &tmpbuf)) {
		 if (tmpbuf == '0')
			*pState = 0;
		 else
			*pState = 1;
	  } else
		 *pState = 0;
		 fclose(fp);
   } else
	  *pState = 0;

}


#if defined(CONFIG_ANDLINK_SUPPORT)
static int is_andlink_alive(void)
{
	int pid = -1;
	pid = find_pid_by_name("andlink");
	if(pid > 0)
		return 1;
	else
		return 0;
}

static int andlink_enable_and_exit(void)
{
	 if((isFileExist(RTL_LINK_PID_FILE)) && (!is_andlink_alive()))
	    return 1;
	 else
	 	return 0;
}
#endif

#ifdef CALC_CPU_OCCUPY
unsigned int rtk_get_cpu_total_occupy(CPU_INFO_Tp pInfo)
{
	FILE *fh;
  	char buf[512]={0}, cpu_index[10];
	unsigned int d1, d2, d3, d4, d5, d6, d7, d8, d9, sum;

	fh = fopen("/proc/stat", "r");
	if (!fh) {
		printf("Warning: cannot open /proc/stat\n");
		return RTK_FAILED;
	}

	fgets(buf, sizeof(buf), fh);	
	sscanf(buf, "%*s%d %d %d %d %d %d %d %d %d",
					&d1, &d2, &d3, &d4, &d5, &d6, &d7, &d8, &d9);
	fclose(fh);	
	sum = d1+d2+d3+d4+d5+d6+d7+d8+d9;
	pInfo->cpu_total = sum;
	pInfo->cpu_idle = d4;
	pInfo->cpu_load = sum - d4;	

	//printf("cpu_total:%d, cpu_load:%d, cpu_idle:%d\n", pInfo->cpu_total, pInfo->cpu_load, pInfo->cpu_idle);	
	return RTK_SUCCESS;	
}

#endif

void timeout_handler() 
{
	char tmpBuf[128];
	int tmpValue;
	time_count++;
	if(!(time_count%1))
	{
		#if defined (CONFIG_MODEL_HN100_340EA) ||  defined (CONFIG_MODEL_HN120_341FA) ||  defined (CONFIG_MODEL_HN120_341BA) ||  defined (CONFIG_MODEL_HN140_361EA)
		internet_status_led_new();
		#elif defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)		
		#endif
		
#if defined(APP_WATCHDOG)
		if(is_watchdog_alive() == 0)
		{
			//printf("watchdog is not alive\n");
			system("watchdog 1000&");
		}
#endif

	}	
#if defined(CONFIG_APP_BOA_AUTO_RECOVER)
	if(!(time_count%CHECK_BOA_TIMEOUT))
	{
		if(!is_boa_alive())
		{
			system("boa");
		}
	}
#endif

#ifdef CONFIG_APP_RTK_ADAPTER
	if(!(time_count%CHECK_INTERVAL))
	{		
		rtk_check_port_speed();
		
		rtk_check_dev_is_alive();		

		//rtk_lan_update_accumulated_client_info(&ntp_difference);
	}
#endif

#ifdef CONFIG_CMCC_DM
	if(!(time_count%5))
	{		
		if(find_pid_by_name("CMCC_DM") <= 0)
		{
			char cmdline[128] = {0}, CMEI[32] = {0};
			apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)CMEI);
			if (strlen(CMEI))
				snprintf(cmdline, sizeof(cmdline), "CMCC_DM -m 0 -c %s &",
					CMEI);
			else
				snprintf(cmdline, sizeof(cmdline), "CMCC_DM -m 0 &");
			
			printf("Restart %s\n", cmdline);
			
			system(cmdline);
		}
	}
#endif


#ifdef CONFIG_ANDLINK_SUPPORT
	if(!(time_count%CHECK_ANDLINK_TIMEOUT))
	{		
		if(andlink_enable_and_exit())
		{
			system("andlink -m1 -d2");
		}
	}
#endif

#ifdef CONFIG_CMCC
	if (0 == strlen(telnet_disable) || 0 != strcmp(telnet_disable, "1"))
	{
		if(!(time_count%5))
		{		
			if(find_pid_by_name("telnetd") <= 0)
			{
				system("telnetd &");
			}
		}
	}
#endif

#ifdef CALC_CPU_OCCUPY
	if((time_count%CHECK_CPU_INTERVAL)==0)
	{
		memset(&cpu_info2, 0, sizeof(CPU_INFO_T));		
		rtk_get_cpu_total_occupy(&cpu_info2);
		tmpValue = (cpu_info2.cpu_load-cpu_info1.cpu_load)*100/(cpu_info2.cpu_total-cpu_info1.cpu_total);
		if(tmpValue==0)
			tmpValue = 1;
		snprintf(tmpBuf, sizeof(tmpBuf), "echo %d > %s", tmpValue, CPU_OCCUPY_FILE);		
		system(tmpBuf);
		//printf("===[%s %d]===cpu_load:%d, cpu_total:%d, occu:%d\n", __FUNCTION__,__LINE__,
		//	(cpu_info2.cpu_load-cpu_info1.cpu_load), (cpu_info2.cpu_total-cpu_info1.cpu_total), tmpValue);		
	}else if((time_count%CHECK_CPU_INTERVAL)==1)
	{
		memset(&cpu_info1, 0, sizeof(CPU_INFO_T));
		rtk_get_cpu_total_occupy(&cpu_info1);
	}
#endif

	if(!(time_count%60))
 	{

	}
 	alarm(1);
}

void disable_wlan_state(int val_temp)
{
	int  i;
	char wlan_name[16] = {0};
	char virtual_interface[64] = {0};
	char *token = NULL, *savestr1=NULL;
	char tmpBuff[64];
	
	for (i = 0; i < NUM_WLAN_INTERFACE; i++){
		sprintf(wlan_name, "wlan%d",i);
      	SetWlan_idx(wlan_name);
      	apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&val_temp);

		/*down vxd interface*/
		sprintf(wlan_name,"wlan%d-vxd",i);
		SetWlan_idx(wlan_name);
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&val_temp);

		/*down vap interface*/
      	sprintf(wlan_name,"wlan%d-va",i);
      	bzero(virtual_interface,sizeof(virtual_interface));
		if_readlist_proc(virtual_interface, wlan_name, 0);
		if (virtual_interface[0]){
			token = NULL;
			savestr1=NULL;
			sprintf(tmpBuff, "%s", virtual_interface);
			token = strtok_r(tmpBuff," ", &savestr1);
			do{
				if (token == NULL){
					break;
				}else{
					SetWlan_idx(token);
					apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&val_temp);
				}
				token = strtok_r(NULL, " ", &savestr1);
			}while(token != NULL);
		}
	}
}


void change_wlan_state()
{
	int pState,val[NUM_WLAN_INTERFACE],i,val_temp;
	char tmpBuf[128];
	char wlan_name[16] = {0};
	
	wlan_get_button_state (&pState);
	
	if (pState == 1){
		
		sprintf(tmpBuf, "echo 0 > %s", WIFI_SWITCH_GPIO);
		system(tmpBuf);
		
		apmib_save_wlanIdx();

      	for (i = 0; i < NUM_WLAN_INTERFACE; i++){
      		sprintf(wlan_name, "wlan%d",i);
      		SetWlan_idx(wlan_name);
      		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&val[i]);
      		if (val[i] == 0){
				val_temp = 1;
      			disable_wlan_state(val_temp);
      			break;
      		}else{
      			val_temp = 0;
      			apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&val_temp);
			  }	
		}
		  
		apmib_recov_wlanIdx();
		
		apmib_update(CURRENT_SETTING);
	
		sprintf(tmpBuf, "%s", "init.sh gw all");
		system(tmpBuf);
	}	
}

void clean_dhcp_leases(void)
{
	if(access("/var/lib/misc/udhcpd.leases",F_OK) == 0) {
		system("rm /var/lib/misc/udhcpd.leases");
	}
	
	return;
}

int main(int argc, char** argv)
{	
	char	line[300];
	char action[16];
	int i;

	unsigned int count;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &set, NULL);

	if ( !apmib_init()) {
		printf("Initialize AP MIB failed !\n");
		return -1;
	}	
	signal(SIGALRM,timeout_handler);
	alarm(1);
	
	int op_mode,lan_dhcp_mode=0,access=0,access_bak=0;
	apmib_get(MIB_DHCP,(void *)&lan_dhcp_mode);
	apmib_get(MIB_OP_MODE, &op_mode);
#ifdef CONFIG_CMCC
	apmib_get(MIB_CUSTOMER_HW_TELNETD_DISABLE,(void*)telnet_disable);
#endif
	
#ifdef CONFIG_APP_BOA_HE_UI
	apmib_get(MIB_SITE_ACCESS, &access_bak);
#endif
#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	system("FTY_TEST power on");
#endif
	
#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	int wlan_disable=0;
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void*)&wlan_disable);
	if(wlan_disable){
		system("FTY_TEST wifi off");
	} else {
		system("FTY_TEST wifi on");
	}
#endif
			
	while(1)
	{	

		count++;
		if(!(count%10))
		{
			if(op_mode == BRIDGE_MODE && lan_dhcp_mode==DHCP_LAN_CLIENT)
			{
				clean_dhcp_leases();
			}
		
//			if(op_mode == GATEWAY_MODE)
			{
	//			if(get_eth1_link_status()){
					wan_status=checkConnect(website, 3);
					DEBUG_TRACE("%s(%d):DUT is GATEWAY mode,ping test check internet status,wan_status=%d \n", __FUNCTION__, __LINE__,wan_status);
	//			}

			}/*else if(op_mode == BRIDGE_MODE)
			{
					//wan_status=checkConnect(website, 3);
					wan_status=0;
					DEBUG_TRACE("%s(%d):DUT is BRIDGE mode,ping test check internet status,wan_status=%d \n", __FUNCTION__, __LINE__,wan_status);
			}
			else 
			{
				DEBUG_TRACE("%s(%d):DUT is bridge mode or WISP mode,ping test check wlan_vxd internet status \n", __FUNCTION__, __LINE__);
				if(is_vxd_connected())
				{ 
					wan_status=checkConnect(website, 3);
					DEBUG_TRACE("%s(%d):wlan_vxd is connected successful,wan_status=%d \n", __FUNCTION__, __LINE__,wan_status);
				}
				else 
				{
					DEBUG_TRACE("%s(%d):wlan_vxd is connected fail \n", __FUNCTION__, __LINE__);
					wan_status=1;
				}
			}*/
		}

		//±ÜÃâcountÒç³ö
		if (count >= 65535) count = 0;
		
		access = ( wan_status ? 0 : 1 );
		if(access_bak != access){
#ifdef CONFIG_APP_BOA_HE_UI
			apmib_set(MIB_SITE_ACCESS,(void*)&access);
#endif
			access_bak = access;
		}

		#ifdef SEND_GRATUITOUS_ARP
		checkWanStatus();
		#endif
		change_wlan_state();
		#ifdef CONFIG_IPV6
		checkWanLinkStatus();
		#endif
		sleep(1);
	}
}

