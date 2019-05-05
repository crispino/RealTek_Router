#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <netdb.h>
#include "boa.h"
#include "asp_page.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"

#ifdef CONFIG_APP_FWD
#define FWD_CONF "/var/fwd.conf"
#endif
#define WLAN_DEBUG(fmt, args...)\
	do{\
		if(0)\
			printf("%s %d:"fmt,__FUNCTION__ , __LINE__ , ## args);\
	}while(0)	
#define  UPDATE_IMAGE_FILE "/var/tmp/fw.bin"

#if (NUM_WLAN_INTERFACE==2)
static SS_STATUS_Tp pStatus[NUM_WLAN_INTERFACE]={NULL, NULL}; // [wlan0, wlan1]
#else
static SS_STATUS_Tp pStatus[NUM_WLAN_INTERFACE]={NULL}; // [wlan0]
#endif

/*Adjust the phy port number according to Hardware*/
#define LAN0_PHY_PORT 0
#define LAN1_PHY_PORT 1
#define LAN2_PHY_PORT 2
#define LAN3_PHY_PORT 3

#define VLAN_TYPE_NAT 0
#define VLAN_TYPE_BRIDGE 1
#define DEFAULT_NAT_LAN_VID 9
#define DEFAULT_NAT_WAN_VID 8
#define DEFAULT_NAT_LAN_VID2 7  //for wan tag pri 

static const char *portDisplayName[] =
{
	"port1", "port2", "port3", "port4", "port5",
	"wlan1", "wlan1-va1", "wlan1-va2", "wlan1-va3", "wlan1-va4", "wlan1-vxd",
	"wlan2", "wlan2-va1", "wlan2-va2", "wlan2-va3", "wlan2-va4", "wlan2-vxd"
};

#if defined(CONFIG_RTL_EXCHANGE_PORTMASK)
#define DEFAULT_ETH_WAN_PORT_MASK 0x1
#else
#define DEFAULT_ETH_WAN_PORT_MASK 0x10
#endif

#define BAIDU_SERVER "www.baidu.com"
#define SOHU_SERVER "www.sohu.com"
#define NETEASE_SERVER "www.163.com"

#define CHAN_STR_MAX_LEN 32
typedef struct channel_desc {
	int channel;
	char chan_str[CHAN_STR_MAX_LEN];
} CHANNEL_DESC_T, *CHANNEL_DESC_Tp;

int wlan_idx_to_scan=0;

CHANNEL_DESC_T all_channel[]=
{
	{0,"AutoSelect"},
	{1,"2412MHz (Channel 1)"},
	{2,"2417MHz (Channel 2)"},
	{3,"2422MHz (Channel 3)"},
	{4,"2427MHz (Channel 4)"},
	{5,"2432MHz (Channel 5)"},
	{6,"2437MHz (Channel 6)"},
	{7,"2442MHz (Channel 7)"},
	{8,"2447MHz (Channel 8)"},
	{9,"2452MHz (Channel 9)"},
	{10,"2457MHz (Channel 10)"},
	{11,"2462MHz (Channel 11)"},
	{36,"5180MHz (Channel 36)"},
	{40,"5200MHz (Channel 40)"},
	{44,"5220MHz (Channel 44)"},
	{48,"5240MHz (Channel 48)"},
	{52,"5260MHz (Channel 52)"},
	{56,"5280MHz (Channel 56)"},
	{60,"5300MHz (Channel 60)"},
	{64,"5320MHz (Channel 64)"},
	{100,"5500MHz (Channel 100)"},
	{104,"5520MHz (Channel 104)"},
	{108,"5540MHz (Channel 108)"},
	{112,"5560MHz (Channel 112)"},
	{116,"5580MHz (Channel 116)"},
	{120,"5600MHz (Channel 120)"},
	{124,"5620MHz (Channel 124)"},
	{128,"5640MHz (Channel 128)"},
	{132,"5660MHz (Channel 132)"},
	{136,"5680MHz (Channel 136)"},
	{140,"5700MHz (Channel 140)"},
	{149,"5745MHz (Channel 149)"},
	{153,"5765MHz (Channel 153)"},
	{157,"5785MHz (Channel 157)"},
	{161,"5805MHz (Channel 161)"},
	{165,"5825MHz (Channel 165)"},
	{169,"5845MHz (Channel 169)"},
	{173,"5865MHz (Channel 173)"},
};
char *get_chan_desc(int channel)
{
	int i=0;
	for(i=0;i<(sizeof(all_channel)/sizeof(CHANNEL_DESC_T));i++)
	{
		if(channel == all_channel[i].channel)
			return all_channel[i].chan_str;
	}
	return all_channel[0].chan_str;
}
int get_chan_number(char *chan_desc)
{
	int i=0;
	for(i=0;i<(sizeof(all_channel)/sizeof(CHANNEL_DESC_T));i++)
	{
		if(!strncmp(chan_desc,all_channel[i].chan_str,CHAN_STR_MAX_LEN))
			return all_channel[i].channel;
	}
	return 0;	
}
int  getWanDnsAddr(char dnsIp[][16])
{
	FILE *fp=NULL;
	char tmpbuf[64]={0};
	char *pch=NULL;
	int i=0;
	if(!isFileExist("/etc/resolv.conf"))
		return 0;
	fp = fopen("/etc/resolv.conf", "r");
	if (!fp) 
	{
        fprintf(stderr, "Read DNS file error!\n");
		return 0;
   	}
	while(fgets(tmpbuf, sizeof(tmpbuf), fp))
	{
		tmpbuf[strlen(tmpbuf)-1]='\0';
		pch=strchr(tmpbuf, ' ');
		if(pch!=NULL)
		{
			pch++;
			strcpy(dnsIp[i], pch);
			i++;
		}		
	}
	fclose(fp); 
	return i;	
}

static void bin_to_string(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		sprintf(tmpbuf, "%02x", bin[i]);
		strcat(out, tmpbuf);
	}
}

#if 1
// not concern with wapi and 1x
static int sync_vxd_to_root(unsigned int encrypt)
{
	unsigned int v_idx = 0, enableRS = 0;		

	/* SSID */
	strcpy(pMib->wlan[wlan_idx][v_idx].ssid, pMib->wlan[wlan_idx][vwlan_idx].ssid);

	/* Encrypt */
	pMib->wlan[wlan_idx][v_idx].encrypt = pMib->wlan[wlan_idx][vwlan_idx].encrypt;
	if( encrypt==ENCRYPT_DISABLED )
	{
		pMib->wlan[wlan_idx][v_idx].enable1X = pMib->wlan[wlan_idx][vwlan_idx].enable1X;
	}
	else if( encrypt==ENCRYPT_WEP )
	{		
		pMib->wlan[wlan_idx][v_idx].wep = pMib->wlan[wlan_idx][vwlan_idx].wep; // 64bit/128bit
		
		if( pMib->wlan[wlan_idx][vwlan_idx].wep!=WEP_DISABLED ){
			pMib->wlan[wlan_idx][v_idx].authType = pMib->wlan[wlan_idx][vwlan_idx].authType;
			pMib->wlan[wlan_idx][v_idx].wepKeyType = pMib->wlan[wlan_idx][vwlan_idx].wepKeyType; // ascii / hex

			if(pMib->wlan[wlan_idx][vwlan_idx].wep == WEP64){
				strcpy(pMib->wlan[wlan_idx][v_idx].wep64Key1, pMib->wlan[wlan_idx][vwlan_idx].wep64Key1);
				strcpy(pMib->wlan[wlan_idx][v_idx].wep64Key2, pMib->wlan[wlan_idx][vwlan_idx].wep64Key2);
				strcpy(pMib->wlan[wlan_idx][v_idx].wep64Key3, pMib->wlan[wlan_idx][vwlan_idx].wep64Key3);
				strcpy(pMib->wlan[wlan_idx][v_idx].wep64Key4, pMib->wlan[wlan_idx][vwlan_idx].wep64Key4);							
			}else{
				strcpy(pMib->wlan[wlan_idx][v_idx].wep128Key1, pMib->wlan[wlan_idx][vwlan_idx].wep128Key1);
				strcpy(pMib->wlan[wlan_idx][v_idx].wep128Key2, pMib->wlan[wlan_idx][vwlan_idx].wep128Key2);
				strcpy(pMib->wlan[wlan_idx][v_idx].wep128Key3, pMib->wlan[wlan_idx][vwlan_idx].wep128Key3);
				strcpy(pMib->wlan[wlan_idx][v_idx].wep128Key4, pMib->wlan[wlan_idx][vwlan_idx].wep128Key4);							
			}				
		}
	}
	else if( encrypt==ENCRYPT_WPA || encrypt==ENCRYPT_WPA2 || encrypt==ENCRYPT_WPA2_MIXED )
	{		
		//pMib->wlan[wlan_idx][v_idx].enable1X = pMib->wlan[wlan_idx][vwlan_idx].enable1X; 
		//pMib->wlan[wlan_idx][v_idx].macAuthEnabled = pMib->wlan[wlan_idx][vwlan_idx].macAuthEnabled; // ???
#ifdef CONFIG_IEEE80211W		
		pMib->wlan[wlan_idx][v_idx].wpa11w = pMib->wlan[wlan_idx][vwlan_idx].wpa11w;
		pMib->wlan[wlan_idx][v_idx].wpa2EnableSHA256 = pMib->wlan[wlan_idx][vwlan_idx].wpa2EnableSHA256;	
#endif		
		pMib->wlan[wlan_idx][v_idx].enableSuppNonWpa = pMib->wlan[wlan_idx][vwlan_idx].enableSuppNonWpa;
		if(pMib->wlan[wlan_idx][vwlan_idx].enableSuppNonWpa)			
			pMib->wlan[wlan_idx][v_idx].suppNonWpa = pMib->wlan[wlan_idx][vwlan_idx].suppNonWpa;
		
		pMib->wlan[wlan_idx][v_idx].wpaAuth = pMib->wlan[wlan_idx][vwlan_idx].wpaAuth;
		pMib->wlan[wlan_idx][v_idx].wpaCipher = pMib->wlan[wlan_idx][vwlan_idx].wpaCipher;
		pMib->wlan[wlan_idx][v_idx].wpa2Cipher = pMib->wlan[wlan_idx][vwlan_idx].wpa2Cipher;		

		if( pMib->wlan[wlan_idx][vwlan_idx].wpaAuth==WPA_AUTH_PSK ){
			pMib->wlan[wlan_idx][v_idx].wpaPSKFormat = pMib->wlan[wlan_idx][vwlan_idx].wpaPSKFormat;
			strcpy(pMib->wlan[wlan_idx][v_idx].wpaPSK, pMib->wlan[wlan_idx][vwlan_idx].wpaPSK);			
		}
		/* no change group key rekey ...*/
	}
	else {	
		printf("[%s %d]Invalid encrypt %d!\n", __FUNCTION__,__LINE__,encrypt);
	}
	
	//apmib_update(CURRENT_SETTING);
	return 0;
}
#endif

