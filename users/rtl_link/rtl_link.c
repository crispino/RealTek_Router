#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <time.h>
#include <netdb.h>

#include "apmib.h"
#include "rtl_link_utils.h"
#include "rtl_link_list.h"
#include "../adapter-api/rtk_api/rtk_api.h"
#include "rtl_link.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.2/dmalloc.h"
#endif

char led_on=1;
char gw_ip[16]={0};

#ifdef MULTI_MAC_CLONE
static int find_clone_mac(int idx, unsigned char *mac, unsigned char *clone_mac)
{
    FILE *fp = NULL;
    char filename[64], line_buffer[1024], mac_s[13], clone_mac_s[13];
    int ret=0;

    snprintf(filename, sizeof(filename), "proc/wlan%d/msta_info", idx);
    if((fp= fopen(filename, "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
		return 0;
    }

    snprintf(mac_s, sizeof(mac_s), "%02X%02X%02X%02X%02X%02X", 
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';
		if(strstr(line_buffer, "Multi-STA Address Pool"))
		    break;
		if(strcasestr(line_buffer, mac_s)){
		    sscanf(line_buffer, "%*2d %*4x %*4x %*2x %*4d %s %*s %*s", clone_mac_s);
		    _string_to_hex(clone_mac_s, clone_mac, strlen(clone_mac_s));
		    ret = 1;
		    break;
		}
    }

    fclose(fp);
    return ret;
}

static int find_real_mac(int idx, unsigned char *mac, unsigned char *real_mac)
{
    FILE *fp = NULL;
    char filename[64], line_buffer[1024];
    int ret=0;

    snprintf(filename, sizeof(filename), "proc/wlan%d/msta_info", idx);
    if((fp= fopen(filename, "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
		return 0;
    }

    while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';
		if(strstr(line_buffer, "Multi-STA Address Pool"))
		    break;
		if(strcasestr(line_buffer, mac)){
		    sscanf(line_buffer, "%*2d %*4x %*4x %*2x %*4d %*s %s %*s", real_mac);
		    ret = 1;
		    break;
		}
    }

    fclose(fp);
    return ret;
}
#endif

void rtl_link_macFormat(char *dest, char *src)
{
    int i=0, j=0;

    while(i < strlen(src))
    {
        if(src[i] != ':')
            dest[j++] = src[i];
        i++;
    }
}

static void do_iwpriv(char *ifname, char *mibName, int mibType, void *value, int setMib)
{
	char cmd[100] = {0}, buf[100]={0};

	if(setMib){
		if(!ifname || !mibName){
			printf("invalid ifname or mibName\n");
			return;
		}
		
		switch(mibType){
			case MIB_INT_T:
				snprintf(cmd, sizeof(cmd), "iwpriv %s set_mib %s=%d", ifname, mibName, *(int *)value);
				break;
			default:
				break;
		}		
	}else{
		snprintf(cmd, sizeof(cmd), "%s", (char *)value);	
	}

	//printf("=====cmd:%s\n", cmd);	
	system(cmd);
	return;
}

#ifdef APPLY_IMMEDIATE
static void iwpriv_set_DeviceLimit(WLAN_SBWC_ENTRY_T sbwcEntry)
{
	int k, l, wlan_disable=1;
	char cmd[100]={0}, ifname[20]={0};
	
	for(k=0; k<NUM_WLAN_INTERFACE; k++){					        
        wlan_idx = k;
		for(l=0; l<=NUM_VWLAN; l++){
			vwlan_idx = l;
			memset(cmd, 0, sizeof(cmd));
			memset(ifname, 0, sizeof(ifname));
			if(vwlan_idx==0)
				snprintf(ifname, sizeof(ifname), "wlan%d", wlan_idx);
			else
				snprintf(ifname, sizeof(ifname), "wlan%d-va%d", wlan_idx, vwlan_idx-1);

			wlan_disable = 1;
			apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);
			if(wlan_disable==0){
				snprintf(cmd, sizeof(cmd), "iwpriv %s sta_bw_control %02x%02x%02x%02x%02x%02x,%d,%d", ifname,
					sbwcEntry.sbwc_sta_mac[0], sbwcEntry.sbwc_sta_mac[1], sbwcEntry.sbwc_sta_mac[2],
					sbwcEntry.sbwc_sta_mac[3], sbwcEntry.sbwc_sta_mac[4], sbwcEntry.sbwc_sta_mac[5],
					sbwcEntry.sbwc_sta_tx_limit, sbwcEntry.sbwc_sta_rx_limit);
				do_iwpriv(NULL, NULL, 0, (void *)cmd, 0);
			}							
		}
	} 
}
#endif

int rtl_link_get_file_value(char *filename, int *value)
{
	FILE *fp = NULL;
	char buff[1024] = {0};

	if(isFileExist(filename)){
		fp = fopen(filename, "r");
		if(fp==NULL){
			printf("open %s fail \n", filename);
			return -1;
		}
		
		while (fgets(buff, sizeof(buff), fp)) {
			buff[strlen(buff)-1]='\0';
			*value = atoi(buff);
			break;
		}

		fclose(fp);
	}
	
	return 0;	
}

int rtl_link_set_file_value(char *filename, int value)
{
	char buff[100] = {0};	
	
	snprintf(buff, sizeof(buff), "echo %d > %s", value, filename);
	system(buff);
	
	return 0;	
}


static void rtl_link_changeMacFormat(char *dest, char *src)
{
    int i=0, j=0;

    while(i < strlen(src))
    {
        if(src[i] != ':')
            dest[j++] = src[i];
        i++;
    }
}

static void rtl_link_get_clone_mac(STAInfo *sta)
{
    UPLINK_TYPE_T uplink_type;
    int idx, i;
    char ifname[16], vxd_mac[13]={0}, vxd_mac2[18]={0}; 
    unsigned char tmp_mac[6], clone_mac[6];

    uplink_type = rtl_link_getUplinkType(&idx);
    if(uplink_type == ETHERNET){
        memcpy(sta->clone_mac, sta->mac, sizeof(sta->clone_mac));
    }else{
        snprintf(ifname, sizeof(ifname), VXD_IF, idx);
        rtl_link_getWlanMacAddr(vxd_mac, ifname);
        
        for(i=0; i<12; i+=2){
            strncat(vxd_mac2, &vxd_mac[i], 2);
            strcat(vxd_mac2, ":");
        }
        vxd_mac2[17]='\0';
        
#ifdef MULTI_MAC_CLONE
        format_mac(sta->mac,tmp_mac);
        if(sta->uptype == WIRELESS){
            if(find_clone_mac(idx, tmp_mac, clone_mac)){
                snprintf(sta->clone_mac, sizeof(sta->clone_mac), "%02X:%02X:%02X:%02X:%02X:%02X", 
                    clone_mac[0],clone_mac[1],clone_mac[2],clone_mac[3],clone_mac[4],clone_mac[5]);
            }else{
                memcpy(sta->clone_mac, vxd_mac2, sizeof(sta->clone_mac));
            }
        }else{
            if(find_clone_mac(idx, tmp_mac, clone_mac)){
                memcpy(sta->clone_mac, sta->mac, sizeof(sta->clone_mac));
            }else{
                memcpy(sta->clone_mac, vxd_mac2, sizeof(sta->clone_mac));
            }
        }
#else
        memcpy(sta->clone_mac, vxd_mac2, sizeof(sta->clone_mac));
#endif
    }
}

/*Warnining: Need call list_destroy(&xxx, free) to avoid memory leak*/
static int rtl_link_get_wifi_device_mac_list(char *filename, List *list)
{
    STAInfo *sta;
	FILE *fp = NULL;
	char line_buffer[256], ssid[MAX_SSID_LEN], rssi_s[3];		
	char *pchar, *token;
	int band, sec=0, t, ret=0, i;
	char cnt=1;
            
	if(filename==NULL){
        ERR_PRINT("ERROR (%s)%d filename is NULL\n", __FUNCTION__, __LINE__);
		ret = -1; 
		goto EXIT;
    }
	
	if((fp= fopen(filename, "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
		ret = -1;
		goto EXIT;
    }
    
    fgets(line_buffer, sizeof(line_buffer), fp);
    line_buffer[strlen(line_buffer)-1]='\0';
    if((pchar=strstr(line_buffer, "active: 0")) !=NULL){
        ret = 0;
        goto EXIT;
    }

    apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
    memset(ssid, 0, sizeof(ssid));
    apmib_get(MIB_WLAN_SSID, (void *)ssid);
    
	while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';
		
		if((pchar=strstr(line_buffer, "hwaddr: "))!=NULL)
		{
		    sta = malloc(sizeof(STAInfo));
		    if(!sta){
                ERR_PRINT("ERROR (%s)%d malloc error\n", __FUNCTION__, __LINE__);
                ret = -1;
                goto EXIT;
            }
            memset(sta, 0, sizeof(STAInfo));
            sta->uptype = WIRELESS;
			pchar+=strlen("hwaddr: ");
			for(i=0; i<6; i++){
			    strncat(sta->mac, pchar+i*2, 2);
			    strcat(sta->mac, ":");
			}
			sta->mac[17]='\0';
			rtl_link_get_clone_mac(sta);
			list_insert(list, sta);
			sta->band = band;
			snprintf(sta->ssid, sizeof(sta->ssid), ssid);
		}
		else if((pchar=strstr(line_buffer, "rssi: "))!=NULL)
		{
		    pchar+=strlen("rssi: ");
		    snprintf(rssi_s, sizeof(rssi_s), pchar);
		    snprintf(sta->rssi, sizeof(sta->rssi), "%d", atoi(rssi_s)-100);
		}
		else if((pchar=strstr(line_buffer, "current_tx_rate: "))!=NULL)
		{
		    pchar = strrchr(line_buffer, ' ');
		    pchar += 1;
		    snprintf(sta->txrate, sizeof(sta->txrate), pchar);
		}
		else if((pchar=strstr(line_buffer, "current_rx_rate: "))!=NULL)
		{
		    pchar = strrchr(line_buffer, ' ');
		    pchar += 1;
		    snprintf(sta->rxrate, sizeof(sta->rxrate), pchar);
		}
		else if((pchar=strstr(line_buffer, "link_time: "))!=NULL)
		{
		    pchar+=strlen("link_time: ");
		    token = strtok(pchar, " ");
		    t = atoi(token);
		    cnt = 1;
		    sec = 0;
		    while((token = strtok(NULL, " ")))
            {
                if(cnt == 1){
                    if(!strcmp(token, "hr"))
                        sec += t*60*60;
                    else if(!strcmp(token, "min"))
                        sec += t*60;
                    else
                        sec += t;
                }else{
                    t = atoi(token);
                }
                cnt = !cnt;
            }
            snprintf(sta->uptime, sizeof(sta->uptime), "%d", sec);
		}
	}
EXIT:
    if(fp)
	    fclose(fp);
	return ret;		
}

int get_wifi_traffic_stats(char *filename, APTrafficStats *ap)
{
    FILE *fp;
	char line_buffer[256], ssid[MAX_SSID_LEN];
	char *pchar;
	int band;
	
    if(filename==NULL)
		return -1; 
	
	if((fp= fopen(filename, "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
		return -1;
    }

    apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
    ap->band = band;
    
    memset(ssid, 0, sizeof(ssid));
    apmib_get(MIB_WLAN_SSID, (void *)ssid);
    snprintf(ap->ssid, sizeof(ap->ssid), ssid);

    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        line_buffer[strlen(line_buffer)-1]='\0';
        
        if((pchar=strstr(line_buffer, "tx_packets:"))!=NULL)
        {
            pchar+=strlen("tx_packets:");
            ap->send_packets = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "tx_bytes:"))!=NULL)
        {
            pchar+=strlen("tx_bytes:");
            ap->send_bytes = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "tx_fails:"))!=NULL)
        {
            pchar+=strlen("tx_fails:");
            ap->err_send = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "tx_drops:"))!=NULL)
        {
            pchar+=strlen("tx_drops:");
            ap->discard_send = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "rx_packets:"))!=NULL)
        {
            pchar+=strlen("rx_packets:");
            ap->recv_packets = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "rx_bytes:"))!=NULL)
        {
            pchar+=strlen("rx_bytes:");
            ap->recv_bytes = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "rx_errors:"))!=NULL)
        {
            pchar+=strlen("rx_errors:");
            ap->err_recv = atoi(pchar);
        }
        else if((pchar=strstr(line_buffer, "rx_data_drops:"))!=NULL)
        {
            pchar+=strlen("tx_data_drops:");
            ap->discard_recv = atoi(pchar);
        }
    }
    fclose(fp);
    return 0;
    
}

