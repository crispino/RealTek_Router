/*LAN API*/

#include <netinet/in.h>
#include "rtk_api.h"
#include "rtk_adapter.h"
#include "rtk_common_utility.h"
#include "rtk_ethernet_adapter.h"
//#define RTK_API_DEBUG
extern int rtk_get_arp_table_list(RTK_ARP_TAB_INFO *pArpTabInfo, int array_len, int *real_num);
extern int rtk_change_mac_to_octe(char *str_mac, char *octe_mac);
extern int rtk_ethernet_get_wan_port_num();
extern int rtk_ethernet_get_l2_list(RTK_L2_TAB_INFO *pL2List, int array_len, int *real_num, int wanportnum, int flag);

#ifdef RTK_API_DEBUG
#define RTK_API_DBG_PRINT(fmt, args...)	\
		printf("[rtk_api]%s: " fmt "\n" , __FUNCTION__ , ## args)
#else
#define RTK_API_DBG_PRINT(fmt, args...)	do {} while (0)
#endif

#define RTK_API_ERR_PRINT(fmt, args...)	\
		printf("[rtk_api] err: %s: " fmt "\n" , __FUNCTION__ , ## args)

static int checkSameIpOrMac(struct in_addr *IpAddr, char *macAddr, int entryNum)
{
	if(IpAddr==NULL || macAddr==NULL || entryNum<1)
		return 0;
	int i;
	DHCPRSVDIP_T entry;
		
	for (i=1; i<=entryNum; i++) 
	{
		*((char *)&entry) = (char)i;
		if(!apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry))
		{
			printf("get mib MIB_DHCPRSVDIP_TBL fail!\n");
			return -1;
		}
		if(memcmp(IpAddr, entry.ipAddr, 4)==0)
			return 1;
		if(memcmp(macAddr, entry.macAddr, 6)==0)
			return 2;
	}
	return 0;
}

/*
  *@name rtk_set_lan_ip
  *@ input 
  	p_static_config struct rtk_static_config* including the lan static config of ip,mask,gw
  *@ output
  	none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */
int rtk_set_lan_ip(struct rtk_static_config *p_static_config)
{
	struct in_addr addr;
	if(p_static_config == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	addr.s_addr = p_static_config->ip;
	if (!apmib_set(MIB_IP_ADDR, (void *)&addr))
	{
		printf("Set lan ip fail\n");
		return RTK_FAILED;
	}
	addr.s_addr = p_static_config->mask;
	if(!apmib_set(MIB_SUBNET_MASK,  (void *)&addr))
	{
		printf("Set lan mask fail!\n");
		return RTK_FAILED;
	}
	addr.s_addr = p_static_config->gw;
	if(!apmib_set(MIB_DEFAULT_GATEWAY,  (void*)&addr))
	{
		printf("Set default gw fail!\n");
		return RTK_FAILED;
	}
	RTK_API_DBG_PRINT("success!\n");
	return RTK_SUCCESS;
}

/*
  *@name rtk_get_lan_ip
  *@ input 
	 p_static_config struct rtk_static_config* including the lan static config of ip,mask,gw
  *@output
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */

int rtk_get_lan_ip(struct rtk_static_config *p_static_config)
{
	if(p_static_config == NULL)
		return RTK_FAILED;
	memset(p_static_config, 0 ,sizeof(struct rtk_static_config));

	char *BRIDGE_IF;
	DHCP_T dhcp;
	struct in_addr	intaddr;
	
#ifdef HOME_GATEWAY
	BRIDGE_IF = "br0";
#elif defined(VOIP_SUPPORT) && defined(ATA867x)
	BRIDGE_IF = "eth0";
#else
	BRIDGE_IF = "br0";
#endif

	if ( !apmib_get( MIB_DHCP, (void *)&dhcp) )
		return RTK_FAILED;

	// get ip
	intaddr.s_addr = 0;
	if ( rtk_getInAddr(BRIDGE_IF, IP_ADDR_T, (void *)&intaddr ) )
		p_static_config->ip = intaddr.s_addr;
	else
		p_static_config->ip = 0;

	// get mask
	intaddr.s_addr = 0;
	if ( rtk_getInAddr(BRIDGE_IF, NET_MASK_T, (void *)&intaddr ))
		p_static_config->mask = intaddr.s_addr;
	else
		p_static_config->mask = 0;

	// get default gateway
	intaddr.s_addr = 0;
	if ( dhcp == DHCP_SERVER )
	{
		if ( rtk_getInAddr(BRIDGE_IF, IP_ADDR_T, (void *)&intaddr ) )
			p_static_config->gw = intaddr.s_addr;
		else
			p_static_config->gw = 0;
	}
	else if ( getDefaultRoute(BRIDGE_IF, &intaddr) )
		p_static_config->gw = intaddr.s_addr;
	else
		p_static_config->gw = 0;

	return RTK_SUCCESS;
}


/*
  *@name rtk_set_lan_dhcp
  *@ input 
  	p_config,struct rtk_lan_dhcp_config* including:
  	type,  unsigned int ,  type means diabled-0,dhcpc-1, dhcpd-2
  	start_ip,       unsigned int,        the start ip address of dhcp pool
  	end_ip,   	     unsigned int,        the end ip address of dhcp pool
  	lease_time,  unsigned int,        the lease time in seconds
  	the parameter should depends on Type.
  *@output
 	none	
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */
int rtk_set_lan_dhcp(struct rtk_lan_dhcp_config *p_config)
{
	struct in_addr start_addr,end_addr;
	DHCP_T dhcp_mode;
	if(p_config == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	if(p_config->type < 0 || p_config->type > 2)
	{
		printf("Invalid dhcp mode.\n");
		return RTK_FAILED;
	}
	if(p_config->start_ip > p_config->end_ip)
	{
		printf("Invalid ip range.\n");
		return RTK_FAILED;
	}
	if(p_config->lease_time < 1 || p_config->lease_time > 10080)
	{
		printf("Invalid lease time!\n");
		return RTK_FAILED;
	}
	dhcp_mode = (DHCP_T)p_config->type;
	RTK_API_DBG_PRINT("dhcp type:%d\n",dhcp_mode);
	/*type*/
	if (!apmib_set(MIB_DHCP, (void *)&dhcp_mode)) 
	{
		printf("Set dhcp type failed.\n");
		return RTK_FAILED;
	}
	/*start_ip,end_ip,lease_time*/
	if(dhcp_mode == DHCP_SERVER)
	{
		start_addr.s_addr = p_config->start_ip;
		end_addr.s_addr = p_config->end_ip;
		
		if (!apmib_set(MIB_DHCP_CLIENT_START, (void *)&start_addr)) 
		{
			printf("Set DHCP client start address error!\n");
			return RTK_FAILED;
		}
		if(!apmib_set(MIB_DHCP_CLIENT_END,(void*)&end_addr))
		{
			printf("Set DHCP client end address error!\n");
			return RTK_FAILED;
		}
		if(!apmib_set(MIB_DHCP_LEASE_TIME,(void*)&p_config->lease_time))
		{
			printf("Set DHCP lease time failed!\n");
			return RTK_FAILED;
		}
	}
	return RTK_SUCCESS;			
}

/*
  *@name rtk_get_lan_dhcp
  *@ input 
     none
  *@output
	p_config,struct rtk_lan_dhcp_config* including:
  	type,  unsigned int* ,  type means diabled-0,dhcpc-1, dhcpd-2
  	start_ip,       unsigned int,        the start ip address of dhcp pool
  	end_ip,   	     unsigned int,        the end ip address of dhcp pool
  	lease_time,  unsigned int,        the lease time in seconds
  	the parameter should depends on Type.
  *@output
 	none	
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */

int rtk_get_lan_dhcp(struct rtk_lan_dhcp_config *p_config)
{
	int lan_dhcp_mode = 0;
	int intVal;
	char tmpBuffer[100];
	struct in_addr *addr;
	if(p_config == NULL)
	{
		printf("Invalid input.\n");
		return RTK_FAILED;
	}
	/*type*/
	if(!apmib_get(MIB_DHCP, (void *)&lan_dhcp_mode))
	{
		printf("Get dhcp type failed.\n");
		return RTK_FAILED;
	}
	else
	{
		p_config->type = (unsigned int)lan_dhcp_mode;
	}
	
	/*start_ip*/
	memset(tmpBuffer,0,sizeof(tmpBuffer));
	if(!apmib_get(MIB_DHCP_CLIENT_START,(void*)&tmpBuffer))
	{
		printf("Get DHCP client start address error!\n");
		return RTK_FAILED;
	 }
	else
	{
		addr = (struct in_addr*)tmpBuffer;
		p_config->start_ip = addr->s_addr;
	}
	/*end_ip*/
	memset(tmpBuffer,0,sizeof(tmpBuffer));
	if(!apmib_get(MIB_DHCP_CLIENT_END,(void*)&tmpBuffer))
	{
		printf("Get DHCP client end address error!\n");
		return RTK_FAILED;
	}
	else
	{
		addr = (struct in_addr*)tmpBuffer;
		p_config->end_ip = addr->s_addr;
	}
	/*lease_time*/
	if(!apmib_get(MIB_DHCP_LEASE_TIME,(void*)&intVal))
	{
		printf("Get DHCP lease time err!\n");
		return RTK_FAILED;
	}
	else
	{
		if(intVal <= 0 || intVal > 10080)
		{
			intVal = 480;
			if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&intVal))
			{
				printf("set MIB_DHCP_LEASE_TIME error\n");
				return RTK_FAILED;
			}
		}
		p_config->lease_time = (unsigned int)intVal;
	}
	return RTK_SUCCESS;
}


/*
  *@name rtk_add_lan_static_dhcp
  *@ input 
     rtk_static_lease *, the pointer of static_lease which specific the MAC and IP mapping
  *@output
     none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */

int rtk_add_lan_static_dhcp(struct rtk_static_lease * s_lease)
{
	char buffer[100];
	int entryNum;
	DHCPRSVDIP_T staticIPEntry;
	struct in_addr inIp;
	struct in_addr inLanaddr_orig;
	struct in_addr inLanmask_orig;
	int retval;
	if(s_lease == NULL || s_lease->mac == NULL)
	{
		printf("Invalid input!\n");
		return RTK_FAILED;
	}
	//apmib_get(MIB_IP_ADDR,  (void *)buffer); //save the orig lan subnet
	rtk_getInAddr("br0", IP_ADDR_T, (void *)buffer);
	memcpy((void *)&inLanaddr_orig, buffer, 4);
	
	//apmib_get( MIB_SUBNET_MASK,  (void *)buffer); //save the orig lan mask
	rtk_getInAddr("br0", NET_MASK_T, (void *)buffer);
	memcpy((void *)&inLanmask_orig, buffer, 4);

	memset(&staticIPEntry, '\0', sizeof(staticIPEntry));	
	inIp.s_addr = s_lease->ip;
	memcpy(staticIPEntry.ipAddr, &inIp, 4);
//	strcpy((char*)staticIPEntry.hostName,(char*)s_lease->hostName);
	memcpy(staticIPEntry.macAddr,s_lease->mac,6);
	if (!apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entryNum)) 
	{
		printf("Get entry number error!");
		return RTK_FAILED;
	}
	if ((entryNum + 1) > MAX_DHCP_RSVD_IP_NUM) 
	{
		printf("Cannot add new entry because table is full!\n");
		return RTK_FAILED;
	}
	if((inLanaddr_orig.s_addr & inLanmask_orig.s_addr) != (inIp.s_addr & inLanmask_orig.s_addr))
	{
		printf("Cannot add new entry because the ip is not the same subnet as LAN network!\n");
		return RTK_FAILED;
	}
	retval=checkSameIpOrMac(&inIp, staticIPEntry.macAddr, entryNum);
	if(retval>0)
	{
		if(retval==1)
			printf("This IP address has been setted!\n");
		if(retval==2)				
			printf("This MAC address has been setted!\n");
		return RTK_FAILED;
	}
	// set to MIB. try to delete it first to avoid duplicate case
	apmib_set(MIB_DHCPRSVDIP_DEL, (void *)&staticIPEntry);
	if (apmib_set(MIB_DHCPRSVDIP_ADD, (void *)&staticIPEntry) == 0) 
	{
		printf("Add table entry error!\n");
		return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

#define DHCPD_CONF_FILE "/var/udhcpd.conf"
int rtk_add_lan_static_dhcp_immediately(struct rtk_static_lease * s_lease)
{
	if(rtk_add_lan_static_dhcp(s_lease)==RTK_FAILED)
		return RTK_FAILED;
	
	char line_buffer[128], domain_name[64];
	int i, entry_Num, ret;
	int intValue=0;
	unsigned int lease_time;
	struct in_addr br0_ip, br0_netmask, br0_netip, start_addr, end_addr;
	DHCPRSVDIP_T staticIPEntry;

	system("killall -9 udhcpd 2> /dev/null");
	system("rm -f /var/run/udhcpd.pid 2> /dev/null");
	system("rm -f /var/udhcpd.conf");
	
	rtk_getInAddr("br0", IP_ADDR_T, (void *)&br0_ip);	
	rtk_getInAddr("br0", NET_MASK_T, (void *)&br0_netmask);
	br0_netip.s_addr=br0_ip.s_addr & br0_netmask.s_addr;
	
		
	sprintf(line_buffer,"interface %s\n","br0");
	write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);
		
	apmib_get(MIB_DHCP_CLIENT_START,  (void *)&start_addr);		
	if((start_addr.s_addr & br0_netmask.s_addr) != br0_netip.s_addr)
		start_addr.s_addr=br0_netip.s_addr | (start_addr.s_addr & (~br0_netmask.s_addr));
	sprintf(line_buffer,"start %s\n",inet_ntoa(start_addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
	apmib_get(MIB_DHCP_CLIENT_END,  (void *)&end_addr);	
	if((end_addr.s_addr & br0_netmask.s_addr) != br0_netip.s_addr)
		end_addr.s_addr=br0_netip.s_addr | (end_addr.s_addr & (~br0_netmask.s_addr));
	sprintf(line_buffer,"end %s\n",inet_ntoa(end_addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);		
	
	sprintf(line_buffer,"opt subnet %s\n",inet_ntoa(br0_netmask));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&lease_time);
	if(lease_time<=0 || lease_time>10080)
	{
		lease_time = 480; //8 hours
		if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&lease_time))
				printf("set MIB_DHCP_LEASE_TIME error\n");
	}
	lease_time *= 60;
	sprintf(line_buffer,"opt lease %u\n",lease_time);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	sprintf(line_buffer,"opt router %s\n",inet_ntoa(br0_ip));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	sprintf(line_buffer,"opt dns %s\n",inet_ntoa(br0_ip)); 
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
	memset(domain_name, 0, sizeof(domain_name));
	apmib_get(MIB_DOMAIN_NAME, (void *)domain_name);
	if(domain_name[0])
	{
		sprintf(line_buffer,"opt domain %s\n",domain_name);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
	
	/*static dhcp entry static_lease*/
#ifdef SUPPORT_DHCP_PORT_IP_BIND
	apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
	if(entry_Num>0)
	{
		for (i=1; i<=entry_Num; i++) 
		{
			*((char *)&staticIPEntry) = (char)i;
			apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&staticIPEntry);
			if(((*(unsigned int*)(staticIPEntry.ipAddr)) & br0_netmask.s_addr)!=br0_netip.s_addr)
				(*(unsigned int*)(staticIPEntry.ipAddr))=br0_netip.s_addr | ((*(unsigned int*)(staticIPEntry.ipAddr)) & (~br0_netmask.s_addr));

			if(staticIPEntry.portId>0)
			{					
				sprintf(line_buffer, "static_lease %d %s\n", staticIPEntry.portId, inet_ntoa(*((struct in_addr*)staticIPEntry.ipAddr)));
				write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			}					
		}
	}
#endif
	apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intValue);
	if(intValue==1)
	{
		apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
		if(entry_Num>0)
		{
			for (i=1; i<=entry_Num; i++) 
			{				
				*((char *)&staticIPEntry) = (char)i;
				apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&staticIPEntry);
				if(((*(unsigned int*)(staticIPEntry.ipAddr)) & br0_netmask.s_addr)!=br0_netip.s_addr)
					(*(unsigned int*)(staticIPEntry.ipAddr))=br0_netip.s_addr | ((*(unsigned int*)(staticIPEntry.ipAddr)) & (~br0_netmask.s_addr));			
				if(memcmp(staticIPEntry.macAddr,"\x00\x00\x00\x00\x00\x00",6))
				{
					sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", staticIPEntry.macAddr[0], staticIPEntry.macAddr[1], staticIPEntry.macAddr[2],
					staticIPEntry.macAddr[3], staticIPEntry.macAddr[4], staticIPEntry.macAddr[5], inet_ntoa(*((struct in_addr*)staticIPEntry.ipAddr)));
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}		
			}
		}
	}	
	sprintf(line_buffer, "udhcpd %s", DHCPD_CONF_FILE);
	system(line_buffer);

	//system("ifconfig wlan0 down");
	//system("ifconfig wlan1 down");
	//system("ifconfig eth0 down");
	//system("ifconfig eth2 down");
	//system("ifconfig eth3 down");
	//system("ifconfig eth4 down");

	//sleep(5);
	
	//system("ifconfig wlan0 up");
	//system("ifconfig wlan1 up");
	//system("ifconfig eth0 up");
	//system("ifconfig eth2 up");
	//system("ifconfig eth3 up");
	//system("ifconfig eth4 up");	

	return RTK_SUCCESS; 
}


