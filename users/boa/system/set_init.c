#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "apmib.h"
#include "mibtbl.h"
#include "upmib.h"
#include "sysconf.h"
#include "sys_utility.h"
#include "syswan.h"
#ifdef 	CONFIG_RTL_ULINKER
#include <signal.h>
#endif
//extern int wlan_idx;	// interface index 
//extern int vwlan_idx;	// initially set interface index to root   
extern int set_QoS(int operation, int wan_type, int wisp_wan_id);
extern int setbridge(char *argv);
extern int setWlan_Applications(char *action, char *argv);

#ifdef SHRINK_INIT_TIME
extern int cleanWlan_Applications(char* wlanInterface);
#endif
extern int SetWlan_idx(char *wlan_iface_name);
extern int setFirewallIptablesRules(int argc, char** argv);
extern void set_lan_dhcpd(char *interface, int mode);
#ifdef MULTI_PPPOE
extern void wan_disconnect(char *option , char *conncetOrder);
#else
extern void wan_disconnect(char *option);
#endif
extern void set_wanipv6();
extern void setbridgeipv6();
void set_log(void);
#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
extern void Init_Domain_Query_settings(int operation_mode, int wlan_mode, int lan_dhcp_mode, char *lan_mac);
extern void domain_query_start_dnrd(int wlan_mode, int start_dnrd);
#endif
#ifdef CONFIG_SMART_REPEATER
int getWispRptIfaceName(char*pIface,int wlanId);
#endif
#if defined(CONFIG_APP_TR069)
extern void start_tr069(void);
#endif

//#define SDEBUG(fmt, args...) printf("[%s %d]"fmt,__FUNCTION__,__LINE__,## args)
#define SDEBUG(fmt, args...) {}

/*

//eth0 eth1 eth2 eth3 eth4 wlan0 wlan0-msh wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd
//wlan0-wds0 wlan0-wds1 wlan0-wds2 wlan0-wds3 wlan0-wds4 wlan0-wds5 wlan0-wds6 wlan0-wds7

WLAN=>>> wlan0
WLANVXD=>>>wlan0-vxd
WLANVIRTUAL=>> wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd
NUM_=>>>1
VIRTUALNUL=>>>4

*/

int gateway=0;
int enable_wan=0;
int enable_br=0;
char br_interface[16]={0};
char br_lan1_interface[16]={0};
char br_lan2_interface[16]={0};
char wan_interface[16]={0};
char vlan_interface[32]={0};
static char wlan_interface[16]={0};
char wlan_valid_interface[512]={0};
char wlan_virtual_interface[80]={0};
static char wlan_vxd_interface[32]={0};
int num_wlan_interface=0;
int num_wlan_virtual_interface=0;
int num_wlan_vxd_interface=0;

#if defined(CONFIG_RTK_MESH)
char wlan_mesh_interface[22]={0};
int num_wlan_mesh_interface=0;
#endif

#ifdef CONFIG_APP_TR069
char acsURLStr[CWMP_ACS_URL_LEN+1];
#endif //#ifdef CONFIG_APP_TR069

//#define UPGRADE_BOOT_FROM_ROOT
#if 0//def SAMBA_WEB_SUPPORT
void storage_UpdateSambaConf()
{
	char 				tmpBuff[1024];
	int					number,i;
	STORAGE_GROUP_T		group_info;
	int					anonAccess,anonSambaAccess;
	FILE				*fp;
	memset(tmpBuff,'\0',1024);
	fp = fopen("/etc/samba/smb.conf","w+");

	apmib_get(MIB_STORAGE_ANON_ENABLE,(void*)&anonAccess);
	apmib_get(MIB_STORAGE_ANON_DISK_ENABLE,(void*)&anonSambaAccess);
	
	strcpy(tmpBuff,"[global]\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tserver string\t= RTCN 8190 Samba Server\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlog level\t= 0\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tmax xmit\t= 65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\taio write size\t= 65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\taio read size\t= 65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlarge readwrite =\tyes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tgetwd cache =\tyes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tread raw\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\twrite raw\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlpq cache\t= 30\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\toplocks =\tyes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\twinbind nested groups\t= no\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tdomain master\t= no\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlocal master\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tpublic\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tinterfaces\t= br0\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tload printers\t= no\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tprinting\t= bsd\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tprintcap name\t= /dev/null\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tdisable spoolss\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlog file\t= /var/log/log.%m\n");
	if(anonAccess == 1 && anonSambaAccess == 1)
		strcpy(tmpBuff+strlen(tmpBuff),"\tsecurity\t= share\n");
	else
		strcpy(tmpBuff+strlen(tmpBuff),"\tsecurity\t= user\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tsocket options\t= IPTOS_LOWDELAY IPTOS_THROUGHPUT TCP_NODELAY SO_KEEPALIVE TCP_FASTACK SO_RCVBUF=65536 SO_SNDBUF=65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tgetwd cache\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tdns proxy\t= no\n");

	//anonymous shared folder informatio
	if(anonAccess == 1 && anonSambaAccess == 1){
		strcpy(tmpBuff+strlen(tmpBuff),"[share]\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tpath\t=/tmp/usb\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tcomment\t= anonymous user's share folder\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tread only\t= no\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\twritable\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tpublic\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\toplocks\t= no\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tkernel oplocks\t= no\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tcreate mask\t= 0777\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tbrowseable\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tguest ok\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tdirectory mask\t= 0777\n");
		goto setOk_SambaConfUpdate;
	}

	//shared folder information
	apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		*((char*)&group_info) = (char)(i+1);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&group_info);

		if(group_info.storage_group_sharefolder_flag == 1){
			snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"[%s]\n",group_info.storage_group_displayname);
			snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\tpath\t=%s\n",group_info.storage_group_sharefolder);
			strcpy(tmpBuff+strlen(tmpBuff),"\tcomment\t= smbuser's share folder\n");
			snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\tvalid users\t=@%s\n",group_info.storage_group_name);			
			
			if(!strcmp(group_info.storage_group_access,"r-w"))
				snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\twrite list\t=@%s\n",group_info.storage_group_name);
			else if(!strcmp(group_info.storage_group_access,"r"))
				snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\tread list\t=@%s\n",group_info.storage_group_name);
		}
	}
	
setOk_SambaConfUpdate:

	fwrite(tmpBuff,strlen(tmpBuff),1,fp);
	fclose(fp);

	system("killall smbd");
	system("smbd -D");
}
#endif
#ifdef TR181_V6_SUPPORT
static int init_dns_client_server_table = 0;
#endif

int link_down_restart_dhcp6c=0;
#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)
#define RTL_LINK_PID_FILE "/var/run/rtl_link.pid"
#endif

void restartAndmuProcesses( ) 
{

int opmode = -1;
apmib_get(MIB_OP_MODE, &opmode);

#if defined(CONFIG_CMCC_DM)
	system("killall -9 CMCC_DM");

	char cmdline[128] = {0}, CMEI[32] = {0};
	apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)CMEI);
	if (strlen(CMEI))
		snprintf(cmdline, sizeof(cmdline), "CMCC_DM -c %s &",
			CMEI);
	else
		snprintf(cmdline, sizeof(cmdline), "CMCC_DM &");

	printf("%s\n", cmdline);
	
	system(cmdline);
#endif

#if defined(CONFIG_ONLINE_UPDATE)
	static int update_flag = 1;
	if (update_flag)
	{
		update_flag = 0;
		system("killall -9 online_update");
		
		char cmdline[512] = {0}, ser_addr[256] = {0};
		apmib_get(MIB_CUSTOMER_HW_SER_ADDR, (void *)ser_addr);
		if (strlen(ser_addr))
			snprintf(cmdline, sizeof(cmdline), "online_update -s %s &",
				ser_addr);
		else
			snprintf(cmdline, sizeof(cmdline), "online_update -s ftp://192.168.234.234 &");
		
		printf("%s\n", cmdline);
		
		system(cmdline);
	}
#endif

#if defined(CONFIG_APP_SDK_ONENET)
	char cmdBuffer[100];
	char vendor[32] = {0};
	char sVersion[32] = {0};
	unsigned char hw_mac[32] = {0};
	char sn[32] = {0};

	system("killall edp_client");
#endif

#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)
	char strPID[32];
	int pid = 0;
	if(isFileExist(RTL_LINK_PID_FILE)){
		pid=getPid_fromFile(RTL_LINK_PID_FILE);
		if(pid != -1){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
		}
		unlink(RTL_LINK_PID_FILE);
	}
#endif

#if defined(CONFIG_APP_SDK_ONENET)
	if(opmode==GATEWAY_MODE)
	{		
		apmib_get(MIB_RTL_LINK_SWVERSION, (void *)sVersion);
		apmib_get(MIB_RTL_LINK_VENDOR, (void *)vendor);
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)hw_mac);
		apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)sn);
		snprintf(cmdBuffer, sizeof(cmdBuffer), 
				"edp_client -n %s -m %02X:%02X:%02X:%02X:%02X:%02X -s %s -v %s &", 
				sn, hw_mac[0], hw_mac[1], hw_mac[2], hw_mac[3], hw_mac[4], hw_mac[5],
				sVersion, vendor);
		printf("edp_client -n %s -m %02X:%02X:%02X:%02X:%02X:%02X -s %s -v %s \n", 
				sn, hw_mac[0], hw_mac[1], hw_mac[2], hw_mac[3], hw_mac[4], hw_mac[5],
				sVersion, vendor);
		system(cmdBuffer);
		
	}
#endif

#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)
	if(opmode == BRIDGE_MODE || opmode == GATEWAY_MODE)
	{
		int rtl_link_enabled = 0;
		apmib_get(MIB_RTL_LINK_ENABLE, (void *)&rtl_link_enabled);
		if(rtl_link_enabled){
			if(!isFileExist(RTL_LINK_PID_FILE)){
#ifdef CONFIG_ELINK_SUPPORT
				system("elink -m1 -d2");
#else
				system("andlink -m1 -d2");
#endif
			}
		}
	}
#endif

	return;
}

#if defined(CONFIG_4G_LTE_SUPPORT)
int lte_wan(void)
{
	int lte = -1, wan_dhcp = -1;
	apmib_get( MIB_LTE4G,    (void *)&lte);
	apmib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);

	if (wan_dhcp == DHCP_CLIENT && lte == 1) {
		return 1;
	}

	return 0;
}
#endif /* #if defined(CONFIG_4G_LTE_SUPPORT) */
void set_br_interface(unsigned char *brif)
{

	int opmode=-1;
	unsigned char tmpBuff[512]={0};
  apmib_get(MIB_OP_MODE,(void *)&opmode);
	
	if(vlan_interface[0]){
		strcat(tmpBuff, vlan_interface);
		strcat(tmpBuff, " ");
	}
	if(wlan_interface[0]){
		strcat(tmpBuff, wlan_interface);
		strcat(tmpBuff, " ");
	}			
	if(wlan_virtual_interface[0]){
		strcat(tmpBuff, wlan_virtual_interface);
		strcat(tmpBuff, " ");
	}
#if defined(CONFIG_RTL_8198_AP_ROOT) || defined(CONFIG_RTL_8197D_AP)
	if(br_lan2_interface[0]){
		strcat(tmpBuff, br_lan2_interface);
		strcat(tmpBuff, " ");
	}
#endif
	if(wlan_vxd_interface[0]){
		strcat(tmpBuff, wlan_vxd_interface);
		strcat(tmpBuff, " ");
	}
#if defined(CONFIG_RTK_MESH)
#if defined(CONFIG_RTL_MESH_SINGLE_IFACE)
	if(wlan_mesh_interface[0]){
		strcat(tmpBuff, wlan_mesh_interface);
		strcat(tmpBuff, " ");
	}
#else	//!CONFIG_RTL_MESH_SINGLE_IFACE
	if(num_wlan_mesh_interface == 2) {
		sprintf(wlan_mesh_interface, "wlan0-msh0 wlan1-msh0");
		strcat(tmpBuff, wlan_mesh_interface);
		strcat(tmpBuff, " ");
	}
#endif	//CONFIG_RTL_MESH_SINGLE_IFACE
#endif	//CONFIG_RTK_MESH

	strcat(tmpBuff, br_interface);
	strcat(tmpBuff, " ");
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	if(opmode == GATEWAY_MODE)
	{
		strcat(tmpBuff, "eth0 eth2 eth3 eth4");
	}
	else
	{
		strcat(tmpBuff, "eth0 eth1 eth2 eth3 eth4");
	}	
#else
	
	strcat(tmpBuff, br_lan1_interface);

#if defined(CONFIG_RTL_IVL_SUPPORT)
	/*add eth1 to br0 when in bridge&wisp mode*/
	if(opmode == BRIDGE_MODE || opmode == WISP_MODE) 
	{
		strcat(tmpBuff, " ");
		strcat(tmpBuff, br_lan2_interface);
	}
#endif
	#if !defined(CONFIG_RTL_MULTI_LAN_DEV) && !defined(CONFIG_RTL_IVL_SUPPORT)
	/* rtk vlan enable and in bridge&wisp mode and disable ivl and disable multi lan, add eth1 to br0 */
	if (vlan_interface[0])
	{
		if(opmode == BRIDGE_MODE || opmode == WISP_MODE) 
		{
			strcat(tmpBuff, " ");
			strcat(tmpBuff, br_lan2_interface);
		}
	}
	#endif
#endif

#if defined(CONFIG_RTL_ULINKER)
	strcat(tmpBuff, " ");
	strcat(tmpBuff, "usb0");
#endif
	memcpy(brif, tmpBuff, sizeof(tmpBuff));
	return;

}
int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}
int _string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
		{
			return 0;
		}

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}

char * strToValue(char *str,TYPE_T type)
{
	switch(type)
	{
		case BYTE_T:
		case WORD_T:
		case DWORD_T:
		{
			int i=atoi(str);
			*((int*)str)=i;
			return str;
		}
		case STRING_T:
			return str;
		case IA_T:
		{
			struct in_addr addr;
			inet_aton(str,&addr);
			*((struct in_addr*)str)=addr;
			return str;
		}
		case DHCPRSVDIP_ARRY_T:
		{
			DHCPRSVDIP_T staticIPEntry={0};
			char ipAddr[32]={0};
			char macAddr[32]={0};
#ifdef SUPPORT_DHCP_PORT_IP_BIND
			char portId[8]={0};
			sscanf(str,"%s\t%s\t%s\t%s",ipAddr,portId,macAddr,staticIPEntry.hostName);
			staticIPEntry.portId=atoi(portId);
#else
			sscanf(str,"%s\t%s\t%s",ipAddr,macAddr,staticIPEntry.hostName);
#endif
			inet_aton(ipAddr,(struct in_addr *)staticIPEntry.ipAddr);
			_string_to_hex(macAddr,staticIPEntry.macAddr,12);
			*((DHCPRSVDIP_T*)str)=staticIPEntry;
			return str;
		}
		default:
			return str;
	}
}
#if defined(UPGRADE_BOOT_FROM_ROOT)
void upgrade_boot()
{
	int filelen;
	char *msg;
	FILE *src, *dst;
	struct stat buf;

	stat("/etc/boot.bin", &buf);
	filelen=buf.st_size;

	src = fopen("/etc/boot.bin", "rb");
	dst = fopen("/dev/mtdblock0", "wb");

	if(src && dst) 
	{
		msg = (char *)malloc(filelen*sizeof(char));
		fread(msg, filelen, 1, src);
		if(!memcmp(msg, "boot", 4))
		{
			fwrite(&msg[sizeof(IMG_HEADER_T)], filelen, 1, dst);
		}
		free(msg);
		fclose(src);
		fclose(dst);
	}

}
#endif
#ifdef DEFSETTING_AUTO_UPDATE
int defsetting_auto_update()
{
	int need_auto_update=0;
	apmib_get(MIB_NEED_AUTO_UPDATE,(void*)&need_auto_update);
	if(!need_auto_update) return 0;
	apmib_dsAutoUpdate();
	need_auto_update=0;
	apmib_set(MIB_NEED_AUTO_UPDATE,(void*)&need_auto_update);
	apmib_update(CURRENT_SETTING);
}
#endif

#ifdef CONFIG_CMCC
static void convert_bin_to_str(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		sprintf(tmpbuf, "%02x", bin[i]);
		strcat(out, tmpbuf);
	}

	for (i=0; i<strlen(out); i++) {
		out[i] = toupper(out[i]);
	}
}

#ifdef CMCC_SSID_CUSTOMIZE_AP_RPT
int cmcc_is_unwanted_char(unsigned char ch)
{
	int ret = 0;
	if(ch=='0' || ch=='o' || ch=='O' || ch=='B' || ch=='8' || ch=='1' || ch=='I' || ch=='l')
	{
		//printf("unwant %c\n", ch);
		ret = 1;	
	}
	return ret;
}

void get_random_ssid_str(unsigned char s[],int number)
{
	unsigned char str[64] = "00123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int cnt = 0;;
	char ss[2];
	unsigned char ch;
	int idx = 0;
	
	for(;;){
		idx = rand()%62+1;		
		ch = str[idx];			
		if(cmcc_is_unwanted_char(ch))
			continue;			
		cnt++;
		sprintf(ss, "%c", ch);		
		strcat(s,ss);	
		if(cnt==number)
			break;
	}	
	return;
}

int is_ssid_same(unsigned char *ssid)
{
	int ret = 0;
	int i = 0, j=0;
	unsigned char ssidtmp[MAX_SSID_LEN] = {0};

	for(i=0; i<NUM_WLAN_INTERFACE; ++i){
		wlan_idx = i;
		for(j=0; j<(NUM_VWLAN+1); ++j){
			vwlan_idx = j;
			apmib_get(MIB_WLAN_SSID, (void *)ssidtmp);
			//printf("[%s %d]wlan_idx:%d, vwlan_idx:%d, ssidtmp:%s, target ssid:%s\n", __FUNCTION__,__LINE__,
			//	wlan_idx, vwlan_idx, ssidtmp, ssid);
			if(strcmp(ssidtmp, ssid)==0){
				ret = 1;
				break;
			}				
		}
		if(ret)
			break;
	}	
	return ret;
}

int set_ssid_customize_ap_repeater(int flag)
{
	unsigned char ssid[MAX_SSID_LEN] = {0};  
	unsigned char ssidtmp[CMCC_SSID_RANDOM_CHAR_NUM] = {0};
	unsigned int phyBandSelect = 0, is_guest = 0, pos=0;
	int i, j, len = 0;
	unsigned int cnt = 0;
	int wlanidx_bak = 0;

	apmib_save_wlanIdx();
	
	srand((unsigned int)time((time_t *)NULL)); // important		
	for(j=0; j<NUM_WLAN_INTERFACE;++j)
	{
		cnt = 0;
		wlan_idx = j;	
		apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyBandSelect);
		
		if( phyBandSelect==PHYBAND_5G){
			len = strlen(CMCC_SSID_PREFIX_AP_5G);
			memcpy(ssid, CMCC_SSID_PREFIX_AP_5G, len);			
		}else if( phyBandSelect==PHYBAND_2G){
			len = strlen(CMCC_SSID_PREFIX_AP_2G);
			memcpy(ssid, CMCC_SSID_PREFIX_AP_2G, len);			
		}
		
		for(;;)
		{
			memset(ssidtmp, 0, CMCC_SSID_RANDOM_CHAR_NUM);
			get_random_ssid_str(ssidtmp, CMCC_SSID_RANDOM_CHAR_NUM);
			pos = len;			
			memcpy(ssid+pos, ssidtmp, CMCC_SSID_RANDOM_CHAR_NUM);	
			pos += CMCC_SSID_RANDOM_CHAR_NUM;			
			ssid[pos] = '\0';			

		#if 1
			/* if same ssid, then re-generate ssid */	
			wlanidx_bak = wlan_idx;			
			
			if(is_ssid_same(ssid)){
				wlan_idx = wlanidx_bak;
				continue;
			}

			wlan_idx = wlanidx_bak;
			vwlan_idx = cnt;			
			apmib_set(MIB_WLAN_SSID, (void *)&ssid);
			//printf("wlan_idx:%d, vwlan_idx:%d, ssid:%s\n", wlan_idx, vwlan_idx, ssid);
		#endif
		
			cnt++;
			if(cnt == (NUM_VWLAN+1)) // root+vap
				break;
		}				
	}

	apmib_recov_wlanIdx();
	
	apmib_update(CURRENT_SETTING);
	return 0;
}
#endif

/* generate ssid by wlan mac addr */
#if 0
int set_ssid_customize_gw(int flag)
{
	unsigned char ssid[MAX_SSID_LEN] = {0};   
	unsigned char mac[MAC_ADDR_LEN] = {0};
	unsigned char macstr[MAC_ADDR_LEN*2+1] = {0}, macstrtmp[MAC_ADDR_LEN+1] = {0};
	int i = 0, j=0, len=0;	
	unsigned int phyBandSelect = 0, is_guest = 0, pos=0;	
	
#ifdef CUSTOMER_HW_SETTING_SUPPORT
	unsigned char hw_ssid[MAX_SSID_LEN] = {0}, hw_wpa_psk[MAX_PSK_LEN+1] = {0};	
	int set_ssid = 0;
#endif

	apmib_save_wlanIdx();
	
	for(j=0; j<NUM_WLAN_INTERFACE;++j)
	{
		wlan_idx = j;	
		vwlan_idx = 0;
		set_ssid = 0;
		
		apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyBandSelect);
		if( phyBandSelect==PHYBAND_5G ){
			len = strlen(CMCC_SSID_PREFIX_GW_5G);
			memcpy(ssid, CMCC_SSID_PREFIX_GW_5G, len);			
		}else if( phyBandSelect==PHYBAND_2G){
			len = strlen(CMCC_SSID_PREFIX_GW_2G);
			memcpy(ssid, CMCC_SSID_PREFIX_GW_2G, len);			
		}	

#ifdef CUSTOMER_HW_SETTING_SUPPORT
		if(flag == HW_SSID_GEN){
			if(wlan_idx==0){
				apmib_get(MIB_CUSTOMER_HW_WLAN0_SSID, (void *)&hw_ssid);
				apmib_get(MIB_CUSTOMER_HW_WLAN0_WPA_PSK, (void *)&hw_wpa_psk);	
			}else if(wlan_idx==1){
				apmib_get(MIB_CUSTOMER_HW_WLAN1_SSID, (void *)&hw_ssid);
				apmib_get(MIB_CUSTOMER_HW_WLAN1_WPA_PSK, (void *)&hw_wpa_psk);	
			}
			if(strlen(hw_ssid)==0)
				set_ssid = 1;
		}
#endif
		for(i=0; i<NUM_VWLAN_INTERFACE;++i)
		{
			vwlan_idx = i;			
			switch (vwlan_idx)
			{
				case 0:
					apmib_get(MIB_HW_WLAN_ADDR, (void *)mac);
					break;
				case 1:
					apmib_get(MIB_HW_WLAN_ADDR1, (void *)mac);
					break;
				case 2:
					apmib_get(MIB_HW_WLAN_ADDR2, (void *)mac);
					break;
				case 3:
					apmib_get(MIB_HW_WLAN_ADDR3, (void *)mac);
					break;
				case 4:
					apmib_get(MIB_HW_WLAN_ADDR4, (void *)mac);
					break;
				default:
					printf("Fail to get MAC address of VAP%d!\n", vwlan_idx-1);
					apmib_recov_wlanIdx();
					return 0;
			}
			convert_bin_to_str(mac, MAC_ADDR_LEN, macstr);
			pos = len;			
			memcpy(ssid+pos, macstr+6, 6);	
			pos += 6;			
			apmib_get(MIB_WLAN_ACCESS, (void *)&is_guest);
			if(is_guest){									
				memcpy(ssid+pos, CMCC_GUEST_SUFFIX, strlen(CMCC_GUEST_SUFFIX));		
				pos += strlen(CMCC_GUEST_SUFFIX);				
			}			
			ssid[pos] = '\0';			
			
			//printf("[%s %d]ssid:%s\n", __FUNCTION__,__LINE__,ssid);
#ifndef CUSTOMER_HW_SETTING_SUPPORT
			apmib_set(MIB_WLAN_SSID, (void *)&ssid);
#else
			if(set_ssid && (vwlan_idx==0)){						
				apmib_set(MIB_WLAN_SSID, (void *)&ssid);	
			}
			if(vwlan_idx)
				apmib_set(MIB_WLAN_SSID, (void *)&ssid);
#endif				
		}		
	}

	apmib_recov_wlanIdx();

	apmib_update(CURRENT_SETTING);
	return 0;	
}
#else
/* generate ssid by hw wan mac - for cmcc-he router */
int set_ssid_customize_gw(int flag)
{
	unsigned char ssid[MAX_SSID_LEN] = {0};   
	unsigned char mac[MAC_ADDR_LEN] = {0};
	unsigned char macstr[MAC_ADDR_LEN*2+1] = {0}, macstrtmp[MAC_ADDR_LEN+1] = {0};
	int i = 0, j=0, len=0;	
	unsigned int phyBandSelect = 0, is_guest = 0, pos=0;	
	
#ifdef CUSTOMER_HW_SETTING_SUPPORT
	unsigned char hw_ssid[MAX_SSID_LEN] = {0};	
	int set_ssid = 0;
#endif

	apmib_save_wlanIdx();
	
	for(j=0; j<NUM_WLAN_INTERFACE;++j)
	{
		wlan_idx = j;	
		vwlan_idx = 0;
		set_ssid = 0;
		
		apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyBandSelect);
		if(NUM_WLAN_INTERFACE==2){
			if( phyBandSelect==PHYBAND_5G ){
				len = strlen(CMCC_SSID_PREFIX_GW_5G);
				memcpy(ssid, CMCC_SSID_PREFIX_GW_5G, len);			
			}else if( phyBandSelect==PHYBAND_2G){
				len = strlen(CMCC_SSID_PREFIX_GW_2G);
				memcpy(ssid, CMCC_SSID_PREFIX_GW_2G, len);			
			}
		}else {
			len = strlen(CMCC_SSID_PREFIX_SINGLE_BAND);
			memcpy(ssid, CMCC_SSID_PREFIX_SINGLE_BAND, len);	
		}	

#ifdef CUSTOMER_HW_SETTING_SUPPORT
		if(flag == HW_SSID_GEN){
			if(wlan_idx==0){
				apmib_get(MIB_CUSTOMER_HW_WLAN0_SSID, (void *)&hw_ssid);
			}else if(wlan_idx==1){
				apmib_get(MIB_CUSTOMER_HW_WLAN1_SSID, (void *)&hw_ssid);
			}
			if(strlen(hw_ssid)==0)
				set_ssid = 1;
		}
#endif
		for(i=0; i<=NUM_VWLAN_INTERFACE;++i)
		{
			vwlan_idx = i;	
			
			apmib_get(MIB_HW_NIC0_ADDR, (void *)&mac);

			/* GUEST SSID */
			if(vwlan_idx==1)
				(*(char *)(mac+5)) += 0x00;	
			else if(vwlan_idx==2)
				(*(char *)(mac+5)) += 0x01;
			else if(vwlan_idx==3)
				(*(char *)(mac+5)) += 0x02;
			else if(vwlan_idx==4)
				(*(char *)(mac+5)) += 0x03;
			else if(vwlan_idx==5)
				(*(char *)(mac+5)) += 0x04;
			
			convert_bin_to_str(mac, MAC_ADDR_LEN, macstr);
			pos = len;	
			
			#if defined (CONFIG_MODEL_HN100_340EA) ||  defined (CONFIG_MODEL_HN120_341FA)|| defined (CONFIG_MODEL_HN120_341BA) ||defined (CONFIG_MODEL_HN140_361EA)
			memcpy(ssid+pos, macstr+6, 6);	
			pos += 6;
			
			#elif defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
			memcpy(ssid+pos, macstr+8, 4);	
			pos += 4;
			#endif
			
			/* GUEST SSID */
			if(vwlan_idx == 1){									
				memcpy(ssid+pos, CMCC_GUEST_SUFFIX, strlen(CMCC_GUEST_SUFFIX));		
				pos += strlen(CMCC_GUEST_SUFFIX);				
			}			
			ssid[pos] = '\0';			
			
#ifndef CUSTOMER_HW_SETTING_SUPPORT
			apmib_set(MIB_WLAN_SSID, (void *)&ssid);
#else
			if(set_ssid && (vwlan_idx==0)){	
				apmib_set(MIB_WLAN_SSID, (void *)&ssid);	
			}
			if(vwlan_idx)
				apmib_set(MIB_WLAN_SSID, (void *)&ssid);
#endif				
		}		
	}

	apmib_recov_wlanIdx();

	apmib_update(CURRENT_SETTING);
	return 0;	
}
#endif