int rtl_link_getLocalIp(char *ip_s)
{
    char ifname[16] = {0};
    int wisp_wan_id, opmode, wan_dhcp;
    struct in_addr ip;

    apmib_get(MIB_OP_MODE, (void *)&opmode);
	apmib_get(MIB_WAN_DHCP, (void *)&wan_dhcp);
     
    if(opmode==GATEWAY_MODE){
		if(wan_dhcp==3) //pppoe
			snprintf(ifname, sizeof(ifname), "%s", PPPOE_IF);
		else
        	snprintf(ifname, sizeof(ifname), "%s", WAN_IF);
    } else if(opmode==WISP_MODE){
        apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
        snprintf(ifname, sizeof(ifname), VXD_IF, wisp_wan_id);
    } else if(opmode == BRIDGE_MODE) {
        snprintf(ifname, sizeof(ifname), "%s", BR_IF);
    }
    
    if(!getInAddr(ifname, IP_ADDR_T, (void *)&ip)){
        DEBUG("(%s)%d get local ip failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    strcpy(ip_s, inet_ntoa(ip));	
    return 0;
}

int rtl_link_getBr0Ip(char *ip_s)
{
    char ifname[16] = {0};
    struct in_addr ip;
    
    snprintf(ifname, sizeof(ifname), "%s", BR_IF);

    if(!getInAddr(ifname, IP_ADDR_T, (void *)&ip)){
        DEBUG("(%s)%d get local ip failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    strcpy(ip_s, inet_ntoa(ip));
    return 0;
}

int rtl_link_isGatewayExist(char *gwip)
{
	char ifname[16]={0}, cmd_buf[128];
    int wisp_wan_id, opmode;
    struct in_addr ipaddr;
	int ret = 0;

    apmib_get(MIB_OP_MODE, (void *)&opmode);
     
    if(opmode==GATEWAY_MODE){
        snprintf(ifname, sizeof(ifname), "%s", WAN_IF);
    } else if(opmode==WISP_MODE){
        apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
        snprintf(ifname, sizeof(ifname), VXD_IF, wisp_wan_id);
    } else if(opmode == BRIDGE_MODE) {
        system("ifconfig eth1 0.0.0.0");
        snprintf(ifname, sizeof(ifname), "%s", BR_IF);
    }	

	if(getDefaultRoute(ifname, (void *)&ipaddr)) {
        DEBUG("(%s)%d gwip %s exist via %s\n", __FUNCTION__, __LINE__, inet_ntoa(ipaddr), ifname);
        snprintf(gwip, 16, inet_ntoa(ipaddr));	
        ret = 1;
    }

	return ret;
}

int rtl_link_getGatewayIp(char *gwip)
{
    char ifname[16]={0}, cmd_buf[128];
    int wisp_wan_id, opmode;
    struct in_addr ipaddr;

    apmib_get(MIB_OP_MODE, (void *)&opmode);
     
    if(opmode==GATEWAY_MODE){
        snprintf(ifname, sizeof(ifname), "%s", WAN_IF);
    } else if(opmode==WISP_MODE){
        apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
        snprintf(ifname, sizeof(ifname), VXD_IF, wisp_wan_id);
    } else if(opmode == BRIDGE_MODE) {
        system("ifconfig eth1 0.0.0.0");
        snprintf(ifname, sizeof(ifname), "%s", BR_IF);
    }
     
    if(getDefaultRoute(ifname, (void *)&ipaddr)) {
        DEBUG("(%s)%d gain gwip %s from %s\n", __FUNCTION__, __LINE__, inet_ntoa(ipaddr), ifname);
        snprintf(gwip, 16, inet_ntoa(ipaddr));
        snprintf(gw_ip, 16, inet_ntoa(ipaddr));
        sprintf(cmd_buf, "ping %s -c1 > /dev/null 2>&1", gw_ip);   //bridge mode, add gw info in arp table
        system(cmd_buf);
        return 0;
    } else {
        DEBUG("(%s)%d Obtaining ip\n", __FUNCTION__, __LINE__);
        return -1;
    }
    
}

int rtl_link_getGatewayMac(char *gw_mac)
{
	FILE *fp=NULL;
	char line_buffer[256]={0};
	char cur_ip[16]={0}, tmp_mac[18]={0}, gwip[16]={0};
	int ret=0;

	if(rtl_link_isGatewayExist(gwip)){
		//get gw_mac through gw_ip
	    if((fp= fopen("/proc/net/arp", "r"))==NULL){
	        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/net/arp");
	        return -1;
	    }
	    while(fgets(line_buffer, sizeof(line_buffer), fp))
	    {
	        line_buffer[strlen(line_buffer)-1]='\0';
			sscanf(line_buffer, "%s %*s %*s %s %*s %*s", cur_ip, tmp_mac);
			
	        if(!strcmp(cur_ip, gwip)) 
		    {		        
		        DEBUG("ANDLINK (%s)%d GW-MAC: %s\n", __FUNCTION__, __LINE__, tmp_mac);
				ret = 1;
		        break;
		    }
	    }
	    if(fp)
	        fclose(fp);
		 
		rtl_link_macFormat(gw_mac, tmp_mac);
	}else
		DEBUG("default gateway ip not exist\n");

	return ret;
}

int rtl_link_getLanMacAddr(char *lan_mac)
{
    unsigned char mac[6];
    char ifname[16] = {0};
    int wisp_wan_id, opmode;
    struct sockaddr hwaddr;
    
    apmib_get(MIB_OP_MODE, (void *)&opmode);
     
    if(opmode==GATEWAY_MODE){
        snprintf(ifname, sizeof(ifname), "%s", BR_IF);
    } else if(opmode == BRIDGE_MODE) {
        snprintf(ifname, sizeof(ifname), "%s", BR_IF);
    }

    if(getInAddr(ifname, HW_ADDR_T, (void *)&hwaddr)){
        memcpy(mac, (unsigned char *)hwaddr.sa_data, 6);
        snprintf(lan_mac, 18, "%02X%02X%02X%02X%02X%02X", \
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        //DEBUG("(%s)%d gain LAN MAC: %s\n", __FUNCTION__, __LINE__, lan_mac);
    } else {
        ERR_PRINT("ERROR (%s)%d get %s mac failed\n", __FUNCTION__, __LINE__, ifname);
        return -1;
    }
    return 0;
}

int rtl_link_getWlanMacAddr(char *wlan_mac, char *ifname)
{
    unsigned char mac[6];
    int wisp_wan_id, opmode;
    struct sockaddr hwaddr;

    if(getInAddr(ifname, HW_ADDR_T, (void *)&hwaddr)){
        memcpy(mac, (unsigned char *)hwaddr.sa_data, 6);
        snprintf(wlan_mac, 18, "%02X%02X%02X%02X%02X%02X", \
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        //DEBUG("(%s)%d gain %s WLAN MAC: %s\n", __FUNCTION__, __LINE__, ifname, wlan_mac);
    } else {
        ERR_PRINT("ERROR (%s)%d get %s mac failed\n", __FUNCTION__, __LINE__, ifname);
        return -1;
    }
    return 0;
}

void rtl_link_setRecoverMib(int mib, int value, int bit)
{
    int recover_map;
    apmib_get(MIB_RTL_LINK_RECOVER_MAP, (void *)&recover_map);
    if(recover_map & 0x1<<bit){
        return;
    }else{
        recover_map |= 0x1 << bit;
        apmib_set(MIB_RTL_LINK_RECOVER_MAP, (void *)&recover_map);
        apmib_set(mib, (void *)&value);
    }
}

void rtl_link_recoverSettings(void)
{
    int recover_map, tmp_value, bac_value, i;
    apmib_get(MIB_RTL_LINK_RECOVER_MAP, (void *)&recover_map);
    if(recover_map)
    {
        if(recover_map & 0x1<<OPMODE_BIT)
        {
            apmib_get(MIB_OP_MODE, (void *)&tmp_value);
            //if(tmp_value == BRIDGE_MODE)
            {
                apmib_get(MIB_RTL_LINK_OPMODE_BACKUP, (void *)&bac_value);
                apmib_set(MIB_OP_MODE, (void *)&bac_value);
            }
        }

        if(recover_map & 0x1<<DHCP_BIT)
        {
            apmib_get(MIB_DHCP, (void *)&tmp_value);
            //if(tmp_value == DHCP_LAN_CLIENT)
            {
                apmib_get(MIB_RTL_LINK_DHCP_BACKUP, (void *)&bac_value);
                apmib_set(MIB_DHCP, (void *)&bac_value);
            }
        }

#ifdef CONFIG_CMCC
        if(recover_map & 0x1<<REPEATER_MODE_BIT)
        {
            apmib_get(MIB_RTL_LINK_REPEATER_MODE_BACKUP, (void *)&bac_value);
            apmib_set(MIB_REPEATER_MODE, (void *)&bac_value);
        }
#endif

        apmib_save_wlanIdx();
        for(i=0; i<NUM_WLAN_INTERFACE; i++)
        {
            if(i == 0)
            {
                wlan_idx = 0;
                vwlan_idx = NUM_VWLAN_INTERFACE;
                if(recover_map & 0x1<<REPEATER1_BIT)
                {
                    apmib_get(MIB_REPEATER_ENABLED1, (void *)&tmp_value);
                    //if(tmp_value == 1)
                    {
                        apmib_get(MIB_RTL_LINK_REPEATER1_BACKUP, (void *)&bac_value);
                        apmib_set(MIB_REPEATER_ENABLED1, (void *)&bac_value);
                        tmp_value = !bac_value;
                        apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmp_value);
                    }
                }

                if(recover_map & 0x1<<MODE1_BIT)
                {
                    apmib_get(MIB_WLAN_MODE, (void *)&tmp_value);
                    //if(tmp_value == CLIENT_MODE)
                    {
                        apmib_get(MIB_RTL_LINK_MODE1_BACKUP, (void *)&bac_value);
                        apmib_set(MIB_WLAN_MODE, (void *)&bac_value);
                    }
                }
            }else{
                wlan_idx = 1;
                vwlan_idx = NUM_VWLAN_INTERFACE;

                if(recover_map & 0x1<<REPEATER2_BIT)
                {
                    apmib_get(MIB_REPEATER_ENABLED2, (void *)&tmp_value);
                    //if(tmp_value == 1)
                    {
                        apmib_get(MIB_RTL_LINK_REPEATER2_BACKUP, (void *)&bac_value);
                        apmib_set(MIB_REPEATER_ENABLED2, (void *)&bac_value);
                        tmp_value = !bac_value;
                        apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmp_value);
                    }
                }

                if(recover_map & 0x1<<MODE2_BIT)
                {
                    apmib_get(MIB_WLAN_MODE, (void *)&tmp_value);
                    //if(tmp_value == CLIENT_MODE)
                    {
                        apmib_get(MIB_RTL_LINK_MODE2_BACKUP, (void *)&bac_value);
                        apmib_set(MIB_WLAN_MODE, (void *)&bac_value);
                    }
                }
            }
        }
        
        apmib_recov_wlanIdx();

        recover_map = 0;
        apmib_set(MIB_RTL_LINK_RECOVER_MAP, (void *)&recover_map);
    }

}

int rtl_link_switchOpMode(RTL_LINK_MODE_T work_mode, char *addr_s, int wisp_idx)
{
    int opmode, dhcp, auto_dhcp, ret=0;
    int value, rpt_enabled = 1;
    int i, band, enable, tmpvalue;
    int enable_mib, ssid_mib;
    int enable_back_mib, mode_back_mib, enable_back_bit, mode_back_bit;
    struct in_addr addr, addr_ori;

    apmib_get(MIB_OP_MODE, (void *)&opmode);
    apmib_get(MIB_DHCP, (void *)&dhcp);
    apmib_get(MIB_RTL_LINK_AUTO_DHCP, (void *)&auto_dhcp);

    if(work_mode == RTL_LINK_BRIDGE || work_mode == RTL_LINK_REPEATER){
        if(opmode != BRIDGE_MODE)
        {
            DEBUG("(%s)%d change OP MODE to BRIDGE MODE\n", __FUNCTION__, __LINE__);
            rtl_link_setRecoverMib(MIB_RTL_LINK_OPMODE_BACKUP, opmode, OPMODE_BIT);
            
            opmode = BRIDGE_MODE;
            apmib_set(MIB_OP_MODE, (void *)&opmode);
            ret = 0x1<<30;
        }else{
            system("ifconfig eth1 0.0.0.0");    //清除路由表，否则可能造成br0无法联网
        }       
        if(addr_s == NULL || !strcmp(addr_s, "0.0.0.0")){
#ifdef CONFIG_AUTO_DHCP_CHECK        
            if(auto_dhcp){
                //DHCP SERVER for AUTO DHCP
                if(dhcp != DHCP_LAN_SERVER){
                    DEBUG("(%s)%d change DHCP MODE to DHCP SERVER for Auto DHCP\n", __FUNCTION__, __LINE__);
                    rtl_link_setRecoverMib(MIB_RTL_LINK_DHCP_BACKUP, dhcp, DHCP_BIT);
                    
                    dhcp = DHCP_LAN_SERVER;
                    apmib_set(MIB_DHCP, (void *)&dhcp);
                    ret = 0x1<<30; 
                }
            }else
#endif            
            {
                //DHCP CLIENT
                if(dhcp != DHCP_LAN_CLIENT){
                    DEBUG("(%s)%d change DHCP MODE to DHCP CLIENT\n", __FUNCTION__, __LINE__);
                    rtl_link_setRecoverMib(MIB_RTL_LINK_DHCP_BACKUP, dhcp, DHCP_BIT);
                    
                    dhcp = DHCP_LAN_CLIENT;
                    apmib_set(MIB_DHCP, (void *)&dhcp);
                    ret = 0x1<<30;
                }
            }
            
        }else{
            if(!inet_aton(addr_s, &addr)){
                ERR_PRINT("ERROR (%s)%d Invalid IP Address!\n");
                return -1;
            }
            if(dhcp != DHCP_DISABLED){
                DEBUG("(%s)%d change DHCP MODE to STATIC\n", __FUNCTION__, __LINE__);
                rtl_link_setRecoverMib(MIB_RTL_LINK_DHCP_BACKUP, dhcp, DHCP_BIT);
                
                dhcp = DHCP_DISABLED;
                apmib_set(MIB_DHCP, (void *)&dhcp);
                ret = 0x1<<30;   
            }
            apmib_get(MIB_IP_ADDR, (void *)&addr_ori);
            if(memcmp(&addr, &addr_ori, sizeof(struct in_addr))){
                DEBUG("(%s)%d change IP ADDR to %s\n", __FUNCTION__, __LINE__, addr_s);
                apmib_set(MIB_IP_ADDR, (void *)&addr);
                ret = 0x1<<30;
            }
        }
        
        apmib_save_wlanIdx();
        for(i=0; i<NUM_WLAN_INTERFACE; i++)
        {
            wlan_idx = i;
            vwlan_idx = NUM_VWLAN_INTERFACE;    //set to vxd (refer to SetWlan_idx)

            if(i == 0){
                enable_mib = MIB_REPEATER_ENABLED1;
                ssid_mib = MIB_REPEATER_SSID1;
                enable_back_mib = MIB_RTL_LINK_REPEATER1_BACKUP;
                mode_back_mib = MIB_RTL_LINK_MODE1_BACKUP;
                enable_back_bit = REPEATER1_BIT;
                mode_back_bit = MODE1_BIT;
            }else{
                enable_mib = MIB_REPEATER_ENABLED2;
                ssid_mib = MIB_REPEATER_SSID2;
                enable_back_mib = MIB_RTL_LINK_REPEATER2_BACKUP;
                mode_back_mib = MIB_RTL_LINK_MODE2_BACKUP;
                enable_back_bit = REPEATER2_BIT;
                mode_back_bit = MODE2_BIT;
            }
#if 1
            if(work_mode == RTL_LINK_REPEATER){
                //REPEATER MODE: enable vxd 
                apmib_get(enable_mib, (void *)&enable);
                if(!enable)
                {
                    DEBUG("(%s)%d enable wlan%d-vxd\n", __FUNCTION__, __LINE__, i);
                    rtl_link_setRecoverMib(enable_back_mib, enable, enable_back_bit);

                    enable = 1;
                    apmib_set(enable_mib, (void *)&enable); 
    				tmpvalue = 0;
        			apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }
                apmib_get(MIB_WLAN_MODE, (void *)&tmpvalue);
                if(tmpvalue != CLIENT_MODE)
                {
                    rtl_link_setRecoverMib(mode_back_mib, tmpvalue, mode_back_bit);
                   
                    tmpvalue = CLIENT_MODE;
                    apmib_set(MIB_WLAN_MODE, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);// this ret is right??
                }
            }else
#endif            
            if(work_mode == RTL_LINK_BRIDGE)
            {
                //BRIDGE MODE: disable vxd
                apmib_get(enable_mib, (void *)&enable);
                if(enable){
                    DEBUG("(%s)%d disable wlan%d-vxd\n", __FUNCTION__, __LINE__, i);                   
                    rtl_link_setRecoverMib(enable_back_mib, enable, enable_back_bit);
                    
                    enable = 0;
                    apmib_set(enable_mib, (void *)&enable);
                    tmpvalue = 1;
        			apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
        			ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }
            }
            
        }
        apmib_recov_wlanIdx();
        
#ifdef CONFIG_CMCC
        if(work_mode == RTL_LINK_REPEATER){
            apmib_get(MIB_REPEATER_MODE, (void *)&rpt_enabled);
            if(rpt_enabled != 1){
                DEBUG("(%s)%d change Repeater Mode enable for CMCC\n", __FUNCTION__, __LINE__);
                rtl_link_setRecoverMib(MIB_RTL_LINK_REPEATER_MODE_BACKUP, rpt_enabled, REPEATER_MODE_BIT);
                rpt_enabled = 1;
                apmib_set(MIB_REPEATER_MODE, (void *)&rpt_enabled);
            }
        }else{
            apmib_get(MIB_REPEATER_MODE, (void *)&rpt_enabled);
            if(rpt_enabled != 0){
                DEBUG("(%s)%d change Repeater Mode disable for CMCC\n", __FUNCTION__, __LINE__);
                rtl_link_setRecoverMib(MIB_RTL_LINK_REPEATER_MODE_BACKUP, rpt_enabled, REPEATER_MODE_BIT);
                rpt_enabled = 0;
                apmib_set(MIB_REPEATER_MODE, (void *)&rpt_enabled);
            }
        }
#endif

    }else if(work_mode == RTL_LINK_GW){
        if(opmode != GATEWAY_MODE)
        {
            DEBUG("(%s)%d change OP MODE to GATEWAY MODE\n", __FUNCTION__, __LINE__);
            rtl_link_setRecoverMib(MIB_RTL_LINK_OPMODE_BACKUP, opmode, OPMODE_BIT);
            
            opmode = GATEWAY_MODE;
            apmib_set(MIB_OP_MODE, (void *)&opmode);
            ret = 0x1<<30;
        }
        if(dhcp != DHCP_LAN_SERVER)
        {
            DEBUG("(%s)%d change DHCP MODE to DHCP SERVER\n", __FUNCTION__, __LINE__);
            rtl_link_setRecoverMib(MIB_RTL_LINK_DHCP_BACKUP, dhcp, DHCP_BIT);
            
            dhcp = DHCP_LAN_SERVER;
            apmib_set(MIB_DHCP, (void *)&dhcp);
            ret = 0x1<<30;
        }
    }else if(work_mode == RTL_LINK_WISP){
        if(wisp_idx != -1)
        {
            apmib_save_wlanIdx();
            if(opmode != WISP_MODE){
                DEBUG("(%s)%d change OP MODE to WISP MODE\n", __FUNCTION__, __LINE__);
                rtl_link_setRecoverMib(MIB_RTL_LINK_OPMODE_BACKUP, opmode, OPMODE_BIT);
                
                opmode = WISP_MODE;
                apmib_set(MIB_OP_MODE, (void *)&opmode);
                ret = 0x1<<30;
            }
            apmib_get(MIB_WISP_WAN_ID, (void *) &value);
            if(value != wisp_idx){
                DEBUG("(%s)%d change wisp_idx to %d\n", __FUNCTION__, __LINE__, wisp_idx);
                apmib_set(MIB_WISP_WAN_ID, (void *)&wisp_idx);
                ret = 0x1<<30;
            }
            
			if(wisp_idx == 0)
			{
				apmib_get( MIB_REPEATER_ENABLED2, (void *)&rpt_enabled);
				if(rpt_enabled){
				    DEBUG("(%s)%d disable wlan1-vxd\n", __FUNCTION__, __LINE__);
				    ret = 0x1<<30;
				}
				rpt_enabled = 0;
				apmib_set(MIB_REPEATER_ENABLED2,(void *)&rpt_enabled);
			}
			else
			{
				apmib_get( MIB_REPEATER_ENABLED1, (void *)&rpt_enabled);
				if(rpt_enabled){
				    DEBUG("(%s)%d disable wlan0-vxd\n", __FUNCTION__, __LINE__);
				    ret = 0x1<<30;
				}
				rpt_enabled = 0;
				apmib_set(MIB_REPEATER_ENABLED1,(void *)&rpt_enabled);
			}
			apmib_recov_wlanIdx();
        }
    }
    
    return ret;
}

void rtl_link_restart(int ret)
{
    pid_t pid;
    char tmpbuf[64], fwdir[256];
    FILE *fp=NULL;
	char gwip[16] = {0};
	unsigned int rtl_link_mode = 0;
    
	if(ret&0x1<<31){
		ERR_PRINT("(%s)%d Reboot!!!\n", __FUNCTION__, __LINE__);
		if(isFileExist(RTL_LINK_FW_DIR)){
		    fp = fopen(RTL_LINK_FW_DIR, "r");
		    if(!fp){
		        ERR_PRINT("ERROR (%s)%d fopen error\n", __FUNCTION__, __LINE__);
		        return;
		    }
		    fgets(fwdir, sizeof(fwdir), fp);
		    DEBUG("(%s)%d Doing fw upgrade...\n", __FUNCTION__, __LINE__);
		    rtl_link_do_upgrade(fwdir); 
		}
#ifndef CONFIG_APP_FWD
        sleep(2);
#endif
        system("reboot");
	}else{
#if defined(RTK_REINIT_SUPPORT)
#ifdef REDUCE_WEBCLIENT_WAITTIME_SUPPORT
        sprintf(tmpbuf,"reinitCli -e %d&",REINIT_EVENT_APPLYCAHNGES);
#else
        sprintf(tmpbuf,"reinitCli -e %d",REINIT_EVENT_APPLYCAHNGES);
#endif
        if(ret&0x1<<30){
            apmib_update(CURRENT_SETTING);
            ERR_PRINT("(%s)%d Restart AP!!!\n", __FUNCTION__, __LINE__);
            unlink(RTL_LINK_PID_FILE);
            system(tmpbuf);
            exit(0);
        }else if(ret){
            fp = fopen(RTL_LINK_RESTART_BLOCK, "w+");
        	if(!fp)
        		return;
        	apmib_update(CURRENT_SETTING);
            pid = fork();
            if(pid == 0){
                ERR_PRINT("(%s)%d sync config to GATEWAY restart wifi only!!!\n", __FUNCTION__, __LINE__);  
                system(tmpbuf);
                unlink(RTL_LINK_RESTART_BLOCK);
                exit(0);
            }
        }
#else
        if(ret&0x1<<30){
            apmib_update(CURRENT_SETTING);
            ERR_PRINT("(%s)%d Restart AP!!!\n", __FUNCTION__, __LINE__);
        	unlink(RTL_LINK_PID_FILE);
    		system("sysconf init gw all");
        	exit(0);
        }else if(ret){
            fp = fopen(RTL_LINK_RESTART_BLOCK, "w+");
        	if(!fp)
        		return;
        	apmib_update(CURRENT_SETTING);
			
			/*clear gw route info avoid ping block when andlink repeater */
			apmib_get(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
			if((rtl_link_mode==RTL_LINK_REPEATER) && (isFileExist(RTL_LINK_DEL_ROUTE)) && (rtl_link_isGatewayExist(gwip))){
				snprintf(tmpbuf, sizeof(tmpbuf), "route del default gw %s", gwip);	
				system(tmpbuf);
				unlink(RTL_LINK_DEL_ROUTE);
			}
			
            pid = fork();
            if(pid == 0){
                ERR_PRINT("(%s)%d sync config to GATEWAY restart wifi only!!!\n", __FUNCTION__, __LINE__);    
                snprintf(tmpbuf, sizeof(tmpbuf), "sysconf rtl_link restart %d", ret);
                system(tmpbuf);
				if(isFileExist(RTL_LINK_RESTART_BLOCK))
	                unlink(RTL_LINK_RESTART_BLOCK);
                exit(0);
            }
        }
#endif
    }
    if(fp)
        fclose(fp);
}

int rtl_link_getLoginInfo(LoginInfo *login)
{
    int idx;
#ifdef CONFIG_ELINK_SUPPORT
        apmib_get(MIB_RTL_LINK_VENDOR, (void *)login->vendor);
        apmib_get(MIB_RTL_LINK_DEVMODEL, (void *)login->model);
        apmib_get(MIB_RTL_LINK_HWVERSION, (void *)login->hwversion);
        apmib_get(MIB_RTL_LINK_SWVERSION, (void *)login->swversion);
        apmib_get(MIB_RTL_LINK_URL, (void *)login->url);
        apmib_get(MIB_RTL_LINK_WIRELESS, (void *)&login->wireless);
#endif
#ifdef CONFIG_ANDLINK_SUPPORT
    int value;

    if(rtl_link_getLocalIp(login->localIp))
    {
        DEBUG("(%s)%d get local ip failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    
    if(NUM_WLAN_INTERFACE == 1){
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&value);
        if(value == PHYBAND_2G)
            login->band = BAND2G;
        else
            login->band = BAND5G;
    }else{
        login->band = DUALBAND;
    }
    login->uptype = rtl_link_getUplinkType(&idx);
#endif
    return 0;
}

int rtl_link_check_pwd(char *key, int encrypt, int wep_type)
{
	int ret = 1, is_hex_key=0, key_len=0;

	if(key==NULL){
		DEBUG("(%s)%d key NULL\n", __FUNCTION__, __LINE__);
	}

	key_len = strlen(key);	
	if( encrypt==ENCRYPT_WEP ){
		if(wep_type==WEP64){
			if((key_len!=WEP64_KEY_LEN) && (key_len!=WEP64_KEY_LEN*2)){
				ret = 0;
				goto exit;
			}
			if(key_len==WEP64_KEY_LEN*2){
				is_hex_key = 1;				
			}			
		}else if(wep_type==WEP128){
			if((key_len!=WEP128_KEY_LEN) && (key_len!=WEP128_KEY_LEN*2)){
				ret = 0;
				goto exit;
			}
			if(key_len==WEP128_KEY_LEN*2){
				is_hex_key = 1;
			}	
		}		
	}else if( (encrypt==ENCRYPT_WPA) || (encrypt==ENCRYPT_WPA2) || (encrypt==ENCRYPT_WPA2_MIXED) ){
		if(key_len<MIN_PSK_LEN || key_len>MAX_PSK_LEN){
			ret = 0;
			goto exit;
		}
		if(key_len==MAX_PSK_LEN){
			is_hex_key = 1;
		}
	}else if(encrypt==ENCRYPT_DISABLED){

	}else{
		 DEBUG("(%s)%d not supported encrypt [%d]\n", __FUNCTION__, __LINE__, encrypt);
		 ret = 0;
	}

	if(is_hex_key && !is_hex_string(key, key_len))
		ret = 0;
exit:
	return ret;
}

static int rtl_link_setOpenEncrypt(APConfig ap)
{
    int value, tmpvalue, ret=0;
#ifdef WIFI_SIMPLE_CONFIG 
    int wsc_auth, wsc_encrypt;
#endif

    value = ENCRYPT_DISABLED;
    apmib_get(MIB_WLAN_ENCRYPT, (void *)&tmpvalue);
    if(value != tmpvalue){
        DEBUG("(%s)%d change encrypt method to disabled\n", __FUNCTION__, __LINE__); 
        apmib_set(MIB_WLAN_ENCRYPT, (void *)&value);
#ifdef WIFI_SIMPLE_CONFIG        
        wsc_auth = WSC_AUTH_OPEN;
        wsc_encrypt = WSC_ENCRYPT_NONE;
        apmib_set(MIB_WLAN_WSC_AUTH, (void *)&wsc_auth);
        apmib_set(MIB_WLAN_WSC_ENC, (void *)&wsc_encrypt);
#endif
        ret = 1;
    }
    return ret;
}

static int rtl_link_setWepEncrypt(APConfig ap)
{
    int value, tmpvalue, ret=0;
    char tmpbuf[128] = {0};
    int def_wep_auth_type = AUTH_BOTH;
    unsigned char wep_hex_key[32] = {0};

    value = ENCRYPT_WEP;
    apmib_get(MIB_WLAN_ENCRYPT, (void *)&tmpvalue);
    if(value != tmpvalue){
        DEBUG("(%s)%d change encrypt method to wep\n", __FUNCTION__, __LINE__); 
        apmib_set(MIB_WLAN_ENCRYPT, (void *)&value);
        ret = 1;
    }
    apmib_get(MIB_WLAN_AUTH_TYPE, (void *)&tmpvalue);
    if(tmpvalue != def_wep_auth_type){
        DEBUG("(%s)%d change wep_auth_type to AUTH_BOTH by default\n", __FUNCTION__, __LINE__);
        apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&def_wep_auth_type);
        ret = 1;
    }

    if(strlen(ap.key) == WEP128_KEY_LEN*2){
        value = WEP128;
        apmib_get(MIB_WLAN_WEP, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_type to WEP128\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP, (void *)&value);
            ret = 1;
        }
        value = KEY_HEX;
        apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_key_type to HEX\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&value);
            ret = 1;
        }
        _string_to_hex(ap.key, wep_hex_key, WEP128_KEY_LEN*2);
        apmib_get(MIB_WLAN_WEP128_KEY1, (void *)tmpbuf);
        if(memcmp(wep_hex_key, tmpbuf, WEP128_KEY_LEN)){
            DEBUG("(%s)%d change wep key\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP128_KEY1, (void *)wep_hex_key);
            ret = 1;
        }
    }else if(strlen(ap.key) == WEP128_KEY_LEN){
        value = WEP128;
        apmib_get(MIB_WLAN_WEP, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_type to WEP128\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP, (void *)&value);
            ret = 1;
        }
        value = KEY_ASCII;
        apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_key_type to ASCII\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&value);
            ret = 1;
        }
        apmib_get(MIB_WLAN_WEP128_KEY1, (void *)tmpbuf);
        if(strcmp(ap.key, tmpbuf)){
            DEBUG("(%s)%d sync wep key\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP128_KEY1, (void *)ap.key);
            ret = 1;
        }
    }else if(strlen(ap.key) == WEP64_KEY_LEN*2){
        value = WEP64;
        apmib_get(MIB_WLAN_WEP, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_type to WEP64\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP, (void *)&value);
            ret = 1;
        }
        value = KEY_HEX;
        apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_key_type to HEX\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&value);
            ret = 1;
        }
        _string_to_hex(ap.key, wep_hex_key, WEP64_KEY_LEN*2);
        apmib_get(MIB_WLAN_WEP64_KEY1, (void *)tmpbuf);
        if(memcmp(wep_hex_key, tmpbuf, WEP64_KEY_LEN)){
            DEBUG("(%s)%d change wep key\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP64_KEY1, (void *)wep_hex_key);
            ret = 1;
        }
    }else {
        value = WEP64;
        apmib_get(MIB_WLAN_WEP, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_type to WEP64\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP, (void *)&value);
            ret = 1;
        }
        value = KEY_ASCII;
        apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&tmpvalue);
        if(value != tmpvalue){
            DEBUG("(%s)%d change wep_key_type to ASCII\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&value);
            ret = 1;
        }
        apmib_get(MIB_WLAN_WEP64_KEY1, (void *)tmpbuf);
        if(strcmp(ap.key, tmpbuf)){
            DEBUG("(%s)%d change wep key\n", __FUNCTION__, __LINE__);
            apmib_set(MIB_WLAN_WEP64_KEY1, (void *)ap.key);
            ret = 1;
        }
    }
    return ret;
}