int getHeInfo(request *wp, int argc, char **argv)
{
	char	*name=NULL;
	char buffer[256]={0};
	name = argv[0];
	unsigned int val, num, i;
	bss_info bss;
	VLAN_CONFIG_T entry;
	int key_len=0;
	int cipher_suite;
	int channel_sel;
	int weptype,key_format,def_keyid;
	
	if (name == NULL) {
   		fprintf(stderr, "Insufficient args\n");
   		return -1;
   	}
	if(argv[1]){	
		if(strstr(argv[1], "wlan1-vxd")){
			wlan_idx=0;	
			vwlan_idx=NUM_VWLAN_INTERFACE;			
		}else if(strstr(argv[1], "wlan2-vxd")){
			wlan_idx=1;	
			vwlan_idx=NUM_VWLAN_INTERFACE;			
		}else if(strstr(argv[1], "wlan1-va0")){
			wlan_idx=0;		
			vwlan_idx = CMCC_WLAN_GUEST_IDX;
		}else if(strstr(argv[1], "wlan2-va0")){
			if(NUM_WLAN_INTERFACE==2)
				wlan_idx=1;
			else
				wlan_idx=0;	
			vwlan_idx = CMCC_WLAN_GUEST_IDX;
		}
		else if(strcmp(argv[1],"wlan1")==0){
			wlan_idx=0;		
			vwlan_idx = 0;
		}
		else if(strcmp(argv[1],"wlan2")==0){
			if(NUM_WLAN_INTERFACE==2)
				wlan_idx=1;
			else
				wlan_idx=0;	
			vwlan_idx = 0;
		}		
	}
	if(!strcmp(name,"sysstateHeight")){
		unsigned int opmode = 0;
		unsigned int repeaterMode = 0;
		apmib_get(MIB_OP_MODE, (void *)&opmode);
		apmib_get(MIB_REPEATER_MODE, (void *)&repeaterMode);
	
		if(wlan_num>=2){
			if(opmode== BRIDGE_MODE && repeaterMode==1)
				return req_format_write(wp, "500");
			else
				return req_format_write(wp, "510");
		}
		else{
			if(opmode== BRIDGE_MODE && repeaterMode==1)
				return req_format_write(wp, "470");
			else
				return req_format_write(wp, "450");
		}
	}
	else if ( !strcmp(name, "repeaterMode")) {
		if ( !apmib_get( MIB_REPEATER_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		req_format_write(wp, buffer);
		return 0;
	}
	else if ( !strcmp(name, "wlanMaxMacACNum")) {		
		sprintf(buffer, "%d", MAX_WLAN_AC_NUM) ;
		req_format_write(wp, buffer);
		return 0;
	}
	if(!strcmp(name,"ConMode"))
	{
		int dhcp=0;
		if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
		if(dhcp==DHCP_DISABLED)
			return req_format_write(wp, "STATIC");
		else if(dhcp==DHCP_CLIENT)
			return req_format_write(wp, "DHCP");
		else if(dhcp==PPPOE)
			return req_format_write(wp, "PPPOE");
	}
    else if(!strcmp(name,"opMode"))
    {
        int opmode_value=0;
        apmib_get( MIB_OP_MODE, (void *)&opmode_value);
        return req_format_write(wp, "%d",opmode_value);
    }
	else if(!strcmp(name,"WanPortPlug"))
	{
		if(getWanLink("eth1")<0)
			return req_format_write(wp, "fail");
		else
			return req_format_write(wp, "pass");
	}
	else if(!strcmp(name,"WIFIAuthMode"))
	{
		 ENCRYPT_T encrypt=0;
		 if ( !apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt) )
	                    return -1;

		if (encrypt == ENCRYPT_DISABLED)
                strcpy( buffer, "OPEN");
        else if (encrypt == ENCRYPT_WPA)
                strcpy( buffer, "WPAPSK");
		else if (encrypt == ENCRYPT_WPA2)
                strcpy( buffer, "WPA2PSK");
		else if (encrypt == (ENCRYPT_WPA | ENCRYPT_WPA2))
                strcpy( buffer, "WPAPSKWPA2PSK");
		else if (encrypt == ENCRYPT_WEP)
			strcpy( buffer, "WEP");
		return req_format_write(wp, "%s",buffer);
		
	}
    else if(!strcmp(name,"WlanAuthMode"))
    {
        ENCRYPT_T encrypt=0;
        if ( !apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt) )
            return -1;

        if (encrypt == ENCRYPT_DISABLED)
            strcpy( buffer, "OPEN");
        else if (encrypt == ENCRYPT_WEP)
            strcpy( buffer, "WEP");
        else if (encrypt == ENCRYPT_WPA)
            strcpy( buffer, "WPAPSK");
        else if (encrypt == ENCRYPT_WPA2)
            strcpy( buffer, "WPA2PSK");
        else if (encrypt == ENCRYPT_WPA2_MIXED)
            strcpy( buffer, "WPAPSKWPA2PSK");
        else if (encrypt == ENCRYPT_WAPI)
            strcpy( buffer, "WAPI");
        else
            strcpy( buffer, "Unknown");

        return req_format_write(wp, "%s", buffer);
    }
	else if(!strcmp(name,"EncrypType"))
	{
		ENCRYPT_T encrypt=0;
		int cipherSuite=0;
		 if ( !apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt) )
	                    return -1;

		if (encrypt == ENCRYPT_DISABLED)
                strcpy( buffer, "NONE");
        else if (encrypt == ENCRYPT_WPA)
             apmib_get( MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipherSuite);
		else if (encrypt == ENCRYPT_WPA2)
              apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipherSuite);
		else if (encrypt == (ENCRYPT_WPA | ENCRYPT_WPA2))
             apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipherSuite);
		if(cipherSuite == WPA_CIPHER_TKIP)
			strcpy( buffer, "TKIP");
		else if (cipherSuite == WPA_CIPHER_AES)
			strcpy( buffer, "AES");
		else if(cipherSuite == WPA_CIPHER_MIXED)
			strcpy( buffer, "TKIPAES");
		return req_format_write(wp, "%s",buffer);
	}
	else if ( !strcmp(name, "encrypt")) {
		if ( !apmib_get( MIB_WLAN_ENCRYPT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);		
		return req_format_write(wp, buffer);
	}
	else if(!strcmp(name,"wifi_off"))
	{
		int wlan_disable=0;
		apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan_disable);
		if(wlan_disable)
			return req_format_write(wp, "%d",1);
		else
			return req_format_write(wp, "%d",0);
	}
	else if(!strcmp(name,"Autochannel"))
	{
		int wlan_channel=0;
		apmib_get(MIB_WLAN_CHANNEL,(void*)&wlan_channel);
		if(wlan_channel==0)
			return req_format_write(wp, "%d",1);
		else
			return req_format_write(wp, "%d",0);
	}
	else if(!strcmp(name,"ChannelSeleted"))
	{
		int wlan_channel=0;
		apmib_get(MIB_WLAN_CHANNEL,(void*)&wlan_channel);
		if(wlan_channel==0)
			return req_format_write(wp, "AutoSelect");
		else
			return req_format_write(wp, "%d",wlan_channel);
	}
	else if(!strcmp(name,"STBID"))
	{
		char stbId[33]={0};
		apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM,(void*)stbId);
		return req_format_write(wp, "%s",stbId);//How to generate STBID?
	}
	else if(!strcmp(name,"userName"))
	{
		char userName[16]={0};
		apmib_get(MIB_USER_NAME,(void*)userName);
		return req_format_write(wp, "%s", userName);
	}
	else if(!strcmp(name,"lanTelnet"))
	{
		char disable[5]={0};
		apmib_get(MIB_CUSTOMER_HW_TELNETD_DISABLE,(void*)disable);
		return req_format_write(wp, "%s", disable);
	}
	else if(!strcmp(name,"Website"))
	{
		char webSite[33]={0};
		apmib_get(MIB_DOMAIN_NAME,(void*)webSite);
		return req_format_write(wp, "%s", webSite);
	}
	else if(!strcmp(name, "svnVersion")) 
	{
		FILE *file = fopen("/etc/version", "r" );
		if (file == NULL) return req_format_write(wp, "%s", "");

		char *p;
		char line[64] = {0}, version[16] = {0};
		while (fgets(line, sizeof(line), file)) {
			printf("line: %s\n", line);
			if(strstr(line, "Revision")) {
				p = strstr(line, ":");
				if (p) {
					strcpy(version, p + 1);
					break;
				}
			}
		}

		//clear spaces
		p = version;
		while(*p == ' ') p++;
		
		fclose(file);
		return req_format_write(wp, "%s", p);
	}
	else if(!strcmp(name,"cgw_ipaddr"))
	{
		struct in_addr ip;
		
		apmib_get(MIB_RTL_LINK_CGW_IP_ADDR, (void*)&ip);
		return req_format_write(wp, "%s", inet_ntoa(ip));
	}
	else if(!strcmp(name,"CurrentChannel"))
	{
		int wlan_channel=0;
		apmib_get(MIB_WLAN_CHANNEL,(void*)&wlan_channel);
		if(wlan_channel==0){
			char WlanIf[32];
			sprintf(WlanIf, "wlan%d", wlan_idx);
			if (getWlBssInfo(WlanIf, &bss) < 0)
			{
				return -1;
			}
			wlan_channel = bss.channel;
		}
		return req_format_write(wp, "%d",wlan_channel);
	}
	else if(!strcmp(name,"isRepeaterEnabled"))
	{
		int intVal, intVal2;
		if (wlan_idx == 0)
			apmib_get(MIB_REPEATER_ENABLED1, (void *)&intVal);
		else
			apmib_get(MIB_REPEATER_ENABLED2, (void *)&intVal);

		apmib_get(MIB_WLAN_NETWORK_TYPE, (void *)&intVal2);
		apmib_get(MIB_WLAN_MODE, (void *)&val);
		if (intVal != 0 && val != WDS_MODE && !(val==CLIENT_MODE && intVal2==ADHOC))
		{
			val = 1;
		}
		else
		{
			val = 0;
		}		
		return req_format_write(wp, "%d", val);
	}
	else if ( !strcmp(name, "repeaterState")) {
		char *pMsg;
		if (wlan_idx == 0)
			strcpy(buffer, "wlan0-vxd");
		else
			strcpy(buffer, "wlan1-vxd");
		
		getWlBssInfo(buffer, &bss);		
		switch (bss.state) {
			case STATE_DISABLED:
				pMsg = "Disabled";
				break;
			case STATE_IDLE:
				pMsg = "Idle";
				break;
			case STATE_STARTED:
				pMsg = "Started";
				break;
			case STATE_CONNECTED:
				pMsg = "Connected";
				break;
			case STATE_WAITFORKEY:
				pMsg = "Waiting for keys";
				break;
			case STATE_SCANNING:
				pMsg = "Scanning";
				break;
			default:
				pMsg=NULL;
		}
		return req_format_write(wp, "%s", pMsg);
	}
	else if ( !strcmp(name, "repeaterConnectState")) {
		char *pMsg;
		int ii = 0;
		for(ii=0; ii<NUM_WLAN_INTERFACE; ++ii){
			snprintf(buffer, sizeof(buffer), "wlan%d-vxd", ii);			
			getWlBssInfo(buffer, &bss);	
			if(bss.state==STATE_CONNECTED){				
				pMsg = "Connected";
				break;
			}
			else{
				pMsg = "Disconnect";
			}			
		}				
		return req_format_write(wp, "%s", pMsg);
	}
	else if( !strcmp(name, "repeaterConnectSsid"))
	{
		char *pMsg;
		unsigned char ssid[MAX_SSID_LEN] = {0};
		int ii = 0;
		for(ii=0; ii<NUM_WLAN_INTERFACE; ++ii){
			snprintf(buffer, sizeof(buffer), "wlan%d-vxd", ii);
			getWlBssInfo(buffer, &bss);	
			if(bss.state==STATE_CONNECTED){				
				if(ii==0)
					apmib_get(MIB_REPEATER_SSID1, (void *)ssid);
				else if(ii==1)
					apmib_get(MIB_REPEATER_SSID2, (void *)ssid);
				return req_format_write(wp, "%s", ssid);				
			}
			else{
				pMsg = "";					
			}			
		}	
		return req_format_write(wp, "%s", pMsg);
	}
	else if ( !strcmp(name, "wlanMode")) {
		if ( !apmib_get( MIB_WLAN_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);		
		return req_format_write(wp, buffer);
	}
	else if(!strcmp(name,"isNeedWizard"))
	{
		int isNeedNoWizard=0;
		apmib_get(MIB_NEED_NO_WIZARD,(void*)&isNeedNoWizard);
		if(isNeedNoWizard)
			return req_format_write(wp, "%d",0);
		else
			return req_format_write(wp, "%d",1);
	}
	else if(!strcmp(name,"updateTime"))
	{
		char updateTime[64]={0};		
		apmib_get(MIB_UPDATE_TIME,(void*)updateTime);
		if(updateTime[0])
			return req_format_write(wp, "%s",updateTime);
		else{
			if(getBuildImageTime(updateTime, sizeof(updateTime))<0)
				strcpy(updateTime, "Unknown");

			return req_format_write(wp, "%s",updateTime);
		}
	}
	else if(!strcmp(name,"isneedUpgrade"))
	{
		int needUpdate=0;
		apmib_get(MIB_NEED_UPDATE,(void*)&needUpdate);
		if(needUpdate)
			return req_format_write(wp,"1");
		else
			return req_format_write(wp,"0");
	}
	else if(!strcmp(name,"PingStatus"))
	{
	#if 0
		struct addrinfo *answer=NULL, hint={0}, *curr=NULL;
		bzero(&hint, sizeof(hint));
        hint.ai_family = AF_INET;
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		if(getaddrinfo("www.baidu.com","domain",&hint,&answer)<0){
			printf("%s:%d\n",__FUNCTION__,__LINE__);
			if(answer)
				freeaddrinfo(answer);
			printf("%s:%d\n",__FUNCTION__,__LINE__);
			return req_format_write(wp, "%s","fail");
		}
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		if(answer)
			freeaddrinfo(answer);
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		return req_format_write(wp, "%s","pass");
	#endif
		int siteAccess=0;
		apmib_get(MIB_SITE_ACCESS,(void*)&siteAccess);
		if(!siteAccess)
			return req_format_write(wp,"fail");
		else
			return req_format_write(wp,"pass");
	}
	else if(!strcmp(name,"checkMode"))
	{
		if(discovery_ppp("eth1") == 1)
			return req_format_write(wp, "%s","PPPOE");
		else //if(discovery_dhcp("eth1")==1)
			return req_format_write(wp, "%s","DHCP");
	//	else
		//	return req_format_write(wp, "%s","STATIC");
	}
	else if(!strcmp(name,"wan-dnsaddr"))
	{
		char dnsIp[3][16];
		int i, num;
		memset(dnsIp, 0, sizeof(dnsIp));
		num=getWanDnsAddr(dnsIp);
		for(i=0;i<num;i++){
			if(i!=num-1)
				req_format_write(wp, "%s/",dnsIp[i]);
			else
				req_format_write(wp, "%s",dnsIp[i]);
		}
	}
	else if(!strcmp(name,"SSID"))
	{
		char ssid[MAX_SSID_LEN]={0};
		apmib_get(MIB_WLAN_SSID,(void*)ssid);
		return req_format_write(wp,"%s",ssid);
	}
	else if(!strcmp(name,"wizard_SSID"))
	{
		char ssid[MAX_SSID_LEN]={0}, ssid1[MAX_SSID_LEN]={0};
		int len, len1;
		apmib_get(MIB_WLAN_SSID,(void*)ssid);
		/**
		if(NUM_WLAN_INTERFACE==2){
			if(strstr(ssid, CMCC_SSID_PREFIX_GW_2G)){
				len = strlen(CMCC_SSID_PREFIX_GW_2G);
				len1 = strlen(ssid) - len;
				memcpy(ssid1, &ssid[len], len1);
			}
		}else if(NUM_WLAN_INTERFACE==1){
			if(strstr(ssid, CMCC_SSID_PREFIX_SINGLE_BAND)){
				len = strlen(CMCC_SSID_PREFIX_SINGLE_BAND);
				len1 = strlen(ssid) - len;
				memcpy(ssid1, &ssid[len], len1);
			}
		}**/
		
		return req_format_write(wp,"%s",ssid);
	}
	else if(!strcmp(name,"wirelessName"))
	{
		if(wlan_idx==0){
			if(NUM_WLAN_INTERFACE==2){
				if(vwlan_idx==0){
					snprintf(buffer, sizeof(buffer), "%s", "5G无线网络配置");
				}else if(vwlan_idx==NUM_VWLAN_INTERFACE)
					snprintf(buffer, sizeof(buffer), "%s", "5G无线中继配置");
			}else{
				if(vwlan_idx==0){
					snprintf(buffer, sizeof(buffer), "%s", "无线网络配置");
				}else if(vwlan_idx==NUM_VWLAN_INTERFACE)
					snprintf(buffer, sizeof(buffer), "%s", "无线中继配置");
			}
		}else if(wlan_idx==1){
			if(vwlan_idx==0){
				snprintf(buffer, sizeof(buffer), "%s", "2.4G无线网络配置");
			}else if(vwlan_idx==NUM_VWLAN_INTERFACE)
				snprintf(buffer, sizeof(buffer), "%s", "2.4G无线中继配置");
		}
		return req_format_write(wp,"%s",buffer);
	}
	else if ( !strcmp(name, "repeaterSSID_drv")) {
#if defined(CONFIG_RTL_819X) && !defined(CONFIG_WLAN_REPEATER_MODE)
		return req_format_write(wp, "%s", "e0:00:19:78:01:10");
#else
		if (wlan_idx == 0)
			strcpy(buffer, "wlan0-vxd");
		else
			strcpy(buffer, "wlan1-vxd");
		getWlBssInfo(buffer, &bss);
		memcpy(buffer, bss.ssid, SSID_LEN+1);
		translate_control_code(buffer);
		return req_format_write(wp, "%s", buffer);
#endif
	}
	else if(!strcmp(name,"wlan_guest_idx"))
	{		
		return req_format_write(wp, "%d", CMCC_WLAN_GUEST_IDX);				
	}
	else if(!strcmp(name,"dhcpServerEnable"))
	{
		int lan_dhcp_type=0;
		apmib_get(MIB_DHCP,(void*)&lan_dhcp_type);

		if(lan_dhcp_type==DHCP_SERVER)
			return req_format_write(wp, "%d",1);
		else
			return req_format_write(wp, "%d",0);		
	}
	else if ( !strcmp(name, "wanMac")) 
	{
		if ( !apmib_get(MIB_WAN_MAC_ADDR,  (void *)buffer) )
			return -1;
		return req_format_write(wp, "%02x:%02x:%02x:%02x:%02x:%02x", (unsigned char)buffer[0], (unsigned char)buffer[1],
						(unsigned char)buffer[2], (unsigned char)buffer[3], (unsigned char)buffer[4], (unsigned char)buffer[5]);
	}
	else if (!strcmp(name, "he_ntpenabled")) {
        unsigned char ntp_enabled = 0;
        apmib_get(MIB_NTP_ENABLED, (void *)&ntp_enabled);
        if(ntp_enabled)
			return req_format_write(wp, "%d",1);
        else
			return req_format_write(wp, "%d",0);
    }
	else if (!strcmp(name, "he_system_time_get")) {
		time_t current_secs;
		struct tm *tm_time;
		
		time(&current_secs);
		tm_time = localtime(&current_secs);
		sprintf(buffer,"%d-%d-%d %02d:%02d:%02d", (tm_time->tm_year)+1900, (tm_time->tm_mon)+1, (tm_time->tm_mday), (tm_time->tm_hour), (tm_time->tm_min),(tm_time->tm_sec));
		return req_format_write(wp, "%s", buffer);
	}
	else if (!strcmp(name, "he_ntp_server_get")) {
		if (!apmib_get(MIB_NTP_SERVER_IP1,  (void *)buffer))
			return -1;
		
		return req_format_write(wp, "value=\"%s\"", buffer);
	}
	else if (!strcmp(name, "he_ntp_sync_get")) {
		unsigned int hour;
		if (!apmib_get(MIB_NTP_SYNC_INTERVAL,  (void *)buffer))
			return -1;
		return req_format_write(wp, "value=\"%d\"", (*(unsigned int *)buffer)/3600);
	}
	else if(!strcmp(name,"wlan_num"))
	{
		sprintf(buffer, "%d", wlan_num);
		return req_format_write(wp, buffer);		 
	}
	else if ( !strcmp(name, "RFPower")) 
	{		
		if ( !apmib_get( MIB_WLAN_RFPOWER_SCALE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		return req_format_write(wp, buffer);		
	}
	else if ( !strcmp(name, "hiddenSSID")) {
		if ( !apmib_get( MIB_WLAN_HIDDEN_SSID, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		return req_format_write(wp, buffer);		
	}
#ifdef WIFI_SIMPLE_CONFIG
	else if ( !strcmp(name, "wscDisable")) {
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&val);
		sprintf(buffer, "%d", val);
		return req_format_write(wp, buffer);		
	}
#endif
	else if ( !strcmp(name, "pskValue")) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer) )
			return -1;
		translate_control_code_sprintf(buffer);		
		return req_format_write(wp, buffer);
	}
	else if ( !strcmp(name, "enable1X")) {
		if ( !apmib_get( MIB_WLAN_ENABLE_1X, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		return req_format_write(wp, buffer);		 
	}
	else if ( !strcmp(name, "groupRekeyTime")) {
		if ( !apmib_get( MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val );
   		return req_format_write(wp, buffer);
	}
	else if ( !strcmp(name, "wep")) {
		if ( !apmib_get( MIB_WLAN_WEP, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		return req_format_write(wp, buffer);   	    	
	}
	else if ( !strcmp(name, "keyType")) {
		if ( !apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val) )
			return -1;
		if( val==KEY_HEX )
			val = 0;
		else if( val==KEY_ASCII )
			val = 1;
		
		sprintf(buffer, "%d", (int)val) ;
		return req_format_write(wp, buffer);		
	}
	else if(!strcmp(name,"wired_dhcp_client_num"))
	{
		extern int wired_dhcp_client_num;

		//printf("\nwired_dhcp_client_num=%d\n",wired_dhcp_client_num);

		return req_format_write(wp, "%d", wired_dhcp_client_num);				
	}
	else if(!strcmp(name,"wireless_dhcp_client_num"))
	{
		extern int wireless_dhcp_client_num;

		dhcpWirelessClientList(wp, argc, argv);

		//printf("\nwired_dhcp_client_num=%d\n",wireless_dhcp_client_num);

		return req_format_write(wp, "%d", wireless_dhcp_client_num);				
	}
	else if(!strcmp(name,"admin_passwd"))
	{
		unsigned char passwd[MAX_NAME_LEN] = {0};
		if ( !apmib_get( MIB_USER_PASSWORD, (void *)passwd) )
			return -1;

		//printf("passwd:%s\n", passwd);
		return req_format_write(wp, "%s", passwd);				
	}
	else if ( !strcmp(name, "wdsenablever")) 
	{
		int intVal, intVal2;
		if (wlan_idx == 0)
			apmib_get(MIB_REPEATER_ENABLED1, (void *)&intVal);
		else
			apmib_get(MIB_REPEATER_ENABLED2, (void *)&intVal);

		apmib_get(MIB_WLAN_NETWORK_TYPE, (void *)&intVal2);
		apmib_get(MIB_WLAN_MODE, (void *)&val);
		if (intVal != 0 && val != WDS_MODE && !(val==CLIENT_MODE && intVal2==ADHOC))
		{
			val = 3; // repeater mode
		}
		else if(val==WDS_MODE)
		{
			val = 4; // lazy WDS
		}
		else
			val = 0;
		
		sprintf(buffer, "%d", val);
		return req_format_write(wp, buffer);		
	}
	else if(!strcmp(name,"wlanband"))
	{
		apmib_get(MIB_WLAN_BAND, (void *)&val);
		return req_format_write(wp, "%d",val);
	}
	else if ( !strcmp(name, "channel_desc")) {
		int chan=0;
		if ( !apmib_get( MIB_WLAN_CHANNEL, (void *)&chan))
			return -1;
		return req_format_write(wp, "%s",get_chan_desc(chan));
	}
	else if (!strcmp(name,"he_wan_mac")) {
		
		if (!apmib_get(MIB_WAN_MAC_ADDR,  (void *)buffer))
			return -1;
		/*if NULL wan mac using default hw mac*/
		if(buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0
			&& buffer[3] == 0 && buffer[4] == 0  && buffer[5] == 0)
			apmib_get(MIB_HW_NIC1_ADDR,  (void *)buffer);
		
		return req_format_write(wp, "%02x:%02x:%02x:%02x:%02x:%02x", (unsigned char)buffer[0], (unsigned char)buffer[1],
						(unsigned char)buffer[2], (unsigned char)buffer[3], (unsigned char)buffer[4], (unsigned char)buffer[5]);
	}
	else if(!strcmp(name,"VlanEnable")){
	    apmib_get(MIB_VLAN_ENABLED, (void *)&val);
		return req_format_write(wp, "%d",val);
	}
	else if (!strcmp(name, "ipfilterEnable")) {
		if ( !apmib_get( MIB_IPFILTER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   ipfilterEnable:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "currentIpfiltermode")) {
		if ( !apmib_get( MIB_IPFILTER_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   currentIpfiltermode:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "currentIpfiltertype")) {
		if ( !apmib_get( MIB_IPFILTER_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   currentIpfilterType:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "currentIpfilterprotocol")) {
		int entryNum, i;
		IPFILTER_T entry;
		if ( !apmib_get(MIB_IPFILTER_TBL_NUM, (void *)&entryNum)) {
  		    fprintf(stderr, "Get table entry error!\n");
		    return -1;
	    }
		for (i=entryNum; i>=1; i--) {
		    *((char *)&entry) = (char)i;
		    if ( !apmib_get(MIB_IPFILTER_TBL, (void *)&entry))
			    return -1;
			if(entry.ipFilterType==PROTO_BASED)
		    {
		        if(entry.protoType == PROTO_ICMP)
		        {
		            val = 1;
					break;
		        }
				else if(entry.protoType == PROTO_ARP)
				{
					val = 2;
					break;
				}
				else
				{
					val = 0;
					break;
				}
			}
		}
		sprintf(buffer, "%d", val);
		req_format_write(wp, buffer);

		return 0;	
	}
	else if (!strcmp(name, "getIpfilterRules")) {
		int entryNum, i, mode, iplen;
		char ip[16];
		IPFILTER_T entry;
		if ( !apmib_get(MIB_IPFILTER_TBL_NUM, (void *)&entryNum)) {
  		    fprintf(stderr, "Get table entry error!\n");
		    return -1;
	    }
		apmib_get(MIB_IPFILTER_MODE, (void *)&mode);
		for (i=1; i<=entryNum; i++) {
		    *((char *)&entry) = (char)i;
		    if ( !apmib_get(MIB_IPFILTER_TBL, (void *)&entry))
			    return -1;
			
		    //if(entry.ipFilterMode != mode)continue;
			
			iplen = strlen(inet_ntoa(*((struct in_addr *)entry.ipAddr)));
		    memcpy(ip,inet_ntoa(*((struct in_addr *)entry.ipAddr)),iplen);
			ip[iplen]='\0';

			char tempbuffer[128] ={0};
			sprintf(tempbuffer, "any/0,0,0,0,%s,0,0,0,5,0,,,%d,%d;", ip, mode, entry.ipFilterMode);
			strcat(buffer, tempbuffer);
			//printf("---------------------%s:%d   ip:%s--------------------\n",__FUNCTION__,__LINE__,ip);
			//printf("---------------------%s:%d   tempbuffer:%s--------------------\n",__FUNCTION__,__LINE__,tempbuffer);
			//printf("---------------------%s:%d   buffer:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		}
		buffer[strlen(buffer)-1] = 0;
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "urlfilterEnable")) {
		if ( !apmib_get( MIB_URLFILTER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   urlfilterEnable:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "currentURLfiltermode")) {
		if ( !apmib_get( MIB_URLFILTER_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   currentURLfiltermode:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "getURLfilterRules")) {
		int entryNum, i, mode;
		URLFILTER_T entry;
		if ( !apmib_get(MIB_URLFILTER_TBL_NUM, (void *)&entryNum)) {
  		    fprintf(stderr, "Get table entry error!\n");
		    return -1;
	    }
		apmib_get(MIB_URLFILTER_MODE, (void *)&mode);
		for (i=1; i<=entryNum; i++) {
		    *((char *)&entry) = (char)i;
		    if ( !apmib_get(MIB_URLFILTER_TBL, (void *)&entry))
			    return -1;
			
		    //if(entry.ruleMode != mode)continue;

			char tempbuffer[128] ={0};
			sprintf(tempbuffer, "%s,%d,%d;", entry.urlAddr, mode, entry.ruleMode);
			strcat(buffer, tempbuffer);
			//printf("---------------------%s:%d   url:%s--------------------\n",__FUNCTION__,__LINE__,entry.urlAddr);
			//printf("---------------------%s:%d   tempbuffer:%s--------------------\n",__FUNCTION__,__LINE__,tempbuffer);
		}
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "macfilterEnable")) {
		if ( !apmib_get( MIB_MACFILTER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   macfilterEnable:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "currentMACfiltermode")) {
		if ( !apmib_get( MIB_MACFILTER_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		//printf("---------------------%s:%d   currentMACfiltermode:%s--------------------\n",__FUNCTION__,__LINE__,buffer);
		req_format_write(wp, buffer);
		return 0;	
	}
	else if (!strcmp(name, "getMACfilterRules")) {
		int entryNum, i, mode;
		MACFILTER_T entry;
		if ( !apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum)) {
  		    fprintf(stderr, "Get table entry error!\n");
		    return -1;
	    }
		apmib_get(MIB_MACFILTER_MODE, (void *)&mode);
		for (i=1; i<=entryNum; i++) {
		    *((char *)&entry) = (char)i;
		    if ( !apmib_get(MIB_MACFILTER_TBL, (void *)&entry))
			    return -1;
			
		    //if(entry.ruleMode != mode)continue;

			char tempbuffer[128] ={0};
			sprintf(tempbuffer, "any/0,0,0,0,any/0,0,0,0,5,0,,%02x:%02x:%02x:%02x:%02x:%02x,%d,%d;", 
				entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			    entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], mode, entry.mode);
			strcat(buffer, tempbuffer);
			//printf("---------------------%s:%d   tempbuffer:%s--------------------\n",__FUNCTION__,__LINE__,tempbuffer);
		}
		req_format_write(wp, buffer);
		return 0;	
	}
	else if ( !strcmp(name, "sip_algonoff")) {
 		//if ( !apmib_get( MIB_ALG_SIP_DISABLE, (void *)&val) )
		//	return -1;

		bool enable;
		if(rtk_alg_get("sip",&enable) == 0)
			val=enable;
		else
			val=2;


		sprintf(buffer, "%d", (int)val);
		req_format_write(wp, buffer);
		return 0;
	}
	else if ( !strcmp(name, "ftp_algonoff")) {
 		//if ( !apmib_get( MIB_ALG_SIP_DISABLE, (void *)&val) )
		//	return -1;

		bool enable;
		if(rtk_alg_get("ftp",&enable) == 0)
			val=enable;
		else
			val=2;


		sprintf(buffer, "%d", (int)val);
		req_format_write(wp, buffer);
		return 0;
	}
	else if ( !strcmp(name, "rtsp_algonoff")) {
 		//if ( !apmib_get( MIB_ALG_SIP_DISABLE, (void *)&val) )
		//	return -1;

		bool enable;
		if(rtk_alg_get("rtsp",&enable) == 0)
			val=enable;
		else
			val=2;


		sprintf(buffer, "%d", (int)val);
		req_format_write(wp, buffer);
		return 0;
	}
	else if ( !strcmp(name, "l2tp_algonoff")) {
 		//if ( !apmib_get( MIB_ALG_SIP_DISABLE, (void *)&val) )
		//	return -1;

		bool enable;
		if(rtk_alg_get("l2tp",&enable) == 0)
			val=enable;
		else
			val=2;


		sprintf(buffer, "%d", (int)val);
		req_format_write(wp, buffer);
		return 0;
	}
	else if(!strcmp(name,"VlanId")){
		apmib_get(MIB_VLAN_TBL_NUM, (void *)&num);
		for(i=1; i<=num; i++)
		{
			*(char *)&entry = (char)i;
			if(entry.VlanId != DEFAULT_NAT_LAN_VID2){
				apmib_get(MIB_VLAN_TBL, (void *)&entry);
				return req_format_write(wp, "%d",entry.VlanId);
			}
		}	   
	}
	else if(!strcmp(name,"VlanPriority")){
		apmib_get(MIB_VLAN_TBL_NUM, (void *)&num);
		for(i=1; i<=num; i++)
		{
			*(char *)&entry = (char)i;
			if(entry.VlanId != DEFAULT_NAT_LAN_VID2){
				apmib_get(MIB_VLAN_TBL, (void *)&entry);
				return req_format_write(wp, "%d",entry.VlanPriority);
			}
		}
	}
	else if(!strcmp(name,"SnoopDisable")){
		apmib_get(MIB_IGMP_SNOOPING_DISABLE, (void *)&val);
		return req_format_write(wp, "%d",!val);		
	}
	else if(!strcmp(name,"ProxyDisable")){
		apmib_get(MIB_IGMP_PROXY_DISABLED, (void *)&val);
		return req_format_write(wp, "%d",!val);		
	}
	else if(!strcmp(name,"UPNPEnable")){
		apmib_get(MIB_UPNP_ENABLED, (void *)&val);
		return req_format_write(wp, "%d",val);		
	}
	else if(!strcmp(name,"NTPEnable")){
		apmib_get(MIB_NTP_ENABLED, (void *)&val);
		return req_format_write(wp, "%d",val);		
	}
	else if (!strcmp(name,"cipher_type")){
		apmib_get(MIB_WLAN_WPA_CIPHER_SUITE,(void *)&cipher_suite);
		if(cipher_suite == WPA_CIPHER_TKIP)
			sprintf(buffer,"TKIP");
		else if (cipher_suite == WPA_CIPHER_AES)
			sprintf(buffer,"AES");
		else
			sprintf(buffer,"TKIPAES");

		return req_format_write(wp,"%s",buffer);
	}
	else if(!strcmp(name,"wpa2_cipher_type")){
		apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE,(void *)&cipher_suite);
		if(cipher_suite == WPA_CIPHER_TKIP)
			sprintf(buffer,"TKIP");
		else if(cipher_suite == WPA_CIPHER_AES)
			sprintf(buffer,"AES");
		else
			sprintf(buffer,"TKIPAES");

		return req_format_write(wp,"%s",buffer);
	}
	else if(!strcmp(name,"channel_selected")){
		apmib_get(MIB_WLAN_CHANNEL,(void *)&channel_sel);
		return req_format_write(wp, "%d",channel_sel);
	}
	else if(!strcmp(name,"bandwidth")){
		apmib_get(MIB_WLAN_CHANNEL_BONDING,(void *)&val);
		return req_format_write(wp,"%d",val);
	}
	else if(!strcmp(name,"ht_2040_coexist")){
		apmib_get(MIB_WLAN_COEXIST_ENABLED,(void *)&val);
		return req_format_write(wp,"%d",val); 
	}
	else if(!strcmp(name,"key_type")){
		apmib_get(MIB_WLAN_WEP_KEY_TYPE,(void *)&val);
		return req_format_write(wp,"%d",val);
	}
	else if(!strcmp(name,"key_string_length")){
		apmib_get(MIB_WLAN_WEP,(void *)&weptype);
		apmib_get(MIB_WLAN_WEP_KEY_TYPE,(void *)&key_format);
		if(weptype == WEP64){
			if(key_format == KEY_ASCII){
				key_len=5;
			}
			else if(key_format == KEY_HEX){
				key_len=10;
			}
		}
		else if(weptype ==  WEP128){
				if(key_format == KEY_ASCII){
					key_len=13;
				}
				else if(key_format == KEY_HEX){
					key_len=26;
				}
		}
   		return req_format_write(wp,"%d", key_len);
	}
	else if (!strcmp(name,"wlan_interface_num")){
		return req_format_write(wp,"%d", NUM_WLAN_INTERFACE);
	}
	else if(!strcmp(name,"default_keyid")){
		apmib_get(MIB_WLAN_WEP_DEFAULT_KEY,(void *)&def_keyid);
		return req_format_write(wp,"%d", def_keyid+1);
	}
}

void heUIIpfilter(request *wp, char *path, char *query)
{
    int num_id, get_id, add_id, del_id, delall_id, enable_id;
	char *strVal, *submitUrl, *filterType, *strip;
	char *strEndIpAddr, *strDstIpAddr, *strDstEndIpAddr, *strIpProtocol, *strProtoProtocol;
	IPFILTER_T ipEntry, ipentrytmp;
	void *pEntry, *ptempEntry;
    int intVal, i, j;
	int entryNum;
	int mode;/*ipfilter mode:white list or black list*/
	int curFilterType, curFilter;
	char tmpBuf[128];
	struct in_addr curIpAddr, curSubnet;
	unsigned long v1, v2, v3;
	
#ifndef NO_ACTION
		int pid;
#endif
	
    //printf("---------------------%s:%d--------------------\n",__FUNCTION__,__LINE__);
	num_id = MIB_IPFILTER_TBL_NUM;
	get_id = MIB_IPFILTER_TBL;
	add_id = MIB_IPFILTER_ADD;
	del_id = MIB_IPFILTER_DEL;
	delall_id = MIB_IPFILTER_DELALL;
	enable_id = MIB_IPFILTER_ENABLED;
	memset(&ipEntry, '\0', sizeof(ipEntry));
	pEntry = (void *)&ipEntry;	


    // Set enable flag
	strVal = req_get_cstream_var(wp, ("apselect_0"), "");
	//printf("---------------------%s:%d    enable:%s--------------------\n",__FUNCTION__,__LINE__,strVal);
	if ( !strcmp(strVal, "0"))
		intVal = 0;
	else
		intVal = 1;

	if ( apmib_set(enable_id, (void *)&intVal) == 0) {
		strcpy(tmpBuf, ("Set enabled flag error!"));
		goto setErr_filter;
	}

	if(intVal==0)
		goto setOk_filter;

	//get filtertype
	filterType = req_get_cstream_var(wp, ("filter_type"), "");
	//printf("---------------------%s:%d    filterType:%s--------------------\n",__FUNCTION__,__LINE__,filterType);
	if ( !strcmp(filterType, "1")){
			ipEntry.ipFilterType = PROTO_BASED; //proto based filter
			curFilterType = 1;
	}
	else
	{
			ipEntry.ipFilterType = IP_BASED; //ip based filter
			curFilterType = 0;
	}
	if ( apmib_set(MIB_IPFILTER_TYPE, (void *)&curFilterType) == 0) 
	{
	        strcpy(tmpBuf, ("Set ipfilter type mib error!"));
		    goto setErr_filter;
	}
	
			

	if(ipEntry.ipFilterType == IP_BASED)
	{
	    //printf("---------------------%s:%d    IP_BASED--------------------\n",__FUNCTION__,__LINE__);
	    if ( !strcmp(strVal, "1"))
			mode = 1;
		else if ( !strcmp(strVal, "2"))
			mode = 0;
		if ( apmib_set(MIB_IPFILTER_MODE, (void *)&mode) == 0) 
	    {
	        strcpy(tmpBuf, ("Set ipfilter mode mib error!"));
		    goto setErr_filter;
	    }
        //set ipfilter mode and protocal
		ipEntry.ipFilterMode = mode;
		ipEntry.protoType = PROTO_BOTH;
		
	    //get ip addr
	    strip = req_get_cstream_var(wp, ("dip_address"), "");
	    //printf("---------------------%s:%d    strip:%s--------------------\n",__FUNCTION__,__LINE__,strip);

		if(strip[0])
		{   
		    inet_aton(strip, (struct in_addr *)&ipEntry.ipAddr);
			inet_aton(strip, (struct in_addr *)&ipEntry.ipAddrEnd);

			getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		    getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);
			
#if 0  //添加注释: IP 过滤，仅支持内网过滤，更改为支持内网，外网过�?
		    v1 = *((unsigned long *)ipEntry.ipAddr);
		    v2 = *((unsigned long *)&curIpAddr);
		    v3 = *((unsigned long *)&curSubnet);

			if ( (v1 & v3) != (v2 & v3)) 
			{
				strcpy(tmpBuf, ("Invalid IP address! It should be set within the current subnet."));
				goto setErr_filter;
   			}	
#endif

			if ( !apmib_get(num_id, (void *)&entryNum)) {
				strcpy(tmpBuf, ("Get entry number error!"));
			    goto setErr_filter;
		    }
			if ( (entryNum + 1) > MAX_FILTER_NUM) {
				strcpy(tmpBuf, "Cannot add new entry because table is full!");
				goto setErr_filter;
			}
			//add same ipfilter rule check
			for(j=1;j<=entryNum;j++)
		    {
			    memset(&ipentrytmp, 0x00, sizeof(ipentrytmp));
			    *((char *)&ipentrytmp) = (char)j;
			    if ( apmib_get(MIB_IPFILTER_TBL, (void *)&ipentrytmp))
				{
				    if ((*((unsigned int*)ipentrytmp.ipAddr)) == (*((unsigned int*)ipEntry.ipAddr)))
				    {
				        strcpy(tmpBuf, ("rule already exist!"));
					    goto setErr_filter;
				    }
				}
			}
			
		}
		else
		{
		    /*strip="0.0.0.0";
			strEndIpAddr="0.0.0.0";
		    inet_aton(strip, (struct in_addr *)&ipEntry.ipAddr);
			inet_aton(strEndIpAddr, (struct in_addr *)&ipEntry.ipAddrEnd);*/
			goto setOk_filter;
		}
	    strDstIpAddr="0.0.0.0";
		strDstEndIpAddr="0.0.0.0";
		inet_aton(strDstIpAddr, (struct in_addr *)&ipEntry.dstipAddr);
		inet_aton(strDstEndIpAddr, (struct in_addr *)&ipEntry.dstipAddrEnd);


	}
	else if(ipEntry.ipFilterType == PROTO_BASED)
	{
	    //delete previous proto_based entry first
	    if ( !apmib_get(num_id, (void *)&entryNum)) {
  		    fprintf(stderr, "Get table entry error!\n");
		    return -1;
	    }
		for (i=1; i<=entryNum; i++) {
		    memset(&ipentrytmp, 0x00, sizeof(ipentrytmp));
			*((char *)&ipentrytmp) = (char)i;
		    if ( !apmib_get(MIB_IPFILTER_TBL, (void *)&ipentrytmp))
			    return -1;
			if(ipentrytmp.ipFilterType == PROTO_BASED)
			{
			    ptempEntry = (void *)&ipentrytmp;	
			    apmib_set(del_id, ptempEntry);
			}
				
		}
	   //printf("---------------------%s:%d    PROTO_BASED--------------------\n",__FUNCTION__,__LINE__);
	   //protocol based filter
	   strVal = req_get_cstream_var(wp, ("filter_protocol"), "");
	   //printf("---------------------%s:%d    PROTOtype:%s--------------------\n",__FUNCTION__,__LINE__,strVal);
	   if (strVal[0]) {
				if(strVal[0]=='0'){
					ipEntry.protoType = PROTO_ICMP_ARP;
				}
				else if(strVal[0]=='1'){
					ipEntry.protoType = PROTO_ICMP;
				}
				else if(strVal[0]=='2'){
					ipEntry.protoType = PROTO_ARP;
				}		
				else {
					strcpy(tmpBuf, ("Error! Invalid protocol type."));
					goto setErr_filter;
				}
		}
		else {
				strcpy(tmpBuf, ("Error! Protocol type cannot be empty."));
				goto setErr_filter;
			}
	}

	
	// set to MIB. try to delete it first to avoid duplicate case
	apmib_set(del_id, pEntry);
	if ( apmib_set(add_id, pEntry) == 0) {
		strcpy(tmpBuf, ("Add table entry error!"));
		goto setErr_filter;
	}

	setOk_filter:

	if ( !apmib_get(num_id, (void *)&entryNum)) {
			strcpy(tmpBuf, ("Get entry number error!"));
			goto setErr_filter;
	}
	for (i=entryNum; i>0; i--) {
		snprintf(tmpBuf, 20, "select_%d", i-1);

		strVal = req_get_cstream_var(wp, tmpBuf, "");
	    //printf("---------------------%s:%d    select_%d:%s--------------------\n",__FUNCTION__,__LINE__,i-1,strVal);
		if ( !strcmp(strVal, "disabled") ) {

			*((char *)pEntry) = (char)i;
			if ( !apmib_get(get_id, pEntry)) {
				strcpy(tmpBuf, ("Get table entry error!"));
				goto setErr_filter;
			}
			if ( !apmib_set(del_id, pEntry)) {
				strcpy(tmpBuf, ("Delete table entry error!"));
				goto setErr_filter;
			}
		}
	}
	

	
		apmib_update_web(CURRENT_SETTING);
		
#if !defined(MULTI_WAN_SUPPORT)
#ifndef NO_ACTION
		pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
		}
			else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
			execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
					exit(1);
			}
#endif
#endif
	
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
	#if defined(MULTI_WAN_SUPPORT)
		OK_MSG(submitUrl);
		return;
	#else
#ifdef REBOOT_CHECK
		if(needReboot == 1)
		{
			OK_MSG(submitUrl);
			return;
		}
#endif
	
		if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
		return;
	#endif
	setErr_filter:
		//ERR_MSG(tmpBuf);
		apmib_update_web(CURRENT_SETTING);
		#if !defined(MULTI_WAN_SUPPORT)
#ifndef NO_ACTION
		pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
		}
			else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
			execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
					exit(1);
			}
#endif
#endif
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
		fprintf(stderr,"%s\n",tmpBuf);

}

void heUIURLfilter(request *wp, char *path, char *query)
{
     //printf("---------------------%s:%d--------------------\n",__FUNCTION__,__LINE__);
	 int num_id, get_id, add_id, del_id, delall_id, enable_id;
	 char tmpBuf[128];
	 int intVal, i, j;
	 int entryNum;
	 int mode;/*urlfilter mode:white list or black list*/
	 URLFILTER_T urlEntry, urlEntrytmp;
	 void *pEntry;
	 char *strVal, *submitUrl;
#ifndef NO_ACTION
		int pid;
#endif

     num_id = MIB_URLFILTER_TBL_NUM;
	 get_id = MIB_URLFILTER_TBL;
	 add_id = MIB_URLFILTER_ADD;
	 del_id = MIB_URLFILTER_DEL;
	 delall_id = MIB_URLFILTER_DELALL;
	 enable_id = MIB_URLFILTER_ENABLED;
	 memset(&urlEntry, '\0', sizeof(urlEntry));
	 pEntry = (void *)&urlEntry;


	 // Set enable flag
	strVal = req_get_cstream_var(wp, ("apselect_0"), "");
	//printf("---------------------%s:%d    enable:%s--------------------\n",__FUNCTION__,__LINE__,strVal);
	if ( !strcmp(strVal, "0"))
		intVal = 0;
	else
		intVal = 1;

	if ( apmib_set(enable_id, (void *)&intVal) == 0) {
		strcpy(tmpBuf, ("Set enabled flag error!"));
		goto setErr_filter;
	}

	if(intVal==0)
		goto setOk_filter;

	 //set urlfilter mode
	if ( !strcmp(strVal, "1"))   //white
		mode = 1;
	else if ( !strcmp(strVal, "2"))   //black
		mode = 0;
	if ( apmib_set(MIB_URLFILTER_MODE, (void *)&mode) == 0)
	{
	    strcpy(tmpBuf, ("Set urlfilter mode mib error!"));
		goto setErr_filter;
	}


	//set urlfilter entry
	strVal = req_get_cstream_var(wp, "addURLFilter", "");
	//printf("---------------------%s:%d    URLFilteraddr:%s--------------------\n",__FUNCTION__,__LINE__,strVal);
	if (!strVal[0])// && !strComment[0])
		goto setOk_filter;
	else
	{
		for(j=0; strVal[j]!='\0'; j++)
			strVal[j] = tolower(strVal[j]);
		//printf("---------------------%s:%d    URLFilteraddr  after tolower:%s--------------------\n",__FUNCTION__,__LINE__,strVal);	
		strcpy((char *)urlEntry.urlAddr, strVal);
		urlEntry.ruleMode=mode;
	}

	//add same url rule check
	apmib_get(num_id, (void *)&entryNum);
	for(j=1;j<=entryNum;j++)
	{
		memset(&urlEntrytmp, 0x00, sizeof(urlEntrytmp));
		*((char *)&urlEntrytmp) = (char)j;
		if ( apmib_get(MIB_URLFILTER_TBL, (void *)&urlEntrytmp))
		{
			if(urlEntry.ruleMode == urlEntrytmp.ruleMode)
			if (strlen(urlEntry.urlAddr) == strlen(urlEntrytmp.urlAddr))
			{
				if (!memcmp(urlEntrytmp.urlAddr, urlEntry.urlAddr, strlen(urlEntry.urlAddr)))
				{
						strcpy(tmpBuf, ("rule already exist!"));
						goto setErr_filter;
				}
			}
		}
	}
	//check entry max num
	if ( (entryNum + 1) > MAX_URLFILTER_NUM) {
		strcpy(tmpBuf, ("Cannot add new URL entry because table is full!"));
		goto setErr_filter;
	}

	
	apmib_set(del_id, pEntry);
	if ( apmib_set(add_id, pEntry) == 0) {
		strcpy(tmpBuf, ("Add table entry error!"));
		goto setErr_filter;
	}

	
	 setOk_filter:

	if ( !apmib_get(num_id, (void *)&entryNum)) {
		strcpy(tmpBuf, ("Get entry number error!"));
		goto setErr_filter;
	}
	for (i=entryNum; i>0; i--) {
		snprintf(tmpBuf, 20, "select_%d", i-1);

		strVal = req_get_cstream_var(wp, tmpBuf, "");
	    //printf("---------------------%s:%d    select_%d:%s--------------------\n",__FUNCTION__,__LINE__,i-1,strVal);
		if ( !strcmp(strVal, "disabled") ) {

			*((char *)pEntry) = (char)i;
			if ( !apmib_get(get_id, pEntry)) {
				strcpy(tmpBuf, ("Get table entry error!"));
				goto setErr_filter;
			}
			if ( !apmib_set(del_id, pEntry)) {
				strcpy(tmpBuf, ("Delete table entry error!"));
				goto setErr_filter;
			}
		}
	}
	 
	 	apmib_update_web(CURRENT_SETTING);
		
#if !defined(MULTI_WAN_SUPPORT)
#ifndef NO_ACTION
		pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
		}
			else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
			execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
					exit(1);
			}
#endif
#endif
	
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
	#if defined(MULTI_WAN_SUPPORT)
		OK_MSG(submitUrl);
		return;
	#else
#ifdef REBOOT_CHECK
		if(needReboot == 1)
		{
			OK_MSG(submitUrl);
			return;
		}
#endif
	
		if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
		return;
	#endif
	setErr_filter:
		//ERR_MSG(tmpBuf);
		apmib_update_web(CURRENT_SETTING);
		#if !defined(MULTI_WAN_SUPPORT)
#ifndef NO_ACTION
		pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
		}
			else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
			execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
					exit(1);
			}
