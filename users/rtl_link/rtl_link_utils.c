#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/wireless.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>

#include "rtl_link_utils.h"
#include "apmib.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.2/dmalloc.h"
#endif


static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

int is_hex_string(const char *str, int len)
{
	int ret = 1, i;

	for(i=0; i<len; ++i){
		if(!_is_hex(str[i])){
			ret = 0;
			break;
		}
	}

	return ret;
}

void format_mac(unsigned char *str_mac, unsigned char *mac)
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

int getDefaultRoute(char *interface, struct in_addr *route)
{
	char buff[1024], iface[16];
	char gate_addr[128], net_addr[128], mask_addr[128];
	int num, iflags, metric, refcnt, use, mss, window, irtt;
	FILE *fp = fopen(_PATH_PROCNET_ROUTE, "r");
	char *fmt;
	int found=0;
	unsigned long addr;

	if (!fp) {
       	ERR_PRINT("ERROR (%s)%d Open %s file error.\n", __FUNCTION__, __LINE__, _PATH_PROCNET_ROUTE);
		return 0;
    }

	fmt = "%16s %128s %128s %X %d %d %d %128s %d %d %d";

	while (fgets(buff, 1023, fp)) {
		num = sscanf(buff, fmt, iface, net_addr, gate_addr,
		     		&iflags, &refcnt, &use, &metric, mask_addr, &mss, &window, &irtt);
		if (num < 10 || !(iflags & RTF_UP) || !(iflags & RTF_GATEWAY) || strcmp(iface, interface))
	    		continue;
		sscanf(gate_addr, "%lx", &addr );
		*route = *((struct in_addr *)&addr);

		found = 1;
		break;
	}

    fclose(fp);
    return found;
}

int _string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1])){
		    ERR_PRINT("ERROR (%s)%d wrong format\n", __FUNCTION__, __LINE__);
			return -1;
	    }

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 0;
}

int equal_mac(unsigned char mac1[6],unsigned char mac2[6])
{
	if((mac1[0]==mac2[0])&& (mac1[1]==mac2[1])&&(mac1[2]==mac2[2])&&
	   (mac1[3]==mac2[3])&& (mac1[4]==mac2[4])&&(mac1[5]==mac2[5]))
	   return 1;
	return 0;
}

/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
static inline int iw_get_ext(int skfd,    /* Socket to the kernel */
           			char *ifname,        	/* Device name */
           			int request,        		/* WE ID */
           			struct iwreq *pwrq)    /* Fixed part of the request */
{
  	strncpy(pwrq->ifr_name, ifname, IFNAMSIZE);	/* Set device name */
	//printf("[iw_get_ext] skfd=%d, request=%d, pwrq->ifr_name=%s", skfd, request,pwrq->ifr_name);
  	return(ioctl(skfd, request, pwrq));			/* Do the request */
}

void convert_bin_to_str(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		snprintf(tmpbuf, sizeof(tmpbuf), "%02x", bin[i]);
		strcat(out, tmpbuf);
	}
}

int pidfile_acquire(char *pidfile)
{
	int pid_fd;
	if (pidfile == NULL){ 
        ERR_PRINT("ERROR (%s)%d invalid pidfile name\n", __FUNCTION__, __LINE__);
		return -1;
    }

	pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
	if (pid_fd < 0) 
		ERR_PRINT("ERROR (%s)%d Unable to open pidfile %s\n", __FUNCTION__, __LINE__, pidfile);
	else 
		lockf(pid_fd, F_LOCK, 0);
	
	return pid_fd;
}

void pidfile_write_release(int pid_fd)
{
	FILE *out;

	if (pid_fd < 0) 
		return;

	if ((out = fdopen(pid_fd, "w")) != NULL) 
	{
		fprintf(out, "%d\n", getpid());
		fclose(out);
	}
	lockf(pid_fd, F_UNLCK, 0);
	close(pid_fd);
}

int isFileExist(char *file_name)
{
	struct stat status;

	if ( stat(file_name, &status) < 0)
		return 0;

	return 1;
}