static int rtl_link_setWpaEncrypt(APConfig ap)
{
    int tmpvalue, ret=0;
    char tmpbuf[128] = {0};

    //sync wpa encrypt method
    apmib_get(MIB_WLAN_ENCRYPT, (void *)&tmpvalue);
    if(ap.encrypt != tmpvalue){
        DEBUG("(%s)%d change encrypt method to wpa[%d]\n", __FUNCTION__, __LINE__, ap.encrypt); 
        apmib_set(MIB_WLAN_ENCRYPT, (void *)&ap.encrypt);
        ret = 1;
    }
#ifdef WIFI_SIMPLE_CONFIG
    int wsc_auth, wsc_encrypt;
    if(ap.encrypt == ENCRYPT_WPA)
        wsc_auth = WSC_AUTH_WPAPSK;
    else if(ap.encrypt  == ENCRYPT_WPA2)
        wsc_auth = WSC_AUTH_WPA2PSK;    
    else
        wsc_auth = WSC_AUTH_WPA2PSKMIXED;
    apmib_get(MIB_WLAN_WSC_AUTH, (void *)&tmpvalue);
    if(tmpvalue != wsc_auth){
        DEBUG("(%s)%d change wsc encrypt method to %d\n", __FUNCTION__, __LINE__, wsc_auth); 
        apmib_set(MIB_WLAN_WSC_AUTH, (void *)&wsc_auth);
        ret = 1;
    }
#endif        

    //sync auth type by default
    apmib_get(MIB_WLAN_WPA_AUTH, (void *)&tmpvalue);
    if(tmpvalue != ap.wpa_auth){
        DEBUG("(%s)%d change wpa_auth_type to psk\n", __FUNCTION__, __LINE__); 
        apmib_set(MIB_WLAN_WPA_AUTH, (void *)&ap.wpa_auth);
        ret = 1;
    }
    //sync key format by default
    apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&tmpvalue);
    if(tmpvalue != ap.wpa_key_fmt){
		if(ap.wpa_key_fmt==KEY_ASCII)
        	DEBUG("(%s)%d change wpa_psk_format to passphrase\n", __FUNCTION__, __LINE__);
		else if(ap.wpa_key_fmt==KEY_HEX)
			DEBUG("(%s)%d change wpa_psk_format to hex\n", __FUNCTION__, __LINE__);
        apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&ap.wpa_key_fmt);
        ret = 1;
    }
    //sync wap cypher
    if(ap.encrypt == ENCRYPT_WPA){
        apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&tmpvalue);
        if(ap.wpa_cypher != tmpvalue){
            DEBUG("(%s)%d change wpa cipher_suite to %d\n", __FUNCTION__, __LINE__, ap.wpa_cypher);
            apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&ap.wpa_cypher);            
            ret = 1;
        }
    }
    else if(ap.encrypt  == ENCRYPT_WPA2){
        apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&tmpvalue);
        if(ap.wpa_cypher != tmpvalue){
            DEBUG("(%s)%d change wpa cipher_suite to %d\n", __FUNCTION__, __LINE__, ap.wpa_cypher);
            apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&ap.wpa_cypher);           
            ret = 1;
        }
    }
    else{
        apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&tmpvalue);
        if(ap.wpa_cypher != tmpvalue){
            DEBUG("(%s)%d change wpa cipher_suite to %d\n", __FUNCTION__, __LINE__, ap.wpa_cypher);
            apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&ap.wpa_cypher);            
            ret = 1;
        }
        apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&tmpvalue);
        if(ap.wpa_cypher != tmpvalue){
            DEBUG("(%s)%d change wpa cipher_suite to %d\n", __FUNCTION__, __LINE__, ap.wpa_cypher);
            apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&ap.wpa_cypher);               
            ret = 1;
        }
    }
    
#ifdef WIFI_SIMPLE_CONFIG
    if(ap.wpa_cypher == WPA_CIPHER_TKIP)
        wsc_encrypt = WSC_ENCRYPT_TKIP;
    else if(ap.wpa_cypher == WPA_CIPHER_AES)
        wsc_encrypt = WSC_ENCRYPT_AES;    
    else
        wsc_encrypt = WSC_ENCRYPT_TKIPAES;

    apmib_get(MIB_WLAN_WSC_ENC, (void *)&tmpvalue);
    if(tmpvalue != wsc_encrypt){
        DEBUG("(%s)%d change wsc cipher_suite to %d\n", __FUNCTION__, __LINE__, wsc_encrypt);
        apmib_set(MIB_WLAN_WSC_ENC, (void *)&wsc_encrypt);
        ret = 1;
    }
#endif 

    //sync wpa key
    apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpbuf);
    if(strcmp(ap.key, tmpbuf)){
        DEBUG("(%s)%d change wpakey\n", __FUNCTION__, __LINE__);
        apmib_set(MIB_WLAN_WPA_PSK, (void *)ap.key);
        ret = 1;
    }
    
#ifdef WIFI_SIMPLE_CONFIG
    apmib_get(MIB_WLAN_WSC_PSK, (void *)tmpbuf);
    if(strcmp(ap.key, tmpbuf)){
        DEBUG("(%s)%d change wsc key\n", __FUNCTION__, __LINE__);
        apmib_set(MIB_WLAN_WSC_PSK, (void *)ap.key);
        ret = 1;
    }
#endif 

    return ret;
}

int rtl_link_syncRepeaterConfig(WiFiConfig wifi)
{
    int i, j, ret=0, tmpvalue, band, enable, disable, repeater_enable=1,opmode=1; 
    int enable_mib, ssid_mib;
    int enable_back_mib, mode_back_mib, enable_back_bit, mode_back_bit;
    RTL_LINK_MODE_T rtl_link_mode;

    char tmpbuf[128], peer_mac[6], empty_mac[6]={0};

    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;

        if(i == 0){
            enable_mib = MIB_REPEATER_ENABLED1;
            ssid_mib = MIB_REPEATER_SSID1;
            enable_back_mib = MIB_RTL_LINK_REPEATER1_BACKUP;
            mode_back_mib = MIB_RTL_LINK_MODE1_BACKUP;
            enable_back_bit = REPEATER1_BIT;
            mode_back_bit = MODE1_BIT;
            
        }else{
            enable_mib = MIB_REPEATER_ENABLED2;
            ssid_mib = MIB_REPEATER_SSID2;
            enable_back_mib = MIB_RTL_LINK_REPEATER2_BACKUP;
            mode_back_mib = MIB_RTL_LINK_MODE2_BACKUP;
            enable_back_bit = REPEATER2_BIT;
            mode_back_bit = MODE2_BIT;
            
        }

        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);

        apmib_get(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);

        for(j=0; j<wifi.num; j++)
        {
            //if(wifi.ap[j].band == band && band == PHYBAND_2G && wifi.ap[j].idx == 0) /*only sync repeater with root ap on band 2.4g*/
            if(wifi.ap[j].band == band && band == PHYBAND_2G && wifi.ap[j].idx == 0 && rtl_link_mode == RTL_LINK_REPEATER) /*only sync repeater with root ap on band 2.4g*/
            {
                vwlan_idx = NUM_VWLAN_INTERFACE;    //set to vxd (refer to SetWlan_idx)
                
                DEBUG("(%s)%d syncing wlan%d-vxd configuration\n", __FUNCTION__, __LINE__, i);

                //sync enable status
                apmib_get(enable_mib, (void *)&enable);
                if(enable != wifi.ap[j].enable){
                    DEBUG("(%s)%d change enable status\n", __FUNCTION__, __LINE__);
                    enable = wifi.ap[j].enable;
                    apmib_set(enable_mib, (void *)&enable);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disable);
                if(disable != !wifi.ap[j].enable)
                {
                    disable = !wifi.ap[j].enable;
                    DEBUG("(%s)%d change enable status\n", __FUNCTION__, __LINE__); 
                    apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disable);
                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                //sync mode
                apmib_get(MIB_WLAN_MODE, (void *)&tmpvalue);
                if(tmpvalue != CLIENT_MODE)
                {
                    rtl_link_setRecoverMib(mode_back_mib, tmpvalue, mode_back_bit);
                    DEBUG("(%s)%d change mode to client\n", __FUNCTION__, __LINE__);
                    tmpvalue = CLIENT_MODE;
                    apmib_set(MIB_WLAN_MODE, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                memset(tmpbuf, 0, sizeof(tmpbuf));
                //sync ssid
                apmib_get(ssid_mib, (void *)tmpbuf);
                if(strcmp(tmpbuf, wifi.ap[j].ssid))
                {
                    DEBUG("(%s)%d change ssid to %s\n", __FUNCTION__, __LINE__, wifi.ap[j].ssid);
                    tmpvalue = 1;
                    apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&tmpvalue);
                    //need to set these three mibs refer to "setRepeaterSsid"
                    apmib_set(ssid_mib, (void *)wifi.ap[j].ssid);
                    apmib_set(MIB_WLAN_WSC_SSID, (void *)wifi.ap[j].ssid);
                    apmib_set(MIB_WLAN_SSID, (void *)wifi.ap[j].ssid);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }
                //sync encryption
                if(wifi.ap[j].encrypt == ENCRYPT_DISABLED)
                    ret |= rtl_link_setOpenEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                else if(wifi.ap[j].encrypt == ENCRYPT_WEP)
                    ret |= rtl_link_setWepEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                else if(wifi.ap[j].encrypt >= ENCRYPT_WPA)
                    ret |= rtl_link_setWpaEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);

				apmib_update(CURRENT_SETTING);
            }
        }
    }
    
    apmib_recov_wlanIdx();

    return ret;
}

int rtl_link_syncSearchAPWiFiConfig(WiFiConfig wifi)
{
    int i, j, ret=0;
    int band, tmpvalue, flag=0, is_configured, reset_flag;
    char tmpbuf[128] = {0};

    apmib_save_wlanIdx();
	
#if 0
    if(wifi.synccode[0])
        apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)wifi.synccode);