int set_ssid_customize(int flag)
{
	unsigned int op_mode = 0;

	printf("SSID Customized !\n");	
	apmib_get(MIB_OP_MODE, (void *)&op_mode);	
	
	if(op_mode==GATEWAY_MODE || op_mode==BRIDGE_MODE)
	{
		set_ssid_customize_gw(flag);
	}
#ifdef CMCC_SSID_CUSTOMIZE_AP_RPT
	else if(op_mode==BRIDGE_MODE)	
	{
		set_ssid_customize_ap_repeater(flag);
	}else
		printf("[%s %d]Invalid OPMODE %d\n", __FUNCTION__,__LINE__,op_mode);
#endif
	
	return 0;
}

int set_admin_password()
{
	int ret = 0;
	unsigned char hw_admin_passwd[MAX_USER_PASSWORD_LEN+1] = {0};

	printf("Admin Password Customized !\n");	
	
	apmib_get(MIB_CUSTOMER_HW_ADMIN_PASSWORD, (void *)&hw_admin_passwd);	
	if(strlen(hw_admin_passwd)>0){
		apmib_set(MIB_USER_PASSWORD, (void *)hw_admin_passwd);
	}else
		printf("HW_ADAMIN_PSSWORD NULL\n");	

	return ret;
}

void set_op_mode()
{
	unsigned char hw_op_mode[MAX_OP_MODE_LEN+1] = {0};
	int op_mode = 0;

	printf("Operating mode Customized !\n");
	
	apmib_get(MIB_CUSTOMER_HW_OP_MODE, (void *)&hw_op_mode);	
	if(strlen(hw_op_mode)>0){
		op_mode = atoi(hw_op_mode);
		apmib_set(MIB_OP_MODE, (void *)&op_mode);
	}else
		printf("Operating mode NULL\n");	

	return ;
}

void set_lanip()
{
	char hw_lan_ip[MAX_IP_LEN+1] = {0};
	struct in_addr lan_ip, start_ip, end_ip;

	printf("Lan ip Customized !\n");	
	
	apmib_get(MIB_CUSTOMER_HW_LAN_IP, (void *)&hw_lan_ip);	
	if(strlen(hw_lan_ip)>0){
		int a, b, c, d;
		char hw_dhcp_start[MAX_IP_LEN+1] = {0};
		char hw_dhcp_end[MAX_IP_LEN+1] = {0};

		inet_aton(hw_lan_ip, &lan_ip);
		apmib_set(MIB_IP_ADDR, (void *)&lan_ip);

		if (4 == sscanf(hw_lan_ip, "%d.%d.%d.%d", &a, &b, &c, &d))
		{
			sprintf(hw_dhcp_start, "%d.%d.%d.100", a, b, c);
			sprintf(hw_dhcp_end, "%d.%d.%d.200", a, b, c);
			inet_aton(hw_dhcp_start, &start_ip);
			inet_aton(hw_dhcp_end, &end_ip);
			apmib_set(MIB_DHCP_CLIENT_START, (void *)&start_ip);
			apmib_set(MIB_DHCP_CLIENT_END, (void *)&end_ip);
		}else
			printf("Lan ip format error\n");
	}else
		printf("Lan ip NULL\n");	

	return ;
}

void set_domain_name()
{
	unsigned char hw_domain_name[MAX_DOMAIN_LEN+1] = {0};

	printf("Domain name Customized !\n");	
	
	apmib_get(MIB_CUSTOMER_HW_DOMAIN_NAME, (void *)&hw_domain_name);	
	if(strlen(hw_domain_name)>0){
		apmib_set(MIB_DOMAIN_NAME, (void *)hw_domain_name);
	}else
		printf("Domain name NULL\n");	

	return ;
}

void set_provincial_code()
{
	unsigned char hw_provincial_code[MAX_OP_MODE_LEN+1] = {0};
	int provincial_code = 0;

	printf("Provincial code Customized !\n");
	
	apmib_get(MIB_CUSTOMER_HW_PROVINCIAL_CODE, (void *)&hw_provincial_code);	
	if(strlen(hw_provincial_code)>0){
		provincial_code = atoi(hw_provincial_code);
		apmib_set(MIB_PROVINCIAL_CODE, (void *)&provincial_code);
	}else
		printf("Domain name NULL\n");	

	return ;
}

int ipv4_addr_isValid(const char *ip_addr)   
{  
    char temp[MAX_IP_LEN+1] = {0};
    int a,b,c,d;

    if(sscanf(ip_addr, "%d.%d.%d.%d ",&a,&b,&c,&d)==4 && a>=0&&a<=255&&b>=0&&b<=255&&c>=0&&c<=255&&d>=0&&d<=255) {
        sprintf(temp, "%d.%d.%d.%d",a,b,c,d);
        return strcmp(temp, ip_addr);
    }

    return -1;
}  

#ifdef CONFIG_ANDLINK_SUPPORT
void set_cgw_ip_addr()
{
	int port = 0;
	char hw_cgw_ip_addr[MAX_IP_LEN+6] = {0};
	char cgw_ip_addr[MAX_IP_LEN+1] = {0};
	struct in_addr ia_val;

	printf("Andlink cgw IP Addr Customized !\n");
	
	apmib_get(MIB_CUSTOMER_HW_CGW_IP_ADDR, (void *)&hw_cgw_ip_addr);
	if(strlen(hw_cgw_ip_addr) == 0) {
		printf("HW_CGW_IP_ADDR NULL\n");
		return;
	}

	char *pos = strchr(hw_cgw_ip_addr, ':');
	if (pos) {
		strncpy(cgw_ip_addr, hw_cgw_ip_addr, pos - hw_cgw_ip_addr);
		port = atoi(pos + 1);
	} else {
		strncpy(cgw_ip_addr, hw_cgw_ip_addr, MAX_IP_LEN);
	}
	
	if (ipv4_addr_isValid(cgw_ip_addr) != 0) {
		printf("HW_CGW_IP_ADDR invalid\n");
		return;
	}
	
	inet_aton(cgw_ip_addr, &ia_val);
	apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ia_val);

	if (port > 0) {
		apmib_set(MIB_RTL_LINK_CGW_UDPPORT, (void *)&port);
	}
	
	return;
}
#endif

#endif

int up_mib_value()
{
        int old_ver=0;
        int new_ver=0;
        int i=0;
 
        apmib_get(MIB_MIB_VER, (void *)&old_ver);
#ifdef RTL_DEF_SETTING_IN_FW
		apmib_getDef(MIB_MIB_VER,(void *)&new_ver);
#else
        new_ver = atoi(update_mib[0].value);
#endif

        if(old_ver == new_ver)
        {
                return -1;
        }
        else
                printf("MIB Version update!\n");
		
		
 
#if defined(UPGRADE_BOOT_FROM_ROOT)
	 upgrade_boot();
#endif
#ifdef RTL_DEF_SETTING_IN_FW
		if(new_mib[0].id==0&&update_mib[1].id==0)
		{
			system("flash reset");
			return 0;
		}
#endif		
        i=0;
        while(new_mib[i].id != 0)
        {
        		apmib_set(new_mib[i].id,strToValue(new_mib[i].value,new_mib[i].type)); 
                //RunSystemCmd(NULL_FILE, "flash", "set", new_mib[i].name, new_mib[i].value, NULL_STR);
                i++;
        }
 
        i=0;
        while(update_mib[i].id != 0)
        {
        	apmib_set(update_mib[i].id,strToValue(update_mib[i].value,update_mib[i].type));        	
              //  RunSystemCmd(NULL_FILE, "flash", "set", update_mib[i].name, update_mib[i].value, NULL_STR);
                i++;
        }
 		apmib_update(CURRENT_SETTING);
        return 0;
 
}

#ifdef CONFIG_CMCC
int up_ssid_value()
{
    int new_ver=0;	

	apmib_get(MIB_SSID_VER, (void *)&new_ver);
	//printf("##### new_ver:%d\n", new_ver);
	if( new_ver ){
		return -1;
	}else{		
		return 0;
	}
}

#ifdef CUSTOMER_HW_SETTING_SUPPORT
int up_cust_hw_value(int wlan_idx)
{ 
	unsigned char ifname[10]={0};
	unsigned char hw_ssid[MAX_SSID_LEN] = {0}, def_ssid[MAX_SSID_LEN]={0}; 
	unsigned char hw_wpa_psk[MAX_PSK_LEN+1] = {0};	
	unsigned int ret = 0, flag_ssid = 0, flag_psk = 0;

	apmib_save_wlanIdx();
			
	if(wlan_idx==0){
		apmib_get(MIB_CUSTOMER_HW_WLAN0_SSID, (void *)&hw_ssid);
		apmib_get(MIB_CUSTOMER_HW_WLAN0_WPA_PSK, (void *)&hw_wpa_psk);	
	}else if(wlan_idx==1){
		apmib_get(MIB_CUSTOMER_HW_WLAN1_SSID, (void *)&hw_ssid);
		apmib_get(MIB_CUSTOMER_HW_WLAN1_WPA_PSK, (void *)&hw_wpa_psk);	
	}

	/* if CUSTOMER_HW_SSID not set, then use original method to generate SSID */
	if(strlen(hw_ssid)==0){
		printf("hw_ssid NULL\n");
		flag_ssid = 1;	
		ret = 1;
	}

	/* if CUSTOMER_HW_WPA_PSK not set, then use original method to generate WPA_PSK */
	if(strlen(hw_wpa_psk)==0){
		printf("hw_wpa_psk NULL\n");
		flag_psk = 1;	
		ret = 1;
	}

	if(flag_ssid ==0 || flag_psk==0){
		snprintf(ifname, sizeof(ifname), "wlan%d", wlan_idx);
		SetWlan_idx(ifname);

		if(flag_ssid==0){
			printf("ifname:%s, hw_ssid:%s\n", ifname, hw_ssid);
			apmib_set(MIB_WLAN_SSID,(void *)&hw_ssid);
		}

		if(flag_psk==0){
			printf("ifname:%s, hw_wpa_psk:%s\n",ifname,  hw_wpa_psk);		
			apmib_set(MIB_WLAN_WPA_PSK,(void *)&hw_wpa_psk);
		}	
	}	

	apmib_recov_wlanIdx();
	
	if(flag_ssid ==0 || flag_psk==0)
		apmib_update(CURRENT_SETTING);
	
	return ret;	
}
#endif
#endif

void set_log(void)
{
	int intValue=0,  intValue1=0;
	char tmpBuffer[32];
	char syslog_para[32];
	char localServer[32];
	
	apmib_get(MIB_SCRLOG_ENABLED, (void*)&intValue);
	if(intValue !=0 && intValue !=2 && intValue !=4 && intValue !=6 && intValue !=8 &&
		intValue !=10 && intValue !=12 && intValue !=14) {
			apmib_get(MIB_REMOTELOG_ENABLED, (void*)&intValue1);
			if(intValue1 != 0){
				apmib_get(MIB_REMOTELOG_SERVER,  (void *)tmpBuffer);
				if (memcmp(tmpBuffer, "\x0\x0\x0\x0", 4)){
#ifdef RINGLOG /* ring log */
					sprintf(localServer, "%s", inet_ntoa(*((struct in_addr *)tmpBuffer)));
					system("rm /var/log/log_split >/dev/null 2>&1");
					sprintf(tmpBuffer, "echo %d > /var/log/log_split", LOG_SPLIT);
					system(tmpBuffer);
					sprintf(tmpBuffer, "%d", MAX_LOG_SIZE);
					sprintf(syslog_para, "%d", LOG_SPLIT);
					fprintf(stderr, "syslog will use %dKB for log(%d rotate, 1 original, %dKB for each)\n",
						MAX_LOG_SIZE * (LOG_SPLIT+1), LOG_SPLIT, MAX_LOG_SIZE);
					RunSystemCmd(NULL_FILE, "syslogd", "-L","-R", localServer, "-s", tmpBuffer, "-b", syslog_para, NULL_STR);
#else
					sprintf(syslog_para, "%s", inet_ntoa(*((struct in_addr *)tmpBuffer)));
					RunSystemCmd(NULL_FILE, "syslogd", "-L", "-R", syslog_para, NULL_STR);
#endif					
				}
			}else{
			
//### add by sen_liu 2011.4.21 sync the system log update (enlarge from 1 pcs to 8 pcs) to	SDKv2.5 from kernel 2.4
#ifdef RINGLOG /* ring log */
				system("rm /var/log/log_split >/dev/null 2>&1");
				sprintf(tmpBuffer, "echo %d > /var/log/log_split", LOG_SPLIT);
				system(tmpBuffer);
				sprintf(tmpBuffer, "%d", MAX_LOG_SIZE);
				sprintf(syslog_para, "%d", LOG_SPLIT);
				fprintf(stderr, "syslog will use %dKB for log(%d rotate, 1 original, %dKB for each)\n",
					MAX_LOG_SIZE * (LOG_SPLIT+1), LOG_SPLIT, MAX_LOG_SIZE);
				RunSystemCmd(NULL_FILE, "syslogd", "-L", "-s", tmpBuffer, "-b", syslog_para, NULL_STR);
#else
//### end
				RunSystemCmd(NULL_FILE, "syslogd", "-L", NULL_STR);
#endif
			}
			RunSystemCmd(NULL_FILE, "klogd", NULL_STR);
		} 
		 
	
	return;
}

void start_wlanapp(int action)
{
	char tmpBuff[256];

#if defined(CONFIG_RTL_ULINKER_WLAN_DELAY_INIT)
	int ulinker_auto = 0;
	apmib_get(MIB_ULINKER_AUTO,  (void *)&ulinker_auto);
	if (ulinker_auto == 1) {
		if (isFileExist("/var/ulinker_init2")==0) {
			BDBG_FPRINT(stderr, "[%s:%d] break!\n", __FUNCTION__, __LINE__);
			return 0;
		}
		else {
			BDBG_FPRINT(stderr, "[%s:%d] exec\n", __FUNCTION__, __LINE__);
		}
	}
#endif

#if defined(CONFIG_RTL_ULINKER)
	/* notice ulinker_process to reset domain name query */
	system("echo 1 > /var/ulinker_reset_domain");
#endif

		//add vxd ifname to setWlan_Applications() arg list
		int opmode=0;
		apmib_get(MIB_OP_MODE,(void *)&opmode);
		if(opmode==WISP_MODE)
		{
			if(strstr(wan_interface,"-vxd"))
			{
				if(strlen(wlan_vxd_interface) != 0)
					strcat(wlan_vxd_interface," "); 
	
				strcat(wlan_vxd_interface,wan_interface);
			}			
		}

	memset(tmpBuff, 0x00, sizeof(tmpBuff));
	if(action==1){
		if(wlan_interface[0] && wlan_virtual_interface[0] && wlan_vxd_interface[0] && br_interface[0])
		sprintf(tmpBuff, "%s %s %s %s", wlan_interface, wlan_virtual_interface, wlan_vxd_interface, br_interface); 			
		else if(wlan_interface[0] && wlan_virtual_interface[0] && !wlan_vxd_interface[0] && br_interface[0])
			sprintf(tmpBuff, "%s %s %s", wlan_interface, wlan_virtual_interface, br_interface); 
		else if(wlan_interface[0] && !wlan_virtual_interface[0] && wlan_vxd_interface[0] && br_interface[0])
			sprintf(tmpBuff, "%s %s %s", wlan_interface, wlan_vxd_interface, br_interface); 	
		else if(wlan_interface[0] && !wlan_virtual_interface[0] && !wlan_vxd_interface[0] && br_interface[0])
			sprintf(tmpBuff, "%s %s", wlan_interface, br_interface); 						
	}else {
		//V_WLAN_APP_ENABLE=0 or para2=wlan_app
		if(wlan_interface[0] && wlan_vxd_interface[0] && br_interface[0])
		sprintf(tmpBuff, "%s %s %s", wlan_interface, wlan_vxd_interface, br_interface); 		
		else if(wlan_interface[0] && br_interface[0])
		sprintf(tmpBuff, "%s %s", wlan_interface, br_interface); 	
	}

	RunSystemCmd(PROC_GPIO, "echo", "I", NULL_STR);

//printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);

	if(tmpBuff[0])
		setWlan_Applications("start", tmpBuff);

	#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_APP_APPLE_MFI_WAC)
		system("rm -f  /var/system/start_init 2> /dev/null");
	#endif

}

void start_upnpd(int isgateway, int sys_op)
{
#ifdef   HOME_GATEWAY	
	int intValue=0,  intValue1=0;
	if(SetWlan_idx("wlan0")){
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&intValue);
	}
	if(isgateway==1 && sys_op !=1)
		apmib_get(MIB_UPNP_ENABLED, (void *)&intValue1);
	else 
		intValue1=0;
	if(intValue==0 && intValue1==0){//wps enabled and upnp igd is disabled
		RunSystemCmd(NULL_FILE, "mini_upnpd", "-wsc", "/tmp/wscd_config", "-daemon", NULL_STR);
		//system("mini_upnpd -wsc /tmp/wscd_config &");
	}else if(intValue==1 && intValue1==1){//wps is disabled, and upnp igd is enabled
		RunSystemCmd(NULL_FILE, "mini_upnpd", "-igd", "/tmp/igd_config", "-daemon", NULL_STR);
		//system("mini_upnpd -igd /tmp/igd_config &");
	}else if(intValue==0 && intValue1==1){//both wps and upnp igd are enabled
		RunSystemCmd(NULL_FILE, "mini_upnpd", "-wsc", "/tmp/wscd_config", "-igd", "/tmp/igd_config","-daemon",  NULL_STR);
		//system("mini_upnpd -wsc /tmp/wscd_config -igd /tmp/igd_config &");
	}else if(intValue==1 && intValue1==0){//both wps and upnp igd are disabled
		/*do nothing*/
	}
#endif		
}

#if defined(CONFIG_ANDLINK_IF5_SUPPORT) && defined(NEW_SCHEDULE_SUPPORT) && defined(TIMER_TASK_SUPPORT)
#define BIT(x)			(1<<x)
#define MAX_TASK_BITS 	7

void andlink_setup_tasktimer()
{
	char filename[50]={0}, line_buffer[100]={0};
	int entry_num=0, idx, newfile=1, date, start, end, nextline=0, i;
	WLAN_TIMER_TASK_ENTRY_T timer_task_entry;
	int cnt = 0, cmd_loop=0;	
	
	snprintf(filename, sizeof(filename), "%s", "/var/wlsch.conf");
	strcat(filename, ".task");
	
	apmib_get(MIB_TIMER_TASK_NUM, (void *)&entry_num);	
	for(idx=1; idx<=entry_num; ++idx){
		memset(&timer_task_entry, 0, sizeof(WLAN_TIMER_TASK_ENTRY_T));
		memset(line_buffer, 0, sizeof(line_buffer));
		
		*((char *)&timer_task_entry) = (char)idx;
		apmib_get(MIB_TIMER_TASK_TBL, (void *)&timer_task_entry);
		if(timer_task_entry.enable){
			if(timer_task_entry.enable==1)
				cmd_loop = 1;
			else if(timer_task_entry.enable==2)
				cmd_loop = 0;		
			
			//taskid, week, timeoffset, action, timeroffset2, index, command loop or not
			if(timer_task_entry.week){
				for(i=0; i<MAX_TASK_BITS; ++i){
					if(timer_task_entry.week & BIT(i)){
						nextline = 0;
						date = i; //0:Sun, 1:Mon, 2:Tue, 3:Wen, 4:Thu, 5:Fri, 6:Sat
						cnt++;

						//if timeoffset2>=86400, divided it into two entries
						start = timer_task_entry.timeoffset;
						if(timer_task_entry.timeoffset2 >= SECONDS_OF_A_DAY){
							nextline = 1;
							end = SECONDS_OF_A_DAY-1;
						}else{
							end = timer_task_entry.timeoffset2;
						}
						//entry idx, taskid, week, start, end, action, index, loop  
						sprintf(line_buffer,"%d,%d,%d,%d,%d,%s,%d,%d\n", 
								cnt, atoi(timer_task_entry.taskid), date, start, end, timer_task_entry.action, 
								timer_task_entry.index, cmd_loop);
						write_line_to_file(filename, (newfile==1?1:2), line_buffer);	
						newfile = 2;
						if(nextline){
							if(date <= 5)
								date++;
							else
								date = 0;
							cnt++;
							start = 0;
							end = timer_task_entry.timeoffset2 - SECONDS_OF_A_DAY;
							sprintf(line_buffer,"%d,%d,%d,%d,%d,%s,%d,%d\n", 
									cnt, atoi(timer_task_entry.taskid), date, start, end, timer_task_entry.action, 
									timer_task_entry.index, cmd_loop);
							write_line_to_file(filename, (newfile==1?1:2), line_buffer);	
						}
					}					
				}
			}			
		}		
	}

	return;
}
#endif

/*method to start reload is co-operate to parse rule of reload.c*/
void start_wlan_by_schedule(int index)
{
	int intValue=0,  intValue1=0, i=0, entryNum=0, bak_idx=0, bak_vidx=0;
	char tmp1[64]={0};
	SCHEDULE_T wlan_sched;
	int newfile=1;

	bak_idx=wlan_idx;
	wlan_idx=index;
	bak_idx=vwlan_idx;
	vwlan_idx=0;
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&intValue);
#if defined(NEW_SCHEDULE_SUPPORT)
	#define WLAN_SCHEDULE_FILE "/var/wlsch.conf"
	sprintf(tmp1,WLAN_SCHEDULE_FILE"%d",index);
	unlink(tmp1);