void Create_script(char *script_path, char *iface, int network, char *ipaddr, char *mask, char *gateway)
{
	
	unsigned char tmpbuf[100];
	int fh;
	
	fh = open(script_path, O_RDWR|O_CREAT|O_TRUNC, S_IRWXO|S_IRWXG);	
	if (fh < 0) {
		fprintf(stderr, "Create %s file error!\n", script_path);
		return;
	}
	if(network==LAN_NETWORK){
		sprintf((char *)tmpbuf, "%s", "#!/bin/sh\n");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "ifconfig %s %s netmask %s\n", iface, ipaddr, mask);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "while route del default dev %s\n", iface);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "%s\n", "do :");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "%s\n", "done");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "route add -net default gw %s dev %s\n", gateway, iface);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		//sprintf((char *)tmpbuf, "%s\n", "init.sh ap wlan_app");
		//write(fh, tmpbuf, strlen((char *)tmpbuf));
	}
	if(network==WAN_NETWORK){
		sprintf((char *)tmpbuf, "%s", "#!/bin/sh\n");
		write(fh, tmpbuf, strlen((char *)tmpbuf));

#if 0 //def CONFIG_POCKET_ROUTER_SUPPORT //it needn't do this
		sprintf((char *)tmpbuf, "sysconf disc dhcpc\n");
		write(fh, tmpbuf, strlen((char *)tmpbuf));			
#endif	

		sprintf((char *)tmpbuf, "ifconfig %s 0.0.0.0\n", iface);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		
	}
	close(fh);
}

int write_line_to_file(char *filename, int mode, char *line_data)
{
	unsigned char tmpbuf[512];
	int fh=0;

	if(mode == 1) {/* write line datato file */
		
		fh = open(filename, O_RDWR|O_CREAT|O_TRUNC);
		
	}else if(mode == 2){/*append line data to file*/
		
		fh = open(filename, O_RDWR|O_APPEND);	
	}
	
	
	if (fh < 0) {
		fprintf(stderr, "Create %s error!\n", filename);
		return 0;
	}


	sprintf((char *)tmpbuf, "%s", line_data);
	write(fh, tmpbuf, strlen((char *)tmpbuf));



	close(fh);
	return 1;
}

void set_lan_dhcpc(char *iface)
{
	char script_file[100], deconfig_script[100], pid_file[100];
	char *strtmp=NULL;
	char tmp[32], Ip[32], Mask[32], Gateway[32];
	char cmdBuff[200];
#ifdef  HOME_GATEWAY
	int intValue=0;
#endif
	sprintf(script_file, "/usr/share/udhcpc/%s.sh", iface); /*script path*/
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", iface);/*deconfig script path*/
	sprintf(pid_file, "/etc/udhcpc/udhcpc-%s.pid", iface); /*pid path*/
	apmib_get( MIB_IP_ADDR,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Ip, "%s",strtmp);

	apmib_get( MIB_SUBNET_MASK,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Mask, "%s",strtmp);

	apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Gateway, "%s",strtmp);

	Create_script(deconfig_script, iface, LAN_NETWORK, Ip, Mask, Gateway);

	//RunSystemCmd(NULL_FILE, "udhcpc", "-i", iface, "-p", pid_file, "-s", script_file,  "-n", "-x", NULL_STR);
	//sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -n &", iface, pid_file, script_file);
	sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s &", iface, pid_file, script_file);
	system(cmdBuff);

#if defined(CONFIG_APP_SIMPLE_CONFIG)
	system("echo 0 > /var/sc_ip_status");
#endif
}