/*
  *@name rtk_del_lan_static_dhcp
  *@ input 
     rtk_static_lease *, the pointer of static_lease which specific the MAC and IP mapping
     delall: delete flag
		 0: delete an entry, 1: delete all entry 
  *@output
     none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */
int rtk_del_lan_static_dhcp(struct rtk_static_lease * s_lease,unsigned int delall)
{
	DHCPRSVDIP_T delEntry;
	struct in_addr inIp;
	char buffer[100];
	int entryNum,i;
	struct in_addr * addr;
	if(delall)
	{
		memset(&delEntry, '\0', sizeof(delEntry));	
		if(!apmib_set(MIB_DHCPRSVDIP_DELALL, (void *)&delEntry))
		{
			printf("Delete table entry error!\n");
			return RTK_FAILED;
		}
		return RTK_SUCCESS;
	}
	if(!apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entryNum))
	{
		printf("Get entry number error!");
		return RTK_FAILED;
	}
	for (i=entryNum; i>0; i--) 
	{
		memset(&delEntry, '\0', sizeof(delEntry));	
		*((char *)&delEntry) = (char)i;
		if(!apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&delEntry))
		{
			printf("Failed to get table entry!\n");
			return RTK_FAILED;
		}
		addr = (struct in_addr*)delEntry.ipAddr;
		//if(addr->s_addr == s_lease->ip && !memcmp(delEntry.macAddr,s_lease->mac,6))
		if(!memcmp(delEntry.macAddr,s_lease->mac,6))
		{
			if(!apmib_set(MIB_DHCPRSVDIP_DEL, (void *)&delEntry))
			{
				printf("Delete table entry error!\n");
				return RTK_FAILED;
			}
			return RTK_SUCCESS;
		}
	}
	return RTK_SUCCESS;
}