#endif 

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
        DEBUG("(%s)%d syncing WiFi-%s configuration\n", __FUNCTION__, __LINE__, (band-1)?"5G":"2.4G");

        //set wsc to configured
        apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&tmpvalue);
        if(tmpvalue){
            tmpvalue = 0;
            DEBUG("(%s)%d enable WiFi[%d] wps\n", __FUNCTION__, __LINE__, i);
            apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&tmpvalue);
            ret |= 0x1 << (i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
        }

        #if 0
        //set led type
        apmib_get(MIB_HW_LED_TYPE, (void *)&tmpvalue);
        if((tmpvalue < LEDTYPE_SW_LINK_TXRX) || (tmpvalue >= LEDTYPE_SW_MAX)){
            tmpvalue = LEDTYPE_SW_LINK_TXRX; //by default
            DEBUG("(%s)%d change WiFiled[%d] type\n", __FUNCTION__, __LINE__, i);
            snprintf(tmpbuf, sizeof(tmpbuf), "flash set wlan%d HW_LED_TYPE %d", wlan_idx, tmpvalue);
            system(tmpbuf);
            ret |= 0x1 << (i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
        }
        #endif

        flag = 0;

        for(j=0; j<wifi.num; j++)
        {

            if(band == wifi.ap[j].band)
            {
                flag = 1;
                //disable 2.4G channel-bonding before sync channel
                if(wifi.ap[j].band == PHYBAND_2G)
                {
                    apmib_get(MIB_WLAN_CHANNEL_BONDING, (void *)&tmpvalue);
                    if(tmpvalue)
                    {
                        DEBUG("(%s)%d disable WiFi-2.4G channel bonding\n", __FUNCTION__, __LINE__);
                        tmpvalue = 0;
                        apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&tmpvalue);
                        ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                    }
                }

                //sync channel
                apmib_get(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
                if(tmpvalue != wifi.ap[j].channel && (wifi.ap[j].channel!=0))
                {
                    DEBUG("(%s)%d change channel from %d to %d\n", __FUNCTION__, __LINE__, tmpvalue, wifi.ap[j].channel); 
                    tmpvalue = wifi.ap[j].channel;
                    apmib_set(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                //sync txpower
#if 0
                if(wifi.ap[j].txpower == 100)
                    wifi.ap[j].txpower = 0;
                apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
                if(tmpvalue != wifi.ap[j].txpower)
                {
                    DEBUG("(%s)%d change txpower\n", __FUNCTION__, __LINE__, tmpvalue, wifi.ap[j].channel); 
                    tmpvalue = wifi.ap[j].txpower;
                    apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
                    ret |= 1<<i;
                }
#endif
                //set vwlan index
                if(wifi.ap[j].idx > NUM_VWLAN)
                {
                    ERR_PRINT("(%s)%d invalid virtual wlan interface index number\n", __FUNCTION__, __LINE__);
                    continue;
                }
                else
                    vwlan_idx = wifi.ap[j].idx;

                if(wifi.ap[j].idx == 0)
                    DEBUG("(%s)%d syncing rootap configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx);
                else
                    DEBUG("(%s)%d syncing vap%d configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx);

#if 0
                //sync enable status
                if(wifi.ap[j].enable != -1){
	                apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
	                if(tmpvalue == wifi.ap[j].enable)
	                {
	                    tmpvalue = !tmpvalue;
	                    DEBUG("(%s)%d change enable status\n", __FUNCTION__, __LINE__); 
	                    apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
	                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                }
                }
#endif
#ifdef CONFIG_ELINK_SUPPORT
				if(wifi.ap[j].disable_cfg != 1){
#endif
	                //sync ssid
	                memset(tmpbuf, 0, sizeof(tmpbuf));
	                apmib_get(MIB_WLAN_SSID, (void *)tmpbuf);
	                if(strcmp(tmpbuf, wifi.ap[j].ssid))
	                {
	                    DEBUG("(%s)%d change ssid to %s\n", __FUNCTION__, __LINE__, wifi.ap[j].ssid); 
	                    apmib_set(MIB_WLAN_SSID, (void *)wifi.ap[j].ssid);
                        apmib_set(MIB_WLAN_WSC_SSID, (void *)wifi.ap[j].ssid);
	                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                }

	                //sync key related stuff
	                if(wifi.ap[j].encrypt == ENCRYPT_DISABLED)
	                    ret |= rtl_link_setOpenEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                else if(wifi.ap[j].encrypt == ENCRYPT_WEP)
	                    ret |= rtl_link_setWepEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                else if(wifi.ap[j].encrypt >= ENCRYPT_WPA)
	                    ret |= rtl_link_setWpaEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
#ifdef CONFIG_ELINK_SUPPORT
				}
#endif

#if 0 //def CONFIG_ANDLINK_SUPPORT
                //sync max sta num
                apmib_get(MIB_WLAN_STA_NUM, (void *)&tmpvalue);
                if(wifi.ap[j].max_sta_num > NUM_STAT)
                {
                   ERR_PRINT("ERROR (%s)%d max sta num is more than %d\n", __FUNCTION__, __LINE__, NUM_STAT); 
                   wifi.ap[j].max_sta_num = NUM_STAT;
                }
                if(tmpvalue != wifi.ap[j].max_sta_num)
                {
                    DEBUG("(%s)%d change max sta num to %d\n", __FUNCTION__, __LINE__, wifi.ap[j].max_sta_num); 
                    tmpvalue = wifi.ap[j].max_sta_num;
                    apmib_set(MIB_WLAN_STA_NUM, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                //sync broadcast ssid
                apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&tmpvalue);
                tmpvalue = !tmpvalue;
                if(tmpvalue != wifi.ap[j].broadcast_ssid)
                {
                    DEBUG("(%s)%d change SSIDAdvertisement %s\n", __FUNCTION__, __LINE__, tmpvalue?"disabled":"enabled"); 
                    apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }
#endif
                //set WSC configured
                if(vwlan_idx == 0){
                    reset_flag = 0;
                    apmib_set(MIB_WLAN_WSC_CONFIGBYEXTREG, (void *)&reset_flag);
                    apmib_get(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
                    if(!is_configured){
                        is_configured = 1;
                        DEBUG("(%s)%d change WSC CONFIGURED\n", __FUNCTION__, __LINE__); 
                        apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
                        ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                    }
                }
            }
        }

        if(!flag){
            ERR_PRINT("(%s)%d DUT donot support WiFi-%s\n", __FUNCTION__, __LINE__, (band-1)?"5G":"2.4G");
        }
    }

    apmib_recov_wlanIdx();
    return ret;
}

int rtl_link_syncWiFiConfig(WiFiConfig wifi)
{
    int i, j, ret=0;
    int band, tmpvalue, flag=0, is_configured, reset_flag;
    char tmpbuf[128] = {0};

    apmib_save_wlanIdx();

	sscanf(wifi.synccode,"%d",&tmpvalue);
    if(wifi.synccode[0] && tmpvalue!=-1)
        apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)wifi.synccode);
	    
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
        DEBUG("(%s)%d syncing WiFi-%s configuration\n", __FUNCTION__, __LINE__, (band-1)?"5G":"2.4G");

        //set wsc to configured
        apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&tmpvalue);
        if(tmpvalue){
            tmpvalue = 0;
            DEBUG("(%s)%d enable WiFi[%d] wps\n", __FUNCTION__, __LINE__, i);
            apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&tmpvalue);
            ret |= 0x1 << (i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
        }

        #if 0
        //set led type
        apmib_get(MIB_HW_LED_TYPE, (void *)&tmpvalue);
        if((tmpvalue < LEDTYPE_SW_LINK_TXRX) || (tmpvalue >= LEDTYPE_SW_MAX)){
            tmpvalue = LEDTYPE_SW_LINK_TXRX; //by default
            DEBUG("(%s)%d change WiFiled[%d] type\n", __FUNCTION__, __LINE__, i);
            snprintf(tmpbuf, sizeof(tmpbuf), "flash set wlan%d HW_LED_TYPE %d", wlan_idx, tmpvalue);
            system(tmpbuf);
            ret |= 0x1 << (i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
        }
        #endif

        flag = 0;

        for(j=0; j<wifi.num; j++)
        {

            if(band == wifi.ap[j].band)
            {
                flag = 1;
                //disable 2.4G channel-bonding before sync channel
                if(wifi.ap[j].band == PHYBAND_2G)
                {
                    apmib_get(MIB_WLAN_CHANNEL_BONDING, (void *)&tmpvalue);
                    if(tmpvalue)
                    {
                        DEBUG("(%s)%d disable WiFi-2.4G channel bonding\n", __FUNCTION__, __LINE__);
                        tmpvalue = 0;
                        apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&tmpvalue);
                        ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                    }
                }

                //sync channel
                apmib_get(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
                if((tmpvalue != wifi.ap[j].channel) && (wifi.ap[j].channel!=0))
                {
                    DEBUG("(%s)%d change channel from %d to %d\n", __FUNCTION__, __LINE__, tmpvalue, wifi.ap[j].channel); 
                    tmpvalue = wifi.ap[j].channel;
                    apmib_set(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                //sync txpower
#if 0
                if(wifi.ap[j].txpower == 100)
                    wifi.ap[j].txpower = 0;
                apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
                if(tmpvalue != wifi.ap[j].txpower)
                {
                    DEBUG("(%s)%d change txpower\n", __FUNCTION__, __LINE__, tmpvalue, wifi.ap[j].channel); 
                    tmpvalue = wifi.ap[j].txpower;
                    apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
                    ret |= 1<<i;
                }
#endif
                //set vwlan index
                if(wifi.ap[j].idx > NUM_VWLAN)
                {
                    ERR_PRINT("(%s)%d invalid virtual wlan interface index number\n", __FUNCTION__, __LINE__);
                    continue;
                }
                else
                    vwlan_idx = wifi.ap[j].idx;

                if(wifi.ap[j].idx == 0)
                    DEBUG("(%s)%d syncing rootap configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx);
                else
                    DEBUG("(%s)%d syncing vap%d configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx);

                //sync enable status
                if(wifi.ap[j].enable != -1){
	                apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
	                if(tmpvalue == wifi.ap[j].enable)
	                {
	                    tmpvalue = !tmpvalue;
	                    DEBUG("(%s)%d change enable status\n", __FUNCTION__, __LINE__); 
	                    apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
	                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                }
                }
#ifdef CONFIG_ELINK_SUPPORT
				if(wifi.ap[j].disable_cfg != 1){
#endif
	                //sync ssid
	                memset(tmpbuf, 0, sizeof(tmpbuf));
	                apmib_get(MIB_WLAN_SSID, (void *)tmpbuf);
	                if(strcmp(tmpbuf, wifi.ap[j].ssid))
	                {
	                    DEBUG("(%s)%d change ssid to %s\n", __FUNCTION__, __LINE__, wifi.ap[j].ssid); 
	                    apmib_set(MIB_WLAN_SSID, (void *)wifi.ap[j].ssid);
                        apmib_set(MIB_WLAN_WSC_SSID, (void *)wifi.ap[j].ssid);
	                    ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                }

	                //sync key related stuff
	                if(wifi.ap[j].encrypt == ENCRYPT_DISABLED)
	                    ret |= rtl_link_setOpenEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                else if(wifi.ap[j].encrypt == ENCRYPT_WEP)
	                    ret |= rtl_link_setWepEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
	                else if(wifi.ap[j].encrypt >= ENCRYPT_WPA)
	                    ret |= rtl_link_setWpaEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
#ifdef CONFIG_ELINK_SUPPORT
				}
#endif

#ifdef CONFIG_ANDLINK_SUPPORT
                //sync max sta num
                apmib_get(MIB_WLAN_STA_NUM, (void *)&tmpvalue);
                if(wifi.ap[j].max_sta_num > NUM_STAT)
                {
                   ERR_PRINT("ERROR (%s)%d max sta num is more than %d\n", __FUNCTION__, __LINE__, NUM_STAT); 
                   wifi.ap[j].max_sta_num = NUM_STAT;
                }
                if(tmpvalue != wifi.ap[j].max_sta_num)
                {
                    DEBUG("(%s)%d change max sta num to %d\n", __FUNCTION__, __LINE__, wifi.ap[j].max_sta_num); 
                    tmpvalue = wifi.ap[j].max_sta_num;
                    apmib_set(MIB_WLAN_STA_NUM, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }

                //sync broadcast ssid
                apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&tmpvalue);
                tmpvalue = !tmpvalue;
                if(tmpvalue != wifi.ap[j].broadcast_ssid)
                {
                    DEBUG("(%s)%d change SSIDAdvertisement %s\n", __FUNCTION__, __LINE__, tmpvalue?"disabled":"enabled"); 
                    apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&tmpvalue);
                    ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                }
#endif
                //set WSC configured
                if(vwlan_idx == 0){
                    reset_flag = 0;
                    apmib_set(MIB_WLAN_WSC_CONFIGBYEXTREG, (void *)&reset_flag);
                    apmib_get(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
                    if(!is_configured){
                        is_configured = 1;
                        DEBUG("(%s)%d change WSC CONFIGURED\n", __FUNCTION__, __LINE__); 
                        apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
                        ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
                    }
                }
            }
        }

        if(!flag){
            ERR_PRINT("(%s)%d DUT donot support WiFi-%s\n", __FUNCTION__, __LINE__, (band-1)?"5G":"2.4G");
        }
    }

    apmib_recov_wlanIdx();
    return ret;
}

int rtl_link_if5_syncWiFiConfig(WiFiConfig wifi)
	{
		int i, j, ret=0;
		int band, tmpvalue, flag=0, is_configured, reset_flag, opmode;
		char tmpbuf[128] = {0};
		char ifname[32] = {0}, gw_mac[13]={0}, macAddr[6]={0};
	
		apmib_save_wlanIdx();
	
		sscanf(wifi.synccode,"%d",&tmpvalue);
		if(wifi.synccode[0] && tmpvalue!=-1)
			apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)wifi.synccode);

		apmib_get(MIB_OP_MODE, (void *)&opmode);
			
		for(i=0; i<NUM_WLAN_INTERFACE; i++)
		{
			wlan_idx = i;
			vwlan_idx = 0;
			apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
			DEBUG("(%s)%d syncing WiFi-%s configuration\n", __FUNCTION__, __LINE__, (band-1)?"5G":"2.4G");
	
			//set wsc to configured
			apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&tmpvalue);
			if(tmpvalue){
				tmpvalue = 0;
				DEBUG("(%s)%d enable WiFi[%d] wps\n", __FUNCTION__, __LINE__, i);
				apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&tmpvalue);
				ret |= 0x1 << (i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
			}
	
        #if 1
			//set led type
			apmib_get(MIB_HW_LED_TYPE, (void *)&tmpvalue);
			if((tmpvalue < LEDTYPE_SW_LINK_TXRX) || (tmpvalue >= LEDTYPE_SW_MAX)){
				tmpvalue = LEDTYPE_SW_LINK_TXRX; //by default
				DEBUG("(%s)%d change WiFiled[%d] type\n", __FUNCTION__, __LINE__, i);
				snprintf(tmpbuf, sizeof(tmpbuf), "flash set wlan%d HW_LED_TYPE %d", wlan_idx, tmpvalue);
				system(tmpbuf);
				ret |= 0x1 << (i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
			}
        #endif
	
			flag = 0;
	
			for(j=0; j<wifi.num; j++)
			{
	
				if(band == wifi.ap[j].band)
				{
					flag = 1;
					//disable 2.4G channel-bonding before sync channel
					if(wifi.ap[j].band == PHYBAND_2G)
					{
						apmib_get(MIB_WLAN_CHANNEL_BONDING, (void *)&tmpvalue);
						if(tmpvalue)
						{
							DEBUG("(%s)%d disable WiFi-2.4G channel bonding\n", __FUNCTION__, __LINE__);
							tmpvalue = 0;
							apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&tmpvalue);
							ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
						}
					}
	
					//sync channel
					apmib_get(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
					if((tmpvalue != wifi.ap[j].channel) && (wifi.ap[j].channel!=0))
					{
						DEBUG("(%s)%d change channel from %d to %d\n", __FUNCTION__, __LINE__, tmpvalue, wifi.ap[j].channel); 
						tmpvalue = wifi.ap[j].channel;
						apmib_set(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
						ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
					}
	
					//sync txpower
#if 0
					if(wifi.ap[j].txpower == 100)
						wifi.ap[j].txpower = 0;
					apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
					if(tmpvalue != wifi.ap[j].txpower)
					{
						DEBUG("(%s)%d change txpower\n", __FUNCTION__, __LINE__, tmpvalue, wifi.ap[j].channel); 
						tmpvalue = wifi.ap[j].txpower;
						apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
						ret |= 1<<i;
					}
#endif
					//set vwlan index
					if(wifi.ap[j].idx > NUM_VWLAN)
					{
						ERR_PRINT("(%s)%d invalid virtual wlan interface index number\n", __FUNCTION__, __LINE__);
						continue;
					}
					else
						vwlan_idx = wifi.ap[j].idx;
	
					if(wifi.ap[j].idx == 0)
						DEBUG("(%s)%d syncing rootap configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx);
					else if(wifi.ap[j].idx == 2){
						DEBUG("(%s)%d syncing vap%d configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx-1);
						if(wifi.ap[j].enable == 1){
							DEBUG("(%s)%d change vap%d to guest wifi\n", __FUNCTION__, __LINE__, wifi.ap[j].idx-1);
							snprintf(ifname, sizeof(ifname), "wlan%d-va%d", wlan_idx, vwlan_idx-1);
							//printf("[====%s %d====] ifname is %s\n",__FUNCTION__,__LINE__,ifname);
							if(opmode==GATEWAY_MODE){							
								rtk_wlan_set_isolation(ifname, 1);
							}
							else{
								rtk_wlan_set_isolation(ifname, 2);															
							}							
						}else{
							DEBUG("(%s)%d disable vap%d guest wifi\n", __FUNCTION__, __LINE__, wifi.ap[j].idx-1);
							snprintf(ifname, sizeof(ifname), "wlan%d-va%d", wlan_idx, vwlan_idx-1);
							//printf("[====%s %d====] ifname is %s\n",__FUNCTION__,__LINE__,ifname);
							rtk_wlan_set_isolation(ifname, 0);
						}						
						ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
					}else
						DEBUG("(%s)%d syncing vap%d configuration\n", __FUNCTION__, __LINE__, wifi.ap[j].idx-1);
	
					//sync enable status
					if(wifi.ap[j].enable != -1){
						apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
						if(tmpvalue == wifi.ap[j].enable)
						{
							tmpvalue = !tmpvalue;
							DEBUG("(%s)%d change enable status\n", __FUNCTION__, __LINE__); 
							apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
							ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
						}
					}
#ifdef CONFIG_ELINK_SUPPORT
					if(wifi.ap[j].disable_cfg != 1){
#endif
						//sync ssid
						memset(tmpbuf, 0, sizeof(tmpbuf));
						apmib_get(MIB_WLAN_SSID, (void *)tmpbuf);
						if(strcmp(tmpbuf, wifi.ap[j].ssid))
						{
							DEBUG("(%s)%d change ssid to %s\n", __FUNCTION__, __LINE__, wifi.ap[j].ssid); 
							apmib_set(MIB_WLAN_SSID, (void *)wifi.ap[j].ssid);
							apmib_set(MIB_WLAN_WSC_SSID, (void *)wifi.ap[j].ssid);
							ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
						}
	
						//sync key related stuff
						if(wifi.ap[j].encrypt == ENCRYPT_DISABLED)
							ret |= rtl_link_setOpenEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
						else if(wifi.ap[j].encrypt == ENCRYPT_WEP)
							ret |= rtl_link_setWepEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
						else if(wifi.ap[j].encrypt >= ENCRYPT_WPA)
							ret |= rtl_link_setWpaEncrypt(wifi.ap[j])<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
#ifdef CONFIG_ELINK_SUPPORT
					}
#endif
	
#ifdef CONFIG_ANDLINK_SUPPORT
					//sync max sta num
					apmib_get(MIB_WLAN_STA_NUM, (void *)&tmpvalue);
					if(wifi.ap[j].max_sta_num > NUM_STAT)
					{
					   ERR_PRINT("ERROR (%s)%d max sta num is more than %d\n", __FUNCTION__, __LINE__, NUM_STAT); 
					   wifi.ap[j].max_sta_num = NUM_STAT;
					}
					if(tmpvalue != wifi.ap[j].max_sta_num)
					{
						DEBUG("(%s)%d change max sta num to %d\n", __FUNCTION__, __LINE__, wifi.ap[j].max_sta_num); 
						tmpvalue = wifi.ap[j].max_sta_num;
						apmib_set(MIB_WLAN_STA_NUM, (void *)&tmpvalue);
						ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
					}
	
					//sync broadcast ssid
					apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&tmpvalue);
					tmpvalue = !tmpvalue;
					if(tmpvalue != wifi.ap[j].broadcast_ssid)
					{
						DEBUG("(%s)%d change SSIDAdvertisement %s\n", __FUNCTION__, __LINE__, tmpvalue?"disabled":"enabled"); 
						apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&tmpvalue);
						ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
					}
#endif
					//set WSC configured
					if(vwlan_idx == 0){
						reset_flag = 0;
						apmib_set(MIB_WLAN_WSC_CONFIGBYEXTREG, (void *)&reset_flag);
						apmib_get(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
						if(!is_configured){
							is_configured = 1;
							DEBUG("(%s)%d change WSC CONFIGURED\n", __FUNCTION__, __LINE__); 
							apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&is_configured);
							ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
						}
					}
				}
			}
	
			if(!flag){
				ERR_PRINT("(%s)%d DUT donot support WiFi-%s\n", __FUNCTION__, __LINE__, (band-1)?"5G":"2.4G");
			}
		}
	
		apmib_recov_wlanIdx();
		return ret;
	}

int rtl_link_syncWiFiSwitch(WiFiSwitch wifiswitch)
{
    int i, k, tmpvalue, ret=0, vwlan_idx_tmp;
	char ifname[32] = {0};
	
    apmib_save_wlanIdx();

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
        if(wifiswitch.band == DUALBAND || (tmpvalue == wifiswitch.band))
        {
            apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
            if(tmpvalue == wifiswitch.enable)
            {
                DEBUG("(%s)%d change WiFi[%d] enable status\n", __FUNCTION__, __LINE__, i);
                tmpvalue = !tmpvalue;
                apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
                ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
            }
			#if 0
			if(vwlan_idx == 0 && wifiswitch.enable == 0){  //root ap disabled, need to disabled vap
				vwlan_idx_tmp = vwlan_idx;
				for(k=0; k<NUM_VWLAN_INTERFACE; k++){
					vwlan_idx = k;
					snprintf(ifname, sizeof(ifname), "wlan%d-va%d", wlan_idx, vwlan_idx);
					//printf("[====%s %d====] ifname is %s\n",__FUNCTION__,__LINE__,ifname);
					rtk_wlan_set_wlan_enable(ifname, 0);
					ret |= 1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
				}
				vwlan_idx = vwlan_idx_tmp;
			}
			#endif
        }
    }
	
	char sTemp[32] = {0};
	if(wifiswitch.enable) {
		sprintf(sTemp, "FTY_TEST wifi on %d", wifiswitch.band);
		system(sTemp);
	} else {
		sprintf(sTemp, "FTY_TEST wifi off %d", wifiswitch.band);
		system(sTemp);
	}
	
    apmib_recov_wlanIdx();
    return ret;
}

int rtl_link_syncRadioConfig(RadioConfig radio)
{
    int i, tmpvalue, ret=0;

    apmib_save_wlanIdx();

    //radio.power = (radio.power/10)*10;  
    if(radio.power == 100)
        radio.power = 0;

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
        if(radio.band == DUALBAND || (tmpvalue == radio.band))
        {
            apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&tmpvalue);
            if(tmpvalue != radio.power)
            {
                DEBUG("(%s)%d change WiFi[%d] Transmit power\n", __FUNCTION__, __LINE__, i);
                apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&radio.power);
                ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
            }
        }
    }

    apmib_recov_wlanIdx();
    return ret;
}

int rtl_link_syncWPSSwitch(WPSSwitch wps)
{
    int i, tmpvalue;
    char tmpbuf[128]; 

    apmib_save_wlanIdx();

    if(wps.enable == 0){
        DEBUG("(%s)%d change wpsswitch OFF\n", __FUNCTION__, __LINE__);
        snprintf(tmpbuf, sizeof(tmpbuf), "echo wlan%d > /tmp/wscd_cancel ", 0);
        system(tmpbuf);
    }else{
        if(wps.band == DUALBAND){
            DEBUG("(%s)%d Do WPS on AllBand\n", __FUNCTION__, __LINE__);

			for(i=0; i<NUM_WLAN_INTERFACE; i++){
				wlan_idx = i;
				vwlan_idx = 0;
				apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
				if(tmpvalue)
					continue;
				else
					break;
			}
            snprintf(tmpbuf, sizeof(tmpbuf), "%s -sig_pbc wlan%d", _WSC_DAEMON_PROG, wlan_idx);
            system(tmpbuf);
        }else{
            for(i=0; i<NUM_WLAN_INTERFACE; i++)
            {
                wlan_idx = i;
                vwlan_idx = 0;
                apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
                if(tmpvalue == wps.band)
                {
                    DEBUG("(%s)%d Do WPS on wifi[%d] Only\n", __FUNCTION__, __LINE__, i);
                    snprintf(tmpbuf, sizeof(tmpbuf), "echo 1 > /var/wps_start_interface%d", i);
                    system(tmpbuf);
                    snprintf(tmpbuf, sizeof(tmpbuf), "%s -sig_pbc wlan%d", _WSC_DAEMON_PROG, i);
                    system(tmpbuf); 
                }
            }
        }
    }

    apmib_recov_wlanIdx();
    return 0;   
}

int rtl_link_syncMacFilterSetting(MacFilter macfilter)
{
    int i, j, k, l,m, entryNum, tmpvalue, value, do_update=0, ret=0;
	int match[MAX_WLAN_AC_NUM]={0};
    MACFILTER_T macEntry, macEntrytmp;
    MacNode *node;
    char real_mac[13];
    int idx;
    UPLINK_TYPE_T uplink_type;
	unsigned char cmd[128], ifname[12];
#if 1//def APPLY_IMMEDIATE
	int do_update_iw=0, isMibMacfilterChange=0, isMibMacacChange=0, wlan_disable=1;
#endif
	CLIENT_ACCESS_CONTROL_REQ_T req;

    uplink_type = rtl_link_getUplinkType(&idx);
    if(uplink_type == WIRELESS){
#ifdef MULTI_MAC_CLONE
        for(i=0; i<macfilter.mac_list.len; i++){
            node = (MacNode *)list_get_element(&macfilter.mac_list, i);
            if(find_real_mac(idx, node->mac, real_mac)){
                strcpy(node->mac, real_mac);
            }
        }
#endif
    }
	
    apmib_save_wlanIdx();

	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
	if(entryNum){
		DEBUG("(%s)%d Delete all MACFILTER_TBL\n",__FUNCTION__,__LINE__);
		apmib_set(MIB_MACFILTER_DELALL, (void *)&macEntry);
		isMibMacfilterChange = 1;
		//system("sysconf firewall");
	}

	for(k=0; k<NUM_WLAN_INTERFACE; k++){
		wlan_idx = k;
		for(l=0; l<=NUM_VWLAN; l++){
			vwlan_idx = l;
			
			wlan_disable = 1;
			apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);

			apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum);
			
			if(entryNum){
				memset(ifname, 0, sizeof(ifname));
				if(vwlan_idx == 0){
					snprintf(ifname, sizeof(ifname)-1, "wlan%d", wlan_idx);
				}else{
					snprintf(ifname, sizeof(ifname)-1, "wlan%d-va%d", wlan_idx, vwlan_idx-1);
				}
				
				DEBUG("(%s)%d Delete %s MACAC TBL\n",__FUNCTION__,__LINE__,ifname);
				apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&macEntry);
				isMibMacacChange = 1;

#ifdef APPLY_IMMEDIATE
				if(!wlan_disable){
					memset(cmd, 0, sizeof(cmd));
					snprintf(cmd, sizeof(cmd)-1, "iwpriv %s clear_acl_table", ifname);
					do_iwpriv(NULL, NULL, 0, (void *)cmd, 0);
				}
#endif
			}
		}
	}

	// get item from macfilter.mac_list
	for(i=0; i<macfilter.mac_list.len; i++){
		node = (MacNode *)list_get_element(&macfilter.mac_list, i);
		if(node->is_lan_sta == 1){  //lan sta
			DEBUG("(%s)%d sta %s is lan sta\n", __FUNCTION__, __LINE__,node->mac);
			memset(&req, 0, sizeof(CLIENT_ACCESS_CONTROL_REQ_T));
			strncpy(req.clientMAC, node->mac, sizeof(req.clientMAC));
			req.policy = macfilter.policy;
			strncpy(req.name, node->name, sizeof(req.name));
			if(rtk_set_client_access_control_req(&req)){
				ret = -1;
				goto EXIT;
			}
			if(req.mib_changed){
				isMibMacfilterChange = 1;
				//system("sysconf firewall");
			}
		}
		else if(node->is_lan_sta == 0){ //wlan sta
			DEBUG("(%s)%d sta %s is wlan sta\n", __FUNCTION__, __LINE__,node->mac);
		    for(k=0; k<NUM_WLAN_INTERFACE; k++){
		        do_update = 0;
		        wlan_idx = k;
				for(l=0; l<=NUM_VWLAN; l++){
					vwlan_idx = l;
			        apmib_get(MIB_WLAN_MACAC_ENABLED, (void *)&tmpvalue);
			        
			        if(!macfilter.enable)
			            value = 0;
			        else if(macfilter.enable && macfilter.policy == 1)
			            value = 1;
			        else if(macfilter.enable && macfilter.policy == 0)
			            value = 2;

					memset(ifname, 0, sizeof(ifname));
					if(vwlan_idx == 0){
						snprintf(ifname, sizeof(ifname)-1, "wlan%d", wlan_idx);
					}else{
						snprintf(ifname, sizeof(ifname)-1, "wlan%d-va%d", wlan_idx, vwlan_idx-1);
					}

					wlan_disable = 1;
					apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);

			        if(tmpvalue != value)
			        {
						DEBUG("(%s)%d change WiFi[%d]-va[%d] Access Control Mode to %d\n", __FUNCTION__, __LINE__, k, l, value);

						/*ACL mode switch*/
						apmib_set(MIB_WLAN_MACAC_ENABLED, (void *)&value);
						isMibMacacChange = 1;
						
#ifndef APPLY_IMMEDIATE
						ret |= 0x1<<(k*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
#else
						do_update_iw = 1;
						if(!wlan_disable){
							do_iwpriv(ifname, "aclmode", MIB_INT_T, (void *)&value, 1);
						}
#endif			
			        }
					if(macfilter.v25 && !macfilter.contain_entries)
						continue;
				
			
	           		 //compare each entry
		            _string_to_hex(node->mac, macEntry.macAddr, strlen(node->mac));
		            snprintf((char *)macEntry.comment, COMMENT_LEN, node->name);

		            //get item from flash
		            for(j=1; j<=entryNum; j++)
		            {
		                memset(&macEntrytmp, 0x00, sizeof(macEntrytmp));
		                *((char *)&macEntrytmp) = (char)j;
		                apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&macEntrytmp);

		                if (!memcmp(macEntrytmp.macAddr, macEntry.macAddr, 6) &&
							!strcmp((char *)macEntrytmp.comment, (char *)macEntry.comment))
		                {
		                    //get a match
		                    match[i] = 1;
		                    break;
		                }
		            }
					//if no match, do update
		            if(match[i] != 1){
						do_update = 1;
		               // break;
		            }
					
				   if(do_update 
#ifdef APPLY_IMMEDIATE
					|| do_update_iw
#endif
					)
		        	{
		        	if(do_update){
			            DEBUG("(%s)%d change %s Access Control List\n", __FUNCTION__, __LINE__, ifname);
			            if(macfilter.mac_list.len > MAX_WLAN_AC_NUM)
			                macfilter.mac_list.len = MAX_WLAN_AC_NUM;

			            /* Delete all entry */
			            //apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&macEntry);

						//Add a new MACAC entry//
			            DEBUG("(%s)%d Add %s into Access Control List\n", __FUNCTION__, __LINE__, node->mac);
						apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry);
						isMibMacacChange = 1;

#ifdef APPLY_IMMEDIATE				
						/*dynamic*/	
						if(!wlan_disable){
							memset(cmd, 0, sizeof(cmd));
							snprintf(cmd, sizeof(cmd)-1, "iwpriv %s add_acl_table %02x%02x%02x%02x%02x%02x", ifname,
										macEntry.macAddr[0], macEntry.macAddr[1], macEntry.macAddr[2],
										macEntry.macAddr[3], macEntry.macAddr[4], macEntry.macAddr[5]);	
							do_iwpriv(NULL, NULL, 0, (void *)cmd, 0);
						}
					
#endif

		        	}
		            
#ifndef APPLY_IMMEDIATE
		            ret |= 0x1<<(k*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
#endif
		        	}
        		}

			}
		}        
   }

	if(isMibMacfilterChange || isMibMacacChange){
		apmib_update(CURRENT_SETTING);
		if(isMibMacfilterChange)
			system("sysconf firewall");
	}
	
    apmib_recov_wlanIdx();
	
EXIT:
    return ret;
}

int rtl_link_syncReboot(Reboot reboot)
{
    int i, tmpvalue, ret=0;
	char cmdbuf[128];

    if(reboot.band == DUALBAND)
        return 0x1<<31;

    apmib_save_wlanIdx();

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
        if(tmpvalue != reboot.band)
            continue;
        apmib_get(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
        if(tmpvalue != 0){
            tmpvalue = 0;
            DEBUG("(%s)%d change WiFi[%d] channel to AutoSelect!\n", __FUNCTION__, __LINE__, i);
            apmib_set(MIB_WLAN_CHANNEL, (void *)&tmpvalue);
			ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);  
			
        }else{
            DEBUG("(%s)%d restart WiFi[%d] to AutoSelect channel!\n", __FUNCTION__, __LINE__, i);
			snprintf(cmdbuf, sizeof(cmdbuf), "ifconfig wlan%d down", i);
			system(cmdbuf);
			snprintf(cmdbuf, sizeof(cmdbuf), "ifconfig wlan%d up", i);
			system(cmdbuf);
		}
    }
    
    apmib_recov_wlanIdx();
    return ret;
 }

