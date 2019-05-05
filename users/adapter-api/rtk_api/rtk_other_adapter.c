/*STATS API*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/sysinfo.h>
#include <sys/select.h>
#include <sys/times.h>
#include <setjmp.h>
#include <time.h>
#include <sys/stat.h>
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "rtk_api.h"
#include "rtk_adapter.h"
#ifdef CONFIG_APP_FWD
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define FWD_CONF "/var/fwd.conf"

#endif

#ifdef HAVE_RTK_APMIB
#define TYPE_INT 0
#define TYPE_ARRAY 1
#define TYPE_TBL 2
#endif

extern char *fwVersion;


#define URL_LIST_FILE "/var/urllist"
#define WAN_STATUS_FILE "/var/wan_status"
#define RES_LOCK_FILE "/var/wan_res_lock"
#define LIST_LOCK_FILE "/var/url_list_lock"


int rtk_file_lock(int fd)
{
	int ret;
	struct flock lock;
	lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
try_again:
	ret = fcntl(fd,F_SETLKW,&lock);
     if(ret == -1) {
		if (errno == EINTR) {
			goto try_again;
		}
	}
	return ret;
}

int rtk_file_unlock(int fd)
{
	close(fd);
}
int equal_mac(unsigned char mac1[6],unsigned char mac2[6])
{
	if((mac1[0]==mac2[0])&& (mac1[1]==mac2[1])&&(mac1[2]==mac2[2])&&
	   (mac1[3]==mac2[3])&& (mac1[4]==mac2[4])&&(mac1[5]==mac2[5]))
	   return RTK_SUCCESS;
	return RTK_FAILED;
}

void format_mac(unsigned char str_mac[6],unsigned char *mac)
{
	int i,j;
	i = 0;
	j = 0;
	while(str_mac[i] != 0)
	{
		if(str_mac[i]>='0' && str_mac[i]<='9')
		{
			mac[j] = mac[j]*16 + str_mac[i] - '0';
		}
		else if(str_mac[i]>='a' && str_mac[i]<='f')
		{
			mac[j] = mac[j]*16 + str_mac[i] - 'a' + 10;
		}
		else if(str_mac[i]>='A' && str_mac[i]<='F')
		{
			mac[j] = mac[j]*16 + str_mac[i] - 'A' + 10;
		}
		else if(str_mac[i] == ':')
		{
			j++;
		}
		i++;
	}
}
/*
	get port status info by proc/rtl865x/asicCounter
*/
void GetPortStatus(int port_number,struct rtk_link_type_info *info)
{
	/*fill cur_rx /cur_tx parememter */
	FILE *fp=NULL;
	int  line_cnt =0;
	unsigned char buffer[128];
	//system("cat /proc/rtl865x/asicCounter  > /var/tmpResult");	
	
	//if((fp = fopen("/var/tmpResult","r+")) != NULL)
	if((fp = fopen("/proc/rtl865x/asicCounter","r+")) != NULL)
	{
		while(fgets(buffer, 128, fp))
		{
			line_cnt++;
			if(line_cnt == 12*port_number+3)	//update receive bytes
			{
				sscanf(buffer," Rcv %u ",&(info->cur_rx_bytes));
			}
			
			if(line_cnt == 12*port_number+10)	//update send bytes
			{
				sscanf(buffer," Snd %u ",&(info->cur_tx_bytes));
				fclose(fp);
				return ;
			}
		}
	}
	fclose(fp);
}

int check_layer2(unsigned char mac[6],struct rtk_link_type_info *info)
{
	FILE *fp = NULL;
	unsigned char mac_str[32],buffer[128];
	memset(mac_str,0,32);

	#if 0
	printf("%s.%d. mac(%02x:%02x:%02x:%02x:%02x:%02x)\n",
		__FUNCTION__,__LINE__,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	#endif
	
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	//system("cat /proc/rtl865x/l2 > /var/tmpResult");	

	//if((fp = fopen("/var/tmpResult","r+")) != NULL)
	if((fp = fopen("/proc/rtl865x/l2","r+")) != NULL)
	{	
		while(fgets(buffer, 128, fp))
		{
			if(strstr(buffer,mac_str))
			{
				unsigned char *p;
				p = strstr(buffer,"mbr");
				sscanf(p,"mbr(%d",&(info->port_number));
				fclose(fp);
				GetPortStatus(info->port_number,info);
				return RTK_SUCCESS;
			}
		}
		fclose(fp);
	}
	return RTK_FAILED;
}

int check_wifi(char *wifi_interface, unsigned char mac[6],struct rtk_link_type_info *info)
{
	int sta_num = 0 ;
	RTK_WLAN_STA_INFO_Tp pInfo;
	int i;
	char *buff;
	#if 0
	printf("%s.%d. mac(%02x:%02x:%02x:%02x:%02x:%02x)\n",
	__FUNCTION__,__LINE__,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);	
	#endif
	
	if(!getWlStaNum(wifi_interface,&sta_num))
	{
		if(sta_num <= 0)
			return RTK_FAILED;
	}
	else		
		return RTK_FAILED;
	
	buff = malloc(sizeof(RTK_WLAN_STA_INFO_T) * (MAX_STA_NUM+1));
	if(!buff)
		return RTK_FAILED;
	
	memset(buff,0,sizeof(RTK_WLAN_STA_INFO_T) * (MAX_STA_NUM +1));	
	if(rtk_get_wlan_sta(wifi_interface,(RTK_WLAN_STA_INFO_Tp)buff) == RTK_SUCCESS)
	{		
		for(i = 1; i <= sta_num; ++i)
		{
			pInfo = (RTK_WLAN_STA_INFO_Tp)&buff[i*sizeof(RTK_WLAN_STA_INFO_T)];
			if(equal_mac(mac,pInfo->addr)==RTK_SUCCESS)
			{				
				info->cur_rx_bytes = pInfo->rx_bytes;
				info->cur_tx_bytes = pInfo->tx_bytes;
                free(buff);
				return RTK_SUCCESS;
			}
		}
	}	
	free(buff);
	return RTK_FAILED;
}
/*get arp info from proc/net/arp */
int getArpInfo(struct rtk_link_type_info *info,int *term_number,int entry_number)
{	
	int count =0;
	int term_index = 0 ;
	FILE *fp = NULL;
	unsigned char buffer[128],ip_str[32],mac_str[32],interface_name[12];

	///////// modify by chenbo(realtek)
	struct in_addr br0_ip, br0_netmask, br0_netip;
	//apmib_get(MIB_IP_ADDR, (void *)&br0_ip);
	rtk_getInAddr("br0", IP_ADDR_T, (void *)&br0_ip);	
	//apmib_get(MIB_SUBNET_MASK, (void *)&br0_netmask);
	rtk_getInAddr("br0", NET_MASK_T, (void *)&br0_netmask);
	br0_netip.s_addr=br0_ip.s_addr & br0_netmask.s_addr;
	/////////
	
	//system("cat /proc/net/arp  > /var/tmpResult");
	//if((fp = fopen("/var/tmpResult","r+")) != NULL)
	if((fp = fopen("/proc/net/arp","r+")) != NULL)
	{
		while(fgets(buffer, 128, fp))
		{
			++count;
			if(count ==1)	continue;
			
			sscanf(buffer,"%s %*s %*s %s %*s %s",ip_str,mac_str,interface_name);
			//printf("%s.%d. ip_str(%s)\n",__FUNCTION__,__LINE__,ip_str);
			//printf("%s.%d. mac_str(%s)\n",__FUNCTION__,__LINE__,mac_str);	
			//printf("%s.%d. interface_name(%s)\n",__FUNCTION__,__LINE__,interface_name);
			if(strcmp(interface_name,"br0")==0)
			if(strcmp(mac_str,"00:00:00:00:00:00")!=0)
			{
				inet_aton(ip_str, &(info[term_index].ip));	
				//modify by chenbo(realtek)
				if((info[term_index].ip.s_addr & br0_netmask.s_addr)!=br0_netip.s_addr)
					continue;
				
				format_mac(mac_str,&(info[term_index].mac));
				++term_index;				
				if(term_index >= entry_number)
					break;
				//printf("%s,%d.update success\n",__FUNCTION__,__LINE__);
			}
		}
		fclose(fp);
		if(term_index != 0)
		{
			*term_number = term_index;
			return RTK_SUCCESS;
		}
	}	
	return RTK_FAILED;
}

void dumpAllTerminalInfo(int term_number,struct rtk_link_type_info *info)
{
	int i ;
	for(i = 0 ; i < term_number; ++i)
	{
		printf("\n***********************************\n");
		printf("LINK_TYPE(%d),ipAddr(%s),mac(%02x:%02x:%02x:%02x:%02x:%02x),\
			last_rx_bytes(%u),last_tx_bytes(%u),cur_rx_bytes(%u),cur_tx_bytes(%u),\
			port_number(%d),download_speed(%u),upload_speed(%u),all_bytes(%u),brand(%s),link_time(%u)\n",
			info[i].type,inet_ntoa(info[i].ip),
			info[i].mac[0],info[i].mac[1],info[i].mac[2],
			info[i].mac[3],info[i].mac[4],info[i].mac[5],
			info[i].last_rx_bytes,info[i].last_tx_bytes,
			info[i].cur_rx_bytes,info[i].cur_tx_bytes,
			info[i].port_number,info[i].download_speed,info[i].upload_speed,
			info[i].all_bytes,info[i].brand,info[i].link_time);
		
		printf("\n***********************************\n");		
	}
}

static int getWanIf(char * wanIf)
{
	int wisp_id=0, wan_mode=0, opmode=0;
	
#ifdef CONFIG_SMART_REPEATER
	int rptEnabled=0;
#endif
	if(!wanIf){
		return RTK_FAILED;
	}
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	switch(opmode)
	{
		case GATEWAY_MODE:
				sprintf(wanIf,"%s","eth1");
			break;
		case WISP_MODE:
				apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_id);				
				sprintf(wanIf,"wlan%d",wisp_id);
#ifdef CONFIG_SMART_REPEATER
				if(wisp_id == 0)
					apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
				else if(wisp_id == 1)
					apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
				if(rptEnabled)
				{
					sprintf(wanIf,"wlan%d-vxd",wisp_id);
				}
#endif				
			break;
		default:
			return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

static uint16_t in_cksum(uint16_t *addr, int len)  
{
	int nleft = len;
	uint32_t sum = 0;
	uint16_t *w = addr;
	uint16_t answer = 0;
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}  

	if (nleft == 1) {
		*(unsigned char *)(&answer) = *(unsigned char *)w ;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return(answer);  
}

static int getFileLineValue(char *line, unsigned int *value)
{
	char string[64];
	unsigned char *p;
	
	sscanf(line, "%*[^:]:%[^?]",string);
	p=string;
	
	while(*p == ' ')
		p++;

	*value = atoi(p);
	return 0;
}

static int send_echo_req(int sockfd, struct sockaddr_in *dstaddr)
{
	char buf[100];
	size_t len = sizeof(struct icmp);
	struct icmp *icmp;
	socklen_t dstlen = sizeof(struct sockaddr_in);

	bzero(buf, sizeof(buf));
	icmp = (struct icmp *)buf;
	icmp->icmp_type = ICMP_ECHO;//8
	icmp->icmp_code = 0;
	icmp->icmp_id = getpid();
	icmp->icmp_seq = 1;
	icmp->icmp_cksum = in_cksum((uint16_t *) icmp, sizeof(struct icmp));
	if (sendto(sockfd, buf, len, 0, (struct sockaddr *)dstaddr, dstlen) == -1)
	{
		printf("Error: send failed!\n");
		return RTK_FAILED;
	}

	return RTK_SUCCESS;
}

static int recv_echo_reply(int sockfd)  
{
	char buf[100];
	ssize_t n;
	int i = 0;
	int rec_flag = 0;
	struct ip *ip;
	struct icmp *icmp;
	fd_set fds;
    struct timeval timeout={5,0};
	
	while (1) {
		FD_ZERO(&fds);
		FD_SET(sockfd,&fds);
		rec_flag = 0;
		switch(select(sockfd+1, &fds, NULL, NULL, &timeout))
		{
		case -1:
			printf("Error: select error!\n");
			return RTK_FAILED;
		case 0:
			break;
		default:
			rec_flag = 1;
		}
		if(rec_flag == 1 && FD_ISSET(sockfd, &fds))
		{
			if ((n = read(sockfd, buf, sizeof(buf))) == -1)
			{
				printf("Error: read fail!\n");
				return RTK_FAILED;
			}
			
			ip = (struct ip *)buf;

			if (ip->ip_p != IPPROTO_ICMP) {
				printf("Error: protocol error.\n");
				return RTK_FAILED;
			}

			icmp = (struct icmp *)(buf + sizeof(struct ip));

			if (icmp->icmp_type == ICMP_ECHOREPLY) {
				if (icmp->icmp_id != getpid()) {
				printf("Error: not this process.\n");
					return RTK_FAILED;
				} else {
					//printf("destination host is alive.\n");
					return RTK_SUCCESS;
				}
			}
		}

		if(++i > 3)
		{
			printf("Error: Time out.\n");
			return RTK_FAILED;
		}
	}  
}

static int apmib_update_web(int type)
{
	int ret = apmib_update(type);

	if (ret == 0)
		return 0;

	if (type & CURRENT_SETTING) {
		save_cs_to_file();
	}
	return ret;
}

static int get_dev_fields(int type, char *bp, RTK_NET_DEVICE_STATSp pStats)
{
	switch (type) {
	case 3:
		sscanf(bp,
			"%*s %llu %llu %lu %lu %lu %lu %lu %lu %llu %llu %lu %lu %lu %lu %lu %lu",
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
		sscanf(bp, "%*s %llu %llu %lu %lu %lu %lu %llu %llu %lu %lu %lu %lu %lu",
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
		sscanf(bp, "%*s %llu %lu %lu %lu %lu %llu %lu %lu %lu %lu %lu",
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
	return RTK_SUCCESS;
}


static get_wlan_bss_info(char *iface, RTK_BSS_INFOp pInfo)
{
}

int rtk_get_net_stats(char *interface, RTK_NET_DEVICE_STATSp pStats)
{
}

rtk_get_eth_port_status(int port, RTK_ETH_PORT_STATUSp port_status)
{
}


int rtk_get_sys_info(RTK_SYSTEM_INFOp info)
{
}

/*need detailed info*/
int rtk_set_opmode(unsigned int mode);
int rtk_set_ddns();
int rtk_set_timezone();
int rtk_set_dos();

int rtk_set_time(struct rtk_time time)
{
    struct tm *tm_time;
    int time_value;
    
    if(stime(&time.tm) < 0){
		printf("ERROR (%s)%d set Time Error\n", __FUNCTION__, __LINE__);
        return RTK_FAILED;
	}

	tm_time = localtime(&time.tm);
	time_value = tm_time->tm_year+ 1900;
	apmib_set( MIB_SYSTIME_YEAR, (void *)&time_value);
	time_value = tm_time->tm_mon + 1;
	apmib_set( MIB_SYSTIME_MON, (void *)&time_value);
	time_value = tm_time->tm_mday;
	apmib_set( MIB_SYSTIME_DAY, (void *)&time_value);
	time_value = tm_time->tm_hour;
	apmib_set( MIB_SYSTIME_HOUR, (void *)&time_value);
	time_value = tm_time->tm_min;
	apmib_set( MIB_SYSTIME_MIN, (void *)&time_value);
	time_value = tm_time->tm_sec;
	apmib_set( MIB_SYSTIME_SEC, (void *)&time_value);

    apmib_set(MIB_NTP_TIMEZONE, (void *)time.tm_zone);

    return RTK_SUCCESS;
}

int rtk_get_dut_info(struct rtk_dut_info *info)
{
    struct in_addr ip;
    char ifname[16], ifname_2g[16], ifname_5g[16];
    int i, enable, rf_band;

    info->enable_2g = WIFI_STATUS_NOT_SUPPORT;
    info->enable_5g = WIFI_STATUS_NOT_SUPPORT;
    
    if(NUM_WLAN_INTERFACE == 2)
        info->band = DUALBAND;
    
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
        snprintf(ifname, sizeof(ifname), "wlan%d", i);
        rtk_wlan_get_rf_band(ifname, &rf_band);
        rtk_wlan_get_enable(ifname, &enable);
        if(rf_band == PHYBAND_2G){
            snprintf(ifname_2g, sizeof(ifname_2g), "%s", ifname);
            info->enable_2g = enable;
            if(NUM_WLAN_INTERFACE == 1)
                info->band = BAND2G;
        }else{
            snprintf(ifname_5g, sizeof(ifname_5g), "%s", ifname);
            info->enable_5g = enable;
            if(NUM_WLAN_INTERFACE == 1)
                info->band = BAND5G;
        }
    }

    rtk_wlan_get_enable_repeater(ifname_2g, &info->repeater_enable_2g);
    rtk_wlan_get_enable_repeater(ifname_5g, &info->repeater_enable_5g);
    
    if(!apmib_get(MIB_OP_MODE, (void *)&info->opmode))
        return RTK_FAILED;
    if(info->opmode == BRIDGE_MODE){
        apmib_get(MIB_REPEATER_MODE, (void *)&info->repeater_mode);
    }
    if(!apmib_get(MIB_WISP_WAN_ID, (void *)&info->wisp_idx))
        return RTK_FAILED;

#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT) || defined(CONFIG_APP_CMCC_ADAPTER)    
    if(!apmib_get(MIB_RTL_LINK_VENDOR, (void *)info->vendor))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_DEVMODEL, (void *)info->model))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_HWVERSION, (void *)info->hwversion))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_SWVERSION, (void *)info->swversion))
        return RTK_FAILED;