#endif	

	if(intValue==0){
		apmib_get(MIB_WLAN_SCHEDULE_ENABLED, (void *)&intValue1);
		apmib_get(MIB_WLAN_SCHEDULE_TBL_NUM, (void *)&entryNum);

		if(intValue1==1 && entryNum > 0){
			
			for (i=1; i<=entryNum; i++) {
				*((char *)&wlan_sched) = (char)i;
				apmib_get(MIB_WLAN_SCHEDULE_TBL, (void *)&wlan_sched);
#if defined(NEW_SCHEDULE_SUPPORT)

				char line_buffer[100]={0};
				if(wlan_sched.eco == 1 && !(wlan_sched.fTime == 0 && wlan_sched.tTime == 0))
				{
					sprintf(line_buffer,"%d,%d,%d,%d\n",i,wlan_sched.day,wlan_sched.fTime, wlan_sched.tTime);
					sprintf(tmp1,WLAN_SCHEDULE_FILE"%d",index);
					write_line_to_file(tmp1, (newfile==1?1:2), line_buffer);
					newfile = 2;
				}
#endif				
			}
			if(index == (NUM_WLAN_INTERFACE-1)){
#if defined(NEW_SCHEDULE_SUPPORT)
				sprintf(tmp1, "reload -k %s &", WLAN_SCHEDULE_FILE);
#else			
				sprintf(tmp1, "reload -e %d,%d,%d,%d,%s &", wlan_sched.eco, wlan_sched.fTime, wlan_sched.tTime, wlan_sched.day, wlan_sched.text);
#endif			
				system(tmp1);
			}
		}
		else{ /* do not care schedule*/

			if(index == (NUM_WLAN_INTERFACE-1)){
#if defined(NEW_SCHEDULE_SUPPORT)
				sprintf(tmp1, "reload -k %s &", WLAN_SCHEDULE_FILE);
				system(tmp1);
#else
				system("reload &");
#endif
			}
		}
	}
	else{
		/*wlan is disabled, we do not care wlan schedule*/
		if(index ==(NUM_WLAN_INTERFACE-1)){
#if defined(NEW_SCHEDULE_SUPPORT)
			sprintf(tmp1, "reload -k %s &", WLAN_SCHEDULE_FILE);
			system(tmp1);
#else
			system("reload &");
#endif
		}
	}

	vwlan_idx=bak_vidx;
	wlan_idx=bak_idx;
}
/* 
* dhcpc daemon pid file name must be /etc/udhcpc/udhcpc-*.pid
*/
int killDhcpcDaemons()
{
	DIR *dir;
	struct dirent *next;
	char dhcpPidPath[256]={"/etc/udhcpc/"};

	dir = opendir("/etc/udhcpc");
	if (!dir) 
	{
		printf("Cannot open /etc/udhcpc");
		return -1;
	}
	
	while ((next = readdir(dir)) != NULL)
	{
		if(strncmp("udhcpc-",next->d_name,strlen("udhcpc-"))==0
		&& strncmp(".pid",next->d_name+strlen(next->d_name)-strlen(".pid"),strlen(".pid"))==0)
		{//begin with "udhcpc-" end with ".pid" 
			strcpy(dhcpPidPath,"/etc/udhcpc/");
			strcat(dhcpPidPath,next->d_name);
			killDaemonByPidFile(dhcpPidPath);
		}
	}
	closedir(dir);
	return 0;
}
void clean_process(int sys_opmode,int wan_dhcp_mode,int gateway, int enable_wan, char *lanInterface, char *wlanInterface, char *wanInterface)
{
	char strPID[10], tmpBuff[200];
	int pid=-1;
	int i;
/*clean the process before take new setting*/	
#ifdef   HOME_GATEWAY
#ifdef CONFIG_POCKET_AP_SUPPORT
#else
	if(isFileExist(HW_NAT_FILE)){/*hw nat supported*/
		/*cleanup hardware tables*/
		if(sys_opmode==0)
			RunSystemCmd(HW_NAT_FILE, "echo", "1", NULL_STR);	/*gateway mode*/
		else if(sys_opmode==1)
			RunSystemCmd(HW_NAT_FILE, "echo", "2", NULL_STR);	/*bridge mode*/
		else if(sys_opmode==2)
			RunSystemCmd(HW_NAT_FILE, "echo", "3", NULL_STR);	/*wisp mode*/
		else if(sys_opmode==3)
			RunSystemCmd(HW_NAT_FILE, "echo", "4", NULL_STR);	/*bridge mode with multiple vlan*/
		else
			RunSystemCmd(HW_NAT_FILE, "echo", "5", NULL_STR); /*wisp mode with multiple vlan*/
		
	}else{/*software nat supported*/ 
	if(isFileExist(SOFTWARE_NAT_FILE)){	
		if(sys_opmode==0)
		{
#ifdef RTK_USB3G
			if(wan_dhcp_mode == USB3G)
      	RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "1", NULL_STR);
			else
#endif
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "0", NULL_STR);
		}
		if(sys_opmode==1)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "1", NULL_STR);
		if(sys_opmode==2)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "2", NULL_STR);
		if(sys_opmode==3)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "3", NULL_STR);
		if(sys_opmode==4)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "4", NULL_STR);
	}
		
	}
#endif	//CONFIG_POCKET_AP_SUPPORT
#endif	
	
		RunSystemCmd(NULL_FILE, "killall", "-15", "miniigd", NULL_STR);
		if(isFileExist(IGD_PID_FILE)){
			unlink(IGD_PID_FILE);
		}		
		RunSystemCmd(NULL_FILE, "killall", "-15", "routed", NULL_STR);
		if(isFileExist(RIP_PID_FILE)){
			unlink(RIP_PID_FILE);
		}	
#ifdef RIP6_SUPPORT
		RunSystemCmd(NULL_FILE, "killall", "-15", "bird6", NULL_STR);
#endif
		
#if defined(CONFIG_APP_TR069) // Move to webpage form handler when user modify setting
		/* Keep Tr069 alive unless 1.ACSURL changed 2.Turn off tr069 */
		if ((pid=find_pid_by_name("cwmpClient")) > 0)
		//if(isFileExist(TR069_PID_FILE))
		{									
			unsigned char acsUrltmp[CWMP_ACS_URL_LEN+1];
			int tr069Flag;
			
			apmib_get( MIB_CWMP_ACS_URL_OLD, (void *)acsUrltmp);
			apmib_get( MIB_CWMP_ACS_URL, (void *)acsURLStr);
			apmib_get( MIB_CWMP_FLAG, (void *)&tr069Flag);
			
			if(strcmp(acsUrltmp, acsURLStr) != 0 || ((tr069Flag & CWMP_FLAG_AUTORUN) == 0))
			{
			//pid=getPid_fromFile(TR069_PID_FILE);
			if(pid != -1){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-15", strPID, NULL_STR);
			}
			unlink(TR069_PID_FILE);
		}
		}
#endif //#if defined(CONFIG_APP_TR069)	
	
		//RunSystemCmd(NULL_FILE, "killall", "-9", "pptp.sh", NULL_STR);
		//RunSystemCmd(NULL_FILE, "killall", "-9", "pppoe.sh", NULL_STR);
		//RunSystemCmd(NULL_FILE, "killall", "-9", "l2tp.sh", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/etc/ppp/first", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/etc/ppp/firstpptp", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/etc/ppp/firstl2tp", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/etc/ppp/firstdemand", NULL_STR);
		//RunSystemCmd(NULL_FILE, "disconnect.sh", "all", NULL_STR);
		RunSystemCmd(NULL_FILE, "killall", "-9", "timelycheck", NULL_STR); //LZQ
		#ifdef CONFIG_AUTO_DHCP_CHECK
		RunSystemCmd(NULL_FILE, "killall", "-9", "Auto_DHCP_Check", NULL_STR);
		#endif

#if defined(CONFIG_APP_FWD)
		RunSystemCmd(NULL_FILE, "killall", "-9", "fwd", NULL_STR); //LZQ
#endif		
#ifdef   HOME_GATEWAY		
#ifdef MULTI_PPPOE
				wan_disconnect("all","");
#else
				wan_disconnect("all");
#endif
#endif			
	RunSystemCmd(NULL_FILE, "killall", "-9", "ntp_inet", NULL_STR);	
	RunSystemCmd(NULL_FILE, "killall", "-9", "ddns.sh", NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "syslogd", NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "klogd", NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "mini_upnpd", NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "reload", NULL_STR);
#if defined(CONFIG_APP_RTK_INBAND_CTL)
	RunSystemCmd(NULL_FILE, "killall", "-9", "hcd", NULL_STR);
#endif
	//RunSystemCmd(NULL_FILE, "killall", "-9", "ntfs-3g", NULL_STR);
	if(isFileExist(L2TPD_PID_FILE)){
			pid=getPid_fromFile(L2TPD_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(L2TPD_PID_FILE);
	}
	/* kill dhcp client */
	killDhcpcDaemons();
	sprintf(tmpBuff, "/etc/udhcpc/udhcpc-%s.pid", "br0");
	if(isFileExist(tmpBuff)){
		unlink(tmpBuff);
	}
#if 0
/*kill dhcp client if br interface is dhcp client*/	
	sprintf(tmpBuff, "/etc/udhcpc/udhcpc-%s.pid", lanInterface);
	if(isFileExist(tmpBuff)){
			pid=getPid_fromFile(tmpBuff);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(tmpBuff);
	}
	sprintf(tmpBuff, "/etc/udhcpc/udhcpc-%s.pid", wanInterface);
	if(isFileExist(tmpBuff)){
			pid=getPid_fromFile(tmpBuff);
			if(pid !=0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(tmpBuff);
	}
	
	#if defined(CONFIG_RTL_92D_SUPPORT)
	for(i=0;i<NUM_WLAN_INTERFACE;i++)
	{
		sprintf(tmpBuff, "/etc/udhcpc/udhcpc-wlan%d.pid",i);
		if(isFileExist(tmpBuff)){
				pid=getPid_fromFile(tmpBuff);
				if(pid != 0){
					sprintf(strPID, "%d", pid);
					RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
				}
				unlink(tmpBuff);
		}
	}
	#else
	if(wlanInterface[0]){
	sprintf(tmpBuff, "/etc/udhcpc/udhcpc-%s.pid", wlanInterface);
	if(isFileExist(tmpBuff)){
			pid=getPid_fromFile(tmpBuff);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				printf("%s:%d,tmpBuff is %s\n",__FUNCTION__,__LINE__,tmpBuff);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(tmpBuff);
	}
	}
	#endif
#endif
	
	if(isFileExist(DNRD_PID_FILE)){
			pid=getPid_fromFile(DNRD_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(DNRD_PID_FILE);
	}
	if(isFileExist(IGMPPROXY_PID_FILE)){
			pid=getPid_fromFile(IGMPPROXY_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(IGMPPROXY_PID_FILE);
			RunSystemCmd(PROC_BR_MCASTFASTFWD, "echo", "1,1", NULL_STR);
	}
	if(isFileExist(LLTD_PID_FILE)){
			pid=getPid_fromFile(LLTD_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(LLTD_PID_FILE);
	}
	if(isFileExist(DHCPD_PID_FILE)){
			pid=getPid_fromFile(DHCPD_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-16", strPID, NULL_STR);/*inform dhcp server write lease table to file*/
				sleep(1);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(DHCPD_PID_FILE);
	}
	
	
#ifdef CONFIG_IPV6
	if(isFileExist(DHCP6S_PID_FILE)) {
		pid=getPid_fromFile(DHCP6S_PID_FILE);
		if(pid){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);							
		}
		unlink(DHCP6S_PID_FILE);
	}
		
	if(isFileExist(DHCP6C_PID_FILE)) {
		pid=getPid_fromFile(DHCP6C_PID_FILE);
		if(pid){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-16", strPID, NULL_STR);/*inform dhcp server write lease table to file*/
			sleep(1);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);						
		}
		unlink(DHCP6C_PID_FILE);
	}
			
	if(isFileExist(DNSV6_PID_FILE)) {
		pid=getPid_fromFile(DNSV6_PID_FILE);
		if(pid){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);						
		}
		unlink(DNSV6_PID_FILE);
	}
		
	if(isFileExist(RADVD_PID_FILE)) {
		pid=getPid_fromFile(RADVD_PID_FILE);
		if(pid){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);						
		}
		unlink(RADVD_PID_FILE);
	}
		
	if(isFileExist(ECMH_PID_FILE)) {
		pid=getPid_fromFile(ECMH_PID_FILE);
		if(pid){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);						
		}
		unlink(ECMH_PID_FILE);
	}	

	/*MLD proxy*/
	if(isFileExist(MLDPROXY_PID_FILE)){
			pid=getPid_fromFile(MLDPROXY_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
			}
			unlink(MLDPROXY_PID_FILE);			
	}

	/* add for rm the configure file */
	RunSystemCmd(NULL_FILE, "rm", "-f", DHCP6S_CONF_FILE, NULL_STR);
	RunSystemCmd(NULL_FILE, "rm", "-f", DHCP6C_CONF_FILE, NULL_STR);
	RunSystemCmd(NULL_FILE, "rm", "-f", DNSV6_CONF_FILE, NULL_STR);
	RunSystemCmd(NULL_FILE, "rm", "-f", RADVD_CONF_FILE, NULL_STR);
	RunSystemCmd(NULL_FILE, "rm", "-f", DHCP6PD_CONF_FILE, NULL_STR);
	RunSystemCmd(NULL_FILE, "rm", "-f", DNSV6_ADDR_FILE, NULL_STR);
	/* end add */

#ifdef CONFIG_MAP_E_SUPPORT
	if(isFileExist(MAP_E_FILE))
	{
		RunSystemCmd(NULL_FILE, "ivictl", "-q", NULL_STR);
		RunSystemCmd(NULL_FILE, "rmmod", "/lib/modules/ivi.ko", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", MAP_E_FILE, NULL_STR);
	}
#endif

#ifdef CONFIG_DSLITE_SUPPORT
	if(isFileExist("/var/aftr.conf")){ // when kill dhcp6c, need remove aftr.conf
		unlink("/var/aftr.conf");
	}
#endif
#endif
/*end of clean the process before take new setting*/		
	
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
	/* Reset Firewall Rules*/
	system("iptables -F");
	system("iptables -F -t nat");
	system("iptables -F -t mangle");
	sprintf(tmpBuff, "iptables -A INPUT -i %s -j ACCEPT", lanInterface);
	system(tmpBuff);
	if(sys_opmode==1){
		system("iptables -P INPUT ACCEPT");
		system("iptables -P FORWARD ACCEPT");
	}
#endif	
	
#if defined(CONFIG_APP_SIMPLE_CONFIG)
	system("killall simple_config >/dev/null 2>&1");
#endif
#if defined(CONFIG_APP_APPLE_MFI_WAC)
	system("killall wfaudio");
	system("killall WACServer");
#if defined(CONFIG_APPLE_HOMEKIT)
	system("killall hapserver");
#endif
#endif
#if defined(CONFIG_CPU_UTILIZATION)
	if(isFileExist(CPU_PID_FILE)){
		system("killall cpu >/dev/null 2>&1");
		system("rm -f  "CPU_PID_FILE" 2> /dev/null");		
		system("rm -f  "CPU_LOAD_FILE" 2> /dev/null");
	}
#endif
#ifdef SHRINK_INIT_TIME
	cleanWlan_Applications(wlanInterface);
#endif
#ifdef CONFIG_APP_OPENVPN
	if(find_pid_by_name("openvpn")>0)
		system("killall openvpn > /dev/null 2>&1");
#endif
#if defined(CONFIG_APP_ZIGBEE)
	RunSystemCmd(NULL_FILE, "killall", "-9", "zigbeed", NULL_STR);
#endif

#if defined(CONFIG_APP_SAMBA)
	system("killall usbStorageAppController 2>/dev/null");
#endif

#ifdef CONFIG_APP_MINIDLNA
	RunSystemCmd(NULL_FILE, "killall", "-9", "minidlna",NULL_STR);	
#endif
}
#if defined(CONFIG_APP_USBMOUNT)
#define	PARTITION_FILE "/proc/partitions"

int get_blockDevPartition(char *str, char *partition)
{

	unsigned char tk[50];
	unsigned int i,j;
	unsigned int curCnt,preCnt;
	
	if(str==NULL)
	{
		return -1;
	}
	
	memset(tk,0, sizeof(tk));

	/*	partition table format:
		major minor  #blocks  name
	*/
	
	preCnt=0;
	curCnt=0;
	for (i=0;i<strlen(str);i++)
	{          
		if( (str[i]!=' ') && (str[i]!='\n') && (str[i]!='\r'))
		{
			if(preCnt==curCnt)
			{
				tk[curCnt]=i;
				curCnt++;
			}
		}
		else if((str[i]==' ') || (str[i]=='\n') ||(str[i]=='\r') )
		{
			preCnt=curCnt;
		}
	}
	
	/*to check device major number is 8*/
	
	if(!isdigit(str[tk[0]]))
	{
		return -1;
	}

	if(tk[1]==0)
	{
		return -1;
	}

	if(tk[1]<=tk[0])
	{
		return -1;
	}

	if((str[tk[0]]!='8') ||(str[tk[0]+1]!=' '))
	{
		return -1;
	}
	
	if(tk[3]==0)
	{
		return -1;
	}

	/*to get partition name*/
	j=0;
	for(i=tk[3]; i<strlen(str); i++)
	{
		
		if((str[i]==' ') || (str[i]=='\n') ||(str[i]=='\n'))
		{
			partition[j]='\0';
			return 0;
		}
		else
		{
			partition[j]=str[i];
			j++;
		}
			
	}
	return 0;
}
int Check_shouldMount(char *partition_name)
{
	DIR *dir=NULL;
	struct dirent *next;
	int found=0;
	dir = opendir("/tmp/usb");
	if (!dir) {
		printf("Cannot open %s", "/tmp/usb");
		return -1;
	}
	while ((next = readdir(dir)) != NULL) {
		//printf("Check_shouldMount:next->d_reclen=%d, next->d_name=%s\n",next->d_reclen, next->d_name);
			/* Must skip ".." */
			if (strcmp(next->d_name, "..") == 0)
				continue;
			if (strcmp(next->d_name, ".") == 0)
				continue;
			if (strcmp(next->d_name, "mnt_map") == 0)
				continue;
			if(!strcmp(next->d_name, partition_name)){
				found=1;
				break;
			}
	}
	closedir(dir);
	return found;
}

void autoMountOnBootUp(void)
{
	FILE *fp;
	
	int line=0;
	char buf[512];
	char partition[32];
	char usbMntCmd[64];
	int ret=-1;
	if(isFileExist(PARTITION_FILE)){
		fp= fopen(PARTITION_FILE, "r");
		if (!fp) {
	        	printf("can not  open /proc/partitions\n");
			return; 
	   	}

		while (fgets(buf, sizeof(buf), fp)) 
		{
			ret=get_blockDevPartition(buf, &partition);
			if(ret==0)
			{
				if(Check_shouldMount(partition)==0){
				sprintf(usbMntCmd, "DEVPATH=/sys/block/sda/%s ACTION=add usbmount block", partition);
				RunSystemCmd(NULL_FILE,  "echo", usbMntCmd, NULL_STR);
				system(usbMntCmd);
			}
			}
			
		}
	
		fclose(fp);
	}
	

}

void start_mount()
{
#if defined(HTTP_FILE_SERVER_SUPPORTED) || (defined(RTL_USB_IP_HOST_SPEEDUP) && !defined(CONFIG_RTL_8198C))
	RunSystemCmd("/proc/sys/vm/min_free_kbytes", "echo", "2048", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_max", "echo", "1048576", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_max", "echo", "1048576", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_rmem", "echo", "4096 108544 4194304", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_wmem", "echo", "4096 108544 4194304", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_moderate_rcvbuf", "echo", "0", NULL_STR);
#else
	/*config linux parameter for improving samba performance*/
	RunSystemCmd("/proc/sys/vm/min_free_kbytes", "echo", "1024", NULL_STR);
	
	RunSystemCmd("/proc/sys/net/core/netdev_max_backlog", "echo", "8192", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/optmem_max", "echo", "131072", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_default", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_max", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_default", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_max", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_rmem", "echo", "131072 262144 393216", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_wmem", "echo", "131072 262144 393216", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_mem", "echo", "768 1024 1380", NULL_STR);
#endif
	/*config hot plug and auto-mount*/
	RunSystemCmd("/proc/sys/kernel/hotplug", "echo", "/usr/hotplug", NULL_STR);
	RunSystemCmd(NULL_FILE, "mkdir", "-p /tmp/usb/", NULL_STR);

	/*force kernel to write data to disk, don't cache in memory for a long time*/
	RunSystemCmd("/proc/sys/vm/vfs_cache_pressure", "echo", "10000", NULL_STR);
	RunSystemCmd("/proc/sys/vm/dirty_background_ratio", "echo", "5", NULL_STR);
	RunSystemCmd("/proc/sys/vm/dirty_writeback_centisecs", "echo", "100", NULL_STR);
	/*automatically mount partions listed in /proc/partitions*/
	autoMountOnBootUp();
}
#endif
#if defined(CONFIG_RTL_HIGH_PERFORMANCE_FILESYSTEM)
void load_rtl_fs_module(void)
{
	RunSystemCmd(NULL_FILE, "insmod", "/lib/modules/jnl.ko", NULL_STR);
	RunSystemCmd(NULL_FILE, "insmod", "/lib/modules/ufsd.ko", NULL_STR);
}
#endif

#if defined(CONFIG_APP_VSFTPD)
void start_vsftpd()
{
	RunSystemCmd(NULL_FILE, "echo","start vsftpd", NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/vsftpd.conf", "/var/config/vsftpd.conf",  NULL_STR);
	system("vsftpd /var/config/vsftpd.conf &");
}
#endif
#ifdef MULTI_PPPOE
void setInitMultiPPPoE()
{
	/*
		-----------------------Multi PPPoE--------------------------
		when something would lock global resource, should clear here,		
	*/
	
	system("rm /etc/ppp/firewall_lock >/dev/null 2>&1");
	system("echo 1 > /etc/ppp/firewall_lock ");
//	RunSystemCmd("tmp/firewall_lock", "echo", "1", NULL_STR);	
	//remove("/etc/ppp/hasPppoedevice");
	//remove("/etc/ppp/ppp_order_info");
	/*
	if((flushF = fopen("/etc/ppp/flushCmds","r+")) != NULL)
	{
		while(!feof(flushF))
		{
			// get the line and execute it , just for clear the ip policy rules
			fgets(cmd,sizeof(cmd),flushF);		
			system(cmd);
		}
		// delete flushCmds file
		unlink("/etc/ppp/flushCmds");
	}		
	*/
}

#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT) 
int init_EthDot1x(int wan_mode, int sys_op, char *wan_iface, char *lan_iface)
{
	int val, dot1xenable, dot1xmode, proxy_port_mask = 0 , client_port_mask = 0, maxport = 0, i = 0;
	int type, unicast_enable, server_port;
	ETHDOT1X_T entry;
	unsigned char cmdBuffer[100];
	
	RunSystemCmd(NULL_FILE, "killall", "-9", "auth_eth", NULL_STR);	
	
	apmib_get( MIB_ELAN_ENABLE_1X, (void *)&dot1xenable);
	apmib_get( MIB_ELAN_DOT1X_MODE, (void *)&dot1xmode);
	apmib_get( MIB_ELAN_DOT1X_PROXY_TYPE, (void *)&type);
	apmib_get( MIB_ELAN_EAPOL_UNICAST_ENABLED, (void *)&unicast_enable);
	apmib_get( MIB_ELAN_DOT1X_SERVER_PORT, (void *)&server_port);
	if (dot1xenable)
	{
		//client mode enable check in flash.c
		//if (dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)
		{
			maxport =  MAX_ELAN_DOT1X_PORTNUM - 1;
			if (sys_op ==BRIDGE_MODE || sys_op == WISP_MODE)
			{
				#if !defined(CONFIG_RTL_IVL_SUPPORT)
				maxport =  MAX_ELAN_DOT1X_PORTNUM;
				#endif
			}
			if ((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_SNOOPING_MODE_BIT))//snooping mode enable
			{
				sprintf(cmdBuffer,"echo \"1 1\" > /proc/802dot1x/enable");
				system(cmdBuffer);
				sprintf(cmdBuffer,"echo \"%d\" > /proc/802dot1x/server_port", server_port);
				system(cmdBuffer);
			}
			else if ((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_PROXY_MODE_BIT))//proxy mode enable
			{
				
				sprintf(cmdBuffer,"echo \"1 %d\" > /proc/802dot1x/enable", unicast_enable);
				system(cmdBuffer);
			}			
			else if ((dot1xenable & ETH_DOT1X_CLIENT_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_CLIENT_MODE_BIT))//client mode enable
			{
				sprintf(cmdBuffer,"echo \"1 1\" > /proc/802dot1x/enable");
				system(cmdBuffer);
			}
			
			sprintf(cmdBuffer,"echo \"%d\" > /proc/802dot1x/type", type);
			system(cmdBuffer);
			
			for(i=1; i<=maxport ; i++)
			{
				memset(&entry, '\0', sizeof(entry));
				//	printf("--%s(%d)--i is %d\n", __FUNCTION__, __LINE__, i);

				*((char *)&entry) = (char)i;
				apmib_get(MIB_ELAN_DOT1X_TBL, (void *)&entry);
				if (!entry.enabled)
					continue;
				
				printf("%s %d entry.enabled=%d entry.portnum=%d\n", __FUNCTION__, __LINE__, entry.enabled, entry.portnum);
				proxy_port_mask |= (1<<entry.portnum);
				if (dot1xmode & ETH_DOT1X_SNOOPING_MODE_BIT)
				{
					sprintf(cmdBuffer,"echo \"1 %d 1 1\" > /proc/802dot1x/mode", entry.portnum);
				}
				else if (dot1xmode & ETH_DOT1X_PROXY_MODE_BIT)
				{
					if (unicast_enable)
						sprintf(cmdBuffer,"echo \"1 %d 2 1\" > /proc/802dot1x/mode", entry.portnum);
					else
						sprintf(cmdBuffer,"echo \"1 %d 2 0\" > /proc/802dot1x/mode", entry.portnum);
				}
				system(cmdBuffer);
				
			}
			if (((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_PROXY_MODE_BIT))||
				((dot1xenable & ETH_DOT1X_CLIENT_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_CLIENT_MODE_BIT)))
				{
					val = 1;
					apmib_set( MIB_ELAN_MAC_AUTH_ENABLED, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_ACCOUNT_RS_MAXRETRY, (void *)&val);
					val = 0;
					apmib_set( MIB_ELAN_RS_REAUTH_TO, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_RS_MAXRETRY, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_RS_INTERVAL_TIME, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_ACCOUNT_RS_MAXRETRY, (void *)&val);

					if ((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
						(dot1xmode & ETH_DOT1X_PROXY_MODE_BIT))
					{
						apmib_set( MIB_ELAN_DOT1X_PROXY_MODE_PORT_MASK, (void *)&proxy_port_mask);
					}
					
					if ((dot1xenable & ETH_DOT1X_CLIENT_MODE_ENABLE_BIT)&&
						(dot1xmode & ETH_DOT1X_CLIENT_MODE_BIT))
					{
						client_port_mask = 1<<ETH_DOT1X_CLIENT_PORT;//default .....
						apmib_set( MIB_ELAN_DOT1X_CLIENT_MODE_PORT_MASK, (void *)&client_port_mask);
						sprintf(cmdBuffer,"echo \"1 %d 3 0\" > /proc/802dot1x/mode", ETH_DOT1X_CLIENT_PORT);
						system(cmdBuffer);
						sprintf(cmdBuffer,"rsCert -rd");
						system(cmdBuffer);
					}
					sprintf(cmdBuffer,"flash ethdot1x /var/1x/eth_1x.conf");
					system(cmdBuffer);
					sprintf(cmdBuffer,"auth_eth eth0 br0 eth /var/1x/eth_1x.conf");
					system(cmdBuffer);
				}
			
		}
		
	}

	return 0;
	
}
#endif

#ifdef RTK_CAPWAP
#define CAPWAP_APP_VAR_DIR "/var/capwap"
#define CAPWAP_APP_ETC_DIR "/etc/capwap"
// capwap_config_changed(): check whether the string in var_file equals to var_str.
int capwap_config_changed(const char *var_str, const char *var_file)
{
	char *old_var_str = NULL;
	FILE *fp = NULL;
	size_t bufsize, bufread;
	char filepath[64];

	if(!isFileExist(CAPWAP_APP_VAR_DIR)) return 1;
	
    sprintf(filepath, "%s/%s", CAPWAP_APP_VAR_DIR, var_file);
	fp = fopen(filepath, "r");
	if (fp == NULL) return 1;
	if (fseek(fp, 0L, SEEK_END) != 0) {
		fclose(fp);
		return 1;
	}
	bufsize = ftell(fp);
	if (bufsize <= 0) {
		fclose(fp);
		return 1;
	}
	if (fseek(fp, 0L, SEEK_SET) != 0) {
		fclose(fp);
		return 1;
	}

	old_var_str = malloc(sizeof(char) * (bufsize + 1));
	old_var_str[0] = '\0';

	bufread = fread(old_var_str, sizeof(char), bufsize, fp);
    if (bufread != bufsize) {
            printf("bufread != bufsize, %u, %u, %s\n", bufread, bufsize, old_var_str);
            fclose(fp);
            return 1;
	}
	fclose(fp);

	if(strcmp(old_var_str, var_str)!=0) {
		free(old_var_str);
		return 1;
	} else {
		free(old_var_str);
		return 0;
	}

}
// capwap_set_config_to_file(): set the string in var_file as var_str.
/*
static void capwap_set_config_to_file(const char *var_str, const char *var_file)
{
    FILE *fp = NULL;
    size_t lengthToWrite, lengthWritten;
    char filepath[64];
	
	if(!isFileExist(CAPWAP_APP_VAR_DIR)) {		
		system("mkdir "CAPWAP_APP_VAR_DIR);
	}
		
    sprintf(filepath, "%s/%s", CAPWAP_APP_VAR_DIR, var_file);

    fp = fopen(filepath, "w"); // if file exists, clear content.
    if (fp == NULL) return;

    lengthToWrite = strlen(var_str)+1;
    lengthWritten = fwrite(var_str, sizeof(char), lengthToWrite, fp);

    if (lengthToWrite != lengthWritten)
            printf("lengthToWrite, lengthWritten, %u, %u, %s\n", lengthToWrite, lengthWritten, var_str);

    fclose(fp);
}

void capwap_app()
{	
	int capwapMode;
	apmib_get(MIB_CAPWAP_MODE, &capwapMode);

	// for wtp
	printf("babylon test MIB_CAPWAP_MODE=%d, CAPWAP_WTP_ENABLE=%d, CAPWAP_AC_ENABLE=%d\n", capwapMode, CAPWAP_WTP_ENABLE, CAPWAP_AC_ENABLE);
	if (capwapMode & CAPWAP_WTP_ENABLE) {
		char ac_ip_str[16], wtp_id_str[8];		
		int restart_flag;
		unsigned char tmp_ip[4];
		int tmp_int_val;
		
		// get config from flash
		apmib_get(MIB_CAPWAP_AC_IP, tmp_ip);
		sprintf(ac_ip_str, "%d.%d.%d.%d", tmp_ip[0], tmp_ip[1], tmp_ip[2], tmp_ip[3]);
		apmib_get(MIB_CAPWAP_WTP_ID, &tmp_int_val);
                sprintf(wtp_id_str, "%d", tmp_int_val);

		// get original config & compare
		restart_flag = 0;
		if (capwap_config_changed("wtp_started", "wtp_status")) {			
			restart_flag = 1;
		}
		
		if (capwap_config_changed(ac_ip_str, "ac_ip")) {			
			restart_flag = 1;
		}

		if (capwap_config_changed(wtp_id_str, "wtp_id")) {			
			restart_flag = 1;
		}

		// restart wtp
                //if (restart_flag) {
                        system("killall -9 WTP >/dev/null 2>&1");
			capwap_set_config_to_file("wtp_started", "wtp_status");
			capwap_set_config_to_file(ac_ip_str, "ac_ip");
			capwap_set_config_to_file(wtp_id_str, "wtp_id");
			sleep(2);
			system("WTP "CAPWAP_APP_ETC_DIR);
                        printf("WTP started or restarted\n");
                //}
		
	} else {
                system("killall -9 WTP >/dev/null 2>&1");
		capwap_set_config_to_file("wtp_disabled", "wtp_status");
	}

	// for AC
	if (capwapMode & CAPWAP_AC_ENABLE) {		
		int restart_flag = 0;		
		if (capwap_config_changed("ac_started", "ac_status")) {			
			restart_flag = 1;
		}
                //if (restart_flag) {
                        system("killall -9 AC >/dev/null 2>&1");
			capwap_set_config_to_file("ac_started", "ac_status");
			sleep(2);
			system("AC "CAPWAP_APP_ETC_DIR);
                        printf("AC started or restarted\n");

                //}
	} else {
                system("killall -9 AC >/dev/null 2>&1");
		capwap_set_config_to_file("ac_disabled", "ac_status");
	}
	
}
*/
void capwap_app()
{
	if(find_pid_by_name("WTP")>0)
		system("killall -9 WTP >/dev/null 2>&1");
	if(find_pid_by_name("AC")>0)
		system("killall -9 AC >/dev/null 2>&1");
	if(find_pid_by_name("AACWTP")>0)
		system("killall -9 AACWTP >/dev/null 2>&1");

	//disable daemon when both interfaces disabled
	extern int wlan_idx;
	extern int vwlan_idx;
	int wlan0_disabled, wlan1_disabled;
	wlan_idx=0; vwlan_idx=0;
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan0_disabled);
	wlan_idx=1; vwlan_idx=0;
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan1_disabled);
	int wlan_disabled = wlan0_disabled && wlan1_disabled;

#ifdef CONFIG_ANDLINK_SUPPORT
    int qos, roaming_switch, enable;
    apmib_get(MIB_RTL_LINK_ENABLE, (void *)&enable);
    apmib_get(MIB_RTL_LINK_ROAMING_SWITCH, (void *)&roaming_switch);
    apmib_get(MIB_RTL_LINK_ROAMING_QOS, (void *)&qos);
    if(enable && roaming_switch && qos)
        return;
#endif
	int capwapMode;
	apmib_get(MIB_CAPWAP_MODE, (void *)&capwapMode);
	printf("[%s]: mode=%d wlan_disabled=%d, ", __FUNCTION__, capwapMode, wlan_disabled);

	if(capwapMode&CAPWAP_AUTO_CONFIG_ENABLE
		|| (capwapMode&CAPWAP_ROAMING_ENABLE && !wlan_disabled)
		|| (capwapMode&CAPWAP_11V_ENABLE && !wlan_disabled))
	{	
		printf("capwap is running\n");
		system("WTP "CAPWAP_APP_ETC_DIR);
//		system("AC "CAPWAP_APP_ETC_DIR);
//		system("AACWTP " CAPWAP_APP_ETC_DIR);
	}
	else
		printf("capwap is disabled\n");
	
}
#endif

#if defined(CONFIG_SYSFS)
static void rtl_mount_sysfs(void)
{
	system("mount -t sysfs sysfs /sys");
}
#endif

#if defined(CONFIG_RPS)
static void rtl_configRps(void)
{
	system("echo 2 > /sys/class/net/eth0/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth1/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth2/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth3/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth4/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/wlan0/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/wlan1/queues/rx-0/rps_cpus");
	system("echo 4096 > /sys/class/net/eth0/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth1/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth2/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth3/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth4/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /proc/sys/net/core/rps_sock_flow_entries");
}
#endif
#ifdef CONFIG_RTL_DNS_TRAP
#define HOSTS_FILE "/var/hosts"
#define DNS_PROC_FILE "/proc/rtl_dnstrap/domain_name"
static	void strtolower(char *str, int len)
{
	int i;
	for (i = 0; i<len; i++) {
		str[i] = tolower(str[i]);
	}
}
int create_hosts_file(char* domain_name)
{
	FILE* fp;
	fp = fopen(HOSTS_FILE,"w+");
	if(fp == NULL)
	{
		printf("Failed to open hosts file!!!!\n");
		return -1;
	}
	fprintf(fp,"# Do not remove the following line, or various programs\n");
	fprintf(fp,"# that require network functionality will fail.\n");
	fprintf(fp,"127.0.0.1   localhost.localdomain   localhost   rlx-linux\n");
	fprintf(fp,"127.0.0.1   %s\n",domain_name);
	fclose(fp);
	return 0;
}
#endif
#if defined(CONFIG_APP_USBMOUNT)
static void rtl_config_read_ahead(void)
{
	FILE *fp;
	
	char buf[512];
	char partition[32];
	char usbMntCmd[64];
	int ret=-1;
	fp= fopen(PARTITION_FILE, "r");
	if (!fp) {
			printf("can not  open /proc/partitions\n");
		return; 
	}

	while (fgets(buf, sizeof(buf), fp)) 
	{
		ret=get_blockDevPartition(buf, &partition);
		if(ret==0 && Check_shouldMount(partition)==0 && strlen(partition) == 3)
		{
			sprintf(usbMntCmd, "echo 4096 > /sys/block/%s/queue/read_ahead_kb", partition);
			system(usbMntCmd);
		}
	}

	fclose(fp);
}
#endif



#if defined(CONFIG_APP_SAMBA)
void cp_samba_conf()
{
	//RunSystemCmd(NULL_FILE,  "echo", "start samba", NULL_STR);
	RunSystemCmd(NULL_FILE,  "mkdir", "/var/samba", NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/smb.conf", "/var/samba/smb.conf",  NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/smbpasswd", "/var/samba/smbpasswd",	NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/upcase.dat", "/var/lib/",  NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/lowcase.dat", "/var/lib/",  NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/valid.dat", "/var/lib/",  NULL_STR);
//	RunSystemCmd("/var/group",	"echo", " ",  NULL_STR);
//	RunSystemCmd(NULL_FILE,  "cp", "/etc/group", "/var/group",	NULL_STR);
}

#ifdef SAMBA_WEB_SUPPORT
void add_samba_group_user(){
	int i, number;
	char cmdBuffer[100];
	STORAGE_USER_T	user_info;
	STORAGE_GROUP_T group_info;
#if 0
	apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		memset(&group_info,'\0',sizeof(STORAGE_GROUP_T));
		*((char*)&group_info) = (char)(i+1);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&group_info);

		memset(cmdBuffer,'\0',100);
		snprintf(cmdBuffer,100,"addgroup %s",group_info.storage_group_name);
		system(cmdBuffer);
	}
#endif
	apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		memset(&user_info,'\0',sizeof(STORAGE_USER_T));
		*((char*)&user_info) = (char)(i+1);
		apmib_get(MIB_STORAGE_USER_TBL,(void*)&user_info);

		memset(cmdBuffer,'\0',100);
#if 0
		if(!strcmp(user_info.storage_user_group,"--")){
			snprintf(cmdBuffer,100,"adduser %s",user_info.storage_user_name);
			system(cmdBuffer);
		}else{
			snprintf(cmdBuffer,100,"adduser -G %s %s",user_info.storage_user_group,user_info.storage_user_name);
			system(cmdBuffer);
		}
		
#else
		snprintf(cmdBuffer,100,"adduser %s",user_info.storage_user_name);
		system(cmdBuffer);
#endif
		
		memset(cmdBuffer,'\0',100);
#ifndef CONFIG_APP_SAMBA_3_6_24
		snprintf(cmdBuffer,100,"smbpasswd %s %s",user_info.storage_user_name,user_info.storage_user_password);
#else
		snprintf(cmdBuffer,100,"echo -e \"%s\\n%s\" | smbpasswd -a %s",user_info.storage_user_password,user_info.storage_user_password,user_info.storage_user_name);
#endif
		system(cmdBuffer);
	}
}