int rtl_link_syncLEDOnOff(LEDStatus led)
{
    int i, tmpvalue;
    char tmpbuf[128];

    apmib_save_wlanIdx();
    //set led type
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
        wlan_idx = i;
        vwlan_idx = 0;
#if 0
        apmib_get(MIB_HW_LED_TYPE, (void *)&tmpvalue);
        if((tmpvalue < LEDTYPE_SW_LINK_TXRX) || (tmpvalue >= LEDTYPE_SW_MAX)){
            tmpvalue = LEDTYPE_SW_LINK_TXRX; //by default
            DEBUG("(%s)%d change WiFiled[%d] type\n", __FUNCTION__, __LINE__, i);
            snprintf(tmpbuf, sizeof(tmpbuf), "flash set wlan%d HW_LED_TYPE %d", wlan_idx, tmpvalue);
            system(tmpbuf);
        }
#endif   
		DEBUG("(%s)%d LedController[%d] type\n", __FUNCTION__, __LINE__, led.on);
        if(led.on){
			/*
			system("echo led_state on > /var/rtl_link_led");
            snprintf(tmpbuf, sizeof(tmpbuf), "echo 1 > proc/wlan%d/led", i);
            system(tmpbuf);
			*/
			system("FTY_TEST led on");
            led_on = 1;
        }else{
        	/*
            system("echo led_state off > /var/rtl_link_led");
            snprintf(tmpbuf, sizeof(tmpbuf), "echo 0 > proc/wlan%d/led", i);
            system(tmpbuf);
			*/
			system("FTY_TEST led off");
            led_on = 0;
        }
    }

	//apmib_set(MIB_RTL_LED_SWITCH, (void *)&led_on);
	//apmib_update(CURRENT_SETTING);
	
    apmib_recov_wlanIdx();
    return 0;
}

int rtl_link_syncWiFiSchedule(WiFiSchedule schedule)
{
    int ret=0, i, j, tmpvalue;
    SCHEDULE_T entry[MAX_SCHEDULE_NUM];

    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_SCHEDULE_ENABLED, (void *)&tmpvalue);
        if(!tmpvalue){
            DEBUG("(%s)%d enable WiFi[%d] schedule\n", __FUNCTION__, __LINE__, i);
            tmpvalue = 1;
            apmib_set(MIB_WLAN_SCHEDULE_ENABLED, (void *)&tmpvalue);
            ret  |= 0x1 << i;
        }
        memset(entry, '0', sizeof(SCHEDULE_T)*MAX_SCHEDULE_NUM);
        for(j=0; j<MAX_SCHEDULE_NUM; j++){
            *((char *)&entry[j]) = (char)(j) + 1;
            apmib_get(MIB_WLAN_SCHEDULE_TBL, (void *)&entry[j]);
            if(j < schedule.num){
                if(entry[j].eco != 1){
                    entry[j].eco = 1;
                    ret |= 0x1 << i;
                }
                if(entry[j].day != schedule.item[j].weekday){
                    entry[j].day = schedule.item[j].weekday;
                    ret |= 0x1 << i;
                }
                if(entry[j].fTime != schedule.item[j].from_time){
                    entry[j].fTime = schedule.item[j].from_time;
                    ret |= 0x1 << i;
                }
                if(entry[j].tTime != schedule.item[j].to_time){
                    entry[j].tTime = schedule.item[j].to_time;
                    ret |= 0x1 << i;
                }
            }else{
                if(entry[j].eco == 1){
                    entry[j].eco = 0;
                    ret |= 0x1 << i;
                }
            }
        }
        if(ret){
            DEBUG("(%s)%d change WiFi[%d] schedule\n", __FUNCTION__, __LINE__, i);
            apmib_set(MIB_WLAN_SCHEDULE_DELALL, (void *)&entry[0]);
            for(j=0; j<MAX_SCHEDULE_NUM; j++){
                apmib_set(MIB_WLAN_SCHEDULE_ADD, (void *)&entry[j]);
            }
        }
    }
    
    apmib_recov_wlanIdx();
    return ret;
}

static void rtl_link_write_file(char *path_name, char *MSG)
{
	int count = 0;	
	int fd;  
	
	if ((fd = open(path_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) { 	
		 printf("open file:%s fail\n",path_name);  
		 return;	
	}  
	if (write(fd, MSG, strlen(MSG)) == -1) {  
	 	ERR_PRINT("write %s to %s fail\n", MSG, path_name);  	
	}  
	DEBUG("write %s to %s success\n", MSG, path_name);

	close(fd);	
}

int rtl_link_get_percent()
{
	FILE* fp=NULL;
	char line_buffer[256];
	char *pchar, *token;
	int percent=0;

	if ((fp = fopen(DOWNLOAD_LOG_REPORT,"r")) == NULL)
	{
		ERR_PRINT("ERROR (%s)%d open file %s failed! \n", __FUNCTION__,__LINE__,DOWNLOAD_LOG_REPORT);
		exit(1);
	}else{
		while(fgets(line_buffer, sizeof(line_buffer), fp)){
			line_buffer[strlen(line_buffer)-1]='\0';
			token = strtok(line_buffer, " ");
			do{
				if(token == NULL)
				{
					ERR_PRINT("ERROR (%s)%d read file %s failed! \n", __FUNCTION__,__LINE__,DOWNLOAD_LOG_REPORT);
					break;
				}
				else
				{
					if ((pchar=strstr(token, "%"))!=NULL)
					{
						pchar = strtok(token, "%");
						percent = atoi(pchar);
						if(percent == 100)
						{
							break;
						}
					}
				}
				token = strtok(NULL, " ");
			}while(token != NULL);
			if(percent == 100)
			{
				break;
			}
		}
		
		DEBUG("(%s)%d download %d%% success!\n", __FUNCTION__,__LINE__, percent);
	}
	fclose(fp);
	return percent;
}
int rtl_link_upgradeFirmware(FWUpgrade fw)
{
    int ret=0, tmpvalue, pid, fd,check_file_times=0;
    char fwdir[256];
//#ifdef ANDLINK_IF5_SUPPORT
//#ifdef ANDLINK_V25_SUPPORT
	int percent=0;
	char cmdbuf[128]={0};
	char data[30]={0};
//#endif
    
#if defined(CONFIG_WGET_SUPPORT)
#if defined(CONFIG_ELINK_SUPPORT)
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
        apmib_get(MIB_DUALBANK_ENABLED, (void *)&tmpvalue);
        if(!tmpvalue){
            ERR_PRINT("ERROR (%s)%d enable dual firmware firstly\n", __FUNCTION__, __LINE__);
            return -1;
        }
#else
        ERR_PRINT("ERROR (%s)%d donot support upgrade\n", __FUNCTION__, __LINE__);
        return -1;
#endif
#endif 
    
    pid = fork();
    if(pid == 0){        
        if(rtl_link_download_fw(fw.url, fwdir)){			
			rtl_link_set_file_value(FWCHECK_PATH_NAME, FW_TYPE_INVALID);
			DEBUG("(%s)%d fw format invalid !\n", __FUNCTION__, __LINE__);
			exit(0);
		}
		DEBUG("(%s)%d downloading fw...\n", __FUNCTION__, __LINE__);
		//TODO: 得到下载百分比，每隔3S更新一次下载进度
		for(check_file_times=0;check_file_times<=3;check_file_times++){	
			if(isFileExist(DOWNLOAD_LOG)){
				snprintf(cmdbuf, sizeof(cmdbuf), "cat %s | tail -n 5 > %s", DOWNLOAD_LOG, DOWNLOAD_LOG_REPORT);
				system(cmdbuf);
				
				percent = rtl_link_get_percent();	
				if(percent == 100 && isFileExist(fwdir)){
					DEBUG("(%s)%d download fw to %s ok!\n", __FUNCTION__, __LINE__, fwdir);
					snprintf(data,sizeof(data),"FW_DOWNLOAD_SUCCESS\n");
					rtl_link_write_file(UPGRADE_STATUS_PATH_NAME, data);
					unlink(DOWNLOAD_LOG_REPORT);
					system("echo 1 > /var/downloadok");
					break;
				}else{
					DEBUG("(%s)%d download fw to %s %d%%!\n", __FUNCTION__, __LINE__, fwdir, percent);
					snprintf(data,sizeof(data),"%d%% FW_IS_DOWNLOADING\n", percent);
					rtl_link_write_file(UPGRADE_STATUS_PATH_NAME, data);
				}	
			}
			sleep(3);
		}
		if((!isFileExist(fwdir)) || (!isFileExist("/var/downloadok"))){
            DEBUG("(%s)%d download fw to %s fail!, fork process exit \n", __FUNCTION__, __LINE__, fwdir);
			rtl_link_write_file(UPGRADE_STATUS_PATH_NAME, "FW_DOWNLOAD_FAIL");
			exit(0);
        }
		unlink("/var/downloadok");
//#else
/*
		for(check_file_times=0;check_file_times<3;check_file_times++){
			if(isFileExist(fwdir)){
                DEBUG("(%s)%d download fw to %s ok!\n", __FUNCTION__, __LINE__, fwdir);
                rtl_link_write_file(UPGRADE_STATUS_PATH_NAME, "FW_DOWNLOAD_SUCCESS");
				break;
			}
			else{
				DEBUG("(%s)%d downloading fw to %s\n", __FUNCTION__, __LINE__, fwdir);
				sleep(3);
			}
		}

//#endif

        if(!isFileExist(fwdir)){
            DEBUG("(%s)%d download fw to %s fail!, fork process exit \n", __FUNCTION__, __LINE__, fwdir);
			rtl_link_write_file(UPGRADE_STATUS_PATH_NAME, "FW_DOWNLOAD_FAIL");
			exit(0);
        }
*/
        fd = open(RTL_LINK_FW_DIR, O_RDWR|O_CREAT|O_TRUNC);
        if (fd < 0) {
    		ERR_PRINT("ERROR (%s)%d Create file error!\n", __FUNCTION__, __LINE__);
            return -1;
        }
        write(fd, fwdir, strlen(fwdir));
        close(fd);

        if(fw.reboot){
            ret = rtl_link_do_upgrade(fwdir);
            if(ret == -1){                
				rtl_link_set_file_value(FWCHECK_PATH_NAME, FW_UPGRADE_FAIL);
				DEBUG("(%s)%d fw upgrade fail !\n", __FUNCTION__, __LINE__);
            }else{
#ifndef CONFIG_APP_FWD
                sleep(2);
                system("reboot");
#endif
            }
        }
        exit (0);
    }
#else
    ERR_PRINT("ERROR (%s)%d enable wget firstly\n", __FUNCTION__, __LINE__);     
#endif

    return 0;

}

int rtl_link_syncRoamingConfigDrv(RoamingConfig roaming)
{
	unsigned char ifname[20]={0};
	int i, rssi_2g, rssi_5g, band;

	apmib_save_wlanIdx();
	
	rssi_2g = roaming.rssi_2g + 100;
    rssi_5g = roaming.rssi_5g + 100;

	for(i=0; i<NUM_WLAN_INTERFACE; ++i){
		wlan_idx = i;
		vwlan_idx = 0;
		snprintf(ifname, sizeof(ifname), "wlan%d", wlan_idx);

		do_iwpriv(ifname, "roaming_switch", MIB_INT_T, (void *)&roaming.enable, 1);
		do_iwpriv(ifname, "roaming_qos", MIB_INT_T, (void *)&roaming.qos, 1);		
		do_iwpriv(ifname, "fail_ratio", MIB_INT_T, (void *)&roaming.fail_ratio, 1);
		do_iwpriv(ifname, "retry_ratio", MIB_INT_T, (void *)&roaming.retry_ratio, 1);		
		do_iwpriv(ifname, "block_aging", MIB_INT_T, (void *)&roaming.dismiss_time, 1);

		apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
		if(band==PHYBAND_2G && roaming.rssi_2g){
			do_iwpriv(ifname, "RSSIThreshold", MIB_INT_T, (void *)&rssi_2g, 1);
		}else if(band==PHYBAND_5G && roaming.rssi_5g){
			do_iwpriv(ifname, "RSSIThreshold", MIB_INT_T, (void *)&rssi_5g, 1);
		}		
	}

	apmib_recov_wlanIdx();

	return 0;
}

int rtl_link_syncRoamingConfig(RoamingConfig roaming)
{
    int i, rssi_2g, rssi_5g, tmpvalue, band, ret=0;
    int wifi_restart=0;

    apmib_save_wlanIdx();
    rssi_2g = roaming.rssi_2g + 100;
    rssi_5g = roaming.rssi_5g + 100;

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
        wifi_restart |=  0x1<<(i*(NUM_VWLAN_INTERFACE+1));

    apmib_get(MIB_RTL_LINK_ROAMING_SWITCH, (void *)&tmpvalue);
    if(tmpvalue != roaming.enable){
        DEBUG("(%s)%d change Roaming switch %s\n", __FUNCTION__, __LINE__, roaming.enable?"on":"off");
        tmpvalue = roaming.enable;
        apmib_set(MIB_RTL_LINK_ROAMING_SWITCH, (void *)&tmpvalue);
        ret |= wifi_restart;
    }
        
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;		
        
        apmib_get(MIB_WLAN_RSSI_TH, (void *)&tmpvalue);
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
        
        if(band == PHYBAND_2G){
            if(roaming.rssi_2g && tmpvalue != rssi_2g){
                DEBUG("(%s)%d change WiFi[%d] RSSI Threshhold to %d\n", __FUNCTION__, __LINE__, i, rssi_2g-100);
                apmib_set(MIB_WLAN_RSSI_TH, (void *)&rssi_2g);
                ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);
            }
        }
        
        if(band == PHYBAND_5G){
            if(roaming.rssi_5g && tmpvalue != rssi_5g){
                DEBUG("(%s)%d change WiFi[%d] RSSI Threshhold to %d\n", __FUNCTION__, __LINE__, i, rssi_5g-100);
                apmib_set(MIB_WLAN_RSSI_TH, (void *)&rssi_5g);
                ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);				
            }
        }            
    }

    apmib_get(MIB_RTL_LINK_ROAMING_QOS, (void *)&tmpvalue);
    if(tmpvalue != roaming.qos){
        tmpvalue = roaming.qos;
        DEBUG("(%s)%d change Roaming QOS switch %s\n", __FUNCTION__, __LINE__, tmpvalue?"on":"off");
        apmib_set(MIB_RTL_LINK_ROAMING_QOS, (void *)&tmpvalue);
        ret |= wifi_restart;
    }

    apmib_get(MIB_RTL_LINK_ROAMING_FAIL_RATIO, (void *)&tmpvalue);
    if(tmpvalue != roaming.fail_ratio){
        tmpvalue = roaming.fail_ratio;
        DEBUG("(%s)%d change Roaming fail ratio to %d\n", __FUNCTION__, __LINE__, tmpvalue);
        apmib_set(MIB_RTL_LINK_ROAMING_FAIL_RATIO, (void *)&tmpvalue);
        ret |= wifi_restart;
    }

    apmib_get(MIB_RTL_LINK_ROAMING_RETRY_RATIO, (void *)&tmpvalue);
    if(tmpvalue != roaming.retry_ratio){
        tmpvalue = roaming.retry_ratio;
        DEBUG("(%s)%d change Roaming retry ratio to %d\n", __FUNCTION__, __LINE__, tmpvalue);
        apmib_set(MIB_RTL_LINK_ROAMING_RETRY_RATIO, (void *)&tmpvalue);
        ret |= wifi_restart;
    }

    apmib_get(MIB_RTL_LINK_ROAMING_DISMISS_TIME, (void *)&tmpvalue);
    if(tmpvalue != roaming.dismiss_time){
        tmpvalue = roaming.dismiss_time;
        DEBUG("(%s)%d change Roaming dismiss time to %d\n", __FUNCTION__, __LINE__, tmpvalue);
        apmib_set(MIB_RTL_LINK_ROAMING_DISMISS_TIME, (void *)&tmpvalue);
        ret |= wifi_restart;
    }

    apmib_get(MIB_RTL_LINK_ROAMING_DETEC_PERIOD, (void *)&tmpvalue);
    if(tmpvalue != roaming.detect_period){
        tmpvalue = roaming.detect_period;
        apmib_set(MIB_RTL_LINK_ROAMING_DETEC_PERIOD, (void *)&tmpvalue);
    }

    apmib_get(MIB_RTL_LINK_ROAMING_RSSI_DELTA, (void *)&tmpvalue);
    if(tmpvalue != roaming.rssi_delta){
        tmpvalue = roaming.rssi_delta;
        apmib_set(MIB_RTL_LINK_ROAMING_RSSI_DELTA, (void *)&tmpvalue);
    }
    apmib_recov_wlanIdx();

#ifdef APPLY_IMMEDIATE
	if(ret){
		rtl_link_syncRoamingConfigDrv(roaming);
		apmib_update(CURRENT_SETTING);
	}
	return 0;
#else
    return ret;
#endif
}

int rtl_link_syncDeviceLimit(DeviceLimit D_Limit)
{
	int i, j, k, l, tmpvalue, entryNum, index;
	int match[MAX_WLAN_SBWC_NUM]={0};
	double rx_rate, tx_rate;
	WLAN_SBWC_ENTRY_T sbwcEntry, sbwcEntrytmp;
	NodeLimit *N_limit=NULL;
	unsigned char cmd[128], ifname[12];
	int ret = 0, do_update=0, wlan_disable=1;
	QOS_REQ_T req;
	int wisp_id=0, wan_mode=0, opmode=0;

	apmib_save_wlanIdx();
	

	//get item from D_Limit list
	for(i=0;i<D_Limit.limit_list.len;i++){
		N_limit = (NodeLimit *)list_get_element(&D_Limit.limit_list, i);
		if(N_limit->is_lan_sta){  //lan sta
			DEBUG("(%s)%d sta %s is lan sta\n", __FUNCTION__, __LINE__,N_limit->mac);
			memset(&req, 0, sizeof(QOS_REQ_T));
			strncpy(req.clientMAC, N_limit->mac, sizeof(req.clientMAC));
			strncpy(req.maxDownlinkRate, N_limit->limit_rxrate, sizeof(req.maxDownlinkRate));
			strncpy(req.maxUplinkRate, N_limit->limit_txrate, sizeof(req.maxUplinkRate));
			if(rtk_set_qos_req(&req)){
				ret = -1;
				goto EXIT;
			}
			if(req.mib_changed){
				//system("sysconf firewall");
				system("sysconf setQos");
				
				apmib_update(CURRENT_SETTING);
			}
		}else{ // wlan sta
			DEBUG("(%s)%d sta %s is wlan sta\n", __FUNCTION__, __LINE__,N_limit->mac);
			apmib_get(MIB_SBWC_ENABLE, (void *)&tmpvalue);
			if(tmpvalue == 0){
				tmpvalue = 1;
				DEBUG("(%s)%d change SBWC Mode to %d\n", __FUNCTION__, __LINE__,tmpvalue);
				apmib_set(MIB_SBWC_ENABLE, (void *)&tmpvalue);
				do_update = 1;
			}

			//compare to each entry
			apmib_get(MIB_SBWC_NUM, (void *)&entryNum);
	        _string_to_hex(N_limit->mac, sbwcEntry.sbwc_sta_mac, strlen(N_limit->mac));
			tx_rate = atof(N_limit->limit_txrate);
			rx_rate = atof(N_limit->limit_rxrate);
			//printf("[====%s %d====] tx_rate is %0.4f\n",__FUNCTION__,__LINE__,tx_rate);
			//printf("[====%s %d====] rx_rate is %0.4f\n",__FUNCTION__,__LINE__,rx_rate);
			sbwcEntry.sbwc_sta_tx_limit = rx_rate * 1024;
			sbwcEntry.sbwc_sta_rx_limit = tx_rate * 1024;
		//	printf("[====%s %d====] sbwcEntry.sbwc_sta_tx_limit is %d\n",__FUNCTION__,__LINE__,sbwcEntry.sbwc_sta_tx_limit);
		//	printf("[====%s %d====] sbwcEntry.sbwc_sta_rx_limit is %d\n",__FUNCTION__,__LINE__,sbwcEntry.sbwc_sta_rx_limit);
			//sbwcEntry.sbwc_sta_tx_limit = atoi(N_limit->limit_txrate);
			//sbwcEntry.sbwc_sta_rx_limit = atoi(N_limit->limit_rxrate);

			//get item from flash
	        for(j=1; j<=entryNum; j++)
	        {
	            memset(&sbwcEntrytmp, 0x00, sizeof(sbwcEntrytmp));
	            *((char *)&sbwcEntrytmp) = (char)j;
	            apmib_get(MIB_SBWC_TBL, (void *)&sbwcEntrytmp);
	            if (!memcmp(sbwcEntrytmp.sbwc_sta_mac, sbwcEntry.sbwc_sta_mac, MAC_ADDR_LEN))
	            {
	                //get a match
	                DEBUG("(%s)%d Device(MAC:%s) has already been store into SBWC TBL\n",__FUNCTION__,__LINE__,N_limit->mac);
					if((sbwcEntrytmp.sbwc_sta_tx_limit != sbwcEntry.sbwc_sta_tx_limit) || 
						(sbwcEntrytmp.sbwc_sta_rx_limit != sbwcEntry.sbwc_sta_rx_limit))
					{
						//update device's tx or rx rate limit
						apmib_set(MIB_SBWC_DEL, (void *)&sbwcEntrytmp);
						
						DEBUG("(%s)%d Updata device(MAC:%s)'s tx or rx rate limit\n",__FUNCTION__,__LINE__,N_limit->mac);
						sbwcEntrytmp.sbwc_sta_tx_limit = sbwcEntry.sbwc_sta_tx_limit;
						sbwcEntrytmp.sbwc_sta_rx_limit = sbwcEntry.sbwc_sta_rx_limit;
						apmib_set(MIB_SBWC_ADD, (void *)&sbwcEntrytmp);
						do_update = 1;	

#ifdef APPLY_IMMEDIATE
					    iwpriv_set_DeviceLimit(sbwcEntry);
#endif					
					}
					match[i] = 1;	
	                break;
	            }
	        }
				
			if(match[i] != 1){
				if((entryNum+1) > MAX_WLAN_SBWC_NUM){
					DEBUG("(%s)%d SBWC TBL has reached it's maximum capacity, delete the oldest entry!\n",
					__FUNCTION__,__LINE__);
					int index = 1;
					memset(&sbwcEntrytmp, 0x00, sizeof(sbwcEntrytmp));
	           	 	*((char *)&sbwcEntrytmp) = (char)index;
	            	apmib_get(MIB_SBWC_TBL, (void *)&sbwcEntrytmp);
					apmib_set(MIB_SBWC_DEL, (void *)&sbwcEntrytmp);
				}
				DEBUG("(%s)%d Store new device(MAC:%s) into SBWC TBL!\n",__FUNCTION__,__LINE__,N_limit->mac);
				apmib_set(MIB_SBWC_ADD, (void *)&sbwcEntry);
				do_update = 1;
				
#ifdef APPLY_IMMEDIATE	
				iwpriv_set_DeviceLimit(sbwcEntry);		
#endif
			}
		}
	}
				
#ifndef APPLY_IMMEDIATE	
	if(do_update){
		for(k=0; k<NUM_WLAN_INTERFACE; k++){
			ret |= 0x1<<(k*(NUM_VWLAN_INTERFACE+1) + 0);
			goto EXIT;
		}
	}
#endif

	if(do_update)
		apmib_update(CURRENT_SETTING);

	apmib_recov_wlanIdx();

EXIT:	
	return ret;
}