void set_lan_dhcpd(char *interface, int mode)
{
	char tmpBuff1[32]={0}, tmpBuff2[32]={0};
	char cmdBuff[200];
	int intValue=0, dns_mode=0;
	char line_buffer[100]={0};
	char tmp1[64]={0};
	char tmp2[64]={0};
	char *strtmp=NULL, *strtmp1=NULL;
	DHCPRSVDIP_T entry;
	int i, entry_Num=0, ret_val=0;
#ifdef   HOME_GATEWAY
	char tmpBuff3[32]={0};
#endif
	unsigned int lan_ip, lan_netmask, reserve_ip; 
#if defined(CONFIG_RTL_ULINKER)
{//// must be gateway mode!
	int opmode;
	int auto_wan;
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if (opmode==GATEWAY_MODE || opmode==WISP_MODE) {
		system("brctl addif br0 usb0 > /dev/null 2>&1");
	#if 0
		apmib_get(MIB_ULINKER_AUTO,(void *)&auto_wan);
		if (auto_wan == 0)
			system("ifconfig usb0 0.0.0.0 > /dev/null 2>&1");
	#endif
	}
}
#endif

#if defined(CONFIG_RTL_ULINKER_NOT_8881A)
	system("ifconfig usb0 up");
	system("brctl addif br0 usb0");
#endif

#if 0//def _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	ret_val=create_dhcpd_configfile(interface);
	if(ret_val<1)
		create_dhcpd_default_configfile(interface, mode);
#else	
	
	sprintf(line_buffer,"interface %s\n",interface);
	write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_START,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"start %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_END,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"end %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_SUBNET_MASK,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"opt subnet %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&intValue);
    if( (intValue==0) || (intValue<0) || (intValue>10080))
    {
		intValue = 480; //8 hours
		if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&intValue))
		{
			printf("set MIB_DHCP_LEASE_TIME error\n");
		}

		apmib_update(CURRENT_SETTING);
    }
	intValue *= 60;
    sprintf(line_buffer,"opt lease %ld\n",intValue);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	if(mode==1){//ap
		apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp2);
		if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmp2));
			sprintf(line_buffer,"opt router %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}else
		{
			apmib_get(MIB_IP_ADDR,  (void *)tmp1);
			strtmp= inet_ntoa(*((struct in_addr *)tmp1));
			sprintf(line_buffer,"opt router %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}


	}else{
		apmib_get(MIB_IP_ADDR,  (void *)tmp1);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(line_buffer,"opt router %s\n",strtmp);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
#ifdef   HOME_GATEWAY
		apmib_get( MIB_DNS_MODE, (void *)&dns_mode);
		if(dns_mode==0){
			sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
#endif
	}
	intValue=0;
	if((mode==1) 
#if 1
	||(mode==2 && dns_mode==1)
#endif
	){
#if defined(HOME_GATEWAY) && !(defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_RTL_ULINKER))
		apmib_get( MIB_DNS1,  (void *)tmpBuff1);
		apmib_get( MIB_DNS2,  (void *)tmpBuff2);
		apmib_get( MIB_DNS3,  (void *)tmpBuff3);

		if (memcmp(tmpBuff1, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff1));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
		if (memcmp(tmpBuff2, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff2));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
		if (memcmp(tmpBuff3, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff3));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
#endif

#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_RTL_ULINKER)
		apmib_get(MIB_IP_ADDR,  (void *)tmp1);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
#endif //#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT

		if(intValue==0){ /*no dns option for dhcp server, use default gatewayfor dns opt*/

			if(mode==1){
				apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp2);
				if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
					strtmp= inet_ntoa(*((struct in_addr *)tmp2));
					sprintf(line_buffer,"opt dns %s\n",strtmp);
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}else{
					apmib_get( MIB_IP_ADDR,  (void *)tmp2);
					if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
						strtmp= inet_ntoa(*((struct in_addr *)tmp2));
						sprintf(line_buffer,"opt dns %s\n",strtmp);
						write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
					}
				}
			}else {
				apmib_get( MIB_IP_ADDR,  (void *)tmp2);
				if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
					strtmp= inet_ntoa(*((struct in_addr *)tmp2));
					sprintf(line_buffer,"opt dns %s\n",strtmp);
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}
			}
		}
	}
	memset(tmp1, 0x00, 64);
	apmib_get( MIB_DOMAIN_NAME, (void *)&tmp1);
	if(tmp1[0]){
		sprintf(line_buffer,"opt domain %s\n",tmp1);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
/*static dhcp entry static_lease 000102030405 192.168.1.199*/
	intValue=0;
#ifdef SUPPORT_DHCP_PORT_IP_BIND
	apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
	if(entry_Num>0){
			for (i=1; i<=entry_Num; i++) {
				*((char *)&entry) = (char)i;
				apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry);
				if(entry.portId>0)
				{
					sprintf(line_buffer, "static_lease %d %s\n", entry.portId, inet_ntoa(*((struct in_addr*)entry.ipAddr)));
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}			
			}
		}			