#endif
#endif
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
		fprintf(stderr,"%s\n",tmpBuf);
}

void heUIMACfilter(request *wp, char *path, char *query)
{
    char tmpBuf[128];
	int intVal, i, j, idx;
	int mode;/*macfilter mode:white list or black list*/
	int num_id, get_id, add_id, del_id, delall_id, enable_id;
	int entryNum;
	char *strVal, *submitUrl;
	char strMac[18];
	MACFILTER_T macEntry, macEntrytmp;
	void *pEntry;
#ifndef NO_ACTION
	int pid;
#endif
	//printf("---------------------%s:%d--------------------\n",__FUNCTION__,__LINE__);

    num_id = MIB_MACFILTER_TBL_NUM;
	get_id = MIB_MACFILTER_TBL;
	add_id = MIB_MACFILTER_ADD;
	del_id = MIB_MACFILTER_DEL;
	delall_id = MIB_MACFILTER_DELALL;
	memset(&macEntry, '\0', sizeof(macEntry));
	enable_id = MIB_MACFILTER_ENABLED;
	pEntry = (void *)&macEntry;

	// Set enable flag
	strVal = req_get_cstream_var(wp, ("apselect_0"), "");
	//printf("---------------------%s:%d    enable:%s--------------------\n",__FUNCTION__,__LINE__,strVal);
	if ( !strcmp(strVal, "0"))
		intVal = 0;
	else
		intVal = 1;

	if ( apmib_set(enable_id, (void *)&intVal) == 0) {
		strcpy(tmpBuf, ("Set enabled flag error!"));
		goto setErr_filter;
	}

	if(intVal==0)
		goto setOk_filter;

	//set macfilter mode
	if ( !strcmp(strVal, "1"))   //white
		mode = 1;
	else if ( !strcmp(strVal, "2"))   //black
		mode = 0;
	if ( apmib_set(MIB_MACFILTER_MODE, (void *)&mode) == 0)
	{
	    strcpy(tmpBuf, ("Set macfilter mode mib error!"));
		goto setErr_filter;
	}

	//set macfilter entry
	strVal = req_get_cstream_var(wp, ("mac_address"), "");
	//printf("---------------------%s:%d    UImac:%s--------------------\n",__FUNCTION__,__LINE__,strVal);
	if (!strVal[0])
		goto setOk_filter;

	if(strlen(strVal)!=17)
	{
	    strcpy(tmpBuf, ("Error! Invalid MAC address."));
		goto setErr_filter;
	}
	else
	{
	    i = 0;
	    for (idx=0;idx<17;idx+=3 )
		{
		    strMac[i++] = strVal[idx];
		    strMac[i++] = strVal[idx+1];
		}
		strMac[i] = 0;
	}
	//printf("---------------------%s:%d    strMac:%s--------------------\n",__FUNCTION__,__LINE__,strMac);

	if (strlen(strMac)!=12 || !string_to_hex(strMac, macEntry.macAddr, 12)) {
		strcpy(tmpBuf, ("Error! Invalid MAC address."));
		goto setErr_filter;
	}
	apmib_get(MIB_MACFILTER_MODE, (void *)&mode);
	macEntry.mode=mode;
	


	//add same mac rule check
	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
	for(j=1;j<=entryNum;j++)
	{
		memset(&macEntrytmp, 0x00, sizeof(macEntrytmp));
		*((char *)&macEntrytmp) = (char)j;
		if ( apmib_get(MIB_MACFILTER_TBL, (void *)&macEntrytmp))
		{
			if ((!memcmp(macEntrytmp.macAddr, macEntry.macAddr, 6))&&(mode == macEntrytmp.mode))
			{
				strcpy(tmpBuf, ("rule already exist!"));
				goto setErr_filter;
			}
					
		}
	}

	//check entry max num
	if ( (entryNum + 1) > MAX_FILTER_NUM) {
		strcpy(tmpBuf, ("Cannot add new mac entry because table is full!"));
		goto setErr_filter;
	}

	
	apmib_set(del_id, pEntry);
	if ( apmib_set(add_id, pEntry) == 0) {
		strcpy(tmpBuf, ("Add table entry error!"));
		goto setErr_filter;
	}

	
	setOk_filter:

	if ( !apmib_get(num_id, (void *)&entryNum)) {
			strcpy(tmpBuf, ("Get entry number error!"));
			goto setErr_filter;
	}
	for (i=entryNum; i>0; i--) {
		snprintf(tmpBuf, 20, "select_%d", i-1);

		strVal = req_get_cstream_var(wp, tmpBuf, "");
	    //printf("---------------------%s:%d    select_%d:%s--------------------\n",__FUNCTION__,__LINE__,i-1,strVal);
		if ( !strcmp(strVal, "disabled") ) {

			*((char *)pEntry) = (char)i;
			if ( !apmib_get(get_id, pEntry)) {
				strcpy(tmpBuf, ("Get table entry error!"));
				goto setErr_filter;
			}
			if ( !apmib_set(del_id, pEntry)) {
				strcpy(tmpBuf, ("Delete table entry error!"));
				goto setErr_filter;
			}
		}
	}
	 
	 	apmib_update_web(CURRENT_SETTING);
		
#if !defined(MULTI_WAN_SUPPORT)
#ifndef NO_ACTION
		pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
		}
			else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
			execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
					exit(1);
			}
#endif
#endif
	
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
	#if defined(MULTI_WAN_SUPPORT)
		OK_MSG(submitUrl);
		return;
	#else
#ifdef REBOOT_CHECK
		if(needReboot == 1)
		{
			OK_MSG(submitUrl);
			return;
		}
#endif
	
		if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
		return;
	#endif
	setErr_filter:
	    //ERR_MSG(tmpBuf);
		apmib_update_web(CURRENT_SETTING);
		#if !defined(MULTI_WAN_SUPPORT)
#ifndef NO_ACTION
		pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
		}
			else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
			execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
					exit(1);
			}
#endif
#endif
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
		fprintf(stderr,"%s\n",tmpBuf);
}

void heUIformALGOnOff(request * wp, char *path, char *query)
{
	char	*str, *submitUrl;
	char	*str_alg;
	char tmpBuf[100];
	bool enable;
	
	str = req_get_cstream_var(wp, "ftpEn", "");
	//printf("---------------------%s:%d    ftpEn:%s--------------------\n",__FUNCTION__,__LINE__,str);
		
	if(str[0])
	{
		if ((atoi(str)==1)||!strcmp(str, "on"))
			enable = 1;
		else
			enable = 0;
		rtk_alg_set("ftp",enable);
	}

	str = req_get_cstream_var(wp, "sipEn", "");
	//printf("---------------------%s:%d    sipEn:%s--------------------\n",__FUNCTION__,__LINE__,str);
		
	if(str[0])
	{
		if ((atoi(str)==1)||!strcmp(str, "on"))
			enable = 1;
		else
			enable = 0;
		rtk_alg_set("sip",enable);
	}

	str = req_get_cstream_var(wp, "rtspEn", "");
	//printf("---------------------%s:%d    rtspEn:%s--------------------\n",__FUNCTION__,__LINE__,str);
		
	if(str[0])
	{
		if ((atoi(str)==1)||!strcmp(str, "on"))
			enable = 1;
		else
			enable = 0;
		rtk_alg_set("rtsp",enable);
	}

	str = req_get_cstream_var(wp, "l2tpEn", "");
	//printf("---------------------%s:%d    l2tpEn:%s--------------------\n",__FUNCTION__,__LINE__,str);
		
	if(str[0])
	{
		if ((atoi(str)==1)||!strcmp(str, "on"))
			enable = 1;
		else
			enable = 0;
		rtk_alg_set("l2tp",enable);
	}


	apmib_update_web(CURRENT_SETTING);
	submitUrl = req_get_cstream_var(wp, "submit-url", "");   // hidden page
	//printf("%s:%d get submit=%s \n",__FUNCTION__,__LINE__,submitUrl);
#ifdef REBOOT_CHECK
	if(needReboot == 1)
	{
		OK_MSG(submitUrl);
		return;
	}
#endif

	if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
	return;

return;

setErr:
	//ERR_MSG(tmpBuf);
	fprintf(stderr,"%s\n",tmpBuf);
}


void heUITelnetd(request * wp, char *path, char *query)
{
	system("telnetd");
	char *submitUrl = "/herouter/telnetd.asp";
	send_redirect_perm(wp, submitUrl);

	return;
}

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
	if(protocol == NULL)
		return -1;
	enable=!enable;
	if(strcmp("ftp",protocol)==0)
		apmib_set(MIB_ALG_FTP_DISABLE, (void *) &enable);
	else if(strcmp("tftp",protocol)==0)
		apmib_set(MIB_ALG_TFTP_DISABLE, (void *) &enable);
	else if(strcmp("rtsp",protocol)==0)
		apmib_set(MIB_ALG_RTSP_DISABLE, (void *) &enable);
	else if(strcmp("pptp",protocol)==0)
		apmib_set(MIB_ALG_PPTP_DISABLE, (void *) &enable);
	else if(strcmp("l2tp",protocol)==0)
		apmib_set(MIB_ALG_L2TP_DISABLE, (void *) &enable);
	else if(strcmp("ipsec",protocol)==0)
		apmib_set(MIB_ALG_IPSEC_DISABLE, (void *) &enable);
	else if(strcmp("sip",protocol)==0)
		apmib_set(MIB_ALG_SIP_DISABLE, (void *) &enable);
	else if(strcmp("h323",protocol)==0)
		apmib_set(MIB_ALG_H323_DISABLE, (void *) &enable);
	else
		return -1;
	sprintf(buf,"echo %s %d > /proc/alg\n",protocol,!enable);
	system(buf);
	return 0;
}


void QuickSetUp(request *wp, char *path, char *query)
{
	char  *connectionType=NULL,*pppUserName=NULL,*pppPsw=NULL;
	int wan_connect_mode=0;
	char errMsg[256]={0};

	connectionType = req_get_cstream_var(wp, ("connectionType"), "");

	if(strcmp(connectionType,"PPPOE")==0){
		wan_connect_mode=PPPOE;
		pppUserName=req_get_cstream_var(wp, ("pppoeUser"), "");
		if(!pppUserName[0]){
			snprintf(errMsg,sizeof(errMsg),"invalid pppoeUser is null");
			goto QuickSetUp_FAIL;
		}
		pppPsw=req_get_cstream_var(wp, ("pppoePass"), "");
		if(!pppPsw[0]){
			snprintf(errMsg,sizeof(errMsg),"invalid pppoePass is null");
			goto QuickSetUp_FAIL;
		}
		if(apmib_set(MIB_PPP_USER_NAME,(void*)pppUserName)==0){
			snprintf(errMsg,sizeof(errMsg),"get mib MIB_PPP_USER_NAME fail!");
			goto QuickSetUp_FAIL;
		}
		if(apmib_set(MIB_PPP_PASSWORD,(void*)pppPsw)==0){
			snprintf(errMsg,sizeof(errMsg),"get mib MIB_PPP_PASSWORD fail!");
			goto QuickSetUp_FAIL;
		}
	}else if(strcmp(connectionType,"DHCP")==0){
		wan_connect_mode=DHCP_CLIENT;
	}else{
		snprintf(errMsg,sizeof(errMsg),"invalid connectionType=%s",connectionType);
		goto QuickSetUp_FAIL;
	}
	if(apmib_set(MIB_WAN_DHCP,(void*)&wan_connect_mode)==0){
		snprintf(errMsg,sizeof(errMsg),"get mib MIB_WAN_DHCP fail!");
		goto QuickSetUp_FAIL;
	}
	send_redirect_perm(wp,"/herouter/intstatus.asp?quicksetup=1");
	
	return;
QuickSetUp_FAIL:
	ERR_MSG(errMsg);
}
void netState(request *wp, char *path, char *query)
{
	char strWanIP[16]={0};
	char strWanMask[16]={0};
	char strWanDefIP[16]={0};
	char strWanHWAddr[18]={0};
	int siteAccess=0;
	if(getWanLink("eth1")<0){
		req_format_write(wp,"cablePlug:fail\n");
		req_format_write(wp,"WanIp:fail\n");
		req_format_write(wp,"siteAccess:fail\n");
		req_format_write(wp,"PPPOEStatus:fail\n");
		return;
	}
	req_format_write(wp,"cablePlug:pass\n");

	getWanInfo(strWanIP,strWanMask,strWanDefIP,strWanHWAddr);
	if(strcmp(strWanIP,"0.0.0.0")==0){
		req_format_write(wp,"WanIp:fail\n");
		req_format_write(wp,"siteAccess:fail\n");
		req_format_write(wp,"PPPOEStatus:fail\n");
		return;
	}

	req_format_write(wp,"WanIp:pass\n");	
	//checkWanping(BAIDU_SERVER,SOHU_SERVER,NETEASE_SERVER);
	apmib_get(MIB_SITE_ACCESS,(void*)&siteAccess);

	if(!siteAccess)
		req_format_write(wp,"siteAccess:fail\n");
	else
		req_format_write(wp,"siteAccess:pass\n");

	if ( !isConnectPPP() )
		req_format_write(wp,"PPPOEStatus:fail\n");
	else
		req_format_write(wp,"PPPOEStatus:pass\n");
}

void netDiagnosis(request *wp, char *path, char *query)
{
	int wanLink = 0;
	char strWanIP[16]={0};
	char strWanMask[16]={0};
	char strWanDefIP[16]={0};
	char strWanHWAddr[18]={0};
	int siteAccess=0;
	struct in_addr intaddr;

	OPMODE_T opmode=-1;
	apmib_get(MIB_OP_MODE, (void *)&opmode);

	if(opmode!=BRIDGE_MODE)	
	{	
		if(getWanLink("eth1")<0){
			wanLink = 0;
		} else {
			wanLink = 1;
		}

		if(wanLink == 0){
			req_format_write(wp,"cablePlug:fail\n");
			req_format_write(wp,"WanIp:fail\n");
			req_format_write(wp,"siteAccess:fail\n");
		} else
		{		
			req_format_write(wp,"cablePlug:pass\n");
			getWanInfo(strWanIP,strWanMask,strWanDefIP,strWanHWAddr);
			if(strcmp(strWanIP,"0.0.0.0")==0){			
				req_format_write(wp,"WanIp:fail\n");			
				req_format_write(wp,"siteAccess:fail\n");
			} else {
				req_format_write(wp,"WanIp:pass\n");
				siteAccess=checkWanping(BAIDU_SERVER,SOHU_SERVER,NETEASE_SERVER);
				apmib_set(MIB_SITE_ACCESS,(void*)&siteAccess);
				if(!siteAccess)
					req_format_write(wp,"siteAccess:fail\n");
				else
					req_format_write(wp,"siteAccess:pass\n");
			}
		}
	}
	
	/*lanNetwork*/
	if (!getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr )) {		
		if(opmode==BRIDGE_MODE)	
		{	
			//bridge mode, only don't check cablePlug and WanIp
			req_format_write(wp,"cablePlug:pass\n");
			req_format_write(wp,"WanIp:fail\n");			
			req_format_write(wp,"siteAccess:fail\n");			
		}	
		req_format_write(wp,"lanNetwork:fail\n");
	} else {		
		if(opmode==BRIDGE_MODE)	
		{	
			//bridge mode, only don't check cablePlug and WanIp
			req_format_write(wp,"cablePlug:pass\n");
			req_format_write(wp,"WanIp:pass\n");			
		
			siteAccess=checkWanping(BAIDU_SERVER,SOHU_SERVER,NETEASE_SERVER);
			apmib_set(MIB_SITE_ACCESS,(void*)&siteAccess);
			if(!siteAccess)
				req_format_write(wp,"siteAccess:fail\n");
			else
				req_format_write(wp,"siteAccess:pass\n");
		}
		req_format_write(wp,"lanNetwork:pass\n");
	}	

	/*USBDisk*/
	req_format_write(wp,"USBDisk:0\n");

	
	/*LAN1*/
	if(checkLink(LAN0_PHY_PORT)==1){	
		req_format_write(wp,"LAN0:pass\n");
	} else {
		req_format_write(wp,"LAN0:fail\n");
	}
	
	/*LAN2*/
	if(checkLink(LAN1_PHY_PORT)==1){
		req_format_write(wp,"LAN1:pass\n");
	} else {
		req_format_write(wp,"LAN1:fail\n");
	}

	/*LAN3*/
	if(checkLink(LAN2_PHY_PORT)==1){
		req_format_write(wp,"LAN2:pass\n");
	} else {
		req_format_write(wp,"LAN2:fail\n");
	}	

	/*LAN4*/
	if(checkLink(LAN3_PHY_PORT)==1){
		req_format_write(wp,"LAN3:pass\n");
	} else {
		req_format_write(wp,"LAN3:fail\n");
	}	
}

void doInitInternet(request *wp, char *path, char *query)
{
	apmib_update_web(CURRENT_SETTING);
	run_init_script_flag = 1;
	run_init_script("all");
}
void heUiReboot(request *wp, char *path, char *query)
{
	REBOOT_WAIT_COMMAND(2);
}
#if 0
int find_head_offset(char *upload_data)
{
	int head_offset=0 ;
	char *pStart=NULL;
	int iestr_offset=0;
	char *dquote;
	char *dquote1;
	
	if (upload_data==NULL) {
		//fprintf(stderr, "upload data is NULL\n");
		return -1;
	}

	pStart = strstr(upload_data, WINIE6_STR);
	if (pStart == NULL) {
		pStart = strstr(upload_data, LINUXFX36_FWSTR);
		if (pStart == NULL) {
			pStart = strstr(upload_data, MACIE5_FWSTR);
			if (pStart == NULL) {
				pStart = strstr(upload_data, OPERA_FWSTR);
				if (pStart == NULL) {
					pStart = strstr(upload_data, "filename=");
					if (pStart == NULL) {
						return -1;
					}
					else {
						dquote =  strstr(pStart, "\"");
						if (dquote !=NULL) {
							dquote1 = strstr(dquote, LINE_FWSTR);
							if (dquote1!=NULL) {
								iestr_offset = 4;
								pStart = dquote1;
							}
							else {
								return -1;
							}
						}
						else {
							return -1;
						}
					}
				}
				else {
					iestr_offset = 16;
				}
			} 
			else {
				iestr_offset = 14;
			}
		}
		else {
			iestr_offset = 26;
		}
	}
	else {
		iestr_offset = 17;
	}
	//fprintf(stderr,"####%s:%d %d###\n",  __FILE__, __LINE__ , iestr_offset);
	head_offset = (int)(((unsigned long)pStart)-((unsigned long)upload_data)) + iestr_offset;
	return head_offset;
}
#endif
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
	#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
	    // do nothing
	#else
		shutdown_netdev();
		system("ifconfig br0 down 2> /dev/null");
	#endif
		killDaemon(1);
#endif

if(!upload_data || !buffer || upload_len<=0 || bufferLen<=0 || upload_len>CONFIG_FLASH_SIZE){
	  return 0;
	}

while(head_offset +sizeof(IMG_HEADER_T) <   upload_len) {
    locWrite = 0;
    pHeader = (IMG_HEADER_Tp) &upload_data[head_offset];
    len = pHeader->len;
#ifdef _LITTLE_ENDIAN_
    len  = DWORD_SWAP(len);
#endif    
    numLeft = len + sizeof(IMG_HEADER_T) ;
    
	//printf("%s:%d FW_HEADER=%s FW_HEADER_WITH_ROOT=%s WEB_HEADER=%s ROOT_HEADER=%s head_offset=%d,upload_len=%d\n",
		//__FUNCTION__,__LINE__,FW_HEADER,FW_HEADER_WITH_ROOT,WEB_HEADER,ROOT_HEADER,head_offset,upload_len);
//	printf("%c%c%c%c\n",upload_data[head_offset],upload_data[head_offset+1],upload_data[head_offset+2],upload_data[head_offset+3]);
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
		if(flag)
			break;
       	snprintf(buffer,bufferLen, "Invalid file format [%s-%d]!",__FILE__,__LINE__);
		goto ret_upload;
    }

	//printf("%s:%d flag=%d\n",__FUNCTION__,__LINE__,flag);

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
		  return 0;
	  	}
	  	shm_memory = (char *)shmat(shm_id, NULL, 0);
	  	memcpy(shm_memory,upload_data,upload_len);			
		sprintf(tmpStr,"%d",shm_id);			
		write_line_to_file("/var/fwd.ready", 1, tmpStr);			
		sync();
	}
#endif
  return 1;
  ret_upload:
  	if(fh>0) close(fh);
	return 0;
}

void heUIUpload(request *wp, char *path, char *query)
{
	char *uploadData=NULL;
	int uploadDataLen=0;
	char errmsg[128]={0};
	
	//printf("%s:%d upload_data=0x%p\n",__FUNCTION__,__LINE__,wp->upload_data);
	int head_offset = find_head_offset((char *)wp->upload_data);
	if (head_offset == -1) {
		fprintf(stderr,"invalid file format!\n");
		return;
	}
	//printf("%s:%d head_offset=%d upload_len=%d\n",__FUNCTION__,__LINE__,head_offset,wp->upload_len);
	uploadData=wp->upload_data+head_offset;
	uploadDataLen= wp->upload_len-head_offset;
	//printf("%c%c%c%c%c%c%c%c\n",uploadData[0],uploadData[1],uploadData[2],uploadData[3],uploadData[4],uploadData[5],uploadData[6],uploadData[7]);
	if(upgrade_firmware(uploadData,uploadDataLen,errmsg,sizeof(errmsg))!=1){
		fprintf(stderr,"%s",errmsg);
		return;
	}
	send_redirect_perm(wp,"herouter/sysupdate.asp?uploadingFirmware=1");
}
#define IP_DOT_FMT(ipaddr) ((unsigned char*) &ipaddr)[0],\
	((unsigned char*) &ipaddr)[1],\
	((unsigned char*) &ipaddr)[2],\
	((unsigned char*) &ipaddr)[3]