#endif

void start_samba()
{
	int samba_enabled, anon_access = 0;
	char cmdBuffer[100];
	
	system("killall usbStorageAppController 2>/dev/null");
	if(!apmib_get(MIB_SAMBA_ENABLED, (void*)&samba_enabled) || !samba_enabled)
		return;
	
	cp_samba_conf();

#ifdef SAMBA_WEB_SUPPORT	
	add_samba_group_user();
	apmib_get(MIB_STORAGE_ANON_ENABLE,(void*)&anon_access);
#endif
	
	snprintf(cmdBuffer,100,"usbStorageAppController -a %d &",anon_access);
	system(cmdBuffer);
	return;
}

#endif

#ifdef CONFIG_APP_MINIDLNA
#define MINIDLNA_CONF_FILE "/var/minidlna.conf"

void setMinidlnaSharedFolder(){
	int i, num;
	char tmp[10] = {0};
	char buf[100] = {0};
	MINIDLNA_SHAREFOLDER_T entry;
	if(!apmib_get(MIB_MINIDLNA_SHAREDFOLDER_TBL_NUM, (void *)&num))
		return;
	
	if(num){
		for(i = 1; i <= num; i++){
			*((char *)&entry) = (char)i;
		if(!apmib_get(MIB_MINIDLNA_SHAREDFOLDER_TBL, (void *)&entry))
			return;
		if(strlen(entry.minidlna_sharefolder_name) == 0)
			return;
		
		bzero(buf, sizeof(buf));
		bzero(tmp, sizeof(tmp));
		snprintf(buf, sizeof(buf), "%s", "media_dir=");
		if(strcmp("audio", entry.minidlna_sharedfolder_type) == 0) 
			snprintf(tmp, sizeof(tmp), "%s", "A,");
		else if(strcmp("video", entry.minidlna_sharedfolder_type) == 0) 
			snprintf(tmp, sizeof(tmp), "%s", "V,");
		else if(strcmp("images", entry.minidlna_sharedfolder_type) == 0) 
			snprintf(tmp, sizeof(tmp), "%s", "P,");
		if(strlen(tmp))
			strcat(buf,tmp);
		
		strcat(buf, entry.minidlna_sharefolder_name);

		//TODO
		//write buf to the /var/minidlna.conf	at the original media_dir=/tmp/usb/sda6/Media location
		}
	}
	
}


void clean_lan_minidlna()
{	
	RunSystemCmd(NULL_FILE, "killall", "-9", "minidlna",NULL_STR);	
}

void start_lan_minidlna()
{
	int enabled;

	clean_lan_minidlna();	
	if(!apmib_get(MIB_MINIDLNA_ENABLED, (void *)&enabled) || !enabled)
		return;
	
	system("cp /etc/minidlna.conf /var/minidlna.conf");

	//TODO : write share folder to /var/minidlna.conf	
	setMinidlnaSharedFolder();
	
	system("minidlna -R -f /var/minidlna.conf");	
}
#endif

#ifdef CONFIG_APP_OPENVPN
#define OPENVPN_SERVER_CONFIG_FILE "/var/openvpn/server.ovpn"
#define OPENVPN_CLIENT_CONFIG_FILE "/var/openvpn/client.ovpn"

int getWanIpAddr(char *wanIp)
{
	DHCP_T dhcp;
	OPMODE_T opmode=-1;
	unsigned int wispWanId=0;
	char wan_ifname[16]={0};
	struct in_addr	 intaddr;

	apmib_get(MIB_WAN_DHCP, (void *)&dhcp);
	apmib_get(MIB_OP_MODE, (void *)&opmode);
	apmib_get(MIB_WISP_WAN_ID, (void *)&wispWanId);
	
	if( dhcp == PPPOE || dhcp == PPTP || dhcp == L2TP)
		strcpy(wan_ifname, "ppp0");
	else 
	{
		if(opmode == WISP_MODE)
		{
			if(wispWanId==0)
				strcpy(wan_ifname, "wlan0");
			else if(wispWanId==1)
				strcpy(wan_ifname, "wlan1");

			#ifdef CONFIG_SMART_REPEATER
				int rptEnabled=0, wlanMode=0;
				if(wispWanId == 0)
					apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
				else if(wispWanId == 1)
					apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);

				if(rptEnabled)
				{		
					SetWlan_idx(wan_ifname);
					apmib_get(MIB_WLAN_MODE, (void *)&wlanMode);
					if(AP_MODE==wlanMode || AP_MESH_MODE==wlanMode || MESH_MODE==wlanMode || AP_WDS_MODE==wlanMode)
					{
						if(wispWanId == 0)
							strcpy(wan_ifname, "wlan0-vxd");
						else if(wispWanId == 1)
							strcpy(wan_ifname, "wlan1-vxd");
					}
					apmib_recov_wlanIdx();
				}
			#endif
		}
		else
			strcpy(wan_ifname, "eth1");
	}

	if(getInAddr(wan_ifname, IP_ADDR_T, (void *)&intaddr))
	{
		if(wanIp)
			strcpy(wanIp, inet_ntoa(intaddr));
		
		return 1;
	}

	return 0;	
}

void create_openvpn_server_configfile()
{
	int port_num;
	int auth_type=0, auth_none=0;
	char line_buffer[256]={0};	

	FILE *fp;
	char dns_ip[24]={0};
	
	apmib_get(MIB_OPENVPN_AUTH_TYPE, (void *)&auth_type);
	apmib_get(MIB_OPENVPN_AUTH_NONE, (void *)&auth_none);
	apmib_get(MIB_OPENVPN_PORT, (void *)&port_num);	
	if(!(port_num>0 && port_num<65536))
		port_num=1194;

	sprintf(line_buffer, "port %d\n", port_num);
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 1, line_buffer);

	sprintf(line_buffer, "%s\n", "proto udp");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "dev tun");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "tun-mtu 1314\n");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "cipher AES-256-CBC");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
	if(auth_none)
	{
		sprintf(line_buffer, "%s\n", "auth none");
		write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
	}

	sprintf(line_buffer, "%s\n", "server 10.8.0.0 255.255.255.0");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);	

	if(isFileExist("/etc/resolv.conf"))
	{
		if((fp=fopen("/etc/resolv.conf", "r"))!=NULL)		
		{
			while(fgets(line_buffer,sizeof(line_buffer),fp) !=NULL)
			{
				if(strstr(line_buffer, "nameserver")==NULL)
					continue;
				
				sscanf(line_buffer, "nameserver %s", dns_ip);
				sprintf(line_buffer, "push \"dhcp-option DNS %s\"\n", dns_ip);
				write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
			}
			fclose(fp);
		}
	}	

	sprintf(line_buffer, "%s\n", "ifconfig-pool-persist /var/openvpn/openvpn-ipp.txt");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "push \"redirect-gateway\"");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "push \"route 0.0.0.0 0.0.0.0\"");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "ca /var/openvpn/ca.crt");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "dh /var/openvpn/dh1024.pem");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "cert /var/openvpn/server.crt");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "key /var/openvpn/server.key");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "duplicate-cn");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);	

	if(auth_type==NAME_PWD)
	{	
		sprintf(line_buffer, "%s\n", "client-cert-not-required");
		write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
	}
	
	if(auth_type==NAME_PWD || auth_type==PWD_CERTS)
	{
		sprintf(line_buffer, "%s\n", "auth-user-pass-verify /var/openvpn/checkpsw.sh via-env");
		write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
		
		sprintf(line_buffer, "%s\n", "username-as-common-name");
		write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
	}
	
	sprintf(line_buffer, "%s\n", "script-security 3 system");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "status /var/openvpn/openvpn-status.log");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);	

	sprintf(line_buffer, "%s\n", "log /var/openvpn/openvpn.log");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "keepalive 10 60");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "comp-lzo no");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "max-clients 16");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "persist-key");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "persist-tun");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);

#if 0
	sprintf(line_buffer, "%s\n", "no-replay");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
#endif

	sprintf(line_buffer, "%s\n", "verb 3");
	write_line_to_file(OPENVPN_SERVER_CONFIG_FILE, 2, line_buffer);
}

void create_openvpn_client_configfile()
{
	int port_num;
	int auth_type=0, auth_none=0;
	char wanIp[16]={0};
	char line_buffer[256]={0};	
	
	apmib_get(MIB_OPENVPN_AUTH_TYPE, (void *)&auth_type);
	apmib_get(MIB_OPENVPN_AUTH_NONE, (void *)&auth_none);
	apmib_get(MIB_OPENVPN_PORT, (void *)&port_num);	
	if(!(port_num>0 && port_num<65536))
		port_num=1194;

	sprintf(line_buffer, "%s\n", "client");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 1, line_buffer);
	
	getWanIpAddr(wanIp);
	sprintf(line_buffer, "remote %s %d\n", wanIp, port_num);
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "dev tun");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "proto udp");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "cipher AES-256-CBC");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
	if(auth_none)
	{
		sprintf(line_buffer, "%s\n", "auth none");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
	}

	sprintf(line_buffer, "%s\n", "<ca>");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
	sprintf(line_buffer, "cat /var/openvpn/ca.crt >> %s\n", OPENVPN_CLIENT_CONFIG_FILE);
	system(line_buffer);
	sprintf(line_buffer, "%s\n", "</ca>");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	if(auth_type==CERTS || auth_type==PWD_CERTS)
	{
		sprintf(line_buffer, "%s\n", "ns-cert-type server");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

		sprintf(line_buffer, "%s\n", "<cert>");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
		sprintf(line_buffer, "cat /var/openvpn/client.crt >> %s\n", OPENVPN_CLIENT_CONFIG_FILE);
		system(line_buffer);
		sprintf(line_buffer, "%s\n", "</cert>");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

		sprintf(line_buffer, "%s\n", "<key>");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
		sprintf(line_buffer, "cat /var/openvpn/client.key >> %s\n", OPENVPN_CLIENT_CONFIG_FILE);
		system(line_buffer);
		sprintf(line_buffer, "%s\n", "</key>");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
	}

	if(auth_type==NAME_PWD || auth_type==PWD_CERTS)
	{
		sprintf(line_buffer, "%s\n", "auth-user-pass");
		write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
	}	

	sprintf(line_buffer, "%s\n", "comp-lzo no");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
	
	sprintf(line_buffer, "%s\n", "reneg-sec 0");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "ping 10");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "ping-restart 60");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "persist-key");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "persist-tun");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

#if 0
	sprintf(line_buffer, "%s\n", "no-replay");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
#else
	sprintf(line_buffer, "%s\n", "replay-window 65536");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);