#ifdef CUSTOMER_HW_SETTING_SUPPORT
    if(!apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)info->sn))
        return RTK_FAILED;
#else
    if(!apmib_get(MIB_RTL_LINK_SN, (void *)info->sn))
        return RTK_FAILED;
#endif
    if(!apmib_get(MIB_RTL_LINK_SYNCCODE, (void *)&info->synccode))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_DEVTYPE, (void *)info->deviceType))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_PRODTOKEN, (void *)info->productToken))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_DEVID, (void *)info->deviceId))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_DEVTOKEN, (void *)info->deviceToken))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_ANDLINKTOKEN, (void *)info->andlinkToken))
        return RTK_FAILED;
    if(!info->andlinkToken[0])
        strcpy(info->andlinkToken, "01234567890123456789012345678901");
    if(!apmib_get(MIB_RTL_LINK_HBTIME, (void *)&info->HBTime))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_NEED_RECOVERY, (void *)&info->need_recover))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_SEARCH_GW_MAX_RETRY, (void *)&info->search_gw_max_retry))
        return RTK_FAILED;
    if(!apmib_get(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip))
        return RTK_FAILED;
    strcpy(info->gwIp, inet_ntoa(ip));
#endif
    return RTK_SUCCESS;
}

static int updateConfigIntoFlash(unsigned char *data, int total_len, int *pType, int *pStatus)
{
	int len=0, status=1, type=0, ver, force;
#ifdef HEADER_LEN_INT
		HW_PARAM_HEADER_Tp phwHeader;
		int isHdware=0;
#endif
	PARAM_HEADER_Tp pHeader;
#ifdef COMPRESS_MIB_SETTING
	COMPRESS_MIB_HEADER_Tp pCompHeader;
	unsigned char *expFile=NULL;
	unsigned int expandLen=0;
	int complen=0;
#endif
	char *ptr;

	do {
#ifdef COMPRESS_MIB_SETTING
		pCompHeader =(COMPRESS_MIB_HEADER_Tp)&data[complen];
#ifdef _LITTLE_ENDIAN_
		pCompHeader->compRate = WORD_SWAP(pCompHeader->compRate);
		pCompHeader->compLen = DWORD_SWAP(pCompHeader->compLen);
#endif
		/*decompress and get the tag*/
		expFile=malloc(pCompHeader->compLen*pCompHeader->compRate);
		if(NULL==expFile)
		{
			
			return 0;
		}
		expandLen = Decode(data+complen+sizeof(COMPRESS_MIB_HEADER_T), pCompHeader->compLen, expFile);
		pHeader = (PARAM_HEADER_Tp)expFile;
#ifdef HEADER_LEN_INT

		if (memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN)==0 ||
			memcmp(pHeader->signature, HW_SETTING_HEADER_FORCE_TAG, TAG_LEN)==0 ||
			memcmp(pHeader->signature, HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN)==0 )
			{
				isHdware=1;
				phwHeader=(HW_PARAM_HEADER_Tp)expFile;
			}			
#endif
#else
#ifdef HEADER_LEN_INT
		if(isHdware)
			phwHeader=(HW_PARAM_HEADER_Tp)&data[len];
		else
#endif
		pHeader = (PARAM_HEADER_Tp)&data[len];
#endif
		
#ifdef _LITTLE_ENDIAN_
#ifdef HEADER_LEN_INT
		if(isHdware)
			phwHeader->len = WORD_SWAP(phwHeader->len);
		else
#endif
		pHeader->len = HEADER_SWAP(pHeader->len);
#endif
#ifdef HEADER_LEN_INT
		if(isHdware)
			len += sizeof(HW_PARAM_HEADER_T);
		else
#endif
		len += sizeof(PARAM_HEADER_T);

		if ( sscanf(&pHeader->signature[TAG_LEN], "%02d", &ver) != 1)
			ver = -1;
			
		force = -1;
		if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN) )
			force = 1; // update
		else if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_FORCE_TAG, TAG_LEN))
			force = 2; // force
		else if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN))
			force = 0; // upgrade

		if ( force >= 0 ) {
#if 0
			if ( !force && (ver < CURRENT_SETTING_VER || // version is less than current
				(pHeader->len < (sizeof(APMIB_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif

#ifdef COMPRESS_MIB_SETTING
#ifdef HEADER_LEN_INT
			if(isHdware)
				ptr = expFile+sizeof(HW_PARAM_HEADER_T);
			else
#endif
			ptr = expFile+sizeof(PARAM_HEADER_T);
#else
			ptr = &data[len];
#endif

#ifdef COMPRESS_MIB_SETTING
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				DECODE_DATA(ptr, phwHeader->len);
			else
#endif
			DECODE_DATA(ptr, pHeader->len);
#endif
			
#ifdef HEADER_LEN_INT
			if(isHdware)
			{
				if ( !CHECKSUM_OK(ptr, phwHeader->len)) {
				status = 0;
				break;
				}
			}
			else
#endif
			if ( !CHECKSUM_OK(ptr, pHeader->len)) {
				status = 0;
				break;
			}

//#ifdef _LITTLE_ENDIAN_
//			swap_mib_word_value((APMIB_Tp)ptr);
//#endif

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
		#ifndef VOIP_SUPPORT_TLV_CFG
			flash_voip_import_fix(&((APMIB_Tp)ptr)->voipCfgParam, &pMib->voipCfgParam);
#endif
#endif

#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(CURRENT_SETTING, &data[complen], pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				apmib_updateFlash(CURRENT_SETTING, ptr, phwHeader->len-1, force, ver);
			else
#endif
			apmib_updateFlash(CURRENT_SETTING, ptr, pHeader->len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			complen += pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T);
			if(expFile)
			{
				free(expFile);
				expFile=NULL;
			}
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				len += phwHeader->len;
			else
#endif
			len += pHeader->len;
#endif
			type |= CURRENT_SETTING;
			continue;
		}


		if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_TAG, TAG_LEN) )
			force = 1;	// update
		else if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
			force = 2;	// force
		else if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) )
			force = 0;	// upgrade

		if ( force >= 0 ) {
#if 0
			if ( (ver < DEFAULT_SETTING_VER) || // version is less than current
				(pHeader->len < (sizeof(APMIB_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif

#ifdef COMPRESS_MIB_SETTING
#ifdef HEADER_LEN_INT
			if(isHdware)
				ptr = expFile+sizeof(HW_PARAM_HEADER_T);
			else
#endif
			ptr = expFile+sizeof(PARAM_HEADER_T);
#else
			ptr = &data[len];
#endif

#ifdef COMPRESS_MIB_SETTING
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				DECODE_DATA(ptr, phwHeader->len);
			else
#endif
			DECODE_DATA(ptr, pHeader->len);
#endif
#ifdef HEADER_LEN_INT
			if(isHdware)
			{
				if ( !CHECKSUM_OK(ptr, phwHeader->len)) {
				status = 0;
				break;
				}
			}
			else
#endif
			if ( !CHECKSUM_OK(ptr, pHeader->len)) {
				status = 0;
				break;
			}

//#ifdef _LITTLE_ENDIAN_
//			swap_mib_word_value((APMIB_Tp)ptr);
//#endif

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
		#ifndef VOIP_SUPPORT_TLV_CFG
			flash_voip_import_fix(&((APMIB_Tp)ptr)->voipCfgParam, &pMibDef->voipCfgParam);
#endif
#endif

#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(DEFAULT_SETTING, &data[complen], pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				apmib_updateFlash(DEFAULT_SETTING, ptr, phwHeader->len-1, force, ver);
			else
#endif
			apmib_updateFlash(DEFAULT_SETTING, ptr, pHeader->len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			complen += pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T);
			if(expFile)
			{
				free(expFile);
				expFile=NULL;
			}	
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				len += phwHeader->len;
			else
#endif
			len += pHeader->len;
#endif
			type |= DEFAULT_SETTING;
			continue;
		}

		if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN) )
			force = 1;	// update
		else if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
			force = 2;	// force
		else if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) )
			force = 0;	// upgrade

		if ( force >= 0 ) {
#if 0
			if ( (ver < HW_SETTING_VER) || // version is less than current
				(pHeader->len < (sizeof(HW_SETTING_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif
#ifdef COMPRESS_MIB_SETTING
#ifdef HEADER_LEN_INT
			if(isHdware)
				ptr = expFile+sizeof(HW_PARAM_HEADER_T);
			else
#endif
			ptr = expFile+sizeof(PARAM_HEADER_T);
#else
			ptr = &data[len];
#endif
			

#ifdef COMPRESS_MIB_SETTING
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				DECODE_DATA(ptr, phwHeader->len);
			else
#endif
			DECODE_DATA(ptr, pHeader->len);
#endif
#ifdef HEADER_LEN_INT
			if(isHdware)
			{
				if ( !CHECKSUM_OK(ptr, phwHeader->len)) {
				status = 0;
				break;
				}
			}
			else
#endif
			if ( !CHECKSUM_OK(ptr, pHeader->len)) {
				status = 0;
				break;
			}
#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(HW_SETTING, &data[complen], pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				apmib_updateFlash(HW_SETTING, ptr, phwHeader->len-1, force, ver);
			else
#endif
			apmib_updateFlash(HW_SETTING, ptr, pHeader->len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			complen += pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T);
			if(expFile)
			{
				free(expFile);
				expFile=NULL;
			}
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				len += phwHeader->len;
			else
#endif
			len += pHeader->len;
#endif

			type |= HW_SETTING;
			continue;
		}
	}
#ifdef COMPRESS_MIB_SETTING	
	while (complen < total_len);
#else
	while (len < total_len);
#endif
	if(expFile)
	{
		free(expFile);
		expFile=NULL;
	}

	*pType = type;
	*pStatus = status;
#ifdef COMPRESS_MIB_SETTING	
	return complen;
#else
	return len;
#endif
}
int fwChecksumOk(char *data, int len)
{
	unsigned short sum=0;
	int i;

	for (i=0; i<len; i+=2) {
#ifdef _LITTLE_ENDIAN_
		sum += WORD_SWAP( *((unsigned short *)&data[i]) );
#else
		sum += *((unsigned short *)&data[i]);
#endif

	}
	return( (sum==0) ? 1 : 0);
}

int rtl_get_file_via_url(unsigned char* url,unsigned char* dstName)
{
	unsigned char command[128]={0};
	if(!url || !url[0] || !dstName || !dstName[0]) return RTK_FAILED;
	
	snprintf(command,sizeof(command),"wget %s -O %s",url,dstName);
	system(command);

	return RTK_SUCCESS;
}
static int daemonKilled = 0;
//static void killDaemon()
void killDaemon(int wait)
{
#ifndef WIFI_SIMPLE_CONFIG
    int pid;
#endif
		
    if (daemonKilled)
    	return;

    daemonKilled = 1;

#if 1	
	FILE *stream;
	system("ps > /var/alive_daemons");
	stream = fopen ( "/var/alive_daemons", "r" );
	if ( stream != NULL ) { 	
		char *strtmp, *pid_token=NULL;
		char line[100], cmdBuf[50];

		while (fgets(line, sizeof(line), stream))
		{
			strtmp = line;
			while(*strtmp == ' ')
				strtmp++;
            if(	(strstr(strtmp,"boa") != 0) 
            		|| (strstr(strtmp,"/bin/sh") != 0) 
            		|| (strstr(strtmp,"ps") != 0) 
            		|| (strstr(strtmp,"testcase") != 0) 
            		|| (strstr(strtmp, "root") == 0)
#if defined(CONFIG_APP_FWD) // fwd is daemon to write firmware in flash at last.
            		|| (strstr(strtmp, "fwd") != 0)
#endif            		
#if defined(APP_WATCHDOG)
					|| (strstr(strtmp,"watchdog") != 0)
#endif
            )
				continue;
			else
			{

				pid_token = strtok(strtmp, " ");
				if((pid_token != NULL) && (strlen(pid_token) >= 3))
				{
					sprintf(cmdBuf,"kill -9 %s > /dev/null", pid_token);
					system(cmdBuf);
				}
			}
		}
		fclose(stream);
	}
	system("rm -rf /var/alive_daemons");

#endif
	
// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
	system("killall -9 snmpd 2> /dev/null");
	system("killall -9 solar_monitor 2> /dev/null");
	system("killall -9 solar 2> /dev/null");
	system("killall -9 dns_task 2> /dev/null");
	system("killall -9 ivrserver 2> /dev/null");
	system("killall -9 fwupdate 2> /dev/null");
	system("killall -9 netlink 2> /dev/null");
#endif

#ifdef CONFIG_SNMP
	system("killall -9 snmpd 2> /dev/null");
#endif
}




char *get_ifname(char *name, char *p)
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

#define _PATH_PROCNET_DEV "/proc/net/dev"

int shutdown_netdev(void)
{
	FILE *fh;
	char buf[512];
	char *s, name[16], tmp_str[64];
	int iface_num=0;
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		return 0;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);
	while (fgets(buf, sizeof buf, fh)) {
		s = get_ifname(name, buf);
		if(strstr(name, "eth") || strstr(name, "wlan") || strstr(name, "ppp")|| strstr(name, "sit")){
			iface_num++;
			snprintf(tmp_str,sizeof(tmp_str),"ifconfig %s down",name);
			system(tmp_str);
		}
	}
	
	fclose(fh);
	return iface_num;
}

void kill_processes(void)
{


	printf("upgrade: killing tasks...\n");
	
	kill(1, SIGTSTP);		/* Stop init from reforking tasks */
	kill(1, SIGSTOP);		
	kill(2, SIGSTOP);		
	kill(3, SIGSTOP);		
	kill(4, SIGSTOP);		
	kill(5, SIGSTOP);		
	kill(6, SIGSTOP);		
	kill(7, SIGSTOP);		
	//atexit(restartinit);		/* If exit prematurely, restart init */
	sync();

	signal(SIGTERM,SIG_IGN);	/* Don't kill ourselves... */
	setpgrp(); 			/* Don't let our parent kill us */
	sleep(1);
	signal(SIGHUP, SIG_IGN);	/* Don't die if our parent dies due to
					 * a closed controlling terminal */
	
}

int upgrade_firmware(char *upload_data, int upload_len, char *buffer,int bufferLen)
{
int head_offset=0 ;
int isIncludeRoot=0;
 int		 len;
    int          locWrite;
    int          numLeft;
    int          numWrite;
    IMG_HEADER_Tp pHeader;
	int flag=0, startAddr=-1, startAddrWeb=-1;
	int update_fw=0, update_cfg=0;

    int fh;
	unsigned char cmdBuf[30];
#if defined(CONFIG_APP_FWD)
	int newfile = 1;
#else
	killDaemon(1);
	#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
	    // do nothing
	#else
		shutdown_netdev();
		system("ifconfig br0 down 2> /dev/null");
	#endif
#endif

if(!upload_data || !buffer || upload_len<=0 || bufferLen<=0 || upload_len>CONFIG_FLASH_SIZE){
	  return RTK_FAILED;
	}

while(head_offset <   upload_len) {
    locWrite = 0;
    pHeader = (IMG_HEADER_Tp) &upload_data[head_offset];
    len = pHeader->len;
#ifdef _LITTLE_ENDIAN_
    len  = DWORD_SWAP(len);
#endif    
    numLeft = len + sizeof(IMG_HEADER_T) ;
    
    // check header and checksum
    if (!memcmp(&upload_data[head_offset], FW_HEADER, SIGNATURE_LEN) || !memcmp(&upload_data[head_offset], FW_HEADER_WITH_ROOT, SIGNATURE_LEN))
    	flag = 1;
    else if (!memcmp(&upload_data[head_offset], WEB_HEADER, SIGNATURE_LEN))
    	flag = 2;
    else if (!memcmp(&upload_data[head_offset], ROOT_HEADER, SIGNATURE_LEN))
    {
    	flag = 3;
    	isIncludeRoot = 1;
    }
    else {
       	snprintf(buffer,bufferLen, "Invalid file format [%s-%d]!",__FILE__,__LINE__);
		goto ret_upload;
    }


    if(len > CONFIG_FLASH_SIZE){ //len check by sc_yang
      		snprintf(buffer,bufferLen, "Image len exceed max size 0x%x ! len=0x%x</b><br>",CONFIG_FLASH_SIZE, len);
		goto ret_upload;
    }
    if ( (flag == 1) || (flag == 3)) {
    	if ( !fwChecksumOk(&upload_data[sizeof(IMG_HEADER_T)+head_offset], len)) 
    	{
      		snprintf(buffer,bufferLen, "Image checksum mismatched! len=0x%x, checksum=0x%x</b><br>", len,
			*((unsigned short *)&upload_data[len-2]) );
		goto ret_upload;
	}
    }
    else {
    	char *ptr = &upload_data[sizeof(IMG_HEADER_T)+head_offset];
    	if ( !CHECKSUM_OK(ptr, len) ) 
    	{
     		snprintf(buffer,bufferLen, "Image checksum mismatched! len=0x%x</b><br>", len);
		goto ret_upload;
	}
    }


    if(flag == 3){
    	fh = open(FLASH_DEVICE_NAME1, O_RDWR);
#if defined(CONFIG_APP_FWD)	
			write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME1);
			newfile = 2;
#endif	
    	}
	else if(flag == 1){
       fh = open(FLASH_DEVICE_NAME, O_RDWR);
#if defined(CONFIG_APP_FWD)			
		   write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME);
		   newfile = 2;
#endif			

	}else
	{
		fh = open(FLASH_DEVICE_NAME, O_RDWR);
		write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME);
		newfile = 2;
	}

    if ( fh == -1 ) 
	{
       snprintf(buffer,bufferLen, "File open failed!");
		goto ret_upload;
    }
	else {

	if (flag == 1) {
		if ( startAddr == -1){
			//startAddr = CODE_IMAGE_OFFSET;
			startAddr = pHeader->burnAddr ;
			#ifdef _LITTLE_ENDIAN_
    				startAddr = DWORD_SWAP(startAddr);
    			#endif
		}

	}
	else if (flag == 3) {
		if ( startAddr == -1){
			startAddr = 0; // always start from offset 0 for 2nd FLASH partition
		}
	}
	else {
		if ( startAddrWeb == -1){
			//startAddr = WEB_PAGE_OFFSET;
			startAddr = pHeader->burnAddr ;
			#ifdef _LITTLE_ENDIAN_
    				startAddr = DWORD_SWAP(startAddr);
    			#endif
		}
		else
			startAddr = startAddrWeb;
	}
	lseek(fh, startAddr, SEEK_SET);
#if defined(CONFIG_APP_FWD)			
	{
		char tmpStr[20]={0};
		sprintf(tmpStr,"\n%d",startAddr);
		write_line_to_file(FWD_CONF, (newfile==1?1:2), tmpStr);
		
		newfile = 2;
	}
#endif	

	if(flag == 3){
		locWrite += sizeof(IMG_HEADER_T); // remove header
		numLeft -=  sizeof(IMG_HEADER_T);
#ifndef CONFIG_APP_FWD		
		kill_processes();
		sleep(2);
#endif
	}

#if defined(CONFIG_APP_FWD)
	{
		char tmpStr[20]={0};
		
		sprintf(tmpStr,"\n%d",numLeft);
		write_line_to_file(FWD_CONF, (newfile==1?1:2), tmpStr);
		sprintf(tmpStr,"\n%d\n",locWrite+head_offset);
		write_line_to_file(FWD_CONF, (newfile==1?1:2), tmpStr); 				
		newfile = 2;
	}
	
#else //#if defined(CONFIG_APP_FWD)

	numWrite = write(fh, &(upload_data[locWrite+head_offset]), numLeft);
	if (numWrite < numLeft) {
		snprintf(buffer,bufferLen, "File write failed. locWrite=%d numLeft=%d numWrite=%d Size=%d bytes.", locWrite, numLeft, numWrite, upload_len);
		goto ret_upload;
	}
#endif //#if defined(CONFIG_APP_FWD)
	locWrite += numWrite;
 	numLeft -= numWrite;
	sync();
#ifndef CONFIG_APP_FWD	
#ifdef KERNEL_3_10
#define BLKFLSBUF  _IO(0x12,97) /* flush buffer cache */
    if(ioctl(fh,BLKFLSBUF,NULL) < 0){ 
		snprintf(buffer,bufferLen,"flush mtd system cache error\n");
		goto ret_upload;
    }   
#endif
#endif
		close(fh);
		fh=0;

	head_offset += len + sizeof(IMG_HEADER_T) ;
	startAddr = -1 ; //by sc_yang to reset the startAddr for next image
	update_fw = 1;
    }
} //while //sc_yang   
#ifdef CONFIG_APP_FWD
	{
		int shm_id=0, shm_size=upload_len;		
		char tmpStr[20]={0};
	  	char *shm_memory=NULL; 
	  	/* Generate a System V IPC key */ 
	  	key_t key=ftok("/bin/fwd", 0xE05);
	
	  	/* Allocate a shared memory segment */
	  	shm_id = shmget(key, shm_size, IPC_CREAT | IPC_EXCL | 0666);
	  	if(shm_id==-1){
		  snprintf(buffer,bufferLen,"Allocate share memory segment fail!\n");
		  return RTK_FAILED;
	  	}
	  	shm_memory = (char *)shmat(shm_id, NULL, 0);
	  	memcpy(shm_memory,upload_data,upload_len);			
		sprintf(tmpStr,"%d",shm_id);			
		write_line_to_file("/var/fwd.ready", 1, tmpStr);			
		sync();
	}