int rtl_link_syncTimerTask(TimerTask T_Task)
{
	int i, k, entryNum, ret=0, do_update, seconds=0, tmp_taskid=0;
	int index=1;
	WLAN_TIMER_TASK_ENTRY_T timer_task_entry, timer_task_entry_tmp;
	WLAN_TIMER_TASK_ENTRY_T target[2];
	time_t tm;
  	struct tm tm_time;

	memset(&timer_task_entry, 0, sizeof(WLAN_TIMER_TASK_ENTRY_T));
	memset(&timer_task_entry_tmp, 0, sizeof(WLAN_TIMER_TASK_ENTRY_T));
	memset(target, 0x00, sizeof(WLAN_TIMER_TASK_ENTRY_T)*2);

	apmib_get(MIB_TIMER_TASK_NUM, (void *)&entryNum);
	if(strcmp(T_Task.operation, "Add") == 0){
		if((entryNum+1) > MAX_TIMER_TASK_NUM){
			ERR_PRINT("ERROR ANDLINK_IF5(%s)%d TimerTask TBL has reached it's maximum capacity\n", __FUNCTION__, __LINE__);
			ret = 0;
			goto EXIT;
		}
		DEBUG("(%s)%d Store new task into TIMER TASK TBL!\n",__FUNCTION__,__LINE__);
		//add一条时，taskid自增
		*((char *)&timer_task_entry_tmp) = (char)entryNum;
		apmib_get(MIB_TIMER_TASK_TBL, (void *)&timer_task_entry_tmp);
		tmp_taskid = atoi(timer_task_entry_tmp.taskid);
		snprintf(timer_task_entry.taskid, sizeof(timer_task_entry.taskid), "%d", (tmp_taskid+1));

		timer_task_entry.timeoffset = T_Task.timeoffset;

		snprintf(timer_task_entry.action, sizeof(timer_task_entry.action), T_Task.action);

		if(T_Task.week == 0){
			timer_task_entry.enable = 2;
			time(&tm);
  			memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
			seconds = tm_time.tm_hour*3600 + tm_time.tm_min*60 + tm_time.tm_sec;
			if((strcmp(T_Task.action, "ToReboot") == 0) && (seconds > T_Task.timeoffset)){
				//if(seconds > T_Task.timeoffset){
					if(tm_time.tm_wday <= 5)
						T_Task.week = 0x1<<(tm_time.tm_wday + 1);
					else
						T_Task.week = 1;
				//}else
				//	T_Task.week = 0x1<<(tm_time.tm_wday);
			}else
				T_Task.week = 0x1<<(tm_time.tm_wday);
			timer_task_entry.week = T_Task.week;			
		}else{
			timer_task_entry.week = T_Task.week;
			timer_task_entry.enable = T_Task.enable;
		}
		
		if(strcmp(T_Task.action, "ToSetHealthMode") == 0){
			//如果timeoffset2值小于timeoffset的值，则将timeoffset2的值加上86400存入mib
			if(T_Task.timeoffset2 < T_Task.timeoffset)
				timer_task_entry.timeoffset2 = T_Task.timeoffset2 + SECONDS_OF_A_DAY;
			else
				timer_task_entry.timeoffset2 = T_Task.timeoffset2;

			//index 0:all band   1-4:2.4G    5-8:5G 
			timer_task_entry.index = T_Task.index;
		}

#if 0
		DEBUG("timer_task_entry.taskid = %s\n", timer_task_entry.taskid);
		DEBUG("timer_task_entry.timeoffset = %d\n", timer_task_entry.timeoffset);		
		DEBUG("timer_task_entry.week = %d\n", timer_task_entry.week);		
		DEBUG("timer_task_entry.enable = %d\n", timer_task_entry.enable);		
		DEBUG("timer_task_entry.action = %s\n", timer_task_entry.action);
		DEBUG("timer_task_entry.timeoffset2 = %d\n", timer_task_entry.timeoffset2);
		DEBUG("timer_task_entry.index = %d\n", timer_task_entry.index);
#endif

		apmib_set(MIB_TIMER_TASK_ADD, (void *)&timer_task_entry);
		do_update = 1;
	}else if(strcmp(T_Task.operation, "Delete") == 0){
		//delete taskid index，delete一条时，按照taskid删除，其余taskid不变
		index = atoi(T_Task.taskid);
		for(i=1; i<=entryNum; i++){
			*((char *)&timer_task_entry_tmp) = (char)i;
			apmib_get(MIB_TIMER_TASK_TBL, (void *)&timer_task_entry_tmp);
			tmp_taskid = atoi(timer_task_entry_tmp.taskid);
			if(tmp_taskid == index){
				apmib_set(MIB_TIMER_TASK_DEL, (void *)&timer_task_entry_tmp);
			}
		}
		do_update = 1;
	}else if(strcmp(T_Task.operation, "Update") == 0){
		//update一条时，找到对应taskid做mod操作
		index = atoi(T_Task.taskid);
		for(i=1; i<=entryNum; i++){
			*((char *)&timer_task_entry_tmp) = (char)i;
			apmib_get(MIB_TIMER_TASK_TBL, (void *)&timer_task_entry_tmp);
			tmp_taskid = atoi(timer_task_entry_tmp.taskid);
			if(tmp_taskid == index){
				memcpy(&target[0], &timer_task_entry_tmp, sizeof(WLAN_TIMER_TASK_ENTRY_T));
				if(timer_task_entry_tmp.timeoffset != T_Task.timeoffset)
					timer_task_entry_tmp.timeoffset = T_Task.timeoffset;
				if(timer_task_entry_tmp.week != T_Task.week)
					timer_task_entry_tmp.week = T_Task.week;
				if(timer_task_entry_tmp.enable != T_Task.enable)
					timer_task_entry_tmp.enable = T_Task.enable;
				if(strcmp(timer_task_entry_tmp.action, "ToSetHealthMode") == 0){
					if(timer_task_entry_tmp.timeoffset2 != T_Task.timeoffset2){
						//如果timeoffset2值小于timeoffset的值，则将timeoffset2的值加上86400存入mib
						if(T_Task.timeoffset2 < T_Task.timeoffset)
							timer_task_entry_tmp.timeoffset2 = T_Task.timeoffset2 + SECONDS_OF_A_DAY;
						else
							timer_task_entry_tmp.timeoffset2 = T_Task.timeoffset2;
					}
					if(timer_task_entry_tmp.index != T_Task.index)
						timer_task_entry_tmp.index = T_Task.index;
				}

				//update一条时，如果week为0，需要做特殊处理
				if(timer_task_entry_tmp.week == 0){
			 		timer_task_entry_tmp.enable = 2;
					time(&tm);
					memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
					seconds = tm_time.tm_hour*3600 + tm_time.tm_min*60 + tm_time.tm_sec;
					if((strcmp(timer_task_entry_tmp.action, "ToReboot") == 0) && (seconds > timer_task_entry_tmp.timeoffset)){
						if(tm_time.tm_wday <= 5)
							timer_task_entry_tmp.week = 0x1<<(tm_time.tm_wday + 1);
						else
							timer_task_entry_tmp.week = 1;
					}else
						timer_task_entry_tmp.week = 0x1<<(tm_time.tm_wday);
				}else{
					//timer_task_entry_tmp.week = T_Task.week;
					//timer_task_entry_tmp.enable = T_Task.enable;
				}
				
				memcpy(&target[1], &timer_task_entry_tmp, sizeof(WLAN_TIMER_TASK_ENTRY_T));
    			apmib_set(MIB_TIMER_TASK_MOD, (void *)&target); 
			}
		}
		do_update = 1;
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d unknown Operation item\n", __FUNCTION__, __LINE__);
        ret = 0;
		goto EXIT;
	}

	if(do_update){
		ret |= 0x1<<NUM_WLAN_INTERFACE*(NUM_VWLAN_INTERFACE+1);
	}

EXIT:
	return ret;
}

int rtl_link_getRFInfo(RfStatus *RF)
{
    int i, value, ret=0;
    char wlan_if[16];
    bss_info bss;

    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx=i;
        vwlan_idx = 0;

        RF->num++;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&value);
        RF->rf[i].band= value;
        
        apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&value);
        RF->rf[i].enable = !value;

        apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&value);
        RF->rf[i].power = value;
        if(RF->rf[i].power == 0)
            RF->rf[i].power = 100;
        else if(RF->rf[i].power == 1)
            RF->rf[i].power = 70;
        else if(RF->rf[i].power == 2)
            RF->rf[i].power = 50;
        else if(RF->rf[i].power == 3)
            RF->rf[i].power = 35;
        else if(RF->rf[i].power == 4)
            RF->rf[i].power = 15;

        snprintf(wlan_if, sizeof(wlan_if), WLAN_IF, i);
        if(getWlBssInfo_andlink(wlan_if, &bss)){
            ERR_PRINT("ERROR (%s)%d getbssinfo error\n", __FUNCTION__, __LINE__);
            ret = -1;
            goto EXIT;
        }
        RF->rf[i].channel = bss.channel;
    }
EXIT:
    apmib_recov_wlanIdx();
    return ret;    
}

void rtl_link_getSysMemInfo(SysMemInfo *memInfo)
{
	SYS_MEMINFO_T info;
	int nValue = 0;
	
	if(memInfo==NULL){
		ERR_PRINT("ERROR (%s)%d memInfo NULL\n", __FUNCTION__, __LINE__);
		return;
	}

	memset(&info, 0, sizeof(SYS_MEMINFO_T));
	rtk_get_system_mem_info(&info); 
	rtk_get_cpu_occupy(&nValue);
	
	memInfo->ramSize = info.ramSizeTotal;
	memInfo->ramUtility = info.ramUtility;
	memInfo->flashSize = info.flashSizeTotal;
	memInfo->cpuUtility = nValue;

	return;
}

void rtl_link_getWiFiConfig(WiFiConfig *wifi)
{
    int i, j, k=0, value;
    char tmpbuf[128], key_str[128];

    apmib_save_wlanIdx();
    
    apmib_get(MIB_RTL_LINK_SYNCCODE, (void *)&wifi->synccode);
    if(!wifi->synccode[0]){
        strcpy(wifi->synccode, "0");
    }

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx=i;

        for(j=0; j<=NUM_VWLAN; j++)
        {
            vwlan_idx = j;
            //radio
            apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&value);
            wifi->ap[k].band = value;
			//channel
			apmib_get(MIB_WLAN_CHANNEL, (void *)&wifi->ap[k].channel);
            //index
            wifi->ap[k].idx = j;
            //enable
            apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&value);
			
            wifi->ap[k].enable = !value;
#ifdef CONFIG_ANDLINK_SUPPORT            
            if(wifi->ap[k].idx == 0){
                wifi->ap[k].root_disabled = value;
            }else{
                wifi->ap[k].root_disabled = wifi->ap[k-j].root_disabled;
            }
			if(wifi->ap[k].root_disabled)
				wifi->ap[k].enable = 0;
#endif      
			
            //ssid
            memset(tmpbuf, 0, sizeof(tmpbuf));
            apmib_get(MIB_WLAN_SSID, (void *)tmpbuf);
            snprintf(wifi->ap[k].ssid, sizeof(wifi->ap[k].ssid), "%s", tmpbuf);
            //SecurityMode & Pwd
            memset(tmpbuf, 0, sizeof(tmpbuf));
            apmib_get(MIB_WLAN_ENCRYPT, (void *)&value);
            if(value == ENCRYPT_WEP){
                wifi->ap[k].encrypt = ENCRYPT_WEP;

                apmib_get(MIB_WLAN_WEP, (void *)&value);
                wifi->ap[k].wep_type = value;
                if(value == WEP128){
                    apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&value);
                    apmib_get(MIB_WLAN_WEP128_KEY1, (void *)tmpbuf);
                    if(value == KEY_HEX){
                        convert_bin_to_str((unsigned char*)tmpbuf, WEP128_KEY_LEN, key_str);
                        snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", key_str);
                    }else{
                        snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", tmpbuf);
                    }
                }else{
                    apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&value);
                    apmib_get(MIB_WLAN_WEP64_KEY1, (void *)tmpbuf);
                    if(value == KEY_HEX){
                        convert_bin_to_str((unsigned char*)tmpbuf, WEP64_KEY_LEN, key_str);
                        snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", key_str);
                    }else{
                        snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", tmpbuf);
                    }
                }
            } else if (value == ENCRYPT_WPA) {
                wifi->ap[k].encrypt = ENCRYPT_WPA;
                apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&value);
                wifi->ap[k].wpa_cypher = value;
                apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpbuf);
                snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", tmpbuf);
            } else if(value == ENCRYPT_WPA2) {
                wifi->ap[k].encrypt = ENCRYPT_WPA2;
                apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&value);
                wifi->ap[k].wpa_cypher = value;
                apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpbuf);
                snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", tmpbuf);
            } else if(value == ENCRYPT_WPA2_MIXED) {
                wifi->ap[k].encrypt = ENCRYPT_WPA2_MIXED;
                apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&value);
                wifi->ap[k].wpa_cypher = value;
                apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpbuf);
                snprintf(wifi->ap[k].key, sizeof(wifi->ap[k].key), "%s", tmpbuf);
            } else {
                wifi->ap[k].encrypt = ENCRYPT_DISABLED;
            }
#ifdef CONFIG_ANDLINK_SUPPORT
            //MaxAssociateNum
            apmib_get(MIB_WLAN_STA_NUM, (void *)&value);
            if(!value)
                wifi->ap[k].max_sta_num = NUM_STAT;
            else
                wifi->ap[k].max_sta_num = value;
            //SSIDAdvertisement
            apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&value);
            wifi->ap[k].broadcast_ssid = !value;
#endif            
            k++;
        }
    }
    wifi->num = k;

    apmib_recov_wlanIdx();
}

void rtl_link_getLEDStatus(LEDStatus *led)
{
    led->on = led_on;
}

void rtl_link_getRoamingStatus(RoamingConfig *roaming)
{
    int band, value, i;

    apmib_get(MIB_RTL_LINK_ROAMING_SWITCH, (void *)&value);    
    roaming->enable = value;

    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
        apmib_get(MIB_WLAN_RSSI_TH, (void *)&value);
        if(band == PHYBAND_2G){
            roaming->rssi_2g = value;
        }else{
            roaming->rssi_5g = value;
        }
    }
    apmib_recov_wlanIdx();
}

void rtl_link_getWorkMode(int *workmode)
{
	int rtl_link_workmode;
	apmib_get(MIB_OP_MODE, (void *)&rtl_link_workmode); 
	if(rtl_link_workmode == BRIDGE_MODE)
		*workmode=0;
	else
		*workmode=1;
}

void rtl_link_getWiFiSwitchStatus(WiFiSwitch *wifi)
{
    int i, tmpvalue;

    apmib_save_wlanIdx();

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&tmpvalue);
        wifi->enable = !tmpvalue;
    }
    
    apmib_recov_wlanIdx();
}

void rtl_link_getWiFiTimerSchedule(WiFiSchedule *schedule)
{
    int i, k=0;
    SCHEDULE_T entry;
    
    for(i=0; i<MAX_SCHEDULE_NUM; i++){
        *((char *)&entry) = (char)(i) + 1;
        apmib_get(MIB_WLAN_SCHEDULE_TBL, (void *)&entry);
        if(entry.eco == 1){
            schedule->item[k].weekday = entry.day;
            schedule->item[k].from_time = entry.fTime;
            schedule->item[k].to_time = entry.tTime;
            k++;
        }
    }

    schedule->num = k;
}

void rtl_link_getAPStatus(APStatus *status)
{
    int i, value, ret; 
    int memtotal, memfree, tx_pre, tx_now, rx_pre, rx_now, terminal_num, channel, channelload;
    char filename[64], line_buffer[256], ifname[16];
    char *pchar, *token;
    FILE *fp;
    bss_info bss;
    struct sysinfo info ;

    apmib_save_wlanIdx();
    //band
    if(NUM_WLAN_INTERFACE == 1){
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&value);
        if(value == PHYBAND_2G)
            status->band = BAND2G;
        else
            status->band = BAND5G;
    }else{
        status->band = DUALBAND;
    }
    
    //cpurate
    snprintf(filename, sizeof(filename), "/proc/cpuinfo");
    if((fp=fopen(filename, "r")) == NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
        ret = -1;
        goto EXIT;
    }
    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        line_buffer[strlen(line_buffer)-1]='\0';
        if((pchar=strstr(line_buffer, "BogoMIPS"))!=NULL){
            pchar = strchr(line_buffer, ':');
            pchar+=1;
            status->cpurate = atoi(pchar);
        }
    }
        
    fclose(fp);

    //memoryuserate
    snprintf(filename, sizeof(filename), "/proc/meminfo");
    if((fp=fopen(filename, "r")) == NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
        ret = -1;
        goto EXIT;
    }
    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        line_buffer[strlen(line_buffer)-1]='\0';
        if((pchar=strstr(line_buffer, "MemTotal:"))!=NULL){
            pchar+=strlen("MemTotal:");
		    token = strtok(pchar, " ");
		    memtotal = atoi(token);
        }
        if((pchar=strstr(line_buffer, "MemFree:"))!=NULL){
            pchar+=strlen("MemFree:");
		    token = strtok(pchar, " ");
		    memfree = atoi(token);
		    break;
        }
    }
    
    status->memoryuserate = 100-(memfree*100/memtotal);
    
    fclose(fp);
    
    //get up/downloadspeed & up/downloadstat & channelload
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        snprintf(filename, sizeof(filename), "/proc/wlan%d/stats", i);
        snprintf(ifname, sizeof(ifname), WLAN_IF, i);
        if((fp = fopen(filename, "r")) == NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
            ret = -1;
            goto EXIT;
        }
        
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            if((pchar=strstr(line_buffer, "tx_bytes:"))!=NULL){
                pchar+=strlen("tx_bytes:");
    		    token = strtok(pchar, " ");
    		    tx_pre = atoi(token);
            }else if((pchar=strstr(line_buffer, "rx_bytes:"))!=NULL){
                pchar+=strlen("rx_bytes:");
    		    token = strtok(pchar, " ");
    		    rx_pre = atoi(token);
    		    break;
            }
        }
        fclose(fp);

        sleep(1);
        
        if((fp=fopen(filename, "r")) == NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
            ret = -1;
            goto EXIT;
        }
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            if((pchar=strstr(line_buffer, "tx_bytes:"))!=NULL){
                pchar+=strlen("tx_bytes:");
    		    token = strtok(pchar, " ");
    		    tx_now= atoi(token);
            }else if((pchar=strstr(line_buffer, "rx_bytes:"))!=NULL){
                pchar+=strlen("rx_bytes:");
    		    token = strtok(pchar, " ");
    		    rx_now = atoi(token);
            }else if((pchar=strstr(line_buffer, "ch_utilization:"))!=NULL){
                pchar+=strlen("ch_utilization:");
    		    token = strtok(pchar, " ");
    		    channelload = atoi(token);
            }else if((pchar=strstr(line_buffer, "chbusytime:"))!=NULL){
                pchar+=strlen("chbusytime:");
    		    token = strtok(pchar, " ");
    		    channelload = atoi(token)*100/1310;
            }
            
        }
        fclose(fp);

        //get terminal_num
        snprintf(filename, sizeof(filename), "/proc/wlan%d/sta_info", i);
        if((fp=fopen(filename, "r")) == NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
            ret = -1;
            goto EXIT;
        }
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            if((pchar=strstr(line_buffer, "active: "))!=NULL){
                pchar+=strlen("active: ");
    		    terminal_num = atoi(token);
    		    break;
            }
        }
        fclose(fp);

        //get channel_num
        if(getWlBssInfo_andlink(ifname, &bss)){
            ERR_PRINT("ERROR (%s)%d getbssinfo error\n", __FUNCTION__, __LINE__);
            ret = -1;
            goto EXIT;
        }
        channel = bss.channel;
        
        wlan_idx = i;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&value);
        if(value == PHYBAND_2G){
            status->uploadspeed_2G = (rx_now - rx_pre)/1000;
            status->downloadspeed_2G = (tx_now - tx_pre)/1000;
            status->uploadstat_2G = rx_now;
            status->downloadstat_2G = tx_now;
            status->channelload_2G = channelload;
            status->channel_2G = channel;
            status->terminalNum_2G = terminal_num;
        }else{
            status->uploadspeed_5G = (rx_now - rx_pre)/1000;
            status->downloadspeed_5G = (tx_now - tx_pre)/1000;
            status->uploadstat_5G = rx_now;
            status->downloadstat_5G = tx_now;
            status->channelload_5G = channelload;
            status->channel_5G = channel;
            status->terminalNum_5G = terminal_num;
        }
    }

    //get wired terminal_num
    status->wired_terminalNum = rtl_link_getLanDevInfo(NULL);

    //get online time
    sysinfo(&info);
    status->onlinetime = (unsigned long)info.uptime;
    
EXIT:
    apmib_recov_wlanIdx();
    
}