int rtk_del_lan_static_dhcp_immediately(struct rtk_static_lease * s_lease,unsigned int delall)
{
	if(rtk_del_lan_static_dhcp(s_lease, delall)==RTK_FAILED)
		return RTK_FAILED;
	
	char line_buffer[128], domain_name[64];
	int i, entry_Num, ret;
	int intValue=0;
	unsigned int lease_time;
	struct in_addr br0_ip, br0_netmask, br0_netip, start_addr, end_addr;
	DHCPRSVDIP_T staticIPEntry;

	system("killall -9 udhcpd 2> /dev/null");
	system("rm -f /var/run/udhcpd.pid 2> /dev/null");
	system("rm -f /var/udhcpd.conf");
	
	rtk_getInAddr("br0", IP_ADDR_T, (void *)&br0_ip);	
	rtk_getInAddr("br0", NET_MASK_T, (void *)&br0_netmask);
	br0_netip.s_addr=br0_ip.s_addr & br0_netmask.s_addr;
	
		
	sprintf(line_buffer,"interface %s\n","br0");
	write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);
		
	apmib_get(MIB_DHCP_CLIENT_START,  (void *)&start_addr);		
	if((start_addr.s_addr & br0_netmask.s_addr) != br0_netip.s_addr)
		start_addr.s_addr=br0_netip.s_addr | (start_addr.s_addr & (~br0_netmask.s_addr));
	sprintf(line_buffer,"start %s\n",inet_ntoa(start_addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
	apmib_get(MIB_DHCP_CLIENT_END,  (void *)&end_addr);	
	if((end_addr.s_addr & br0_netmask.s_addr) != br0_netip.s_addr)
		end_addr.s_addr=br0_netip.s_addr | (end_addr.s_addr & (~br0_netmask.s_addr));
	sprintf(line_buffer,"end %s\n",inet_ntoa(end_addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);		
	
	sprintf(line_buffer,"opt subnet %s\n",inet_ntoa(br0_netmask));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&lease_time);
	if(lease_time<=0 || lease_time>10080)
	{
		lease_time = 480; //8 hours
		if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&lease_time))
				printf("set MIB_DHCP_LEASE_TIME error\n");
	}
	lease_time *= 60;
	sprintf(line_buffer,"opt lease %u\n",lease_time);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	sprintf(line_buffer,"opt router %s\n",inet_ntoa(br0_ip));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	sprintf(line_buffer,"opt dns %s\n",inet_ntoa(br0_ip)); 
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
	memset(domain_name, 0, sizeof(domain_name));
	apmib_get(MIB_DOMAIN_NAME, (void *)domain_name);
	if(domain_name[0])
	{
		sprintf(line_buffer,"opt domain %s\n",domain_name);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
	
	/*static dhcp entry static_lease*/
#ifdef SUPPORT_DHCP_PORT_IP_BIND
	apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
	if(entry_Num>0)
	{
		for (i=1; i<=entry_Num; i++) 
		{
			*((char *)&staticIPEntry) = (char)i;
			apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&staticIPEntry);
			if(((*(unsigned int*)(staticIPEntry.ipAddr)) & br0_netmask.s_addr)!=br0_netip.s_addr)
				(*(unsigned int*)(staticIPEntry.ipAddr))=br0_netip.s_addr | ((*(unsigned int*)(staticIPEntry.ipAddr)) & (~br0_netmask.s_addr));

			if(staticIPEntry.portId>0)
			{					
				sprintf(line_buffer, "static_lease %d %s\n", staticIPEntry.portId, inet_ntoa(*((struct in_addr*)staticIPEntry.ipAddr)));
				write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			}					
		}
	}
#endif
	apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intValue);
	if(intValue==1)
	{
		apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
		if(entry_Num>0)
		{
			for (i=1; i<=entry_Num; i++) 
			{				
				*((char *)&staticIPEntry) = (char)i;
				apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&staticIPEntry);
				if(((*(unsigned int*)(staticIPEntry.ipAddr)) & br0_netmask.s_addr)!=br0_netip.s_addr)
					(*(unsigned int*)(staticIPEntry.ipAddr))=br0_netip.s_addr | ((*(unsigned int*)(staticIPEntry.ipAddr)) & (~br0_netmask.s_addr)); 		
				if(memcmp(staticIPEntry.macAddr,"\x00\x00\x00\x00\x00\x00",6))
				{
					sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", staticIPEntry.macAddr[0], staticIPEntry.macAddr[1], staticIPEntry.macAddr[2],
					staticIPEntry.macAddr[3], staticIPEntry.macAddr[4], staticIPEntry.macAddr[5], inet_ntoa(*((struct in_addr*)staticIPEntry.ipAddr)));
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}		
			}
		}
	}	
	
	sprintf(line_buffer, "udhcpd %s", DHCPD_CONF_FILE);
	system(line_buffer);

	//system("ifconfig wlan0 down");
	//system("ifconfig wlan1 down");
	//system("ifconfig eth0 down");
	//system("ifconfig eth2 down");
	//system("ifconfig eth3 down");
	//system("ifconfig eth4 down");

	//sleep(5);
	
	//system("ifconfig wlan0 up");
	//system("ifconfig wlan1 up");
	//system("ifconfig eth0 up");
	//system("ifconfig eth2 up");
	//system("ifconfig eth3 up");
	//system("ifconfig eth4 up");	

	return RTK_SUCCESS; 
}

/*
  *@name rtk_get_lan_static_dhcp
  *@ input 
     rtk_static_lease *, the pointer of static_lease which specific the MAC and IP mapping
     empty_entry_num , unsigned int, the num of entries where pLease pointed
  *@output
     num , unsigned int *, which hold the num of the static lease entry gotten.
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */

int rtk_get_lan_static_dhcp(unsigned int *num, struct rtk_static_lease *pLease, unsigned int empty_entry_num)
{
	int	entryNum, i,j;
	DHCPRSVDIP_T entry;
	struct in_addr * addr;
	if(!apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entryNum))
	{
		printf("Failed to get table entry num!\n");
		return RTK_FAILED;
	}
	*num = (unsigned int)entryNum;
	if(entryNum > empty_entry_num)
	{
		entryNum = empty_entry_num;
	}
	for(i=1;i<=entryNum;i++)
	{
		*((char*)&entry) = (char)i;
		if(!apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry))
		{
			printf("Failed to get table entry!\n");
			return RTK_FAILED;
		}
		addr = (struct in_addr*)entry.ipAddr;
		pLease[i-1].ip = addr->s_addr;
		memset(pLease[i-1].mac,0,6);
		memcpy(pLease[i-1].mac,entry.macAddr,6);
	}
	return RTK_SUCCESS;
}


/*
  *@name rtk_set_lan_static_dhcp_enable
  *@ input 
	enable:unsigned int
	0:disable
	1:enable
  *@output
    	none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */

int rtk_set_lan_static_dhcp_enable(unsigned int enable)
{
	if(enable !=0 && enable != 1)
	{
		printf("Invalid Input!\n");
		return RTK_FAILED;
	}
	if (!apmib_set(MIB_DHCPRSVDIP_ENABLED,(void *)&enable))
	{
		printf("Enable static DHCP failed!\n");
		return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

/*
  *@name rtk_disable_lan_static_dhcp
  *@ input 
	enable:unsigned int*
  *@output
    	none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */

int rtk_get_lan_static_dhcp_enable(unsigned int* enable)
{
	int intVal;
	if(!apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intVal))
	{
		printf("Disable static DHCP failed!\n");
		return RTK_FAILED;
	}
	*enable = intVal;
	return RTK_SUCCESS;
}


/*
  *@ name: rtk_set_lan_clone_mac
  *@ input
	mac , unsigned char *, the mac in hex array. 112233445566h means 11:22:33:44:55:66
	
  *@ output
  	none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *@ function description
  	Set lan interface Clone MAC
  *
  */
int rtk_set_lan_clone_mac(unsigned char *mac)
{
	int i,j;
	if(mac == NULL)
	{
		printf("Invalid Input.\n");
		return RTK_FAILED;
	}
	if(!apmib_set(MIB_ELAN_MAC_ADDR, (void *)mac))
	{
		printf("Set MIB_ELAN_MAC_ADDR mib error!\n");
		return RTK_FAILED;
	}
	if(mac[0] == 0x00 && mac[1] == 0x00 && mac[2] == 0x00 && mac[3] == 0x00 && mac[4] == 0x00 && mac[5] == 0x00)
	{
		for(i=0;i<NUM_WLAN_INTERFACE;i++)
		{
			wlan_idx=i;
			for(j=0;j<NUM_VWLAN_INTERFACE;j++)
			{
				vwlan_idx=j;
				if ( !apmib_set(MIB_WLAN_WLAN_MAC_ADDR, (void *)mac))
				{
					printf("Set MIB_WLAN_WLAN_MAC_ADDR mib error!\n");
					return RTK_FAILED;
				}
			}
		}
	}
	else
	{
		for(i=0;i<NUM_WLAN_INTERFACE;i++)
		{
			wlan_idx=i;
			for(j=0;j<NUM_VWLAN_INTERFACE;j++)
			{
				vwlan_idx=j;
				if (!apmib_set(MIB_WLAN_WLAN_MAC_ADDR, (void *)mac))
				{
					printf("Set MIB_WLAN_WLAN_MAC_ADDR mib error!");
					return RTK_FAILED;
				}
				mac[5]++;
			}
			mac[5]-=NUM_VWLAN_INTERFACE;
			mac[5]+=0x10;
		}
	}
	if(!apmib_set(MIB_WLAN_WLAN_MAC_ADDR, (void *)mac))
	{
		printf("Set MIB_ELAN_MAC_ADDR mib error!\n");
		return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

/*
  *@ name: rtk_get_lan_clone_mac
  *@ input
	mac , unsigned char *, the mac in hex array. 112233445566h means 11:22:33:44:55:66
	
  *@ output
  	none
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *@ function description
  	Get lan interface Clone MAC
  *
  */
int rtk_get_lan_clone_mac(unsigned char *mac)
{
	if(mac == NULL)
	{
		printf("Invalid Input.\n");
		return RTK_FAILED;
	}
	memset(mac,0,6);
	if(!apmib_get(MIB_ELAN_MAC_ADDR, (void *)mac))
	{
		printf("Get MIB_ELAN_MAC_ADDR mib error!\n");
		return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

/*
  *@name rtk_get_device_brand
  *@ input 
     mac , the pointer of lan device mac address 
     mac_file , contains the prefix mac and brand list, such as "/etc/device_mac_brand.txt"
  *@output
     brand ,  hold the brand of device, such as Apple, Samsung, Xiaomi, Nokia, Huawei, etc.
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */
int rtk_get_device_brand(unsigned char *mac, char *mac_file, char *brand)
{	
	if(mac==NULL || mac_file==NULL || brand==NULL)
		return RTK_FAILED;
	
	FILE *fp;
	int index;
	unsigned char prefix_mac[16], mac_brand[64];
	char *pchar;
	int found=0;
	if((fp= fopen(mac_file, "r"))==NULL)
		return RTK_FAILED;

	sprintf(prefix_mac, "%02X-%02X-%02X", mac[0], mac[1], mac[2]);

	for(index = 0 ; index < 8; ++index)
	{
		if((prefix_mac[index]  >= 'a')  && (prefix_mac[index]<='f'))
			prefix_mac[index] -= 32;
	}

	//printf("%s.%d. str(%s)\n",__FUNCTION__,__LINE__,prefix_mac);

	while(fgets(mac_brand, sizeof(mac_brand), fp))
	{			
		mac_brand[strlen(mac_brand)-1]='\0';		
		if((pchar=strstr(mac_brand, prefix_mac))!=NULL)
		{
			pchar+=9;
			strcpy(brand, pchar);
			found=1;
			break;
		}
	}
	fclose(fp);
	
	if(found==1)
		return RTK_SUCCESS;
	
	return RTK_FAILED;
}


#define _PATH_DHCPS_LEASES	"/var/lib/misc/udhcpd.leases"
#define _PATH_DHCPS_PID	"/var/run/udhcpd.pid" 
#define _PATH_DEVICE_MAC_BRAND "/etc/device_mac_brand.txt"
#include <dirent.h>

static pid_t find_pid_by_name( char* pidName)
{
	DIR *dir;
	struct dirent *next;
	pid_t pid;
	
	if ( strcmp(pidName, "init")==0)
		return 1;
	
	dir = opendir("/proc");
	if (!dir) {
		printf("Cannot open /proc");
		return 0;
	}

	while ((next = readdir(dir)) != NULL) {
		FILE *status;
		char filename[64];
		char buffer[64];
		char name[64];

		/* Must skip ".." since that is outside /proc */
		if (strcmp(next->d_name, "..") == 0)
			continue;

		/* If it isn't a number, we don't want it */
		if (!isdigit(*next->d_name))
			continue;

		sprintf(filename, "/proc/%s/status", next->d_name);
		if (! (status = fopen(filename, "r")) ) {
			continue;
		}
		if (fgets(buffer, 63, status) == NULL) {
			fclose(status);
			continue;
		}
		fclose(status);

		/* Buffer should contain a string like "Name:   binary_name" */
		sscanf(buffer, "%*s %s", name);
		if (strcmp(name, pidName) == 0) {
		//	pidList=xrealloc( pidList, sizeof(pid_t) * (i+2));
			pid=(pid_t)strtol(next->d_name, NULL, 0);
			closedir(dir);
			return pid;
		}
	}	
	closedir(dir);
	return 0;
}

static int getOneDhcpClient(char **ppStart, unsigned long *size, unsigned char *hname, unsigned int *ip, unsigned char *mac, unsigned int *lease)
{
	struct dhcpOfferedAddr 
	{
		unsigned char chaddr[16];
		unsigned int yiaddr;       /* network order */
		unsigned int expires;      /* host order */
		char hostname[64]; 
		//unsigned int isUnAvailableCurr;	
	};

	struct dhcpOfferedAddr entry;
	unsigned char empty_haddr[16]; 

	memset(empty_haddr, 0, 16); 
	if ( *size < sizeof(entry) )
		return -1;

	entry = *((struct dhcpOfferedAddr *)*ppStart);
	*ppStart = *ppStart + sizeof(entry);
	*size = *size - sizeof(entry);

	if (entry.expires == 0)
		return 0;

	if(!memcmp(entry.chaddr, empty_haddr, 16))
		return 0;

	//strcpy(ip, inet_ntoa(*((struct in_addr *)&entry.yiaddr)) );
	*ip=entry.yiaddr;
	memcpy(mac, entry.chaddr, 6);
	
	//snprintf(mac, 20, "%02x:%02x:%02x:%02x:%02x:%02x",entry.chaddr[0],
	//	entry.chaddr[1],entry.chaddr[2],entry.chaddr[3],entry.chaddr[4], entry.chaddr[5]);
	//if(entry.expires == 0xffffffff)
	//	sprintf(liveTime,"%s", "Always");
	//else
	//	snprintf(liveTime, 10, "%lu", (unsigned long)ntohl(entry.expires));
	*lease=entry.expires;
	
	if(entry.hostname[0])
		strcpy(hname, entry.hostname);
	else
		strcpy(hname, "null");

	return 1;
}

/*
  *@name rtk_get_dhcp_client_list
  *@ input 
     rtk_dhcp_client_info *, the pointer of lan dhcp client list which specific every client info, such as host name, ip, mac, lease time 
  *@output
     num , unsigned int *, which hold the num of dhcp client.
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */
int rtk_get_dhcp_client_list(unsigned int *num, struct rtk_dhcp_client_info *pclient)
{	
	FILE *fp;
	int idx=0, ret;
	char *buf=NULL, *ptr, tmpBuf[100];
	unsigned int ip, lease;
	unsigned char mac[6], hostname[64];

	struct stat status;
	int pid;
	unsigned long fileSize=0;
	// siganl DHCP server to update lease file
	pid = find_pid_by_name("udhcpd");

	if(pid > 0)
	{
		snprintf(tmpBuf, 100, "kill -SIGUSR1 %d\n", pid);
		system(tmpBuf);
	}

	usleep(1000);

	if ( stat(_PATH_DHCPS_LEASES, &status) < 0 )
		goto err;

	fileSize=status.st_size;
	buf = malloc(fileSize);
	if ( buf == NULL )
		goto err;
	fp = fopen(_PATH_DHCPS_LEASES, "r");
	if ( fp == NULL )
		goto err;

	fread(buf, 1, fileSize, fp);
	fclose(fp);

	ptr = buf;
	while (1) 
	{
		ret = getOneDhcpClient(&ptr, &fileSize, hostname, &ip, mac, &lease);

		if (ret < 0)
			break;
		if (ret == 0)
			continue;

		strcpy(pclient[idx].hostname, hostname);
		pclient[idx].ip=ip;
		memcpy(pclient[idx].mac, mac, 6);
		pclient[idx].expires=lease;
#ifdef GET_DEVICE_BRAND_FROM_MAC
		ret=rtk_get_device_brand(pclient[idx].mac, _PATH_DEVICE_MAC_BRAND, pclient[idx].brand);
		if(ret==RTK_FAILED)
			strcpy(pclient[idx].brand, "Computer"); //default
		
		if(strcmp(pclient[idx].hostname, "null")==0)
			strcpy(pclient[idx].hostname, pclient[idx].brand);
#endif		
		idx++;
	}
	
err:
	*num=idx;
	if (buf)
		free(buf);
	
	return RTK_SUCCESS;
}
int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

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

int get_wifi_device_mac_list(char *filename, unsigned char maclist[][6])
{
	if(filename==NULL)
		return RTK_FAILED; 

	FILE *fp;
	char line_buffer[256];	
	char mac_str[13];
	unsigned char mac_addr[6];
	int idx=0;	
	char *pchar;
	
	if((fp= fopen(filename, "r"))==NULL)
		return RTK_FAILED;
	
	while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';		
		if((pchar=strstr(line_buffer, "hwaddr: "))!=NULL)
		{
			pchar+=strlen("hwaddr: ");
			strcpy(mac_str, pchar);
			if (strlen(mac_str)==12 && string_to_hex(mac_str, mac_addr, 12)) 
			{
				memcpy(maclist[idx], mac_addr, 6);
				idx++;
			}			
		}
	}
	fclose(fp);
	return idx;		
}

int get_arp_table_list(char *filename, RTK_ARP_ENTRY_Tp parplist)
{
	if(filename==NULL || parplist==NULL)
		return RTK_FAILED; 

	FILE *fp;
	char line_buffer[512];	
	char mac_str[13], tmp_mac_str[18];
	char ip_str[16], if_name[16];
	unsigned char mac_addr[6];
	int idx=0, i, j;	
	char *pchar, *pstart, *pend;
	struct in_addr ip_addr;
	
	if((fp= fopen(filename, "r"))==NULL)
		return RTK_FAILED;
	
	while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';		

		sscanf(line_buffer,"%s %*s %*s %s %*s %s",ip_str,tmp_mac_str,if_name);
		if(strcmp(if_name, "br0")!=0)
			continue;

		inet_aton(ip_str, &ip_addr);
		parplist[idx].ip=ip_addr.s_addr;
		
		for(i=0, j=0; i<17 && j<12; i++)
		{
			if(tmp_mac_str[i]!=':')
			{
				mac_str[j++]=tmp_mac_str[i];
			}
		}
		mac_str[12]=0;			
			
		if (strlen(mac_str)==12 && string_to_hex(mac_str, mac_addr, 12)) 
		{
			memcpy(parplist[idx].mac, mac_addr, 6);
			idx++;
		}		
	}
	fclose(fp);
	return idx;		
}

int get_mac_list_from_l2_tab(char *filename, unsigned char maclist[][6])
{
	if(filename==NULL)
		return RTK_FAILED; 

	FILE *fp;
	char line_buffer[512];	
	char mac_str[13];
	unsigned char mac_addr[6];
	int idx=0, i, j;	
	char *pchar, *pstart;
	
	unsigned char br0_mac[6];
	unsigned char br0_mac_str[32];
	
	if((fp= fopen(filename, "r"))==NULL)
		return RTK_FAILED;
	
	memset(br0_mac,0,6);
	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)br0_mac);
	if(!memcmp(br0_mac, "\x00\x00\x00\x00\x00\x00", 6))
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)br0_mac);
	
	sprintf(br0_mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", br0_mac[0], br0_mac[1], br0_mac[2], br0_mac[3], br0_mac[4], br0_mac[5]);
	br0_mac_str[strlen("ff:ff:ff:ff:ff:ff")]='\0';
	
	while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';

		if(strstr(line_buffer, "ff:ff:ff:ff:ff:ff") || strstr(line_buffer, "CPU") || strstr(line_buffer, "FID:1") || strstr(line_buffer, br0_mac_str))
			continue;	
		
		pchar=strchr(line_buffer, ':');
		pstart=pchar-2;
		for(i=0, j=0; i<17 && j<12; i++)
		{
			if(pstart[i]!=':')
			{
				mac_str[j++]=pstart[i];
			}
		}
		mac_str[j]=0;
		if (strlen(mac_str)==12 && string_to_hex(mac_str, mac_addr, 12)) 
		{
			memcpy(maclist[idx], mac_addr, 6);
			idx++;
		}		
	}
	fclose(fp);
	return idx;		
}