#endif
	apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intValue);
	if(intValue==1){
		apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
		if(entry_Num>0){
			apmib_get(MIB_IP_ADDR, (void *)&lan_ip);
			apmib_get(MIB_SUBNET_MASK, (void *)&lan_netmask);
			for (i=1; i<=entry_Num; i++) {
				*((char *)&entry) = (char)i;
				apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry);
				memcpy(&reserve_ip, entry.ipAddr, 4);
				if((reserve_ip & lan_netmask) != (lan_ip & lan_netmask))
				{
					apmib_set(MIB_DHCPRSVDIP_DEL, (void *)&entry);
					reserve_ip=((~lan_netmask) & reserve_ip) | (lan_ip & lan_netmask);
					memcpy(entry.ipAddr, &reserve_ip, 4);
					apmib_set(MIB_DHCPRSVDIP_ADD, (void *)&entry);
				}
				if(memcmp(entry.macAddr,"\x00\x00\x00\x00\x00\x00",6))
				{
				sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], inet_ntoa(*((struct in_addr*)entry.ipAddr)));
				write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}
			}
		}
	}
#endif

	/* may not need to set ip again*/
	apmib_get(MIB_IP_ADDR,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(tmpBuff1, "%s", strtmp);
	apmib_get(MIB_SUBNET_MASK,  (void *)tmp2);
	strtmp1= inet_ntoa(*((struct in_addr *)tmp2));
	sprintf(tmpBuff2, "%s", strtmp1);
	//RunSystemCmd(NULL_FILE, "ifconfig", interface, tmpBuff1, "netmask", tmpBuff2,  NULL_STR);
	sprintf(cmdBuff, "ifconfig %s %s netmask %s", interface, tmpBuff1, tmpBuff2);
	system(cmdBuff);
	/*start dhcp server*/
	#if defined(CONFIG_AUTO_DHCP_CHECK)
		int opmode;
		apmib_get(MIB_OP_MODE,(void *)&opmode);
		if(opmode != BRIDGE_MODE)
		{
			char tmpBuff4[100];
			sprintf(tmpBuff4,"udhcpd %s\n",DHCPD_CONF_FILE);
			system(tmpBuff4);
		}
	#else
		char tmpBuff4[100];
		sprintf(tmpBuff4,"udhcpd %s\n",DHCPD_CONF_FILE);
		system(tmpBuff4);
	#endif
	//RunSystemCmd(stdout, "udhcpd", DHCPD_CONF_FILE, NULL_STR);
	
#if defined(CONFIG_APP_SIMPLE_CONFIG)
	system("echo 1 > /var/sc_ip_status");
#endif

}

void start_wan_dhcp_client(char *iface)
{
	char hostname[100];
	char cmdBuff[200];
	char script_file[100], deconfig_script[100], pid_file[100];
	
	sprintf(script_file, "/usr/share/udhcpc/%s.sh", iface); /*script path*/
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", iface);/*deconfig script path*/
	sprintf(pid_file, "/etc/udhcpc/udhcpc-%s.pid", iface); /*pid path*/
	//killDaemonByPidFile(pid_file);
	Create_script(deconfig_script, iface, WAN_NETWORK, 0, 0, 0);
	memset(hostname, 0x00, 100);
	apmib_get( MIB_HOST_NAME, (void *)&hostname);

	if(hostname[0]){
		sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -h %s -a 5 &", iface, pid_file, script_file, hostname);
		//RunSystemCmd(NULL_FILE, "udhcpc", "-i", iface, "-p", pid_file, "-s", script_file,  "-a", "30", "-h", hostname,  NULL_STR);
	}else{
		sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -a 5 &", iface, pid_file, script_file);
		//RunSystemCmd(NULL_FILE, "udhcpc", "-i", iface, "-p", pid_file, "-s", script_file,  "-a", "30", NULL_STR);
	}
	system(cmdBuff);
}