#define IP_DOT_FMT_MASK(ipaddr) ((unsigned char*) &ipaddr)[0],\
		((unsigned char*) &ipaddr)[1],\
		((unsigned char*) &ipaddr)[2]
		
void dhcpbanding(request *wp, char *path, char *query)
{
	int enabled, lan_dhcp_type, lease_time;
	char *dhcphidden=NULL, *startip1=NULL, *endip1=NULL, *entconfiglan=NULL;
	char *startip4=NULL, *endip4=NULL, *leasetime=NULL;
	char startip[16]={0}, endip[16]={0}, newip[64]={0},newip_1[64]={0},newip_2[64]={0},newip_mask[64]={0};
	struct in_addr inIp={0};
	struct in_addr ip_addr;	
	char errMsg[256]={0};
	unsigned int opmode = 0;
	unsigned int lan_ip;
	char lan_ip_s[24]={0},lan_ip_s_mask[24]={0}; 
	char lanip0[8]={0},lanip1[8]={0},lanip2[8]={0},lanip3[8]={0};
	char cmd[64]={0};
	
	apmib_get(MIB_OP_MODE, (void *)&opmode);
	apmib_get(MIB_IP_ADDR, (void *)&lan_ip);
	sprintf(lan_ip_s,"%u.%u.%u.%u",IP_DOT_FMT(lan_ip));
	sprintf(lan_ip_s_mask,"%u.%u.%u",IP_DOT_FMT_MASK(lan_ip));
	printf("lan_ip_s:%s,lan_ip_s_mask:%s\n",lan_ip_s,lan_ip_s_mask);


	entconfiglan = req_get_cstream_var(wp, ("lanIp"), "");
	sprintf(newip,"%s",entconfiglan);
	sscanf(newip,"%[^.].%[^.].%[^.].%s",lanip0,lanip1,lanip2,lanip3);
	printf("lanip0: %s %s %s %s\n",lanip0,lanip1,lanip2,lanip3);
	sprintf(newip_mask,"%s.%s.%s",lanip0,lanip1,lanip2);
	sprintf(newip_1,"%s.%s.%s.1",lanip0,lanip1,lanip2);
	sprintf(newip_2,"%s.%s.%s.253",lanip0,lanip1,lanip2);
	
	
#ifdef CONFIG_STATIC_IP		
	sprintf(cmd,"ifconfig br0:1 %s.1 netmask 255.255.255.0 broadcast %s.255 down",lan_ip_s_mask,lan_ip_s_mask);
	system(cmd);
#endif
    if(entconfiglan)
    {
#ifdef CONFIG_STATIC_IP					
		if(opmode !=0)
		{
		
        printf("newip: %s,newip_1:%s\n", newip,newip_1);
			if(strcmp(newip,newip_1)==0)
				entconfiglan = newip_2;
		}
#endif
		if ( !inet_aton(entconfiglan, &inIp) ) 
		{
            strcpy(errMsg, ("Invalid IP-address value!"));
            goto dhcpbanding_FAIL;
        }
        if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp)) 
		{
            strcpy(errMsg, ("Set IP-address error!"));
            goto dhcpbanding_FAIL;
        }
    }
	else
    {
        snprintf(errMsg,sizeof(errMsg),"Get lan IP fail!!\n");
        goto dhcpbanding_FAIL;
    }

	dhcphidden = req_get_cstream_var(wp, ("dhcphidden"), "");
	if(dhcphidden)
	{
		enabled=atoi(dhcphidden);
		if(enabled)
			lan_dhcp_type=DHCP_SERVER;
		else
			lan_dhcp_type=DHCP_DISABLED;
		
		if(!apmib_set(MIB_DHCP, (void *)&lan_dhcp_type))
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_DHCP fail!\n");
			goto dhcpbanding_FAIL;
		}
	}

	startip1 = req_get_cstream_var(wp, ("startip1"), "");	
	startip4 = req_get_cstream_var(wp, ("startip4"), "");

	if(startip1 && startip4)
	{
		//printf("\n%s:%d startip1=%s startip4=%s\n",__FUNCTION__,__LINE__,startip1,startip4);
		snprintf(startip, sizeof(startip), "%s.%s", startip1, startip4);
		
		//printf("\n%s:%d startip=%s\n",__FUNCTION__,__LINE__,startip);

		inet_aton(startip, &ip_addr);
		if(!apmib_set(MIB_DHCP_CLIENT_START, (void *)&ip_addr))	
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_DHCP_CLIENT_START fail!\n");
			goto dhcpbanding_FAIL;
		}
	}

	endip1 = req_get_cstream_var(wp, ("endip1"), "");	
	endip4 = req_get_cstream_var(wp, ("endip4"), "");

	if(endip1 && endip4)
	{
		//printf("\n%s:%d endip1=%s endip4=%s\n",__FUNCTION__,__LINE__,endip1,endip4);
		snprintf(endip, sizeof(endip), "%s.%s", endip1, endip4);	
		
		//printf("\n%s:%d endip=%s\n",__FUNCTION__,__LINE__,endip);

		inet_aton(endip, &ip_addr);
		if(!apmib_set(MIB_DHCP_CLIENT_END, (void *)&ip_addr))	
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_DHCP_CLIENT_END fail!\n");
			goto dhcpbanding_FAIL;
		}
	}

	leasetime = req_get_cstream_var(wp, ("leasetime"), "");
	if(leasetime)
	{		
		//printf("\n%s:%d leasetime=%s\n",__FUNCTION__,__LINE__,leasetime);
		lease_time=atoi(leasetime);		
		lease_time=lease_time*60;
		if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&lease_time))	
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_DHCP_LEASE_TIME fail!\n");
			goto dhcpbanding_FAIL;
		}
	}

	apmib_update_web(CURRENT_SETTING);
	run_init_script_flag = 1;
	run_init_script("all");
	
	return ;

dhcpbanding_FAIL:
	ERR_MSG(errMsg);
}

void MACCloneCfg(request *wp, char *path, char *query)
{
	char *macCloneMac=NULL;	
	char str_mac[16]={0}, wan_mac[6]={0};
	int i,j=0;
	char errMsg[256]={0};	

	macCloneMac = req_get_cstream_var(wp, ("macCloneMac"), "");

	if(macCloneMac)
	{		
		//printf("\n%s:%d macCloneMac=%s\n",__FUNCTION__,__LINE__,macCloneMac);
		
		for(i=0;i<17;i++)
			if(macCloneMac[i]!=':')
				str_mac[j++]=macCloneMac[i];

		if (strlen(str_mac)!=12 || !string_to_hex(str_mac, wan_mac, 12)) 
		{
			snprintf(errMsg,sizeof(errMsg),"Invalid mac address!\n");
			goto MACCloneCfg_FAIL;
		}

		if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)wan_mac)) 
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_WAN_MAC_ADDR fail!\n");
			goto MACCloneCfg_FAIL;
		}		
	}

	
	doInitInternet(wp,path,query);

	return;
	
MACCloneCfg_FAIL:
	ERR_MSG(errMsg);
}

void setWan(request *wp, char *path, char *query)
{
	char *conTp_radio=NULL;	
	char *staticIp=NULL, *staticNetmask=NULL, *staticGateway=NULL, *staticPriDns=NULL, *staticSecDns=NULL;
	char *pppoeUser=NULL, *pppoePass=NULL, *MTU=NULL;
	struct in_addr ip_addr, mask_addr, gw_addr, pridns_addr, secdns_addr;
	int wan_type;
	char errMsg[256]={0};	

	conTp_radio = req_get_cstream_var(wp, ("conTp_radio"), "");
	
	//printf("\n%s:%d conTp_radio=%s\n",__FUNCTION__,__LINE__,conTp_radio);
	
	if(conTp_radio==NULL)
	{
		snprintf(errMsg,sizeof(errMsg),"Don't select wan dial type!\n");
		goto setWan_FAIL;
	}

	if(strcmp(conTp_radio, "0")==0)
		wan_type=DHCP_CLIENT;
	else if(strcmp(conTp_radio, "1")==0)
		wan_type=DHCP_DISABLED;
	else if(strcmp(conTp_radio, "2")==0)
		wan_type=PPPOE;
	else
	{
		snprintf(errMsg,sizeof(errMsg),"Invalid wan type!\n");
		goto setWan_FAIL;
	}

	//printf("\n%s:%d wan_type=%d\n",__FUNCTION__,__LINE__,wan_type);

	if ( !apmib_set(MIB_WAN_DHCP, (void *)&wan_type)) 
	{
		snprintf(errMsg,sizeof(errMsg),"set MIB_WAN_DHCP fail!\n");
		goto setWan_FAIL;
	}	

	if(wan_type==DHCP_CLIENT)
	{
		//need not set mib
	}
	else if(wan_type==DHCP_DISABLED)
	{
		staticIp = req_get_cstream_var(wp, ("staticIp"), "");
		if (staticIp[0]) 
		{			
			//printf("\n%s:%d staticIp=%s\n",__FUNCTION__,__LINE__,staticIp);
			if ( !inet_aton(staticIp, &ip_addr) ) 
			{
				strcpy(errMsg, ("Invalid IP-address value!"));
				goto setWan_FAIL;
			}
			if ( !apmib_set(MIB_WAN_IP_ADDR, (void *)&ip_addr)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_WAN_IP_ADDR fail!\n");
				goto setWan_FAIL;
			}
		}

		staticNetmask = req_get_cstream_var(wp, ("staticNetmask"), "");
		if ( staticNetmask[0] ) 
		{
			//printf("\n%s:%d staticNetmask=%s\n",__FUNCTION__,__LINE__,staticNetmask);
			if ( !inet_aton(staticNetmask, &mask_addr) ) 
			{
				strcpy(errMsg, ("Invalid subnet-mask value!"));
				goto setWan_FAIL;
			}
			if ( !apmib_set(MIB_WAN_SUBNET_MASK, (void *)&mask_addr)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_WAN_SUBNET_MASK fail!\n");
				goto setWan_FAIL;
			}
		}

		staticGateway = req_get_cstream_var(wp, ("staticGateway"), "");
		if ( staticGateway[0] ) 
		{
			//printf("\n%s:%d staticGateway=%s\n",__FUNCTION__,__LINE__,staticGateway);
			if ( !inet_aton(staticGateway, &gw_addr) ) 
			{
				strcpy(errMsg, ("Invalid gateway value!"));
				goto setWan_FAIL;
			}
			if ( !apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&gw_addr)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_WAN_DEFAULT_GATEWAY fail!\n");
				goto setWan_FAIL;
			}
		}

		staticPriDns = req_get_cstream_var(wp, ("staticPriDns"), "");
		if ( staticPriDns[0] ) 
		{			
			//printf("\n%s:%d staticPriDns=%s\n",__FUNCTION__,__LINE__,staticPriDns);
			if ( !inet_aton(staticPriDns, &pridns_addr) ) 
			{
				strcpy(errMsg, ("Invalid primary dns value!"));
				goto setWan_FAIL;
			}
			if ( !apmib_set(MIB_DNS1, (void *)&pridns_addr)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_DNS1 fail!\n");
				goto setWan_FAIL;
			}
		}

		staticSecDns = req_get_cstream_var(wp, ("staticSecDns"), "");
		if ( staticSecDns[0] ) 
		{			
			//printf("\n%s:%d staticSecDns=%s\n",__FUNCTION__,__LINE__,staticSecDns);
			if ( !inet_aton(staticSecDns, &secdns_addr) ) 
			{
				strcpy(errMsg, ("Invalid second dns value!"));
				goto setWan_FAIL;
			}
			if ( !apmib_set(MIB_DNS2, (void *)&secdns_addr)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_DNS2 fail!\n");
				goto setWan_FAIL;
			}
		}		
	}
	else if(wan_type==PPPOE)
	{
		pppoeUser = req_get_cstream_var(wp, ("pppoeUser"), "");
		if ( pppoeUser[0] ) 
		{			
			//printf("\n%s:%d pppoeUser=%s\n",__FUNCTION__,__LINE__,pppoeUser);
			if ( !apmib_set(MIB_PPP_USER_NAME, (void *)pppoeUser)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_PPP_USER_NAME fail!\n");
				goto setWan_FAIL;
			}
		}

		pppoePass = req_get_cstream_var(wp, ("pppoePass"), "");
		if ( pppoePass[0] ) 
		{			
			//printf("\n%s:%d pppoePass=%s\n",__FUNCTION__,__LINE__,pppoePass);
			if ( !apmib_set(MIB_PPP_PASSWORD, (void *)pppoePass)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_PPP_PASSWORD fail!\n");
				goto setWan_FAIL;
			}
		}

		MTU = req_get_cstream_var(wp, ("MTU"), "");
		if ( MTU[0] ) 
		{			
			//printf("\n%s:%d MTU=%s\n",__FUNCTION__,__LINE__,MTU);
			int mtuSize;
			mtuSize = strtol(MTU, (char**)NULL, 10);
			if ( !apmib_set(MIB_PPP_MTU_SIZE, (void *)&mtuSize)) 
			{
				snprintf(errMsg,sizeof(errMsg),"set MIB_PPP_MTU_SIZE fail!\n");
				goto setWan_FAIL;
			}
		}
	}
	
	apmib_update_web(CURRENT_SETTING);
	
	send_redirect_perm(wp,"/herouter/intstatus.asp?quicksetup=0");

	return;
	
setWan_FAIL:
	ERR_MSG(errMsg);
}

void entconfig(request *wp, char *path, char *query)
{
	struct in_addr inIp={0};
	struct in_addr startIp, endIp, netmask;
	unsigned int net_addr, host_addr;
	char errMsg[256]={0};
	char * entconfiglan = req_get_cstream_var(wp, ("entconfiglan"), "");
	if ( entconfiglan[0] ) {
			if ( !inet_aton(entconfiglan, &inIp) ) {
				strcpy(errMsg, ("Invalid IP-address value!"));
				goto entconfig_FAIL;
			}
			if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp)) {
				strcpy(errMsg, ("Set IP-address error!"));
				goto entconfig_FAIL;
			}

			if (!apmib_get(MIB_DHCP_CLIENT_START, (void *)&startIp))
			{
				strcpy(errMsg, ("get MIB_DHCP_CLIENT_START fail!"));
				goto entconfig_FAIL;
			}

			if (!apmib_get(MIB_DHCP_CLIENT_END, (void *)&endIp))
			{
				strcpy(errMsg, ("get MIB_DHCP_CLIENT_END fail!"));
				goto entconfig_FAIL;
			}

			if (!apmib_get(MIB_SUBNET_MASK, (void *)&netmask))
			{
				strcpy(errMsg, ("get MIB_SUBNET_MASK fail!"));
				goto entconfig_FAIL;
			}

			net_addr=(inIp.s_addr & netmask.s_addr);
			host_addr= (startIp.s_addr & (~ netmask.s_addr));
			startIp.s_addr=(net_addr | host_addr);

			if (!apmib_set(MIB_DHCP_CLIENT_START, (void *)&startIp))
			{
				strcpy(errMsg, ("set MIB_DHCP_CLIENT_START fail!"));
				goto entconfig_FAIL;
			}

			host_addr= (endIp.s_addr & (~ netmask.s_addr));
			endIp.s_addr=(net_addr | host_addr);

			if (!apmib_set(MIB_DHCP_CLIENT_END, (void *)&endIp))
			{
				strcpy(errMsg, ("set MIB_DHCP_CLIENT_END fail!"));
				goto entconfig_FAIL;
			}
	}
	doInitInternet(wp,path,query);
	return;
entconfig_FAIL:
	ERR_MSG(errMsg);
}
void LoadDefaultSettings(request *wp, char *path, char *query)
{
	system("flash default-sw");
	doInitInternet(wp,path,query);
}
void setSysAdm(request *wp, char *path, char *query)
{
	char pasword[MAX_NAME_LEN]={0};
	char superpasword[MAX_NAME_LEN]={0};
	char errMsg[256]={0};
	char * oldpass = req_get_cstream_var(wp, ("oldpass"), "");
	char *admpass = req_get_cstream_var(wp, ("admpass"), "");
	int issuperpwd = 0;
	
	if(wp->auth_flag == 1)
		{
			if(!apmib_get(MIB_SUPER_PASSWORD,(void*)superpasword)){
				strcpy(errMsg, ("ERROR: Get user password from MIB database failed."));
				goto setSysAdm_ERR;
			}
			if(!apmib_get(MIB_USER_PASSWORD,(void*)pasword)){
				strcpy(errMsg, ("ERROR: Get user password from MIB database failed."));
				goto setSysAdm_ERR;
			}
			
			if( oldpass != NULL )
				{
					if(strcmp(pasword,oldpass) == 0){
						issuperpwd = 2;
					}

					if(strcmp(oldpass,superpasword) == 0){
						issuperpwd = 1;
					}	
				}
			if(issuperpwd == 0)
				{
				send_redirect_perm(wp,"/herouter/changepassword.asp?oldpass_error=1");
				return;
			}
			
		}
	else
		{
		
			if(!apmib_get(MIB_USER_PASSWORD,(void*)pasword)){
				strcpy(errMsg, ("ERROR: Get user password from MIB database failed."));
				goto setSysAdm_ERR;
			}
			
			//fprintf(stderr,"%s:%d oldpass=%s pasword=%s\n",__FUNCTION__,__LINE__,oldpass,pasword);
			if(strcmp(pasword,oldpass)!=0){
				//fprintf(stderr,"%s:%d oldpass=%s pasword=%s\n",__FUNCTION__,__LINE__,oldpass,pasword);
				send_redirect_perm(wp,"/herouter/changepassword.asp?oldpass_error=1");
				return;
			}
		}
	if(wp->auth_flag == 2 || issuperpwd == 2)
		{
			if ( !apmib_set(MIB_USER_PASSWORD, (void *)admpass) ) {
				strcpy(errMsg, ("ERROR: Set user password to MIB database failed."));
				goto setSysAdm_ERR;
			}
		}
	else if(wp->auth_flag == 1)
		{
		if ( !apmib_set(MIB_SUPER_PASSWORD, (void *)admpass) ) {
			strcpy(errMsg, ("ERROR: Set user password to MIB database failed."));
			goto setSysAdm_ERR;
		}

	}
	
	send_redirect_perm(wp,"/herouter/changepassword.asp?oldpass_error=0");
	apmib_update_web(CURRENT_SETTING);
	return;
setSysAdm_ERR:
	ERR_MSG(errMsg);
}

#define SYSLOG_FILE "/var/log/messages"
void he_form_syslog(request *wp, char *path, char *query)
{
	FILE *fp;
	struct stat file_stat;
	char buf[1024];
	char tmpBuf[100];
	unsigned int i = 1;

	if (stat(SYSLOG_FILE, &file_stat) != 0) {
		snprintf(tmpBuf, sizeof(tmpBuf), "Stat file %s failed :%s\n", SYSLOG_FILE, strerror(errno));
		goto setErr_end;
	}
	
	fp = fopen(SYSLOG_FILE, "r");
	if (fp == NULL) {
		snprintf(tmpBuf, sizeof(tmpBuf),"Fopen file %s failed :%s\n", SYSLOG_FILE, strerror(errno));
		goto setErr_end;
	}

	while (!feof(fp)) {
	    if (fgets(buf,sizeof(buf),fp))
			req_format_write(wp, ("<tr><td>%d</td><td>%s</td></tr>"),i++, buf);
	}
	fclose(fp);
	return;

setErr_end:
	req_format_write(wp, ("<tr><td>%s</td></tr>"), tmpBuf);
}

static void restart_ntp(char *host)
{
	char cmdbuf[64];
	
	if (host == NULL)
		return;
	
	unlink("/tmp/ntp_tmp");
	system("killall -9 ntp_inet 2>/dev/null");
	snprintf(cmdbuf, sizeof(cmdbuf), "ntp_inet -x %s 2>/dev/null", host);
	system(cmdbuf);
}

void he_setSysTime(request *wp, char *path, char *query)
{
	char *time_radio;
	unsigned char timeRadio;
	char tmpBuf[100];
	char *submitUrl;
	struct in_addr ip_addr;
	char enabled = 1;
	
	time_radio = req_get_cstream_var(wp, ("time_radio"), "");
	
	if (!time_radio[0]) {
		snprintf(tmpBuf, sizeof(tmpBuf), "Error, time_radio is null!\n");
		goto setErr_end;
	}
	
	timeRadio = atoi(time_radio);
	
	if (timeRadio) { /*NTP Setting*/
		char * ntp_server_ip;
		char *ntp_sync;
		unsigned int ntpSync;
		int pid;
		
		ntp_server_ip = req_get_cstream_var(wp, ("NTPServerIP"), "");
		ntp_sync = req_get_cstream_var(wp, ("NTPSync"), "");
		if (ntp_server_ip[0]==0 || ntp_sync[0]==0) {
			snprintf(tmpBuf, sizeof(tmpBuf), "Error, ntp_server_ip or ntp_sync is null!\n");
			goto setErr_end;
		}
		
		ntpSync = atoi(ntp_sync) * 3600;
		if (apmib_set( MIB_NTP_ENABLED, (void *)&enabled) == 0) {
			snprintf(tmpBuf, sizeof(tmpBuf), "Set NTP enabled flag error!");
			goto setErr_end;
		}
		//snprintf(host_ip, sizeof(host_ip), "%s", inet_ntoa(*(struct in_addr *)ntpserver->h_addr));
		
		if (apmib_set(MIB_NTP_SERVER_IP1, (void *)ntp_server_ip) == 0) {
			snprintf(tmpBuf, sizeof(tmpBuf), "Set NTP server error!");
			goto setErr_end;
		} 
		
		if (apmib_set(MIB_NTP_SYNC_INTERVAL, (void *)&ntpSync) == 0) {
			snprintf(tmpBuf, sizeof(tmpBuf), "Set NTP Sync time error!");
			goto setErr_end;	
		}

		apmib_update_web(CURRENT_SETTING);
		restart_ntp(ntp_server_ip);	
	} else { /*Time Setting*/
		char *strVal;
		struct tm tm_time;
		time_t tm;
		unsigned int time_value=0;
		
		strVal = req_get_cstream_var(wp, ("ymd"), "");
		if (sscanf(strVal, "%d-%d-%d", &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday) != 3) {
			snprintf(tmpBuf, sizeof(tmpBuf), "Error, Get ymd(%s) failed!\n", strVal);
			goto setErr_end;
		}
		tm_time.tm_year -= 1900;
		tm_time.tm_mon -= 1;
		
		strVal = req_get_cstream_var(wp, ("hour"), "");
		tm_time.tm_hour = atoi(strVal);
		strVal = req_get_cstream_var(wp, ("minute"), "");
		tm_time.tm_min = atoi(strVal);
		strVal = req_get_cstream_var(wp, ("second"), "");
		tm_time.tm_sec = atoi(strVal);
		tm = mktime(&tm_time);
		
		if (tm < 0) {
			snprintf(tmpBuf, sizeof(tmpBuf), "set Time Error\n");
			goto setErr_end;
		}
		if (stime(&tm) < 0) {
			snprintf(tmpBuf, sizeof(tmpBuf), "set Time Error\n");
			goto setErr_end;
		}
		time_value = tm_time.tm_year + 1900; /*To accommodate flash.c*/
		apmib_set( MIB_SYSTIME_YEAR, (void *)&time_value);
		time_value = tm_time.tm_mon;
		apmib_set( MIB_SYSTIME_MON, (void *)&time_value);
		time_value = tm_time.tm_mday;
		apmib_set( MIB_SYSTIME_DAY, (void *)&time_value);
		time_value = tm_time.tm_hour;
		apmib_set( MIB_SYSTIME_HOUR, (void *)&time_value);
		time_value = tm_time.tm_min;
		apmib_set( MIB_SYSTIME_MIN, (void *)&time_value);
		time_value = tm_time.tm_sec;
		apmib_set( MIB_SYSTIME_SEC, (void *)&time_value);
		
		apmib_update_web(CURRENT_SETTING);
	}
	
	submitUrl = req_get_cstream_var(wp, "submit-url", "");   // hidden page	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);

	return;
setErr_end:
	ERR_MSG(tmpBuf)
}
int he_wlanChannelHandler(request *wp, char *tmpBuf, int *mode, int wlan_id)
{
	char varName[32];
	char *chan_str;
	int channel=0;
	if(wlan_id == (NUM_WLAN_INTERFACE-1))
		sprintf(varName, "%s", "select1");
	else
		sprintf(varName, "%s", "select2");
	chan_str=req_get_cstream_var(wp, varName, "");	
	channel=get_chan_number(chan_str);
	//printf("#### varName:%s, chan_str:%s, channel:%d\n", varName, chan_str, channel);
	
	if( apmib_set(MIB_WLAN_CHANNEL, (void *)&channel) == 0) {
		strcpy(tmpBuf, ("Set wlan channel error!"));
		goto setErr_wlan;
	}
	return 0;
setErr_wlan:
	printf("[%s %d]%s\n", __FUNCTION__,__LINE__,tmpBuf);
	return -1;
}

/*--------------------------- for wlan form-------------------------- */
int he_wpaHandler(request *wp, char *tmpBuf, int wlan_id)
{
   	char *strEncrypt, *strVal, *cipher, *strCode;
	ENCRYPT_T encrypt;
	int enableRS=0, intVal, getPSK=0, len, val, keylen;
	unsigned long reKeyTime;
	SUPP_NONWAP_T suppNonWPA;
	struct in_addr inIp;
	char varName[20];
	int j;

	//snprintf(varName, sizeof(varName), "%s", "wirelesskey");
	snprintf(varName, sizeof(varName), "%s", "apcli_wpapsk");
	strCode = req_get_cstream_var(wp, varName, "");
	keylen = strlen(strCode);
	WLAN_DEBUG("strCode:%s, keylen:%d\n", strCode, keylen);

	if(keylen==64){
		for(j=0; j<keylen; ++j){
			if(!_is_hex(strCode[j])){
				strcpy(tmpBuf, ("Invalid key format!"));
				goto setErr_encrypt;
			}
		}
	}	
	else if(keylen<8 || keylen>63 ){
		strcpy(tmpBuf, ("Invalid key length!"));
		goto setErr_encrypt;
	}

	/* for client mode, just support WPA-MIXED */
	encrypt = ENCRYPT_WPA2_MIXED;
	if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
  		strcpy(tmpBuf, ("Set MIB_WLAN_ENCRYPT mib error!"));
		goto setErr_encrypt;
	}
	
	if(1) { 
		// WPA authentication	
		/* set auth */
		intVal = WPA_AUTH_PSK;
		if ( apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WPA_AUTH failed!"));
			goto setErr_encrypt;
		}			
		
		// get cipher suite from user setting, for wpa-aes
		sprintf(varName, "%s", "wdssecurity_cipher");
		cipher = req_get_cstream_var(wp, varName, "");	 	
		if (cipher[0]) {
			intVal = 0;	
			if( !strcmp(cipher, ("TKIPAES")))
				intVal = WPA_CIPHER_MIXED;
			else if ( !strcmp(cipher, ("TKIP"))) 
				intVal = WPA_CIPHER_TKIP;
			else if ( !strcmp(cipher, ("AES"))) 
				intVal = WPA_CIPHER_AES;	
				
			if (intVal == 0) {
				strcpy(tmpBuf, ("Invalid value of cipher suite!"));
				goto setErr_encrypt;
			}
		}
		else{
				intVal = WPA_CIPHER_MIXED;	
		}		
		
		if ( apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intVal) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
			goto setErr_encrypt;							
		}
		
		if ( apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intVal) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WPA2_CIPHER_SUITE failed!"));
			goto setErr_encrypt;							
		}
	
		if( ((encrypt ==  ENCRYPT_WPA2) || (encrypt == ENCRYPT_WPA2_MIXED)) &&
		    enableRS == 1){
			sprintf(varName, "preAuth%d", wlan_id);
			strVal = req_get_cstream_var(wp, varName, "");
			if ( !strcmp(strVal, "ON"))
				intVal = 1 ;
			else
				intVal = 0 ;
			if ( apmib_set(MIB_WLAN_WPA2_PRE_AUTH, (void *)&intVal) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
				goto setErr_encrypt;
			}					
		}

		// pre-shared key
		getPSK = 1;
		if ( getPSK ) {
			int oldFormat, oldPskLen, i, newFormat, newPskLen;
			if(keylen==64)
				newFormat = KEY_HEX;
			else
				newFormat = KEY_ASCII;		

			// remember current psk format and length to compare to default case "****"
			apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&oldFormat);
			apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf);
			oldPskLen = strlen(tmpBuf);	
			newPskLen = keylen;

			WLAN_DEBUG("oldFormat:%d, newFormat:%d, oldPskLen:%d, newPskLen:%d\n", oldFormat,newFormat,oldPskLen, newPskLen);
			if (oldFormat == newFormat && newPskLen == oldPskLen ) {
				if(!strcmp(tmpBuf,strCode))
					goto rekey_time;
			}

			if ( apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&newFormat) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_PSK_FORMAT failed!"));
				goto setErr_encrypt;
			}

			if (newFormat==KEY_HEX) { // hex
				if (newPskLen!=MAX_PSK_LEN || !string_to_hex(strCode, tmpBuf, MAX_PSK_LEN)) {
	 				strcpy(tmpBuf, ("Error! invalid hex psk value."));
					goto setErr_encrypt;
				}
			}
			else { // passphras
				if (newPskLen==0 || newPskLen > (MAX_PSK_LEN-1)|| newPskLen < MIN_PSK_LEN ) {
	 				strcpy(tmpBuf, ("Error! invalid psk passphras value."));
					goto setErr_encrypt;
				}
			}
			if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)strCode)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_PSK error!"));
				goto setErr_encrypt;
			}			
		}
		