/*
  *@name rtk_get_lan_device_info
  *@ input 
     pdevinfo , the pointer of lan device info
  *@output
     num ,  hold the lan device number
  *@ return value
  	RTK_SUCCESS
  	RTK_FAILED
  *
  */
int rtk_get_lan_device_info(unsigned int *num, RTK_LAN_DEVICE_INFO_Tp pdevinfo)
{	
	if(num==NULL || pdevinfo==NULL)
		return RTK_FAILED;

	unsigned int dhcp_device_num=0, arp_entry_num=0, wifi_sta_num=0, l2_tab_num=0;
	int i, j,k,n, idx, ret;
	struct rtk_dhcp_client_info dhcp_client_info[254];
	RTK_ARP_ENTRY_T arp_tab[254];
	unsigned char maclist[MAX_STA_NUM+1][6];
	
	rtk_get_dhcp_client_list(&dhcp_device_num, &dhcp_client_info);
	for(idx=0; idx<dhcp_device_num; idx++)
	{
		strcpy(pdevinfo[idx].hostname, dhcp_client_info[idx].hostname);
		pdevinfo[idx].ip=dhcp_client_info[idx].ip;
		memcpy(pdevinfo[idx].mac, dhcp_client_info[idx].mac, 6);
		pdevinfo[idx].expires=dhcp_client_info[idx].expires;
		pdevinfo[idx].conType=WIRE_CONNECT;
		strcpy(pdevinfo[idx].brand, dhcp_client_info[idx].brand);		
	}

	arp_entry_num=get_arp_table_list("/proc/net/arp", arp_tab);
	
	wifi_sta_num=get_wifi_device_mac_list("/proc/wlan0/sta_info", maclist);
	
	for(i=0; i<wifi_sta_num; i++)
	{
		for(j=0;j<dhcp_device_num;j++)
		{
			if(memcmp(maclist[i], pdevinfo[j].mac, 6)==0)
			{
				pdevinfo[j].conType=WIRELESS_CONNECT;
				break;
			}
		}
		if(j==dhcp_device_num)
		{
			strcpy(pdevinfo[idx].hostname, "null");
			pdevinfo[idx].ip=0;
			memcpy(pdevinfo[idx].mac, maclist[i], 6);
			pdevinfo[idx].expires=0;
			pdevinfo[idx].conType=WIRELESS_CONNECT;			
			
			for(k=0;k<arp_entry_num;k++)
			{
				if(memcmp(maclist[i], arp_tab[k].mac, 6)==0)
				{					
					pdevinfo[idx].ip=arp_tab[k].ip;
					break;
				}
			}
			idx++;
		}
	}

	wifi_sta_num=get_wifi_device_mac_list("/proc/wlan1/sta_info", maclist);
		
	for(i=0; i<wifi_sta_num; i++)
	{
		for(j=0;j<dhcp_device_num;j++)
		{
			if(memcmp(maclist[i], pdevinfo[j].mac, 6)==0)
			{
				pdevinfo[j].conType=WIRELESS_CONNECT;
				break;
			}
		}
		if(j==dhcp_device_num)
		{
			strcpy(pdevinfo[idx].hostname, "null");
			pdevinfo[idx].ip=0;
			memcpy(pdevinfo[idx].mac, maclist[i], 6);
			pdevinfo[idx].expires=0;
			pdevinfo[idx].conType=WIRELESS_CONNECT;				
			
			for(k=0;k<arp_entry_num;k++)
			{
				if(memcmp(maclist[i], arp_tab[k].mac, 6)==0)
				{					
					pdevinfo[idx].ip=arp_tab[k].ip;
					break;
				}
			}
			idx++;
		}
	}

	
	l2_tab_num=get_mac_list_from_l2_tab("/proc/rtl865x/l2", maclist);
	for(i=0; i<l2_tab_num; i++)
	{
		for(j=0;j<dhcp_device_num;j++)
		{
			if(memcmp(maclist[i], pdevinfo[j].mac, 6)==0)
				break;
		}
		if(j==dhcp_device_num)
		{
			for(k=dhcp_device_num;k<idx;k++)
			{
				if(memcmp(maclist[i], pdevinfo[k].mac, 6)==0)
					break;
			}			
			if(k==idx)
			{
				strcpy(pdevinfo[idx].hostname, "null");
				pdevinfo[idx].ip=0;
				memcpy(pdevinfo[idx].mac, maclist[i], 6);
				pdevinfo[idx].expires=0;
				pdevinfo[idx].conType=WIRE_CONNECT;
			
				for(n=0;n<arp_entry_num;n++)
				{
					if(memcmp(maclist[i], arp_tab[n].mac, 6)==0)
					{					
						pdevinfo[idx].ip=arp_tab[n].ip;
						break;
					}
				}
				idx++;
			}
		}
	}

	for(i=0;i<idx;i++)
	{
		ret=rtk_get_device_brand(pdevinfo[i].mac, _PATH_DEVICE_MAC_BRAND, pdevinfo[i].brand);
		if(ret==RTK_FAILED)
			strcpy(pdevinfo[i].brand, "Computer"); //default
		
		if(strcmp(pdevinfo[i].hostname, "null")==0)
			strcpy(pdevinfo[i].hostname, pdevinfo[i].brand);	
	}	

	*num=idx;
	
	return RTK_SUCCESS;	
}
/**************************************************
* @NAME:
* 	rtk_get_lan_info
* 
* @PARAMETERS:
* 	@Input
* 		info: point of lan_info
* 	@Output
*	 	none
*
* @RETRUN:
* 	RTK_SUCCESS
*	RTK_FAILED
* 
* @FUNCTION :
* 	get lan information
*
***************************************************/
int rtk_get_lan_info(RTK_LAN_INFOp info)
{
	if(info == NULL)
		return RTK_FAILED;
	
	char *BRIDGE_IF;
	DHCP_T dhcp;
	struct in_addr	intaddr;
	struct sockaddr hwaddr;
	unsigned char *pMacAddr;
	struct rtk_static_config p_static_config;

	memset(info, 0, sizeof(RTK_LAN_INFO));

#ifdef HOME_GATEWAY
	BRIDGE_IF = "br0";
#elif defined(VOIP_SUPPORT) && defined(ATA867x)
	BRIDGE_IF = "eth0";
#else
	BRIDGE_IF = "br0";
#endif

	if ( !apmib_get( MIB_DHCP, (void *)&dhcp) )
		return RTK_FAILED;

	// get status
	if (dhcp==DHCP_CLIENT) {
		if (!isDhcpClientExist(BRIDGE_IF) &&
				!rtk_getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr))
			info->status = 0;
		if (isDhcpClientExist(BRIDGE_IF))
			info->status = 1;
	}
	info->status = 2;

	// get ip, mask, def_gateway
	if( rtk_get_lan_ip(&p_static_config)==RTK_FAILED )
		return RTK_FAILED;
	info->ip.s_addr = p_static_config.ip;
	info->mask.s_addr = p_static_config.mask;
	info->def_gateway.s_addr = p_static_config.gw;

	// get dhcp_server
	info->dhcp_server = dhcp;
	if ( dhcp == DHCP_CLIENT && !isDhcpClientExist(BRIDGE_IF))
		info->dhcp_server = DHCP_DISABLED;

	// get mac
	if ( rtk_getInAddr(BRIDGE_IF, HW_ADDR_T, (void *)&hwaddr ) )
	{
		pMacAddr = (unsigned char *)hwaddr.sa_data;
		memcpy(info->mac, pMacAddr, 6);
	}
	else
		memset(info->mac, 0, 6);

	return RTK_SUCCESS;
}
int rtk_get_lan_dev_link_time(int *entry_num, struct rtk_link_type_info *info)
{
	if(*entry_num==0 || info==NULL)
		return RTK_FAILED;
	
	int idx=0, i, j, find_mac=0;
	int tmp_idx=0;
	char line_buffer[128], tmp_mac_str[18], tmp_ip_str[16];	
	FILE *fp;
	int fh=0; 
	struct rtk_dev_link_time dev_entry[MAX_TERM_NUMBER];	
	struct rtk_link_type_info tmp_info[MAX_TERM_NUMBER];

	unsigned char *pMacAddr;
	unsigned char br0_mac[6];
	struct in_addr br0_ip, br0_netmask, br0_netip;
	struct sockaddr hwaddr;
	
	memset(tmp_info, 0, sizeof(tmp_info));	
	rtk_getInAddr("br0", IP_ADDR_T, (void *)&br0_ip);	
	rtk_getInAddr("br0", NET_MASK_T, (void *)&br0_netmask);
	br0_netip.s_addr=br0_ip.s_addr & br0_netmask.s_addr;	
	rtk_getInAddr("br0", HW_ADDR_T, (void *)&hwaddr);
	pMacAddr = (unsigned char *)hwaddr.sa_data;
	memcpy(br0_mac, pMacAddr, 6);
#ifndef CONFIG_APP_CMCC_ADAPTER
	for(i=0; i<*entry_num; i++)
		info[i].link_time=0;
#endif
	if((fp= fopen("/var/lan_dev_link_time", "r"))==NULL)
		return RTK_FAILED;
	if((fh=fileno(fp))<0)
		return RTK_FAILED;
	
	flock(fh, LOCK_EX);
	while(fgets(line_buffer, sizeof(line_buffer), fp) && idx<MAX_TERM_NUMBER)
	{			
		line_buffer[strlen(line_buffer)-1]='\0';
		sscanf(line_buffer,"%s %s %u %d",dev_entry[idx].ip, dev_entry[idx].mac, &dev_entry[idx].link_time, &dev_entry[idx].is_alive);
		dev_entry[idx].ip[15]='\0';
		dev_entry[idx].mac[17]='\0';
		idx++;
	}
	flock(fh, LOCK_UN);
	fclose(fp);
	
	for(i=0;i<*entry_num;i++)
	{		
		find_mac=0;
		for(j=0;j<idx;j++)
		{
			sprintf(tmp_mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", info[i].mac[0], info[i].mac[1], info[i].mac[2],
			info[i].mac[3],info[i].mac[4],info[i].mac[5]);
			tmp_mac_str[17]='\0';
			strcpy(tmp_ip_str, inet_ntoa(info[i].ip));			
			if(strcmp(tmp_mac_str, dev_entry[j].mac)==0)
			{
				find_mac=1;
				if(dev_entry[j].is_alive && strcmp(tmp_ip_str, dev_entry[j].ip)==0)
				{
					info[i].link_time=dev_entry[j].link_time;
					memcpy(&tmp_info[tmp_idx++], &info[i], sizeof(struct rtk_link_type_info));
					break;
				}				
			}	
		}
		if(j>=idx && find_mac==0)
			memcpy(&tmp_info[tmp_idx++], &info[i], sizeof(struct rtk_link_type_info));
	}
	
	memcpy(info, tmp_info, sizeof(struct rtk_link_type_info)*tmp_idx);	
	*entry_num=tmp_idx;
	
	return RTK_SUCCESS;
}
int rtk_get_offline_dev_list(int *entry_num, struct rtk_offline_dev_info *info)
{
	if(entry_num==NULL || info==NULL)
		return RTK_FAILED;
	
	int idx=0, i, j,k, count=0, ret;	
	char line_buffer[128], mac_str[13];
	unsigned char mac_addr[6];
	
	FILE *fp;
	int fh=0;
	struct rtk_dev_link_time dev_entry[MAX_TERM_NUMBER];

	DHCPRSVDIP_T staticIPEntry;
	RTK_MACFILTER_T netLimitEntry;
	RTK_IPQOS_T speedLimitEntry;
	RTK_NASFILTER_T nasLimitEntry;
	
	int staticIpEntryNum=0, netLimitNum=0, speedLimitNum=0, nasLimitNum=0;
	int is_static_ip, is_net_limit, is_speed_limit, is_nas_limit;	

	memset(dev_entry, 0, sizeof(dev_entry));

	if((fp= fopen("/var/lan_dev_link_time", "r"))==NULL)
		return RTK_FAILED;
	if((fh=fileno(fp))<0)
		return RTK_FAILED;
	
	flock(fh, LOCK_EX);	
	while(fgets(line_buffer, sizeof(line_buffer), fp) && idx<MAX_TERM_NUMBER)
	{			
		line_buffer[strlen(line_buffer)-1]='\0';
		sscanf(line_buffer,"%s %s %u %d",dev_entry[idx].ip, dev_entry[idx].mac, &dev_entry[idx].link_time, &dev_entry[idx].is_alive);

		if(dev_entry[idx].is_alive==0)
			continue;
		
		dev_entry[idx].ip[15]='\0';
		dev_entry[idx].mac[17]='\0';
		for(j=0, k=0; j<17 && k<12; j++)
		{
			if(dev_entry[idx].mac[j]!=':')
				mac_str[k++]=dev_entry[idx].mac[j];
		}		
		mac_str[k]=0;
		if (strlen(mac_str)==12 && string_to_hex(mac_str, mac_addr, 12)) 
			memcpy(dev_entry[idx].mac, mac_addr, 6);
		idx++;
	}
	flock(fh, LOCK_UN);
	fclose(fp);
	
	apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&staticIpEntryNum);
	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&netLimitNum);
	apmib_get(MIB_QOS_RULE_TBL_NUM,(void*)&speedLimitNum);
	apmib_get(MIB_NASFILTER_TBL_NUM,(void*)&nasLimitNum);

	for (j=1; j<=staticIpEntryNum; j++) 
	{
		*((char *)&staticIPEntry) = (char)j;
		apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&staticIPEntry);

		if(memcmp(staticIPEntry.macAddr,"\x00\x00\x00\x00\x00\x00",6)==0)
			continue;

		for(i=0;i<idx;i++)
		{
			if(memcmp(staticIPEntry.macAddr,dev_entry[i].mac,6)==0)
				break;
		}		
		if(i==idx)
		{
			for(k=0;k<count;k++)
			{
				if(memcmp(staticIPEntry.macAddr,info[k].mac,6)==0)
				{
					info[k].is_static_ip=1;
					break;
				}
			}
			if(k==count)
			{
				//inet_aton(staticIPEntry.macAddr, &info[count].ip);
				memcpy(info[count].mac, staticIPEntry.macAddr, 6);
				info[count].is_static_ip=1;
				//info[count].link_time=dev_entry[i].link_time;
				ret=rtk_get_device_brand(info[count].mac, _PATH_DEVICE_MAC_BRAND, info[count].brand);
				if(ret==RTK_FAILED)
					strcpy(info[count].brand, "Computer"); //default
					
				count++;
			}
		}				
	}

	for (j=1; j<=netLimitNum; j++) 
	{
		*((char *)&netLimitEntry) = (char)j;
		apmib_get(MIB_MACFILTER_TBL, (void *)&netLimitEntry);

		if(memcmp(netLimitEntry.macAddr,"\x00\x00\x00\x00\x00\x00",6)==0)
			continue;
				
		for(i=0;i<idx;i++)
		{
			if(memcmp(netLimitEntry.macAddr,dev_entry[i].mac,6)==0)
				break;
		}		
		if(i==idx)
		{
			for(k=0;k<count;k++)
			{
				if(memcmp(netLimitEntry.macAddr,info[k].mac,6)==0)
				{
					info[k].is_net_limit=1;
					break;
				}
			}
			if(k==count)
			{
				memcpy(info[count].mac, netLimitEntry.macAddr, 6);
				info[count].is_net_limit=1;
				ret=rtk_get_device_brand(info[count].mac, _PATH_DEVICE_MAC_BRAND, info[count].brand);
				if(ret==RTK_FAILED)
					strcpy(info[count].brand, "Computer"); //default
					
				count++;
			}
		}				
	}

	for (j=1; j<=speedLimitNum; j++) 
	{
		*((char *)&speedLimitEntry) = (char)j;
		apmib_get(MIB_QOS_RULE_TBL, (void *)&speedLimitEntry);

		if(memcmp(speedLimitEntry.mac,"\x00\x00\x00\x00\x00\x00",6)==0)
			continue;
				
		for(i=0;i<idx;i++)
		{
			if(memcmp(speedLimitEntry.mac,dev_entry[i].mac,6)==0)
				break;
		}		
		if(i==idx)
		{
			for(k=0;k<count;k++)
			{
				if(memcmp(speedLimitEntry.mac,info[k].mac,6)==0)
				{
					info[k].is_speed_limit=1;
					break;
				}
			}
			if(k==count)
			{
				memcpy(info[count].mac, speedLimitEntry.mac, 6);
				info[count].is_speed_limit=1;
				ret=rtk_get_device_brand(info[count].mac, _PATH_DEVICE_MAC_BRAND, info[count].brand);
				if(ret==RTK_FAILED)
					strcpy(info[count].brand, "Computer"); //default
					
				count++;
			}
		}				
	}
	

	for (j=1; j<=nasLimitNum; j++) 
	{
		*((char *)&nasLimitEntry) = (char)j;
		apmib_get(MIB_NASFILTER_TBL, (void *)&nasLimitEntry);

		if(memcmp(nasLimitEntry.macAddr,"\x00\x00\x00\x00\x00\x00",6)==0)
			continue;
				
		for(i=0;i<idx;i++)
		{
			if(memcmp(nasLimitEntry.macAddr,dev_entry[i].mac,6)==0)
				break;
		}		
		if(i==idx)
		{
			for(k=0;k<count;k++)
			{
				if(memcmp(nasLimitEntry.macAddr,info[k].mac,6)==0)
				{
					info[k].is_nas_limit=1;
					break;
				}
			}
			if(k==count)
			{
				memcpy(info[count].mac, nasLimitEntry.macAddr, 6);
				info[count].is_nas_limit=1;
				ret=rtk_get_device_brand(info[count].mac, _PATH_DEVICE_MAC_BRAND, info[count].brand);
				if(ret==RTK_FAILED)
					strcpy(info[count].brand, "Computer"); //default
					
				count++;
			}
		}				
	}	
			
	*entry_num=count;	
	
	/*
	for(i=0; i<count; i++)
	{
		printf("\n###########################\n");
		//printf("ip=%s\n",inet_ntoa(*((struct in_addr *)&info[i].ip)));
		printf("mac=%02x:%02x:%02x:%02x:%02x:%02x\n",info[i].mac[0], info[i].mac[1], info[i].mac[2],
			info[i].mac[3],info[i].mac[4],info[i].mac[5]);

		//printf("link_time=%u\n",info[i].link_time);
		printf("brand=%s\n",info[i].brand);
		printf("is_static_ip=%d is_net_limit=%d is_speed_limit=%d is_nas_limit=%d\n",info[i].is_static_ip, info[i].is_net_limit, info[i].is_speed_limit, info[i].is_nas_limit);
		printf("\n###########################\n");
	}
	*/
	
	return RTK_SUCCESS;
}