#endif
  return RTK_SUCCESS;
  ret_upload:
  	if(fh>0) close(fh);
	return RTK_FAILED;
}
#ifdef USER_PASSWORD_CHANGE_DUMP
int rtl_dump_passwd_change_uptime()
{
	FILE *fp;
	char *ptr = NULL;
	char tmpBuff[32] = {0};
	int  time_seconds;
	char cmd_buf[64] = {0};

	fp = fopen("/proc/uptime", "r");
	if(fp == NULL)
	{
        printf("[%s %d] Error: read /proc/uptime error!\n", __FILE__, __LINE__);
		return RTK_FAILED;
	}

	memset(tmpBuff, 0, sizeof(tmpBuff));
	fgets(tmpBuff, sizeof(tmpBuff), fp);
	
	fclose(fp);
	
	ptr = strstr(tmpBuff, ".");
    memset(ptr, 0x0, sizeof(tmpBuff)-(ptr-tmpBuff));
	time_seconds = atoi(tmpBuff);
	//printf("[%s %d] time_seconds: %d\n", __FILE__, __LINE__, time_seconds);

	memset(cmd_buf, 0, sizeof(cmd_buf));
	sprintf(cmd_buf, "echo %d > /tmp/passwd_change_uptime.txt", time_seconds);
	//printf("[%s %d] cmd_buf: %s\n", __FILE__, __LINE__, cmd_buf);
	system(cmd_buf);
	sync();

	return RTK_SUCCESS;
}
#endif

/**************************************************
* @NAME:
* 	rtk_set_user_passwd
* 
* @PARAMETERS:
* 	@Input
* 		user:
*		passwd:
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	set username and password
*
***************************************************/
int rtk_set_user_passwd(unsigned char *user, unsigned char *passwd)
{
	if(user == NULL || passwd == NULL)
		return RTK_FAILED;

	if ( !apmib_set(MIB_USER_NAME, (void *)user) ) {
		return RTK_FAILED;
	}

	if ( !apmib_set(MIB_USER_PASSWORD, (void *)passwd) ) {
		return RTK_FAILED;
	}
	
#ifdef USER_PASSWORD_CHANGE_DUMP
	rtl_dump_passwd_change_uptime();
#endif

	return RTK_SUCCESS;
}

int rtk_get_ddns();
int rtk_get_timezone();
int rtk_get_dos();

/**************************************************
* @NAME:
* 	rtk_get_user_passwd
* 
* @PARAMETERS:
* 	@Input
* 		user:
*		passwd:
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get username and password
*
***************************************************/
int rtk_get_user_passwd(unsigned char *user, unsigned char *passwd)
{
	if(user == NULL || passwd == NULL)
		return RTK_FAILED;

	if ( !apmib_get(MIB_USER_NAME, (void *)user) ) {
		return RTK_FAILED;
	}

	if ( !apmib_get(MIB_USER_PASSWORD, (void *)passwd) ) {
		return RTK_FAILED;
	}

	return RTK_SUCCESS;
}
int rtk_get_unp_enable(int enable);