#endif

	sprintf(line_buffer, "%s\n", "resolv-retry infinite");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "nobind");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);

	sprintf(line_buffer, "%s\n", "verb 3");
	write_line_to_file(OPENVPN_CLIENT_CONFIG_FILE, 2, line_buffer);	
}

void start_openvpn()
{
	char line_buffer[256]={0};	
	int openvpn_enabled=0;
	int port_num;
	char cmdBuf[128]={0};
	
	apmib_get(MIB_OPENVPN_ENABLED, (void *)&openvpn_enabled);
	apmib_get(MIB_OPENVPN_PORT, (void *)&port_num);
	if((openvpn_enabled==0) || (getWanIpAddr(NULL)==0))
		return ;	
	
	//create openvpn server config file
	create_openvpn_server_configfile();

	//create openvpn client config file
	create_openvpn_client_configfile();

	//kill existed daemon
	if(find_pid_by_name("openvpn")>0)
		system("killall openvpn > /dev/null 2>&1");
	
	//start openvpn daemon
	printf("\n%s:%d start openvpn!!!\n",__FUNCTION__,__LINE__);
	sprintf(line_buffer, "openvpn --config %s &", OPENVPN_SERVER_CONFIG_FILE);
	system(line_buffer);	

	//add iptable rules
	if(!(port_num>0 && port_num<65536))
			port_num=1194;

	sprintf(cmdBuf, "iptables -t filter -D INPUT -p udp -m udp --dport %d -j ACCEPT > /dev/null 2>&1", port_num);
	system(cmdBuf);
	system("iptables -t filter -D INPUT -i tun0 -j ACCEPT > /dev/null 2>&1");
	system("iptables -t filter -D FORWARD -i tun0  -j ACCEPT > /dev/null 2>&1");

	sprintf(cmdBuf, "iptables -t filter -A INPUT -p udp -m udp --dport %d -j ACCEPT > /dev/null 2>&1", port_num);
	system(cmdBuf);
	system("iptables -t filter -A INPUT -i tun0 -j ACCEPT > /dev/null 2>&1");
	system("iptables -t filter -I FORWARD 1 -i tun0  -j ACCEPT > /dev/null 2>&1");	
}
#endif

#if defined(CONFIG_APP_BT_REPEATER_CONFIG)
#include <fcntl.h>
#if defined(CONFIG_RTK_BLUETOOTH_HW_RTL8822B_S)
#define RTK_BT_CFG_FILE 			"/var/firmware/rtlbt/rtl8822b_config"
#define RTK_BT_MP_FILE_DATA_LENGTH     (9+9+4+19)
#elif defined(CONFIG_RTK_BLUETOOTH_HW_RTL8761A_S)
#define RTK_BT_CFG_FILE				 "/var/firmware/rtlbt/rtl8761a_config"
#define RTK_BT_MP_FILE_DATA_LENGTH     (9+4+4+19)
#endif

static void gen_rtlbt_fw_config(void)
{
	int fd;
	int count;
	int i,j;
	unsigned char hw_btaddr[6] = {0};
	unsigned char hw_txpwr_idx[6] = {0};
	unsigned char hw_xtal_cap_val = 0;
	unsigned char hw_tx_dac_val = 0;
	
	system("mkdir /var/firmware");
#if defined(CONFIG_USB_BLUETOOTH)
	system("cp /etc/rtlbt/* /var/firmware/");
#elif defined(CONFIG_SERIAL_BLUETOOTH)
	system("mkdir /var/firmware/rtlbt");
	system("cp /etc/rtlbt/* /var/firmware/rtlbt/");
	apmib_get(MIB_BLUETOOTH_HW_BT_ADDR,(void*)&hw_btaddr);
	apmib_get(MIB_BLUETOOTH_HW_TX_POWER_IDX,(void*)&hw_txpwr_idx);
	apmib_get(MIB_BLUETOOTH_HW_TX_DAC_VAL,(void*)&hw_tx_dac_val);

	unsigned char header[6] = {0x55,0xab,0x23,0x87,0x00,0x00};
	
	unsigned char baudrate[19] = {0x0c,0x00,0x10,\
		0x0a,0xc0,0x52,0x02,\
		0x50,0xc5,0xea,0x19,\
		0xe1,0x1b,0xfd,0xaf,\
		0x5f,0x01,0xa4,0x0b};
	
	/*****here can 0x15b+4(len)+1M+2M+3M+LE or 0x15a+6+Max+1/2/3/le+0x1*****/
	unsigned char txpwr_idx[9] = {0x5a,0x01,0x06,0x19,0x16,0x16,0x16,0x16,0x01};
	unsigned char xtal_cap_val[4] = {0xe6,0x01,0x01,0x00};
	unsigned char tx_dac_val[4] = {0x61,0x01,0x01,0x00};
	unsigned char bt_addr[9] = {0x44,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};
#if defined(CONFIG_RTK_BLUETOOTH_HW_RTL8822B_S)
	bt_addr[0] = 0x44;
	apmib_get(MIB_HW_11N_XCAP,(void*)&hw_xtal_cap_val);//8822B's XTALL is same with wifi's
#elif defined(CONFIG_RTK_BLUETOOTH_HW_RTL8761A_S)
	bt_addr[0] = 0x3c;
	apmib_get(MIB_BLUETOOTH_HW_XTAL_CAP_VAL,(void*)&hw_xtal_cap_val);
#endif
	header[4] = RTK_BT_MP_FILE_DATA_LENGTH & 0xff;
	header[5] = (RTK_BT_MP_FILE_DATA_LENGTH>>8) & 0xff;
	
	for(i=0,j=5;i<6;i++)
	{
		bt_addr[3+i] = hw_btaddr[j];
		j--;
	}
	for(i=0;i<5;i++)
	{
		if(hw_txpwr_idx[i] != 0)
		{
			txpwr_idx[3+i] = hw_txpwr_idx[i];
		}
	}
	txpwr_idx[8] = 0x01;
	xtal_cap_val[3] = hw_xtal_cap_val;
	tx_dac_val[3]  = hw_tx_dac_val;
	fd = open(RTK_BT_CFG_FILE,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if(fd >= 0)
	{
		count = write(fd,header,sizeof(header));
		if(count < 0)
		{
			printf("Failed to write bt config file header!\n");
			goto end;
		}
		count = write(fd,baudrate,sizeof(baudrate));
		if(count < 0)
		{
			printf("Failed to write bt config file baudrate!\n");
			goto end;
		}
		count = write(fd,bt_addr,sizeof(bt_addr));
		if(count < 0)
		{
			printf("Failed to write bt config file BT address!\n");
			goto end;
		}
		count = write(fd,xtal_cap_val,sizeof(xtal_cap_val));
		if(count < 0)
		{
			printf("Failed to write bt config file xtal_cap_val!\n");
			goto end;
		}
#if defined(CONFIG_RTK_BLUETOOTH_HW_RTL8822B_S)
		count = write(fd,txpwr_idx,sizeof(txpwr_idx));
		if(count < 0)
		{
			printf("Failed to write bt config file txpwr_idx!\n");
			goto end;
		}
#elif defined(CONFIG_RTK_BLUETOOTH_HW_RTL8761A_S)
		count = write(fd,tx_dac_val,sizeof(tx_dac_val));
		if(count < 0)
		{
			printf("Failed to write bt config file hw_tx_dac_val!\n");
			goto end;
		}
#endif	
	}
end:
	if(fd >= 0)
	{
		close(fd);
	}
#endif	
}
#define RTK_BT_DEV_NAME_PREFIX "realtek_repeater"
static void gen_rtk_bt_dev_name(char *dev_name)
{
	unsigned char hw_btaddr[6] = {0};
	FILE* fp;
	if(dev_name == NULL)
	{
		return;
	}
	apmib_get(MIB_BLUETOOTH_HW_BT_ADDR,(void*)&hw_btaddr);
	sprintf(dev_name,"%s%02x%02x%02x",RTK_BT_DEV_NAME_PREFIX,
		hw_btaddr[3],
		hw_btaddr[4],
		hw_btaddr[5]);
	fp = fopen("/usr/local/etc/bluetooth/main.conf","a");
	if(fp != NULL)
	{
		fprintf(fp,"\nName = %s\n",dev_name);
		fclose(fp);
	}
}
#endif
#ifdef RTK_CROSSBAND_REPEATER
void crossband_app()
{
	int wlan_disabled1 = 1, wlan_disabled2 = 1;
	int repeater_enabled1 = 0, repeater_enabled2 = 0;
	int val = 0;
	
	if(isFileExist("/var/run/crossband.pid"))
	{
		system("killall crossband_daemon >/dev/null 2>&1");
	}
	
	apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enabled1);
	apmib_get(MIB_REPEATER_ENABLED2,(void *)&repeater_enabled2);

#if defined(FOR_DUAL_BAND)
	if(SetWlan_idx("wlan1")){ 
		apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled1);	  
	}
#endif
	if(SetWlan_idx("wlan0")){ 
		apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled2);	  
	}

	if(repeater_enabled1 && repeater_enabled2 && !wlan_disabled1 && !wlan_disabled2) //dependency check
	{	
		val = 1;
	}

	apmib_set(MIB_WLAN_CROSSBAND_ACTIVATE, (void *)&val);

}
#endif
#ifdef RTL_NF_ALG_CTL
int rtk_alg_get(unsigned char *protocol,bool *enable)
{
	FILE *fp;
	char buf[64];
	unsigned char *saveStr;
	unsigned char *token;
	memset(buf,0,sizeof(buf));
	if((protocol == NULL)||enable == NULL)
		return -1;
	if ((fp = fopen("/proc/alg", "r")) != NULL)
	{
			while(fgets(buf, sizeof(buf),fp)){
				saveStr = buf;
				token = strsep(&saveStr," ");
				if(token != NULL)
				{
					if(strcmp(token, "|")==0)
					{
						token = strsep(&saveStr,"=");
						if(token != NULL)
							if(strcmp(token,protocol)==0)
							{
								if(saveStr != NULL)
								{
									*enable=atoi(saveStr);
									fclose(fp);
									return 0;
								}

							}
							
					}
				}
				memset(buf,0,sizeof(buf));
			}
			fclose(fp);
			return -1;
	}
	return -1;
	
}
int rtk_alg_set(unsigned char *protocol,bool enable)
{
	char buf[64];
	memset(buf,0,sizeof(buf));
	bool enable_tmp;
	if(protocol == NULL)
		return -1;
	enable=!enable;
	if(strcmp("ftp",protocol)==0)
		if(rtk_alg_get("ftp",&enable_tmp) == 0){
			apmib_set(MIB_ALG_FTP_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else if(strcmp("tftp",protocol)==0)
		if(rtk_alg_get("tftp",&enable_tmp) == 0){
			apmib_set(MIB_ALG_TFTP_DISABLE, (void *) &enable);
		}else{

			return -1;
		}
	else if(strcmp("rtsp",protocol)==0)
		if(rtk_alg_get("rtsp",&enable_tmp) == 0){
			apmib_set(MIB_ALG_RTSP_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else if(strcmp("pptp",protocol)==0)
		if(rtk_alg_get("pptp",&enable_tmp) == 0){
			apmib_set(MIB_ALG_PPTP_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else if(strcmp("l2tp",protocol)==0)
		if(rtk_alg_get("l2tp",&enable_tmp) == 0){
			apmib_set(MIB_ALG_L2TP_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else if(strcmp("ipsec",protocol)==0)
		if(rtk_alg_get("ipsec",&enable_tmp) == 0){
			apmib_set(MIB_ALG_IPSEC_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else if(strcmp("sip",protocol)==0)
		if(rtk_alg_get("sip",&enable_tmp) == 0){
			apmib_set(MIB_ALG_SIP_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else if(strcmp("h323",protocol)==0)
		if(rtk_alg_get("h323",&enable_tmp) == 0){
			apmib_set(MIB_ALG_H323_DISABLE, (void *) &enable);
		}else{
			return -1;
		}
	else
		return -1;
	sprintf(buf,"echo %s %d > /proc/alg\n",protocol,!enable);
	system(buf);
	return 0;
}

int reinit_fun_alg_reinit(void)
{
	char buf[64];
	bool disable;
	bool enable;
	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_FTP_DISABLE,(void *) &disable);
	if(rtk_alg_get("ftp",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","ftp",!disable);
		system(buf);
	}

	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_TFTP_DISABLE,(void *) &disable);
	if(rtk_alg_get("tftp",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","tftp",!disable);
		system(buf);
	}
	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_RTSP_DISABLE,(void *) &disable);
	if(rtk_alg_get("rtsp",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","rtsp",!disable);
		system(buf);
	}
	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_PPTP_DISABLE,(void *) &disable);
	if(rtk_alg_get("pptp",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","pptp",!disable);
		system(buf);
	}

	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_L2TP_DISABLE,(void *) &disable);
	if(rtk_alg_get("l2tp",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","l2tp",!disable);
		system(buf);
	}
	
	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_IPSEC_DISABLE,(void *) &disable);
	if(rtk_alg_get("ipsec",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","ipsec",!disable);
		system(buf);
	}
	
	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_SIP_DISABLE,(void *) &disable);
	if(rtk_alg_get("sip",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","sip",!disable);
		system(buf);
	}
	
	memset(buf,0,sizeof(buf));
	apmib_get(MIB_ALG_H323_DISABLE,(void *) &disable);
	if(rtk_alg_get("h323",&enable) == 0)
	{
		sprintf(buf,"echo %s %d > /proc/alg\n","h323",!disable);
		system(buf);
	}
	return 0;
	
}

#endif
#ifdef CONFIG_SET_PHY_PORT_POWER_DOWN
void set_physical_port_power_down(void)
{
	system("echo 0x1f 0 > /proc/phyPower\n");
}
#endif
#if defined(MTD_NAME_MAPPING) && defined(CONFIG_RTL_JFFS2_FILE_PARTITION)
#define MTD_JFFS2_PARTITION_NAME "jffs2 file"
void mount_jffs2_partition()
{
	char buf[1024];
	char cmdbuf[128];
	char mtdname[32] = {0};
	FILE *fp;
	char *mtdnum;
	int status=0;
	
	rtl_name_to_mtdblock(MTD_JFFS2_PARTITION_NAME, mtdname);	
	snprintf(cmdbuf, sizeof(cmdbuf), "mount -t jffs2 %s /jffs2 2>&1", mtdname);
	
	fp = popen(cmdbuf,"r");
	if(fp == NULL) {
		printf("[%s %d] popen failed:%s",__FUNCTION__,__LINE__, strerror(errno));
		return;
	}
	
	while(!feof(fp))
	{	
		if(fgets(buf, sizeof(buf), fp)) {
			printf("[mount]: %s",buf);
			//if(strstr(buf,"Cowardly refusing to erase blocks on filesystem with no valid JFFS2 nodes")) {
			if(strstr(buf,"Input/output error")) {
				mtdnum = mtdname + strlen("/dev/mtdblock");
				snprintf(cmdbuf, sizeof(cmdbuf), "flash_erase -j /dev/mtd%d 0 0", atoi(mtdnum));
				system(cmdbuf);
				printf("flash_erase to format jffs2 partition!\n");
				printf("%s\n",cmdbuf);

				/*remount after erase partition*/
				snprintf(cmdbuf, sizeof(cmdbuf), "mount -t jffs2 %s /jffs2", mtdname);
				system(cmdbuf);
				break;
			}
		}
	}	
	printf("%s\n",cmdbuf);

	if(access("/jffs2/cwmp_config",F_OK) != 0) {
		system("mkdir /jffs2/cwmp_config");
		printf("Create cwmp_config direction!\n");
	}
	
	pclose(fp);
	return;
}
#endif

#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)
#if 0
#define RTL_LINK_PID_FILE 		"/var/run/rtl_link.pid"
#define RTL_LINK_SWITCH_PPPOE	"/var/tmp/switch_pppoe"
#define RTL_LINK_IF5_PID_FILE 	"/var/run/rtl_link_if5.pid"
#define ANDLINK_IF5_EXIT   "/var/tmp/andlink_if5_exit"
#define ANDLINK_RECEIVE_NETINFO   "/var/tmp/andlink_receive_netinfo"
#endif

#if defined(CONFIG_ANDLINK_SUPPORT)
int cleanAndlink()
{
	int pid=0;
	char strPID[32];
	
	//clean andlink
	 if(isFileExist(RTL_LINK_PID_FILE)){
        pid=getPid_fromFile(RTL_LINK_PID_FILE);
        if(pid != -1){
            sprintf(strPID, "%d", pid);
            RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
        }
        unlink(RTL_LINK_PID_FILE);
    }

	//if(isFileExist(RTL_LINK_SWITCH_PPPOE))
	//	unlink(RTL_LINK_SWITCH_PPPOE);
	if(isFileExist(ANDLINK_IF6_STATUS))
		unlink(ANDLINK_IF6_STATUS);

#ifdef CONFIG_ANDLINK_IF5_SUPPORT
	 //clean andlink_if5
	 if(isFileExist(RTL_LINK_IF5_PID_FILE)){
        pid=getPid_fromFile(RTL_LINK_IF5_PID_FILE);
        if(pid != -1){
            sprintf(strPID, "%d", pid);
            RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
        }
        unlink(RTL_LINK_IF5_PID_FILE);
    }
#endif

	return 0;
}
#endif
#endif


unsigned int rtk_multicast_get_snoopy_enable(bool *enable)
{
	FILE *fp;
	char buf[100];
	unsigned char *saveStr;
	unsigned char *token;
	memset(buf,0,sizeof(buf));
	if ((fp = fopen("/proc/br_igmpsnoop", "r")) != NULL)
	{
			while(fgets(buf, sizeof(buf),fp)){
				saveStr = buf;
				token = strsep(&saveStr,":");
				if(token != NULL)
				{
					if(strcmp(token, "igmpsnoopenabled")==0)
					{
						token = strsep(&saveStr," ");
						if(token != NULL)
						{
							fclose(fp);
							if(atoi(token))
								*enable=1;
							else
								*enable=0;
							return 0;

						}
							
					}
				}
				memset(buf,0,sizeof(buf));
			}
			fclose(fp);
			return -1;
	}
	return -1;

}

unsigned int rtk_multicast_set_mld_snoopy_enable(bool enable)
{
	char buf[64];
	memset(buf,0,sizeof(buf));
	enable=!enable;
	apmib_set(MIB_MLD_SNOOPING_DISABLE, (void *) &enable);
	sprintf(buf,"echo %d > /proc/br_mldsnoop\n",!enable);
	system(buf);
	return 0;

}


unsigned int rtk_multicast_get_mld_snoopy_enable(bool *enable)
{
	FILE *fp;
	char buf[100];
	unsigned char *saveStr;
	unsigned char *token;
	memset(buf,0,sizeof(buf));
	if ((fp = fopen("/proc/br_mldsnoop", "r")) != NULL)
	{
			while(fgets(buf, sizeof(buf),fp)){
				saveStr = buf;
				token = strsep(&saveStr,":");
				if(token != NULL)
				{
					if(strcmp(token, "mldsnoopenabled")==0)
					{
						token = strsep(&saveStr," ");
						if(token != NULL)
						{
							fclose(fp);
							if(atoi(token))
								*enable=1;
							else
								*enable=0;
							return 0;

						}
							
					}
				}
				memset(buf,0,sizeof(buf));
			}
			fclose(fp);
			return -1;
	}
	return -1;

}

void rtk_igmp_and_mld_snoop_reinit(void)
{
	bool disable;
	char buf[30];
	apmib_get(MIB_IGMP_SNOOPING_DISABLE, (void *) &disable);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"echo %d > /proc/br_igmpsnoop\n",!disable);
	system(buf);
	apmib_get(MIB_MLD_SNOOPING_DISABLE, (void *) &disable);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"echo %d > /proc/br_mldsnoop\n",!disable);
	system(buf);
}

int setinit(int argc, char** argv)
{
	int wlan0_disabled;
	int i, cmdRet=-1;
	int opmode=-1, v_wlan_app_enabled=0, intValue=0, intValue1=0;
	char cmdBuffer[100], tmpBuff[512];
	int repeater_enabled1=0, repeater_enabled2=0;
	char *token=NULL, *savestr1=NULL;
	char tmp_args[16];
	int wisp_wan_id=0;
	int lan_dhcp_mode=0;
	int wan_dhcp_mode=0;
	char Ip[32], Mask[32], Gateway[32];
	int wlan_mode_root=0, wlan_root_disabled=0;
	int br_wlan_block=0;
	char strPID[32];
	int pid = 0;
	int wlan_support = 0;
	int index; 
	int old_wlan_idx;
	int repeater_enable1=0;
	int repeater_enable2=0;
#if defined(CONFIG_RTL_92D_SUPPORT)
	int wlan_mode_root1=0, wlan_root1_disabled=0;
#endif
#ifdef RTL_NF_ALG_CTL
	bool alg_enable;
#endif
#if defined(CONFIG_RTL_92D_SUPPORT)
	int wispWanId=0;
#endif
#if defined(CONFIG_APP_TR069) && defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
	int capEntryNum;
	CAP_PORTAL_T capEntry;
#endif
	int reinit=1;

#if defined(CONFIG_RTL_ULINKER_WLAN_DELAY_INIT)
	int ulinker_auto = 0;
#endif
#ifdef CONFIG_RTL_DNS_TRAP
	unsigned char domain_name[MAX_NAME_LEN];
	apmib_get(MIB_DOMAIN_NAME, (void *)domain_name);
	strtolower(domain_name,strlen(domain_name));
	/* For HeMu, do not add `.com` if domain is cmiot.10086.cn */
	//if (strncasecmp(domain_name, "cmiot.10086.cn", 12))
	//	strcat(domain_name,".com");

	create_hosts_file(domain_name);
	RunSystemCmd(DNS_PROC_FILE, "echo", domain_name, NULL_STR);
#endif
#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)	
	int rtl_link_enabled = 0;
#endif
#if defined(CONFIG_APP_SDK_ONENET)
	char vendor[32] = {0};
	unsigned char hw_mac[32] = {0};
	char sn[32] = {0};
#endif
#if defined(CONFIG_APP_RTK_BLUETOOTH_FM)  
        system("echo /bin/mdev > /proc/sys/kernel/hotplug");
        system("mount -t sysfs sysfs /sys");
        system("echo 1 > /sys/class/firmware/timeout");
#endif

#if defined(CONFIG_RTL_JFFS2_FILE_PARTITION) && defined(MTD_NAME_MAPPING) 
	mount_jffs2_partition();
#endif
//for 2.4g-wlan0 5g-wlan1, not normal 5g-wlan0 2.4g-wlan1
#if defined(CONFIG_BAND_2G_ON_WLAN0) || defined(CONFIG_BAND_5G_ON_WLAN0)
	int wlanBand2G5GSelect;
	int phyBandSelect;
	apmib_get(MIB_WLAN_BAND2G5G_SELECT,(void *)&wlanBand2G5GSelect);
	if(wlanBand2G5GSelect == BANDMODEBOTH)//dual band
	{
		old_wlan_idx = wlan_idx;
		wlan_idx = 0;
		apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyBandSelect);	
#if defined(CONFIG_BAND_2G_ON_WLAN0)
		if(phyBandSelect != PHYBAND_2G)
			swapWlanMibSetting(0,1);
#else
#ifndef CONFIG_APP_APPLE_MFI_WAC
        if(phyBandSelect != PHYBAND_5G)
			swapWlanMibSetting(0,1);
#endif
#endif
		wlan_idx = old_wlan_idx;
	}
#endif

    if(strcmp(argv[2], "ap")||strcmp(argv[3], "wlan_app")){
		//it will take about 7s for LAN PC to send DHCP request packet, down eth0 early
		RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "down", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "br0", "down", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "br0", "up", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "up", NULL_STR);
	}
	
#ifdef MULTI_PPPOE
	//now modify. if need , add parameter
	setInitMultiPPPoE();
#endif
	if(isFileExist(SET_TIME)==0){
		RunSystemCmd(NULL_FILE, "flash", "settime", NULL_STR);
	}



	if(isFileExist(REINIT_FILE)==0){
		up_mib_value();

#ifdef DEFSETTING_AUTO_UPDATE
		defsetting_auto_update();
#endif
		reinit = 0;
	}
	else
		reinit = 1;

	if(1){		
#ifdef CONFIG_CMCC
		if(up_ssid_value()==0){
			int retValue = 0, kk;	
			CUST_SET_SSID_T set_type;
#ifdef CUSTOMER_HW_SETTING_SUPPORT 
			for(kk=0; kk<NUM_WLAN_INTERFACE; ++kk){
				retValue = up_cust_hw_value(kk);
				if( retValue==1 ){
					set_type = HW_SSID_GEN;
					set_ssid_customize(set_type);
				}
			}
			set_admin_password();

			//set andlink v2.5 cloud gateway ip address
#ifdef CONFIG_ANDLINK_SUPPORT
			set_cgw_ip_addr();
#endif
			set_op_mode();
			set_lanip();
			set_domain_name();
			set_provincial_code();
#else			
			set_type = NO_HW_SSID_GEN;
			set_ssid_customize(NO_HW_SSID_GEN);
#endif
			retValue = 1;
			apmib_set(MIB_SSID_VER, (void *)&retValue);
			apmib_update(CURRENT_SETTING);
		}
#endif
	}

#if defined(CONFIG_RTL_819XD)
	if(reinit == 1)
	{
		//The powerful CPU won't take 7s from interface down to up, so sleep 1s to force LAN/WLAN PC to renew IP.
		//The timeout is 25s in WEB UI, it can redirect to new IP after count down timeout, so sleep 1s is not side effect.
		sleep(1);
	}	
#endif

#ifdef CONFIG_RTL_802_1X_CLIENT_SUPPORT
	if((isFileExist(RS_USER_CERT_5G)==0) && (isFileExist(RS_ROOT_CERT_5G)==0) &&  (isFileExist(RS_USER_CERT_2G)==0) && (isFileExist(RS_ROOT_CERT_2G)==0)){
		RunSystemCmd(NULL_FILE, "rsCert","-rd", NULL_STR);
	}
#endif

#ifdef CONFIG_RTL_WAPI_SUPPORT
#ifdef CONFIG_RTL_WAPI_LOCAL_AS_SUPPORT
	if(isFileExist(CA_CERT_FILE)==0){
		RunSystemCmd(NULL_FILE, "loadWapiFiles", NULL_STR);
	}
#else
	if((isFileExist(CA4AP_CERT)==0 && isFileExist(AP_CERT)==0 )
		&& (isFileExist(CA4AP_CERT_AS0)==0 && isFileExist(AP_CERT_AS0)==0 )
		&& (isFileExist(CA4AP_CERT_AS1)==0 && isFileExist(AP_CERT_AS1)==0 )){
		RunSystemCmd(NULL_FILE, "loadWapiFiles", NULL_STR);
	}
#endif
#endif	
	#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_APP_APPLE_MFI_WAC)
		system("echo 1 > /var/system/start_init");
	#endif 	
	
#if defined(CONFIG_DYNAMIC_WAN_IP)
	//Added for pptp/l2tp use dynamic wan ip
	if(isFileExist(TEMP_WAN_CHECK))
		unlink(TEMP_WAN_CHECK);
	if(isFileExist(TEMP_WAN_DHCP_INFO))
		unlink(TEMP_WAN_DHCP_INFO);
	if(isFileExist(MANUAL_CONNECT_NOW))
		unlink(MANUAL_CONNECT_NOW);
#endif

#ifdef TR181_V6_SUPPORT
	intValue = 1;
	if(init_dns_client_server_table == 0)
	{
		for(i=0; i<DNS_CLIENT_SERVER_NUM; i++)
		{
			sprintf(tmpBuff, "Device.DNS.Client.Server.{%d}.Enable", i);
			if(tr181_ipv6_set(tmpBuff ,(void *) &intValue) == -1)
			{
				printf("%s(%d): set %s fail!\n",__FUNCTION__,__LINE__, tmpBuff);
				return -1;
			}
		}
	}
	init_dns_client_server_table = 1;
#endif

	printf("Init Start...\n");

//#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void*)&wlan0_disabled);
	if(wlan0_disabled){
		system("FTY_TEST wifi off");
	} else {
		system("FTY_TEST wifi on");
	}
//#endif

#if defined(CONFIG_APP_APPLE_MFI_WAC)
	{
        char wac_nameBuf[64]={0};
        char hostname_cmd[80]={0};

        apmib_get(MIB_MFI_WAC_DEVICE_NAME,  (void *)wac_nameBuf);
        if(wac_nameBuf[0]){
            sprintf(hostname_cmd,"hostname \'%s\'",wac_nameBuf);
            system(hostname_cmd);
        }
    }		
#endif
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
	apmib_get(MIB_DHCP,(void *)&lan_dhcp_mode);
#ifdef   HOME_GATEWAY	
	apmib_get(MIB_WAN_DHCP,(void *)&wan_dhcp_mode);
#endif	
	memset(br_lan2_interface, 0x00, sizeof(br_lan2_interface));
	memset(vlan_interface, 0x00, sizeof(vlan_interface));
	if(opmode==GATEWAY_MODE)
		RunSystemCmd("/var/sys_op", "echo", "0", NULL_STR);
	else if(opmode==BRIDGE_MODE)
		RunSystemCmd("/var/sys_op", "echo", "1", NULL_STR);
	else if(opmode==WISP_MODE)
		RunSystemCmd("/var/sys_op", "echo", "2", NULL_STR);
#ifdef RTL_NF_ALG_CTL
	if((opmode ==BRIDGE_MODE)){
		alg_enable=0;
	}else{
		alg_enable=1;
	}
	rtk_alg_set("l2tp",alg_enable);
	rtk_alg_set("ipsec",alg_enable);
	rtk_alg_set("h323",alg_enable);
	rtk_alg_set("h323",alg_enable);
	rtk_alg_set("rtsp",alg_enable);
	rtk_alg_set("sip",0);
	rtk_alg_set("ftp",alg_enable);
	rtk_alg_set("pptp",alg_enable);
	rtk_alg_set("tftp",alg_enable);
#endif

	rtk_igmp_and_mld_snoop_reinit();

/* set interface name  start*/		
	sprintf(tmp_args,"%s", argv[2]);  
	if(strcmp(tmp_args, "ap") == 0){
        sprintf(br_interface, "%s", "br0");
		sprintf(br_lan1_interface, "%s" , "eth0");
#if !defined(CONFIG_RTL_AP_PACKAGE) 
		if(opmode==BRIDGE_MODE)
			sprintf(br_lan2_interface, "%s", "eth1");
#endif
		gateway=0;
	}	
	if(!strcmp(tmp_args, "gw")){		
        gateway=1;
		if(opmode==WISP_MODE)
		{
			sprintf(wan_interface, "wlan%d", wisp_wan_id);
#if defined(CONFIG_SMART_REPEATER)	
			getWispRptIfaceName(wan_interface,wisp_wan_id);

			//strcat(wan_interface,"-vxd");
#endif			
		}
		else
		{
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
			if(opmode == GATEWAY_MODE)
				sprintf(wan_interface, "%s", "eth1");
			
#else
			sprintf(wan_interface, "%s", "eth1");

	#if defined(CONFIG_4G_LTE_SUPPORT)
	if (lte_wan()) {
			sprintf(wan_interface, "%s", "usb0");
	}
	#endif /* #if defined(CONFIG_4G_LTE_SUPPORT) */
#endif
		}
			
//printf("\r\n wan_interface=[%s],__[%s-%u]\r\n",wan_interface,__FILE__,__LINE__);
			
		sprintf(br_interface, "%s", "br0");
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
		//if(opmode != 0)
#ifdef RTK_USB3G
 		if((wan_dhcp_mode == USB3G) || (opmode != 0))
			sprintf(br_lan1_interface, "%s" , "eth0");
		else
#endif
		if(opmode != GATEWAY_MODE)
			sprintf(br_lan1_interface, "%s" , "eth0");
			
#else
		sprintf(br_lan1_interface, "%s" , "eth0");
#endif

		if(opmode ==BRIDGE_MODE || opmode == WISP_MODE) {
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
#else
			sprintf(br_lan2_interface, "%s", "eth1");
#endif
		}
		/**/
		//RunSystemCmd("/proc/wan_port", "echo", "4", NULL_STR);	
	}
	/*
	if(isFileExist(ETH_VLAN_SWITCH)){//vlan init file is exist
		sprintf(vlan_interface, "%s %s %s", "eth2", "eth3", "eth4");
	}
	*/

	//enable stormctrl to avoid init fail with high loading multicast.
	//RunSystemCmd("/proc/StormCtrl", "echo", "1 3", NULL_STR);
	RunSystemCmd(PROC_BR_IGMPPROXY, "echo", "0", NULL_STR);
	
#if defined(VLAN_CONFIG_SUPPORTED)
	apmib_get(MIB_VLANCONFIG_ENABLED, (void *)&intValue);
	if(intValue !=0) {
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
#elif defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
		sprintf(vlan_interface, "%s %s %s %s", "eth2", "eth3", "eth4", "eth7");
#else
		sprintf(vlan_interface, "%s %s %s", "eth2", "eth3", "eth4");
#endif
	}
	else		
		memset(vlan_interface, 0x00, sizeof(vlan_interface));

#endif //#if defined(VLAN_CONFIG_SUPPORTED)

	memset(wlan_interface, 0x00, sizeof(wlan_interface));
	memset(wlan_virtual_interface, 0x00, sizeof(wlan_virtual_interface));
	memset(wlan_vxd_interface, 0x00, sizeof(wlan_vxd_interface));
	memset(wlan_valid_interface, 0x00, sizeof(wlan_valid_interface));
	
	for(i=0;i<NUM_WLAN_INTERFACE;i++){
		if(wlan_interface[0]==0x00)
			sprintf(wlan_interface, "wlan%d", i);
		else{
			sprintf(tmp_args, " wlan%d", i);
			strcat(wlan_interface, tmp_args); 
		}
	}
	/* collect all wlan interface for clean_process() */
	num_wlan_interface=NUM_WLAN_INTERFACE;
	num_wlan_virtual_interface=if_readlist_proc(wlan_virtual_interface, "va", 0);
	num_wlan_vxd_interface=if_readlist_proc(wlan_vxd_interface, "vxd", 0);
#if defined(CONFIG_RTK_MESH)
	num_wlan_mesh_interface=if_readlist_proc(wlan_mesh_interface, "msh", 0);
#endif
	wlan_support = if_readlist_proc(wlan_valid_interface, "wlan", 0);
	if(wlan_support==0)
		memset(wlan_interface, 0x00, sizeof(wlan_interface));
		
//printf("\r\n wlan_vxd_interface=[%s],__[%s-%u]\r\n",wlan_vxd_interface,__FILE__,__LINE__);
		
/* set interface name  end*/			
#if defined(CONFIG_POCKET_AP_SUPPORT) && defined(HOME_GATEWAY)
	sprintf(tmp_args,"%s", argv[3]);
	if(strcmp(tmp_args, "wlan_app") != 0)
	{
		clean_process(opmode,wan_dhcp_mode,gateway, enable_wan, br_interface, wlan_interface, wan_interface);
	
		/*init wlan interface*/
		if (wlan_support != 0)
		{
			memset(wlan_interface, 0x00, sizeof(wlan_interface));
			for(i=0;i<NUM_WLAN_INTERFACE;i++)
			{
				int wlan_disable = 1;			
				unsigned char wlan_name[10];
				memset(wlan_name,0x00,sizeof(wlan_name));
				sprintf(wlan_name, "wlan%d",i);
				if(SetWlan_idx(wlan_name))
				{			
					apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);	  

				
					if(wlan_disable == 1)
					{
						RunSystemCmd(NULL_FILE, "iwpriv", wlan_name, "radio_off", NULL_STR);					
					}
					else
					{
						if(wlan_interface[0]==0x00)
							sprintf(wlan_interface, "%s", wlan_name);
						else
						{
							sprintf(tmp_args, " %s", wlan_name);
							strcat(wlan_interface, tmp_args); 
						}							
					}
				}
			}				
		}
	}
#endif	//#if defined(CONFIG_POCKET_AP_SUPPORT) && defined(HOME_GATEWAY)

#ifdef RTK_CROSSBAND_REPEATER
	crossband_app();
#endif

/*currently, we just support init gw/ap all */	
	sprintf(tmp_args,"%s", argv[3]);  
	
	if(!strcmp(tmp_args, "all")){
		enable_wan=1;
		enable_br=1;
	}else if(!strcmp(tmp_args, "wan")){
		enable_wan=1;
		enable_br=1;
	}else if(!strcmp(tmp_args, "bridge")){
		enable_wan=1;
		enable_br=1;
	}else if(!strcmp(tmp_args, "wlan_app")){
		start_wlanapp(0);
#ifdef CONFIG_SET_PHY_PORT_POWER_DOWN
		set_physical_port_power_down();
#endif
		return 0;
	}

	/*save the last wan type*/ /*no this operate in shell script*/


#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
	clean_process(opmode,wan_dhcp_mode,gateway, enable_wan, br_interface, wlan_interface, wan_interface);
	RunSystemCmd(NULL_FILE, "ifconfig", "eth1", "down", NULL_STR);
	RunSystemCmd(NULL_FILE, "ifconfig", "peth0", "down", NULL_STR);
#if defined(CONFIG_SMART_REPEATER)
	getWispRptIfaceName(wan_interface,wisp_wan_id);
	if(strstr(wan_interface,"-vxd"))
			RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "0.0.0.0", NULL_STR);
	RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "down", NULL_STR);
#endif
#else
	if(reinit == 1)
	{
		clean_process(opmode,wan_dhcp_mode,gateway, enable_wan, br_interface, wlan_interface, wan_interface);
		
#if !defined(CONFIG_RTL_AP_PACKAGE) 
		//MUST down ppp related interface before eth1 down, otherwise it will cause some puzzling problems.
		RunSystemCmd(NULL_FILE, "ifconfig", "eth1", "down", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "peth0", "down", NULL_STR);
#if defined(CONFIG_SMART_REPEATER)
		if(strstr(wan_interface,"-vxd"))
			RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "0.0.0.0", NULL_STR);
#endif
		if(wan_interface[0])
        {
			RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "0.0.0.0", NULL_STR);
			RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "down", NULL_STR);
        }
#endif
	}
	else
	{
#ifdef   HOME_GATEWAY
#ifdef CONFIG_POCKET_AP_SUPPORT
#else
	if(isFileExist(HW_NAT_FILE)){/*hw nat supported*/
		/*cleanup hardware tables*/
		if(opmode==GATEWAY_MODE)
			RunSystemCmd(HW_NAT_FILE, "echo", "1", NULL_STR);	/*gateway mode*/
		else if(opmode==BRIDGE_MODE)
			RunSystemCmd(HW_NAT_FILE, "echo", "2", NULL_STR);	/*bridge mode*/
		else if(opmode==WISP_MODE)
			RunSystemCmd(HW_NAT_FILE, "echo", "3", NULL_STR);	/*wisp mode*/
		else if(opmode==3)
			RunSystemCmd(HW_NAT_FILE, "echo", "4", NULL_STR);	/*bridge mode with multiple vlan*/
		else
			RunSystemCmd(HW_NAT_FILE, "echo", "5", NULL_STR); /*wisp mode with multiple vlan*/
		
	}else{/*software nat supported*/ 
		if(isFileExist(SOFTWARE_NAT_FILE)){
		if(opmode==GATEWAY_MODE)
		{
#ifdef RTK_USB3G
			if(wan_dhcp_mode == USB3G)
      	RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "1", NULL_STR);
			else
#endif
				RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "0", NULL_STR);
		}
		else if(opmode==BRIDGE_MODE)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "1", NULL_STR);
		else if(opmode==WISP_MODE)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "2", NULL_STR);
		else if(opmode==3)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "3", NULL_STR);
		else if(opmode==4)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "4", NULL_STR);
	}
		
	}