int rtl_link_getUplinkType(int *idx)
{
    int i, cnt=0;
    char filename[64], line_buffer[256];
    char *pchar;
    FILE *fp;
   
#ifdef CONFIG_RTL_REPEATER_MODE_SUPPORT
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        snprintf(filename, sizeof(filename), "/proc/wlan%d-vxd/sta_info", i);
            
        if((fp= fopen(filename, "r"))==NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
            return -1;
        }
        fgets(line_buffer, sizeof(line_buffer), fp);
        line_buffer[strlen(line_buffer)-1]='\0';
        if((pchar=strstr(line_buffer, "active: 0")) !=NULL)
            cnt++;
        else
            *idx = i;
        fclose(fp);
    }
    
    if(cnt == NUM_WLAN_INTERFACE)
        return ETHERNET;
    else
        return WIRELESS;
#else
    return ETHERNET;
#endif
}

int rtl_link_getUplinkStatus(UplinkStatus *uplink)
{
    int i, snr=0, cnt=0, rssi, value, ret=0;
    char ifname[16], tmpbuf[MAX_SSID_LEN]={0}, filename[64], line_buffer[256], line_buffer1[256]; 
    char gw_mac[18]={0}, tmp_mac[18], port[6], rssi_s[3], cur_ip[16]={0};
    char *pchar;
    bss_info bss;
    FILE *fp;
    int idx;
    struct sockaddr hwaddr;
    struct in_addr ip;
    char buffer[512]={0};
	int lan_sta_index;
	char *token=NULL;
	char *savestr=NULL;
	char lan_txrate[56]={0};
	char lan_rxrate[56]={0};


#ifdef CONFIG_ANDLINK_SUPPORT
    if(!gw_ip[0]){
        //apmib_get(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip);
        //strcpy(gw_ip, inet_ntoa(ip));
    }
#endif

    apmib_save_wlanIdx();

    if((uplink->type=rtl_link_getUplinkType(&idx)) == -1){
        ret = -1;
        goto EXIT;
    }
    
    if(uplink->type == WIRELESS){
        //wireless
        snprintf(filename, sizeof(filename), "/proc/wlan%d-vxd/sta_info", idx);
        wlan_idx = idx;
        vwlan_idx = 0;
        
        if((fp= fopen(filename, "r"))==NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
            ret = -1;
            goto EXIT;
        }
        
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&value);
        uplink->band = value;

        vwlan_idx = NUM_VWLAN_INTERFACE;  //vxd
        apmib_get(MIB_WLAN_SSID, (void *)tmpbuf);
        snprintf(uplink->ssid, sizeof(uplink->ssid), tmpbuf);
        
        snprintf(ifname, sizeof(ifname), VXD_IF, idx);
        if(getWlBssInfo_andlink(ifname, &bss)){
            ERR_PRINT("ERROR (%s)%d getbssinfo error\n", __FUNCTION__, __LINE__);
            ret = -1;
            goto EXIT;
        }
        uplink->channel = bss.channel;
        
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            if((pchar=strstr(line_buffer, "hwaddr: "))!=NULL)
            {
                pchar+=strlen("hwaddr: ");
                for(i=0; i<6; i++){
                    strncat(uplink->mac, pchar+i*2, 2);
                    strcat(uplink->mac, ":");
                }
                uplink->mac[17]='\0';
            }
            else if((pchar=strstr(line_buffer, "rssi: "))!=NULL)
	        {
	            pchar += strlen("rssi: ");
	            snprintf(rssi_s, sizeof(rssi_s), pchar);
	            rssi = atoi(rssi_s)-100;
	            snprintf(uplink->rssi, sizeof(uplink->rssi), "%d", rssi);
	        }
	        else if((pchar=strstr(line_buffer, "current_tx_rate:"))!=NULL)
	        {
	            pchar = strrchr(line_buffer, ' ');
	            pchar += 1;
	            snprintf(uplink->txrate, sizeof(uplink->txrate), pchar);
	        }
	        else if((pchar=strstr(line_buffer, "current_rx_rate:"))!=NULL || (pchar=strstr(line_buffer, "rx_rate:"))!=NULL)
	        {
	            pchar = strrchr(line_buffer, ' ');
	            pchar += 1;
	            snprintf(uplink->rxrate, sizeof(uplink->rxrate), pchar);
	        }
	        else if((pchar=strstr(line_buffer, "rx_snr"))!=NULL)
	        {
	            pchar += strlen("rx_snrX: ");
	            if(atoi(pchar) != 0)
	            {
	                snr += atoi(pchar);
	                cnt++;
	            }
	        }
	        else if((pchar=strstr(line_buffer, "2: stat_info..."))!=NULL)
	            break;
        }
        fclose(fp);
        if(cnt)
            snr = snr/cnt;
        snprintf(uplink->snr, sizeof(uplink->snr),  "%d", snr);
        snprintf(uplink->noise, sizeof(uplink->noise), "%d", rssi-snr);
    }else if(uplink->type == ETHERNET){
#if 0//CONFIG_APP_RTK_ADAPTER       //2018.05.10 LAN_DEV_LINK_INFO have bug: it won't contine port4 info under bridge mode or wanport info under gw mode 
		FILE *fp=NULL;
		
		if((fp = fopen(LAN_DEV_LINK_INFO,"r")) == NULL)
		return -1;
		
		while(fgets(buffer, sizeof(buffer), fp) && lan_sta_index<MAX_STA_NUM)
		{
			for(i=0;buffer[i]!='\0';i++)
			if(buffer[i]=='\n')
				buffer[i]='\0';
			token=strtok_r(buffer,",",&savestr);
			while(token != NULL)
			{
			    if(strncmp("ip=", token, strlen("ip="))==0)
			    {
			        if(strcmp(gw_ip, (token+strlen("ip="))))
			            continue;
			    }
			    if(strncmp("conn_port=", token, strlen("conn_port="))==0)
			    {
			        sscanf(token, "conn_port=%d", &uplink->wan_port_num);
			    }
				if(strncmp("up_speed=",token,strlen("up_speed="))==0)
				{
				    sscanf(token, "up_speed=%d", &uplink->cur_txrate);
				}
				if(strncmp("down_speed=",token,strlen("down_speed="))==0)
				{
				    sscanf(token, "down_speed=%d", &uplink->rxrate);
				}		
				token=strtok_r(NULL,",",&savestr);
			}
			lan_sta_index++;	
		}
		fclose(fp);
#else        
        //get gw_mac through gw_ip
        if((fp= fopen("/proc/net/arp", "r"))==NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/net/arp");
            ret = -1;
            goto EXIT;
        }
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            sscanf(line_buffer, "%s %*s %*s %s %*s %*s", cur_ip, tmp_mac);
            if(!strcmp(cur_ip, gw_ip))
		    {
		        strcpy(gw_mac, tmp_mac);
		        DEBUG("ANDLINK (%s)%d GW-MAC: %s\n", __FUNCTION__, __LINE__, gw_mac);
		        break;
		    }
        }
        if(fp)
            fclose(fp);

        //get port num through gw_mac
        if(gw_mac[0]){
            if((fp= fopen("/proc/rtl865x/l2", "r"))==NULL){
                ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/rtl865x/l2");
                ret = -1;
                goto EXIT;
            }
            while(fgets(line_buffer, sizeof(line_buffer), fp))
            {
                line_buffer[strlen(line_buffer)-1]='\0';
                if((pchar=strstr(line_buffer, gw_mac))!=NULL)
    		        {
    		            pchar = strstr(line_buffer, "mbr(");
    		            pchar += strlen("mbr(");
    		            snprintf(port, sizeof(port), "Port%c", *pchar);
    		            uplink->wan_port_num = *pchar - '0';
    		            break;
    		        }
            }
            fclose(fp);
        }else{
            ERR_PRINT("ERROR ANDLINK (%s)%d get GW-MAC failed\n", __FUNCTION__, __LINE__);
        }

        //get ethernet tx/rx speed through port id
        if((fp= fopen("/proc/rtl865x/port_status", "r"))==NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/rtl865x/port_status");
            ret = -1;
            goto EXIT;
        }
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            if((pchar=strstr(line_buffer, port))!=NULL)
		    {
		        fgets(line_buffer1, sizeof(line_buffer1), fp);
		        line_buffer1[strlen(line_buffer1)-1]='\0';
		        if(strstr(line_buffer, "LinkUp |") || strstr(line_buffer1, "LinkUp |")){
		            while(fgets(line_buffer, sizeof(line_buffer), fp))
		            {
		                line_buffer[strlen(line_buffer)-1]='\0';
		                if((pchar=strstr(line_buffer, "Speed "))!=NULL)
		                {
		                    pchar += strlen("Speed ");
            		        if(pchar[strlen(pchar)-1] == 'G'){
            		            pchar[strlen(pchar)-1]='\0';    //remove 'G'
            		            strcat(pchar, "000");
            		        }else{
            		            pchar[strlen(pchar)-1]='\0';    //remove 'M'
            		        }
            		        snprintf(uplink->rxrate, sizeof(uplink->rxrate), pchar);
            		        snprintf(uplink->txrate, sizeof(uplink->txrate), pchar);
            		        break;
		                }
		            }
		            break;
		        }else if(strstr(line_buffer, "LinkDown") || strstr(line_buffer1, "LinkDown")){
		            ERR_PRINT("ERROR (%s)%d %s is linkdown!\n", __FUNCTION__, __LINE__, port);
                    break;
		        }
		    } 
        }
        fclose(fp);
#endif
    }

    if(rtl_link_getLocalIp(uplink->ip))
    {
        DEBUG("(%s)%d get local ip failed\n", __FUNCTION__, __LINE__);
        goto EXIT;
    }

EXIT:
    apmib_recov_wlanIdx();
    return ret;
}
#ifdef CONFIG_APP_RTK_ADAPTER
/*Warnining: Need call list_destroy(&xxx, free) to avoid memory leak*/
int rtl_link_getWlanSTAInfo(List *sta_list)
{
    int i, ret=0;
    STAInfo *sta;
    LAN_DEV_BASE_INFO wlan_sta[MAX_STA_NUM];
    int connect_num;
    if(rtk_lan_get_wifi_client_info(wlan_sta, MAX_STA_NUM, &connect_num)==-1){
        ret = -1;
        goto EXIT;
    }
        
    for(i=0; i<connect_num; i++){
        sta = malloc(sizeof(STAInfo));
	    if(!sta){
            ERR_PRINT("ERROR (%s)%d malloc error\n", __FUNCTION__, __LINE__);
            ret = -1;
            goto EXIT;
        }

        sprintf(sta->mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
            wlan_sta[i].mac[0], wlan_sta[i].mac[1], wlan_sta[i].mac[2], wlan_sta[i].mac[3], wlan_sta[i].mac[4], wlan_sta[i].mac[5]);
		sta->uptype = WIRELESS;
		rtl_link_get_clone_mac(sta);
        snprintf(sta->hostname, sizeof(sta->hostname), "%s", wlan_sta[i].dev_name);
        snprintf(sta->uptime, sizeof(sta->uptime), "%d", wlan_sta[i].link_time);
        snprintf(sta->rssi, sizeof(sta->rssi), "%d", wlan_sta[i].rssi-100);
		snprintf(sta->ip_addr,sizeof(sta->ip_addr),"%s",inet_ntoa(*((struct in_addr *)&wlan_sta[i].ip)));
		snprintf(sta->ssid,sizeof(sta->ssid),"%s",wlan_sta[i].ssid);
		snprintf(sta->txrate,sizeof(sta->txrate),wlan_sta[i].txrate);
		snprintf(sta->rxrate,sizeof(sta->rxrate),wlan_sta[i].rxrate);
		sta->cur_rxrate = wlan_sta[i].down_speed;
		sta->cur_txrate = wlan_sta[i].up_speed;
        sta->uptype = WIRELESS;
        if(wlan_sta[i].conn_type == LAN_DEV_CONN_WLAN_2G)
            sta->band = PHYBAND_2G;
        else
            sta->band = PHYBAND_5G;
        list_insert(sta_list, sta);
    }

EXIT:
    return ret;
}
#else
int rtl_link_getWlanSTAInfo(List *sta_list)
{
    int i, j, value, ret=0;
    char filename[64];

    apmib_save_wlanIdx();
    
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        
        for(j=0; j<=NUM_VWLAN; j++)
        {
            vwlan_idx = j;
            apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&value);
            if(j == 0)
                snprintf(filename, sizeof(filename), "/proc/wlan%d/sta_info", i);
            else
                snprintf(filename, sizeof(filename), "/proc/wlan%d-va%d/sta_info", i, j-1);

            ret = rtl_link_get_wifi_device_mac_list(filename, sta_list);
            if(ret==-1){
                ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
                goto EXIT;
            }
        }
    }
EXIT:
    apmib_recov_wlanIdx();
    return ret;
}
#endif

#ifdef CONFIG_APP_RTK_ADAPTER
static int rtl_link_getLanTRxrate(STAInfo *station)
{
	if(!station)
		goto EXIT;
	FILE *fp=NULL;
	char *pchar=NULL;
	int ret;
	char line_buffer[256]={0},line_buffer1[256]={0},port[6],temp_mac[18]={0},str_port_num[5]={0},sta_mac[18];
	if((fp= fopen("/proc/rtl865x/l2", "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen error\n", __FUNCTION__, __LINE__);
		ret=-1;
        goto EXIT;
	}
	fgets(line_buffer,sizeof(line_buffer),fp);	//skip ASIC L2 Table:
	while(fgets(line_buffer,sizeof(line_buffer),fp))
	{
		line_buffer[sizeof(line_buffer)-1]='\0';
		sscanf(line_buffer,"%*[^]] %*s %s %*s %s",temp_mac,str_port_num);
		snprintf(sta_mac,sizeof(sta_mac),
"%s",station->mac);
		if(!strcasecmp(temp_mac,sta_mac))
		   {
			   pchar = strstr(str_port_num, "mbr(");
			   pchar += strlen("mbr(");
			   snprintf(port, sizeof(port), "Port%c", *pchar);
			   break;
		   }
		else
			continue;
	}
	fclose(fp);
    if((fp= fopen("/proc/rtl865x/port_status", "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/rtl865x/port_status");
        ret = -1;
        goto EXIT;
    }
    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        line_buffer[strlen(line_buffer)-1]='\0';
        if((pchar=strstr(line_buffer, port))!=NULL)
	    {
	        fgets(line_buffer1, sizeof(line_buffer1), fp);
	        line_buffer1[strlen(line_buffer1)-1]='\0';
	        if(strstr(line_buffer, "LinkUp |") || strstr(line_buffer1, "LinkUp |")){
	            while(fgets(line_buffer, sizeof(line_buffer), fp))
	            {
	                line_buffer[strlen(line_buffer)-1]='\0';
	                if((pchar=strstr(line_buffer, "Speed "))!=NULL)
	                {
	                    pchar += strlen("Speed ");
        		        if(pchar[strlen(pchar)-1] == 'G'){
        		            pchar[strlen(pchar)-1]='\0';    //remove 'G'
        		            strcat(pchar, "000");
        		        }else{
        		            pchar[strlen(pchar)-1]='\0';    //remove 'M'
        		        }
						snprintf(station->rxrate, sizeof(station->rxrate), pchar);
            		    snprintf(station->txrate, sizeof(station->txrate), pchar);
        		        break;
	                }
	            }
	            break;
	        }else if(strstr(line_buffer, "LinkDown") || strstr(line_buffer1, "LinkDown")){
	            ERR_PRINT("ERROR (%s)%d %s is linkdown!\n", __FUNCTION__, __LINE__, port);
                break;
	        }
	    } 
    }
EXIT:
	if(fp)
		fclose(fp);
	return ret;
}
/*Warnining: Need call list_destroy(&xxx, free) to avoid memory leak*/
int rtl_link_getLanDevInfo(List *sta_list)
{
    int i, ret=0;
    STAInfo *sta=NULL;
    LAN_DEV_BASE_INFO lan_sta[MAX_STA_NUM];
    int connect_num=0;
    char gw_mac[18]={0}, cur_ip[16]={0};
    char tmp_mac[18], line_buffer[512];
    FILE *fp=NULL;
    char *pchar;
    int wan_port=-1;

    //get gw_mac through gw_ip
    if((fp= fopen("/proc/net/arp", "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/net/arp");
        return -1;
    }
    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        line_buffer[strlen(line_buffer)-1]='\0';
		sscanf(line_buffer, "%s %*s %*s %s %*s %*s", cur_ip, tmp_mac);
        if(!strcmp(cur_ip, gw_ip))
	    {
	        strcpy(gw_mac, tmp_mac);
	        DEBUG("ANDLINK (%s)%d GW-MAC: %s\n", __FUNCTION__, __LINE__, gw_mac);
	        break;
	    }
    }
    if(fp)
        fclose(fp);
   
    //get wan port num through gw_mac
    if(gw_mac[0]){
        if((fp= fopen("/proc/rtl865x/l2", "r"))==NULL){
           ERR_PRINT("ERROR (%s)%d fopen error\n", __FUNCTION__, __LINE__);
           ret = -1;
        }
        while(fgets(line_buffer, sizeof(line_buffer), fp))
        {
            if(strstr(line_buffer, gw_mac)){
                pchar = strstr(line_buffer, "mbr(");
                sscanf(pchar, "mbr(%d)", &wan_port);
                break;
            }
        }
        if(fp)
            fclose(fp);
    }else{
        ERR_PRINT("ERROR ANDLINK (%s)%d get GW-MAC failed\n", __FUNCTION__, __LINE__);
    }

    if(rtk_lan_get_wired_client_info(lan_sta, MAX_STA_NUM, &connect_num)==-1){
        return -1;
    }

    if(sta_list == NULL)
        return connect_num;
        
    for(i=0; i<connect_num; i++){
        if(lan_sta[i].conn_port == wan_port)    //skip wan port
            continue;
        sta = malloc(sizeof(STAInfo));
	    if(!sta){
            ERR_PRINT("ERROR (%s)%d malloc error\n", __FUNCTION__, __LINE__);
            return -1;
        }
		memset(sta, 0, sizeof(STAInfo));
		sta->uptype = ETHERNET;
        sprintf(sta->mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
            lan_sta[i].mac[0], lan_sta[i].mac[1], lan_sta[i].mac[2], lan_sta[i].mac[3], lan_sta[i].mac[4], lan_sta[i].mac[5]);
		rtl_link_getLanTRxrate(sta);
        rtl_link_get_clone_mac(sta);
        snprintf(sta->hostname, sizeof(sta->hostname), "%s", lan_sta[i].dev_name);
        snprintf(sta->ip_addr, sizeof(sta->ip_addr), "%s", inet_ntoa(*((struct in_addr *)&lan_sta[i].ip)));
        snprintf(sta->uptime, sizeof(sta->uptime), "%d", lan_sta[i].link_time);
        sta->cur_rxrate = lan_sta[i].down_speed;
		sta->cur_txrate = lan_sta[i].up_speed;
        
        list_insert(sta_list, sta);
    }
    
    return connect_num;    
}
#else
/*Warnining: Need call list_destroy(&xxx, free) to avoid memory leak*/
int rtl_link_getLanDevInfo(List *sta_list)
{
    FILE *fp1=NULL, *fp2=NULL, *fp3=NULL, *fp=NULL;
    char line_buffer[512], line_buffer1[512], port[6], wan_port[6], tmpmac[18], gw_mac[18]; 
    int i, j, ret=0;    
    char *pchar, *pstart;
    STAInfo *sta=NULL;
    unsigned char br0_mac[6];
    unsigned char br0_mac_str[32];
    struct in_addr ip;
	char cur_ip[16]={0};

#ifdef CONFIG_ANDLINK_SUPPORT
    if(!gw_ip[0]){
        //apmib_get(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip);
        //strcpy(gw_ip, inet_ntoa(ip));
    }
#endif

	//get gw_mac through gw_ip
    if((fp= fopen("/proc/net/arp", "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/net/arp");
        ret = -1;
        goto EXIT;
    }
    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        line_buffer[strlen(line_buffer)-1]='\0';
		sscanf(line_buffer, "%s %*s %*s %s %*s %*s", cur_ip, gw_mac);
        if(!strcmp(cur_ip, gw_ip))
	    {
	        DEBUG("ANDLINK (%s)%d GW-MAC: %s\n", __FUNCTION__, __LINE__, gw_mac);
	        break;
	    }
    }
    if(fp)
        fclose(fp);

    //get wan port num through gw_mac
    if((fp= fopen("/proc/rtl865x/l2", "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen error\n", __FUNCTION__, __LINE__);
        ret = -1;
        goto EXIT;
	}

    while(fgets(line_buffer, sizeof(line_buffer), fp))
    {
        if(strstr(line_buffer, gw_mac)){
            pchar = strstr(line_buffer, "mbr(");
            pchar += strlen("mbr(");
            snprintf(wan_port, sizeof(wan_port), "Port%c", *pchar);
            break;
        }
    }
    if(fp)
        fclose(fp);

    //get lan dev info
	if((fp1= fopen("/proc/rtl865x/l2", "r"))==NULL){
        ERR_PRINT("ERROR (%s)%d fopen error\n", __FUNCTION__, __LINE__);
        ret = -1;
        goto EXIT;
	}
	memset(br0_mac,0,6);
	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)br0_mac);
	if(!memcmp(br0_mac, "\x00\x00\x00\x00\x00\x00", 6))
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)br0_mac);
	
	snprintf((char *)br0_mac_str, sizeof(br0_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", br0_mac[0], br0_mac[1], br0_mac[2], br0_mac[3], br0_mac[4], br0_mac[5]);
	br0_mac_str[strlen("ff:ff:ff:ff:ff:ff")]='\0';

    fgets(line_buffer, sizeof(line_buffer), fp1); //skip line 1: "ASIC L2 Table:"

    while(fgets(line_buffer, sizeof(line_buffer), fp1))
	{
	    //get lan dev port num
		line_buffer[strlen(line_buffer)-1]='\0';

		if(strstr(line_buffer, "ff:ff:ff:ff:ff:ff") || strstr(line_buffer, "CPU") || strstr(line_buffer, "FID:1") || strstr(line_buffer, (char *)br0_mac_str) || strstr(line_buffer, gw_mac))
			continue;
	    pchar = strstr(line_buffer, "mbr(");
	    pchar += strlen("mbr(");
	    snprintf(port, sizeof(port), "Port%c", *pchar);
	    //skip wan port
	    if(!strcmp(port, wan_port))
	        continue;
	    //get lan dev mac
	    pchar=strchr(line_buffer, ':');
		pstart=pchar-2;
		memcpy(tmpmac, pstart, 17);
		tmpmac[17] = '\0';

        //get uplink status
        if((fp2= fopen("/proc/rtl865x/port_status", "r"))==NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/rtl865x/port_status");
            ret = -1;
            goto EXIT;
        }
        while(fgets(line_buffer, sizeof(line_buffer), fp2))
        {
            line_buffer[strlen(line_buffer)-1]='\0';
            if((pchar=strstr(line_buffer, port))!=NULL)
		    {
		        fgets(line_buffer1, sizeof(line_buffer1), fp2);
		        line_buffer1[strlen(line_buffer1)-1]='\0';
		        if(strstr(line_buffer, "LinkUp |") || strstr(line_buffer1, "LinkUp |")){
		            if(sta_list){
    		            sta = malloc(sizeof(STAInfo));
    	                if(!sta){
                            ERR_PRINT("ERROR (%s)%d malloc\n", __FUNCTION__, __LINE__);
                            ret = -1;
                            goto EXIT;
                        }
                        memset(sta, 0, sizeof(STAInfo));
                        list_insert(sta_list, sta);
                        sta->uptype = ETHERNET;
                        snprintf(sta->mac, sizeof(sta->mac), tmpmac);
                        rtl_link_get_clone_mac(sta);
                        snprintf(sta->port, sizeof(sta->port), port);
                        //get tx/rx speed
                        while(fgets(line_buffer, sizeof(line_buffer), fp2))
                        {
                            line_buffer[strlen(line_buffer)-1]='\0';
                            if((pchar=strstr(line_buffer, "Speed "))!=NULL)
                            {
                                pchar += strlen("Speed ");
                                if(pchar[strlen(pchar)-1] == 'G'){
                                    pchar[strlen(pchar)-1]='\0';    //remove 'G'
                                    strcat(pchar, "000");
                                }else{
                                    pchar[strlen(pchar)-1]='\0';    //remove 'M'
                                }
                                snprintf(sta->rxrate, sizeof(sta->rxrate), pchar);
                                snprintf(sta->txrate, sizeof(sta->txrate), pchar);
                            }
                            if(strlen(line_buffer) == 0)
                                break;
                        }
                        //get uptime
                        if((fp3= fopen("/proc/portUptime", "r"))!=NULL){
                            while(fgets(line_buffer, sizeof(line_buffer), fp3))
                            {
                                if((pchar=strstr(line_buffer, port))!=NULL){
                                    if((pchar=strstr(line_buffer, "UpTime:")) != NULL){
                                        pchar += strlen("UpTime:");
                                        pchar[strlen(pchar)-2]='\0';    // remove 's' and '\n'
                                        snprintf(sta->uptime, sizeof(sta->uptime), pchar);
                                        break;
                                    }
                                }
                            }
                        }else{
                            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, "/proc/portUptime");
                        }
                    }
                    ret++;
                    break;
                } else if(strstr(line_buffer, "LinkDown") || strstr(line_buffer1, "LinkDown")){
                    DEBUG("(%s)%d %s is linkdown continue!\n", __FUNCTION__, __LINE__, port);
                    system("cat proc/rtl865x/l2");
                    system("cat proc/rtl865x/port_status");
                    break;
                }
            } 
        }
        fclose(fp2);
        fp2 = NULL;
    }
EXIT:
    
    if(fp1)
        fclose(fp1);
    if(fp2)
        fclose(fp2);
    if(fp3)
        fclose(fp3);
    return ret;         
}
#endif