rekey_time:
		return 0;
	}

	return 0 ;

setErr_encrypt:
	//fprintf(stderr,tmpBuf);
	return -1 ;		
}

static int he_wepHandler(request *wp, char *tmpBuf, int wlan_id)
{  
   	char *strFormat, *strAuth, *strEnabled, *wepKey;
	char key[30];
	int enabled, keyLen, ret, i, authType, wlanMode, rptEnabled, keyformat;
	WEP_T wep;
	ENCRYPT_T encrypt=ENCRYPT_WEP;
	char varName[20];
	int keylen = 0;	
			
	//snprintf(varName, sizeof(varName), "%s", "wirelesskey");	
	snprintf(varName, sizeof(varName), "%s", "apcli_wpapsk");
	wepKey = req_get_cstream_var(wp, varName, "");
	keylen = strlen(wepKey);
	WLAN_DEBUG("wepKey:%s, keylen:%d\n", wepKey, keylen);
	
	if(keylen==5 || keylen==10)		
		wep = WEP64;
	else if(keylen==13 || keylen==26)
		wep = WEP128;	
	else{
		strcpy(tmpBuf, ("Wrong WEP KEY length!"));
		goto setErr_wep;
	}

	/* ENCRYPT */
	encrypt = ENCRYPT_WEP;
	if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
		strcpy(tmpBuf, ("Set MIB_WLAN_ENCRYPT mib error!"));
		goto setErr_wep;
	}

	/* WEP (WEP64 / WEP128)*/
	if ( apmib_set( MIB_WLAN_WEP, (void *)&wep) == 0) {
  		strcpy(tmpBuf, ("Set WEP MIB error!"));
		goto setErr_wep;
	}	

	/* AUTH TYPE */
	authType = AUTH_BOTH;
	apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);

	/* KEY TYPE (ASICC / HEX) & KEY LEN */
	if(keylen==5 ){
		keyformat = KEY_ASCII;
		keyLen = WEP64_KEY_LEN;
	}else if(keylen==10){
		keyformat = KEY_HEX;
		keyLen = WEP64_KEY_LEN*2;
	}else if(keylen==13){
		keyformat = KEY_ASCII;
		keyLen = WEP128_KEY_LEN;
	}else if(keylen==26){
		keyformat = KEY_HEX;
		keyLen = WEP128_KEY_LEN*2;
	}	
	if ( apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&keyformat) == 0) {
  		strcpy(tmpBuf, ("Set WEP key type error!"));
		goto setErr_wep;
	}	

	/* WEP KEY */
	if (keyformat == KEY_ASCII) // ascii
		strcpy(key, wepKey);
	else { // hex
		if ( !string_to_hex(wepKey, key, keyLen)) {
			strcpy(tmpBuf, ("Invalid wep-key value!"));
			goto setErr_wep;
		}
	}
	if (wep == WEP64){
		ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
		ret=apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
		ret=apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
		ret=apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);
	}else{
		ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
		ret=apmib_set(MIB_WLAN_WEP128_KEY2, (void *)key);
		ret=apmib_set(MIB_WLAN_WEP128_KEY3, (void *)key);
		ret=apmib_set(MIB_WLAN_WEP128_KEY4, (void *)key);
	}
	if (!ret) {
		strcpy(tmpBuf, ("Set wep-key error!"));
		goto setErr_wep;
	}

	WLAN_DEBUG("wepKey:%s, keylen:%d, keyformat:%d, keyLen:%d\n", wepKey, keylen, keyformat, keyLen);
	return 0 ;

setErr_wep:
	//printf("[%s %d]%s\n", __FUNCTION__,__LINE__,tmpBuf);
	return -1 ;	
}	

#if defined(CONFIG_RTL_92D_SUPPORT)
static int _isBandModeBoth()
{
	int val;
	apmib_get(MIB_WLAN_BAND2G5G_SELECT, (void *)&val);
	if(val == BANDMODEBOTH)
		return 1;
	else
		return 0;
}
#endif

static void _Start_Wlan_Applications(void)
{
	unsigned int repeater_enable1 = 0, repeater_enable2=0;
	unsigned char cmd[100] = {0};

	int pid;
	{						
		pid=getPid("/etc/udhcpc/udhcpc-br0.pid");
		if(pid>0)
		{			
			kill(pid, SIGUSR1);
		}
	}

	if(NUM_WLAN_INTERFACE==2){
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&repeater_enable1);
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&repeater_enable2);
		if(repeater_enable1==0){
			snprintf(cmd, sizeof(cmd), "%s", "ifconfig wlan0-vxd down");
			system(cmd);
			system("brctl addif br0 wlan1-vxd");
			system("brctl delif br0 wlan0-vxd");
		}else if(repeater_enable2==0){
			snprintf(cmd, sizeof(cmd), "%s", "ifconfig wlan1-vxd down");
			system(cmd);
			system("brctl addif br0 wlan0-vxd");
			system("brctl delif br0 wlan1-vxd");
		}			
	}
	
#if defined (CONFIG_RTL_92D_SUPPORT)
	if(_isBandModeBoth())
		system("sysconf wlanapp start wlan0 wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd wlan1 wlan1-va0 wlan1-va1 wlan1-va2 wlan1-va3 wlan1-vxd br0");
	else
		system("sysconf wlanapp start wlan0 wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd br0");
#else
	system("sysconf wlanapp start wlan0 wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd br0");
#endif
		
	sleep(1);
	/*sysconf upnpd 1(isgateway) 1(opmode is bridge)*/
	system("sysconf upnpd 1 1");
	sleep(1);
}

/* for repeater connect flow */
int he_vxd_connect(request *wp, char *tmpBuf, int *mode, int wlan_id)
{
	char *strEnc=NULL, *strCode=NULL, *wlan_band=NULL;	
	char varName[32];
	ENCRYPT_T encrypt;
	AUTH_TYPE_T authType;
	unsigned int cipher =0 , cipher2 = 0;
	char *strBSSID;
	int rptEnabled=0,  wlan_mode=0, wlanvxd_mode=0;
	int vxd_wisp_wan=0;
	char wlanifp_bak[15] = {0}, wlanifp[15] = {0};
	BssDscr *pBss=NULL;
	char tmpBuf1[MAX_MSG_BUFFER_SIZE]={0};
	char ssidbuf[MAX_SSID_LEN];
	int k,ssid_idx = 0, wait_time, max_wait_time;
	unsigned char res;
	NETWORK_TYPE_T net;
	int chan;		
	char *tmpStr, *pMsg=NULL;;	
	unsigned int i,val,status;
	char command[50];	
	unsigned int setValue = 0;

	/* encrypt */
	snprintf(varName, sizeof(varName), "%s", "apcli_mode");
	strEnc = req_get_cstream_var(wp, varName, "");
	WLAN_DEBUG("varName:%s, strEncrypt:%s\n", varName, strEnc);	
	
	/* if not return back from site survey, then return */
	if(strEnc==NULL || strEnc=="")
		return -1;

	snprintf(varName, sizeof(varName), "%s", "wlan_idx");
	wlan_band = req_get_cstream_var(wp, varName, "");
	if(wlan_band[0]){
		wlan_id = wlan_band[0] - '0';
		if(wlan_id==0){
			if(NUM_WLAN_INTERFACE==2)
				wlan_id = 1;
		}
		else if(wlan_id==1)			
			wlan_id = 0;
	}
	WLAN_DEBUG("wlan_idx:%s\n", wlan_band);
	
	snprintf(wlanifp, sizeof(wlanifp), "wlan%d", wlan_id);
	snprintf(wlanifp_bak, sizeof(wlanifp_bak), "%s", wlanifp); // bak root interface
	
	SetWlan_idx(wlanifp);
	WLAN_DEBUG("wlanifp:%s\n", wlanifp);
	
    apmib_get(MIB_WLAN_MODE, (void *)&wlan_mode);    
    if(1)
    {
        vxd_wisp_wan = 1;
    }	   
	
	if(vxd_wisp_wan== 1)
	{	
		/* disable another interface to avoid dual connection */
		if(NUM_WLAN_INTERFACE==2){			
			if(wlan_id==0){	
				setValue = 1;
				apmib_set(MIB_REPEATER_CONNECT, (void *)&setValue);
				setValue = 0;
				apmib_set(MIB_REPEATER_ENABLED2, (void *)&setValue);
				setValue = 1;
				apmib_set(MIB_REPEATER_ENABLED1, (void *)&setValue);

				sprintf(wlanifp, "%s", "wlan0-vxd");
				SetWlan_idx(wlanifp);
				setValue = 0;
				apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&setValue);

				sprintf(wlanifp, "%s", "wlan1-vxd");
				SetWlan_idx(wlanifp);
				setValue = 1;
				apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&setValue);				
			}else if(wlan_id==1){	
				setValue = 2;
				apmib_set(MIB_REPEATER_CONNECT, (void *)&setValue);
				setValue = 0;
				apmib_set(MIB_REPEATER_ENABLED1, (void *)&setValue);
				setValue = 1;
				apmib_set(MIB_REPEATER_ENABLED2, (void *)&setValue);
				
				sprintf(wlanifp, "%s", "wlan0-vxd");
				SetWlan_idx(wlanifp);
				setValue = 1;
				apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&setValue);

				sprintf(wlanifp, "%s", "wlan1-vxd");
				SetWlan_idx(wlanifp);
				setValue = 0;
				apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&setValue);	
			}			  
		}		
	
		sprintf(wlanifp, "%s-vxd", wlanifp_bak);
		SetWlan_idx(wlanifp);		
	}		
	
	/* set SSID */
	snprintf(varName, sizeof(varName), "%s", "bssid");
    strBSSID = req_get_cstream_var(wp, varName, "");
	WLAN_DEBUG("varName:%s, strBSSID:%s\n", varName, strBSSID);
	
    for (k=0; k<pStatus[wlan_id]->number && pStatus[wlan_id]->number!=0xff; k++) 
    {
        pBss = &pStatus[wlan_id]->bssdb[k];
        snprintf(tmpBuf1, sizeof(tmpBuf1), ("%02x:%02x:%02x:%02x:%02x:%02x"),
            pBss->bdBssId[0], pBss->bdBssId[1], pBss->bdBssId[2],
            pBss->bdBssId[3], pBss->bdBssId[4], pBss->bdBssId[5]);

        if(strcmp(strBSSID, tmpBuf1)==0)
        {
            memcpy(ssidbuf, pBss->bdSsIdBuf, pBss->bdSsId.Length);
            ssidbuf[pBss->bdSsId.Length] = '\0';	
			ssid_idx = k;
            break;
        }		
    }
    apmib_set(MIB_WLAN_SSID, (void *)ssidbuf);
    if(vxd_wisp_wan== 1) {
        if(wlan_id == 0)
            apmib_set(MIB_REPEATER_SSID1, (void *)ssidbuf);
        else
            apmib_set(MIB_REPEATER_SSID2, (void *)ssidbuf);
    }       
    //apmib_update(CURRENT_SETTING);	
	
	/* set encrypt */
	if (1) {
		if(1){ // 2g or 5g
			if(strstr(strEnc, "OPEN")){
				ENCRYPT_T encrypt = ENCRYPT_DISABLED;
				apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt);
				apmib_set( MIB_WLAN_ENABLE_1X, (void *)&encrypt);
			}else if(strstr(strEnc, "WEP")){				
				if(he_wepHandler(wp, tmpBuf, wlan_id) < 0)
				{
					goto ss_err;
				}					
			}else{				
				if(he_wpaHandler(wp, tmpBuf, wlan_id) < 0)
				{
					goto ss_err;
				}		
			}
		}
		
		if ( pStatus[wlan_id]->bssdb[ssid_idx].bdCap & cESS )
			net = INFRASTRUCTURE;
		else
			net = ADHOC;

		if ( apmib_set(MIB_WLAN_NETWORK_TYPE, (void *)&net) == 0) {
			strcpy(tmpBuf, ("设置MIB_WLAN_NETWORK_TYPE错误!"));
			goto ss_err;
		}

		if (net == ADHOC){
			chan = pStatus[wlan_id]->bssdb[ssid_idx].ChannelNumber;
			if ( apmib_set( MIB_WLAN_CHANNEL, (void *)&chan) == 0) {
				strcpy(tmpBuf, ("设置信道编号错误!"));
				goto ss_err;
			}
			int is_40m_bw ;// = (pStatus->bssdb[idx].bdTstamp[1] & 2) ? 1 : 0;
			if(pStatus[wlan_id]->bssdb[ssid_idx].bdTstamp[1] & 64)
				is_40m_bw = 2;
			else if(pStatus[wlan_id]->bssdb[ssid_idx].bdTstamp[1] & 2)
				is_40m_bw = 1;
			else
				is_40m_bw = 0;				
			apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&is_40m_bw);				
		} else if (vxd_wisp_wan) {			
			//switch to flash of root interface
			SetWlan_idx(wlanifp_bak);
			chan = pStatus[wlan_id]->bssdb[ssid_idx].ChannelNumber;
			if ( apmib_set( MIB_WLAN_CHANNEL, (void *)&chan) == 0){
				strcpy(tmpBuf, ("设置信道编号错误!"));
				goto ss_err;
			}
			int sideband;
			apmib_get( MIB_WLAN_CONTROL_SIDEBAND, (void *)&sideband);
			if ((chan<=4 && sideband==0) || (chan>=10 && sideband==1)){
				sideband = ((sideband + 1) & 0x1);
				apmib_set( MIB_WLAN_CONTROL_SIDEBAND, (void *)&sideband);
			}
			//switch back to flash of vxd interface
			SetWlan_idx(wlanifp);			
		}
		
#if 1//def SYNC_VXD_TO_ROOT
		if(1){
			// get vxd encrypt
			apmib_get( MIB_WLAN_ENCRYPT, (void *)&encrypt);
			sync_vxd_to_root(encrypt);
			printf("sync vxd to root \n");
		}else
			printf("not sync vxd to root \n");
#endif
		
		apmib_update_web(CURRENT_SETTING);

#if 1 //reinit wlan interface and mib			
#if 1//def CONFIG_SMART_REPEATER
		if(vxd_wisp_wan){
			sprintf(command,"ifconfig %s-vxd down",wlanifp_bak);
			system(command);
		}
#endif
		sprintf(command,"ifconfig %s down",wlanifp_bak);
		system(command);
		sprintf(command,"flash set_mib %s",wlanifp_bak);
		system(command);
		sprintf(command,"ifconfig %s up",wlanifp_bak);
		system(command);
#if 1//def CONFIG_SMART_REPEATER
		if(vxd_wisp_wan){
			sprintf(command,"flash set_mib %s-vxd",wlanifp_bak);                      
			system(command);
			sprintf(command,"ifconfig %s-vxd up",wlanifp_bak);
			system(command);
		}
#endif	
		// wlan0 entering forwarding state need some time
		sleep(3);
		_Start_Wlan_Applications();
#endif
		res = ssid_idx;
		wait_time = 0;						
		max_wait_time = 30;	

		/* to connect */
		while (1) 
		{
			if ( getWlJoinResult(wlanifp, &res) < 0 ) {
				strcpy(tmpBuf, ("获取加入结果失败!"));
				goto ss_err;
			}

			if(res==STATE_Bss  || res==STATE_Ibss_Idle || res==STATE_Ibss_Active) { // completed 
				break;
			}
			else
			{
				if (wait_time++ > max_wait_time) {
					strcpy(tmpBuf, ("连接超时!"));
					goto ss_err;
				}
			}
			sleep(1);
		}

		if ( res!=STATE_Bss && res!=STATE_Ibss_Idle && res!=STATE_Ibss_Active ){
			pMsg = (unsigned char *)"连接失败 4!";
			printf("连接失败 4!\n");
		}
		else {					
			status = 0;				
			apmib_get( MIB_WLAN_ENCRYPT, (void *)&encrypt);			
			if (encrypt == ENCRYPT_WPA || encrypt == ENCRYPT_WPA2 || encrypt == ENCRYPT_WPA2_MIXED || encrypt == ENCRYPT_WAPI) {
				bss_info bss;
				wait_time = 0;				
				max_wait_time=15;	//Need more test, especially for 802.1x client mode	
				while (wait_time++ < max_wait_time) {
					getWlBssInfo(wlanifp, &bss);
					if (bss.state == STATE_CONNECTED){
						break;
					}
					sleep(1);
				}
				if (wait_time > max_wait_time)						
					status = 1; //fail
			}

			if (status)
				pMsg =  (unsigned char *)"连接超时!";//"Connect failed 5!";//Input negative AES password can trigger here
			else
				pMsg =  (unsigned char *)"连接成功!";
		}			
	}
		
#if 1//defined(CONFIG_SMART_REPEATER)
	if(vxd_wisp_wan)
	{		
		SetWlan_idx(wlanifp_bak); // root		
	}
#endif

#if 1	
	int j;
	for(j=0; j<NUM_WLAN_INTERFACE; ++j){
		if(pStatus[j])
			free(pStatus[j]); 
		pStatus[j] = NULL;
	}
#endif
		WLAN_DEBUG("connect done\n");
		return 0;

ss_err:

#if 1		
	for(j=0; j<NUM_WLAN_INTERFACE; ++j){
		if(pStatus[j])
			free(pStatus[j]); 
		pStatus[j] = NULL;
	}
#endif

#if 1//defined(CONFIG_SMART_REPEATER)
	if(vxd_wisp_wan)
	{		
		SetWlan_idx(wlanifp_bak);		
	}
#endif
	//ERR_MSG(tmpBuf);
	fprintf(stderr,tmpBuf);
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
static inline int isAllStar(char *data)
{
	int i;
	for (i=0; i<strlen(data); i++) {
		if (data[i] != '*')
			return 0;
	}
	return 1;
}
//////////////////////

static void he_update_wps_configured(int reset_flag)
{
	int is_configured, encrypt1, encrypt2, auth, disabled, iVal, format, shared_type;
	char ssid1[100];
	unsigned char tmpbuf[MAX_MSG_BUFFER_SIZE]={0};	
	
	apmib_get(MIB_WLAN_ENCRYPT, (void *)&encrypt1);
	if (encrypt1 == ENCRYPT_DISABLED) {
		auth = WSC_AUTH_OPEN;
		encrypt2 = WSC_ENCRYPT_NONE;
	}
	else if (encrypt1 == ENCRYPT_WEP) {
		apmib_get(MIB_WLAN_AUTH_TYPE, (void *)&shared_type);
		if (shared_type == AUTH_OPEN || shared_type == AUTH_BOTH)
			auth = WSC_AUTH_OPEN;
		else
			auth = WSC_AUTH_SHARED;
		encrypt2 = WSC_ENCRYPT_WEP;		
	}
	else if (encrypt1 == ENCRYPT_WPA) {
		auth = WSC_AUTH_WPAPSK;
		apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&encrypt1);
		if (encrypt1 == WPA_CIPHER_TKIP)
			encrypt2 = WSC_ENCRYPT_TKIP;		
		else if (encrypt1 == WPA_CIPHER_AES)
			encrypt2 = WSC_ENCRYPT_AES;		
		else 
			encrypt2 = WSC_ENCRYPT_TKIPAES;				
	}
	else if (encrypt1 == ENCRYPT_WPA2) {
		auth = WSC_AUTH_WPA2PSK;
		apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&encrypt1);
		if (encrypt1 == WPA_CIPHER_TKIP)
			encrypt2 = WSC_ENCRYPT_TKIP;		
		else if (encrypt1 == WPA_CIPHER_AES)
			encrypt2 = WSC_ENCRYPT_AES;		
		else 
			encrypt2 = WSC_ENCRYPT_TKIPAES;				
	}
	else {// mixed
		auth = WSC_AUTH_WPA2PSKMIXED;
		encrypt2 = WSC_ENCRYPT_TKIPAES;			

		// When mixed mode, if no WPA2-AES, try to use WPA-AES or WPA2-TKIP
		apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&encrypt1);
		apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iVal);
		if (!(iVal & WPA_CIPHER_AES)) {
			if (encrypt1 &	WPA_CIPHER_AES) {			
				//auth = WSC_AUTH_WPAPSK;
				encrypt2 = WSC_ENCRYPT_AES;	
				//printf("%s %d : %d\n",__FUNCTION__,__LINE__ ,encrypt2);
			}
			else{
				encrypt2 = WSC_ENCRYPT_TKIP;	
				//printf("%s %d : %d\n",__FUNCTION__,__LINE__ ,encrypt2);
			}
		}

		if(encrypt1==WPA_CIPHER_AES && iVal ==WPA_CIPHER_AES){
			encrypt2 = WSC_ENCRYPT_AES;	
			printf("%s %d\n",__FUNCTION__,__LINE__);			
		}
		//printf("%s %d :auth=0x%02X\n",__FUNCTION__,__LINE__ ,auth);		
		// for correct wsc_auth wsc_encry value when security is mixed mode
	}
	apmib_set(MIB_WLAN_WSC_AUTH, (void *)&auth);
	apmib_set(MIB_WLAN_WSC_ENC, (void *)&encrypt2);

	apmib_get(MIB_WLAN_ENCRYPT, (void *)&encrypt1);
	if (encrypt1 == ENCRYPT_WPA || encrypt1 == ENCRYPT_WPA2 || encrypt1 == ENCRYPT_WPA2_MIXED) {
		apmib_get(MIB_WLAN_WPA_AUTH, (void *)&format);
		if (format & 2) { // PSK
			apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpbuf);
			apmib_set(MIB_WLAN_WSC_PSK, (void *)tmpbuf);					
		}		
	}

	reset_flag = 0;
	apmib_set(MIB_WLAN_WSC_CONFIGBYEXTREG, (void *)&reset_flag);
	apmib_get(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
	if (!is_configured) {
		is_configured = 1;
		apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
#if defined(CONFIG_RTL_92D_SUPPORT)
		if(wlan_idx==0){
			wlan_idx = 1;
			apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
			wlan_idx = 0;			
		}else if(wlan_idx == 1){
			wlan_idx = 0;
			apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
			wlan_idx = 1;			
		}
#endif
	}
	
	return ;
}

/* for router set wifi mib */
int he_wlanHandler(request *wp, char *tmpBuf, int iswizard, int wlan_id)
{
	char *strSSID, *strDisabled, *strVal,*strwps,*strwep_key_id, strtmp[80], *strSSIDPrefix, *strchannel,*strchanbond,*strchannel5g,*strbandwidth5g;
	char *strEncrypt, *strAuth;
	int chan,chan5g, disabled,channel_bonding,bandwidth5g,enable11n_tkip;
	NETWORK_TYPE_T net;
	char *strRate;
	int val, repeater_enabled = 0,wepkey_idx,default_keyid;
	char varName[32];
	char wep_key_id[32];
	char key[30];
	ENCRYPT_T encrypt;
	AUTH_TYPE_T authType;
	unsigned int cipher =0 , cipher2 = 0, phyband;
	unsigned char ssid[MAX_SSID_LEN] = {0};	
	
	/* WLAN ENABLE */ 
	sprintf(varName, "%s", "wifihiddenButton");
	strDisabled = req_get_cstream_var(wp, varName, "");
	if ( !strcmp(strDisabled, "0"))
		disabled = 1;
	else
		disabled = 0;
	if ( apmib_set( MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
		strcpy(tmpBuf, ("Set disabled flag error!"));
		goto setErr_wlan;
	}	

	if ( disabled )
		return 0;
	
	/* SSID */
#if 0 
	sprintf(varName, "%s", "mssid_prefix");
	strSSIDPrefix = req_get_cstream_var(wp, varName, "");
	WLAN_DEBUG("varName:%s, strSSIDPrefix:%s\n", varName, strSSIDPrefix);
	if ( strSSIDPrefix[0] ) {
		sprintf(varName, "%s", "mssid");
		strSSID = req_get_cstream_var(wp, varName, "");
		WLAN_DEBUG("strSSID:%s\n", strSSID);
	
		strcat(strSSIDPrefix, strSSID); 	
		WLAN_DEBUG("strSSIDPrefix:%s\n", strSSIDPrefix);
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSIDPrefix) == 0){
			strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}
#else
	apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyband);	
	sprintf(varName, "%s", "mssid");
	strSSID = req_get_cstream_var(wp, varName, ""); 
	WLAN_DEBUG("strSSID:%s\n", strSSID);

	if(iswizard){	
		if(NUM_WLAN_INTERFACE==2){
			if(phyband==PHYBAND_2G)
				snprintf(ssid, sizeof(ssid), "%s%s", CMCC_SSID_PREFIX_GW_2G, strSSID);	
			else if(phyband==PHYBAND_5G)
				snprintf(ssid, sizeof(ssid), "%s%s", CMCC_SSID_PREFIX_GW_5G, strSSID);
		}else if(NUM_WLAN_INTERFACE==1){
			snprintf(ssid, sizeof(ssid), "%s%s", CMCC_SSID_PREFIX_SINGLE_BAND, strSSID);
		}	
	}else{
		snprintf(ssid, sizeof(ssid), "%s", strSSID);
	}
	
	if (apmib_set(MIB_WLAN_SSID, (void *)&ssid) == 0){
		strcpy(tmpBuf, ("Set SSID error!"));
		goto setErr_wlan;
	}