void set_dhcp_client(char *wan_iface)
{
	int intValue=0;
	char tmp_args[16], cmd_buf[128];
	
	apmib_get(MIB_DHCP_MTU_SIZE, (void *)&intValue);
	sprintf(tmp_args, "%d", intValue);
	snprintf(cmd_buf, sizeof(cmd_buf), "ifconfig %s mtu %s", wan_iface, tmp_args);
	system(cmd_buf);
	start_wan_dhcp_client(wan_iface);

//	start_upnp_igd(DHCP_CLIENT, sys_op, wisp_id, lan_iface);

}

void start_dhcpd_with_ip(char *ifname, char *ip, char *ip_start, char *ip_end)
{
    char line_buffer[100]={0};
    char *strtmp=NULL;
    char tmp1[64]={0};
    unsigned int tmp1Val;
    struct in_addr myIp, myMask, mask;
    unsigned int inIpVal, inMaskVal, myIpVal, myMaskVal, maskVal;
    char tmpBufIP[64]={0}, tmpBufMask[64]={0};


    getInAddr("br0", IP_ADDR_T, (void *)&myIp );    
    getInAddr("br0", NET_MASK_T, (void *)&myMask );
        
    
    memcpy(&myIpVal, &myIp, 4);
    memcpy(&myMaskVal, &myMask, 4);
    
    {
        struct in_addr myIp_tmp;
        char ip_buf[64] = {0}, mask_buf[64] = {0};
        char route_buf[100] = {0};
        char *strtmp_tmp = NULL;
        
        myIp_tmp.s_addr = myIp.s_addr&myMask.s_addr;
        strtmp_tmp = inet_ntoa(myIp_tmp);
        sprintf(ip_buf, "%s", strtmp_tmp);
        strtmp_tmp = inet_ntoa(myMask);
        sprintf(mask_buf, "%s", strtmp_tmp);
        //printf("\r\n ip_buf=[%s],mask_buf=[%s]__[%s-%u]\r\n", ip_buf, mask_buf, __FILE__, __LINE__);

        sprintf(route_buf, "route del -net %s netmask %s dev br0", ip_buf, mask_buf);
        system(route_buf);
    }
    
    

    system("killall -15 udhcpd 2> /dev/null");
    
    sprintf(line_buffer,"interface %s\n","br0");
    write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);
    
    sprintf(line_buffer,"start %s\n", ip_start);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
    
    sprintf(line_buffer,"end %s\n", ip_end);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

    sprintf(line_buffer,"opt subnet %s\n",inet_ntoa(myMask));
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
    
    apmib_get(MIB_DHCP_LEASE_TIME, (void *)&tmp1Val);
    if( (tmp1Val==0) || (tmp1Val<0) || (tmp1Val>10080))
    {
        tmp1Val = 480; //8 hours
        if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&tmp1Val))
        {
            printf("set MIB_DHCP_LEASE_TIME error\n");
        }
    
        apmib_update(CURRENT_SETTING);
    }
    tmp1Val *= 60;

    sprintf(line_buffer,"opt lease %ld\n",tmp1Val);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

    sprintf(line_buffer,"opt router %s\n",ip);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

    sprintf(line_buffer,"opt dns %s\n",ip); /*now strtmp is ip address value */
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
    
    memset(tmp1,0x00,sizeof(tmp1));
    apmib_get( MIB_DOMAIN_NAME, (void *)&tmp1);
    if(tmp1[0]){
        sprintf(line_buffer,"opt domain %s\n",tmp1);
        write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
    }
    
    memset(line_buffer,0x00,sizeof(line_buffer));
    sprintf(line_buffer, "ifconfig br0 %s netmask %s", ip, inet_ntoa(myMask));
    system(line_buffer);

    sprintf(line_buffer, "udhcpd %s", DHCPD_CONF_FILE);
    system(line_buffer);

    return;
}