int rtk_del_offline_dev(unsigned char *macAddr)
{
	if(macAddr==NULL)
		return RTK_FAILED;
	
	int j;	
	struct rtk_static_lease s_lease;

	DHCPRSVDIP_T staticIPEntry;
	RTK_MACFILTER_T netLimitEntry;
	RTK_IPQOS_T speedLimitEntry;
	RTK_NASFILTER_T nasLimitEntry;
	
	int staticIpEntryNum=0, netLimitNum=0, speedLimitNum=0, nasLimitNum=0;

	apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&staticIpEntryNum);
	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&netLimitNum);
	apmib_get(MIB_QOS_RULE_TBL_NUM,(void*)&speedLimitNum);
	apmib_get(MIB_NASFILTER_TBL_NUM,(void*)&nasLimitNum);

	for (j=1; j<=staticIpEntryNum; j++) 
	{
		*((char *)&staticIPEntry) = (char)j;
		apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&staticIPEntry);

		if(memcmp(staticIPEntry.macAddr,macAddr,6)==0)
		{
			memcpy(&s_lease.ip, staticIPEntry.ipAddr, 4);
			memcpy(s_lease.mac, staticIPEntry.macAddr, 6);
			rtk_del_lan_static_dhcp_immediately(&s_lease, 0);
			break;			
		}							
	}

	for (j=1; j<=netLimitNum; j++) 
	{
		*((char *)&netLimitEntry) = (char)j;
		apmib_get(MIB_MACFILTER_TBL, (void *)&netLimitEntry);

		if(memcmp(netLimitEntry.macAddr,macAddr,6)==0)
		{
			rtk_del_mac_filter_entry(&netLimitEntry, 0);
			break;			
		}							
	}

	for (j=1; j<=speedLimitNum; j++) 
	{
		*((char *)&speedLimitEntry) = (char)j;
		apmib_get(MIB_QOS_RULE_TBL, (void *)&speedLimitEntry);

		if(memcmp(speedLimitEntry.mac,macAddr,6)==0)
		{
			rtk_del_qos_rule_entry_immediately(&speedLimitEntry, 0);
			break;			
		}							
	}

	for (j=1; j<=nasLimitNum; j++) 
	{
		*((char *)&nasLimitEntry) = (char)j;
		apmib_get(MIB_NASFILTER_TBL, (void *)&nasLimitEntry);

		if(memcmp(nasLimitEntry.macAddr,macAddr,6)==0)
		{
			rtk_del_nas_filter_entry(&nasLimitEntry, 0);
			break;
		}							
	}	
	return RTK_SUCCESS;
}
int rtk_lan_get_wifi_client_info(LAN_DEV_BASE_INFO *pWifiClient, int array_len, int *real_num)
{
	int dhcp_client_num=0;
	struct rtk_dhcp_client_info	dhcp_client[128];

	int w_idx,vw_idx;
	char wlan_ifname[16]={0};
	PHYBAND_TYPE_T band;
	unsigned char ssid[MAX_SSID_LEN];
	int sta_num;
	int i,j,idx=0;

	RTK_WLAN_STA_EXTRA_INFO_T wlanStas[MAX_STA_NUM+1];
	memset(pWifiClient, 0, sizeof(LAN_DEV_BASE_INFO)*array_len);
	
	rtk_get_dhcp_client_list(&dhcp_client_num,&dhcp_client);
	for(w_idx=0; w_idx<NUM_WLAN_INTERFACE; w_idx++)
	{
        for(vw_idx=0; vw_idx<NUM_VWLAN_INTERFACE; vw_idx++)
        {
            memset(wlan_ifname, 0, sizeof(wlan_ifname));
            if(vw_idx == 0)
                sprintf(wlan_ifname, "wlan%d", w_idx);
            else
                sprintf(wlan_ifname, "wlan%d-va%d", w_idx, vw_idx-1);   

            if(rtk_wlan_get_rf_band(wlan_ifname, &band)==RTK_FAILED)
                continue;

            if(rtk_wlan_get_ssid(wlan_ifname, ssid)==RTK_FAILED)
                continue;
			
            if(rtk_wlan_get_wlan_sta_num(wlan_ifname,&sta_num)==RTK_FAILED)
                continue;
			
            memset(wlanStas, 0, sizeof(wlanStas)); 			
			if(rtk_wlan_get_wlan_sta_extra_info(wlan_ifname, wlanStas)==RTK_FAILED)
				continue;

			for (i=1; i<=sta_num && idx<array_len; i++)
			{
				for(j=0;j<dhcp_client_num; j++)
				{
					//printf("%s:::%d, mac=%02x:%02x:%02x:%02x:%02x:%02x\n",__FUNCTION__,__LINE__,wlanStas[i].addr[0],wlanStas[i].addr[1],wlanStas[i].addr[2],wlanStas[i].addr[3],wlanStas[i].addr[4],wlanStas[i].addr[5]);
					//printf("%s:::%d, mac=%02x:%02x:%02x:%02x:%02x:%02x\n",__FUNCTION__,__LINE__,dhcp_client[j].mac[0],dhcp_client[j].mac[1],dhcp_client[j].mac[2],dhcp_client[j].mac[3],dhcp_client[j].mac[4],dhcp_client[j].mac[5]);
					if(memcmp(wlanStas[i].addr, dhcp_client[j].mac, MAC_ADDR_LEN)==0)
					{
						pWifiClient[idx].ip=dhcp_client[j].ip;
						strncpy(pWifiClient[idx].dev_name, dhcp_client[j].hostname, sizeof(pWifiClient[idx].dev_name)-1);
						pWifiClient[idx].expires=dhcp_client[j].expires;
						pWifiClient[idx].tx_bytes = wlanStas[i].tx_bytes;
						pWifiClient[idx].rx_bytes = wlanStas[i].rx_bytes;
						break;
					}
				}
				if(j==dhcp_client_num)
				{					
#ifdef CONFIG_RECORD_CLIENT_HOST
					memcpy(&pWifiClient[idx].ip, wlanStas[i].client_host_ip, 4);
					strncpy(pWifiClient[idx].dev_name, wlanStas[i].client_host_name, sizeof(pWifiClient[idx].dev_name)-1);
#endif
				}				
				memcpy(pWifiClient[idx].mac, wlanStas[i].addr, MAC_ADDR_LEN);

				//if(pWifiClient[idx].ip==0 || memcmp(pWifiClient[idx].mac, invalid_mac, MAC_ADDR_LEN)==0)
					//continue;

				//if can't get hostname, use mac address as hostname
				if(pWifiClient[idx].dev_name[0]==0 || strcmp(pWifiClient[idx].dev_name, "unknown")==0)
					sprintf(pWifiClient[idx].dev_name, "%02X-%02X-%02X-%02X-%02X-%02X",wlanStas[i].addr[0],wlanStas[i].addr[1],wlanStas[i].addr[2],wlanStas[i].addr[3],wlanStas[i].addr[4],wlanStas[i].addr[5]);
			
				pWifiClient[idx].rssi=wlanStas[i].rssi;
				strcpy(pWifiClient[idx].conn_ifname, wlan_ifname);
				pWifiClient[idx].link_time=wlanStas[i].link_time;
				pWifiClient[idx].up_speed = wlanStas[i].rx_bytes_1s;
				pWifiClient[idx].down_speed = wlanStas[i].tx_bytes_1s;
				snprintf(pWifiClient[idx].txrate,sizeof(pWifiClient[idx].txrate),wlanStas[i].txrate);
				snprintf(pWifiClient[idx].rxrate,sizeof(pWifiClient[idx].rxrate),wlanStas[i].rxrate);
				pWifiClient[idx].is_alive=1;
				if(band == PHYBAND_2G)
				    pWifiClient[idx].conn_type = LAN_DEV_CONN_WLAN_2G;
				else
				    pWifiClient[idx].conn_type = LAN_DEV_CONN_WLAN_5G;
				snprintf(pWifiClient[idx].ssid, sizeof(pWifiClient[idx].ssid), "%s", ssid);
				idx++;		
			}			   
        }
    }
	*real_num=idx;
	//rtk_dump_lan_client_info(pWifiClient, idx);
	return RTK_SUCCESS;
}