#endif

	/* HIDDEN SSID */
	sprintf(varName, "%s", "hssid");
	strVal = req_get_cstream_var(wp, varName, "");
	if (!strcmp(strVal, "0"))
		disabled = 1;
	else
		disabled = 0;
	if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&disabled) == 0) {
		strcpy(tmpBuf, ("Set MIB_WLAN_HIDDEN_SSID error!"));
		goto setErr_wlan;
	}
	WLAN_DEBUG("varName:%s, strVal:%s, disabled:%d\n", varName, strVal, disabled);

	if(wlan_id == 1 ||(NUM_WLAN_INTERFACE == 1)&& wlan_id == 0){
		/* 2G channel*/
		sprintf(varName,"%s","channel");
		strchannel = req_get_cstream_var(wp, varName, "");
		if(strchannel[0])
			chan=atoi(strchannel);
		if ( apmib_set(MIB_WLAN_CHANNEL, (void *)&chan) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL error!"));
			goto setErr_wlan;
		}
		WLAN_DEBUG("varName:%s, strVal:%s, chan:%d\n", varName, strVal, chan);
	}
	else{
		/*5G channel*/
		sprintf(varName,"%s","channel5G");
		strchannel5g = req_get_cstream_var(wp, varName, "");
		if(strchannel5g[0])
			chan5g=atoi(strchannel5g);
		if ( apmib_set(MIB_WLAN_CHANNEL, (void *)&chan5g) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL error!"));
			goto setErr_wlan;
		}
		WLAN_DEBUG("varName:%s, strchannel5g:%s, chan:%d\n", varName, strchannel5g, chan5g);
	}

	if(wlan_id == 1 ||(NUM_WLAN_INTERFACE == 1)&& wlan_id == 0 ){
		/*20/40 coexist for 2g*/
		sprintf(varName,"%s","bandwith");
		strchanbond = req_get_cstream_var(wp, varName, "");
		if(strchanbond[0]){
			channel_bonding=strchanbond[0]-'0';
			if(channel_bonding == 0){ //20MHz
				int coexist_val=0;
				if(apmib_set(MIB_WLAN_CHANNEL_BONDING,(void *)&channel_bonding) == 0){
					strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL_BONDING error!"));
					goto setErr_wlan;
					}
				if(apmib_set(MIB_WLAN_COEXIST_ENABLED,(void *)&coexist_val) == 0){
					strcpy(tmpBuf, ("Set MIB_WLAN_COEXIST_ENABLED error!"));
					goto setErr_wlan;
					}
			}
			else if(channel_bonding == 1){//20 40 coexist
				int channel_bond_val=0,coexist_val=1;
				if(apmib_set(MIB_WLAN_CHANNEL_BONDING,(void *)&channel_bond_val) == 0){
					strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL_BONDING error!"));
					goto setErr_wlan;
					}
				if(apmib_set(MIB_WLAN_COEXIST_ENABLED,(void *)&coexist_val) == 0){
					strcpy(tmpBuf, ("Set MIB_WLAN_COEXIST_ENABLED error!"));
					goto setErr_wlan;
					}
			}
			else{	//40MHz
				int channel_bond_val=1,coexist_val=0;
				if(apmib_set(MIB_WLAN_CHANNEL_BONDING,(void *)&channel_bond_val) == 0){
					strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL error!"));
					goto setErr_wlan;
					}
				if(apmib_set(MIB_WLAN_COEXIST_ENABLED,(void *)&coexist_val) == 0){
					strcpy(tmpBuf, ("Set MIB_WLAN_COEXIST_ENABLED error!"));
					goto setErr_wlan;
					}
			}
		}
		WLAN_DEBUG("varName:%s, strVal:%s, channel_bonding:%d\n", varName, strVal, channel_bonding);
	}
	else{
		/*5G bandwidth*/
		sprintf(varName,"%s","bandwith5g");
		strbandwidth5g = req_get_cstream_var(wp, varName, "");
		if(strbandwidth5g[0])
			bandwidth5g=atoi(strbandwidth5g);
		if ( apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&bandwidth5g) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL error!"));
			goto setErr_wlan;
		}
		WLAN_DEBUG("varName:%s, strchannel5g:%s, chan:%d\n", varName, strbandwidth5g, bandwidth5g);
	}
	
	/* SECURITY AND PASSWD */
	sprintf(varName, "%s", "security_mode");	
	strAuth = req_get_cstream_var(wp, varName, ""); 
	WLAN_DEBUG("varName:%s, strAuth:%s\n", varName, strAuth);	
	
	if(!strcmp(strAuth, "WEP")){ // wep
		unsigned int wep=0, key_fmt=0, key_index=0; 	
	
		encrypt = ENCRYPT_WEP;
		apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt);
		
		authType = AUTH_BOTH;				
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);	

		sprintf(varName, "%s", "wep_key1");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s LENGTH IS %d\n", varName, strVal,strlen(strVal)); 
		sprintf(wep_key_id, "%s", "wep_default_key");
		strwep_key_id = req_get_cstream_var(wp, wep_key_id, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", wep_key_id, strwep_key_id); 
		wepkey_idx=atoi(strwep_key_id);
		default_keyid=wepkey_idx-1;
		if(!isAllStar(strVal)){
			if(strlen(strVal)==5 ){
			wep = WEP64;
			key_fmt = KEY_ASCII;
			if(strwep_key_id[0]){
				if(wepkey_idx == 1)
					apmib_set(MIB_WLAN_WEP64_KEY1, (void *)strVal);
				else if(wepkey_idx == 2)
					apmib_set(MIB_WLAN_WEP64_KEY2, (void *)strVal);
				else if(wepkey_idx == 3)
					apmib_set(MIB_WLAN_WEP64_KEY3, (void *)strVal);
				else if(wepkey_idx == 4)
					apmib_set(MIB_WLAN_WEP64_KEY4, (void *)strVal);
			}

		}else if(strlen(strVal)==10){
			wep = WEP64;
			key_fmt = KEY_HEX;
			string_to_hex(strVal, key, 10);
			if(strwep_key_id[0]){
				if(wepkey_idx == 1)
					apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else if(wepkey_idx == 2)
					apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
				else if(wepkey_idx == 3)
					apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
				else if(wepkey_idx == 4)
					apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);
			}
		}else if(strlen(strVal)==13){
			wep = WEP128;
			key_fmt = KEY_ASCII;
			if(strwep_key_id[0]){
				if(wepkey_idx == 1)
					apmib_set(MIB_WLAN_WEP128_KEY1, (void *)strVal);
				else if(wepkey_idx == 2)
					apmib_set(MIB_WLAN_WEP128_KEY2, (void *)strVal);
				else if(wepkey_idx == 3)
					apmib_set(MIB_WLAN_WEP128_KEY3, (void *)strVal);
				else if(wepkey_idx == 4)
					apmib_set(MIB_WLAN_WEP128_KEY4, (void *)strVal);
			}
		}else if(strlen(strVal)==26){
			wep = WEP128;
			key_fmt = KEY_HEX;
			string_to_hex(strVal, key, 26);
			if(strwep_key_id[0]){
				if(wepkey_idx == 1)
					apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
				else if(wepkey_idx == 2)
					apmib_set(MIB_WLAN_WEP128_KEY2, (void *)key);
				else if(wepkey_idx == 3)
					apmib_set(MIB_WLAN_WEP128_KEY3, (void *)key);
				else if(wepkey_idx == 4)
					apmib_set(MIB_WLAN_WEP128_KEY4, (void *)key);
			}
		}
		apmib_set(MIB_WLAN_WEP, (void *)&wep);
		apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&key_fmt);
		apmib_set(MIB_WLAN_WEP_DEFAULT_KEY,(void *)&default_keyid);
		}
	}
	else if(!strcmp(strAuth, "WPAPSKWPA2PSK")){ 
		sprintf(varName, "%s", "cipher");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal);
		
		if(strcmp(strVal, "TKIP")==0){
			cipher=cipher2= WPA_CIPHER_TKIP;
		}else if(strcmp(strVal, "AES")==0){
			cipher=cipher2= WPA_CIPHER_AES;
		}else if(strcmp(strVal, "TKIPAES")==0){
			cipher=cipher2= WPA_CIPHER_MIXED;
		}
		apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);
		apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);

		encrypt = ENCRYPT_WPA2_MIXED;
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);

		authType = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&authType);

		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
		
		sprintf(varName, "%s", "passphrase");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal);
	}
	else if(!strcmp(strAuth, "WPAPSK") || (!strcmp(strAuth, "WPA2PSK")))
	{ 
		sprintf(varName, "%s", "cipher");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		
		if(strcmp(strVal, "TKIP")==0){
			cipher = WPA_CIPHER_TKIP;
			cipher2 = WPA_CIPHER_TKIP;
		}else if(strcmp(strVal, "AES")==0){
			cipher = WPA_CIPHER_AES;
			cipher2 = WPA_CIPHER_AES;
		}else if(strcmp(strVal, "TKIPAES")==0){
			cipher = WPA_CIPHER_MIXED;
			cipher2 = WPA_CIPHER_MIXED;
		}
	
		if(!strcmp(strAuth, "WPAPSK")){
			encrypt = ENCRYPT_WPA;
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);
		}else if(!strcmp(strAuth, "WPA2PSK")){
			encrypt = ENCRYPT_WPA2;
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);
		}else if(!strcmp(strAuth, "WPAPSKWPA2PSK")){
			encrypt = ENCRYPT_WPA2_MIXED;
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);
		}
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);

		authType = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&authType);

		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
		
		sprintf(varName, "%s", "passphrase");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal);
		
	}
	else 
	{ // NONE encrypt
		encrypt = ENCRYPT_DISABLED;
		apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt);

		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
	}

#ifdef WIFI_SIMPLE_CONFIG	
	he_update_wps_configured(0);
#endif

	/* WIFI POWER */
	sprintf(varName, "%s", "wifi_strength");
	strVal = req_get_cstream_var(wp, varName, "");	
	WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal);
	
	if (strVal[0]) {		
		if (!strcmp(strVal, ("0"))) 	  // 100
			val = 0;
		else if (!strcmp(strVal, ("1"))) // 60 or 80
			val = 60;
		else if (!strcmp(strVal, ("2"))) // 40
			val = 40;		
		else {
			strcpy(tmpBuf, ("Error! Invalid RF output power value."));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&val) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_RFPOWER_SCALE failed!"));
			goto setErr_wlan;
		}	
	}

		return 0;

setErr_wlan:
	printf("[%s %d]%s\n", __FUNCTION__,__LINE__,tmpBuf);
	return -1 ;
}

static int getWanPortMask(void)
{
	int opMode = 0;

	apmib_get(MIB_OP_MODE, (void *)&opMode);

	if(opMode == GATEWAY_MODE)
		return DEFAULT_ETH_WAN_PORT_MASK;
	else if(opMode == BRIDGE_MODE)
		return DEFAULT_ETH_WAN_PORT_MASK;
	else
		return DEFAULT_ETH_WAN_PORT_MASK;
}

static int setDefaultPVid(void)
{
	int port_idx=0, port_num=0, vlan_idx=0, entry_num=0, opmode=0;
	int default_vid_find=0, wan_mask=0, wan_idx=0, hw_nat_lan_vid=0;
	char pVidArray[MAX_VLAN_PORT_NUM * 2];
	VLAN_CONFIG_T entry;

	apmib_get(MIB_OP_MODE, (void *)&opmode);
	port_num = sizeof(portDisplayName)/sizeof(char *);
	apmib_get(MIB_VLAN_TBL_NUM, (void *)&entry_num);
	memset((void *)pVidArray, 0, MAX_VLAN_PORT_NUM * 2);
	memset((void *)&entry, 0, sizeof(VLAN_CONFIG_T));

	apmib_get(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid);

	wan_mask = getWanPortMask();
	for(wan_idx=0; wan_idx<port_num; wan_idx++)
		if(1<<wan_idx == wan_mask) break;

	//LAN/WLAN port.
	for(port_idx=0; port_idx<port_num; port_idx++)
	{
		if(1<<port_idx == wan_mask)
			continue;

		default_vid_find = 0;

		if(opmode == GATEWAY_MODE)
		{
			//As long as the vlan entry has this lan port, no matter tagged/untagged.
			for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
			{
				*(char *)&entry = (char)vlan_idx;
				apmib_get(MIB_VLAN_TBL, (void *)&entry);

				if(!(entry.MemberPortMask & (1<<port_idx)))
					continue;

				default_vid_find = entry.VlanId;
			}
		}
		else
		{
			//Search vlan group which contains this port untagged.
			for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
			{
				*(char *)&entry = (char)vlan_idx;
				apmib_get(MIB_VLAN_TBL, (void *)&entry);

				if(!(entry.MemberPortMask & (1<<port_idx)))
					continue;

				if(!(entry.TaggedPortMask & (1<<port_idx)))
				{
					default_vid_find = entry.VlanId;
						break;
				}
			}

			//Then search vlan group which contains this port tagged.
			if(default_vid_find == 0)
			{
				for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
				{
					*(char *)&entry = (char)vlan_idx;
					apmib_get(MIB_VLAN_TBL, (void *)&entry);

					if(!(entry.MemberPortMask & (1<<port_idx)))
						continue;

					default_vid_find = entry.VlanId;
						break;
				}
			}
		}

		if(default_vid_find)
			*((short *)pVidArray + port_idx) = default_vid_find;
		else
		{
			if(port_idx>=0 && port_idx<=4)
				*((short *)pVidArray + port_idx) = DEFAULT_NAT_LAN_VID;
			else
				*((short *)pVidArray + port_idx) = DEFAULT_NAT_LAN_VID;
		}
	}

	//WAN port.
	if(wan_mask != 0)
	{	
		default_vid_find = 0;

		for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
		{
			*(char *)&entry = (char)vlan_idx;
			apmib_get(MIB_VLAN_TBL, (void *)&entry);

			//Untagged bridge-wan.
			if(entry.VlanType == VLAN_TYPE_BRIDGE &&
				(entry.MemberPortMask & wan_mask) &&
				!(entry.TaggedPortMask & wan_mask))
			{
				default_vid_find = entry.VlanId;
				break;
			}
		}

		if(default_vid_find == 0)
		{
			for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
			{
				*(char *)&entry = (char)vlan_idx;
				apmib_get(MIB_VLAN_TBL, (void *)&entry);

				//Untagged nat group.
				if(entry.VlanType == VLAN_TYPE_NAT &&
					(entry.MemberPortMask & wan_mask) &&
					!(entry.TaggedPortMask & wan_mask))
				{
					default_vid_find = entry.VlanId;
					break;
				}
			}
		}

		if(default_vid_find)
			*((short *)pVidArray + wan_idx) = default_vid_find;
		else
			*((short *)pVidArray + wan_idx) = DEFAULT_NAT_WAN_VID;
	}
		
	apmib_set(MIB_VLAN_PVID_ARRAY, (void *)pVidArray);
	
	return 0;
} 

void formwanconfig(request *wp, char *path, char *query)
{
    VLAN_CONFIG_T entry, new_entry, lan_entry;
	char *submitUrl,*strTmp;
	int	i,  idx, entry_num, vlan_onoff, vlan_onoff_last, snoop_onoff, proxy_onoff, upnp_onoff, vlan_id, vlanpriority, opmode=0;
	int  port_member_mask=0, port_tagged_mask=0, wan_mask=0;
	int hw_nat_flag=0, hw_nat_lan_vid=0; 
	char strBuf[50];
	char pVidArray[MAX_VLAN_PORT_NUM * 2];
	char tmpBuf[100];
	apmib_get(MIB_OP_MODE, (void *)&opmode);

	wan_mask = getWanPortMask();

	strTmp= req_get_cstream_var(wp, ("vlanEn"), "");
    vlan_onoff = atoi(strTmp);
	strTmp= req_get_cstream_var(wp, ("igmpSnoopingEn"), "");
    snoop_onoff = !(atoi(strTmp));
	strTmp= req_get_cstream_var(wp, ("igmpproxyEn"), "");
    proxy_onoff = !(atoi(strTmp));
	strTmp= req_get_cstream_var(wp, ("UpnpEn"), "");
    upnp_onoff = atoi(strTmp);

	if (!apmib_set(MIB_IGMP_SNOOPING_DISABLE, (void *)&snoop_onoff))
	{
		strcpy(tmpBuf, ("set  MIB_IGMP_SNOOPING_DISABLE error!"));
		goto setErr;
	}
	if (!apmib_set(MIB_IGMP_PROXY_DISABLED, (void *)&proxy_onoff))
	{
		strcpy(tmpBuf, ("set  MIB_IGMP_PROXY_DISABLED error!"));
		goto setErr;
	}
	if (!apmib_set(MIB_UPNP_ENABLED, (void *)&upnp_onoff))
	{
		strcpy(tmpBuf, ("set  MIB_UPNP_ENABLED error!"));
		goto setErr;
	}

	apmib_get(MIB_VLAN_ENABLED, (void *)&vlan_onoff_last);

	if(! vlan_onoff)
	{
	    apmib_set(MIB_VLAN_ENABLED, (void *)&vlan_onoff);
	    apmib_set(MIB_VLAN_DELALL, (void *)&entry);
		goto ApmibUpdate;
	}

	if (!apmib_set(MIB_VLAN_ENABLED, (void *)&vlan_onoff))
	{
		strcpy(tmpBuf, ("set  MIB_VLANCONFIG_ENABLED error!"));
		goto setErr;
	}
	
	if(vlan_onoff)
	{
		/* set VLAN id and pri*/
		memset(&new_entry, 0, sizeof(VLAN_CONFIG_T));
		strTmp = req_get_cstream_var(wp, "vlanid", "");
		vlan_id = strtol(strTmp, NULL, 10);
		new_entry.VlanId = vlan_id;

		strTmp = req_get_cstream_var(wp, "vlanpriority", "");
		vlanpriority = strtol(strTmp, NULL, 10);
		new_entry.VlanPriority = vlanpriority;
//printf("----%s %d -----vlan_id=%d-------vlanpriority=%d----vlan_onoff=%d--- vlan_onoff_last=%d-----\n",__FUNCTION__,__LINE__,vlan_id,vlanpriority,vlan_onoff,vlan_onoff_last);

		//Set port member/tagged port.
		if(opmode == GATEWAY_MODE)
		{
			new_entry.MemberPortMask = wan_mask;
			new_entry.TaggedPortMask = wan_mask;
		}
		if(opmode == BRIDGE_MODE)
		{
		   printf("--------bridge mode----------------\n");
			new_entry.VlanType = VLAN_TYPE_BRIDGE;
			new_entry.MemberPortMask = 0x1ffff;    //lan /wlan /wan port
		   printf("--------bridge mode---------------wan_mask=%x--------\n",wan_mask);
			new_entry.TaggedPortMask = wan_mask;     //wan port
		}

		if(vlan_id==0 && vlanpriority==0)  
		{
			if(vlan_onoff != vlan_onoff_last) //just change vlan_enable
				goto ApmibUpdate;
			else
				goto ApmibNoUpdate;     //no change,do nothing
		}
		//Repeated VID check/repeated NAT wan check/repeated untagged wan group check.
		apmib_get(MIB_VLAN_TBL_NUM, (void *)&entry_num);
		memset(&entry, 0, sizeof(VLAN_CONFIG_T));
		for(i=1; i<=entry_num; i++)
		{
			*(char *)&entry = (char)i;
	    	apmib_get(MIB_VLAN_TBL, (void *)&entry);
     		if(opmode == GATEWAY_MODE)
			{
		  		if(entry.VlanId != DEFAULT_NAT_LAN_VID2)
		   		{
			 		if(entry.VlanId == new_entry.VlanId && entry.VlanPriority == new_entry.VlanPriority){
			    		goto ApmibNoUpdate;	
					}
					else{
            			apmib_set(MIB_VLAN_DELALL, (void *)&entry);
					}
		   		}
     		}
			if(opmode == BRIDGE_MODE)
			{
				if(entry.VlanId == new_entry.VlanId && entry.VlanPriority == new_entry.VlanPriority){
			    	goto ApmibNoUpdate;	
				}
				else{
            		apmib_set(MIB_VLAN_DELALL, (void *)&entry);
				}
			}
		}
		apmib_set(MIB_VLAN_ADD, (void *)&new_entry);

		if(opmode == GATEWAY_MODE)
		{
			memset(&lan_entry, 0, sizeof(VLAN_CONFIG_T));
			lan_entry.VlanId = DEFAULT_NAT_LAN_VID2;
			lan_entry.VlanPriority = new_entry.VlanPriority;
			lan_entry.MemberPortMask |= 0x1ffff & (~wan_mask);
		
			apmib_set(MIB_VLAN_ADD, (void *)&lan_entry);

			apmib_set(MIB_VLAN_HW_NAT_LAN_VID, (void *)&lan_entry.VlanId);	
		}
	}

ApmibUpdate:
	setDefaultPVid();

	apmib_set(MIB_VLAN_OPMODE, (void *)&opmode);
	
	apmib_update_web(CURRENT_SETTING);

ApmibNoUpdate:
#ifndef NO_ACTION
	run_init_script_flag = 1;
	run_init_script("all");
#endif
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page

	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;

setErr:
	ERR_MSG(tmpBuf);
	return;
}

void he_form_wirelessSet(request *wp, char *path, char *query)
{	
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};
	int mode=-1;
	int val;
	char *wlan_idx_str=NULL, *strDisabled=NULL;	
	int repeater_enabled = 0;
	char varName[32];
	int iswizard = 0;

	apmib_save_wlanIdx();
	
	/* is repeater enabled*/
	snprintf(varName, sizeof(varName), "%s", "wds_enable_button");	
	strDisabled = req_get_cstream_var(wp, varName, "");
	WLAN_DEBUG("#### varName:%s, repeater_enabled:%s\n", varName, strDisabled);	
	
	if ( !strcmp(strDisabled, "3") )
		repeater_enabled = 1;
	else
		repeater_enabled = 0;	
	
	if(repeater_enabled==0){	
		wlan_idx_str = req_get_cstream_var(wp, "wlan_idx", "");
		if(wlan_idx_str)
			val=atoi(wlan_idx_str);

		WLAN_DEBUG("#### wlan_idx:%d\n", val);
		
		if(NUM_WLAN_INTERFACE == 2) {
			if(val == 1)
				SetWlan_idx("wlan1");
			else if(val == 0)
				SetWlan_idx("wlan0");
		}	
		if(NUM_WLAN_INTERFACE == 1)
			SetWlan_idx(
"wlan0");
		if(he_wlanHandler(wp, tmpBuf, iswizard, wlan_idx) < 0)
			goto setErr_wlan ;
		
	}else if(repeater_enabled==1){
		wlan_idx = wlan_idx_to_scan;
		he_vxd_connect(wp, tmpBuf, &mode, wlan_idx);
	}
	
	apmib_update_web(CURRENT_SETTING);
	apmib_recov_wlanIdx();

#ifndef NO_ACTION 	
	if(repeater_enabled==0){
		run_init_script_flag = 1;
		run_init_script("all");
	}	
#endif

	return;

setErr_wlan:
	apmib_recov_wlanIdx();
	ERR_MSG(tmpBuf);
}
int he_wlanHandler_for_quick(request *wp, char *tmpBuf, int iswizard, int wlan_id)
{
	char *strSSID, *strDisabled, *strVal, strtmp[80], *strSSIDPrefix;
	char *strEncrypt, *strAuth;
	int chan, disabled ;
	NETWORK_TYPE_T net;
	char *strRate;
	int val, repeater_enabled = 0;
	char varName[32];
	ENCRYPT_T encrypt;
	AUTH_TYPE_T authType;
	unsigned int cipher =0 , cipher2 = 0, phyband;
	unsigned char ssid[MAX_SSID_LEN] = {0};	

	/* WLAN ENABLE */ 
	sprintf(varName, "%s", "wifihiddenButton");
	strDisabled = req_get_cstream_var(wp, varName, "");
	if ( !strcmp(strDisabled, "0"))
		disabled = 1;
	else
		disabled = 0;
	if ( apmib_set( MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
		strcpy(tmpBuf, ("Set disabled flag error!"));
		goto setErr_wlan;
	}	

	if ( disabled )
		return 0;	

	/* SSID */
#if 0 
	sprintf(varName, "%s", "mssid_prefix");
	strSSIDPrefix = req_get_cstream_var(wp, varName, "");
	WLAN_DEBUG("varName:%s, strSSIDPrefix:%s\n", varName, strSSIDPrefix);
	if ( strSSIDPrefix[0] ) {
		sprintf(varName, "%s", "mssid");
		strSSID = req_get_cstream_var(wp, varName, "");
		WLAN_DEBUG("strSSID:%s\n", strSSID);
	
		strcat(strSSIDPrefix, strSSID); 	
		WLAN_DEBUG("strSSIDPrefix:%s\n", strSSIDPrefix);
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSIDPrefix) == 0){
			strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}
#else
	apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyband);	

	disabled = 0;
	if(phyband==PHYBAND_2G){
		sprintf(varName, "%s", "mssid");
		strSSID = req_get_cstream_var(wp, varName, ""); 
		WLAN_DEBUG("strSSID:%s\n", strSSID);

		/* HIDDEN SSID */
		strVal = req_get_cstream_var(wp, "hssid", "");
		if (!strcmp(strVal, "0")) disabled = 1;
	}
	else if(phyband==PHYBAND_5G){
		sprintf(varName, "%s", "mssid_5G");
		strSSID = req_get_cstream_var(wp, varName, ""); 
		WLAN_DEBUG("strSSID:%s\n", strSSID);

		/* HIDDEN SSID 5G */		
		sprintf(varName, "%s", "hssid_5G");
		strVal = req_get_cstream_var(wp, "hssid_5G", "");
		if (!strcmp(strVal, "0")) disabled = 1;
	}

	/**
	if(iswizard){	
		if(NUM_WLAN_INTERFACE==2){
			if(phyband==PHYBAND_2G)
				snprintf(ssid, sizeof(ssid), "%s%s", CMCC_SSID_PREFIX_GW_2G, strSSID);	
			else if(phyband==PHYBAND_5G)
				snprintf(ssid, sizeof(ssid), "%s%s", CMCC_SSID_PREFIX_GW_5G, strSSID);
		}else if(NUM_WLAN_INTERFACE==1){
			snprintf(ssid, sizeof(ssid), "%s%s", CMCC_SSID_PREFIX_SINGLE_BAND, strSSID);
		}	
	}else{
		snprintf(ssid, sizeof(ssid), "%s", strSSID);
	} **/

	snprintf(ssid, sizeof(ssid), "%s", strSSID);
	
	if (apmib_set(MIB_WLAN_SSID, (void *)&ssid) == 0){
		strcpy(tmpBuf, ("Set SSID error!"));
		goto setErr_wlan;
	}
