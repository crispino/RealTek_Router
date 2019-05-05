#ifndef _RTK_ISP_ETHERNET_ADAPTER_H
#define _RTK_ISP_ETHERNET_ADAPTER_H

#ifndef __RTL_API_H__
enum RTK_LINK_STATUS 
{
	LINK_ERROR,
	LINK_UP,
	LINK_DOWN
};
#endif

typedef struct rtk_l2_tab_info
{
	int port_num;
	unsigned char mac[MAC_ADDR_LEN];
	//unsigned int timestamp;
	//unsigned int link_time;
	int is_alive;
}RTK_L2_TAB_INFO;

typedef struct rtk_arp_tab_info
{
	unsigned int ip;
	unsigned char mac[MAC_ADDR_LEN];	
}RTK_ARP_TAB_INFO;

typedef struct rtk_eth_port_info
{
	int port_num;
	unsigned int tx_bytes;
	unsigned int rx_bytes;
	unsigned int up_speed;
	unsigned int down_speed;
	//unsigned int link_time;
	//enum RTK_LINK_STATUS link_status;
}RTK_ETH_PORT_INFO;
	

#endif