int rtk_lan_get_wired_client_info(LAN_DEV_BASE_INFO *pWiredClient, int array_len, int *real_num)
{
	if(pWiredClient==NULL || array_len<1 || real_num==NULL)
		return RTK_FAILED;

	int i, idx=0;
	char buffer[512]={0};
	struct in_addr ip_addr;
	char str_ip[16]={0}, str_mac[32]={0};
	char *token=NULL;
	char *savestr=NULL;
	
	FILE *fp=NULL;

	memset(pWiredClient, 0, sizeof(LAN_DEV_BASE_INFO)*array_len);
	
	if((fp = fopen(LAN_DEV_LINK_INFO,"r")) == NULL)
		return RTK_FAILED;

	while(fgets(buffer, sizeof(buffer), fp) && idx<array_len)
	{
		for(i=0;buffer[i]!='\0';i++)
		if(buffer[i]=='\n')
			buffer[i]='\0';
		
		token=strtok_r(buffer,",",&savestr);
		while(token != NULL)
		{
 			if(strncmp("ip=",token,strlen("ip="))==0)
			{
				memset(str_ip, 0, sizeof(str_ip));
				strcpy(str_ip,(token+strlen("ip=")));
				inet_aton(str_ip, &ip_addr);
				pWiredClient[idx].ip=ip_addr.s_addr;
			}

			if(strncmp("mac=",token,strlen("mac="))==0)
			{
				memset(str_mac, 0, sizeof(str_mac));
				strcpy(str_mac,(token+strlen("mac=")));	
				rtk_change_mac_to_octe(str_mac, pWiredClient[idx].mac);
			}

			if(strncmp("dev_name=",token,strlen("dev_name="))==0)
			{				
				strcpy(pWiredClient[idx].dev_name,(token+strlen("dev_name=")));				
			}

			if(strncmp("expires=",token,strlen("expires="))==0)
			{				
				pWiredClient[idx].expires=atol(token+strlen("expires="));				
			}

			if(strncmp("conn_type=",token,strlen("conn_type="))==0)
			{				
				pWiredClient[idx].conn_type=atoi(token+strlen("conn_type="));				
			}

			if(strncmp("conn_port=",token,strlen("conn_port="))==0)
			{				
				pWiredClient[idx].conn_port=atoi(token+strlen("conn_port="));				
			}

			if(strncmp("link_time=",token,strlen("link_time="))==0)
			{				
				pWiredClient[idx].link_time=atol(token+strlen("link_time="));				
			}

			if(strncmp("is_alive=",token,strlen("is_alive="))==0)
			{				
				pWiredClient[idx].is_alive=atoi(token+strlen("is_alive="));				
			}

			if(strncmp("up_speed=",token,strlen("up_speed="))==0)
			{				
				pWiredClient[idx].up_speed=atol(token+strlen("up_speed="));				
			}

			if(strncmp("down_speed=",token,strlen("down_speed="))==0)
			{				
				pWiredClient[idx].down_speed=atol(token+strlen("down_speed="));				
			}		

			token=strtok_r(NULL,",",&savestr);
		}
		idx++;	
	}
	fclose(fp);
	
	*real_num=idx;
	
	//rtk_dump_lan_client_info(pWiredClient, idx);
	
	return RTK_SUCCESS;
}