#endif	//CONFIG_POCKET_AP_SUPPORT
#endif	
	}
#endif

	sprintf(tmp_args, "%d", wan_dhcp_mode);
	RunSystemCmd("/var/system/last_wan", "echo", tmp_args, NULL_STR);

	apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enabled1);
	apmib_get(MIB_REPEATER_ENABLED2,(void *)&repeater_enabled2);

#if defined(CONFIG_RTL_92D_SUPPORT)
	if(SetWlan_idx("wlan1")){
			apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode_root1); 
			apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_root1_disabled);	  
	}
	if(v_wlan_app_enabled == 0 
		&& (wlan_mode_root1 == AP_MODE || wlan_mode_root1 == AP_WDS_MODE || repeater_enabled2 == 1)
	)
	{
		v_wlan_app_enabled=1; // have virtual or repeat
	}
	
#endif
	if(SetWlan_idx("wlan0")){
		apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode_root); 
		apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_root_disabled);	  
	}

	if(v_wlan_app_enabled == 0 
		&& (wlan_mode_root == AP_MODE || wlan_mode_root == AP_WDS_MODE || repeater_enabled1 == 1)
	)
	{
		v_wlan_app_enabled=1; // have virtual or repeat
	}
	
	memset(wlan_vxd_interface, 0x00, sizeof(wlan_vxd_interface));
	if(repeater_enabled1 == 1 && !wlan_root_disabled){
#if defined(CONFIG_SMART_REPEATER)
		if(strcmp(wan_interface,"wlan0-vxd"))
#endif
		{
			if(strlen(wlan_vxd_interface) != 0)
				strcat(wlan_vxd_interface," ");	
			strcat(wlan_vxd_interface,"wlan0-vxd");
		}
	}
#if defined(CONFIG_RTL_92D_SUPPORT)
	if(repeater_enabled2 == 1 && !wlan_root1_disabled)
	{
#if defined(CONFIG_SMART_REPEATER)
        if(strcmp(wan_interface,"wlan1-vxd"))
#endif
		{

			if(strlen(wlan_vxd_interface) != 0)
				strcat(wlan_vxd_interface," ");	
			strcat(wlan_vxd_interface,"wlan1-vxd");
		}	
	}
#endif

//printf("\r\n wlan_vxd_interface=[%s],__[%s-%u]\r\n",wlan_vxd_interface,__FILE__,__LINE__);

	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)tmpBuff);
	if(!memcmp(tmpBuff, "\x00\x00\x00\x00\x00\x00", 6))
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)tmpBuff);
	sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", (unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1], 
		(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], (unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth2", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth3", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth4", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
#else
	if(br_lan1_interface[0])
		RunSystemCmd(NULL_FILE, "ifconfig", br_lan1_interface, "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
#endif
	
#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
	Init_Domain_Query_settings(opmode, wlan_mode_root, lan_dhcp_mode, cmdBuffer);
#endif
	if(opmode == BRIDGE_MODE || opmode == WISP_MODE){
		apmib_get(MIB_ELAN_MAC_ADDR,  (void *)tmpBuff); // clone lan mac
		if(!memcmp(tmpBuff, "\x00\x00\x00\x00\x00\x00", 6))
		{
#if defined(CONFIG_RTL_8198_AP_ROOT) || defined(CONFIG_RTL_8197D_AP)
			apmib_get(MIB_HW_NIC0_ADDR,  (void *)tmpBuff);
#else
			apmib_get(MIB_HW_NIC1_ADDR,  (void *)tmpBuff);
#endif			
		}
		sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", (unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1], 
			(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], (unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);	
		if(br_lan2_interface[0])	
			RunSystemCmd(NULL_FILE, "ifconfig", br_lan2_interface, "hw", "ether", cmdBuffer, NULL_STR);/*set eth1 mac address when bridge mode*/
	} 
#ifdef   HOME_GATEWAY	
	if(gateway == 1 && opmode != BRIDGE_MODE){
		apmib_get(MIB_WAN_MAC_ADDR,  (void *)tmpBuff); // clone wan mac
		if(!memcmp(tmpBuff, "\x00\x00\x00\x00\x00\x00", 6)){
			if(opmode == WISP_MODE)
			{
				apmib_get(MIB_WISP_WAN_ID, (void *)&index);
				old_wlan_idx=wlan_idx;
				wlan_idx = index;
				apmib_get(MIB_HW_WLAN_ADDR,  (void *)tmpBuff);
				wlan_idx=old_wlan_idx;
			}
			else
				apmib_get(MIB_HW_NIC1_ADDR,  (void *)tmpBuff);
		}
		sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", (unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1], 
			(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], (unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);
		if(wan_interface[0])		
			RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "hw", "ether", cmdBuffer, NULL_STR);/*set wan mac address when it not bridge mode*/
#if defined(CONFIG_SMART_REPEATER)
			if(opmode == WISP_MODE && index == 0)
				RunSystemCmd(NULL_FILE, "ifconfig", "wlan0", "hw", "ether", cmdBuffer, NULL_STR);/*set wan mac address when it not bridge mode*/
			else if(opmode == WISP_MODE && index == 1)
				RunSystemCmd(NULL_FILE, "ifconfig", "wlan1", "hw", "ether", cmdBuffer, NULL_STR);/*set wan mac address when it not bridge mode*/
#endif
	}
#endif	

	/*init wlan interface*/
	RunSystemCmd(NULL_FILE, "ifconfig", "pwlan0", "down", NULL_STR);
#if defined(CONFIG_RTL_ULINKER_WLAN_DELAY_INIT)
	apmib_get(MIB_ULINKER_AUTO,  (void *)&ulinker_auto);
	if (wlan_support != 0 && ulinker_auto == 0)
#else
	if (wlan_support != 0)
#endif
	{
		memset(wlan_interface, 0x00, sizeof(wlan_interface));
		for(i=0;i<NUM_WLAN_INTERFACE;i++)
		{
			int wlan_disable = 1;
			int wlan_blockrelay=0;
			unsigned char wlan_name[10];
			int mc2u_disabled = 0;
			memset(wlan_name,0x00,sizeof(wlan_name));
			sprintf(wlan_name, "wlan%d",i);
			apmib_save_wlanIdx();
			if(SetWlan_idx(wlan_name))
			{			
				apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);	  
#ifdef SHRINK_INIT_TIME
				if(wlan_disable == 1 && reinit == 1)
#else
				if(wlan_disable == 1)
#endif
				{
					RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
					RunSystemCmd(NULL_FILE, "iwpriv", wlan_name, "radio_off", NULL_STR);					
				}
				else
				{
	        if(wlan_interface[0]==0x00)
	            sprintf(wlan_interface, "%s", wlan_name);
	      	else
	        {
						sprintf(tmp_args, " %s", wlan_name);
		        strcat(wlan_interface, tmp_args); 
	        }	
						
					//printf("%s:%d wlan_name=%s\n",__FUNCTION__,__LINE__,wlan_name);
#if defined(CONFIG_SMART_REPEATER)
#ifdef SHRINK_INIT_TIME
					if(reinit == 1)
					{
#endif
					char vxd_interface[16]={0};
					sprintf(vxd_interface,"%s-vxd",wlan_name);
//					RunSystemCmd(NULL_FILE, "ifconfig", vxd_interface, "0.0.0.0", NULL_STR);
					RunSystemCmd(NULL_FILE, "ifconfig", vxd_interface, "down", NULL_STR);
#ifdef SHRINK_INIT_TIME
					}
#endif
					//printf("%s:%d vxd_interface=%s\n",__FUNCTION__,__LINE__,vxd_interface);
#endif
#ifdef SHRINK_INIT_TIME
					if(reinit == 1)
#endif
						RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
					cmdRet=RunSystemCmd(NULL_FILE, "flash", "set_mib", wlan_name, NULL_STR);
			
					if(cmdRet != 0)
					{
						printf("[%s %d]init %s failed!\n", __FUNCTION__, __LINE__ ,wlan_name);
						continue;
					}
					apmib_get(MIB_WLAN_MC2U_DISABLED, (void *)&mc2u_disabled); 
					if(mc2u_disabled)
					{
						//iwpriv wlan0 set_mib mc2u_disable=1
						RunSystemCmd(NULL_FILE, "iwpriv", wlan_name,"set_mib","mc2u_disable=1", NULL_STR);
					}
					else
					{
						//iwpriv wlan0 set_mib mc2u_disable=0
						RunSystemCmd(NULL_FILE, "iwpriv", wlan_name,"set_mib","mc2u_disable=0", NULL_STR);
					}
				
				}
				
				apmib_get( MIB_WLAN_BLOCK_RELAY,(void *)&wlan_blockrelay);
				/*if all wlan interface block then enable br_wlan_block*/
				if(wlan_blockrelay)
				{
					br_wlan_block++;
				}

			}
			apmib_recov_wlanIdx();
		}
		
		if(br_wlan_block == NUM_WLAN_INTERFACE)
		{
			RunSystemCmd("/proc/br_wlanblock", "echo","1",NULL_STR);
		}
		else
		{
			RunSystemCmd("/proc/br_wlanblock", "echo","0",NULL_STR);
		}
	}


	if(wlan_interface[0]){				
		if(wlan_vxd_interface[0]) {
			char		*strptr, *tokptr;
			memcpy(tmpBuff,wlan_vxd_interface,sizeof(wlan_vxd_interface));
			strptr=tmpBuff;
			token = strsep(&strptr," ");
			while(token!=NULL)
			{
#ifdef SHRINK_INIT_TIME
				if(reinit == 1)
#endif
					RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
				RunSystemCmd(NULL_FILE, "flash", "set_mib", token, NULL_STR);/*set vxd wlan iface*/
				token = strsep(&strptr," ");
			}
		}
		if(wlan_virtual_interface[0]){
			token=NULL;
			savestr1=NULL;
			sprintf(tmpBuff, "%s", wlan_virtual_interface);
			token = strtok_r(tmpBuff," ", &savestr1);
			do{
				if (token == NULL){
					break;
				}else{
#ifdef SHRINK_INIT_TIME
					if(reinit == 1)
					{
#endif
					RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
					RunSystemCmd(NULL_FILE, "flash", "set_mib", token, NULL_STR);/*set virtual wlan iface*/
#ifdef SHRINK_INIT_TIME
					}
					else
					{
					
						apmib_save_wlanIdx();
						SetWlan_idx(token);
						
						apmib_get(MIB_WLAN_WLAN_DISABLED, &intValue);
						if(intValue == 0)
						{
							RunSystemCmd(NULL_FILE, "flash", "set_mib", token, NULL_STR);/*set virtual wlan iface*/
						}
						apmib_recov_wlanIdx();
						
					}
#endif
				}
				token = strtok_r(NULL, " ", &savestr1);
			}while(token !=NULL);
		}
#if defined(CONFIG_SMART_REPEATER)
		if(strstr(wan_interface,"-vxd")){
#ifdef SHRINK_INIT_TIME
			if(reinit == 1)
#endif
				RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "down", NULL_STR);
			RunSystemCmd(NULL_FILE, "flash", "set_mib", wan_interface, NULL_STR);
		}
