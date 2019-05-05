#include "rtk_common_utility.h"
#include "rtk_ethernet_adapter.h"

static int string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
				
	}
	return 1;
}


int rtk_change_mac_to_octe(char *str_mac, char *octe_mac)
{	

	int i, j;
	char tmp_str_mac[13]={0};
	if(str_mac==NULL || octe_mac==NULL)
		return 0;

	for(i=0, j=0; i<17 && j<12; i++)
	{
		if(str_mac[i]!=':')
			tmp_str_mac[j++]=str_mac[i];
	}
	tmp_str_mac[12]=0;	
		
	if (strlen(tmp_str_mac)==12 && string_to_hex(tmp_str_mac, octe_mac, 12))
		return 1;

	return 0;
}

int rtk_get_arp_table_list(RTK_ARP_TAB_INFO *pArpTabInfo, int array_len, int *real_num)
{
	if(pArpTabInfo==NULL || array_len<1 || real_num==NULL)
		return RTK_FAILED; 

	FILE *fp;
	char line_buffer[512]={0};	
	char mac_str[13], tmp_mac_str[18];
	char ip_str[16], if_name[16];
	unsigned char mac_addr[6];
	int idx=0, i, j;	
	char *pchar, *pstart, *pend;
	struct in_addr ip_addr;

	memset(pArpTabInfo, 0, sizeof(RTK_ARP_TAB_INFO)*array_len);
	
	if((fp= fopen("/proc/net/arp", "r"))==NULL)
		return RTK_FAILED;	
	
	while(fgets(line_buffer, sizeof(line_buffer), fp) && idx<array_len)
	{			
		line_buffer[strlen(line_buffer)-1]='\0';		

		sscanf(line_buffer,"%s %*s %*s %s %*s %s",ip_str,tmp_mac_str,if_name);
		if(strcmp(if_name, "br0")!=0)
			continue;

		inet_aton(ip_str, &ip_addr);
		pArpTabInfo[idx].ip=ip_addr.s_addr;

		if(rtk_change_mac_to_octe(tmp_mac_str, mac_addr))
		{
			memcpy(pArpTabInfo[idx].mac, mac_addr, MAC_ADDR_LEN);
			//printf("\n from arp table: ip=%s mac=%02X:%02X:%02X:%02X:%02X:%02X\n",inet_ntoa(*((struct in_addr *)&pArpTabInfo[idx].ip)),pArpTabInfo[idx].mac[0],pArpTabInfo[idx].mac[1],pArpTabInfo[idx].mac[2],pArpTabInfo[idx].mac[3],pArpTabInfo[idx].mac[4],pArpTabInfo[idx].mac[5]);
			idx++;
		}		
	}
	fclose(fp);

	*real_num=idx;
	
	return RTK_SUCCESS;		
}


int rtk_ethernet_get_port_status(int port_num, enum RTK_LINK_STATUS *link_status)
{
	if(port_num<0 || port_num>MAX_PORT_NUMBER || link_status==NULL)
		return RTK_FAILED;

	FILE *fp;
	char cur_port[16]={0};
	char buffer[128]={0};
	char next_buffer[128]={0};

	sprintf(cur_port,"Port%d", port_num);	
	if((fp = fopen("/proc/rtl865x/port_status", "r"))==NULL)
	{
		printf("\nCan not open /proc/rtl865x/port_status!\n");
		return RTK_FAILED;
	}

	while(fgets(buffer, 128, fp))
	{
		if(strncmp(buffer, cur_port, strlen(cur_port)) == 0)
		{
			fgets(next_buffer, 128, fp);
			if(strstr(buffer,"LinkUp") || strstr(next_buffer,"LinkUp"))
				*link_status=LINK_UP;
			else if(strstr(buffer,"LinkDown"))
				*link_status=LINK_DOWN;
			else
				*link_status=LINK_ERROR;
		}
	}
	fclose(fp);
	return RTK_SUCCESS;	
}