int rtk_lan_get_wired_client_list(LAN_DEV_BASE_INFO *pWiredClient, int array_len, int *real_num)
{
	if(pWiredClient==NULL || array_len<1 || real_num==NULL)
		return RTK_FAILED;

	int i, j, k, idx=0;

	RTK_L2_TAB_INFO l2_tab[64];
	int l2_tab_num=0;

	RTK_ARP_TAB_INFO arp_tab[64];
	int arp_tab_num=0;	
	
	LAN_DHCP_CLIENT_INFO dhcp_client[128];
	int dhcp_client_num=0;

	memset(pWiredClient, 0, sizeof(LAN_DEV_BASE_INFO)*array_len);

	if(rtk_ethernet_get_l2_list(l2_tab, 64, &l2_tab_num, rtk_ethernet_get_wan_port_num(),0)==RTK_FAILED)
		return RTK_FAILED;

	//printf("\nl2_tab_num=%d\n",l2_tab_num);
	
	rtk_get_dhcp_client_list(&dhcp_client_num,&dhcp_client);
	//printf("\ndhcp_client_num=%d\n",dhcp_client_num);
	
	rtk_get_arp_table_list(arp_tab, 64, &arp_tab_num);
	//printf("\narp_tab_num=%d\n",arp_tab_num);

	for(i=0;i<l2_tab_num && idx<array_len; i++)
	{
		for(j=0;j<dhcp_client_num;j++)
		{
			if(memcmp(l2_tab[i].mac, dhcp_client[j].mac, MAC_ADDR_LEN)==0)
			{
				pWiredClient[idx].ip=dhcp_client[j].ip;					
				strncpy(pWiredClient[idx].dev_name, dhcp_client[j].hostname, sizeof(pWiredClient[idx].dev_name)-1);
				pWiredClient[idx].expires=dhcp_client[j].expires;
				break;
			}				
		}			
		if(j==dhcp_client_num)
		{
			for(k=0; k<arp_tab_num; k++)
			if(memcmp(l2_tab[i].mac, arp_tab[k].mac, MAC_ADDR_LEN)==0)
			{
				//printf("\nmac is equal!\n");
				pWiredClient[idx].ip=arp_tab[k].ip;	
				break;
			}
			else
			{
				//printf("\nl2_tab[%d].mac=%02X:%02X:%02X:%02X:%02X:%02X\n",i,l2_tab[i].mac[0],l2_tab[i].mac[1],l2_tab[i].mac[2],l2_tab[i].mac[3],l2_tab[i].mac[4],l2_tab[i].mac[5]);				
				//printf("\narp_tab[%d].mac=%02X:%02X:%02X:%02X:%02X:%02X\n",k,arp_tab[k].mac[0],arp_tab[k].mac[1],arp_tab[k].mac[2],arp_tab[k].mac[3],arp_tab[k].mac[4],arp_tab[k].mac[5]);
			}
			
			//if(k==arp_tab_num)
				//continue;			
		}
		
		//if can't get hostname, use mac address as hostname
		if(pWiredClient[idx].dev_name[0]==0 || strcmp(pWiredClient[idx].dev_name, "unknown")==0)
			sprintf(pWiredClient[idx].dev_name, "%02X-%02X-%02X-%02X-%02X-%02X",l2_tab[i].mac[0],l2_tab[i].mac[1],l2_tab[i].mac[2],l2_tab[i].mac[3],l2_tab[i].mac[4],l2_tab[i].mac[5]);
		
		memcpy(pWiredClient[idx].mac, l2_tab[i].mac, MAC_ADDR_LEN);					
		pWiredClient[idx].conn_port=l2_tab[i].port_num;

		pWiredClient[idx].conn_type=LAN_DEV_CONN_WIRED;

		idx++;		
	}	
	
	*real_num=idx;
	
	return RTK_SUCCESS;	
}

int rtk_send_arp(unsigned int dst_ip, unsigned int src_ip, unsigned char *src_mac, char *send_ifname)
{
	int	timeout = 1;
	int optval = 1;
	int	s;			/* socket */
	int	rv = 1;			/* return value */
	struct sockaddr addr;		/* for interface name */
	RTK_ARP_MSG	arp;
	fd_set		fdset;
	struct timeval	tm;
	time_t		prevTime;

	if ((s = socket (PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) == -1) 
	{
		printf("Could not open raw socket");
		return -1;
	}
	
	if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) 
	{
		printf("Could not setsocketopt on raw socket");
		close(s);
		return -1;
	}

	/* send arp request */
	memset(&arp, 0, sizeof(arp));
	memcpy(arp.ethhdr.h_dest, MAC_BCAST_ADDR, 6);	/* MAC DA */
	memcpy(arp.ethhdr.h_source, src_mac, 6);		/* MAC SA */
	arp.ethhdr.h_proto = htons(ETH_P_ARP);		/* protocol type (Ethernet) */
	arp.htype = htons(ARPHRD_ETHER);		/* hardware type */
	arp.ptype = htons(ETH_P_IP);			/* protocol type (ARP message) */
	arp.hlen = 6;					/* hardware address length */
	arp.plen = 4;					/* protocol address length */
	arp.operation = htons(ARPOP_REQUEST);		/* ARP op code */
	memcpy(arp.sInaddr, &src_ip, sizeof(src_ip));		/* source IP address */
	memcpy(arp.sHaddr, src_mac, 6);			/* source hardware address */
	memcpy(arp.tInaddr, &dst_ip, sizeof(dst_ip));	/* target IP address */
	
	memset(&addr, 0, sizeof(addr));
	strcpy(addr.sa_data, send_ifname);
	if (sendto(s, &arp, sizeof(arp), 0, &addr, sizeof(addr)) < 0)
		rv = 0;
	
	/* wait arp reply, and check it */
	tm.tv_usec = 0;
	time(&prevTime);
	while (timeout > 0) 
	{
		FD_ZERO(&fdset);
		FD_SET(s, &fdset);
		tm.tv_sec = timeout;
		if (select(s + 1, &fdset, (fd_set *) NULL, (fd_set *) NULL, &tm) < 0) 
		{
			if (errno != EINTR) 
				rv = 0;
		} 
		else if (FD_ISSET(s, &fdset)) 
		{
			if (recv(s, &arp, sizeof(arp), 0) < 0 ) 
				rv = 0;
			if (arp.operation == htons(ARPOP_REPLY) && bcmp(arp.tHaddr, src_mac, 6) == 0 && *((u_int *) arp.sInaddr) == dst_ip) 
			{
				//printf("Valid arp reply receved for this address");
				rv = 0;
				break;
			}
		}
		timeout -= time(NULL) - prevTime;
		time(&prevTime);
	}
	close(s);
	//printf("%salid arp replies for this address", rv ? "No v" : "V");	 
	return rv;
}