#endif
	}	
	else
		RunSystemCmd("/proc/gpio", "echo", "0", NULL_STR);
#ifdef 	CONFIG_RTL_ULINKER
	if(reinit == 1){
		int pid_boa = find_pid_by_name("boa");
		if(pid_boa>0){
			kill(pid_boa,SIGUSR2);
		}
	}
#endif
	if(gateway==1){
		if(enable_br==1){
			/*init bridge interface*/
			//hyking:sure hw initialization first..
#if 0 //for enable VLAN reboot/hang issue sometimes
#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
			if(opmode == 0)
			{
				RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "up", NULL_STR);
			}
#else
			RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "up", NULL_STR);
#endif
#endif 

			
			set_br_interface(tmpBuff);
			
//fprintf(stderr,"\r\n br_interface=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);
			
			setbridge(tmpBuff);
#if defined(CONFIG_IPV6) && !defined(CONFIG_APP_APPLE_MFI_WAC)
			if (opmode == GATEWAY_MODE) {
				setbridgeipv6();
			}
#endif

#if 0
			//Not used at present in order to find the root cause!!!
			//Patch: wlan pc can't visit AP using wapi-psk or wapi-cert when AP boots up
			if(wlan_interface[0]){
				token=NULL;
				savestr1=NULL;
				sprintf(tmpBuff, "%s", wlan_interface);
				token = strtok_r(tmpBuff," ", &savestr1);
				do{
					if (token == NULL){
						break;
					}
					else
					{
						if(strncmp(token,"wlan",4)==0)
						{
							RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
							RunSystemCmd(NULL_FILE, "ifconfig", token, "up", NULL_STR);
						}
					}
					token = strtok_r(NULL, " ", &savestr1);
				}while(token !=NULL);
			}
#endif

			/* init log setting*/
			set_log();

			/* init lan dhcp setting*/
			if(lan_dhcp_mode==DHCP_LAN_NONE){		/*DHCP disabled*/
				apmib_get( MIB_IP_ADDR,  (void *)tmpBuff);
				sprintf(Ip, "%s", inet_ntoa(*((struct in_addr *)tmpBuff)));
				apmib_get( MIB_SUBNET_MASK,  (void *)tmpBuff);
				sprintf(Mask, "%s", inet_ntoa(*((struct in_addr *)tmpBuff)));
				apmib_get(MIB_DEFAULT_GATEWAY,  (void *)tmpBuff);
				
				if (!memcmp(tmpBuff, "\x0\x0\x0\x0", 4))
					memset(Gateway, 0x00, sizeof(Gateway));
				else
					sprintf(Gateway, "%s", inet_ntoa(*((struct in_addr *)tmpBuff)));
					
				RunSystemCmd(NULL_FILE, "ifconfig", br_interface, Ip, "netmask", Mask, NULL_STR);
				
				if(Gateway[0]){
					RunSystemCmd(NULL_FILE, "route", "del", "default", br_interface, NULL_STR);
					RunSystemCmd(NULL_FILE, "route", "add", "-net", "default", "gw", Gateway, "dev", br_interface, NULL_STR);
				}	
				start_wlanapp(v_wlan_app_enabled);
#if defined(CONFIG_APP_SIMPLE_CONFIG)
				system("echo 1 > /var/sc_ip_status");
#endif
			}else
				if(lan_dhcp_mode==DHCP_LAN_SERVER //dhcp disabled or server mode or auto
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT		
				|| lan_dhcp_mode==DHCP_AUTO
#endif
				)			
				{		/*DHCP server enabled*/
					intValue1=0;
					for(i=0;i<NUM_WLAN_INTERFACE;i++){
						sprintf(tmp_args, " wlan%d", i);
						if(SetWlan_idx(tmp_args)){
							apmib_get(MIB_WLAN_WDS_ENABLED, (void *)&intValue);
							//printf("wds enabled=%d\n",intValue);
							if(intValue!=0)
								intValue1=intValue1+5;
							else
								intValue1=intValue1+1;
						}	
					}
#ifndef SHRINK_INIT_TIME
					sleep(intValue1);/*wait wlan wds init */		
#endif
					/*start dhcp server*/
					set_lan_dhcpd(br_interface, 2);
					start_wlanapp(v_wlan_app_enabled);
				}
		}/*for init bridge interface and wlan app*/
#ifdef SHRINK_INIT_TIME
	if( opmode!=BRIDGE_MODE || reinit ==1 ){
#endif
		RunSystemCmd(NULL_FILE, "iptables", "-F", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-F", "-t", "nat",  NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A", "INPUT", "-j", "ACCEPT", NULL_STR);
		
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT		
		/* start dnrd for check dns query with hostname */
		domain_query_start_dnrd(wlan_mode_root, 1);
#endif		

		
		RunSystemCmd(NULL_FILE, "rm", "-f", "/var/eth1_ip", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/var/ntp_run", NULL_STR);
		//RunSystemCmd("/proc/pptp_src_ip", "echo", "0 0", NULL_STR);
		if(wan_interface[0])
		{
			if(opmode==WISP_MODE)
				RunSystemCmd(NULL_FILE, "iwpriv", wan_interface, "set_mib", "keep_rsnie=1", NULL_STR);
#ifdef SHRINK_INIT_TIME
				if( reinit==1 )
#endif
					RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "down", NULL_STR);
				RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "up", NULL_STR);
			}
#ifdef SHRINK_INIT_TIME
		}
#endif
		if(enable_wan==1 && (opmode == GATEWAY_MODE || opmode==WISP_MODE)){/*for init internet wan setting*/ 
			if(opmode==WISP_MODE)
			{
//printf("\r\n wan_interface=[%s],__[%s-%u]\r\n",wan_interface,__FILE__,__LINE__);				
				if(SetWlan_idx(wan_interface)){
					apmib_get(MIB_WLAN_ENCRYPT, (void *)&intValue);
					if(intValue != 0){
#if 0 //why wlan0 is wisp_wan interface and encrypt is not none, than wlan1's keep_rsnie will also be set as 1?????
						for(i=0;i<NUM_WLAN_INTERFACE;i++)
						{
							sprintf(tmp_args, " wlan%d", i);
							RunSystemCmd(NULL_FILE, "iwpriv", tmp_args, "set_mib", "keep_rsnie=1", NULL_STR);
						}
#else
						RunSystemCmd(NULL_FILE, "iwpriv", wan_interface, "set_mib", "keep_rsnie=1", NULL_STR);
#endif						
					}
				}
			}
			RunSystemCmd(PROC_FASTNAT_FILE, "echo", "1", NULL_STR);
			
			#if defined(CONFIG_RTL_FAST_IPV6)
			RunSystemCmd(PROC_FASTIPV6_FILE, "echo", "1", NULL_STR);
			#endif
			
			if(wan_dhcp_mode==PPTP){
				RunSystemCmd(PROC_FASTPPTP_FILE, "echo", "1", NULL_STR);
				apmib_get(MIB_PPTP_CONNECTION_TYPE, (void *)&intValue);
				if(intValue==1){
					RunSystemCmd(PROC_PPTP_CONN_FILE, "echo", "3", NULL_STR);
				}else{
					RunSystemCmd(PROC_PPTP_CONN_FILE, "echo", "0", NULL_STR);
				}
			}else{
				RunSystemCmd(PROC_FASTPPTP_FILE, "echo", "0", NULL_STR);
			}
				
			if(wan_dhcp_mode==L2TP){
				RunSystemCmd(PROC_FASTL2TP_FILE, "echo", "1", NULL_STR);
			}else{
				RunSystemCmd(PROC_FASTL2TP_FILE, "echo", "0", NULL_STR);
			}

			if(wan_dhcp_mode == PPPOE) {
				RunSystemCmd(PROC_FASTPPPOE_FILE, "echo", "1", NULL_STR);
			} else {
				RunSystemCmd(PROC_FASTPPPOE_FILE, "echo", "0", NULL_STR);
			}
			
	#ifdef HOME_GATEWAY		
	#if defined(MULTI_WAN_SUPPORT)
			{
				#define REINIT_EVENT_WAN_POWER_ON 3
				unsigned char command[64];
				sprintf(command,"reinitCli -e %d",REINIT_EVENT_WAN_POWER_ON);
				printf("%s.%d.start reinit wan power on event\n",
					__FUNCTION__,__LINE__);
				system(command);
			}
	#else
			if((wan_dhcp_mode !=DHCP_SERVER && wan_dhcp_mode < 7) || (wan_dhcp_mode == USB3G) || (wan_dhcp_mode == AFTR) || (wan_dhcp_mode == DHCP_NONE)){ /* */
				start_wan(wan_dhcp_mode, opmode, wan_interface, br_interface, wisp_wan_id, 1);
			}else
				printf("Invalid wan type:wan_dhcp_mode=%d\n", wan_dhcp_mode);
	#endif
	#endif			
		}
		else if(enable_wan==1 &&( opmode == BRIDGE_MODE)){
			/*Bridge mode, eth1 mtu should be sync with eth0 mtu (assume that eth0 mtu is 1500 here!!!)
			Otherwise, ping large pkt failed when GW mode changed to Bridge mode
			if eth1 mtu not equal with eth0 mtu.*/
			if(wan_interface[0])
				RunSystemCmd(NULL_FILE, "ifconfig", wan_interface, "mtu", "1500", NULL_STR);

			#if defined(CONFIG_RTL_FAST_IPV6)
			RunSystemCmd(PROC_FASTIPV6_FILE, "echo", "0", NULL_STR);
			#endif
		}

		//set kthreadd high priority for performance
		RunSystemCmd(NULL_FILE, "renice -20 2", NULL_STR);
		//set ksoftirqd high priority for performance
		RunSystemCmd(NULL_FILE, "renice -20 3", NULL_STR);
		//set boa high priority
		if(isFileExist(WEBS_PID_FILE)){
			pid=getPid_fromFile(WEBS_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "renice", "-20", strPID, NULL_STR);
			}
		}
	}
	else
	{ /*gateway is 0, it is ap mode*/
		set_br_interface(tmpBuff);
		setbridge(tmpBuff);

		//Not used at present in order to find the root cause!!!
		//Patch: wlan pc can't visit AP using wapi-psk or wapi-cert when AP boots up
		//RunSystemCmd(NULL_FILE, "ifconfig", wlan_interface, "down", NULL_STR);
		//RunSystemCmd(NULL_FILE, "ifconfig", wlan_interface, "up", NULL_STR);
			
		/* init log setting*/
		set_log();
		if(lan_dhcp_mode==DHCP_LAN_SERVER){	
#ifndef SHRINK_INIT_TIME
			sleep(1);
#endif
			//RunSystemCmd(NULL_FILE, "dhcpd.sh", br_interface, "ap", NULL_STR);
			set_lan_dhcpd(br_interface, 1);
		}	
		if(lan_dhcp_mode==DHCP_LAN_SERVER || lan_dhcp_mode==DHCP_LAN_NONE){	
			start_wlanapp(v_wlan_app_enabled);
		}	
#if defined(CONFIG_APP_APPLE_MFI_WAC)		
		// IOT dhcp client device need to give up br0 ip here , since br0 ip will be decide in IOT networking framework (dhcpc+avahi)
		// it is implement in wfaudio package 
		if(lan_dhcp_mode==DHCP_LAN_NONE || lan_dhcp_mode==DHCP_LAN_CLIENT)
		{
			//printf("give up br0 ip !!!\n");
			system("ifconfig br0 0.0.0.0");		
		}		
#endif	
	}

#ifndef STAND_ALONE_MINIUPNP
	start_upnpd(gateway, opmode);
#endif
	if(gateway==1 && opmode != BRIDGE_MODE){
		if(isFileExist(LLTD_PROCESS_FILE)){
			RunSystemCmd(NULL_FILE, "lld2d", br_interface, NULL_STR);
		}
	}
	if(isFileExist(SNMPD_PROCESS_FILE)){
		RunSystemCmd(NULL_FILE, "snmpd.sh", "restart", NULL_STR);
		//RunSystemCmd(NULL_FILE, "snmpd", "-c", SNMPD_CONF_FILE, "-p", SNMPD_PID_FILE,  NULL_STR);
	}
	if(isFileExist(NMSD_PROCESS_FILE)){
		RunSystemCmd(NULL_FILE, "nmsd", NULL_STR);
	}

#if defined(CONFIG_ANDLINK_IF5_SUPPORT) && defined(NEW_SCHEDULE_SUPPORT) && defined(TIMER_TASK_SUPPORT)
	andlink_setup_tasktimer();
#endif

	for(index=0; index<NUM_WLAN_INTERFACE; index++)
		start_wlan_by_schedule(index);

#if 0//defined(CONFIG_RTL_WAPI_SUPPORT)//Need more test!!!!
#if !defined(CONFIG_RTL_802_1X_CLIENT_SUPPORT)	// wlan0 down-> up will make 802.1x client mode auth fail when reinit or reboot
	system("ifconfig wlan0 down");
	system("ifconfig wlan0 up");
#endif
#endif

#if defined(CONFIG_IPV6) && !defined(CONFIG_APP_APPLE_MFI_WAC)
	system("ip tunnel del tun 2> /dev/null");
	set_wanipv6();
#endif

#ifdef HOME_GATEWAY // To enable/disable ipv6 passthru no matter wan is connected or not
	if(opmode == GATEWAY_MODE)	// Gateway mode
		apmib_get(MIB_CUSTOM_PASSTHRU_ENABLED, (void *)&intValue);
	else
		intValue=0;

	RunSystemCmd("/proc/custom_Passthru", "echo", (intValue & 0x1)?"1":"0", NULL_STR);
	if (intValue == 0)
	{
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", "peth0", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "peth0", "down", NULL_STR);
	}
	else
	{
		RunSystemCmd(NULL_FILE, "brctl", "addif", "br0", "peth0", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "peth0", "up", NULL_STR);
	}
#if defined(CONFIG_RTL_92D_SUPPORT)
	int wlan0_mode=0, wlan1_mode=0;
	if(opmode == WISP_MODE)
	{
		apmib_get(MIB_WISP_WAN_ID, (void *)&wispWanId);
		apmib_get(MIB_CUSTOM_PASSTHRU_ENABLED, (void *)&intValue);		
		apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enable1);
		apmib_get(MIB_REPEATER_ENABLED2,(void *)&repeater_enable2);
		SetWlan_idx("wlan0");
		apmib_get(MIB_WLAN_MODE, &wlan0_mode);
		SetWlan_idx("wlan1");
		apmib_get(MIB_WLAN_MODE, &wlan1_mode);
		if(intValue != 0)
		{
			char tmpStr[16];
			/*should also config wisp wlan index for dual band wireless interface*/
			intValue=((wispWanId&0xF)<<4)|intValue;
			if(((repeater_enable1==1)&&(wlan0_mode!=CLIENT_MODE)) || ((repeater_enable2==1)&&(wlan1_mode!=CLIENT_MODE)))
				intValue = intValue | 0x8;
			memset(tmpStr,0,sizeof(tmpStr));
			sprintf(tmpStr,"%d",intValue);
				
			RunSystemCmd("/proc/custom_Passthru_wlan", "echo", tmpStr, NULL_STR);
			//RunSystemCmd("/proc/custom_Passthru_wlan", "echo", (intValue & 0x1)?"1":"0", NULL_STR);
		}
		else
		{
			RunSystemCmd("/proc/custom_Passthru_wlan", "echo", (intValue & 0x1)?"1":"0", NULL_STR);
		}
	}
#else
	int wlan0_mode=0;
	if(opmode == WISP_MODE)	//WISP mode, wireless enabled  and wireless client mode enabled
	{		
		apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enable1);
		apmib_get(MIB_CUSTOM_PASSTHRU_ENABLED, (void *)&intValue);
		SetWlan_idx("wlan0");
		apmib_get(MIB_WLAN_MODE, &wlan0_mode);
		if(intValue !=0)
		{
			char tmpStr[16];
			if((repeater_enable1==1)&&(wlan0_mode!=CLIENT_MODE))
				intValue |= 0x8;
			memset(tmpStr,0,sizeof(tmpStr));
			sprintf(tmpStr,"%d",intValue);
			RunSystemCmd("/proc/custom_Passthru_wlan", "echo", tmpStr, NULL_STR);
		}
		else
		{
			RunSystemCmd("/proc/custom_Passthru_wlan", "echo", (intValue & 0x1)?"1":"0", NULL_STR);
		}
	}
	else
	{
	intValue=0;
	RunSystemCmd("/proc/custom_Passthru_wlan", "echo", (intValue & 0x1)?"1":"0", NULL_STR);
	}
#endif
	
	if (intValue == 0)
	{
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", "pwlan0", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "pwlan0", "down", NULL_STR);
	}
	else
	{
		RunSystemCmd(NULL_FILE, "brctl", "addif", "br0", "pwlan0", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "pwlan0", "up", NULL_STR);
	}
#endif



	/*enable igmp snooping*/
	/*igmp snooping is independent with igmp proxy*/
#if defined (CONFIG_IGMPV3_SUPPORT)
	RunSystemCmd(PROC_BR_IGMPVERSION, "echo", "3", NULL_STR);
#else
	RunSystemCmd(PROC_BR_IGMPVERSION, "echo", "2", NULL_STR);
#endif
	RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "1", NULL_STR);
	//RunSystemCmd(PROC_BR_IGMPQUERY, "echo", "1", NULL_STR);
#if defined (CONFIG_RTL_MLD_SNOOPING)	
	RunSystemCmd(PROC_BR_MLDSNOOP, "echo", "1", NULL_STR);
	//default disable mldquery. mld query packets may leads ping and throughput down periodically???
	//RunSystemCmd(PROC_BR_MLDQUERY, "echo", "1", NULL_STR);
#endif

	intValue =0;
	apmib_get(MIB_IGMP_FAST_LEAVE_DISABLED, (void *)&intValue);
	if(intValue)
	{
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "fastleave","0","2", NULL_STR);
	}
	else
	{	
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "fastleave","1","0", NULL_STR);
	}
	
#ifdef SUPPORT_ZIONCOM_RUSSIA
	RunSystemCmd("/proc/sys/net/ipv4/conf/eth1/force_igmp_version", "echo", "2", NULL_STR);
#endif
	//RunSystemCmd("/proc/sys/net/ipv4/ip_conntrack_max", "echo", "2048", NULL_STR);

#ifdef CONFIG_RTL_HIGH_PERFORMANCE_FILESYSTEM
	load_rtl_fs_module();
#endif
	
#if defined(CONFIG_APP_USBMOUNT)
	start_mount();

#if defined (CONFIG_APP_VSFTPD)
	start_vsftpd();
#endif	

#if defined (CONFIG_APP_SAMBA)
	start_samba();
#endif

#ifdef CONFIG_APP_MINIDLNA
	start_lan_minidlna();
#endif

#endif


#if defined(CONFIG_APP_DLNA_DMS)
	//printf("%s, %d\n", __FUNCTION__, __LINE__);
	system("ushare -f /etc/ushare.conf &");
#endif
#ifdef RTK_USB3G
    apmib_get(MIB_WAN_DHCP, (void*)&intValue);
	if (opmode == 0 && (intValue == USB3G
#ifdef CONFIG_4G_LTE_SUPPORT 
	|| lte_wan()
#endif
	)) {
        system("echo \"/sbin/hotplug\" > /proc/sys/kernel/hotplug");
        system("mount -t sysfs none /sys           >/dev/null 2>&1");
    #ifdef KERNEL_3_10
        //system("mount -t devtmpfs none /dev           >/dev/null 2>&1");
        system("mount -t debugfs none /sys/kernel/debug >/dev/null 2>&1");
    #else
        system("mount -t usbfs none /proc/bus/usb  >/dev/null 2>&1");
        system("mount -t tmpfs none /dev           >/dev/null 2>&1");
    #endif
        system("mdev -s                            >/dev/null 2>&1");
        system("echo \"remove\" > /var/usb3g.stat");
        system("mnet -d &");
    }
#endif /* #ifdef RTK_USB3G */

#if defined(HTTP_FILE_SERVER_SUPPORTED)
		RunSystemCmd("/proc/http_file/getLanIp", "echo", "1", NULL_STR);
#endif

	#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_APP_APPLE_MFI_WAC)
		system("rm -f  /var/system/start_init 2> /dev/null");
	#endif
	
#ifdef CONFIG_APP_APPLE_MFI_WAC
	if(isFileExist("/var/system/mdnsd_started")==0){
		if(lan_dhcp_mode==DHCP_LAN_NONE || lan_dhcp_mode==DHCP_LAN_CLIENT)
		{
			system("ifconfig br0 0.0.0.0");		
		}		
		system("mdnsd");
		system("echo 1 > /var/system/mdnsd_started");
	}
	system ("wfaudio &"); //some brach start in rcs
//#if defined(CONFIG_APPLE_HOMEKIT) // move to wfaudio.c
//	system("hapserver 1 0 0 &");
//#endif
#endif

#if defined(CONFIG_APP_FWD)
	//##For fwd
		//system("mount -t tmpfs none /sbin"); //put fwd to ram
		//system("cp /bin/fwd /sbin"); //put fwd to ram
#ifdef SHRINK_INIT_TIME
		if(reinit == 0)
#endif
		system("fwd &");
#endif
	//reply only if the target IP address is local address configured on the incoming interface
	RunSystemCmd("/proc/sys/net/ipv4/conf/eth1/arp_ignore", "echo", "1", NULL_STR);
	/*increase routing cache rebuild count from 4 to 2048*/
	RunSystemCmd(RT_CACHE_REBUILD_COUNT, "echo", "2048", NULL_STR);
#ifdef SHRINK_INIT_TIME
	if(opmode != BRIDGE_MODE)
#endif
	system("timelycheck &");
#if defined(CONFIG_AUTO_DHCP_CHECK)
	if((opmode==BRIDGE_MODE) && lan_dhcp_mode == DHCP_SERVER)
	{
		system("Auto_DHCP_Check &");
	}
#endif

#ifdef CONFIG_CPU_UTILIZATION
	int enable_cpu=0;
	apmib_get(MIB_ENABLE_CPU_UTILIZATION, (void *)&enable_cpu);
	if(enable_cpu)
	{
		//RunSystemCmd(NULL_FILE, "killall", "cpu", NULL_STR);
		if(isFileExist(CPU_PID_FILE)==0)
		{
			RunSystemCmd(NULL_FILE, "cpu", NULL_STR);		
		}
	}
	else
	{
		if(isFileExist(CPU_PID_FILE)!=0)
		{
			RunSystemCmd(NULL_FILE, "killall", "cpu", NULL_STR);
//			RunSystemCmd(NULL_FILE, "rm", "-f", "/web/*.cpudat", NULL_STR);
			system("rm -f  "CPU_PID_FILE" 2> /dev/null");
		}
	}
#endif

#if defined(CONFIG_APP_DROPBEAR_SSH)
    if(isFileExist("/var/run/dropbear.pid")==0)
    {
        system("mount devpts /dev/pts -t devpts");
        system("/bin/dropbear -p 22"); 
    }
#endif

#if defined(CONFIG_APP_TR069)
	if (find_pid_by_name("cwmpClient") == 0)
		start_tr069();
#endif

#if defined(CONFIG_APP_RTK_INBAND_CTL)
	//RunSystemCmd(PROC_INBAND_CTL_ACL, "echo", "0x8899", NULL_STR);
	system("hcd -daemon &");
#endif

#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
	init_EthDot1x(wan_dhcp_mode, wan_dhcp_mode, wan_interface, br_interface);
#endif

#ifdef RTK_CAPWAP
	capwap_app();
#endif

#if defined(CONFIG_APP_RTK_INBAND_CTL)
	RunSystemCmd(PROC_INBAND_CTL_ACL, "echo", "0x8899", NULL_STR);
	system("hcd -daemon &");
#endif

#if defined(CONFIG_SYSFS)
	rtl_mount_sysfs();
#endif

#if defined(CONFIG_RPS)
	rtl_configRps();
#endif
#if defined(CONFIG_APP_ZIGBEE)
        printf("start zigbeed\n");
        system("zigbeed &");
        //system("zigbeed permit_join");