int rtk_ethernet_get_l2_list(RTK_L2_TAB_INFO *pL2List, int array_len, int *real_num, int wanportnum, int flag)
{
	if(pL2List==NULL || array_len<1 || real_num==NULL)
		return RTK_FAILED; 

	FILE *fp;
	char line_buffer[512]={0};	
	char mac_str[13];
	char filter_wan_str[10];
	unsigned char mac_addr[6];
	int idx=0, i, j;	
	char *pchar, *pstart;	
	int port_num;
	enum RTK_LINK_STATUS link_status=LINK_ERROR;
	int including_wan, including_cpu_port;

	if(flag & 0x1)
		including_wan = 1;
	else
		including_wan = 0;

	if(flag & 0x2)
		including_cpu_port = 1;
	else
		including_cpu_port = 0;

	snprintf(filter_wan_str,sizeof(filter_wan_str),"mbr(%d",wanportnum);

	memset(pL2List, 0, sizeof(RTK_L2_TAB_INFO)*array_len);
	
	if((fp= fopen("/proc/rtl865x/l2", "r"))==NULL)
		return RTK_FAILED;	
	
	while(fgets(line_buffer, sizeof(line_buffer), fp) && idx<array_len)
	{			
		line_buffer[strlen(line_buffer)-1]='\0';

		if(strstr(line_buffer, "ff:ff:ff:ff:ff:ff") || strstr(line_buffer, "CPU") || (!including_wan && strstr(line_buffer, "FID:1")) || (!including_wan && strstr(line_buffer, filter_wan_str)) || (!including_cpu_port && strstr(line_buffer, "mbr(8")))
			continue;	

		pchar = strstr(line_buffer,"mbr");
		if(pchar==NULL)
			continue;
		sscanf(pchar,"mbr(%d",&port_num);	
		//printf("\nport_num=%d\n",port_num);

		rtk_ethernet_get_port_status(port_num, &link_status);
		if(link_status!=LINK_UP)
		{
			//printf("\nport %d is not link up!\n",port_num);
			continue;
		}		

		pchar=strchr(line_buffer, ':');
		if(pchar==NULL)
			continue;
		
		pstart=pchar-2;		

		if(rtk_change_mac_to_octe(pstart, mac_addr))
		{
			memcpy(pL2List[idx].mac, mac_addr, MAC_ADDR_LEN);
			
			//printf("\nfrom l2 table:mac=%02X:%02X:%02X:%02X:%02X:%02X\n",pL2List[idx].mac[0],pL2List[idx].mac[1],pL2List[idx].mac[2],pL2List[idx].mac[3],pL2List[idx].mac[4],pL2List[idx].mac[5]);
			
			pL2List[idx].port_num=port_num;
			idx++;			
		}		
	}
	fclose(fp);
	*real_num=idx;
	return RTK_SUCCESS;		
}

#if defined(CONFIG_RTL_8367R_SUPPORT) || defined(CONFIG_RTL_83XX_SUPPORT)
int rtk_ethernet_get_port_statistics(int port_num, unsigned int *tx_bytes, unsigned int *rx_bytes)
{
	if(port_num<0 || port_num>MAX_PORT_NUMBER || tx_bytes==NULL || rx_bytes==NULL)
		return RTK_FAILED;

	FILE *fp=NULL;
	int  line_cnt =0;
	char buffer[256]={0};
	
	if((fp = fopen("/proc/rtl865x/switch_asicCounter","r")) == NULL)
		return RTK_FAILED;	
		
	while(fgets(buffer, sizeof(buffer), fp))
	{
		line_cnt++;
		if(line_cnt == 7*port_num+4)	//update receive bytes
		{
			//printf("\nrx buffer=%s\n",buffer);
			sscanf(buffer," ifInOctets  %u",rx_bytes);
		}
		
		if(line_cnt == 7*port_num+7)	//update send bytes
		{
			//printf("\ntx buffer=%s\n",buffer);
			sscanf(buffer," ifOutOctets  %u",tx_bytes);
			break;
		}
	}
	fclose(fp);
	return RTK_SUCCESS;	
}