#endif

	/* HIDDEN SSID */
	if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&disabled) == 0) {
		strcpy(tmpBuf, ("Set MIB_WLAN_HIDDEN_SSID error!"));
		goto setErr_wlan;
	}
	WLAN_DEBUG("varName:%s, strVal:%s, disabled:%d\n", varName, strVal, disabled);
	
	/* SECURITY AND PASSWD */
	sprintf(varName, "%s", "security_mode");	
	strAuth = req_get_cstream_var(wp, varName, ""); 
	WLAN_DEBUG("varName:%s, strAuth:%s\n", varName, strAuth);	
	
	if(!strcmp(strAuth, "wep")){ // wep
		unsigned int wep=0, key_fmt=0, key_index=0; 	
	
		encrypt = ENCRYPT_WEP;
		apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt);
		
		authType = AUTH_BOTH;				
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);	

		sprintf(varName, "%s", "wep_key_1");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		
		if(strlen(strVal)==5 ){
			wep = WEP64;
			key_fmt = KEY_ASCII;
			apmib_set(MIB_WLAN_WEP64_KEY1, (void *)strVal);
			apmib_set(MIB_WLAN_WEP64_KEY2, (void *)strVal);
			apmib_set(MIB_WLAN_WEP64_KEY3, (void *)strVal);
			apmib_set(MIB_WLAN_WEP64_KEY4, (void *)strVal);
		}else if(strlen(strVal)==10){
			wep = WEP64;
			key_fmt = KEY_HEX;
			apmib_set(MIB_WLAN_WEP64_KEY1, (void *)strVal);
			apmib_set(MIB_WLAN_WEP64_KEY2, (void *)strVal);
			apmib_set(MIB_WLAN_WEP64_KEY3, (void *)strVal);
			apmib_set(MIB_WLAN_WEP64_KEY4, (void *)strVal);
		}else if(strlen(strVal)==13){
			wep = WEP128;
			key_fmt = KEY_ASCII;
			apmib_set(MIB_WLAN_WEP128_KEY1, (void *)strVal);
			apmib_set(MIB_WLAN_WEP128_KEY2, (void *)strVal);
			apmib_set(MIB_WLAN_WEP128_KEY3, (void *)strVal);
			apmib_set(MIB_WLAN_WEP128_KEY4, (void *)strVal);
		}else if(strlen(strVal)==26){
			wep = WEP128;
			key_fmt = KEY_HEX;
			apmib_set(MIB_WLAN_WEP128_KEY1, (void *)strVal);
			apmib_set(MIB_WLAN_WEP128_KEY2, (void *)strVal);	
			apmib_set(MIB_WLAN_WEP128_KEY3, (void *)strVal);	
			apmib_set(MIB_WLAN_WEP128_KEY4, (void *)strVal);	
		}
		apmib_set(MIB_WLAN_WEP, (void *)&wep);
		apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&key_fmt);
	}
	else if(!strcmp(strAuth, "WPAPSKWPA2PSK")){ 	
		cipher = cipher2 = WPA_CIPHER_MIXED;
		apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);
		apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);

		encrypt = ENCRYPT_WPA2_MIXED;
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);

		authType = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&authType);

		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
		
		sprintf(varName, "%s", "passphrase");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal);
	}
	else if(!strcmp(strAuth, "WPAPSK") || (!strcmp(strAuth, "WPA2PSK")))
	{ 
		sprintf(varName, "%s", "cipher");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		
		if(strcmp(strVal, "TKIP")==0){
			cipher = WPA_CIPHER_TKIP;
			cipher2 = WPA_CIPHER_TKIP;
		}else if(strcmp(strVal, "AES")==0){
			cipher = WPA_CIPHER_AES;
			cipher2 = WPA_CIPHER_AES;
		}else if(strcmp(strVal, "TKIPAES")==0){
			cipher = WPA_CIPHER_MIXED;
			cipher2 = WPA_CIPHER_MIXED;
		}
	
		if(!strcmp(strAuth, "WPAPSK")){
			encrypt = ENCRYPT_WPA;
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);
		}else if(!strcmp(strAuth, "WPA2PSK")){
			encrypt = ENCRYPT_WPA2;
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);
		}else if(!strcmp(strAuth, "WPAPSKWPA2PSK")){
			encrypt = ENCRYPT_WPA2_MIXED;
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);
		}
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);

		authType = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&authType);

		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
		
		sprintf(varName, "%s", "passphrase");
		strVal = req_get_cstream_var(wp, varName, "");	
		WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal); 
		apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal);
		
	}
	else 
	{ // NONE encrypt
		encrypt = ENCRYPT_DISABLED;
		apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt);

		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
	}

#ifdef WIFI_SIMPLE_CONFIG	
	he_update_wps_configured(0);
#endif
	/* WIFI POWER */
	sprintf(varName, "%s", "wifi_strength");
	strVal = req_get_cstream_var(wp, varName, "");	
	WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal);
	
	if (strVal[0]) {		
		if (!strcmp(strVal, ("0"))) 	  // 100
			val = 0;
		else if (!strcmp(strVal, ("1"))) // 60 or 80
			val = 60;
		else if (!strcmp(strVal, ("2"))) // 40
			val = 40;		
		else {
			strcpy(tmpBuf, ("Error! Invalid RF output power value."));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&val) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_RFPOWER_SCALE failed!"));
			goto setErr_wlan;
		}	
	}

		return 0;

setErr_wlan:
	printf("[%s %d]%s\n", __FUNCTION__,__LINE__,tmpBuf);
	return -1 ;
}

void he_form_wirelessQuick(request *wp, char *path, char *query)
{	
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};
	int mode=-1, val, i;
	unsigned char ifname[10] = {0};
	char *strVal, strtmp[80];
	char varName[20];
	unsigned int setValue = 0;
	int iswizard = 1;

	apmib_save_wlanIdx();

	for(i=0; i<NUM_WLAN_INTERFACE; ++i){

		snprintf(ifname, sizeof(ifname), "wlan%d", i);
		SetWlan_idx(ifname);
		if(he_wlanHandler_for_quick(wp, tmpBuf, iswizard, wlan_idx) < 0)
			goto setErr_wlan ;
	}	

	/* ADMIN PASSWD 
	sprintf(varName, "%s", "admpass");
	strVal = req_get_cstream_var(wp, varName, "");	
	WLAN_DEBUG("varName:%s, strVal:%s\n", varName, strVal);
	if(strVal[0]){
		apmib_set(MIB_USER_PASSWORD, (void *)strVal);		
	}else
		goto setErr_wlan ;		
	*/

	/* set MIB_NEED_NO_WIZARD to 1 */
	setValue = 1;
	apmib_set(MIB_NEED_NO_WIZARD, (void *)&setValue);
	
	apmib_update_web(CURRENT_SETTING);
	apmib_recov_wlanIdx();

#ifndef NO_ACTION 	
	run_init_script_flag = 1;
	run_init_script("all");
#endif

	return;

setErr_wlan:
	apmib_recov_wlanIdx();
	ERR_MSG(tmpBuf);
	return ;
}

void he_form_wireless_channel(request *wp, char *path, char *query)
{
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};
	int mode=-1, val, i;
	unsigned char ifname[10] = {0};
	char *strVal, strtmp[80];
	char varName[20];
	unsigned int setValue = 0;
	apmib_save_wlanIdx();
	for(i=0; i<NUM_WLAN_INTERFACE; ++i){
		snprintf(ifname, sizeof(ifname), "wlan%d", i);
		SetWlan_idx(ifname);
		if(he_wlanChannelHandler(wp, tmpBuf, &mode, i) < 0)
			goto setErr_wlan ;
	}	
	apmib_update_web(CURRENT_SETTING);
	apmib_recov_wlanIdx();
#ifndef NO_ACTION 	
	run_init_script_flag = 1;
	run_init_script("all");
#endif

	return;
setErr_wlan:
	
	apmib_recov_wlanIdx();
	ERR_MSG(tmpBuf);
	return ;	
}

void he_form_wireless_guest(request *wp, char *path, char *query)
{
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};
	char *submitUrl;
	char *strDisabled=NULL, *strSSID=NULL, *strVal=NULL;	
	char varName[32] = {0};
	unsigned int wlan_disable = 0, is_guest=0, phyband=0;
	unsigned int setValue = 0, getValue = 0;
	unsigned char ssid[MAX_SSID_LEN] = {0};	
	unsigned char wlan_ifname[10] = {0};

	apmib_save_wlanIdx();

	/* phyband */
	snprintf(varName, sizeof(varName), "%s", "ssidIndex");	
	phyband = req_get_cstream_var(wp, varName, "");
	if(!strcmp(phyband, "0")){ // 2G
		if(NUM_WLAN_INTERFACE==2)
			snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan1-va%d", CMCC_WLAN_GUEST_IDX-1);		
		else 
			snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan0-va%d", CMCC_WLAN_GUEST_IDX-1);
	}else if(!strcmp(phyband, "1")){	// 5G	
		snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan0-va%d", CMCC_WLAN_GUEST_IDX-1);
	}	
	
	SetWlan_idx(wlan_ifname);
	WLAN_DEBUG("#### wlan_ifname:%s\n", wlan_ifname);

	/* wifi enable */
	snprintf(varName, sizeof(varName), "%s", "wifihiddenButton");	
	strDisabled = req_get_cstream_var(wp, varName, "");	
	if (!strcmp(strDisabled, "1")){
		wlan_disable = 0;
		is_guest = 1;			
	}else {
		wlan_disable = 1;
		is_guest = 0;
	}
	apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);
	apmib_set(MIB_WLAN_ACCESS, (void *)&is_guest);	
	WLAN_DEBUG("#### varName:%s, wlan_disable:%d, guest:%d\n", varName, wlan_disable, is_guest);

	/* wifi ssid */
	apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyband);	
	sprintf(varName, "%s", "mssid_1");
	strSSID = req_get_cstream_var(wp, varName, ""); 
	snprintf(ssid, sizeof(ssid), "%s", strSSID);
	WLAN_DEBUG("strSSID:%s, ssid:%s\n", strSSID, ssid);
	if (apmib_set(MIB_WLAN_SSID, (void *)&ssid) == 0){
		strcpy(tmpBuf, ("Set SSID error!"));
		goto setErr_wlan;
	}

	/* wifi hidden */	
	sprintf(varName, "%s", "hssid_1");
	strVal = req_get_cstream_var(wp, varName, "");
	if (!strcmp(strVal, "1"))
		setValue = 1;
	else
		setValue = 0;
	if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&setValue) == 0) {
		strcpy(tmpBuf, ("Set MIB_WLAN_HIDDEN_SSID error!"));
		goto setErr_wlan;
	}
	WLAN_DEBUG("varName:%s, hidden:%d\n", varName, setValue);
		
	apmib_update_web(CURRENT_SETTING);
	apmib_recov_wlanIdx();

#ifndef NO_ACTION 	
	run_init_script_flag = 1;
	run_init_script("all");	
#endif
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
	//printf("%s:%d get submit=%s \n",__FUNCTION__,__LINE__,submitUrl);
	//add sleep(15) to avoid html is ready to config but system is restarting
	sleep(15);
	if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
	return;

setErr_wlan:
	apmib_recov_wlanIdx();
	ERR_MSG(tmpBuf);
}

void he_form_wps_setting(request *wp, char *path, char *query)
{
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};	
	char *wps_enable=NULL, *phyband=NULL;	
	char varName[32] = {0};	
	unsigned int wps_disable = 0;	
	unsigned char wlan_ifname[10] = {0};

	apmib_save_wlanIdx();	

	/* phyband */
	snprintf(varName, sizeof(varName), "%s", "phyband");	
	phyband = req_get_cstream_var(wp, varName, "");
	if(!strcmp(phyband, "2")){
		if(NUM_WLAN_INTERFACE==2)
			snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan1");		
		else 
			snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan0");
	}else if(!strcmp(phyband, "5")){		
		snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan0");
	}	
	
	SetWlan_idx(wlan_ifname);
	WLAN_DEBUG("#### wlan_ifname:%s\n", wlan_ifname);

	/* wifi enable */
	snprintf(varName, sizeof(varName), "%s", "WPSEnable");	
	wps_enable = req_get_cstream_var(wp, varName, "");	
	if (!strcmp(wps_enable, "1")){
		wps_disable = 0;		
	}else {
		wps_disable = 1;
	}
	WLAN_DEBUG("#### wps_disable:%d\n", wps_disable);
	apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&wps_disable);
	
	apmib_update_web(CURRENT_SETTING);
	
	apmib_recov_wlanIdx();

#ifndef NO_ACTION 	
	run_init_script_flag = 1;
	run_init_script("bridge");	 
#endif
	char *submitUrl;
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
	//printf("%s:%d get submit=%s \n",__FUNCTION__,__LINE__,submitUrl);
	//add sleep(15) to avoid html is ready to config but system is restarting
	sleep(15);
	if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
	return;
}

void he_form_wps_config(request *wp, char *path, char *query)
{
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};	
	char *phyband=NULL;	
	char varName[32] = {0}; 	
	unsigned int wlanidx = 0;

	/* phyband */
	snprintf(varName, sizeof(varName), "%s", "phyband");	
	phyband = req_get_cstream_var(wp, varName, "");	
	if(!strcmp(phyband, "2")){
		if(NUM_WLAN_INTERFACE==2)
			wlanidx = 1;		
		else 
			wlanidx = 0;	
	}else if(!strcmp(phyband, "5")){		
		wlanidx = 0;	
	}	

	/* triggle wps pbc */	
#ifndef NO_ACTION
	snprintf(tmpBuf, sizeof(tmpBuf), "echo 1 > /var/wps_start_interface%d", wlanidx);
	system(tmpBuf);
	snprintf(tmpBuf, sizeof(tmpBuf), "%s -sig_pbc wlan%d", _WSC_DAEMON_PROG, wlanidx);
	WLAN_DEBUG("tmpBuf:%s\n", tmpBuf);
	system(tmpBuf);		
#endif
	char *submitUrl;
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	 // hidden page
	//printf("%s:%d get submit=%s \n",__FUNCTION__,__LINE__,submitUrl);
	//add sleep(15) to avoid html is ready to config but system is restarting
	sleep(15);
	if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
	return;

}

void disconnectbymacfilter(request *wp, char *path, char *query)
{
	char *str_mac=NULL;		
	int entryNum=0;
	char errMsg[256]={0}, tmp_mac[13]={0};
	int i=0, j=0;
	MACFILTER_T macEntry;
	int black_tab_enable=2;
	char wlan_ifname[16]={0};

	str_mac = req_get_cstream_var(wp, ("mac"), "");
	if(str_mac)
	{
		printf("\n%s:%d str_mac=%s\n",__FUNCTION__,__LINE__,str_mac);

		memset(&macEntry, 0, sizeof(macEntry));

		for(i=0;i<17;i++)
			if(str_mac[i]!=':')
				tmp_mac[j++]=str_mac[i];

		
		if(findClientAssoWlanIf("/proc/wlan0/sta_info", tmp_mac))
			strcpy(wlan_ifname, "wlan0");
		else if(findClientAssoWlanIf("/proc/wlan1/sta_info", tmp_mac))
			strcpy(wlan_ifname, "wlan1");
		else
		{
			snprintf(errMsg,sizeof(errMsg),"can't find client associate wlan interface!\n");
			goto disconnectbymacfilter_fail;
		}	

		//printf("\n%s:%d wlan_ifname=%s\n",__FUNCTION__,__LINE__,wlan_ifname);

		if (strlen(tmp_mac)!=12 || !string_to_hex(tmp_mac, macEntry.macAddr, 12)) 
		{
			snprintf(errMsg,sizeof(errMsg),"Invalid mac address!\n");
			goto disconnectbymacfilter_fail;
		}

		apmib_save_wlanIdx();
		
		SetWlan_idx(wlan_ifname);

		if ( !apmib_set( MIB_WLAN_MACAC_ENABLED, (void *)&black_tab_enable)) 
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_WLAN_MACAC_ENABLED fail!\n");
			goto disconnectbymacfilter_fail;
		}

		if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) 
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_WLAN_MACAC_NUM fail!\n");
			goto disconnectbymacfilter_fail;
		}

		if(entryNum>=MAX_WLAN_AC_NUM)
		{
			snprintf(errMsg,sizeof(errMsg),"wlan AC table is full!\n");
			goto disconnectbymacfilter_fail;
		}

		apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry);

		if (!apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry)) 
		{
			snprintf(errMsg,sizeof(errMsg),"set MIB_WLAN_AC_ADDR_ADD fail!\n");
			goto disconnectbymacfilter_fail;
		}

		apmib_update_web(CURRENT_SETTING);

		apmib_recov_wlanIdx();
	}

	run_init_script_flag = 1;
	run_init_script("all");

	return;
disconnectbymacfilter_fail:
	ERR_MSG(errMsg);	
}

int sortSiteSurveyResult(SS_STATUS_DUAL_Tp pResult)
{
	int i = 0, j = 0, k = 0;
	BssDscr *pBss0, *pBss1;

	if(NUM_WLAN_INTERFACE==2){
		if(pStatus[0]->number==0xff || pStatus[1]->number==0xff)
			pResult->number = 0xff;
		else
			pResult->number = pStatus[0]->number + pStatus[1]->number;
		WLAN_DEBUG("pStatus[0]->number:%d, pStatus[1]->number:%d, pResult->number:%d\n", 
			pStatus[0]->number, pStatus[1]->number, pResult->number);
	}
	else{
		pResult->number = pStatus[0]->number;
		WLAN_DEBUG("pStatus[0]->number:%d, pResult->number:%d\n", pStatus[0]->number, pResult->number);
	}
	
	for(k=0; MAX_BSS_DESC*2; ++k){		
		if(pStatus[0] && i<pStatus[0]->number)
			pBss0 = &pStatus[0]->bssdb[i];
		else
			pBss0 = NULL;
		
		if(pStatus[1] && j<pStatus[1]->number)
			pBss1 = &pStatus[1]->bssdb[j];
		else
			pBss1 = NULL;

		if(pBss0 && pBss1){
			if(pBss0->rssi >= pBss1->rssi){				
				memcpy(&pResult->bssdb[k], &pStatus[0]->bssdb[i], sizeof(BssDscr));
				i += 1;
			}else{				
				memcpy(&pResult->bssdb[k], &pStatus[1]->bssdb[j], sizeof(BssDscr));
				j += 1;
			}
		}else if(pBss0==NULL){
			if(pBss1){
				memcpy(&pResult->bssdb[k], &pStatus[1]->bssdb[j], sizeof(BssDscr));
				j += 1;
			}else
				break;
		}else if(pBss1==NULL){
			if(pBss0){
				memcpy(&pResult->bssdb[k], &pStatus[0]->bssdb[i], sizeof(BssDscr));
				i += 1;
			}else
				break;
		}		
	}

	return;
}

int doWlSiteSurvey(int wlanidx)
{   
    int status, idx;
    BssDscr *pBss=NULL;
    char ssidbuf[33];
    int k;
    unsigned char res, *pMsg=NULL;
    int wait_time, max_wait_time=5;
    char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};	
	unsigned int WAIT_TIME_DEFINE = 30;
	unsigned char ifname[10] = {0};
	SS_STATUS_Tp ptmpSS = NULL;

	//pStatus[wlanidx] = NULL;
	if (pStatus[wlanidx]==NULL) {		
		//pStatus[wlanidx] = (SS_STATUS_Tp*)malloc(sizeof(SS_STATUS_T));		
		ptmpSS = malloc(sizeof(SS_STATUS_T));
		pStatus[wlanidx] = ptmpSS;		
		if ( pStatus[wlanidx]==NULL ) {
			goto ss_err;
		}
		memset(pStatus[wlanidx],0,sizeof(pStatus[wlanidx]));
	}

	snprintf(ifname, sizeof(ifname), "wlan%d", wlanidx);
	WLAN_DEBUG("ifname:%s\n", ifname);

	if (1) {
		/* issue scan request */
		wait_time = 0;
		while (1) {		
			//printf("%s is getWlSiteSurveyRequest...\n", ifname);
			switch(getWlSiteSurveyRequest(ifname, &status)) 
			{ 
				case -2: 
					printf("Auto scan running!!\n"); 
					strcpy(tmpBuf, ("<script>dw(wlsurvey_auto_scan_running)</script>")); 					
					break; 
				case -1: 
					printf("Site-survey request failed!\n"); 
					strcpy(tmpBuf, ("<script>dw(wlsurvey_site_survey_failed)</script>"));					 
					break; 
				default: 
					break; 
			} 		
			if (status != 0) {	// not ready
				if (wait_time++ > WAIT_TIME_DEFINE) {
					strcpy(tmpBuf, ("<script>dw(wlsurvey_scan_request_timout)</script>"));
					goto ss_err;
				}
#ifdef	CONFIG_RTK_MESH		
				usleep(1000000 + (rand() % 2000000));
#else
				sleep(1);
#endif
			}
			else
				break;
		}

		/* wait until scan completely */
		wait_time = 0;
		while (1) {
			//printf("%s is getWlSiteSurveyResult...\n", ifname);
			res = 1;	// only request request status
			if ( getWlSiteSurveyResult(ifname, (SS_STATUS_Tp)&res) < 0 ) {
				strcpy(tmpBuf, ("<script>dw(wlsurvey_read_site_error)</script>"));
				free(pStatus[wlanidx]);
				pStatus[wlanidx] = NULL;
				goto ss_err;
			}
			if (res == 0xff) {   // in progress
			#if (defined(CONFIG_RTL_92D_SUPPORT) && defined (CONFIG_POCKET_ROUTER_SUPPORT)) || defined(CONFIG_RTL_DFS_SUPPORT)
				/*prolong wait time due to scan both 2.4G and 5G */
				if (wait_time++ > WAIT_TIME_DEFINE) 
			#else
				if (wait_time++ > WAIT_TIME_DEFINE) 
			#endif		
			{
				strcpy(tmpBuf, ("<script>dw(wlsurvey_scan_timeout)</script>"));
				free(pStatus[wlanidx]);
				pStatus[wlanidx] = NULL;
				goto ss_err;
			}
				sleep(1);
			}
			else
				break;
		}	
	}
    
	return 1;
	
ss_err:
	return -1;
	//ERR_MSG(tmpBuf);
}

/* 2G and 5G site survey */
int draw_wlSiteSurveyTbl(request *wp, int argc, char **argv)
{
	int nBytesSent=0, i, j;
#ifdef CONFIG_RTK_MESH 
	int mesh_enable = 0; 
    char meshidbuf[40];
#endif 
	BssDscr *pBss;
	char tmpBuf[MAX_MSG_BUFFER_SIZE], ssidbuf[40], tmp1Buf[10], tmp2Buf[20], wpa_tkip_aes[20],wpa2_tkip_aes[20], tmp3Buf[128];
	WLAN_MODE_T mode;
	bss_info bss;
	unsigned char ifname[10] = {0};	
	int bss_idx = 0, is_enc = 0;
	int total_bss_num = 0;
	char tmp4Buf[30];
	char *wlan_idx_str;
	int wlanidx = 0;	
	int ret = 0;
	SS_STATUS_DUAL_Tp pStatusSort = NULL, pSortTmp;

	apmib_save_wlanIdx();

	/* malloc for sort array */
	if (pStatusSort==NULL) {			
		pSortTmp = malloc(sizeof(SS_STATUS_DUAL_T));
		pStatusSort = pSortTmp;		
		if ( pStatusSort==NULL ) {
			goto sitesurvey_err;
		}
		memset(pStatusSort, 0, sizeof(SS_STATUS_DUAL_T));
	}

	/* every wlan inetrface do site survey, and get ss result */
	for(j=0; j<NUM_WLAN_INTERFACE; ++j) {		
		ret = doWlSiteSurvey(j);
		if(ret<0){
			printf("do site survey failed!\n");
			goto sitesurvey_err;
		}
		
		snprintf(ifname, sizeof(ifname), "wlan%d", j);		
		pStatus[j]->number = 0; // request BSS DB
		if ( getWlSiteSurveyResult(ifname, pStatus[j]) < 0 ) {	
			printf("Read site-survey status failed!\n");
			goto sitesurvey_err;
		}

		if ( getWlBssInfo(ifname, &bss) < 0) {
			printf("Get bssinfo failed!\n");
			goto sitesurvey_err;
		}
	}

	/* sort ss result */
	sortSiteSurveyResult(pStatusSort);

	/* draw ss result by sort */
	WLAN_DEBUG("###pStatusSort->number:%d\n", pStatusSort->number);				
	for (i=0; i<pStatusSort->number && pStatusSort->number!=0xff; i++) 
	{
		pBss = &pStatusSort->bssdb[i];
		snprintf(tmpBuf, 200, ("%02x:%02x:%02x:%02x:%02x:%02x"),
			pBss->bdBssId[0], pBss->bdBssId[1], pBss->bdBssId[2],
			pBss->bdBssId[3], pBss->bdBssId[4], pBss->bdBssId[5]);

		memcpy(ssidbuf, pBss->bdSsIdBuf, pBss->bdSsId.Length);
		ssidbuf[pBss->bdSsId.Length] = '\0';
		translateChar(ssidbuf,tmp3Buf,sizeof(tmp3Buf));		
		bss_idx = i+1;			
		memset(wpa_tkip_aes,0x00,sizeof(wpa_tkip_aes));
		memset(wpa2_tkip_aes,0x00,sizeof(wpa2_tkip_aes));		
		if ((pBss->bdCap & cPrivacy) == 0){
			sprintf(tmp4Buf, "NONE");
			is_enc = 0;	
		}
		else {
			is_enc = 1;	
			if (pBss->bdTstamp[0] == 0)
				sprintf(tmp4Buf, "WEP");
#if defined(CONFIG_RTL_WAPI_SUPPORT)
			else if (pBss->bdTstamp[0] == SECURITY_INFO_WAPI)
				sprintf(tmp4Buf, "WAPI");
#endif
			else {
				int wpa_exist = 0, idx = 0;
				if (pBss->bdTstamp[0] & 0x0000ffff) {						
					wpa_exist = 1;
					if (((pBss->bdTstamp[0] & 0x00000f00) >> 8) == 0x5){
						sprintf(tmp4Buf,"%s","WPA1PSK/TKIP;AES");
					}							
					else if (((pBss->bdTstamp[0] & 0x00000f00) >> 8) == 0x4){
						sprintf(tmp4Buf,"%s","WPA1PSK/AES");
					}							
					else if (((pBss->bdTstamp[0] & 0x00000f00) >> 8) == 0x1){
						sprintf(tmp4Buf,"%s","WPA1PSK/TKIP");
					}							
				}
				if (pBss->bdTstamp[0] & 0xffff0000) {
					if (wpa_exist){
						if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x5)								
							sprintf(tmp4Buf,"%s","WPA1PSKWPA2PSK/TKIPAES");
						else if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x4)
							sprintf(tmp4Buf,"%s","WPA1PSKWPA2PSK/AES");
						else if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x1)								
							sprintf(tmp4Buf,"%s","WPA1PSKWPA2PSK/TKIP");
					}else{
						if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x5)
							sprintf(tmp4Buf,"%s","WPA2PSK/TKIPAES");
						else if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x4)
							sprintf(tmp4Buf,"%s","WPA2PSK/AES");
						else if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x1)
							sprintf(tmp4Buf,"%s","WPA2PSK/TKIP");
					}						
				}
			}
		}	
		/* FORMAT:
			<tr style="cursor:pointer;" id="1" onClick="apcliSSID(this)">
			<td id="channel1">1</td><td id="ssid1">edgar-netcore</td>
			<td id="security1">NONE</td><td id="signal1">70</td>
			<td><input type=\"radio\" name=\"rowSelect\" id=\"select%d\"></td>
			</tr>
		*/			
		nBytesSent += req_format_write
		(wp, 
			(	
			"<tr id=\"%d\">"
			"<td id=\"channel%d\">%d</td>"
			"<td id=\"ssid%d\">%s</td>"
			"<td id=\"bssid%d\" style=\"display:none;\">%s</td>"
			"<td id=\"security%d\">%s</td>"
			"<td id=\"signal%d\">%d</td>"
			"<td><input type=\"radio\" name=\"rowSelect\" id=\"select%d\"></td>"
			"</tr>"			
			), bss_idx, bss_idx, pBss->ChannelNumber,  bss_idx, tmp3Buf, 
			bss_idx, tmpBuf, bss_idx, tmp4Buf, bss_idx, pBss->rssi, bss_idx
		);			
	}		 			
	
	apmib_recov_wlanIdx();

	if(pStatusSort)
		free(pStatusSort);

	/* if used by he_vxd_connect, then core dump */
#if 0	
	for(j=0; j<NUM_WLAN_INTERFACE; ++j){
		if(pStatus[j])
			free(pStatus[j]); 
		pStatus[j] = NULL;
	}
#endif
	
	return nBytesSent;

sitesurvey_err:
	apmib_recov_wlanIdx();

	if(pStatusSort)
		free(pStatusSort);

	for(j=0; j<NUM_WLAN_INTERFACE; ++j){
		if(pStatus[j])
			free(pStatus[j]); 
		pStatus[j] = NULL;
	}
	
	return 0;
}