/**************************************************
* @NAME:
* 	rtk_get_internet_status
* 
* @PARAMETERS:
* 	@Input
* 		connected: WAN Access Type
*		0-connect fail, 1-connect successful
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get internet status
*
***************************************************/
int rtk_get_internet_status(int *connected)
{
	if(connected == NULL)
		return RTK_FAILED;
	
	int sockfd;
	char *host = "www.baidu.com";
	struct hostent *hptr = NULL;
	struct sockaddr_in dstaddr;
	*connected = 0;

	if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		printf("Error: create socket failed.\n");
		return RTK_FAILED;
	}

	bzero(&dstaddr, sizeof(dstaddr));
	dstaddr.sin_family = AF_INET;
	dstaddr.sin_port = htons(0);

	if( (hptr = gethostbyname(host) ) == NULL )
	{
		printf("Error: gethostbyname error for host:%s\n", host);
		return RTK_FAILED;
	}
	dstaddr.sin_addr.s_addr = ((struct in_addr *)hptr->h_addr)->s_addr;

	if(send_echo_req(sockfd, &dstaddr)==RTK_FAILED)
	{
		close(sockfd);
		return RTK_FAILED;
	}
	if(recv_echo_reply(sockfd)==RTK_FAILED)
	{
		close(sockfd);
		*connected = 0;
		return RTK_FAILED;
	}
	
	close(sockfd);
	*connected = 1;
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_phy_port_status
* 
* @PARAMETERS:
* 	@Input
* 		struct rtk_port_status *info: 
*		struct rtk_port_status
*		{
*			enum LINK_STATUS	port_status;
*			int 	port_number;
*		};
*		port_number: 0/1:two lan port, 2:wan port
*		enum LINK_STATUS 
*		{
*			LINK_ERROR,
*			LINK_UP,
*			LINK_DOWN
*		};
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get wan physical state
*
***************************************************/
int rtk_get_phy_port_status(struct rtk_port_status *info)
{

	FILE *fh;
	int found_port = 0;
	char cur_port[12];
	unsigned char buffer[128];
	
	memset(cur_port,0,12);
	sprintf(cur_port,"Port%d",info->port_number);	
	fh = fopen("/proc/rtl865x/port_status", "r");
	if (!fh) {
		printf("Warning: cannot open /proc/rtl865x/port_status\n");
		return RTK_FAILED;
	}
	
	while(fgets(buffer, 128, fh))
	{
		if(strncmp(buffer, cur_port, strlen(cur_port)) == 0)
		{
			found_port = 1;
		}

		if(found_port)
		if(strstr(buffer,"Link"))
		{		
			if(buffer[4] == 'U')
				info->port_status = LINK_UP;
			else if(buffer[4] == 'D')
				info->port_status = LINK_DOWN;
			else
				info->port_status = LINK_ERROR;
			break;			
		}
	}
	fclose(fh);
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_ports_status
* 
* @PARAMETERS:
* 	@Input
* 		port status memory to store info 
* 	@Output
*	 	all phy port status info 
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	get port[0-4] status
*
***************************************************/


int rtk_get_ports_status(unsigned int *real_num,struct rtk_port_status *info, unsigned int empty_entry_num)
{	
	int i ;
	for(i = 0 ; i < 5 ; ++i)
	{
		info[i].port_number = i ;
		rtk_get_phy_port_status(info+i);
	}
	*real_num = 5;
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_link_status
* 
* @PARAMETERS:
* 	@Input
* 		link_info: link status information
*		iface: interface name
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get link status
*
***************************************************/
int rtk_get_link_status(RTK_NET_DEVICE_STATSp link_info, char *iface)
{
	if(link_info == NULL)
	{	
		printf("link_info is NULL.\n");
		return RTK_FAILED;
	}
	if(iface == NULL){
		printf("iface is NULL.\n");
		return RTK_FAILED;
	}

	FILE *fh;
	char buf[256];
	unsigned long long rx_pkts;
	int found = 0;
	int type;
	char tmp_iface[16];
	char *ptr;

	memset(link_info, 0, sizeof(RTK_NET_DEVICE_STATS));

	fh = fopen("/proc/net/dev", "r");
	if (!fh) {
		printf("Warning: cannot open /proc/net/dev\n");
		return RTK_FAILED;
	}

	fgets(buf, sizeof buf, fh); /* eat line */
	fgets(buf, sizeof buf, fh);

	if (strstr(buf, "compressed"))
		type = 3;
	else if (strstr(buf, "bytes"))
		type = 2;
	else
		type = 1;
	
	while(!feof(fh))
	{
		fgets(buf, sizeof buf, fh);
		sprintf(tmp_iface,"%s:",iface);	
		ptr = strstr(buf, tmp_iface) ;
		if((ptr != NULL) && (ptr == buf || *(ptr-1) == ' '))
		{
			//printf("buf: %s\n", buf);
			
			get_dev_fields(type, buf, link_info);
			found = 1;
			break;
		}
	}
	fclose(fh);

	if(found == 0){
		printf("face not found.\n");
		return RTK_FAILED;
	}

	return RTK_SUCCESS;
}


int rtk_get_disk_info(int *total,int *left)
{
	
}

/**************************************************
* @NAME:
* 	rtk_get_download_statics
* 
* @PARAMETERS:
* 	@Input
* 		download: point of download statics
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get download statics
*
***************************************************/
int rtk_get_download_statics(RTK_DOWNLOAD_STATICSp download)
{
	if(download == NULL)
		return RTK_FAILED;
	
	FILE *fh;
	char buf[256];

	memset(download, 0, sizeof(RTK_DOWNLOAD_STATICS));

	fh = fopen("/var/load_statics", "r");
	if (!fh) {
		printf("Warning: cannot open /var/load_statics\n");
		return RTK_FAILED;
	}

	fgets(buf, sizeof buf, fh);
	sscanf(buf, "%lu %lu %lu %lu %llu %*lu %*lu %*lu %*lu %*llu",
		&(download->max_download_speed),
		&(download->min_download_speed),
		&(download->avg_download_speed),
		&(download->cur_download_speed),
		&(download->download_total));
	fclose(fh);

	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtl_get_uplaod_statics
* 
* @PARAMETERS:
* 	@Input
* 		upload: point of upload statics
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get upload statics
*
***************************************************/
int rtk_get_upload_statics(RTK_UPLOAD_STATICSp upload)
{
	if(upload == NULL)
		return RTK_FAILED;
	
	FILE *fh;
	char buf[256];

	memset(upload, 0, sizeof(RTK_UPLOAD_STATICS));

	fh = fopen("/var/load_statics", "r");
	if (!fh) {
		printf("Warning: cannot open /var/load_statics\n");
		return RTK_FAILED;
	}

	fgets(buf, sizeof buf, fh);
	sscanf(buf, "%*lu %*lu %*lu %*lu %*llu %lu %lu %lu %lu %llu",
		&(upload->max_upload_speed),
		&(upload->min_upload_speed),
		&(upload->avg_upload_speed),
		&(upload->cur_upload_speed),
		&(upload->upload_total));
	fclose(fh);

	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_firmware_version
* 
* @PARAMETERS:
* 	@Input
*		version
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get router firmware version number.
*
***************************************************/
int rtk_get_firmware_version(char **version)
{
	if(version == NULL)
		return RTK_FAILED;
	
	(*version)[0] = 0;
	strcpy(*version, fwVersion);
	printf("version number: %s\n", *version);
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_router_info
* 
* @PARAMETERS:
* 	@Input
*		version
*		cpu
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get router information
*
***************************************************/
int rtk_get_router_info(char **version, char **cpu)
{
	if(version == NULL || cpu == NULL)
		return RTK_FAILED;
	
	FILE *fh;
	char buf[128];
	int tag=0;

	(*version)[0] = 0;
	(*cpu)[0] = 0;

	strcpy(*version, fwVersion);

	fh = fopen("/proc/cpuinfo", "r");
	if (!fh) {
		printf("Warning: cannot open /proc/cpuinfo\n");
		return RTK_FAILED;
	}

	while(!feof(fh))
	{
		fgets(buf, sizeof buf, fh);

		if(strstr(buf, "system type") != NULL)
		{
			sscanf(buf, "%*s %*s : %s", *cpu);
			tag = 1;
			break;
		}
	}
	fclose(fh);

	if(tag == 1)
		return RTK_SUCCESS;
	else
		return RTK_FAILED;
}

/**************************************************
* @NAME:
* 	rtk_get_uptime
* 
* @PARAMETERS:
* 	@Input
* 		uptime
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get uptime
*
***************************************************/
int rtk_get_uptime(RTK_TIMEp uptime)
{
	if(uptime == NULL)
		return RTK_FAILED;

	memset(uptime, 0, sizeof(RTK_TIME));
	
	struct sysinfo info ;
	unsigned sec;
	
	sysinfo(&info);
	
	sec = (unsigned long) info.uptime;
	uptime->day = sec / 86400;
	sec %= 86400;
	uptime->hour = sec / 3600;
	sec %= 3600;
	uptime->min = sec / 60;
	uptime->sec = sec % 60;

	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_reboot
* 
* @PARAMETERS:
* 	@Input
* 		none
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
* 
* @FUNCTION :
* 	reboot
*
***************************************************/
int rtk_reboot()
{
	system("reboot");
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_power_off
* 
* @PARAMETERS:
* 	@Input
* 		none
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
* 
* @FUNCTION :
* 	poweroff
*
***************************************************/
int rtk_power_off()
{
	system("poweroff");
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_firmware_info
* 
* @PARAMETERS:
* 	@Input
* 		info: point of firmware_info
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get firmware information
*
***************************************************/
int rtk_get_firmware_info(RTK_FIRMWARE_INFOp info)
{
	if(info == NULL)
		return RTK_FAILED;
	
	FILE *fh;
	char buf[256];
	char *p;

	memset(info, 0, sizeof(RTK_FIRMWARE_INFO));

	strcpy(info->version, fwVersion);

	fh = fopen("/proc/version", "r");
	if (!fh) {
		printf("Warning: cannot open /proc/version\n");
		return RTK_FAILED;
	}

	fgets(buf, sizeof(buf), fh);
	fclose(fh);
	buf[255] = 0;

	if((p = strchr(buf, '#')) == NULL)
		return RTK_FAILED;

	if((p = strchr(p+1, ' ')) == NULL)
		return RTK_FAILED;

	strcpy(info->buildtime, p+1);

	info->version[15] = 0;
	info->buildtime[63] = 0;

	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_restore_default
* 
* @PARAMETERS:
* 	@Input
* 		none
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	default setting
*
***************************************************/
int rtk_restore_default()
{
#ifdef RTL_DEF_SETTING_IN_FW
	system("flash reset");
#else
	if ( !apmib_updateDef() )
		return RTK_FAILED;
#endif

#ifdef CONFIG_RTL_802_1X_CLIENT_SUPPORT
	//To clear 802.1x certs
	system("rsCert -rst");
#endif
#ifdef CONFIG_RTL_WAPI_SUPPORT
	//To clear CA files
	system("storeWapiFiles -reset");
#endif

	apmib_reinit();
	apmib_update_web(CURRENT_SETTING);
	system("reboot");

	return RTK_SUCCESS;
}
int rtk_api_init()
{
	if(apmib_init()==1)
		return RTK_SUCCESS;
#if defined(HAVE_RTK_DRVMIB)
    if(drvmib_init_rtkapi()>0)
		return RTK_SUCCESS;
#endif
	return RTK_FAILED;
}

int rtk_update()
{
	if(apmib_update_web(CURRENT_SETTING)==0)
		return RTK_FAILED;
#if defined(HAVE_RTK_DRVMIB)
	if(drvmib_apply_rtkapi()==0)
		return RTK_FAILED;
#endif
	return RTK_SUCCESS;
}
int rtk_sys_reinit()
{
	system("init.sh gw all");
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_default_mac_address
* 
* @PARAMETERS:
* 	@Input
* 		mac: mac address
*		iface: interface name
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get default mac address
*
***************************************************/
int rtk_get_default_mac_address(unsigned char* mac, char* iface)
{
	if(mac == NULL || iface == NULL)
		return RTK_FAILED;

	if(strcmp(iface, "eth0") == 0)
	{
		if ( !apmib_get(MIB_HW_NIC0_ADDR,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "eth1") == 0)
	{
		if ( !apmib_get(MIB_HW_NIC1_ADDR,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va0") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR1,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va1") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR2,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va2") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR3,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va3") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR4,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va4") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR5,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va5") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR6,  (void *)mac) )
			return RTK_FAILED;
	}
	else if(strcmp(iface, "wlan0-va6") == 0)
	{
		if ( !apmib_get(MIB_HW_WLAN_ADDR7,  (void *)mac) )
			return RTK_FAILED;
	}
	else
		return RTK_FAILED;

	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_lan_drop_rate
* 
* @PARAMETERS:
* 	@Input
* 		drop_rate,unsigned int*
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get lan drop rate
*
***************************************************/
int rtk_get_lan_drop_rate(unsigned int *drop_rate)
{
	RTK_NET_DEVICE_STATS link_info;
	if(drop_rate == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	if(rtk_get_link_status(&link_info,LAN_IF)!=RTK_SUCCESS)
	{
		printf("Failed to get lan link info.\n");
		return RTK_FAILED;
	}
	if(link_info.rx_packets ==0)
		*drop_rate = 0;
	else
		*drop_rate = link_info.rx_dropped*100/(link_info.rx_packets+link_info.rx_dropped);
	return RTK_SUCCESS;
}
/**************************************************
* @NAME:
* 	rtk_get_wan_drop_rate
* 
* @PARAMETERS:
* 	@Input
* 		drop_rate,unsigned int*
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get wan drop rate
*
***************************************************/
int rtk_get_wan_drop_rate(unsigned int *drop_rate)
{
	char wan_if[16] = {'\0'};
	RTK_NET_DEVICE_STATS  link_info;
	if(drop_rate == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	if(getWanIf(wan_if)==RTK_FAILED)
	{
		printf("Failed to get wan interface.\n");
		return RTK_FAILED;
	}
	if(rtk_get_link_status(&link_info,wan_if)!=RTK_SUCCESS)
	{
		printf("Failed to get wan link info.\n");
		return RTK_FAILED;
	}
	if(link_info.rx_packets == 0 )
		*drop_rate = 0 ;
	else
		*drop_rate = link_info.rx_dropped*100/(link_info.rx_packets+link_info.rx_dropped);
	return RTK_SUCCESS;
}
/**************************************************
* @NAME:
* 	rtk_get_wlan_drop_rate
* 
* @PARAMETERS:
* 	@Input
* 		drop_rate: unsigned int*
*		wlan_if:   wlan interface, wlan0/wlan1/...
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get wlan drop rate
*
***************************************************/
int rtk_get_wlan_drop_rate(unsigned int *drop_rate,unsigned char* wlan_if)
{
	RTK_NET_DEVICE_STATS link_info;
	if(drop_rate == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	if(rtk_get_link_status(&link_info,wlan_if)!=RTK_SUCCESS)
	{
		printf("Failed to get wlan link info.\n");
		return RTK_FAILED;
	}
	if(link_info.rx_packets ==0)
		*drop_rate = 0;
	else
		*drop_rate = link_info.rx_dropped*100/(link_info.rx_packets+link_info.rx_dropped);
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_wan_status_by_url
* 
* @PARAMETERS:
* 	@Input
* 		connected,unsigned int*:0-disconnected,1-connected
* 		url,char*
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get the status whether the AP is connected to the specific url
*
***************************************************/
int rtk_get_wan_status_by_url(unsigned int* connected,char* url)
{
	RTK_WAN_INFO wan_info;

	char cmd[64];
	char resfile[32];
	char buffer[256];
	char url_t[256];
	FILE *fp = NULL;
	int fd,res;
	int found = 0;
	int conn = 0;
	
	if(connected == NULL || url == NULL)
		return RTK_FAILED;

	*connected = 0;	
	if(rtk_get_wan_info(&wan_info) != RTK_SUCCESS)
	{
		return RTK_FAILED;
	}
	
	if(wan_info.status == DISCONNECTED||
	   wan_info.status == FIXED_IP_DISCONNECTED||
	   wan_info.status == PPPOE_DISCONNECTED||
	   wan_info.status == PPTP_DISCONNECTED||
	   wan_info.status == L2TP_DISCONNECED||
	   wan_info.status == USB3G_MODEM_INIT||
	   wan_info.status == USB3G_DAILING||
	   wan_info.status == USB3G_REMOVED||
	   wan_info.status == USB3G_DISCONNECTED)
	{
		return RTK_SUCCESS;
	}
	/*Read the wan status file*/
	fd = open(RES_LOCK_FILE, O_RDWR|O_CREAT|O_TRUNC);
	if(fd < 0)
	{
		return RTK_FAILED;
	}
	res  = rtk_file_lock(fd);
	if(res == -1)
	{
		rtk_file_unlock(fd);
		return RTK_FAILED;
	}
	fp = fopen(WAN_STATUS_FILE,"r");
	if(fp == NULL)
	{
		rtk_file_unlock(fd);
		return RTK_FAILED;
	}
	while(fgets(buffer, sizeof(buffer), fp))
	{
		sscanf(buffer,"%s %d",url_t,connected);
		if(*connected == 1)
		{
			conn = 1;
		}
		if(strstr(url_t,url))
		{
			found = 1;
		}
		if(found == 1)
		{
			if(conn==1)
			{
				*connected = 1;
				break;
			}
			else
			{
				continue;
			}
		}
	}
	fclose(fp);
	fp = NULL;
	rtk_file_unlock(fd);
	/*Read wan status file end*/
	if(found == 0)
	{
		/*Add to the url list file*/
		fd = open(LIST_LOCK_FILE, O_RDWR|O_CREAT|O_TRUNC);
		if(fd < 0)
		{
			rtk_file_unlock(fd);
			return RTK_FAILED;
		}
		res  = rtk_file_lock(fd);
		if(res == -1)
		{
			rtk_file_unlock(fd);
			return RTK_FAILED;
		}
		/*Find in the url list file*/
		fp = fopen(URL_LIST_FILE,"r");
		if(fp != NULL)
		{
			while(fgets(buffer, sizeof(buffer), fp))
			{
				sscanf(buffer,"%s",url_t);
				if(strstr(url_t,url))
				{
					found = 1;
					break;
				}
			}
			fclose(fp);	
			fp = NULL;
		}
		if(found == 0)
		{
			fp = fopen(URL_LIST_FILE,"a+");/*add to the url list*/
			if(fp != NULL)
			{
				fprintf(fp,"%s\n",url);
				fclose(fp);
				fp = NULL;
			}
		}
		rtk_file_unlock(fd);
		/*Add to the url list file end*/
	}
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_get_link_type_by_mac
* 
* @PARAMETERS:
* 	@Input
* 		host mac
* 	@Output
*	 	link type
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	get link type by host mac
*
***************************************************/
int rtk_get_link_info_by_mac(unsigned char mac[6],struct rtk_link_type_info *info)
{
	struct rtk_link_type_info	tmp_info;

	if(check_wifi("wlan0",mac,&tmp_info)==RTK_SUCCESS)
	{
		info->type = RTK_5G;	
		info->last_rx_bytes = tmp_info.cur_rx_bytes;
		info->last_tx_bytes = tmp_info.cur_tx_bytes;
		return RTK_SUCCESS;
	}

	if(check_wifi("wlan1",mac,&tmp_info)==RTK_SUCCESS)
	{
		info->type = RTK_24G;	
		info->last_rx_bytes = tmp_info.cur_rx_bytes;
		info->last_tx_bytes = tmp_info.cur_tx_bytes;		
		return RTK_SUCCESS;
	}
	
	if(check_layer2(mac,&tmp_info)==RTK_SUCCESS)
	{
		info->type = RTK_ETHERNET;
		info->port_number = tmp_info.port_number;
		info->last_rx_bytes = tmp_info.cur_rx_bytes;
		info->last_tx_bytes = tmp_info.cur_tx_bytes;
		return RTK_SUCCESS;
	}

	info->type = RTK_LINK_ERROR;
	return RTK_FAILED;
}

#ifdef CONFIG_APP_CMCC_ADAPTER 
int rtk_get_wlan_link_info(char *ifname, struct rtk_link_type_info *info, int *term_number, int entry_number)
{
    RTK_WLAN_STA_EXTRA_INFO_Tp pWlanInfo;
    int i, rfband, sta_num=0;
    char *buff;

    if(getWlStaNum(ifname,&sta_num))        
        return RTK_FAILED;

    buff = malloc(sizeof(RTK_WLAN_STA_EXTRA_INFO_T) * (MAX_STA_NUM+1));
    if(!buff)
        return RTK_FAILED;
    memset(buff,0,sizeof(RTK_WLAN_STA_EXTRA_INFO_T) * (MAX_STA_NUM +1));
    
    if(rtk_wlan_get_wlan_sta_extra_info(ifname,(RTK_WLAN_STA_EXTRA_INFO_Tp)buff) == RTK_SUCCESS)
    {       
        for(i = 1; i <= sta_num; ++i)
        {
            pWlanInfo = (RTK_WLAN_STA_EXTRA_INFO_Tp)&buff[i*sizeof(RTK_WLAN_STA_EXTRA_INFO_T)];
#if 0
            printf("pWlanInfo->addr = %02x%02x%02x%02x%02x%02x\n", 
                pWlanInfo->addr[0], pWlanInfo->addr[1],pWlanInfo->addr[2],pWlanInfo->addr[3],pWlanInfo->addr[4],pWlanInfo->addr[5]);
            printf("pWlanInfo->link_time = %d\n", pWlanInfo->link_time);
            printf("pWlanInfo->rx_bytes = %d\n", pWlanInfo->rx_bytes);
            printf("pWlanInfo->tx_bytes = %d\n", pWlanInfo->tx_bytes);
            printf("pWlanInfo->rssi = %d\n", pWlanInfo->rssi);
            printf("pWlanInfo->client_host_ip = %d.%d.%d.%d\n", pWlanInfo->client_host_name[0],  pWlanInfo->client_host_name[1], pWlanInfo->client_host_name[2], pWlanInfo->client_host_name[3]);
            printf("pWlanInfo->client_host_name = %d\n", pWlanInfo->client_host_name);
#endif			
            if(*term_number+i-1 >= entry_number)
                break;
            {
                memcpy(info[*term_number+i-1].mac, pWlanInfo->addr, 6);
                info[*term_number+i-1].link_time = pWlanInfo->link_time;
                info[*term_number+i-1].last_rx_bytes = pWlanInfo->rx_bytes;
                info[*term_number+i-1].last_tx_bytes = pWlanInfo->tx_bytes;
                rtk_wlan_get_rf_band(ifname, &rfband);
                if(rfband == PHYBAND_2G)
                    info[*term_number+i-1].type = RTK_24G;
                else
                    info[*term_number+i-1].type = RTK_5G;
                info[*term_number+i-1].rssi = pWlanInfo->rssi;
                snprintf(info[*term_number+i-1].ifname, sizeof(info[*term_number+i-1].ifname), "%s", ifname);
                rtk_wlan_get_ssid(ifname, info[*term_number+i-1].ssid);
#ifdef CONFIG_RECORD_CLIENT_HOST
                snprintf(info[*term_number+i-1].client_host_ip, sizeof(info[*term_number+i-1].client_host_ip), "%d.%d.%d.%d", 
                    pWlanInfo->client_host_ip[0], pWlanInfo->client_host_ip[1], pWlanInfo->client_host_ip[2], pWlanInfo->client_host_ip[3]);
                snprintf(info[*term_number+i-1].client_host_name, sizeof(info[*term_number+i-1].client_host_name), "%s", pWlanInfo->client_host_name);
#endif
            }
            *term_number += i;
        }
    }   

    free(buff);
    return RTK_SUCCESS;
}


int rtk_get_lan_link_info(struct rtk_link_type_info *info,int *term_number,int entry_number)
{
    FILE *fp;
    unsigned char br0_mac[6];
    unsigned char br0_mac_str[32];
    unsigned char line_buffer[512];
    unsigned char mac_addr[6];
    unsigned char mac_str[32];
    unsigned char *pchar, *pstart, *p;
    int i, j, idx=0;
    struct rtk_link_type_info tmp_info;
    
    if((fp = fopen("/proc/rtl865x/l2","r+")) != NULL)
    {
        memset(br0_mac,0,6);
    	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)br0_mac);
    	if(!memcmp(br0_mac, "\x00\x00\x00\x00\x00\x00", 6))
    		apmib_get(MIB_HW_NIC0_ADDR,  (void *)br0_mac);
        snprintf((char *)br0_mac_str, sizeof(br0_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", br0_mac[0], br0_mac[1], br0_mac[2], br0_mac[3], br0_mac[4], br0_mac[5]);
    	br0_mac_str[strlen("ff:ff:ff:ff:ff:ff")]='\0';

        fgets(line_buffer, sizeof(line_buffer), fp); //skip line 1: "ASIC L2 Table:"
        while(fgets(line_buffer, sizeof(line_buffer), fp))
    	{			
    		line_buffer[strlen(line_buffer)-1]='\0';

    		if(strstr(line_buffer, "ff:ff:ff:ff:ff:ff") || strstr(line_buffer, "CPU") || strstr(line_buffer, "FID:1") || strstr(line_buffer, br0_mac_str))
    			continue;	
    		
    		pchar=strchr(line_buffer, ':');
    		pstart=pchar-2;
    		snprintf(mac_str, strlen("ff:ff:ff:ff:ff:ff"), "%s", pstart);
    		if(*term_number+idx >= entry_number)
    		    break;
    		{
    		    format_mac(mac_str,&(info[*term_number+idx].mac));
    		    p = strstr(line_buffer,"mbr");
				sscanf(p,"mbr(%d",&(info[*term_number+idx].port_number));
				GetPortStatus(info[*term_number+idx].port_number, &tmp_info);
				info[*term_number+idx].type = RTK_ETHERNET;
        		info[*term_number+idx].port_number = tmp_info.port_number;
        		info[*term_number+idx].last_rx_bytes = tmp_info.cur_rx_bytes;
        		info[*term_number+idx].last_tx_bytes = tmp_info.cur_tx_bytes;
    		}
    		++idx;				
    	}
    	fclose(fp);
        *term_number += idx;
		return RTK_SUCCESS;
    }
    printf("[%s.%d] open proc/rtl865x/l2 failed\n");
    return RTK_FAILED;
}
#endif

int rtk_get_terminal_RxTx_info(struct rtk_link_type_info *info)
{
	switch(info->type)
	{
		case RTK_ETHERNET:
			GetPortStatus(info->port_number,info);
			break;
			
		case RTK_24G:
			check_wifi("wlan1",info->mac,info);
			break;
			
		case RTK_5G:			
			check_wifi("wlan0",info->mac,info);
			break;

		default:
			break;			
	}
}
void rtk_calculate_result(struct rtk_link_type_info *info)
{
	info->all_bytes = info->cur_rx_bytes + info->cur_tx_bytes;
	info->download_speed = (info->cur_tx_bytes - info->last_tx_bytes)*10;
	info->upload_speed = (info->cur_rx_bytes -info->last_rx_bytes)*10;
}


// modify by dingzhihao(realtek)
void clean_error_station(int term_number, int* real_num,struct rtk_link_type_info *info)
{
	int cur_index, last_index;
	cur_index = 0;
	last_index = term_number - 1;
	while(cur_index <= last_index)
	{
		if((info+cur_index)->type == RTK_LINK_ERROR)
		{
			memcpy((info+cur_index),(info+cur_index+1),(last_index-cur_index)*sizeof(struct rtk_link_type_info));
			--term_number;
			--last_index;
		}
		else
		{
			++cur_index;
		}
	}
	*real_num = term_number;
}
// end

/**************************************************
* @NAME:
* 	rtk_get_terminal_info
* 
* @PARAMETERS:
* 	@Input
* 		terminal memory to store info 
* 	@Output
*	 	all lan terminal status info 
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	get terminal info 
*
***************************************************/
#define _PATH_DEVICE_MAC_BRAND "/etc/device_mac_brand.txt"
#define TERMINAL_RATE_INFO "/var/terminal_rate_info"
static int string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return RTK_FAILED;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return RTK_SUCCESS;
}

int rtk_get_terminal_rate_by_mac(char *mac, int *download_rate, int *upload_rate)
{
    FILE *fp=NULL;
    int fh=0;
    int idx =0,i=0;
    unsigned char buffer[128];
	unsigned char strtmp[24]={0};
	unsigned char macAddr[12]={0};
	struct rtk_terminal_rate_info rate_entry[MAX_TERM_NUMBER];

	if((fp = fopen(TERMINAL_RATE_INFO,"r")) != NULL)
	{
		
		if((fh=fileno(fp))<0){
			fclose(fp);
			goto out;
		}
		flock(fh, LOCK_EX);
		
		while((fgets(buffer, sizeof(buffer), fp) )&& (idx<MAX_TERM_NUMBER))
		{		
			
			buffer[strlen(buffer)-1]='\0';
			sscanf(buffer,"%u %u %s %u %u %u %u",&rate_entry[idx].link_flag,
				&rate_entry[idx].port_number, strtmp,
				&rate_entry[idx].upload_speed, &rate_entry[idx].download_speed,&rate_entry[idx].rx_bytes,&rate_entry[idx].tx_bytes);
			string_to_hex(strtmp, macAddr, 12);
			rate_entry[idx].all_bytes=rate_entry[idx].rx_bytes+rate_entry[idx].tx_bytes;
			memcpy(rate_entry[idx].mac,macAddr,6);
			#if 0
			printf("[%d]:%u %u %x:%x:%x:%x:%x:%x %d %d %d %d %d,[%s]:[%d]\n",idx,rate_entry[idx].link_flag,rate_entry[idx].port_number,
			rate_entry[idx].mac[0], rate_entry[idx].mac[1],	rate_entry[idx].mac[2], 
			rate_entry[idx].mac[3],rate_entry[idx].mac[4], rate_entry[idx].mac[5],
			rate_entry[idx].upload_speed, rate_entry[idx].download_speed,rate_entry[idx].rx_bytes,rate_entry[idx].tx_bytes,
			rate_entry[idx].all_bytes,__FUNCTION__,__LINE__);
			#endif
			idx++;
		}
		flock(fh, LOCK_UN);
		fclose(fp);
		
		for(i=0; i<idx; i++)
    	{
    	    if(equal_mac(mac,rate_entry[i].mac)==RTK_SUCCESS)
    	    {
    	        *upload_rate = rate_entry[i].download_speed;
    			*download_rate = rate_entry[i].upload_speed;
    			return RTK_SUCCESS;
    	    }
    	}
	}

	
out :
    return RTK_FAILED;

}

int rtk_get_terminal_rate_by_port(int port, int *download_rate, int *upload_rate)
{
    FILE *fp=NULL;
    int fh=0;
    int idx =0,i=0;
    unsigned char buffer[128];
	unsigned char strtmp[24]={0};
	unsigned char macAddr[12]={0};
	struct rtk_terminal_rate_info rate_entry[MAX_TERM_NUMBER];

	if((fp = fopen(TERMINAL_RATE_INFO,"r")) != NULL)
	{
		
		if((fh=fileno(fp))<0){
			fclose(fp);
			goto out;
		}
		flock(fh, LOCK_EX);
		
		while((fgets(buffer, sizeof(buffer), fp) )&& (idx<MAX_TERM_NUMBER))
		{		
			
			buffer[strlen(buffer)-1]='\0';
			sscanf(buffer,"%u %u %s %u %u %u %u",&rate_entry[idx].link_flag,
				&rate_entry[idx].port_number, strtmp,
				&rate_entry[idx].upload_speed, &rate_entry[idx].download_speed,&rate_entry[idx].rx_bytes,&rate_entry[idx].tx_bytes);
			string_to_hex(strtmp, macAddr, 12);
			rate_entry[idx].all_bytes=rate_entry[idx].rx_bytes+rate_entry[idx].tx_bytes;
			memcpy(rate_entry[idx].mac,macAddr,6);
			#if 0
			printf("[%d]:%u %u %x:%x:%x:%x:%x:%x %d %d %d %d %d,[%s]:[%d]\n",idx,rate_entry[idx].link_flag,rate_entry[idx].port_number,
			rate_entry[idx].mac[0], rate_entry[idx].mac[1],	rate_entry[idx].mac[2], 
			rate_entry[idx].mac[3],rate_entry[idx].mac[4], rate_entry[idx].mac[5],
			rate_entry[idx].upload_speed, rate_entry[idx].download_speed,rate_entry[idx].rx_bytes,rate_entry[idx].tx_bytes,
			rate_entry[idx].all_bytes,__FUNCTION__,__LINE__);
			#endif
			idx++;
		}
		flock(fh, LOCK_UN);
		fclose(fp);
		
		for(i=0; i<idx; i++)
    	{
    	    if(port == rate_entry[i].port_number)
    	    {
    	        *upload_rate = rate_entry[i].download_speed;
    			*download_rate = rate_entry[i].upload_speed;
    			return RTK_SUCCESS;
    	    }
    	}
	}

	
out :
    return RTK_FAILED;

}

int  rtk_get_terminal_rate( int term_number,struct rtk_link_type_info *info)
{
	int linkFlag =0;
	int ret =RTK_FAILED;
	FILE *fp=NULL;
	unsigned char buffer[128];
	unsigned char strtmp[24]={0};
	unsigned char macAddr[12]={0};
	struct rtk_terminal_rate_info rate_entry[MAX_TERM_NUMBER];	
	struct rtk_link_type_info tmp_info[MAX_TERM_NUMBER];
	int findFlag=0;
	int idx =0,i=0,j=0;
	int fh=0; 
	
	for(i=0; i<term_number; i++){
		info[i].download_speed=0;
		info[i].upload_speed=0;
		info[i].all_bytes=0;
	}
	
	if((fp = fopen(TERMINAL_RATE_INFO,"r")) != NULL)
	{
		
		if((fh=fileno(fp))<0){
			fclose(fp);
			goto out;
		}
		flock(fh, LOCK_EX);
		
		while((fgets(buffer, sizeof(buffer), fp) )&& (idx<MAX_TERM_NUMBER))
		{		
			
			buffer[strlen(buffer)-1]='\0';
			sscanf(buffer,"%u %u %s %u %u %u %u",&rate_entry[idx].link_flag,
				&rate_entry[idx].port_number, strtmp,
				&rate_entry[idx].upload_speed, &rate_entry[idx].download_speed,&rate_entry[idx].rx_bytes,&rate_entry[idx].tx_bytes);
			string_to_hex(strtmp, macAddr, 12);
			rate_entry[idx].all_bytes=rate_entry[idx].rx_bytes+rate_entry[idx].tx_bytes;
			memcpy(rate_entry[idx].mac,macAddr,6);
			#if 0
			printf("[%d]:%u %u %x:%x:%x:%x:%x:%x %d %d %d %d %d,[%s]:[%d]\n",idx,rate_entry[idx].link_flag,rate_entry[idx].port_number,
			rate_entry[idx].mac[0], rate_entry[idx].mac[1],	rate_entry[idx].mac[2], 
			rate_entry[idx].mac[3],rate_entry[idx].mac[4], rate_entry[idx].mac[5],
			rate_entry[idx].upload_speed, rate_entry[idx].download_speed,rate_entry[idx].rx_bytes,rate_entry[idx].tx_bytes,
			rate_entry[idx].all_bytes,__FUNCTION__,__LINE__);
			#endif
			idx++;
		}
		flock(fh, LOCK_UN);
		fclose(fp);
	}
	
	for(i=0;i<term_number;++i)
	{		
		linkFlag =-1;
		if(info[i].type==RTK_ETHERNET)
			linkFlag =0;
		else if ((info[i].type==RTK_24G)||(info[i].type==RTK_5G))
			linkFlag=1;
		else
		{
			continue;
		}
		#if 0
		printf("[%d]:%u %u %x:%x:%x:%x:%x:%x %d %d %d [%s]:[%d]\n",i,linkFlag,info[i].port_number,
					info[i].mac[0], info[i].mac[1],info[i].mac[2], info[i].mac[3],info[i].mac[4], info[i].mac[5],
					info[i].upload_speed, info[i].download_speed,info[i].all_bytes,	__FUNCTION__,__LINE__);
		#endif
		for(j=0;j<idx;j++)
		{
			if(linkFlag ==rate_entry[ j ].link_flag)
			{
				if(((linkFlag==0)&&(info[i].port_number==rate_entry[j].port_number))
				||((linkFlag==1)&&(equal_mac(info[i].mac,rate_entry[j].mac)==RTK_SUCCESS)))
				{
					findFlag=1;
					info[i].upload_speed =rate_entry[j].upload_speed;
					info[i].download_speed=rate_entry[j].download_speed;
					info[i].all_bytes =rate_entry[j].all_bytes;
					#if 0
					printf("-----[%d]:%u %u %x:%x:%x:%x:%x:%x %d %d %d [%s]:[%d]\n",i,linkFlag,info[i].port_number,
					info[i].mac[0], info[i].mac[1],info[i].mac[2], info[i].mac[3],info[i].mac[4], info[i].mac[5],
					info[i].upload_speed, info[i].download_speed,info[i].all_bytes,	__FUNCTION__,__LINE__);
					#endif
				}
			}
		}
		
	}
	
	if(findFlag)
		ret =RTK_SUCCESS;
out :
	return ret;
}

int rtk_get_terminal_info(unsigned int *real_num,struct rtk_link_type_info *info, unsigned int empty_entry_num)
{
	int term_number = 0 ;
	int i;
	
#ifndef CONFIG_APP_CMCC_ADAPTER
	/*get arp/mac info from proc/net/arp */
	if(getArpInfo(info,&term_number,empty_entry_num)==RTK_FAILED)
	{
		printf("%s.%d.get terminal fail\n",__FUNCTION__,__LINE__);
		return RTK_FAILED;
	}
	    // modify by dingzhihao(realtek)
	//*real_num = term_number;
    // end

	/*get link type/rx¡¢tx info by terminal mac*/
	for(i = 0 ;i < term_number; ++i)
	{
		rtk_get_link_info_by_mac(info[i].mac,(info+i));	
		// modify by chenbo(realtek)
		rtk_get_device_brand(info[i].mac,_PATH_DEVICE_MAC_BRAND,info[i].brand);
	}
	rtk_get_terminal_rate( term_number,info);
    // modify by dingzhihao(realtek)
	clean_error_station(term_number,real_num,info);
    // end
	////system("echo 1 > /var/lan_dev_link_time_flag");
	//sleep(2);
	////while(isFileExist("/var/lan_dev_link_time_flag"))
	////	;
	rtk_get_lan_dev_link_time(real_num, info);
	/*dump terminal info*/
	//dumpAllTerminalInfo(term_number,info);
	return RTK_SUCCESS;
#else
    char ifname[16], ip_str[16];
    int j, op_mode;

    apmib_get(MIB_OP_MODE, (void *)&op_mode);
    //if(op_mode == GATEWAY_MODE){  //two same mac may appear in arp table at the same time
    if(0){
        if(getArpInfo(info,&term_number,empty_entry_num)==RTK_FAILED)
    	{
    		printf("%s.%d.get terminal fail\n",__FUNCTION__,__LINE__);
    		return RTK_FAILED;
    	}
    	for(i = 0 ;i < term_number; ++i)
    	{
    		rtk_get_link_info_by_mac(info[i].mac,(info+i));	
    		rtk_get_device_brand(info[i].mac,_PATH_DEVICE_MAC_BRAND,info[i].brand);
    	}
            
    }else{
        if(rtk_get_lan_link_info(info, &term_number, empty_entry_num)==RTK_FAILED)
            return RTK_FAILED;

        for(i=0; i<NUM_WLAN_INTERFACE; i++){
            for(j=0; j<=NUM_VWLAN; j++){
                if(j==0)
                    snprintf(ifname, sizeof(ifname), "wlan%d", i);
                else
                    snprintf(ifname, sizeof(ifname), "wlan%d-va%d", i, j-1);
                if(rtk_get_wlan_link_info(ifname, info, &term_number, empty_entry_num)==RTK_FAILED)
                    return RTK_FAILED;
            }
        }

        for(i=0; i<term_number; i++){
            get_terminal_ip_by_mac(info[i].mac, ip_str);
            inet_aton(ip_str, &info[i].ip);
    		rtk_get_device_brand(info[i].mac,_PATH_DEVICE_MAC_BRAND,info[i].brand);
        }
    }
		
	rtk_get_terminal_rate( term_number,info);
	
	clean_error_station(term_number,real_num,info);
	
	rtk_get_lan_dev_link_time(real_num, info);
	
	return RTK_SUCCESS;
#endif    
}
#ifdef CONFIG_RTL_DNS_TRAP
/**************************************************
* @NAME:
* 	rtk_enable_domain_access
* 
* @PARAMETERS:
* 	@Input
* 		enable:unsigned int,0-disable,1:enable
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	enable domain name access of AP 
*
***************************************************/
int rtk_enable_domain_access(unsigned int enable)
{
	char cmdBuf[100] = {'\0'};
	if(enable!=0 && enable !=1)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	sprintf(cmdBuf,"echo %d > proc/rtl_dnstrap/enable",enable);
	system(cmdBuf);
	return RTK_SUCCESS;
}
/**************************************************
* @NAME:
* 	rtk_set_domain_access_url
* 
* @PARAMETERS:
* 	@Input
* 		url:char*,the url you wan to set for the AP,like test.example.com
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	set the domain name access url for the AP 
*
***************************************************/
int rtk_set_domain_access_url(char* url)
{
	char cmdBuf[200] = {'\0'};
	if(url == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	sprintf(cmdBuf,"echo %s > /proc/rtl_dnstrap/url",url);
	system(cmdBuf);
#ifdef CONFIG_RTL_HTTP_REDIRECT
	memset(cmdBuf,0,sizeof(cmdBuf));
	sprintf(cmdBuf,"echo %s > /proc/http_redirect/default_host",url);
	system(cmdBuf);	
#endif
	return RTK_SUCCESS;
}

#ifdef CONFIG_RTL_HTTP_REDIRECT
/**************************************************
* @NAME:
* 	rtk_enable_welcome_page
* 
* @PARAMETERS:
* 	@Input
* 		enable,unsigned int,0-disable,1-enable
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	enable or disable the welcome page function 
*
***************************************************/
int rtk_enable_welcome_page(unsigned int enable)
{
	char cmdBuf[200] = {'\0'};
	unsigned mib_flag = 0;
#ifdef LE_UI_SUPPORT
	LE_DATA_T pLeData = {0};
#endif
	if(enable !=0 && enable !=1)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	mib_flag = enable? 0: 1;
#ifdef LE_UI_SUPPORT
	apmib_get(MIB_CUSTOMER_SPEC_INFO,&pLeData);
	pLeData.configed = enable?0:1;
	apmib_set(MIB_CUSTOMER_SPEC_INFO,(void*)&pLeData);
#else
	if(!apmib_set(MIB_USER_FIRST_LOGIN_FLAG,(void*)&mib_flag))
		return RTK_FAILED;
#endif
	sprintf(cmdBuf,"echo %d > /proc/http_redirect/enable",enable);
	system(cmdBuf);
	if(enable)
	{
		memset(cmdBuf,0,sizeof(cmdBuf));
		sprintf(cmdBuf,"echo %d > /proc/rtl_dnstrap/enable",enable);
		system(cmdBuf);
	}
	memset(cmdBuf,0,sizeof(cmdBuf));
	sprintf(cmdBuf,"echo %d > /proc/rtl_dnstrap/first",enable);
	system(cmdBuf);
	return RTK_SUCCESS;
}

/**************************************************
* @NAME:
* 	rtk_set_welcome_page
* 
* @PARAMETERS:
* 	@Input
* 		page_name:char*,the welcome page name,like welcome.htm
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED

*   
* @FUNCTION :
* 	set the welcome page for first visit
*
***************************************************/
int rtk_set_welcome_page(char* page_name)
{
	char cmdBuf[200] = {'\0'};
	if(page_name == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	sprintf(cmdBuf,"echo %s > /proc/http_redirect/default_webpage",page_name);
	system(cmdBuf);
	return RTK_SUCCESS;
}
#endif
#endif

void rtk_command_syslog(char * sysCmd)
{
	char tmpBuf[256]={0};
	FILE *fp=NULL;
	if(sysCmd && sysCmd[0]){
		syslog(LOG_INFO,sysCmd);
		snprintf(tmpBuf, 100, "%s 2>&1 > %s",sysCmd,  SYSCMD_LOG_FILE);
		system(tmpBuf);
		fp = fopen(SYSCMD_LOG_FILE, "r");
        if ( fp == NULL )
                return;
		bzero(tmpBuf,sizeof(tmpBuf));
        while(fgets(tmpBuf,sizeof(tmpBuf),fp)){
			syslog(LOG_INFO,tmpBuf);
        }
	fclose(fp);
	unlink(SYSCMD_LOG_FILE);
	}
}

void rtk_dump_log_command(char * command)
{
	char buff[256]={0};
	if(command)
	{
		sprintf(buff,"date >> %s 2>/dev/null",DUMP_DIAG_LOG_FILE);
		system(buff);
		sprintf(buff,"echo \"%s:\" >> %s 2>/dev/null",command,DUMP_DIAG_LOG_FILE);
		system(buff);
		sprintf(buff,"%s >> %s 2>&1",command,DUMP_DIAG_LOG_FILE);
		system(buff);
	}
}

static int rtk_wifi_read_reg_period(char*interf,char flag_dw,int start,int end,int*read_value)
{
	FILE *fp;
	int j;
	char *ptr;
	char buff[64];
	int orig0,orig1,orig2,orig3;
	for(j=start;j<=end;j+=4){
		if(flag_dw)
			sprintf(buff,"iwpriv %s read_reg dw,%x",interf,j);
		else
			sprintf(buff,"iwpriv %s read_reg b,%x",interf,j);
		if(fp = popen(buff, "r")){
			if (fgets(buff,50, fp) != NULL) {
				ptr=strstr(buff,"read_reg:");
				if(ptr){
					if(flag_dw){
						sscanf(ptr+9,"%d  %d  %d  %d",&orig0,&orig1,&orig2,&orig3);
						read_value[(j-start)/4] = (orig0<<24)|(orig1<<16)|(orig2<<8)|orig3;
					}
					else
						sscanf(ptr+9,"%d",read_value);
				}
				else{
					pclose(fp);
					return RTK_FAILED;
				}
			}
		}
		else 
			return RTK_FAILED;
		pclose(fp);
	}
	return RTK_SUCCESS;
}
void rtk_wifi_diagnostic()
{
	int i,j,k,log_count=0;
	char buff[1024];
	char interf[16];
	struct ifreq ifr;
	int skfd;
	int orig_value_838,orig_value_824,orig_value_82c,tmp_orig_value,tmp_addr;
	int orig_value[15];
	
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
PRINT_LOG_AGAIN:	
	for(i=0; i<2; i++){
		//interface wlan0/wlan1
		sprintf(interf,"wlan%d",i);
		strcpy(ifr.ifr_name, interf);
		if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
			continue;
		if(!(ifr.ifr_flags & IFF_UP))
			continue;
		sprintf(buff,"cat /proc/wlan%d/stats",i);
		rtk_dump_log_command(buff);
		sprintf(buff,"cat /proc/wlan%d/sta_info",i);
		rtk_dump_log_command(buff);
		sprintf(buff,"cat /proc/wlan%d/buf_info",i);
		rtk_dump_log_command(buff);
		sprintf(buff,"cat /proc/wlan%d/desc_info",i);
		rtk_dump_log_command(buff);
		sprintf(buff,"cat /proc/wlan%d/mib_misc",i);
		rtk_dump_log_command(buff);
		if(!log_count){
			sprintf(buff,"cat /proc/wlan%d/sta_dbginfo",i);
			rtk_dump_log_command(buff);
			sprintf(buff,"cat /proc/wlan%d/mib_all",i);
			rtk_dump_log_command(buff);
			//DIG AMPDU SIFS RETRY
			sprintf(buff,"echo \"DIG:\" >>%s",DUMP_DIAG_LOG_FILE);
			system(buff);
			sprintf(buff,"iwpriv wlan%d read_reg b,c50",i);
			rtk_dump_log_command(buff);
			sprintf(buff,"echo \"AMPDU:\" >>%s",DUMP_DIAG_LOG_FILE);
			system(buff);
			sprintf(buff,"iwpriv wlan%d read_reg dw,4c8",i);
			rtk_dump_log_command(buff);
			sprintf(buff,"echo \"SIFS:\" >>%s",DUMP_DIAG_LOG_FILE);
			system(buff);
			sprintf(buff,"iwpriv wlan%d read_reg b,63f",i);
			rtk_dump_log_command(buff);
			sprintf(buff,"echo \"Retry:\" >>%s",DUMP_DIAG_LOG_FILE);
			system(buff);
			sprintf(buff,"iwpriv wlan%d read_reg w,42a",i);
			rtk_dump_log_command(buff);
			//reg_dump/reg_dump all
			sprintf(buff,"cat /proc/wlan%d/reg_dump",i);
			rtk_dump_log_command(buff);
		}		
		//interface wlani-vaj
		for(j=0; j<4; j++){	
			sprintf(buff,"wlan%d-va%d",i,j);
			strcpy(ifr.ifr_name, buff);
			if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
				continue;
			if(!(ifr.ifr_flags & IFF_UP))
				continue;
			sprintf(buff,"cat /proc/wlan%d-va%d/stats",i,j);
			rtk_dump_log_command(buff);
			sprintf(buff,"cat /proc/wlan%d-va%d/sta_info",i,j);
			rtk_dump_log_command(buff);
			if(!log_count){
				sprintf(buff,"cat /proc/wlan%d-va%d/sta_dbginfo",i,j);
				rtk_dump_log_command(buff);
				sprintf(buff,"cat /proc/wlan%d-va%d/mib_all",i,j);
				rtk_dump_log_command(buff);		
			}
		}	
		//interface wlan0-vxd/wlan1-vxd
		sprintf(buff,"wlan%d-vxd",i);
		strcpy(ifr.ifr_name, buff);
		if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
			continue;
		if(!(ifr.ifr_flags & IFF_UP))
			continue;			
		sprintf(buff,"cat /proc/wlan%d-vxd/stats",i);
		rtk_dump_log_command(buff);
		sprintf(buff,"cat /proc/wlan%d-vxd/sta_info",i);
		rtk_dump_log_command(buff);
		if(!log_count){
			sprintf(buff,"cat /proc/wlan%d-vxd/sta_dbginfo",i);
			rtk_dump_log_command(buff);
			sprintf(buff,"cat /proc/wlan%d-vxd/mib_all",i);
			rtk_dump_log_command(buff);
		}
	}	
	if(++log_count < 3){
		sleep(1);
		goto PRINT_LOG_AGAIN;
	}
	close(skfd);
}

void rtk_sys_diagnostic()
{
	char buff[256]={0};
	char tmpbuf[64]={0};
	RTK_WAN_INFO wan_info={0};
	char * status_str[]={"Disconnected","fixed IP connected","fixed IP disconnected",
		"getting IP from DHCP server","DHCP","PPPoE connected","PPPoE disconnected",
		"PPTP connected","PPTP disconnected","L2TP connected","L2TP disconnected",
		"Brian 5BGG","USB3G connected","USB3G modem Initializing","USB3G dialing",
		"USB3G removed","USB3G disconnected"};
	sprintf(buff,"rm %s 2>/dev/null",DUMP_DIAG_LOG_FILE_BAK);
	system(buff);
	sprintf(buff,"cp %s %s 2>/dev/null",DUMP_DIAG_LOG_FILE,DUMP_DIAG_LOG_FILE_BAK);
	system(buff);
	sprintf(buff,"rm %s 2>/dev/null",DUMP_DIAG_LOG_FILE);
	system(buff);
	sprintf(buff,"echo \"###########################  system info  ############################\">>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	sprintf(buff,"echo \"fwVersion is %s\" >>%s",fwVersion,DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/version");
	rtk_dump_log_command("cat /etc/version");
	rtk_dump_log_command("free");
	rtk_dump_log_command("ps");
	rtk_dump_log_command("ifconfig");
	sprintf(buff,"echo \"###########################  wan setting and status  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_get_wan_info(&wan_info);
	sprintf(buff,"echo \"wan status=%s ip=%s ",status_str[wan_info.status],
		inet_ntoa(wan_info.ip));
	
	sprintf(tmpbuf,"mask=%s ",inet_ntoa(wan_info.mask));
	strcat(buff,tmpbuf);
	sprintf(tmpbuf,"def_gateway=%s ",inet_ntoa(wan_info.def_gateway));
	strcat(buff,tmpbuf);
	sprintf(tmpbuf,"dns1=%s ",inet_ntoa(wan_info.dns1));
	strcat(buff,tmpbuf);
	sprintf(tmpbuf,"dns2=%s \" >>%s",inet_ntoa(wan_info.dns2),DUMP_DIAG_LOG_FILE);
	strcat(buff,tmpbuf);
	system(buff);
	switch(wan_info.status)
	{
		case DISCONNECTED:
			break;
		case FIXED_IP_CONNECTED:
		case FIXED_IP_DISCONNECTED:
			break;
		case GETTING_IP_FROM_DHCP_SERVER:
		case DHCP:
			break;
		case PPPOE_CONNECTED:			
		case PPPOE_DISCONNECTED:			
			rtk_dump_log_command("cat /etc/ppp/chap-secrets");
			rtk_dump_log_command("cat /etc/ppp/options");
			break;
		case PPTP_CONNECTED:
		case PPTP_DISCONNECTED:
			break;
		case L2TP_CONNECTED:
		case L2TP_DISCONNECED:
			break;
		default:
			break;
	}
	rtk_dump_log_command("cat /etc/resolv.conf");
    rtk_dump_log_command("cat /proc/net/nf_conntrack");
	rtk_dump_log_command("route -n");
	rtk_dump_log_command("iptables -nvL");
	rtk_dump_log_command("iptables -t nat -nvL");
	rtk_dump_log_command("cat /proc/filter_table");
	sprintf(buff,"echo \"###########################  lan setting and status  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /var/udhcpd.conf");	
	rtk_dump_log_command("cat /proc/net/arp");
	rtk_dump_log_command("cat /proc/rtl865x/port_status");
	sprintf(buff,"echo \"########################### wifi status  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_wifi_diagnostic();

	sprintf(buff,"echo \"########################### first time system loading status(total 3 times)  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/stat");
	rtk_dump_log_command("cat /proc/softirqs");
	rtk_dump_log_command("cat /proc/interrupts");
	rtk_dump_log_command("cat /proc/net/softnet_stat");
	rtk_dump_log_command("cat /proc/meminfo");
	rtk_dump_log_command("cat /proc/uptime");
	rtk_dump_log_command("cat /proc/loadavg");

	sprintf(buff,"echo \"########################### ethernet status  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	sprintf(buff,"echo \"first time read  asicCounter(total 3 times)\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/rtl865x/asicCounter");
	rtk_dump_log_command("cat /proc/rtl865x/acl");
	rtk_dump_log_command("cat /proc/rtl865x/l2");
	rtk_dump_log_command("cat /proc/rtl865x/portRate");
	rtk_dump_log_command("cat /proc/rtl865x/storm_control");
	rtk_dump_log_command("cat /proc/rtl865x/arp");
	rtk_dump_log_command("cat /proc/rtl865x/l3");
	rtk_dump_log_command("cat /proc/rtl865x/port_bandwidth");
	rtk_dump_log_command("cat /proc/rtl865x/swMCast");
	rtk_dump_log_command("cat /proc/rtl865x/mac");
	rtk_dump_log_command("cat /proc/rtl865x/port_status");
	rtk_dump_log_command("cat /proc/rtl865x/sw_l2");
	rtk_dump_log_command("cat /proc/rtl865x/debug_mode");
	rtk_dump_log_command("cat /proc/rtl865x/pppoe");
	rtk_dump_log_command("cat /proc/rtl865x/sw_l3");
	rtk_dump_log_command("cat /proc/rtl865x/diagnostic");
	rtk_dump_log_command("cat /proc/rtl865x/memory");
	rtk_dump_log_command("cat /proc/rtl865x/priority_decision");
	rtk_dump_log_command("cat /proc/rtl865x/sw_napt");
	sprintf(buff,"echo \"second time read  asicCounter(total 3 times)\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/rtl865x/asicCounter");
	rtk_dump_log_command("cat /proc/rtl865x/eventMgr");
	rtk_dump_log_command("cat /proc/rtl865x/mirrorPort");
	rtk_dump_log_command("cat /proc/rtl865x/privSkbInfo");
	rtk_dump_log_command("cat /proc/rtl865x/sw_netif");
	rtk_dump_log_command("cat /proc/rtl865x/fc_threshold");
	rtk_dump_log_command("cat /proc/rtl865x/mmd");
	rtk_dump_log_command("cat /proc/rtl865x/pvid");
	rtk_dump_log_command("cat /proc/rtl865x/sw_nexthop");
	rtk_dump_log_command("cat /proc/rtl865x/hs");
	rtk_dump_log_command("cat /proc/rtl865x/napt");
	rtk_dump_log_command("cat /proc/rtl865x/queue_bandwidth");
	rtk_dump_log_command("cat /proc/rtl865x/hwMCast");
	rtk_dump_log_command("cat /proc/rtl865x/netif");
	rtk_dump_log_command("cat /proc/rtl865x/vlan");
	rtk_dump_log_command("cat /proc/rtl865x/igmp");
	rtk_dump_log_command("cat /proc/rtl865x/nexthop");
	rtk_dump_log_command("cat /proc/rtl865x/soft_aclChains");
	rtk_dump_log_command("cat /proc/rtl865x/ip");
	rtk_dump_log_command("cat /proc/rtl865x/phyReg");
	rtk_dump_log_command("cat /proc/rtl865x/stats");
	sprintf(buff,"echo \"third time read  asicCounter(total 3 times)\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/rtl865x/asicCounter");

	sprintf(buff,"echo \"########################### second time system loading status(total 3 times)  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/stat");
	rtk_dump_log_command("cat /proc/softirqs");
	rtk_dump_log_command("cat /proc/interrupts");
	rtk_dump_log_command("cat /proc/net/softnet_stat");
	rtk_dump_log_command("cat /proc/meminfo");
	rtk_dump_log_command("cat /proc/uptime");
	rtk_dump_log_command("cat /proc/loadavg");

	sprintf(buff,"echo \"########################### igmp status ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("ps");
    rtk_dump_log_command("brctl show");
	/*igmp proxy*/
	rtk_dump_log_command("cat /proc/net/ip_mr_vif");
	rtk_dump_log_command("cat /proc/net/ip_mr_cache");
	/*snooping*/
	rtk_dump_log_command("cat /proc/br_igmpsnoop");
	rtk_dump_log_command("cat /proc/br_igmpquery");
	rtk_dump_log_command("cat /proc/br_mCastFastFwd");
	rtk_dump_log_command("cat /proc/br_igmpsnoop");
	rtk_dump_log_command("cat /proc/br_mldquery");
	rtk_dump_log_command("cat /proc/br_mldsnoop");
	rtk_dump_log_command("cat /proc/br_igmpDb");
	/*m2u*/
	rtk_dump_log_command("cat /proc/wlan0/sta_info");
	rtk_dump_log_command("cat /proc/wlan1/sta_info");
	/*passthru*/
	rtk_dump_log_command("cat /proc/custom_Passthru");
	/*hw mcast*/
	rtk_dump_log_command("cat /proc/rtl865x/swMCast");

	
	sprintf(buff,"echo \"########################### qos status ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("ps");
    rtk_dump_log_command("ifconfig");
   	rtk_dump_log_command("flash get QOS_RULE_TBL");
	rtk_dump_log_command("flash get QOS_MONOPOLY_ENABLED");
	rtk_dump_log_command("flash get QOS_MONOPOLY_MAC");
	rtk_dump_log_command("flash get QOS_MONOPOLY_TIME");
	rtk_dump_log_command("flash get QOS_MONOPOLY_DEFAULT_BANDWIDTH");
	rtk_dump_log_command("cat /proc/qos");
	/*qos mnp*/
	rtk_dump_log_command("cat /var/qos_mon_info");
	/*uplink*/
	rtk_dump_log_command("tc qdisc show dev eth1");
	rtk_dump_log_command("tc class show dev eth1");
	rtk_dump_log_command("tc filter show dev eth1");
	
	rtk_dump_log_command("tc qdisc show dev ppp0");
	rtk_dump_log_command("tc class show dev ppp0");
	rtk_dump_log_command("tc filter show dev ppp0");
	/*down link*/
	rtk_dump_log_command("tc qdisc show dev br0");
	rtk_dump_log_command("tc class show dev br0");
	rtk_dump_log_command("tc filter show dev br0");
	
	rtk_dump_log_command("iptables -t mangle -nvL");
	
	sprintf(buff,"echo \"########################### tcp-ip protocal stack and fastpath status  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
    sprintf(buff,"ifconfig >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/net/arp");
	rtk_dump_log_command("cat /proc/net/dev");
	rtk_dump_log_command("cat /proc/net/route");
	rtk_dump_log_command("cat /proc/net/nf_conntrack");
	rtk_dump_log_command("cat /proc/sys/net/ipv4/ip_forward");
	rtk_dump_log_command("cat /proc/sys/net/nf_conntrack_max");

	rtk_dump_log_command("cat /proc/fast_nat");
	rtk_dump_log_command("cat /proc/fast_pppoe");
	rtk_dump_log_command("cat /proc/fast_l2tp");
	rtk_dump_log_command("cat /proc/fast_pptp");
	rtk_dump_log_command("cat /proc/fp_path");
	rtk_dump_log_command("cat /proc/fp_napt");
	rtk_dump_log_command("cat /proc/hw_nat");
	sprintf(buff,"echo \"########################### third time system loading status(total 3 times)  ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /proc/stat");
	rtk_dump_log_command("cat /proc/softirqs");
	rtk_dump_log_command("cat /proc/interrupts");
	rtk_dump_log_command("cat /proc/net/softnet_stat");
	rtk_dump_log_command("cat /proc/meminfo");
	rtk_dump_log_command("cat /proc/uptime");
	rtk_dump_log_command("cat /proc/loadavg");

	sprintf(buff,"echo \"########################### cgi nas ntp and url filter ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	rtk_dump_log_command("cat /tmp/cgiClientTmp");
	rtk_dump_log_command("cat /tmp/ntp_tmp");
	rtk_dump_log_command("cat /etc/TZ");
	rtk_dump_log_command("cat /proc/filter_table");

	sprintf(buff,"echo \"########################### get_terminal_info ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
	{
		unsigned int real_num=0,i=0;
		struct rtk_link_type_info *p = malloc(32*sizeof(struct rtk_link_type_info));					//malloc memory
		rtk_get_terminal_info(&real_num,p,32);//call function
		
		for(i=0;i<real_num;i++)
		{
			sprintf(buff,"echo \"%d: LINK_TYPE(%d)	IP(%s)	MAC(%02x%02x%02x%02x%02x%02x)	DOWNLOAD_SPEED(%u)	UPLOAD_SPEED(%u)	LASTRX(%u)	LASTTX(%u)	CURRENTRX(%u)	CURRENTTX(%u)	ALLBYTE(%u)	PORTNUM(%d)	BRAND(%s)	LINKTIME(%u)\" >>%s",
								  i,p[i].type,inet_ntoa(p[i].ip)
								  		,p[i].mac[0],p[i].mac[1],p[i].mac[2],p[i].mac[3],p[i].mac[4],p[i].mac[5],
								  															p[i].download_speed,p[i].upload_speed,p[i].last_rx_bytes,
								  																										p[i].last_tx_bytes,p[i].cur_rx_bytes,p[i].cur_tx_bytes,p[i].all_bytes,p[i].port_number,
								  																																												p[i].brand,p[i].link_time,DUMP_DIAG_LOG_FILE);
			system(buff);
		}
	}

	sprintf(buff,"echo \"########################### others ############################\" >>%s",DUMP_DIAG_LOG_FILE);
	system(buff);
   rtk_dump_log_command("flash all");
}

//out: percentage
unsigned int rtk_get_cpu_occupy(int *out)
{
	FILE *fp=NULL;
	char buf[10]={0};

	fp = fopen(CPU_OCCUPY_FILE, "r");
	if(!fp){
		printf("open %s fail\n", CPU_OCCUPY_FILE);
		*out = 1;
		return RTK_FAILED;
	}

	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	*out = atoi(buf);

	return RTK_SUCCESS;
}

//flashSize: Mbyte
int rtk_get_flash_size(int *flashSize)
{
	FILE *fp=NULL;
	unsigned char filename[50]={0}, buffer[100]={0}, tmpBuff[30]={0}, tmpBuff1[30]={0};
	int tmpVal=0, nValue=0, copy_size=8;	
	char *p, *pEnd;

	snprintf(filename, sizeof(filename), "%s", "/proc/mtd"); 
	fp = fopen(filename, "r");
	if(!fp){
		printf("open %s fail\n", filename);
		return RTK_FAILED;
	}
	while(fgets(buffer, sizeof(buffer), fp))
	{
		if(strstr(buffer, "mtd"))
		{			
			//eg: mtd0: 00260000 00010000 "boot+cfg+linux"
			p = buffer;
			while(*p != ':')
				p++;
			p++; //skip ':'			
			while(*p == ' ')
				p++;			
			
			strncpy(tmpBuff1, p, copy_size);			
			tmpVal = strtol(tmpBuff1, &pEnd, 16);
			//printf("0x%x\n", tmpVal);
			nValue += tmpVal;
		}
	}
	fclose(fp);
	
	*flashSize = nValue/1024/1024;
	//printf("[Flash]total size:0x%x, %d MB\n", nValue, *flashSize);

	return RTK_SUCCESS;
}

//Mbyte
int rtk_get_memory_user(MEMINFO_USER_Tp pInfo)
{
	FILE *fp=NULL;
	unsigned char filename[50]={0}, buffer[100]={0};
	int nValue=0;	

	snprintf(filename, sizeof(filename), "%s", "/proc/meminfo"); 
	fp = fopen(filename, "r");
	if(!fp){
		printf("open %s fail\n", filename);
		return RTK_FAILED;
	}
	while(fgets(buffer, sizeof(buffer), fp))
	{
		if(strstr(buffer, "MemFree:"))
		{
			getFileLineValue(buffer, &nValue);
			pInfo->mem_free = (nValue/1024);			
		}else if(strstr(buffer, "MemTotal:")){
			getFileLineValue(buffer, &nValue);
			pInfo->mem_userTotal = (nValue/1024);				
		}
	}
	fclose(fp);

	//printf("userTotal:%d MB, free:%d MB\n", pInfo->mem_userTotal,pInfo->mem_free);

	return RTK_SUCCESS;
}

//Size: MByte
int rtk_get_memory_kernel(int *Size)
{
	FILE *fp=NULL;
	unsigned char filename[50]={0}, buffer[100]={0}, buff1[20]={0};
	unsigned long nValue=0, nValue2;	
	char *pEnd=NULL, *p;
	int idx = 0, off=0, len, copy_size=8;
	char ch;

	snprintf(filename, sizeof(filename), "%s", "/proc/kallsyms"); 
	fp = fopen(filename, "r");
	if(!fp){
		printf("open %s fail\n", filename);
		return RTK_FAILED;
	}

	//first line
	fseek(fp, 0, SEEK_SET);
	if(fgets(buffer, sizeof(buffer), fp)){		
		nValue = strtoul(buffer, &pEnd, 16);		
		//printf("start:0x%08x\n", nValue);		
	}
	
	//last line
#if 0
	len = strlen(" B _end\n");	
	off -= (len+8);
	memset(buffer, 0, sizeof(buffer));
	fseek(fp, off, SEEK_END);
	while ((ch = (char)fgetc(fp)) != '\n')
    {     
		buffer[idx] = ch;
 		off++;
		idx++;
        fseek(fp, off, SEEK_END);		
    }
#else
	while(!feof(fp))  
	{
		fgets(buffer,sizeof(buffer),fp); 
		if(feof(fp))    
			break;
	}
#endif	
	nValue2 = strtoul(buffer, &pEnd, 16);
	//printf("end:0x%08x\n", nValue2);
	
	fclose(fp);	

	*Size = (nValue2-nValue)/1024/1024;
	//printf("Kernel Size:%d MB\n", *Size);

	return RTK_SUCCESS;
}

//totalSize: MByte
int rtk_get_memory_total(int *totalSize)
{
	MEMINFO_USER_T meminfo;	
	int kernel_size=0, tmpValue=0, tmpValue1;
	int i=0, offset=5, loop_cnt=20;

	memset(&meminfo, 0, sizeof(MEMINFO_USER_T));
	rtk_get_memory_kernel(&kernel_size);
	rtk_get_memory_user(&meminfo);
	
	tmpValue = kernel_size + meminfo.mem_userTotal;
	for(i=0; i<loop_cnt; ++i){
		tmpValue1 = 1<<i;
		if(abs(tmpValue1-tmpValue)<offset){
			*totalSize = tmpValue1;
			break;
		}
	}
	//printf("total mem:%d MB\n", *totalSize);

	return RTK_SUCCESS;
}

int rtk_get_system_mem_info(SYS_MEMINFO_Tp pmem_info)
{
	int tmpValue;
	MEMINFO_USER_T userInfo;
	
	if(pmem_info==NULL){
		printf("[%s]pmem_info NULL\n", __FUNCTION__);
		return RTK_FAILED;
	}

	memset(&userInfo, 0, sizeof(MEMINFO_USER_T));
	rtk_get_memory_total(&tmpValue);
	pmem_info->ramSizeTotal = tmpValue;
	rtk_get_memory_user(&userInfo);
	pmem_info->ramSizeFree = userInfo.mem_free;
	pmem_info->ramUtility = 100-(pmem_info->ramSizeFree*100 /pmem_info->ramSizeTotal);
	rtk_get_flash_size(&tmpValue);
	pmem_info->flashSizeTotal = tmpValue;	

	//printf("[System]RAM:%d MB, Free:%d MB, Utility:%d, Flash:%d MB\n", 
	//	pmem_info->ramSizeTotal, pmem_info->ramSizeFree, pmem_info->ramUtility, pmem_info->flashSizeTotal);
	
	return RTK_SUCCESS;	
}