#else

int rtk_ethernet_get_port_statistics(int port_num, unsigned int *tx_bytes, unsigned int *rx_bytes)
{
	if(port_num<0 || port_num>MAX_PORT_NUMBER || tx_bytes==NULL || rx_bytes==NULL)
		return RTK_FAILED;

	FILE *fp=NULL;
	int  line_cnt =0;
	char buffer[256]={0};
	
	if((fp = fopen("/proc/rtl865x/asicCounter","r")) == NULL)
		return RTK_FAILED;	
		
	while(fgets(buffer, sizeof(buffer), fp))
	{
		line_cnt++;
		if(line_cnt == 12*port_num+3)	//update receive bytes
			sscanf(buffer," Rcv %u ",rx_bytes);
		
		if(line_cnt == 12*port_num+10)	//update send bytes
		{
			sscanf(buffer," Snd %u ",tx_bytes);
			break;
		}
	}
	fclose(fp);
	return RTK_SUCCESS;	
}
#endif

int rtk_ethernet_get_port_speed(int port_num, unsigned int *up_speed, unsigned int *down_speed)
{
	if(port_num<0 || port_num>4 || up_speed==NULL || down_speed==NULL)
		return RTK_FAILED;
	
	int tmp_port=0;
	char buffer[64]={0};
	FILE *fp=NULL;
	if((fp = fopen(ETH_PORT_INFO,"r")) == NULL)
		return RTK_FAILED;	

	while(fgets(buffer, sizeof(buffer), fp)) {
		sscanf(buffer, "port_num=%d,up_speed=%u,down_speed=%u",&tmp_port, up_speed, down_speed);
		if(tmp_port==port_num)
			break;
	}

	printf("\n%s:%d port_num=%d up_speed=%u down_speed=%u\n",__FUNCTION__,__LINE__,port_num,*up_speed,*down_speed);
	fclose(fp);
	return RTK_SUCCESS;		
}

int rtk_ethernet_get_wan_port_num()
{
	int wan_port=4; /*default is 4*/
	char buffer[64]={0};
	int portMask=0;
	FILE *fp=NULL;
	if((fp = fopen("/proc/rtl865x/stats","r")) == NULL)
		return RTK_FAILED;	

	while(fgets(buffer, sizeof(buffer), fp))
	{
		if(strstr(buffer,"WanPortMask:"))
		{
			sscanf(buffer," WanPortMask: %u",&portMask);
			if(ffs(portMask))
				wan_port=ffs(portMask)-1;
			break;
		}
	}

	fclose(fp);
	return wan_port;
}

int rtk_ethernet_find_port_by_mac(unsigned char *mac, int *port)
{
	RTK_L2_TAB_INFO l2_tab[128];
	int l2_tab_num=0;
	int i=0;
	
	memset(l2_tab, 0, sizeof(RTK_L2_TAB_INFO)*128);
	if(rtk_ethernet_get_l2_list(l2_tab, 128, &l2_tab_num, rtk_ethernet_get_wan_port_num(),1)==RTK_FAILED)
		return RTK_FAILED;

	for(i=0;i<l2_tab_num;i++)
	{
		if(memcmp(mac,l2_tab[i].mac,MAC_ADDR_LEN)==0)
		{
			*port=l2_tab[i].port_num;
			return RTK_SUCCESS;
		}
	}
}

int rtk_set_lock(int fd, int lock_type)
{
	int ret;
	struct flock lock;
	
    lock.l_type = lock_type;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
	
try_again:
	ret=fcntl(fd,lock_type,&lock);

 	if(ret == -1) 
	{		
		printf("\nerrno:%d\n",errno);
		if (errno == EINTR) 
		{
			printf("try again\n");
			goto try_again;
		}
	}		
}