void restartAndmuProcesses(int opmode) 
{
#if defined(CONFIG_APP_SDK_ONENET)
	char cmdBuffer[100];
	char vendor[32] = {0};
	unsigned char hw_mac[32] = {0};
	char sn[32] = {0};

	system("killall edp_client");
#endif

#if defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)
	system("killall -9 andlink");
#endif

#if defined(CONFIG_APP_SDK_ONENET)
	if(opmode==GATEWAY_MODE)
	{
		extern char *fwVersion;
		
		apmib_get(MIB_RTL_LINK_VENDOR, (void *)vendor);
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)hw_mac);
		apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)sn);
		snprintf(cmdBuffer, sizeof(cmdBuffer), 
				"edp_client -n %s -m %02X:%02X:%02X:%02X:%02X:%02X -s %s -v %s &", 
				sn, hw_mac[0], hw_mac[1], hw_mac[2], hw_mac[3], hw_mac[4], hw_mac[5],
				fwVersion, vendor);
		printf("edp_client -n %s -m %02X:%02X:%02X:%02X:%02X:%02X -s %s -v %s \n", 
				sn, hw_mac[0], hw_mac[1], hw_mac[2], hw_mac[3], hw_mac[4], hw_mac[5],
				fwVersion, vendor);
		system(cmdBuffer);		
	}
#endif

	return;
}


void heUiOpMode(request *wp, char *path, char *query)
{
	typedef enum {RTL_LINK_BRIDGE, RTL_LINK_REPEATER, RTL_LINK_GW, RTL_LINK_WISP} RTL_LINK_MODE_T;
	char *strVal=NULL;
	int opmode=0, wlan_idx=0;
	char errMsg[256]={0};	
	unsigned char tmpBuf[100] = {0};
	int mode = 0;
	int ret = -1;
	unsigned int setAndlinkValue = RTL_LINK_BRIDGE;
	unsigned char buffer1[64]={0};
	unsigned char lanip[64]={0};
	char lanip_1[64]={0},lanip_2[64]={0},lanip_mask[64]={0},cmd[64]={0};			
	char lanip0[8]={0},lanip1[8]={0},lanip2[8]={0},lanip3[8]={0};
	struct in_addr inIp1={0};

	if(!apmib_get( MIB_IP_ADDR,  (void *)&buffer1))
	{
		printf("Get ip addr fail\n");
	}
	
	sprintf(lanip,"%s",inet_ntoa(*((struct in_addr *)buffer1)));
	
	sscanf(lanip,"%[^.].%[^.].%[^.].%s",lanip0,lanip1,lanip2,lanip3);
	sprintf(lanip_1,"%s.%s.%s.1",lanip0,lanip1,lanip2);
	sprintf(lanip_2,"%s.%s.%s.253",lanip0,lanip1,lanip2);
	sprintf(lanip_mask,"%s.%s.%s.255",lanip0,lanip1,lanip2);

#ifdef CONFIG_STATIC_IP	
//	sprintf(cmd,"ifconfig br0:1 netmask 255.255.255.0 broadcast down",lanip_1,lanip_mask);
	system("ifconfig br0:1 down");
#endif
	system("rm /var/lib/misc/udhcpd.leases");
	strVal = req_get_cstream_var(wp, "opMode", "");	
	printf("%s:%d strVal=%s\n",__FUNCTION__,__LINE__,strVal);
	if(strVal[0]){
		opmode = strVal[0] - '0' ;
		if ( apmib_set(MIB_OP_MODE, (void *)&opmode) == 0) {
			strcpy(errMsg, ("Set Opmode error!"));
			goto heUiOpMode_ERR;
		}
	}
	if(opmode==3) // repeater mode
	{
		int rptr_enabled = 1, wlan_disabled = 0, rpt_connect = 0;
		int BUF_SIZE = 20;
		char wlan_vxd_if[BUF_SIZE];
		int i = 0;
		unsigned int setValue=0;

		if(!apmib_get( MIB_IP_ADDR,  (void *)&buffer1))
		{
			printf("Get ip addr fail\n");
		}
		else
		{
			sprintf(lanip,"%s",inet_ntoa(*((struct in_addr *)buffer1)));
			
#ifdef CONFIG_STATIC_IP
			if(strcmp(lanip,lanip_1)==0)
			{
				printf("set ip addr %s \n",lanip_2);
				if ( !inet_aton(lanip_2, &inIp1) )
				{
					printf("error!!\n");
				}

				if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp1))
				{
					printf("set ip addr error!!\n");
				}
			}
#else
			if ( !inet_aton(lanip, &inIp1) )
			{
				printf("error!!\n");
			}

			if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp1))
			{
				printf("set ip addr error!!\n");
			}
#endif
		}

		setAndlinkValue = RTL_LINK_REPEATER;
		opmode = 1;
		apmib_set(MIB_OP_MODE, (void *)&opmode);
		apmib_set(MIB_REPEATER_MODE, (void *)&rptr_enabled);
		apmib_get(MIB_REPEATER_CONNECT, (void *)&rpt_connect);
		if(rpt_connect!=2){
			apmib_set(MIB_REPEATER_ENABLED1, (void *)&rptr_enabled);			
		}
		if(rpt_connect!=1){
			apmib_set(MIB_REPEATER_ENABLED2,(void *)&rptr_enabled);	
		}			

		apmib_save_wlanIdx();
		for(i=0; i<NUM_WLAN_INTERFACE;++i){
			snprintf(wlan_vxd_if, BUF_SIZE, "wlan%d-vxd", i);
			SetWlan_idx(wlan_vxd_if);
			if(i==0 && rpt_connect!=2){				
				apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disabled);
				setValue=CLIENT_MODE;
				apmib_set(MIB_WLAN_MODE,(void *)&setValue);	
			}else if(i==1 && rpt_connect!=1){
				apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disabled);
				setValue=CLIENT_MODE;
				apmib_set(MIB_WLAN_MODE,(void *)&setValue);	
			}					
		}	
		apmib_recov_wlanIdx();	
		
		ret = he_vxd_connect(wp, tmpBuf, &mode, wlan_idx); // wlan_idx is useless
	}
	else
	{
		if (GATEWAY_MODE == opmode)
		{
			if(!apmib_get( MIB_IP_ADDR,  (void *)&buffer1))
			{
				printf("Get ip addr fail\n");
			}
			else
			{
				sprintf(lanip,"%s",inet_ntoa(*((struct in_addr *)buffer1)));
#ifdef CONFIG_STATIC_IP
				if(strcmp(lanip,lanip_2)==0)
				{
					printf("set ip addr %s \n",lanip_1);
					if ( !inet_aton(lanip_1, &inIp1) )
					{
						printf("error!!\n");
					}

					if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp1))
					{
						printf("set ip addr error!!\n");
					}
				}
#else
				if ( !inet_aton(lanip, &inIp1) )
				{
					printf("error!!\n");
				}

				if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp1))
				{
					printf("set ip addr error!!\n");
				}
#endif
			}
			setAndlinkValue = RTL_LINK_GW;

			//switch to dhcp server mode
			int lan_dhcp_type = DHCP_SERVER;
			apmib_set(MIB_DHCP, (void *)&lan_dhcp_type);
		
		}
		else 
		{
			if(!apmib_get( MIB_IP_ADDR,  (void *)&buffer1))
			{
				printf("Get ip addr fail\n");
			}
			else
			{
				sprintf(lanip,"%s",inet_ntoa(*((struct in_addr *)buffer1)));
#ifdef CONFIG_STATIC_IP
				if(strcmp(lanip,lanip_1)==0)
				{
					printf("set ip addr %s\n",lanip_2);
					if ( !inet_aton(lanip_2, &inIp1) ) 
					{
						printf("error!!\n");
					}

					if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp1)) 
					{
						printf("set ip addr error!!\n");
					}
				}
#else
				if ( !inet_aton(lanip, &inIp1) )
				{
					printf("error!!\n");
				}

				if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp1))
				{
					printf("set ip addr error!!\n");
				}
#endif
			}

			setAndlinkValue = RTL_LINK_BRIDGE;
		}
		int rpt_enabled=0;
		apmib_set(MIB_REPEATER_MODE, (void *)&rpt_enabled);
		apmib_set(MIB_REPEATER_ENABLED1, (void *)&rpt_enabled);
		apmib_set(MIB_REPEATER_ENABLED2,(void *)&rpt_enabled);
	}
	
	//set andlink operation mode
	apmib_set(MIB_RTL_LINK_MODE,(void *)&setAndlinkValue);

	/* if not back from site survey flow, then do reinit*/
	if(ret)	
		doInitInternet(wp,path,query);

	//restart andlink��edp_client or elink
	restartAndmuProcesses(opmode);
	
	return;
heUiOpMode_ERR:
	//ERR_MSG(errMsg);
	return;
}

void he_wirelessGetMAClist(request *wp, char *path, char *query)
{
	WLAN_DEBUG("\n");
	int nBytesSent=0, entryNum, i;
	MACFILTER_T entry;
	char tmpBuf[MAX_MSG_BUFFER_SIZE]={0};
	char *buf = NULL;
	int enabled;
	char *ssidIndex = NULL;
	unsigned char macInfo[6];
	ssidIndex = req_get_cstream_var(wp, ("ssidIndex"), "");
	apmib_save_wlanIdx();

	if (NUM_WLAN_INTERFACE==2) {/*dual band*/
		if (ssidIndex[0] == '0') {/*2G*/		
			SetWlan_idx("wlan1");
		} else if(ssidIndex[0] == '1') {/*5g*/
			SetWlan_idx("wlan0");
		}		
	} else {/*sigle band, use wlan0*/	
		SetWlan_idx("wlan0");
	}
	
	if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
		fprintf(stderr, ("Set enabled flag error!"));
		goto GetMAClist_END;
	}
	if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) {
  		fprintf(stderr, "Get table entry error!\n");
		goto GetMAClist_END;
	}
	if (!apmib_get(MIB_HW_WLAN_ADDR, (void *)&macInfo)) {
  		fprintf(stderr, "Get Wlan MAC error!\n");
		goto GetMAClist_END;
	}

	buf = calloc(82*entryNum+1, 1); 
	if (buf == NULL) {
		fprintf(stderr, "[%s %d] malloc failed!\n", __FUNCTION__,__LINE__);
		goto GetMAClist_END;
	}
	
	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&entry))
			goto GetMAClist_END;

		if (strlen(entry.comment)) {
			snprintf(tmpBuf, MAX_MSG_BUFFER_SIZE, ("%02x:%02x:%02x:%02x:%02x:%02x/%s;"),
				entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], entry.comment);
		} else {
			snprintf(tmpBuf, MAX_MSG_BUFFER_SIZE, ("%02x:%02x:%02x:%02x:%02x:%02x;"),
				entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		}
		
		strcat(buf, tmpBuf);
	}

	if(entryNum){
		// remove the end ';'
		buf[strlen(buf)-1] = '\0';
	}
	
	WLAN_DEBUG("&%s&%d&%02x:%02x:%02x:%02x:%02x:%02x&&\n", buf, enabled, macInfo[0],macInfo[1],
					macInfo[2],macInfo[3],macInfo[4],macInfo[5]);	
	req_format_write(wp, ("&%s&%d&%02x:%02x:%02x:%02x:%02x:%02x&&"), buf, enabled, macInfo[0],macInfo[1],
		macInfo[2],macInfo[3],macInfo[4],macInfo[5]);

GetMAClist_END:
	apmib_recov_wlanIdx();
	
	if(buf)
		free(buf);
}

#if 0
int string_to_mac(char *string, unsigned char *mac, int len)
{
	int i;
	
	if (string==NULL || mac==NULL)
		return 0;
		
	if (string[2]==':' && len==17) {		
		if(sscanf(string, "%02x:%02x:%02x:%02x:%02x:%02x", (mac+0), (mac+1), (mac+2), (mac+3), (mac+4), (mac+5))!=6)
			return 0;
		printf("string:%s, mac:%02x:%02x:%02x:%02x:%02x:%02x\n", string, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5] );
	} else if (string[2]=='-' && len==17) {	
		if(sscanf(string,"%x-%x-%x-%x-%x-%x", (mac+0), (mac+1), (mac+2), (mac+3), (mac+4), (mac+5))!=6)
			return 0;
	} else if (len==12) {
		string_to_hex(mac, string, 12);
	} else {
		return 0;
	}
	
	return 1;
}
#else
int string_to_mac(char *string, unsigned char *mac, int len)
{
	int i, idx=0;
	unsigned char tmp[13] = {0};	
	
	if (string==NULL || mac==NULL)
		return 0;
	
	if (len!=17 && len!=12)
		return 0;	
	
	for(i=0; i<len; ++i){
		if( string[i]!=':' && string[i]!='-'){
			tmp[idx++] = string[i];				
		}
	}
	tmp[idx]='\0';
	
	string_to_hex(tmp, mac, 12);	
	return 1;
}
#endif

#if 0
//static char *wireless_delete_buf; 	// delete mac entry index
void he_wireless_mac_delete(unsigned char *buf)
{	
	WLAN_DEBUG("\n");
	MACFILTER_T macEntry;
	char *tok;
	char tmpBuf[MAX_MSG_BUFFER_SIZE];
	unsigned char wlan_ifname[10] = {0};
	unsigned int entryNum = 0;
	int i = 0, wlanidx=0;

	apmib_save_wlanIdx();
	// ssidIndex=0
	for(i=0; i<strlen(wireless_delete_buf);++i){
		if(wireless_delete_buf[i]=='s' && wireless_delete_buf[i+1]=='s' 
			&& wireless_delete_buf[i+2]=='i' && wireless_delete_buf[i+3]=='d')
			if(wireless_delete_buf[i+10]=='0'){ // 2g
				if(NUM_WLAN_INTERFACE==2)
					wlanidx = 1;
				else
					wlanidx = 0;
			}				
			else								// 5g
				wlanidx = 0;
	}	
	snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan%d", wlanidx);
	SetWlan_idx(wlan_ifname);	
	WLAN_DEBUG("wlan_ifname:%s\n", wlan_ifname);

	if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) {
		strcpy(tmpBuf, ("Get entry number error!"));
		goto setErr_ac;
	}

	tok = strsep(&wireless_delete_buf, ";");	
#if 1
	while(tok)
	{				
		/* Delete entry */				
		for (i=entryNum; i>0; i--) {			
			if( i==(atoi(tok) + 1) ){		
				memset(&macEntry, 0, sizeof(MACFILTER_T));
				*((char *)&macEntry) = (char)i;
				WLAN_DEBUG("#### entryNum:%d, to delete index:%d\n", entryNum, *((char *)&macEntry));
				if ( !apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&macEntry)) {
					strcpy(tmpBuf, ("Get table entry error!"));
					goto setErr_ac;
				}

				WLAN_DEBUG("###to del mac %02x:%02x:%02x:%02x:%02x:%02x\n", 
					macEntry.macAddr[0], macEntry.macAddr[1], macEntry.macAddr[2],
					macEntry.macAddr[3], macEntry.macAddr[4], macEntry.macAddr[5]);
				
				if ( !apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry)) {
					strcpy(tmpBuf, ("Delete table entry error!"));
					goto setErr_ac;
				}	
				break;
			}
		}
				
		tok = strsep(&wireless_delete_buf, ";");
		//printf("tok=%p\n",tok);
	}
#else	
	while(tok)
	{				
		*((char *)&macEntry) = atoi(tok) + 1;
		WLAN_DEBUG("#### to delete index:%d\n", *((char *)&macEntry));
		
		if (!apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&macEntry)) {
			fprintf(stderr, "[%s %d] Get table entry error!", __FUNCTION__, __LINE__);
			goto WL_DEL_END;
		}
		
		WLAN_DEBUG("###to del mac %02x:%02x:%02x:%02x:%02x:%02x\n", 
			macEntry.macAddr[0], macEntry.macAddr[1], macEntry.macAddr[2],
			macEntry.macAddr[3], macEntry.macAddr[4], macEntry.macAddr[5]);
		
		if (!apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry)) {
			fprintf(stderr, sizeof(tmpBuf), "[%s %d] Delete table entry error!", __FUNCTION__, __LINE__);
			goto WL_DEL_END;
		}
		
		tok = strsep(&wireless_delete_buf, ";");
		//printf("tok=%p\n",tok);
	}
#endif

setErr_ac:	
	apmib_recov_wlanIdx();
	
	if(wireless_delete_buf)
		free(wireless_delete_buf);	
}
#endif

void he_APBatchDeleteAccessPolicyList(request *wp, char *path, char *query)
{	
	int len = 0;
	unsigned int addMac = 0;	
	unsigned char *dataBuf = NULL;
	MACFILTER_T macEntry;
	char *tok;
	char tmpBuf[MAX_MSG_BUFFER_SIZE];
	unsigned char wlan_ifname[10] = {0};
	unsigned int entryNum = 0;
	int i = 0, wlanidx=0;
	unsigned int macEnable = 0;

	len = wp->post_data_len;	
	dataBuf = (char *)calloc(len+1, 1);
	if (dataBuf == NULL) {
		fprintf(stderr, "[%s %d] calloc failed,len=%d\n", __FUNCTION__, __LINE__, len);
		return;
	}

	wp->post_data_idx = 0;
	memcpy(dataBuf, wp->post_data, len);	

	WLAN_DEBUG("dataBuf:%s\n", dataBuf);
	if(strstr(dataBuf, "addMac"))
		addMac = 1;	

	apmib_save_wlanIdx();
	
	// format: ssidIndex=0
	for(i=0; i<MAX_MSG_BUFFER_SIZE;++i){
		if(dataBuf[i]=='s' && dataBuf[i+1]=='s' 
			&& dataBuf[i+2]=='i' && dataBuf[i+3]=='d')
			if(dataBuf[i+10]=='0'){ // 2g
				if(NUM_WLAN_INTERFACE==2)
					wlanidx = 1;
				else
					wlanidx = 0;
			}				
			else								// 5g
				wlanidx = 0;
	}	
	snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan%d", wlanidx);
	SetWlan_idx(wlan_ifname);	
	WLAN_DEBUG("wlan_ifname:%s\n", wlan_ifname);

	if(addMac==0){
		// format: macEnable=0
		for(i=0; i<MAX_MSG_BUFFER_SIZE;++i){
			if(dataBuf[i]=='m' && dataBuf[i+1]=='a' 
				&& dataBuf[i+2]=='c' && dataBuf[i+3]=='E'){
				macEnable = dataBuf[i+10]-'0';
				apmib_set(MIB_WLAN_MACAC_ENABLED, (void *)&macEnable);
				WLAN_DEBUG("macEnable:%d\n", macEnable);
				break;
			}
		}	
	}	

	if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) {
		strcpy(tmpBuf, ("Get entry number error!"));
		goto setErr_ac;
	}

	tok = strsep(&dataBuf, ";");
	while(tok)
	{				
		/* Delete entry */				
		for (i=entryNum; i>0; i--) {			
			if( i==(atoi(tok) + 1) ){		
				memset(&macEntry, 0, sizeof(MACFILTER_T));
				*((char *)&macEntry) = (char)i;
				WLAN_DEBUG("#### entryNum:%d, to delete index:%d\n", entryNum, *((char *)&macEntry));
				if ( !apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&macEntry)) {
					strcpy(tmpBuf, ("Get table entry error!"));
					goto setErr_ac;
				}

				WLAN_DEBUG("###to del mac %02x:%02x:%02x:%02x:%02x:%02x\n", 
					macEntry.macAddr[0], macEntry.macAddr[1], macEntry.macAddr[2],
					macEntry.macAddr[3], macEntry.macAddr[4], macEntry.macAddr[5]);
				
				if ( !apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry)) {
					strcpy(tmpBuf, ("Delete table entry error!"));
					goto setErr_ac;
				}	
				break;
			}
		}
				
		tok = strsep(&dataBuf, ";");
	}
setac_end:	
	apmib_recov_wlanIdx();	
	if(dataBuf)
		free(dataBuf);

	/*Call it while HTTP Client post /boafrm/he_APSecurity,
	  If the HTTP Client donot post /boafrm/he_APSecurity , then reinit */
	if(addMac==0){
		apmib_update_web(CURRENT_SETTING);
#ifndef NO_ACTION 	
		run_init_script_flag = 1;
		run_init_script("all");
#endif		
	}
	return;	

setErr_ac:		
	if(dataBuf)
		free(dataBuf);
	fprintf(stderr,tmpBuf);
	apmib_recov_wlanIdx();	
	return;
	//req_format_write(wp, ("ok\tdown"));
}

void he_APSecurity(request *wp, char *path, char *query)
{	
	char *strMac =NULL, *strEnabled=NULL, *ssidIndex=NULL, *phyband=NULL;
	char tmpBuf[MAX_MSG_BUFFER_SIZE] = {0};
	int entryNum, i, enabled;
	MACFILTER_T macEntry, macEntrytmp;
	int j = 0;
	unsigned char wlan_ifname[10] = {0};
	unsigned char mac_tmp[6] = {0};

	apmib_save_wlanIdx();
	
	memset(&macEntry, 0, sizeof(MACFILTER_T));	
	strMac = req_get_cstream_var(wp, ("newap_text_0"), "");
	strEnabled = req_get_cstream_var(wp, ("apselect_0"), "");	
	
	if (strEnabled[0] == 0) {
		snprintf(tmpBuf, sizeof(tmpBuf), ("Error!Enabled is null!"));
		goto setErr_ac;
	}	

	/* phyband */	
	phyband = req_get_cstream_var(wp, ("phyband"), "");
	WLAN_DEBUG("phyband:%s\n", phyband);
	if(!strcmp(phyband, "2")){
		if(NUM_WLAN_INTERFACE==2)
			snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan1");		
		else 
			snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan0");
	}else if(!strcmp(phyband, "5")){		
		snprintf(wlan_ifname, sizeof(wlan_ifname), "wlan0");
	}	
	SetWlan_idx(wlan_ifname);	
/*
	if (wireless_mac_delete) {
		he_wireless_mac_delete();
	}
*/	
	enabled = strEnabled[0] - '0';
	WLAN_DEBUG("#### wlan_ifname:%s, MACAC_ENABLED:%d\n", wlan_ifname, enabled);
	if (apmib_set( MIB_WLAN_MACAC_ENABLED, (void *)&enabled) == 0) {
		snprintf(tmpBuf, sizeof(tmpBuf), ("Set enabled flag error!"));
		goto setErr_ac;
	}

	/*Not add the mac*/
	if(strMac[0] == 0){		
		goto setOK_ac;
	}
	
	if (!string_to_mac(strMac, (unsigned char *)macEntry.macAddr, strlen(strMac))) {
		snprintf(tmpBuf, sizeof(tmpBuf), ("Error! Invalid MAC address."));
		goto setErr_ac;
	}

	if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) {
		snprintf(tmpBuf, sizeof(tmpBuf), ("Get entry number error!"));
		goto setErr_ac;
	}
	
	if ((entryNum + 1) > MAX_WLAN_AC_NUM) {
		snprintf(tmpBuf, sizeof(tmpBuf), ("Cannot add new entry because table is full!"));
		goto setErr_ac;
	}
	
	//Check if same rule exist
	for(j=1;j<=entryNum;j++)
	{
		memset(&macEntrytmp, 0x00, sizeof(macEntrytmp));
		*((char *)&macEntrytmp) = (char)j;
		if (apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&macEntrytmp)) {
			if (!memcmp(macEntrytmp.macAddr, macEntry.macAddr, 6)) {
				snprintf(tmpBuf, sizeof(tmpBuf), ("rule already exist!"));
				goto setErr_ac;
			}
				
		}
	}

	// set to MIB. try to delete it first to avoid duplicate case
	apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry);
	if (apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry) == 0) {
		snprintf(tmpBuf, sizeof(tmpBuf), ("Add table entry error!"));
		goto setErr_ac;
	}
	
setOK_ac:
	apmib_recov_wlanIdx();

#if 1
	apmib_update_web(CURRENT_SETTING);
#ifndef NO_ACTION 	
	run_init_script_flag = 1;
	run_init_script("all");
#endif
#endif
  	return;

setErr_ac:
	//ERR_MSG(tmpBuf);
	fprintf(stderr,tmpBuf);
	apmib_recov_wlanIdx();
}

int get_file_via_url(unsigned char* url,unsigned char* dstName)
{
	int ret=0;
	int retry_count=0;
	unsigned char command[128]={0};
	
#define MAX_RETRY_COUNT 5

	if(!url || !url[0] || !dstName || !dstName[0]) return 0;
	
	snprintf(command,sizeof(command),"wget %s -O %s",url,dstName);

	do
	{
		//if ret is -1, it meams system() is interrupt by signal
		ret=system(command); 
		retry_count++;
	}while(ret==-1 && retry_count<=MAX_RETRY_COUNT);
	
	//printf("\n%s:%d ret=%d\n",__FUNCTION__,__LINE__,ret);
	
	if(ret==0)
	{
		//printf("\n%s:%d retry_count=%d\n",__FUNCTION__,__LINE__,retry_count);
		return 1;
	}
	else
		return 0;
}

int upgradeImage_via_url(char* url)
{
	char errmsg[128]={0};
	struct stat fileStat={0};
	char * updateData=NULL;
	int updateLen=0;
	char updateTime[64]={0};
	time_t currentTime={0};
	FILE *fp=NULL;
	if(!url||!url[0]) return 1;

	if(get_file_via_url(url,UPDATE_IMAGE_FILE)!=1)
		return 0;
	if(stat(UPDATE_IMAGE_FILE,&fileStat)<0){
		fprintf(stderr,"stat %s fail! %s:%d\n",UPDATE_IMAGE_FILE,__FILE__,__LINE__);
		return 0;
	}
	updateLen=fileStat.st_size;
	//fprintf(stderr,"%s:%d updateLen=%d\n",__FUNCTION__,__LINE__,updateLen);
	if((updateData=malloc(updateLen))==NULL){
		fprintf(stderr,"malloc %d fail! %s:%d\n",updateLen,__FILE__,__LINE__);
		return 0;
	}
	if((fp=fopen(UPDATE_IMAGE_FILE,"r"))==NULL){
		snprintf(errmsg,sizeof(errmsg),"open %s fail %s:%d\n",UPDATE_IMAGE_FILE,__FILE__,__LINE__);
		goto UPGRADEIMAGE_VIA_URL_FAIL;
	}
	if(fread(updateData,updateLen,1,fp)!=1){
		snprintf(errmsg,sizeof(errmsg),"read %s fail %s:%d\n",UPDATE_IMAGE_FILE,__FILE__,__LINE__);
		goto UPGRADEIMAGE_VIA_URL_FAIL;
	}
	fclose(fp);
	fp=NULL;
	time(&currentTime);
	//printf("%s:%d time=%s\n",__FUNCTION__,__LINE__,ctime(&currentTime));
	strcpy(updateTime,ctime(&currentTime));
	//printf("%s:%d updateTime=%s\n",__FUNCTION__,__LINE__,updateTime);
	apmib_set(MIB_UPDATE_TIME,(void*)updateTime);
	apmib_update(CURRENT_SETTING);
	if(upgrade_firmware(updateData,updateLen,errmsg,sizeof(errmsg))!=1){
		goto UPGRADEIMAGE_VIA_URL_FAIL;
	}
	free(updateData);
#ifndef CONFIG_APP_FWD
	REBOOT_WAIT_COMMAND(2);
#endif
	return 1;
UPGRADEIMAGE_VIA_URL_FAIL:
	if(updateData) free(updateData);
	if(fp) fclose(fp);	
	fprintf(stderr,"%s\n",errmsg);
	return 0;
}

void heUiAutoUpload(request *wp, char *path, char *query)
{
	int needUpdate=0;
	char url[256]={0};
	apmib_get(MIB_NEED_UPDATE,(void*)&needUpdate);
	if(needUpdate==0){
		fprintf(stderr,"needn't upgrade!\n");
		send_redirect_perm(wp,"herouter/sysupdate.asp?uploadingFirmware=0");
		return;
	}
	apmib_get(MIB_UPDATE_URL,(void*)url);
	if(!url[0]){
		fprintf(stderr,"update url error!\n");
		send_redirect_perm(wp,"herouter/sysupdate.asp?uploadingFirmware=0");
		return;
	}
	if(upgradeImage_via_url(url)==0){
		fprintf(stderr,"update image fail!\n");
		send_redirect_perm(wp,"herouter/sysupdate.asp?uploadingFirmware=0");
		return;
	}
	send_redirect_perm(wp,"herouter/sysupdate.asp?uploadingFirmware=2");
	return;
}