int getInAddr(char *interface, int type, void *pAddr)
{
    struct ifreq ifr;
    int skfd, found=0;
    struct sockaddr_in *addr;
    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0){
        close( skfd );
        return (0);
    }
    if (type == HW_ADDR_T) {
        if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
            memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
            found = 1;
        }
    }
    else if (type == IP_ADDR_T) {
        if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) {
            addr = ((struct sockaddr_in *)&ifr.ifr_addr);
            *((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
            found = 1;
        }
    }
    else if (type == NET_MASK_T) {
        if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) {
            addr = ((struct sockaddr_in *)&ifr.ifr_addr);
            *((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
            found = 1;
        }
    }else {

        if (ioctl(skfd, SIOCGIFDSTADDR, &ifr) >= 0) {
            addr = ((struct sockaddr_in *)&ifr.ifr_addr);
            *((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
            found = 1;
        }

    }
    close( skfd );
    return found;

}

int getWlBssInfo_andlink(char *interface, bss_info *pInfo)
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
      {
      	 close( skfd );
        return -1;
      }

    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(bss_info);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSINFO, &wrq) < 0){
    	 close( skfd );
	return -1;
	}
    close( skfd );
#else
    memset(pInfo, 0, sizeof(bss_info)); 
#endif

    return 0;
}


int getWlSiteSurveyRequest(char *interface, int *pStatus)
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;
    unsigned char result;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
	}
    wrq.u.data.pointer = (caddr_t)&result;
    wrq.u.data.length = sizeof(result);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSCANREQ, &wrq) < 0){
    	//close( skfd );
	//return -1;
	}
    close( skfd );

    if ( result == 0xff )
    	*pStatus = -1;
    else
	*pStatus = (int) result;
#else
	*pStatus = -1;
#endif
#ifdef CONFIG_RTK_MESH 
	// ==== modified by GANTOE for site survey 2008/12/26 ==== 
	return (int)*(char*)wrq.u.data.pointer; 
#else
	return 0;
#endif
}


int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus )
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
    wrq.u.data.pointer = (caddr_t)pStatus;

    if ( pStatus->number == 0 )
    	wrq.u.data.length = sizeof(SS_STATUS_T);
    else
        wrq.u.data.length = sizeof(pStatus->number);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSDB, &wrq) < 0){
    	close( skfd );
	return -1;
	}
    close( skfd );
#else
	return -1 ;
#endif

    return 0;
}

pid_t find_pid_by_name( char* pidName)
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

void rtl_link_generateRandomData(char *random, int len)
{
    int i;
    struct timeval tod;

    gettimeofday(&tod, NULL);
    srand(tod.tv_usec);
    
    for(i=0; i<len; i++)
    {
        random[i] = rand()%16;
        if(random[i]>=10)
            random[i]=random[i]-10+'a';
        else
            random[i]=random[i]+'0';
    }
    random[len]='\0';
}


int is_valid_ipv4_addr(char *str)   
{  
    char temp[31];  
    int a,b,c,d;  
    
    if(sscanf(str, "%d.%d.%d.%d ",&a,&b,&c,&d)==4 && a>=0&&a<=255&&b>=0&&b<=255&&c>=0&&c<=255&&d>=0&&d<=255)  
    {  
        sprintf(temp, "%d.%d.%d.%d",a,b,c,d);
        if(strcmp(temp,str)==0)   
        {  
            return 1;   
        }   
        else  
        {  
            return 0;  
        }  
    }  
    else   
    {  
        return 0;  
    }

    return 0;
}  