#endif
#if defined(CONFIG_APP_BT_REPEATER_CONFIG)
	if(!reinit)
	{
		gen_rtlbt_fw_config();
	}
	if(!reinit)
	{
		char btConfigPin[16] = {0};
		int btPartErased = 0;
		char dev_name[64] = {0};
		apmib_get(MIB_BT_PARTITION_ERASED,(void*)&btPartErased);
		char tmpCmd[128] ={0};
#ifdef MTD_NAME_MAPPING
		char bluetoothMtd[16];
		char btMtdDev[32] = {0};
#endif
		pid_t pid = -1;
		char cmd[100],tmpStr[16];	
		system("mkdir -p /var/lib");
		system("mkdir -p /var/lib/bluetooth");
#ifdef MTD_NAME_MAPPING
		rtl_name_to_mtdblock("bluetooth",bluetoothMtd);
		if(btPartErased == 0)
		{
			rtl_name_to_mtdchar("bluetooth",btMtdDev);
			sprintf(tmpCmd,"flash_erase -j %s 0 0",btMtdDev);
			system(tmpCmd);
			sleep(1);
			btPartErased = 1;
			apmib_set(MIB_BT_PARTITION_ERASED,(void*)&btPartErased);
			apmib_update(CURRENT_SETTING);
		}
		sprintf(tmpCmd,"mount -t jffs2 %s /var/lib/bluetooth/",bluetoothMtd);
		system(tmpCmd);
#else
		if(btPartErased == 0)
		{
			system("flash_erase -j /dev/mtd2 0 0");
			sleep(1);
			btPartErased = 1;
			apmib_set(MIB_BT_PARTITION_ERASED,(void*)&btPartErased);
			apmib_update(CURRENT_SETTING);
		}
		system("mount -t jffs2 /dev/mtdblock2 /var/lib/bluetooth/");
#endif
		system("mkdir -p /var/local");
		system("mkdir -p /var/dbus-1");
		system("mkdir -p /var/local/etc");
		system("mkdir -p /var/local/lib");
		system("mkdir -p /var/local/etc/dbus-1");
		system("mkdir -p /var/local/etc/bluetooth");
		system("mkdir -p /var/local/etc/dbus-1/system.d");
		system("cp /etc/system.conf /usr/local/etc/dbus-1/");
		system("cp /etc/bluetooth.conf /usr/local/etc/dbus-1/system.d");
		system("cp /etc/main.conf /usr/local/etc/bluetooth/");
		gen_rtk_bt_dev_name(dev_name);
		system("mkdir -p /var/local/var");
		system("mkdir -p /var/local/var/run");
		system("mkdir -p /var/local/var/run/dbus");
		system("mkdir -p /etc/dbus-1/system.d");
		system("cp /etc/session.conf /etc/dbus-1/");
		system("cp /etc/system.conf /etc/dbus-1/");
		system("cp /etc/bluetooth.conf /etc/dbus-1/system.d");
		system("mkdir -p /var/bluetooth");
		system("dbus-daemon --system");
		system("bluetoothd -n -d -C &");
		sleep(4);
		system("echo /bin/mdev > /proc/sys/kernel/hotplug");
		system("mount -t sysfs sysfs /sys");
		system("echo 1 > /sys/class/firmware/timeout");
#if defined(CONFIG_SERIAL_BLUETOOTH)
		system("hciattach -n -s 115200 /dev/ttyS1 rtk_h5 &");
		sleep(6);
#endif
		system("hciconfig hci0 up");
		sleep(2);
        printf("---------------------------------------------------------------------\n");
		sprintf(tmpCmd,"hciconfig hci0 name %s",dev_name);
		system(tmpCmd);
		system("hciconfig hci0 inqparms 18:1024");
		system("hciconfig hci0 pageparms 18:1024");
		system("hciconfig hci0 piscan");
		sleep(1);
        printf("---------------------------------------------------------------------\n");
		system(" sdptool add --channel=1 SP");
		system("start_agent.sh");
        printf("---------------------------------------------------------------------\n");
		sleep(1);
		system("rtk_bt_repeater_config &");
		system("rtk_ble_repeater_config &");
	}
#endif/*CONFIG_APP_BT_REPEATER_CONFIG*/
#if defined(CONFIG_APP_WEAVE)
        printf("start dbus-daemon\n");
        system("dbus-daemon --system");
        printf("start avahi-daemon\n");
        system("avahi-daemon &");
        sleep(3);
        int weave_registered = 0;
        apmib_get(MIB_WEAVE_REGISTERED,(void *)&weave_registered);
        if(weave_registered){
            unsigned char weave_config[MAX_WEAVE_CONFIG_LENGTH]={0};
            FILE * fp=NULL;
            printf("start weave-daemon\n");
            system("mkdir /var/lib/weave 2>/dev/null");
            fp=fopen(WEAVE_FILE_PATH,"w+");
            if(!fp)
            {
                printf("open %s fail!\n",WEAVE_FILE_PATH);
                return;
            }
            apmib_get(MIB_WEAVE_CONFIG,weave_config);

            fwrite(weave_config,strlen(weave_config),1,fp);
            fclose(fp);
            sync();
        }
        system("weave_daemon &");
#endif  
#if defined(CONFIG_APP_USBMOUNT)
	rtl_config_read_ahead();
#endif

	if(isFileExist(REINIT_FILE)==0)
		RunSystemCmd(REINIT_FILE, "echo", "1", NULL_STR);
#ifdef CONFIG_RTK_EVENTD_SUPPORT
	if(isFileExist("/var/run/rtk_eventd.pid"))
		system("killall rtk_eventd > /dev/null 2>&1");
		
	system("rtk_eventd");
#endif
#if defined(CONFIG_APP_TR069) && defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
	int cap_enable = 0;
	char cap_url[2000];
	apmib_get(MIB_CAPTIVEPORTAL_ENABLE,(void *)&cap_enable);
	sprintf(cmdBuffer,"echo %d > /proc/http_redirect/enable",cap_enable);
	system(cmdBuffer);
	apmib_get(MIB_CAPTIVEPORTAL_URL,(void *)cap_url);
	sprintf(cmdBuffer,"echo %s > /proc/http_redirect/CaptivePortalURL",cap_url);
	system(cmdBuffer);
	apmib_get(MIB_CAP_PORTAL_ALLOW_TBL_NUM, (void *)&capEntryNum);
	system("echo flush > /proc/http_redirect/AllowedList");
	for (i=1; i<=capEntryNum; i++) 
	{
		*((char *)&capEntry) = (char)i;
		if(!apmib_get(MIB_CAP_PORTAL_ALLOW_TBL, (void *)&capEntry))
		{
			printf("get mib MIB_CAP_PORTAL_ALLOW_TBL fail!\n");
			return 0;
		}
		sprintf(cmdBuffer,"echo add %s > /proc/http_redirect/AllowedList",capEntry.ipAddr);
		system(cmdBuffer);
	}	
#endif
#if 0 //def CONFIG_APP_OPENVPN
		int openvpn_enabled=0;
		
		apmib_get(MIB_OPENVPN_ENABLED, (void *)&openvpn_enabled);
		
		if(find_pid_by_name("openvpn")>0)
			system("killall openvpn > /dev/null 2>&1");
		
		if(openvpn_enabled)
			start_openvpn();	
#endif
#ifdef CONFIG_SET_PHY_PORT_POWER_DOWN
	set_physical_port_power_down();
#endif

	//disable multicast stormCtrl
	//printf("[%s:%d]\n", __FUNCTION__, __LINE__);
	//RunSystemCmd("/proc/StormCtrl", "echo", "200 3", NULL_STR);

#ifdef RTL_TRAP_BR_SUBNET_PKT_FROM_WAN_TO_CPU
	system("echo 1 > /proc/br_subnet_from_wan_to_cpu");
#endif
	restartAndmuProcesses();
#if defined(CONFIG_ANDLINK_SUPPORT)
	cleanAndlink();
#endif

	//Set default domain as China(13)
	system("flash set HW_WLAN0_REG_DOMAIN 13");
#if defined (CONFIG_MODEL_HN120_341FA) || defined (CONFIG_MODEL_HN120_341BA)  ||  defined (CONFIG_MODEL_HN140_361EA)	
	system("flash set HW_WLAN1_REG_DOMAIN 13");
#endif


	return 0;
}

#if defined (CONFIG_ELINK_SUPPORT) || defined (CONFIG_ANDLINK_SUPPORT) || defined(CONFIG_APP_CMCC_ADAPTER)  
#define RTL_LINK_RESTART_BLOCK 		"/var/rtl_link_restart_block"
#define RTL_LINK_FILE_TIMER_TASK	"/var/wlsch.conf.task"

static int rtl_link_get_channel(char *ifname)
{
	unsigned char root_ifname[10] = {0}, filename[50] = {0};
	FILE *fp;
	char line_buffer[256] = {0};
	char *pchar;
	int channel;
	unsigned int rtl_link_mode = 0;

	apmib_get(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
	if(rtl_link_mode != 1) // andlink repeater mode
		return;

	snprintf(root_ifname, sizeof(root_ifname), "%s", ifname);	
	root_ifname[5] = '\0';
	snprintf(filename, sizeof(filename), "proc/%s/mib_rf", root_ifname);
	if ((fp = fopen(filename, "r")) != NULL) {
			while(fgets(line_buffer, sizeof(line_buffer), fp))
		    {
		        line_buffer[strlen(line_buffer)-1]='\0';
		        
		        if((pchar=strstr(line_buffer, "dot11channel:"))!=NULL)
		        {		        	
		            pchar+=strlen("dot11channel:");
		            channel = atoi(pchar);
					break;
		        } 	      
		    }
			fclose(fp);			
		}else{
			channel = 1; // just care about 2G
		} 

		return channel;		
}

static void rtl_link_set_channel(char *ifname, int channel)
{
	unsigned int rtl_link_mode = 0, phyband=0, getValue = 0;
	unsigned char cmd[50] = {0}, root_ifname[10] = {0};	

	apmib_get(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
	if(rtl_link_mode != 1) // andlink repeater mode
		return;

	apmib_save_wlanIdx();	

	snprintf(root_ifname, sizeof(root_ifname), "%s", ifname);
	root_ifname[5] = '\0';	
	SetWlan_idx(root_ifname);	
	apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyband);
	apmib_get(MIB_WLAN_CHANNEL, (void *)&getValue);
	if(phyband == PHYBAND_2G && getValue==0){ 		
		snprintf(cmd, sizeof(cmd), "iwpriv %s set_mib channel=%d", ifname, channel);		
		system(cmd);
	}

	apmib_recov_wlanIdx();
	return;	
}

void rtl_link_setinit(int idx)
{
    int i=0, j=0, rootap_disabled=0, vap_disabled=0, opmode, wisp_idx;
    char tmpbuff[16],tmpBuff[512],tmpbuff1[16], tmpbuff2[512];
    char *token=NULL, *savestr1=NULL;
    //FILE *fp=NULL;
#ifdef CONFIG_SMART_REPEATER
 	int repeater_enabled;
#endif
    char led_state[16];
	int channel = 0; 
	int rtl_link_mode = 0;
	int index;

    num_wlan_virtual_interface=if_readlist_proc(wlan_virtual_interface, "va", 0);

    apmib_get(MIB_OP_MODE, (void *)&opmode);
    apmib_get(MIB_WISP_WAN_ID, (void *)&wisp_idx);

    printf("===> entry %s\n", __FUNCTION__);
    //fp = fopen(RTL_LINK_RESTART_BLOCK, "w+");
	//if(!fp)
		//return;

    // configure wifi settings, and update bridge wlan interface
    apmib_save_wlanIdx();    
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
		memset(tmpbuff,0x00,sizeof(tmpbuff));
		sprintf(tmpbuff, "wlan%d",i);
        SetWlan_idx(tmpbuff); 
        apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&rootap_disabled);
#ifdef CONFIG_SMART_REPEATER
		strcpy(tmpbuff1, tmpbuff);
		strcat(tmpbuff1, "-vxd");
#endif
        apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&rootap_disabled);
        if(0x1<<(i*(NUM_VWLAN_INTERFACE+1)) & idx){
        //if(0x3F<<(i*(NUM_VWLAN_INTERFACE+1))&idx){
            printf("restart wlan%d rootap\n", wlan_idx);
            //need to restart rootap
#ifdef CONFIG_SMART_REPEATER
            //down repeater
			RunSystemCmd(NULL_FILE, "ifconfig", tmpbuff1, "down", NULL_STR);
		    //if(opmode != WISP_MODE || (opmode == WISP_MODE && wisp_idx != i))
			RunSystemCmd(NULL_FILE, "brctl", "delif", "br0" ,tmpbuff1, NULL_STR);
#endif
            //down vap
            if(wlan_virtual_interface[0]){
                token=NULL;
		        savestr1=NULL;
		        sprintf(tmpbuff2, "%s", wlan_virtual_interface);
		        token = strtok_r(tmpbuff2," ", &savestr1);
		        do{
			        if (token == NULL){
				        break;
			        }else{
			            if(strstr(token, tmpbuff)){
			                RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
			                RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", token, NULL_STR);
			            }
			        }
			        token = strtok_r(NULL," ", &savestr1);
			    }while(token !=NULL);
			}
			//down rootap
			RunSystemCmd(NULL_FILE, "ifconfig", tmpbuff, "down", NULL_STR);
			RunSystemCmd(NULL_FILE, "brctl", "delif", "br0" ,tmpbuff, NULL_STR);
		    RunSystemCmd(NULL_FILE, "iwpriv", tmpbuff, "radio_off", NULL_STR);

            if(!rootap_disabled){
                //up rootap
                channel = rtl_link_get_channel(tmpbuff);
                RunSystemCmd(NULL_FILE, "flash", "set_mib", tmpbuff, NULL_STR);	
				rtl_link_set_channel(tmpbuff, channel);
                RunSystemCmd(NULL_FILE, "ifconfig", tmpbuff, "0.0.0.0", NULL_STR);
                RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,tmpbuff, NULL_STR);
                //up vap
                if(wlan_virtual_interface[0]){
                    token=NULL;
    		        savestr1=NULL;
    		        sprintf(tmpbuff2, "%s", wlan_virtual_interface);
    		        token = strtok_r(tmpbuff2," ", &savestr1);
    		        do{
    			        if (token == NULL){
    				        break;
    			        }else{
    			            if(strstr(token, tmpbuff)){
        			            SetWlan_idx(token);
        						apmib_get(MIB_WLAN_WLAN_DISABLED, &vap_disabled);
        						if(!vap_disabled){
									channel = rtl_link_get_channel(token);
                                    RunSystemCmd(NULL_FILE, "flash", "set_mib", token, NULL_STR);/*set virtual wlan iface*/
									rtl_link_set_channel(token, channel);
                                    RunSystemCmd(NULL_FILE, "ifconfig", token, "0.0.0.0", NULL_STR);
                                    RunSystemCmd(NULL_FILE, "brctl", "addif", "br0", token, NULL_STR);
        						}
    						}
    			        }
    			        token = strtok_r(NULL," ", &savestr1);
    			    }while(token !=NULL);
    			}
    			//up repeater
#ifdef CONFIG_SMART_REPEATER
                if (i == 0)
                    apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enabled);
                else
                    apmib_get(MIB_REPEATER_ENABLED2,(void *)&repeater_enabled);
                if (repeater_enabled){
					channel = rtl_link_get_channel(tmpbuff1);
          		    RunSystemCmd(NULL_FILE, "flash", "set_mib", tmpbuff1, NULL_STR);
					rtl_link_set_channel(tmpbuff1, channel);
          		    RunSystemCmd(NULL_FILE, "ifconfig", tmpbuff1, "0.0.0.0", NULL_STR);
          		    //if(opmode != WISP_MODE || (opmode == WISP_MODE && wisp_idx != i))
          		    RunSystemCmd(NULL_FILE, "brctl", "addif", "br0", tmpbuff1, NULL_STR);
          		}
#endif
            }
        }else{
            //restart vap or vxd only
            if(!rootap_disabled){
                RunSystemCmd(NULL_FILE, "iwpriv", tmpbuff, "set_mib", "vap_enable=1", NULL_STR);
                //restart vap
                if(wlan_virtual_interface[0]){
                    token=NULL;
    		        savestr1=NULL;
    		        sprintf(tmpbuff2, "%s", wlan_virtual_interface);
    		        token = strtok_r(tmpbuff2," ", &savestr1);
    		        do{
    			        if (token == NULL){
    				        break;
    			        }else{
    			            if(strstr(token, tmpbuff)){
        			            SetWlan_idx(token);
        			            if(0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx) & idx){
        			                printf("restart %s\n", token);
        			                
        			                RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
    			                    RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", token, NULL_STR);
    			                    apmib_get(MIB_WLAN_WLAN_DISABLED, &vap_disabled);
            						if(!vap_disabled){
										channel = rtl_link_get_channel(token);
                                        RunSystemCmd(NULL_FILE, "flash", "set_mib", token, NULL_STR);/*set virtual wlan iface*/
										rtl_link_set_channel(token, channel);
                                        RunSystemCmd(NULL_FILE, "ifconfig", token, "0.0.0.0", NULL_STR);
                                        RunSystemCmd(NULL_FILE, "brctl", "addif", "br0", token, NULL_STR);
            						}
        			            }
    			            }
    			        }
    			        token = strtok_r(NULL," ", &savestr1);
    			    }while(token !=NULL);
    			}
                //restart repeater
                if(0x1<<(i*(NUM_VWLAN_INTERFACE+1) + NUM_VWLAN_INTERFACE) & idx){
                    SetWlan_idx(tmpbuff1);
                    printf("restart %s\n", tmpbuff1);
	                RunSystemCmd(NULL_FILE, "ifconfig", tmpbuff1, "down", NULL_STR);
        		    //if(opmode != WISP_MODE || (opmode == WISP_MODE && wisp_idx != i))
        			RunSystemCmd(NULL_FILE, "brctl", "delif", "br0" ,tmpbuff1, NULL_STR);
                    if (i == 0)
                        apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enabled);
                    else
                        apmib_get(MIB_REPEATER_ENABLED2,(void *)&repeater_enabled);
					if(repeater_enabled){
						channel = rtl_link_get_channel(tmpbuff1);
                        RunSystemCmd(NULL_FILE, "flash", "set_mib", tmpbuff1, NULL_STR);
                        RunSystemCmd(NULL_FILE, "iwpriv", tmpbuff1, "set_mib", "wsc_enable=0", NULL_STR);   //after set_mib or wsc_enable may copy from root again
                        rtl_link_set_channel(tmpbuff1, channel);
              		    RunSystemCmd(NULL_FILE, "ifconfig", tmpbuff1, "0.0.0.0", NULL_STR);
              		    //if(opmode != WISP_MODE || (opmode == WISP_MODE && wisp_idx != i))
              		    RunSystemCmd(NULL_FILE, "brctl", "addif", "br0", tmpbuff1, NULL_STR);
					}
                }
                
            }
        }
    }
    apmib_recov_wlanIdx();

	/* shrink restart time for andlink repeater */
	apmib_get(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
	if(rtl_link_mode==1){
		if(isFileExist(RTL_LINK_RESTART_BLOCK)){ 		
	   		unlink(RTL_LINK_RESTART_BLOCK);
		}   
	} 
    
    printf("<=== exit %s\n", __FUNCTION__);
    //fclose(fp);

    //sync led state when restart wifi
    if(isFileExist("/var/rtl_link_led")){
        getDataFormFile("/var/rtl_link_led", "led_state", led_state, 0);
        if(!strcmp(led_state, "on")){
            for(i=0; i<NUM_WLAN_INTERFACE; i++){
                sprintf(tmpBuff, "echo 1 > proc/wlan%d/led", i);
                system(tmpBuff);
            }
        }else{
            for(i=0; i<NUM_WLAN_INTERFACE; i++){
                sprintf(tmpBuff, "echo 0 > proc/wlan%d/led", i);
                system(tmpBuff);
            }
        }
    }
	
#if defined(CONFIG_ANDLINK_IF5_SUPPORT) && defined(NEW_SCHEDULE_SUPPORT) && defined(TIMER_TASK_SUPPORT)
	if((0x1<<(NUM_WLAN_INTERFACE*(NUM_VWLAN_INTERFACE+1))) & idx){
		printf("restart reload...\n");
	
		//kill reload
		system("killall reload > /dev/null 2>&1");
		if(isFileExist(RTL_LINK_FILE_TIMER_TASK)){ 		
	   		unlink(RTL_LINK_FILE_TIMER_TASK);
		} 

		//restart reload
		andlink_setup_tasktimer();
		for(index=0; index<NUM_WLAN_INTERFACE; index++)
			start_wlan_by_schedule(index);
	}
#endif
}
#endif

void Init_Internet(int argc, char** argv)
{
#ifdef   HOME_GATEWAY	
	int wisp_id=0, wan_mode=0, opmode=0;
	char br_interface[16]={0};
	char wan_interface[16]={0};
	char tmp_args[16]={0};
	
	
	if(argc < 4)
		return;
		
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if(opmode == BRIDGE_MODE)
		return;
	
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_id);
	
	sprintf(tmp_args, "%s",argv[1]);
	if(!strcmp(tmp_args, "pppoe"))
		wan_mode=3;
	else if(!strcmp(tmp_args, "pptp"))
		wan_mode=4;	
	else if(!strcmp(tmp_args, "l2tp"))
		wan_mode=6;
	else{
		printf("Un-support wan type for init\n");
		return;
	}
	sprintf(br_interface, "%s", "br0");
	sprintf(wan_interface, "%s",argv[3]);
	start_wan(wan_mode, opmode, wan_interface, br_interface, wisp_id, 0);		
#endif	
}

#if 0
#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
int modifyHwIpv6Status()
{	
	int opmode=-1;
	int qosFlag = 0;
	int QoS_Rule_EntryNum=0;
	IPQOS_T entry;
	int index=1;

	if(apmib_get(MIB_OP_MODE, (void *)&opmode) == 0) {
		printf("Get op mode error! \n");
		return -1;
	}

	if ( apmib_get( MIB_QOS_ENABLED, (void *)&qosFlag) == 0) {
		printf("Get enabled flag error! \n");
		return -1;
	}

	if(opmode == GATEWAY_MODE){
		
		if ( apmib_get( MIB_QOS_ENABLED, (void *)&qosFlag) == 0) {
			printf("Get enabled flag error! \n");
			return -1;
		}

		if(qosFlag){
			#ifdef CONFIG_RTL_HW_QOS_SUPPORT
			apmib_get( MIB_QOS_RULE_TBL_NUM, (void *)&QoS_Rule_EntryNum);

			if(QoS_Rule_EntryNum > 0){
				for (index=1; index<=QoS_Rule_EntryNum; index++) {
					*((char *)&entry) = (char)index;
					apmib_get(MIB_QOS_RULE_TBL, (void *)&entry);
					if(strcmp(entry.ip6_src,"") != 0)
						break;
				}
				if(index == (QoS_Rule_EntryNum + 1))
					qosFlag = 0;
			}
			else
				qosFlag = 0;
			#endif
		}

		if(qosFlag)
			//system("echo 0 >/proc/hwL3v6");
			RunSystemCmd(PROC_HWIPV6_ROUTING_FILE, "echo", "by_ipv6_route 0", NULL_STR);
		else
			//system("echo 1 >/proc/hwL3v6");
			RunSystemCmd(PROC_HWIPV6_ROUTING_FILE, "echo", "by_ipv6_route 1", NULL_STR);
	}
	else if ((opmode==BRIDGE_MODE) ||(opmode==WISP_MODE)){
		//system("echo 0 >/proc/hwL3v6");
		RunSystemCmd(PROC_HWIPV6_ROUTING_FILE, "echo", "by_ipv6_route 0", NULL_STR);
	}
}
#endif
#endif

void Init_QoS(int argc, char** argv)
{
#ifdef   HOME_GATEWAY	
	int wisp_id=0, wan_mode=0, opmode=0, qosFlag=0;;

	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if(opmode == 1)
		return;
		
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_id);
	apmib_get(MIB_WAN_DHCP,(void *)&wan_mode);		

#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	//modifyHwIpv6Status();
	update_hw_ipv6_status();
#endif

#if defined(QOS_OF_TR069)	
	apmib_get(MIB_QOS_ENABLE,(void *)&qosFlag); 
		if(qosFlag) 	// enable tr098 qos will make webpage qos invalid
			return;
#endif	
	set_QoS(opmode, wan_mode, wisp_id);
#endif	
	
} 
 
#ifdef CONFIG_SMART_REPEATER
int getWispRptIfaceName(char*pIface,int wlanId)
{
	int rptEnabled=0,wlanMode=0,opMode=0;
	char wlan_wanIfName[16]={0};
	if(wlanId == 0)
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	else if(1 == wlanId)
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
	else return -1;
	apmib_get(MIB_OP_MODE,(void *)&opMode);
	if(opMode!=WISP_MODE)
		return -1;
	apmib_save_wlanIdx();

	sprintf(wlan_wanIfName,"wlan%d",wlanId);
	SetWlan_idx(wlan_wanIfName);
	//for wisp rpt mode,only care root ap
	apmib_get(MIB_WLAN_MODE, (void *)&wlanMode);
	if((AP_MODE==wlanMode || AP_WDS_MODE==wlanMode || AP_MESH_MODE==wlanMode || MESH_MODE==wlanMode ) && rptEnabled)
	{//root AP mode and rpt enabled, use -vxd as wanIf
		if(!strstr(pIface,"-vxd"))
		{
			strcat(pIface,"-vxd");
		}
	}else
	{
		char * ptmp = strstr(pIface,"-vxd");
		if(ptmp)
			memset(ptmp,0,sizeof(char)*strlen("-vxd"));
	}
	apmib_recov_wlanIdx();
	return 0;
}
#endif
 
 
 
 
 