int rtl_link_get_terminal_rate_by_mac(char *mac_s, int *download_rate, int *upload_rate)
{
    FILE *fp=NULL;
    int fh=0;
    int idx =0,i=0;
    unsigned char buffer[128];
	unsigned char strtmp[24]={0};
	unsigned char macAddr[12]={0};
	unsigned char mac[6]={0};
	struct rtk_terminal_rate_info rate_entry[MAX_TERM_NUMBER];

	format_mac(mac_s, mac);

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
			_string_to_hex(strtmp, macAddr, 12);
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
    		//printf("%s:::%d, mac=%02x:%02x:%02x:%02x:%02x:%02x\n",__FUNCTION__,__LINE__,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			//printf("%s:::%d, mac=%02x:%02x:%02x:%02x:%02x:%02x\n",__FUNCTION__,__LINE__,rate_entry[i].mac[0],rate_entry[i].mac[1],rate_entry[i].mac[2],rate_entry[i].mac[3],rate_entry[i].mac[4],rate_entry[i].mac[5]);
    	    if(equal_mac(mac, rate_entry[i].mac))
    	    {
    	        *download_rate = rate_entry[i].download_speed;
    			*upload_rate = rate_entry[i].upload_speed;
    			return 0;
    	    }
    	}
	}

	
out :
    return -1;

}

int rtl_link_get_terminal_rate_by_port(char *port_s, int *download_rate, int *upload_rate)
{
    FILE *fp=NULL;
    int fh=0;
    int idx =0,i=0;
    int port;
    unsigned char buffer[128];
	unsigned char strtmp[24]={0};
	unsigned char macAddr[12]={0};
	struct rtk_terminal_rate_info rate_entry[MAX_TERM_NUMBER];

	sscanf(port_s, "Port%d", &port);

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
			_string_to_hex(strtmp, macAddr, 12);
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
    	        *download_rate = rate_entry[i].download_speed;
    			*upload_rate = rate_entry[i].upload_speed;
    			return 0;
    	    }
    	}
	}

	
out :
    return -1;

}


int rtl_link_getWiFiTrafficStats(WiFiTrafficStats *wifi)
{
    int i, j, value, k=0, ret=0;
    char filename[64];

    apmib_save_wlanIdx();

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        
        for(j=0; j<=NUM_VWLAN; j++)
        {
            vwlan_idx = j;
            apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&value);
            if(!value)
            {
                wifi->ap[k].idx = j;
                if(j==0)
                    snprintf(filename, sizeof(filename), "/proc/wlan%d/stats", i);
                else
                    snprintf(filename, sizeof(filename), "/proc/wlan%d-va%d/stats", i, j-1);
                
                ret = get_wifi_traffic_stats(filename, &wifi->ap[k]);
                if(ret){
                    ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
                    goto EXIT;
                }
                k++;
            }
        }
    }

    wifi->num = k;
    
EXIT:
    apmib_recov_wlanIdx();
    return ret;
}

int rtl_link_getLanTrafficStats(LanTrafficStats *lan)
{
	char filename[64]={0};
	char buffer[256]={0};
	FILE *fp=NULL;
	FILE *fp_for_get_port_num=NULL;
	char *pchar=NULL;
	int  line_cnt =0;
	int port_index=0;
	int port_num=0;
	char string_port[3]={0};

	long long lan_TotalBytesSent=0;
	long long lan_TotalBytesReceived=0;
	long long lan_TotalPacketsSent=0;
	long long lan_TotalPacketsReceived=0;
	int lan_ErrorsSent=0;
	int lan_ErrorsReceived=0;
	int lan_DiscardPacketsSent=0;
	int lan_DiscardPacketsReceived=0;
	int lan_Send_Unicast_pkts=0;
	int lan_Send_Multicast_pkts=0;
	int lan_Send_Broadcast_pkts=0;
	int lan_Rcv_Unicast_pkts=0;
	int lan_Rcv_Multicast_pkts=0;
	int lan_Rcv_Broadcast_pkts=0;

	// TODO: 8367
	snprintf(filename,sizeof(filename),"proc/rtl865x/asicCounter");
	
	//get ap port_num
	if((fp_for_get_port_num= fopen(filename, "r"))==NULL){
			ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
			return -1;
	}
	while(fgets(buffer, sizeof(buffer), fp_for_get_port_num))
	{
		if(pchar=strstr(buffer,"<Port: ") != NULL)
		{
			sscanf(buffer,"%*s %s%*[^>]",string_port);
			port_num=atoi(string_port);
		}
	}
	fclose(fp_for_get_port_num);

	lan->num=port_num+1;
	//get every port traffic stats
	if((fp= fopen(filename, "r"))==NULL){
			ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
			return -1;
	}
	for(port_index=0;port_index<=port_num;port_index++)
	{
		while(fgets(buffer, sizeof(buffer), fp))
		{
			line_cnt++;
			if(line_cnt == 12*port_index+3) //update receive bytes
			{
				sscanf(buffer," Rcv %lld ",&lan_TotalBytesReceived);
			}
			
			if(line_cnt == 12*port_index+5) //update Unicast/Multicast/Broadcast pkts
			{
				sscanf(buffer," Unicast %d %*s Multicast %d %*s Broadcast %d",&lan_Rcv_Unicast_pkts,&lan_Rcv_Multicast_pkts,&lan_Rcv_Broadcast_pkts);
			}
			
			if(line_cnt == 12*port_index+10)	//update send bytes Unicast/Multicast pkts
			{
				sscanf(buffer," Snd %lld %*s Unicast %d %*s Multicast %d",&lan_TotalBytesSent,&lan_Send_Unicast_pkts,&lan_Send_Multicast_pkts);
			}
			if(line_cnt == 12*port_index+11)	//update Broadcast pkts
			{
				sscanf(buffer," Broadcast %d ",&lan_Send_Broadcast_pkts);
				break;
			}
		}
		lan->ap[port_index].idx=port_index;
		lan->ap[port_index].send_bytes=lan_TotalBytesSent;
		lan->ap[port_index].recv_bytes=lan_TotalBytesReceived;
		lan->ap[port_index].send_packets=lan_Send_Unicast_pkts+lan_Send_Multicast_pkts+lan_Send_Broadcast_pkts;
		lan->ap[port_index].recv_packets=lan_Rcv_Unicast_pkts+lan_Rcv_Multicast_pkts+lan_Rcv_Broadcast_pkts;
		//printf("********%lld,%lld,%lld,%lld\n",lan->ap[port_index].send_bytes,lan->ap[port_index].recv_bytes,lan->ap[port_index].send_packets,lan->ap[port_index].recv_packets);
		//can't get by asicCounter set to 0 by default
		lan->ap[port_index].err_send=0;
		lan->ap[port_index].err_recv=0;
		lan->ap[port_index].discard_send=0;
		lan->ap[port_index].discard_recv=0;
	}
	fclose(fp);
	return 0;
}


/*Warnining: Need call list_destroy(&xxx, free) to avoid memory leak*/
void rtl_link_getMacFilterSetting(MacFilter *macfilter)
{
    int macac_enabled, macfilter_enabled, macfilter_mode, entryNum;
    int i;
    MACFILTER_T *macEntry;

    apmib_get(MIB_WLAN_MACAC_ENABLED, (void *)&macac_enabled);
	apmib_get(MIB_MACFILTER_ENABLED, (void *)&macfilter_enabled);
	apmib_get(MIB_MACFILTER_MODE, (void *)&macfilter_mode);

    if(macac_enabled == 0 && macfilter_enabled == 0){
        macfilter->enable = 0; 
    }else {
        macfilter->enable = 1;
    }

	if(macac_enabled == 1 || macfilter_mode == 0){
		macfilter->policy = 1;
	}else
		macfilter->policy = 0;

    apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum);
    for(i=1; i<=entryNum; i++)
    {
        macEntry = malloc(sizeof(MACFILTER_T));
        *((char *)macEntry) = (char)i;
        apmib_get(MIB_WLAN_MACAC_ADDR, (void *)macEntry);
        list_insert(&macfilter->mac_list, macEntry);
    }

	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
	for(i=1; i<=entryNum; i++)
    {
    	macEntry = malloc(sizeof(MACFILTER_T));
        *((char *)macEntry) = (char)i;
		apmib_get(MIB_MACFILTER_TBL, (void *)macEntry);
		list_insert(&macfilter->mac_list, macEntry);
	}	
    
}

void rtl_link_getRoamingConfig(RoamingConfig *roaming)
{
    int i, value, band;

    apmib_get(MIB_RTL_LINK_ROAMING_SWITCH, (void *)&value);
    roaming->enable = value;

    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
        apmib_get(MIB_WLAN_RSSI_TH, (void *)&value);
        if(band == PHYBAND_2G)
            roaming->rssi_2g = value-100;
        else
            roaming->rssi_5g = value-100;
    }
    apmib_recov_wlanIdx();

    apmib_get(MIB_RTL_LINK_ROAMING_QOS, (void *)&value);
    roaming->qos = value;
    if(roaming->qos){
        apmib_get(MIB_RTL_LINK_ROAMING_RETRY_RATIO, (void *)&value);
        roaming->retry_ratio = value;
        apmib_get(MIB_RTL_LINK_ROAMING_FAIL_RATIO, (void *)&value);
        roaming->fail_ratio = value;
    }
    
    apmib_get(MIB_RTL_LINK_ROAMING_DETEC_PERIOD, (void *)&value);
    roaming->detect_period = value;
    
    apmib_get(MIB_RTL_LINK_ROAMING_RSSI_DELTA, (void *)&value);
    roaming->rssi_delta = value;

    apmib_get(MIB_RTL_LINK_ROAMING_DISMISS_TIME, (void *)&value);
    roaming->dismiss_time = value;
}

int rtl_link_delete_sta(char *if_name, unsigned char *mac)
{
    int sock, ret=0;
    struct iwreq wrq;
    unsigned char mac_str[13];

    sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    DEBUG("(%s)%d DELETE STA:%s !\n", __FUNCTION__, __LINE__, mac_str);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		ERR_PRINT("(%s)%d Error Creating Socket for ioctl.\n", __FUNCTION__, __LINE__); 
		ret = -1;
		goto EXIT;
	}

	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, if_name, IFNAMSIZ);

	wrq.u.data.pointer = (caddr_t)mac_str;
	wrq.u.data.length = 18;

	if(ioctl(sock, RTL8192CD_IOCTL_DEL_STA, &wrq) < 0) {	
		ERR_PRINT("(%s)%d Error to ioctl.\n", __FUNCTION__, __LINE__); 
		ret = -1;
		goto EXIT;
	}

EXIT:
    if(sock>=0)
        close(sock);
    return ret;
}


int rtl_link_silent_sta(unsigned char *mac)
{
    int i, j, sock, ret=0;
    char if_name[16];
    struct iwreq wrq;
    unsigned char mac_str[13];

    sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    DEBUG("(%s)%d SILENT STA:%s !\n", __FUNCTION__, __LINE__, mac_str);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		ERR_PRINT("(%s)%d Error Creating Socket for ioctl.\n", __FUNCTION__, __LINE__); 
		ret = -1;
		goto EXIT;
	}

	memset(&wrq, 0, sizeof(wrq));
    
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        for(j=0; j<=NUM_VWLAN; j++)
        {
            if(j==0)
                sprintf(if_name, "wlan%d", i);
            else
                sprintf(if_name, "wlan%d-va%d", i, j-1);

            strncpy(wrq.ifr_name, if_name, IFNAMSIZ);
            wrq.u.data.pointer = (caddr_t)mac_str;
	        wrq.u.data.length = 18;

	        if(ioctl(sock, RTL8192CD_IOCTL_SILENT_STA, &wrq) < 0) {	
        		ERR_PRINT("(%s)%d Error to ioctl.\n", __FUNCTION__, __LINE__); 
        		ret = -1;
        		goto EXIT;
        	}
        }
    }
    
EXIT:
    if(sock >= 0)
        close(sock);
	return ret;
}

int rtl_link_unsilent_sta(unsigned char *mac)
{
    int i, j, sock, ret=0;
    char if_name[16];
    struct iwreq wrq;
    unsigned char mac_str[13];

    sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    DEBUG("(%s)%d UNSILENT STA:%s !\n", __FUNCTION__, __LINE__, mac_str);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { 
        ERR_PRINT("(%s)%d Error Creating Socket for ioctl.\n", __FUNCTION__, __LINE__); 
        ret = -1;
        goto EXIT;
    }

    memset(&wrq, 0, sizeof(wrq));
    
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        for(j=0; j<=NUM_VWLAN; j++)
        {
            if(j==0)
                sprintf(if_name, "wlan%d", i);
            else
                sprintf(if_name, "wlan%d-va%d", i, j-1);

            strncpy(wrq.ifr_name, if_name, IFNAMSIZ);
            wrq.u.data.pointer = (caddr_t)mac_str;
            wrq.u.data.length = 18;

            if(ioctl(sock, RTL8192CD_IOCTL_UNSILENT_STA, &wrq) < 0) { 
                ERR_PRINT("(%s)%d Error to ioctl.\n", __FUNCTION__, __LINE__); 
                ret = -1;
                goto EXIT;
            }
        }
    }
    
EXIT:
    if(sock >= 0)
        close(sock);
    return ret;
}

int rtl_link_get_port_speed(int port_num, unsigned int *up_speed, unsigned int *down_speed)
{
	if(port_num<0 || port_num>4 || up_speed==NULL || down_speed==NULL)
		return RTL_LINK_FAILED;
	
	int tmp_port=0;
	char buffer[64]={0};
	FILE *fp=NULL;
	if((fp = fopen(ETH_PORT_INFO,"r")) == NULL)
		return RTL_LINK_FAILED;	

	while(fgets(buffer, sizeof(buffer), fp)) {
		sscanf(buffer, "port_num=%d,up_speed=%u,down_speed=%u",&tmp_port, up_speed, down_speed);
		if(tmp_port==port_num)
			break;
	}

	//printf("\n%s:%d port_num=%d up_speed=%u down_speed=%u\n",__FUNCTION__,__LINE__,port_num,*up_speed,*down_speed);
	fclose(fp);
	return RTL_LINK_SUCCESS;		
}

int rtl_link_get_wan_dhcp()
{
	int wan_dhcp=0;

	apmib_get(MIB_WAN_DHCP, (void *)&wan_dhcp);

	return wan_dhcp;
}

int rtl_link_get_pppoe_info(PppoeInfo *info)
{
	int wan_dhcp=0;
	
	if(!info){
		printf("[%s]info NULL\n", __FUNCTION__);
		return -1;
	}

	wan_dhcp = rtl_link_get_wan_dhcp();
	if(wan_dhcp==3){
		apmib_get(MIB_PPP_USER_NAME, (void *)info->username);
		apmib_get(MIB_PPP_PASSWORD, (void *)info->password);
	}

	return 0;
}

int rtl_link_is_pppoe_connect()
{
	RTK_WAN_INFO info;
	int ret = 0;

	memset(&info, 0, sizeof(RTK_WAN_INFO));
	if(rtl_link_get_wan_dhcp()==3){
		rtk_get_wan_info(&info);
		if(info.status == PPPOE_CONNECTED){
			ret = 1;
			DEBUG("(%s)%d PPPOE connect\n", __FUNCTION__, __LINE__);
		}else
			DEBUG("(%s)%d PPPOE disconnect\n", __FUNCTION__, __LINE__);
	}
	else{
		DEBUG("(%s)%d Not PPPOE Mode\n", __FUNCTION__, __LINE__);
	}
	
	return ret;
}

int rtl_link_parse_ping_file(char *filename)
{
	int ret=0;
	FILE *fp=NULL;
	char buffer[200]={0};

	if((fp = fopen(filename,"r")) == NULL)
		return ret;	

	/*
		PING www.baidu.com (180.97.33.107): 56 data bytes
		64 bytes from 180.97.33.107: seq=0 ttl=55 time=6.469 ms

		--- www.baidu.com ping statistics ---
		1 packets transmitted, 1 packets received, 0% packet loss
	*/

	while(fgets(buffer, sizeof(buffer), fp)) {
		if(strstr(buffer, "1 packets transmitted, 1 packets received")){
			ret = 1;
			break;
		}		
	}
	fclose(fp);
	unlink(filename);

	return ret;
}

int rtl_link_checkWanping(char *server1, char *server2, char *server3)
{
	int ret=0;
	char buffer[128]={0};
	
	snprintf(buffer,sizeof(buffer),"ping %s -c 1 2> %s > %s",server1, RTL_LINK_CHK_PING_FILE, RTL_LINK_CHK_PING_FILE);	
	ret=system(buffer);	
	//ret is always -1 when system done
	if(ret){
		if(!rtl_link_parse_ping_file(RTL_LINK_CHK_PING_FILE))
			return 0;
	}			

	snprintf(buffer,sizeof(buffer),"ping %s -c 1 2> %s > %s",server2, RTL_LINK_CHK_PING_FILE, RTL_LINK_CHK_PING_FILE);	
	ret=system(buffer);
	//ret is always -1 when system done
	if(ret){
		if(!rtl_link_parse_ping_file(RTL_LINK_CHK_PING_FILE))
			return 0;
	}

	snprintf(buffer,sizeof(buffer),"ping %s -c 1 2> %s > %s",server3, RTL_LINK_CHK_PING_FILE, RTL_LINK_CHK_PING_FILE);
	ret=system(buffer);
	//ret is always -1 when system done
	if(ret){
		if(!rtl_link_parse_ping_file(RTL_LINK_CHK_PING_FILE))
			return 0;
	}
	
	return 1;
}

int rtl_link_wan_access()
{
	int ret = 0, max_times=1, i;

	for(i=0; i<max_times; ++i){
		if(rtl_link_checkWanping("www.baidu.com","www.sohu.com","www.163.com")==1){
			ret = 1;
			break;
		}else{
			//sleep(1);
		}
	}

	return ret;
}

int rtl_link_get_ip_by_domain(char *domain, char *ip)
{
	char *pchar;
	char tmp_str[200]={0};
	int index=1, i, j, cnt=3, ret=1;
	struct hostent *hp;

	strncpy(tmp_str, domain, sizeof(tmp_str));
	//eg: https://cgw.komect.com
	if(strstr(tmp_str, "//")){  
		pchar = strtok(tmp_str, "//");		
		while(pchar){
			if(index == 1){
				pchar = strtok(NULL, "//");
				index++;				
			}else{
				strcpy(tmp_str, pchar);				
				pchar = strtok(NULL, ":"); //end while
			}
		}
	}

	for(j=0; j<cnt; ++j)
	{
		hp = NULL;
		i=0;
		hp = gethostbyname(tmp_str);	
        if(hp != NULL){
            while(hp->h_addr_list[i] != NULL){
                strcpy(ip, inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
                i++;
            }
			ret = 0;
			printf("get ip [%s] by [%s] success !\n", ip, tmp_str);
			break;
        }else{
            printf("resolve hostname \"%s\" fail !\n", tmp_str);				            
        }	
	}

	return ret;
}

