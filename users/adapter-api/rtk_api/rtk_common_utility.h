#ifndef _RTK_ISP_COMMON_UTILITY_H
#define _RTK_ISP_COMMON_UTILITY_H


#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <errno.h>
#include <linux/sockios.h>
#include <netinet/in.h>
#include <fcntl.h>
//#include <net/if.h>
#include <arpa/inet.h>
#include <linux/wireless.h>

#include <net/if_arp.h>
#include <linux/if_ether.h>
#include <netdb.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <netinet/if_ether.h>
#include <sys/sysinfo.h>

//#include "list.h"

#include "../../boa/apmib/apmib.h"

#define RTK_FAILED      -1
#define RTK_SUCCESS 	 0

#ifndef IFNAMSIZ
#define IFNAMSIZ               16
#endif
#define MAX_PORT_NUMBER 5

#define IFACE_FLAG_T 	0x01
#define IP_ADDR_T 		0x02
#define NET_MASK_T 		0x04
#define HW_ADDR_T 		0x08
#define DST_ADDR_T 		0x10

#define RTF_UP			0x0001          /* route usable                 */
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */


#define ETH_PORT_INFO  "/var/eth_port_info"
#define LAN_DEV_LINK_INFO	"/var/lan_dev_link_info"

#define MAC_BCAST_ADDR	(unsigned char *) "\xff\xff\xff\xff\xff\xff"

typedef struct rtk_arp_msg 
{
	struct ethhdr ethhdr;	 		/* Ethernet header */
	u_short htype;				/* hardware type (must be ARPHRD_ETHER) */
	u_short ptype;				/* protocol type (must be ETH_P_IP) */
	u_char  hlen;				/* hardware address length (must be 6) */
	u_char  plen;				/* protocol address length (must be 4) */
	u_short operation;			/* ARP opcode */
	u_char  sHaddr[6];			/* sender's hardware address */
	u_char  sInaddr[4];			/* sender's IP address */
	u_char  tHaddr[6];			/* target's hardware address */
	u_char  tInaddr[4];			/* target's IP address */
	u_char  pad[18];			/* pad for min. Ethernet payload (60 bytes) */
}RTK_ARP_MSG;













#endif








