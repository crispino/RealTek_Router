#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>

#include "apmib.h"
#include "cJSON.h"

#include "coap.h"

#include "rtl_link.h"
#include "rtl_link_utils.h"
#include "rtl_link_list.h"
#include "andlink_interface.h"
#include "andlink.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.2/dmalloc.h"
#endif

int debug_mode = 2;
int roaming_debug_on = 0;
int version;
Andlink_basicInfo basic_info;
PHYBAND_TYPE_T qlink_band = PHYBAND_2G;
extern char *optarg;

// for andlink IF3, IF5, IF6 coexist
#define ANDLINK_IF356_COEXIST	1

static void Andlink_macFormat(char *dest, char *src)
{
    int i=0, j=0;

    while(i < strlen(src))
    {
        if(src[i] != ':')
            dest[j++] = src[i];
        i++;
    }
}

ERR_NUM_T Andlink_isLanDev(MacNode *node)
{
	MACFILTER_T entry, entry_tmp;
	ERR_NUM_T err = SUCCESS;
	char str_mac[32] = {0}, str_name[RTL_LINK_MAX_INFO_LEN] = {0};
	int entryNum = 0, i = 0, k, l, ret;
	List lan_sta_list, wlan_sta_list;
	STAInfo *lan_sta, *wlan_sta;
	char delete_delim_mac[13]={0};

	list_init(&lan_sta_list);
	ret = rtl_link_getLanDevInfo(&lan_sta_list);
	if(ret==-1){
		ERR_PRINT("ERROR (%s)%d error getLanDevInfo\n", __FUNCTION__, __LINE__);
		err = ERROR;
		goto EXIT;
	}

	list_init(&wlan_sta_list);
    ret = rtl_link_getWlanSTAInfo(&wlan_sta_list);
    if(ret==-1){
        ERR_PRINT("ERROR (%s)%d error getWlanSTAInfo\n", __FUNCTION__, __LINE__);
        err = ERROR;
        goto EXIT;
    }

	node->is_lan_sta = 2;
	//look up in MACFILTER TBL
	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
	for(i = 1; i <= entryNum; i++) {
		memset(&entry, 0x00, sizeof(entry));
		*((char *)&entry) = (char)i;
		apmib_get(MIB_MACFILTER_TBL, (void *)&entry);
		
		snprintf(str_mac, 32, "%02x%02x%02x%02X%02x%02x", 
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2], entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		
		if (!strncasecmp(node->mac, str_mac, sizeof(node->mac))){
			//get a match, 
			DEBUG("ANDLINK_IF5 (%s)%d sta %s is lan sta\n", __FUNCTION__, __LINE__,node->mac);
			node->is_lan_sta = 1;
			goto EXIT;
		}
	}

	//look up in MACAC TBL
	apmib_save_wlanIdx();
	for(k=0; k<NUM_WLAN_INTERFACE; k++){
		wlan_idx = k;
		for(l=0; l<=NUM_VWLAN; l++){
			vwlan_idx = l;

			apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum);
			for(i = 1; i <= entryNum; i++) {
				memset(&entry, 0x00, sizeof(entry));
				*((char *)&entry) = (char)i;
				apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&entry);

				snprintf(str_mac, 32, "%02x%02x%02x%02X%02x%02x", 
					entry.macAddr[0], entry.macAddr[1], entry.macAddr[2], entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
				if (!strncasecmp(node->mac, str_mac, sizeof(node->mac)))
				{
					//get a match
					DEBUG("ANDLINK_IF5 (%s)%d sta %s is wlan sta\n", __FUNCTION__, __LINE__,node->mac);
					node->is_lan_sta = 0;
					goto EXIT;
				}
			}
		}
	}
    apmib_recov_wlanIdx();

	//look up in lan_sta_list
	for(i=0; i<lan_sta_list.len; i++){
		lan_sta = list_get_element(&lan_sta_list, i);
		Andlink_macFormat(delete_delim_mac, lan_sta->mac);
		if(!strncasecmp(delete_delim_mac, node->mac,sizeof(delete_delim_mac))){
			DEBUG("ANDLINK_IF5 (%s)%d sta %s is lan sta\n", __FUNCTION__, __LINE__,node->mac);
			node->is_lan_sta = 1;
			goto EXIT;
		}
	}

	//look up in wlan_sta_list
	for(i=0; i<wlan_sta_list.len; i++){
		wlan_sta = list_get_element(&wlan_sta_list, i);
		Andlink_macFormat(delete_delim_mac, wlan_sta->mac);
		if(!strncasecmp(delete_delim_mac, node->mac,sizeof(delete_delim_mac))){
			DEBUG("ANDLINK_IF5 (%s)%d sta %s is wlan sta\n", __FUNCTION__, __LINE__,node->mac);
			node->is_lan_sta = 0;
			goto EXIT;
		}
	}
	
EXIT:
	list_destroy(&lan_sta_list, free);
	list_destroy(&wlan_sta_list, free);
	return err;  
}

void Andlink_macFormat_add_delim(char *dest, char *src)
{
    int i=0, j=0;
    while(i < strlen(src))
    {
    	if((i !=0) && (i%2 == 0))
    	{
    		
			dest[j++]=':';
			dest[j++]=src[i++];
		}
		else
			dest[j++]=src[i++];
    }
}

int Andlink_formSearchAP(cJSON *root)
{
    cJSON_AddStringToObject(root, "searchKey", "ANDLINK-AP");
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    cJSON_AddStringToObject(root, "andlinkVersion", "V3");
#else    
#endif
    return 0;
} 
int Andlink_formSearchGW(cJSON *root)
{
    cJSON_AddStringToObject(root, "searchKey", "ANDLINK-DEVICE");
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    cJSON_AddStringToObject(root, "andlinkVersion", "V3");
#else    
    cJSON_AddStringToObject(root, "andlinkVersion", "V2");
#endif
#if defined(CONFIG_ANDLINK_V3_SUPPORT) && !defined(ANDLINK_IF356_COEXIST)
    if(version == 0X310C){
        if(Andlink_isCloudGW() && basic_info.rtl_link_mode==RTL_LINK_BRIDGE){
            cJSON_AddStringToObject(root, "deviceType", basic_info.deviceType);
        }
    }
#endif
    return 0;
}

int Andlink_scanQlinkDefBssid(void)     //referto formWlSiteSurvey & wlSiteSurveyTbl(invoked by Pocket_sitesurvey.htm) 
{
    int i, j, ret=0, status, wait_time, idx_2g;
    PHYBAND_TYPE_T tmpband;
    unsigned char res;
    char ifname[16];
    SS_STATUS_Tp pStatus=NULL;

    //start led twinkle (use wps led)
    system("echo 2 > proc/gpio");

    qlink_band = PHYBAND_OFF;

    apmib_save_wlanIdx();

    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpband);
        if(tmpband == PHYBAND_2G)
            idx_2g = i;
        
        snprintf(ifname, sizeof(ifname), WLAN_IF, i);
        //SiteSurveyRequest
        wait_time = 0;
        while (1) {
            switch(getWlSiteSurveyRequest(ifname, &status)) 
    	    { 
    			case -2: 
    				ERR_PRINT("ERROR ANDLINK (%s)%d Auto scan running!!\n", __FUNCTION__, __LINE__); 
    				break; 
    			case -1: 
    				ERR_PRINT("ERROR ANDLINK (%s)%d Site-survey request failed!\n", __FUNCTION__, __LINE__); 
    				break; 
    			default: 
    				break; 
    		}
    		if (status != 0) {	// not ready
				if (wait_time++ > 15) {
				    ERR_PRINT("ERROR ANDLINK (%s)%d scan request timeout!\n", __FUNCTION__, __LINE__); 
					ret = -1;
					goto EXIT;
				}
#ifdef	CONFIG_RTK_MESH
		// ==== modified by GANTOE for site survey 2008/12/26 ==== 
				usleep(1000000 + (rand() % 2000000));
#else
				sleep(1);
#endif
			}
			else
				break;
		}

        //GetSiteSurveyResult
        //step1. wait to complete...
		wait_time = 0;
		while (1) {
			res = 1;	// only request request status
			if ( getWlSiteSurveyResult(ifname, (SS_STATUS_Tp)&res) < 0 ) {
			    ERR_PRINT("ERROR ANDLINK (%s)%d Read site-survey status failed!\n", __FUNCTION__, __LINE__);
				ret = -1;
				goto EXIT;
			}
			if (res == 0xff) {   // in progress
			#if (defined(CONFIG_RTL_92D_SUPPORT) && defined (CONFIG_POCKET_ROUTER_SUPPORT)) || defined(CONFIG_RTL_DFS_SUPPORT)
				/*prolong wait time due to scan both 2.4G and 5G */
				if (wait_time++ > 20) 
			#else
				if (wait_time++ > 20) 
			#endif		
			    {
			        ERR_PRINT("ERROR ANDLINK (%s)%d scan timeout!\n", __FUNCTION__, __LINE__); 
					ret = -1;
					goto EXIT;
				}
				sleep(1);
			}
			else
				break;
		}

        //step2. get result
		if (pStatus==NULL) {
    		pStatus = calloc(1, sizeof(SS_STATUS_T));
    		if ( pStatus == NULL ) {
    			ERR_PRINT("ERROR ANDLINK (%s)%d Allocate buffer failed!\n", __FUNCTION__, __LINE__);
    			ret = -1;
    			goto EXIT;
    		}
	    }

	    pStatus->number = 0; // request BSS DB

        if ( getWlSiteSurveyResult(ifname, pStatus) < 0 ) {
    		ERR_PRINT("ERROR ANDLINK (%s)%d Read site-survey status failed!\n", __FUNCTION__, __LINE__);
    		ret = -1;
    		goto EXIT;
    	}

		//ParseSiteSurveyResult
        for(j=0; j<pStatus->number; j++)
        {   
            //DEBUG("[%s][%d], %s\n", __FUNCTION__, __LINE__, pStatus->bssdb[j].bdSsIdBuf);
            if(!strcmp((char *)pStatus->bssdb[j].bdSsIdBuf, "CMCC-QLINK"))
            {
                if (pStatus->bssdb[j].bdCap & cPrivacy)
                {
                    if ((pStatus->bssdb[j].bdTstamp[0] & 0x0000ffff) && (((pStatus->bssdb[j].bdTstamp[0] & 0x0000f000) >> 12) == 0x4) &&    //wpa-psk
                            (pStatus->bssdb[j].bdTstamp[0] & 0xffff0000) && (((pStatus->bssdb[j].bdTstamp[0] & 0xf0000000) >> 28) == 0x4))  //wpa2-psk
                    {
                        DEBUG("ANDLINK (%s)%d scan CMCC-QLINK on band %s!\n", __FUNCTION__, __LINE__, (tmpband==PHYBAND_2G)?"2.4G":"5G");
                        qlink_band = tmpband;
                        ret |= 0x1 << i;
                        break;
                    }
                }
            }
        }
    }
EXIT:
    apmib_recov_wlanIdx();
    if(pStatus)
        free(pStatus); //sc_yang
    if(ret == 3){
        qlink_band = PHYBAND_2G;    //if scaned CMCC on both band do qlink only on 2.4G
        ret = 0x1 << idx_2g;
    }
    return ret;
}

void Andlink_setQlinkVXD(void)
{
    WiFiConfig param;
    int i, ret=0, tmpvalue;

    memset(&param, 0, sizeof(param));

    apmib_save_wlanIdx();

    if(version >= 0X210E){
        system("echo 2 > proc/gpio");   //start led twinkle (use wps led)
        for(i=0; i<NUM_WLAN_INTERFACE; i++)
        {
            wlan_idx = i;
            apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
            if(tmpvalue == PHYBAND_2G){
                param.ap[0].enable = 1;         
                param.ap[0].band = tmpvalue;
                param.ap[0].idx = 0;            //vxd just sync root ap's configurations
                param.ap[0].encrypt = ENCRYPT_DISABLED;
                snprintf(param.ap[0].ssid, sizeof(param.ap[0].ssid), "%s", "CMCC-QLINK");
                break;
            }
        }
        param.num = 1;
    }else{
        for(i=0; i<NUM_WLAN_INTERFACE; i++)
        {
            wlan_idx = i;
            apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
            if(tmpvalue == qlink_band){
                param.ap[0].enable = 1;
                param.ap[0].band = tmpvalue;
                param.ap[0].idx = 0;            //vxd just sync with root ap's configurations
                param.ap[0].encrypt = ENCRYPT_WPA2_MIXED;
                param.ap[0].wpa_auth = WPA_AUTH_PSK;    //by default
                param.ap[0].wpa_cypher = WPA_CIPHER_MIXED;  //by default
                param.ap[0].wpa_key_fmt = KEY_ASCII;
                snprintf(param.ap[0].ssid, sizeof(param.ap[0].ssid), "%s", "CMCC-QLINK");
                snprintf(param.ap[0].key, sizeof(param.ap[0].key), "%s", "sD5fH%9k");     //"%s" is neccessary
                break;
            }
        }
        param.num = 1;
    }
    apmib_recov_wlanIdx();

    ret = rtl_link_syncRepeaterConfig(param);
    Andlink_set_restart_flag(ret);
}

void Andlink_closeQlinkVXD()
{
    WiFiConfig param;
    int i, ret=0, tmpvalue;

    memset(&param, 0, sizeof(param));

    apmib_save_wlanIdx();
    
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
        if(tmpvalue == PHYBAND_2G){
            param.ap[0].enable = 0;         
            param.ap[0].band = tmpvalue;
            param.ap[0].idx = 0;            //vxd just sync root ap's configurations
            param.ap[0].encrypt = ENCRYPT_DISABLED;
            snprintf(param.ap[0].ssid, sizeof(param.ap[0].ssid), "%s", "CMCC-QLINK");
            break;
        }
    }
    param.num = 1;

    apmib_recov_wlanIdx();

    ret = rtl_link_syncRepeaterConfig(param);
    Andlink_set_restart_flag(ret);
}


#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
void Andlink_openQlinkVAP(void)
{
    WiFiConfig param;
    int i, ret=0, tmpvalue;

    memset(&param, 0, sizeof(param));

    apmib_save_wlanIdx();

    param.ap[0].band = PHYBAND_2G;  //only on band 2g
    param.ap[0].enable = 1;
    param.ap[0].idx = NUM_VWLAN;    //enable vap-max(vap-4)
    param.ap[0].encrypt = ENCRYPT_DISABLED;
    param.ap[0].broadcast_ssid = 1;
    
    snprintf(param.ap[0].ssid, sizeof(param.ap[0].ssid), "CMQLINK-%s-%s", basic_info.deviceType, &basic_info.wlan_mac_2g[8]);
    param.num = 1;

    apmib_recov_wlanIdx();
    
    system("killall udhcpc > /dev/null 2>&1");
    system("killall udhcpd > /dev/null 2>&1");
    start_dhcpd_with_ip(BR_IF, QLINK_DHCP_IP, QLINK_DHCP_START, QLINK_DHCP_END);

    ret = rtl_link_syncWiFiConfig(param);
    Andlink_set_restart_flag(ret);
}

void Andlink_closeQlinkVAP(void)
{
    WiFiConfig param;
    int i, ret=0, tmpvalue;

    memset(&param, 0, sizeof(param));

    apmib_save_wlanIdx();

    param.ap[0].band = PHYBAND_2G;  //only on band 2g
    param.ap[0].enable = 0;
    param.ap[0].idx = NUM_VWLAN;    //disable vap-max(vap-4)
    param.ap[0].encrypt = ENCRYPT_DISABLED;
    param.ap[0].broadcast_ssid = 1;
    snprintf(param.ap[0].ssid, sizeof(param.ap[0].ssid), "CMQLINK-%s-%s", basic_info.deviceType, &basic_info.wlan_mac_2g[8]);
    param.num = 1;

    apmib_recov_wlanIdx();

    system("killall udhcpc > /dev/null 2>&1");
    system("killall udhcpd > /dev/null 2>&1");
    set_lan_dhcpc("br0");

    ret = rtl_link_syncWiFiConfig(param);
    
    Andlink_set_restart_flag(ret);
}
#endif

int Andlink_connectWireless(void)
{
    int i, connect_2g=0, connect_5g=0, tmpband, active_stanum;
    char filename[64], line_buffer[256], ssid[33], peer_mac[6], empty_mac[6]={0};
    char *pchar;
    FILE *fp;
    
    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++)
    {
        wlan_idx = i;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpband);
        
        snprintf(filename, sizeof(filename), "/proc/wlan%d-vxd/sta_info", i);
        if((fp= fopen(filename, "r"))==NULL){
            ERR_PRINT("ERROR (%s)%d fopen %s error\n", __FUNCTION__, __LINE__, filename);
            return -1;
        }
        fgets(line_buffer, sizeof(line_buffer), fp);
        line_buffer[strlen(line_buffer)-1]='\0';
        if(strstr(line_buffer, "-- STA info table")){
            sscanf(line_buffer,"-- STA info table -- (active: %u)",&active_stanum);
            if(active_stanum){
                if(tmpband == PHYBAND_2G){
                    DEBUG("ANDLINK (%s)%d 2G:wlan%d-vxd connected!\n", __FUNCTION__, __LINE__, i);
                    connect_2g = 1;
                }else{
                    DEBUG("ANDLINK (%s)%d 5G:wlan%d-vxd connected!\n", __FUNCTION__, __LINE__, i);
                    connect_5g = 1;
                }

                vwlan_idx = NUM_VWLAN_INTERFACE;
                apmib_get(MIB_WLAN_SSID, (void *)ssid);
                if(!strcmp(ssid, "CMCC-QLINK")){
                    apmib_get(MIB_WLAN_PREFER_BSSID, (void *)peer_mac);
                    if(!memcmp(peer_mac, empty_mac, 6)){
                        while(fgets(line_buffer, sizeof(line_buffer), fp))
                        {
                            line_buffer[strlen(line_buffer)-1]='\0';
                            if((pchar=strstr(line_buffer, "hwaddr: "))!=NULL)
                		        {
                		            pchar += strlen("hwaddr: ");
                		            _string_to_hex(pchar, peer_mac, 12);
                		            break;
                		        }
                        }
                        apmib_set(MIB_WLAN_PREFER_BSSID, (void *)peer_mac);
                        apmib_update(CURRENT_SETTING);
                    }
                }
                    
            }
        }
        fclose(fp);
        fp = NULL;
    }
    apmib_recov_wlanIdx();

   if(connect_2g || connect_5g)
        return 1;
    else
        return 0;//qlink_band???
}

int Andlink_formQlinkReq(cJSON * data, Andlink_basicInfo *info)
{
    cJSON_AddStringToObject(data, "deviceMac", info->wlan_mac_2g);
    cJSON_AddStringToObject(data, "deviceType", info->deviceType);
    return 0;
}

int Andlink_formQlinkSuccess(cJSON * data, Andlink_basicInfo *info)
{
    //stop led twinkle (use wps led)
    system("echo 0 > proc/gpio");

    cJSON_AddStringToObject(data, "deviceMac", info->wlan_mac_2g);
    cJSON_AddStringToObject(data, "deviceType", info->deviceType);
    return 0;
}

ERR_NUM_T Andlink_postGWSearchAck(cJSON *json)
{
    cJSON *item, *subitem;

    item = cJSON_GetObjectItem(json, "searchAck");
    if(!item){
        ERR_PRINT("ERROR ANDLINK(%s)%d miss searchAck item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }
    if(strcmp(item->valuestring, "ANDLINK-GW")){
        ERR_PRINT("ERROR ANDLINK(%s)%d searchAck: %s invalid\n", __FUNCTION__, __LINE__, item->valuestring);
        return AUTHORIZATION_FAILED;
    }
    item = cJSON_GetObjectItem(json, "andlinkVersion");
    if(!item){
        ERR_PRINT("ERROR ANDLINK(%s)%d miss andlinkVersion item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    if(strcmp(item->valuestring, "V3")){
        ERR_PRINT("ERROR ANDLINK(%s)%d andlinkVersion: %s invalid\n", __FUNCTION__, __LINE__, item->valuestring);
        //return AUTHORIZATION_FAILED;
    }
    item = cJSON_GetObjectItem(json, "remoteGw");
    if(item){
        subitem = cJSON_GetObjectItem(item, "user_key");
        if(!subitem){
            ERR_PRINT("ERROR ANDLINK(%s)%d miss user_key item\n", __FUNCTION__, __LINE__);
            return LACK_OF_ARGUMENT;
        }

        subitem = cJSON_GetObjectItem(item, "gwAddress");
        if(!subitem){
            ERR_PRINT("ERROR ANDLINK(%s)%d miss gwAddress item\n", __FUNCTION__, __LINE__);
            return LACK_OF_ARGUMENT;
        }
    }
#else
    if(strcmp(item->valuestring, "V2")){
        ERR_PRINT("ERROR ANDLINK(%s)%d andlinkVersion: %s invalid\n", __FUNCTION__, __LINE__, item->valuestring);
        return AUTHORIZATION_FAILED;
    }
#endif
    return SUCCESS;
}


ERR_NUM_T Andlink_postQlinkInfo(cJSON *json)
{
    WiFiConfig param;
    int i,tmpvalue, ret=0;
    cJSON *ssid, *key, *auth, *remoteGw, *item;

    memset(&param, 0, sizeof(param));
	

    ssid = cJSON_GetObjectItem(json, "SSID");
    if(!ssid){
        ERR_PRINT("WARN ANDLINK(%s)%d miss ssid item\n", __FUNCTION__, __LINE__);
        //return LACK_OF_ARGUMENT;
    }

    key = cJSON_GetObjectItem(json, "password");
    if(!key){
        ERR_PRINT("WARN ANDLINK(%s)%d miss password item\n", __FUNCTION__, __LINE__);
        //return LACK_OF_ARGUMENT;
    }

    auth = cJSON_GetObjectItem(json, "encrypt");
    if(!auth){
        ERR_PRINT("WARN ANDLINK(%s)%d miss encrypt item\n", __FUNCTION__, __LINE__);
        //return LACK_OF_ARGUMENT;
    }
	// TODO: CHANNEL
	
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    if(basic_info.version == 0X310C)
        remoteGw = cJSON_GetObjectItem(json, "CGW");
    else
        remoteGw = cJSON_GetObjectItem(json, "remoteGw");
    if(remoteGw){
        item = cJSON_GetObjectItem(remoteGw, "user_key");
        if(!item){
            ERR_PRINT("ERROR ANDLINK(%s)%d miss user_key item\n", __FUNCTION__, __LINE__);
            return LACK_OF_ARGUMENT;
        }

        item = cJSON_GetObjectItem(remoteGw, "gwAddress");
        if(!item){
            ERR_PRINT("ERROR ANDLINK(%s)%d miss gwAddress item\n", __FUNCTION__, __LINE__);
            return LACK_OF_ARGUMENT;
        }
    }
#endif
	if(ssid && key && auth){
	    apmib_save_wlanIdx();
	    for(i=0; i<NUM_WLAN_INTERFACE; i++)
	    {
	        wlan_idx = i;
	        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&tmpvalue);
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST) 
	        if(tmpvalue == PHYBAND_2G)
#else
	        if(tmpvalue == qlink_band)
#endif 
	        {
	            param.ap[0].enable = 1;
	            param.ap[0].band = tmpvalue;
	            param.ap[0].idx = 0;            //vxd just sync with root ap's configurations
	            param.ap[0].broadcast_ssid = 1;
	            snprintf(param.ap[0].ssid, sizeof(param.ap[0].ssid), "%s", ssid->valuestring);    //"%s" should not bypass

	            if(!strcmp(auth->valuestring, "None") || !strcmp(auth->valuestring, "OPEN")){
	                param.ap[0].encrypt = ENCRYPT_DISABLED;
	                
	            }else if(!strcmp(auth->valuestring, "WEP")){
	                param.ap[0].encrypt = ENCRYPT_WEP;
	                param.ap[0].wep_type = WEP64;       //bydefault
	                snprintf(param.ap[0].key, sizeof(param.ap[0].key), "%s", key->valuestring);
	                
	            }else if(!strcmp(auth->valuestring, "WPAPSK")){
	                param.ap[0].encrypt = ENCRYPT_WPA;
	                param.ap[0].wpa_auth = WPA_AUTH_PSK;    //by default
	                param.ap[0].wpa_key_fmt = KEY_ASCII;    //by default
	                param.ap[0].wpa_cypher = WPA_CIPHER_MIXED;  //by default
	                snprintf(param.ap[0].key, sizeof(param.ap[0].key), "%s", key->valuestring);
	                
	            }else if(!strcmp(auth->valuestring, "WPAPSK2")){
	                param.ap[0].encrypt = ENCRYPT_WPA2;
	                param.ap[0].wpa_auth = WPA_AUTH_PSK;    //by default
	                param.ap[0].wpa_key_fmt = KEY_ASCII;    //by default
	                param.ap[0].wpa_cypher = WPA_CIPHER_MIXED;  //by default
	                snprintf(param.ap[0].key, sizeof(param.ap[0].key), "%s", key->valuestring);

	            }else{
	                param.ap[0].encrypt = ENCRYPT_WPA2_MIXED;
	                param.ap[0].wpa_auth = WPA_AUTH_PSK;    //by default
	                param.ap[0].wpa_key_fmt = KEY_ASCII;    //by default
	                param.ap[0].wpa_cypher = WPA_CIPHER_MIXED;  //by default
	                snprintf(param.ap[0].key, sizeof(param.ap[0].key), "%s", key->valuestring);
	            } 
	        }
	    }
	    apmib_recov_wlanIdx();

	    param.num = 1;

	    ret |= rtl_link_syncRepeaterConfig(param);

	    Andlink_set_restart_flag(ret);
	}
        
    return SUCCESS;
}

int Andlink_formBootstrap(cJSON * root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *XData;
    LoginInfo login;

    time(&tm);
    
    if(version >= 0X210E){
        // 2.10e以上版本 注册
        if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
            cJSON_AddStringToObject(root, "deviceMac", info->lan_mac);
        else
            cJSON_AddStringToObject(root, "deviceMac", info->wlan_mac_2g);
        cJSON_AddStringToObject(root, "deviceType", info->deviceType);
        cJSON_AddStringToObject(root, "productToken", info->productToken);
        cJSON_AddNumberToObject(root, "timestamp", tm);
    }else{
        //2.10b 注册
        memset(&login, 0, sizeof(login));
        if(rtl_link_getLoginInfo(&login)){
            ERR_PRINT("ERROR ANDLINK(%s)%d error\n", __FUNCTION__, __LINE__);
            return -1;
        }
        if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
            cJSON_AddStringToObject(root, "deviceMac", info->lan_mac);
        else
            cJSON_AddStringToObject(root, "deviceMac", info->wlan_mac_2g);
        cJSON_AddStringToObject(root, "deviceType", info->deviceType);
        cJSON_AddStringToObject(root, "firmwareVersion", info->hwversion);
        cJSON_AddStringToObject(root, "softwareVersion", info->swversion);
        cJSON_AddStringToObject(root, "ipAddress", login.localIp);
        cJSON_AddNumberToObject(root, "timestamp", tm);

        XData = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "XData", XData);
        cJSON_AddStringToObject(XData, "deviceVendor", info->vendor);
        cJSON_AddStringToObject(XData, "deviceSn", info->sn);
        if(login.uptype == ETHERNET)
            cJSON_AddStringToObject(XData, "apUplinkType", "Ethernet");
        else if(login.uptype == WIRELESS)
            cJSON_AddStringToObject(XData, "apUplinkType", "Wireless");
        if(login.band == DUALBAND || login.band == BAND5G)
            cJSON_AddNumberToObject(XData, "radio5", 1);
        else
            cJSON_AddNumberToObject(XData, "radio5", 0);
        cJSON_AddStringToObject(XData, "SyncCode", info->synccode);
    }
}

int Andlink_formBoot(cJSON * root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *XData;
    LoginInfo login;
#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    char delim_mac[18]={0};
#endif
	PppoeInfo pppoeInfo;
	int workmode;
	SysMemInfo meminfo;

    time(&tm);	
	
    memset(&login, 0, sizeof(login));
    if(rtl_link_getLoginInfo(&login)){
        ERR_PRINT("ERROR ANDLINK(%s)%d error\n", __FUNCTION__, __LINE__);
        return -1;
    }

	memset(&pppoeInfo, 0, sizeof(PppoeInfo));
	rtl_link_get_pppoe_info(&pppoeInfo);
	rtl_link_getWorkMode(&workmode);
	memset(&meminfo, 0, sizeof(SysMemInfo));
	rtl_link_getSysMemInfo(&meminfo);
	
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    if(!Andlink_is_IF6())
        cJSON_AddStringToObject(root, "deviceId", info->deviceId);
#else
    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
#endif
    cJSON_AddStringToObject(root, "deviceType", info->deviceType);
 
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
	 if(Andlink_is_IF6()){ 
		cJSON_AddStringToObject(root, "deviceMac", info->lan_mac); // if6 mac should be the same for gw, bridge and repeater 
	 }else
#endif
    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
		cJSON_AddStringToObject(root, "deviceMac", info->lan_mac);
    else
		cJSON_AddStringToObject(root, "deviceMac", info->wlan_mac_2g);

    cJSON_AddStringToObject(root, "firmwareVersion", info->hwversion);
    cJSON_AddStringToObject(root, "softwareVersion", info->swversion);	
	cJSON_AddStringToObject(root, "PPPOEuser", pppoeInfo.username);
	cJSON_AddNumberToObject(root, "WorkingMode", workmode);		
    cJSON_AddStringToObject(root, "ipAddress", login.localIp);
    cJSON_AddNumberToObject(root, "timestamp", tm);

    XData = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "XData", XData);
    cJSON_AddStringToObject(XData, "deviceVendor", info->vendor);
    if(version >= 0X210E){
	    cJSON_AddStringToObject(XData, "deviceModel", info->model);
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
		 if(Andlink_is_IF6()){ 
			cJSON_AddStringToObject(root, "deviceMac", info->lan_mac); // if6 mac should be the same for gw, bridge and repeater 
		 }else
#endif
        if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
            cJSON_AddStringToObject(XData, "deviceMac", info->lan_mac);
        else
            cJSON_AddStringToObject(XData, "deviceMac", info->wlan_mac_2g); 
	}
    cJSON_AddStringToObject(XData, "deviceSn", info->sn);
	cJSON_AddNumberToObject(XData, "FlashSize", meminfo.flashSize);
	cJSON_AddNumberToObject(XData, "RamSize", meminfo.ramSize);	
    if(login.uptype == ETHERNET)
        cJSON_AddStringToObject(XData, "apUplinkType", "Ethernet");
    else if(login.uptype == WIRELESS)
        cJSON_AddStringToObject(XData, "apUplinkType", "Wireless");
    if(login.band == DUALBAND || login.band == BAND5G)
        cJSON_AddNumberToObject(XData, "radio5", 1);
    else
        cJSON_AddNumberToObject(XData, "radio5", 0);
#ifdef CONFIG_ANDLINK_IF6_SUPPORT         
    //cJSON_AddStringToObject(XData, "user key", info->user_key);
#endif
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
	 if(!Andlink_is_IF6()){ 
		cJSON_AddStringToObject(XData, "SyncCode", info->synccode); 
	 }else
#endif
    	cJSON_AddStringToObject(XData, "SyncCode", info->synccode);

    return 0;
}

int Andlink_formonlineInform(cJSON *root, Andlink_basicInfo *info)
{
	time_t tm;
    cJSON *data;
    int tmpvalue=0;
	LoginInfo login;
	char delim_mac[18]={0};
	
	if(rtl_link_getLoginInfo(&login)){
		ERR_PRINT("ERROR ANDLINK(%s)%d error\n", __FUNCTION__, __LINE__);
		return -1;
	}
#if defined(CONFIG_ANDLINK_IF6_SUPPORT)
	if(!Andlink_is_IF6())
        cJSON_AddStringToObject(root, "deviceId", info->deviceId);
	else{
		cJSON_AddStringToObject(root, "RPCMethod", "data");
		if(Andlink_is_IF6() || info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
			cJSON_AddStringToObject(root, "MAC", info->lan_mac);
		else
			cJSON_AddStringToObject(root, "MAC", info->wlan_mac_2g);
	}
#else
    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
#endif
	cJSON_AddStringToObject(root, "eventType", "EVENT_DEVICE_ONLINE");
    cJSON_AddNumberToObject(root, "timestamp", tm);
	
    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "deviceType", info->deviceType);
    cJSON_AddStringToObject(data, "firmwareVersion", info->hwversion);
    cJSON_AddStringToObject(data, "softwareVersion", info->swversion);
    cJSON_AddStringToObject(data, "ipAddress", login.localIp);
	cJSON_AddStringToObject(data, "deviceVendor", info->vendor);
    cJSON_AddStringToObject(data, "deviceModel", info->model);
    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
		cJSON_AddStringToObject(data, "deviceMac", info->lan_mac);
    else
		cJSON_AddStringToObject(data, "deviceMac", info->wlan_mac_2g);
    cJSON_AddStringToObject(data, "deviceSn", info->sn);
	if(login.uptype == ETHERNET)
        cJSON_AddStringToObject(data, "apUplinkType", "Ethernet");
    else if(login.uptype == WIRELESS)
        cJSON_AddStringToObject(data, "apUplinkType", "Wireless");
    if(login.band == DUALBAND || login.band == BAND5G)
        cJSON_AddNumberToObject(data, "radio5", 1);
    else
        cJSON_AddNumberToObject(data, "radio5", 0);
    cJSON_AddStringToObject(data, "SyncCode", info->synccode);
    
    return 0;
}

int Andlink_formReonlineInform(cJSON *root, Andlink_basicInfo *info)
{
	time_t tm;
    cJSON *data;
    int tmpvalue=0;
	char localIp[16]={0};

	rtl_link_getLocalIp(localIp);

#if defined(CONFIG_ANDLINK_IF6_SUPPORT)
	if(!Andlink_is_IF6())
    	cJSON_AddStringToObject(root, "deviceId", info->deviceId);
	else{
		cJSON_AddStringToObject(root, "RPCMethod", "data");
		if(Andlink_is_IF6() || info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
			cJSON_AddStringToObject(root, "MAC", info->lan_mac);
		else
			cJSON_AddStringToObject(root, "MAC", info->wlan_mac_2g);
	}
#else
	cJSON_AddStringToObject(root, "deviceId", info->deviceId);
#endif
	cJSON_AddStringToObject(root, "eventType", "EVENT_DEVICE_REONLINE");
    cJSON_AddNumberToObject(root, "timestamp", tm);
	
    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "ipAddress",localIp);
	
    return 0;
}

int Andlink_formUpgradeRelevantInform(cJSON *root, Andlink_basicInfo *info, int respCode)
{
    time_t tm;
    cJSON *data;
    int tmpvalue=0;
	char tmpstr[100]={0};
    
    time(&tm);
	switch(respCode){
		case FILE_TRANSFER_COMPLETE:    //download ok
			snprintf(tmpstr, sizeof(tmpstr), "%s", "File Transfer Complete");
			break;
		case FILE_TRANSFER_INTERRUPT:	//download fail
			snprintf(tmpstr, sizeof(tmpstr), "%s", "File Transfer interrupt");
			break;
		case FILE_UPGRADE_SUCCESS:		//upgrade success
			snprintf(tmpstr, sizeof(tmpstr), "%s", "File Upgrade Success");
			break;
		case SUCCESS:					//success inform
			snprintf(tmpstr, sizeof(tmpstr), "%s", "SUCCESS");
			break;
		case UNKNOW_ERROR:				//unknown error(eg:fw checksum invalid)
			snprintf(tmpstr, sizeof(tmpstr), "%s", "Unknow Error");
			break;
		case INVALID_VALUE:				//fw format invalid
			snprintf(tmpstr, sizeof(tmpstr), "%s", "Invalid value");
			break;
		default:
			ERR_PRINT("ERROR ANDLINK(%s)%d not support inform type %d\n", __FUNCTION__, __LINE__, respCode);
			break;
	}
	
#if defined(CONFIG_ANDLINK_IF6_SUPPORT)
	if(!Andlink_is_IF6())
    	cJSON_AddStringToObject(root, "deviceId", info->deviceId);
	else{
		cJSON_AddStringToObject(root, "RPCMethod", "data");
		if(Andlink_is_IF6() || info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
			cJSON_AddStringToObject(root, "MAC", info->lan_mac);
		else
			cJSON_AddStringToObject(root, "MAC", info->wlan_mac_2g);
	}
#else
	cJSON_AddStringToObject(root, "deviceId", info->deviceId);
#endif

#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
	cJSON_AddStringToObject(root, "eventType", "FILE");
#else
    cJSON_AddStringToObject(root, "eventType", "file");
#endif

    cJSON_AddNumberToObject(root, "timestamp", tm);

    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(data, "respCode", respCode);
    cJSON_AddStringToObject(data, "respCont", tmpstr);

	if(respCode==FILE_UPGRADE_SUCCESS){
		apmib_set(MIB_RTL_LINK_FW_UPGRADE, (void *)&tmpvalue);
    	apmib_update(CURRENT_SETTING);
	}
	
    return 0;
}

//int Andlink_formFwDownloadSuccessInform(cJSON *root, Andlink_basicInfo *info)
//int Andlink_formFwDownloadFailInform(cJSON *root, Andlink_basicInfo *info)
//int Andlink_formFwCheckFailInform(cJSON *root, Andlink_basicInfo *info)

void Andlink_storeServerIP(char *server, Andlink_basicInfo *info)
{
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    char tmp_ip[16]={0};
    char *pchar;
    int udp_port;
    struct in_addr ip;

    pchar = strstr(server, ":");
    strncpy(tmp_ip, server, pchar-server);
    if(strcmp(tmp_ip, info->gwIp_cloud_udp)){
        DEBUG("ANDLINK (%s)%d change serverIp from %s to %s!\n", __FUNCTION__, __LINE__, info->gwIp_cloud_udp, tmp_ip);
        sprintf(info->gwIp_cloud_udp, tmp_ip);
        inet_aton(info->gwIp_cloud_udp, &ip);
        apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ip);
    }

    udp_port = atoi(pchar+1);
    if(info->udp_port != udp_port){
        DEBUG("ANDLINK (%s)%d change UDP PORT from %d to %d!\n", __FUNCTION__, __LINE__, info->udp_port, udp_port);
        info->udp_port = udp_port;
        apmib_set(MIB_RTL_LINK_CGW_UDPPORT, (void *)&info->udp_port);
    }
#endif    
}

void Andlink_storeRegisterInfo(cJSON *root, Andlink_basicInfo *info)
{
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    cJSON *item;
    char server[32]={0};
    char tmp_ip[16]={0};
    char *pchar;
    int tcp_port;
    int HBTime;
    struct in_addr ip;

    item = cJSON_GetObjectItem(root, "heartBeatTime");
    if(item){
        HBTime = item->valueint/1000;
        if(info->HBTime != HBTime){
            info->HBTime = HBTime;
            apmib_set(MIB_RTL_LINK_HBTIME, (void *)&info->HBTime);
            DEBUG("ANDLINK (%s)%d store HBTime!\n", __FUNCTION__, __LINE__);
        }
    }
    
    item = cJSON_GetObjectItem(root, "MessageServer");
    if(item){
        snprintf(server, sizeof(server), item->valuestring);
        pchar = strstr(server, ":");
        strncpy(tmp_ip, server, pchar-server);
        if(strcmp(tmp_ip, info->gwIp_cloud_tcp)){
            DEBUG("ANDLINK (%s)%d change MessageServer from %s to %s!\n", __FUNCTION__, __LINE__, info->gwIp_cloud_tcp, tmp_ip);
            sprintf(info->gwIp_cloud_tcp, tmp_ip);
            inet_aton(info->gwIp_cloud_tcp, &ip);
            apmib_set(MIB_RTL_LINK_CGW_MS_IP_ADDR, (void *)&ip);
        }
        
        tcp_port = atoi(pchar+1);
        if(info->tcp_port != tcp_port){
            DEBUG("ANDLINK (%s)%d change TCP PORT from %d to %d!\n", __FUNCTION__, __LINE__, info->tcp_port, tcp_port);
            info->tcp_port = tcp_port;
            apmib_set(MIB_RTL_LINK_CGW_TCPPORT, (void *)&info->tcp_port);
        }
    }
#endif
}

void Andlink_storeHBRsp(cJSON *root, Andlink_basicInfo *info)
{
    cJSON *item;
    int HBTime;

    item = cJSON_GetObjectItem(root, "heartBeatTime");
    if(item){
        HBTime = item->valueint/1000;
        if(info->HBTime != HBTime){
            info->HBTime = HBTime;
            apmib_set(MIB_RTL_LINK_HBTIME, (void *)&info->HBTime);
            DEBUG("ANDLINK (%s)%d store HBTime!\n", __FUNCTION__, __LINE__);
        }
    }
}
void Andlink_setFWUpgradeMibToZero()
{
	int zero=0;
	apmib_set(MIB_RTL_LINK_FW_UPGRADE, (void *)&zero);
    apmib_update(CURRENT_SETTING);
}

void Andlink_setPreferBssidMibToZero()
{
	int i = 0;
	char empty_mac[6]={0};

	apmib_save_wlanIdx();

	for(i=0; i<NUM_WLAN_INTERFACE; ++i){
		wlan_idx = i;
		vwlan_idx = NUM_VWLAN_INTERFACE;
		apmib_set(MIB_WLAN_PREFER_BSSID, (void *)empty_mac);
	}

	apmib_recov_wlanIdx();
	
    apmib_update(CURRENT_SETTING);

	return;
}

void Andlink_setDefaultGWMac()
{
	unsigned char gw_mac[13]={0}, macAddr[6]={0};
	
	rtl_link_getGatewayMac(gw_mac);	
	_string_to_hex(gw_mac, macAddr, 12);

	DEBUG("ANDLINK (%s)%d set default gw mac:%02x%02x%02x%02x%02x%02x\n", __FUNCTION__, __LINE__,
		macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
	
	apmib_set(MIB_DEF_GW_MAC, (void *)macAddr);

	apmib_update(CURRENT_SETTING);

	return;
}

//int Andlink_formUpGradeInform(cJSON *root, Andlink_basicInfo *info)
//int Andlink_formSuccessInform(cJSON *root, Andlink_basicInfo *info)

int Andlink_storeLoginInfo(cJSON *root, Andlink_basicInfo *info)
{
    cJSON *item;
    time_t tm;

    item = cJSON_GetObjectItem(root, "andlinkToken");
    if(item){
        if(version >= 0X210F) {//2.10f
            if(!strcmp(info->andlinkToken, item->valuestring)){
                DEBUG("ANDLINK (%s)%d check andlinkToken OK!\n", __FUNCTION__, __LINE__);
                DEBUG("ANDLINK register to cloud platform...\n");
            }else if(!strcmp(item->valuestring, "0000000000000000")){
                strcpy(info->andlinkToken, item->valuestring);
                //apmib_set(MIB_RTL_LINK_ANDLINKTOKEN, (void *)info->andlinkToken);
                DEBUG("ANDLINK register to local GW...\n");
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d andlinkToken missmatch\n", __FUNCTION__, __LINE__);
               // return -1;
            }
        }else{
            if(strcmp(info->andlinkToken, item->valuestring)){
                ERR_PRINT("ERROR ANDLINK(%s)%d andlinkToken missmatch\n", __FUNCTION__, __LINE__);
                //return -1;
            }else{
                DEBUG("ANDLINK (%s)%d check andlinkToken OK!\n", __FUNCTION__, __LINE__);
            }
        }
    }

    item = cJSON_GetObjectItem(root, "deviceId");
    if(item){
        strcpy(info->deviceId, item->valuestring);
        apmib_set(MIB_RTL_LINK_DEVID, (void *)info->deviceId);
        DEBUG("ANDLINK (%s)%d store deviceId!\n", __FUNCTION__, __LINE__);
    }

    item = cJSON_GetObjectItem(root, "deviceToken");
    if(item){
        strcpy(info->deviceToken, item->valuestring);
        apmib_set(MIB_RTL_LINK_DEVTOKEN, (void *)info->deviceToken);
        DEBUG("ANDLINK (%s)%d store deviceToken!\n", __FUNCTION__, __LINE__);
    }

    item = cJSON_GetObjectItem(root, "heartBeatTime");
    if(item){
        info->HBTime = item->valueint/1000;
        apmib_set(MIB_RTL_LINK_HBTIME, (void *)&info->HBTime);
        DEBUG("ANDLINK (%s)%d store HBTime!\n", __FUNCTION__, __LINE__);
    }

    item = cJSON_GetObjectItem(root, "timestamp");
    if(item){
        tm = item->valueint;
        stime(&tm);
    }

    if(version >= 0X210E){
        item = cJSON_GetObjectItem(root, "gwToken");
        if(item){
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST) 
            if(Andlink_isCloudGW()){
                strcpy(info->gwToken_cloud, item->valuestring);
                apmib_set(MIB_RTL_LINK_CGWTOKEN, (void *)info->gwToken_cloud);
            }else{
                strcpy(info->gwToken_host, item->valuestring);
                apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)info->gwToken_host);
            }
#else
            strcpy(info->gwToken, item->valuestring);
            apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)info->gwToken);
#endif
            DEBUG("ANDLINK (%s)%d store gwToken!\n", __FUNCTION__, __LINE__);
        }
    }

    apmib_update(CURRENT_SETTING); 
      
    return 0;
}

int Andlink_formRoamingInfo(cJSON *root, Andlink_basicInfo *info, List *lowquality_list)
{
    int i, need_to_send=0;
    time_t tm;
    cJSON *data, *STA, *Node, *QOS;
    Andlink_lowqualitySta *cursta;
    char mac[18];

    time(&tm);

    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    cJSON_AddStringToObject(root, "eventType", "STA_LOWQUALITY");
    cJSON_AddNumberToObject(root, "timestamp", tm);

    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);

    STA = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "STA", STA);

    for(i=0; i<lowquality_list->len; i++){
        Node = cJSON_CreateObject();
        cJSON_AddItemToArray(STA, Node);

        cursta = list_get_element(lowquality_list, i);
        if(cursta->duration >= info->lowquality_check_time){
            snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", 
                cursta->mac[0], cursta->mac[1], cursta->mac[2], cursta->mac[3], cursta->mac[4], cursta->mac[5]);
            cJSON_AddStringToObject(Node, "MacAddress", mac);
            if(cursta->wlanid == info->idx_2g)
                cJSON_AddStringToObject(Node, "Radio", "2.4G");
            else
                cJSON_AddStringToObject(Node, "Radio", "5G");
            cJSON_AddNumberToObject(Node, "RSSI", cursta->rssi-100);
            cJSON_AddNumberToObject(Node, "Channel", cursta->channel);
            if(cursta->bandwith == 0)
                cJSON_AddStringToObject(Node, "FrequencyWidth", "20M");
            else if(cursta->bandwith == 1)
                cJSON_AddStringToObject(Node, "FrequencyWidth", "40M");
            else if(cursta->bandwith == 2)
                cJSON_AddStringToObject(Node, "FrequencyWidth", "80M");
            if(cursta->wlanid == info->idx_2g){
                if(cursta->rssi <= info->rssi_th_2g)
                    need_to_send = 1;
            }else{
                if(cursta->rssi <= info->rssi_th_5g)
                    need_to_send = 1;
            }
            if((cursta->retry_ratio>=info->retry_ratio || cursta->fail_ratio>=info->fail_ratio) && (cursta->traffic_pkts/5 >= info->roaming_rate_th)){
                need_to_send = 1;
                QOS = cJSON_CreateObject();
                cJSON_AddItemToObject(Node, "QOS", QOS);
                if(cursta->retry_ratio>=info->retry_ratio)
                    cJSON_AddNumberToObject(QOS, "RetryRatio", cursta->retry_ratio);
                if(cursta->fail_ratio>=info->fail_ratio)
                    cJSON_AddNumberToObject(QOS, "PacketLoss", cursta->fail_ratio);
            }
        }
    }

    return need_to_send;
}

int Andlink_formRoamingResult(cJSON *root, unsigned char *mac, Andlink_basicInfo *info, int result)
{
    time_t tm;
    cJSON *data, *STA, *Node, *QOS;
    Andlink_roamingSta *cursta;
    char mac_str[18];

    time(&tm);

    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    if(result == 1)
        cJSON_AddStringToObject(root, "eventType", "STA_ONLINE");
    else
        cJSON_AddStringToObject(root, "eventType", "STA_REFUSED");
    cJSON_AddNumberToObject(root, "timestamp", tm);

    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    cJSON_AddStringToObject(data, "MacAddress", mac_str);

    return 0;
}

int Andlink_formStaChangeInfo(cJSON *root, Andlink_basicInfo *info, Andlink_assocSta *sta, int type)
{
    cJSON *data;
    char mac[18];
    time_t tm;

    time(&tm);
    
    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    if(type == AP_STAONLINE)
        cJSON_AddStringToObject(root, "eventType", "AP_STAONLINE");
    else if(type == AP_STAOFFLINE)
        cJSON_AddStringToObject(root, "eventType", "AP_STAOFFLINE");
    else if(type == AP_STAREFUSED)
        cJSON_AddStringToObject(root, "eventType", "AP_STAREFUSED");
    cJSON_AddNumberToObject(root, "timestamp", tm);

    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", 
        sta->mac[0], sta->mac[1], sta->mac[2], sta->mac[3], sta->mac[4], sta->mac[5]);
    cJSON_AddStringToObject(data, "MacAddress", mac);

    return 0;
}

int Andlink_formRoamingReq(cJSON *root, Andlink_neighborSta *sta, Andlink_basicInfo *info)
{
    cJSON *data;
    char mac[18];
    
    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    
    snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", 
        sta->mac[0], sta->mac[1], sta->mac[2], sta->mac[3], sta->mac[4], sta->mac[5]);
    cJSON_AddStringToObject(data, "MacAddress", mac);

    if(sta->wlanid == info->idx_2g)
        cJSON_AddStringToObject(data, "Radio", "2.4G");
    else
        cJSON_AddStringToObject(data, "Radio", "5G");
        
    cJSON_AddNumberToObject(data, "RSSI", sta->rssi-100);
    cJSON_AddNumberToObject(data, "channel", sta->channel);
    
    if(sta->bandwith == 0)
        cJSON_AddStringToObject(data, "FrequencyWidth", "20M");
    else if(sta->bandwith == 1)
        cJSON_AddStringToObject(data, "FrequencyWidth", "40M");
    else if(sta->bandwith == 2)
        cJSON_AddStringToObject(data, "FrequencyWidth", "80M");

    return 0;
}

ERR_NUM_T Andlink_SearchAPWiFiParameterSync(cJSON *data)
{   
    int m, i;
    int SyncCode, ret = 0;
    int tmpvalue;
    WiFiConfig param;
    cJSON *ap, *conf, *item, *_item;

    memset(&param, 0, sizeof(param));

#if 0	
    item = cJSON_GetObjectItem(data, "SyncCode");
    if(item){
		if(item->type == cJSON_Number){
            sprintf(param.synccode, "%d", item->valueint);
        }else if(item->type == cJSON_String){
            sprintf(param.synccode, item->valuestring);
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss SyncCode item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
	    }
#endif
        
    conf = cJSON_GetObjectItem(data, "Configurations");
    if(conf){
        m = cJSON_GetArraySize(conf);
        for(i=0; i<m; i++){
            ap = cJSON_GetArrayItem(conf, i);

            item = cJSON_GetObjectItem(ap, "Radio");
            if(item){
                if(!strcmp(item->valuestring, "2.4G"))
                    param.ap[i].band = PHYBAND_2G;
                else if(!strcmp(item->valuestring, "5G"))
                    param.ap[i].band = PHYBAND_5G;
                else{
                    ERR_PRINT("ERROR ANDLINK(%s)%d invalid \n", __FUNCTION__, __LINE__);
                    return INVALID_VALUE;
                }
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }
            
            item = cJSON_GetObjectItem(ap, "Index");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
                if(param.ap[i].band == PHYBAND_2G)   //andlink v3.1 index: 2.4g(1~4) 5g(5~8)
                    param.ap[i].idx = tmpvalue - 1;  //0 for root, 1~4 for vap
                else
                    param.ap[i].idx = tmpvalue - 5;
#else
                param.ap[i].idx = tmpvalue - 1;  //0 for root, 1~4 for vap                  
#endif
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Index item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }
			
#if 0
            item = cJSON_GetObjectItem(ap, "Enable");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
                param.ap[i].enable = tmpvalue;
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Enable item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
	            }
#endif
            item = cJSON_GetObjectItem(ap, "SSID");
            if(item){
                snprintf(param.ap[i].ssid, sizeof(param.ap[i].ssid), item->valuestring);
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss ssid item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }
            
            item = cJSON_GetObjectItem(ap, "SecurityMode");
            if(item){
                if(!strcmp(item->valuestring, "None") ||
						!strcmp(item->valuestring, "OPEN")){
                    param.ap[i].encrypt = ENCRYPT_DISABLED;
                }else if(!strcasecmp(item->valuestring, "WEP-64") || !strcasecmp(item->valuestring, "WEP")){
                    param.ap[i].encrypt = ENCRYPT_WEP;
                    param.ap[i].wep_type = WEP64;
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcasecmp(item->valuestring, "WEP-128")){
                    param.ap[i].encrypt = ENCRYPT_WEP;
                    param.ap[i].wep_type = WEP128;
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcasecmp(item->valuestring, "WPA-Personal") || 
                            !strcasecmp(item->valuestring, "WPA-PSK") || !strcasecmp(item->valuestring, "WPAPSK") || !strcasecmp(item->valuestring, "WPA")){
                    param.ap[i].encrypt = ENCRYPT_WPA;
                    param.ap[i].wpa_auth = WPA_AUTH_PSK;    //by default
                    param.ap[i].wpa_key_fmt = KEY_ASCII;    //by default
                    param.ap[i].wpa_cypher = WPA_CIPHER_TKIP;  //by default
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcmp(item->valuestring, "WPA2-Personal") || 
                            !strcasecmp(item->valuestring, "WPA2-PSK") || !strcasecmp(item->valuestring, "WPA2PSK") ||
                            !strcasecmp(item->valuestring, "WPA-PSK2") || !strcasecmp(item->valuestring, "WPAPSK2") || !strcasecmp(item->valuestring, "WPA2")){
                    param.ap[i].encrypt = ENCRYPT_WPA2;
                    param.ap[i].wpa_auth = WPA_AUTH_PSK;    //by default
                    param.ap[i].wpa_key_fmt = KEY_ASCII;    //by default
                    param.ap[i].wpa_cypher = WPA_CIPHER_MIXED;  //by default
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcasecmp(item->valuestring, "WPA-WPA2-Personal") || !strcasecmp(item->valuestring, "MIXED-WPA2PSK") ||
                            !strcasecmp(item->valuestring, "MIXED-WPAPSK2") || !strcasecmp(item->valuestring, "WPAWPA2")){
                    param.ap[i].encrypt = ENCRYPT_WPA2_MIXED;
                    param.ap[i].wpa_auth = WPA_AUTH_PSK;    //by default
                    param.ap[i].wpa_key_fmt = KEY_ASCII;    //by default
                    param.ap[i].wpa_cypher = WPA_CIPHER_MIXED;  //by default
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else{
                    ERR_PRINT("ERROR ANDLINK(%s)%d unsupported SecurityMode: %s\n", __FUNCTION__, __LINE__, item->valuestring);
                    return INVALID_VALUE;
                }
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss SecurityMode item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }

			item = cJSON_GetObjectItem(ap, "Channel");
            if(item){
				param.ap[i].channel = item->valueint;
            }
			else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Channel item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }

#if 0
            item = cJSON_GetObjectItem(ap, "MaxAssociateNum");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
                param.ap[i].max_sta_num = tmpvalue;
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss MaxAssociateNum item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }

            item = cJSON_GetObjectItem(ap, "SSIDAdvertisementEnabled");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
                param.ap[i].broadcast_ssid = tmpvalue;
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss SSIDAdvertisementEnabled item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
	            }
#endif
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Configurations item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }
    param.num = m;
#if 0
    for(i=0; i<m; i++)
	{
		printf("ap[%d].band = %d\n", i, param.ap[i].band);
		printf("ap[%d].channel = %d\n", i, param.ap[i].channel);
		printf("ap[%d].idx = %d\n", i, param.ap[i].idx);
		printf("ap[%d].enable = %d\n", i, param.ap[i].enable);
		printf("ap[%d].ssid = %s\n", i, param.ap[i].ssid);
		printf("ap[%d].encrypt = %d\n", i, param.ap[i].encrypt);
		printf("ap[%d].wpa_cypher = %d\n", i, param.ap[i].wpa_cypher);
		printf("ap[%d].wpa_auth = %d\n", i, param.ap[i].wpa_auth);
		printf("ap[%d].wpa_key_fmt = %d\n", i, param.ap[i].wpa_key_fmt);
		printf("ap[%d].wep_type = %d\n", i, param.ap[i].wep_type);
		printf("ap[%d].key = %s\n", i, param.ap[i].key);
		printf("ap[%d].max_sta_num = %d\n", i, param.ap[i].max_sta_num);
		printf("ap[%d].broadcast_ssid = %d\n", i, param.ap[i].broadcast_ssid);
	}
#endif
    ret |= rtl_link_syncSearchAPWiFiConfig(param);
#ifdef UNIVERSAL_REPEATER
    ret |= rtl_link_syncRepeaterConfig(param);
#endif
    Andlink_set_restart_flag(ret);

    return SUCCESS;  
}

ERR_NUM_T Andlink_postWiFiParameterSync(cJSON *data)
{   
    int m, i;
    int SyncCode, ret = 0;
    int tmpvalue;
    WiFiConfig param;
    cJSON *ap, *conf, *item, *_item;
	int pwdLen=0;

    memset(&param, 0, sizeof(param));
    item = cJSON_GetObjectItem(data, "SyncCode");
    if(item){
		if(item->type == cJSON_Number){
            sprintf(param.synccode, "%d", item->valueint);
        }else if(item->type == cJSON_String){
            sprintf(param.synccode, item->valuestring);
        }
    }else{
    	// field SyncCode for IF6 is capable, and not included in general. IF3 must include, IF5????
    	#if 0
        ERR_PRINT("ERROR ANDLINK(%s)%d miss SyncCode item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
		#else
		sprintf(param.synccode, "%d", -1);
		#endif
	}
        
    conf = cJSON_GetObjectItem(data, "Configurations");
    if(conf){
        m = cJSON_GetArraySize(conf);
        for(i=0; i<m; i++){
            ap = cJSON_GetArrayItem(conf, i);

            param.ap[i].channel = 0; //by default

            item = cJSON_GetObjectItem(ap, "Radio");
            if(item){
                if(!strcmp(item->valuestring, "2.4G"))
                    param.ap[i].band = PHYBAND_2G;
                else if(!strcmp(item->valuestring, "5G"))
                    param.ap[i].band = PHYBAND_5G;
                else{
                    ERR_PRINT("ERROR ANDLINK(%s)%d invalid \n", __FUNCTION__, __LINE__);
                    return INVALID_VALUE;
                }
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }
            
            item = cJSON_GetObjectItem(ap, "Index");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
                if(param.ap[i].band == PHYBAND_2G){   //andlink v3.1 index: 2.4g(1~4) 5g(5~8)
                	if(tmpvalue<1 || tmpvalue>4){
						ERR_PRINT("ERROR ANDLINK(%s)%d invalid 2G Index value\n", __FUNCTION__, __LINE__);
						return INVALID_VALUE;						
					}
                    param.ap[i].idx = tmpvalue - 1;  //0 for root, 1~4 for vap
                }
                else{
					if(tmpvalue<5 || tmpvalue>8){
						ERR_PRINT("ERROR ANDLINK(%s)%d invalid 5G Index value\n", __FUNCTION__, __LINE__);
						return INVALID_VALUE;						
					}
                    param.ap[i].idx = tmpvalue - 5;
                }
#else
                param.ap[i].idx = tmpvalue - 1;  //0 for root, 1~4 for vap                  
#endif
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Index item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }
            
            item = cJSON_GetObjectItem(ap, "Enable");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
                param.ap[i].enable = tmpvalue;
				if(tmpvalue!=0 && tmpvalue!=1){
					ERR_PRINT("ERROR ANDLINK(%s)%d invalid Enable value\n", __FUNCTION__, __LINE__);
					return INVALID_VALUE;	
				}
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss Enable item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
	            }
            
            item = cJSON_GetObjectItem(ap, "SSID");
            if(item){
                snprintf(param.ap[i].ssid, sizeof(param.ap[i].ssid), "%s", item->valuestring);
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss ssid item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }
            
            item = cJSON_GetObjectItem(ap, "SecurityMode");
            if(item){
                if(!strcmp(item->valuestring, "None") ||
						!strcmp(item->valuestring, "OPEN")){
                    param.ap[i].encrypt = ENCRYPT_DISABLED;
                }else if(!strcasecmp(item->valuestring, "WEP-64") || !strcasecmp(item->valuestring, "WEP")){
                    param.ap[i].encrypt = ENCRYPT_WEP;
                    param.ap[i].wep_type = WEP64;
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
						if(!rtl_link_check_pwd(_item->valuestring, ENCRYPT_WEP, WEP64)){
							ERR_PRINT("ERROR ANDLINK(%s)%d invalid WEP-64 Pwd value\n", __FUNCTION__, __LINE__);
							return INVALID_VALUE;
						}						
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcasecmp(item->valuestring, "WEP-128")){
                    param.ap[i].encrypt = ENCRYPT_WEP;
                    param.ap[i].wep_type = WEP128;
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
						if(!rtl_link_check_pwd(_item->valuestring, ENCRYPT_WEP, WEP128)){
							ERR_PRINT("ERROR ANDLINK(%s)%d invalid WEP-128 Pwd value\n", __FUNCTION__, __LINE__);
							return INVALID_VALUE;
						}
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcasecmp(item->valuestring, "WPA-Personal") || 
                            !strcasecmp(item->valuestring, "WPA-PSK") || !strcasecmp(item->valuestring, "WPAPSK") || !strcasecmp(item->valuestring, "WPA")){
                    param.ap[i].encrypt = ENCRYPT_WPA;
                    param.ap[i].wpa_auth = WPA_AUTH_PSK;    //by default
                    param.ap[i].wpa_key_fmt = KEY_ASCII;    //by default
                    param.ap[i].wpa_cypher = WPA_CIPHER_TKIP;  //by default
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
						if(!rtl_link_check_pwd(_item->valuestring, ENCRYPT_WPA, 0)){
							ERR_PRINT("ERROR ANDLINK(%s)%d invalid WPA Pwd value\n", __FUNCTION__, __LINE__);
							return INVALID_VALUE;
						}						
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
						pwdLen=strlen(_item->valuestring);
						if(pwdLen==64)
							param.ap[i].wpa_key_fmt = KEY_HEX;
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcmp(item->valuestring, "WPA2-Personal") || 
                            !strcasecmp(item->valuestring, "WPA2-PSK") || !strcasecmp(item->valuestring, "WPA2PSK") ||
                            !strcasecmp(item->valuestring, "WPA-PSK2") || !strcasecmp(item->valuestring, "WPAPSK2") || !strcasecmp(item->valuestring, "WPA2")){
                    param.ap[i].encrypt = ENCRYPT_WPA2;
                    param.ap[i].wpa_auth = WPA_AUTH_PSK;    //by default
                    param.ap[i].wpa_key_fmt = KEY_ASCII;    //by default
                    param.ap[i].wpa_cypher = WPA_CIPHER_MIXED;  //by default
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
						if(!rtl_link_check_pwd(_item->valuestring, ENCRYPT_WPA2, 0)){
							ERR_PRINT("ERROR ANDLINK(%s)%d invalid WPA2 Pwd value\n", __FUNCTION__, __LINE__);
							return INVALID_VALUE;
						}
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
						pwdLen=strlen(_item->valuestring);
						if(pwdLen==64)
							param.ap[i].wpa_key_fmt = KEY_HEX;
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else if(!strcasecmp(item->valuestring, "WPA-WPA2-Personal") || !strcasecmp(item->valuestring, "MIXED-WPA2PSK") ||
                            !strcasecmp(item->valuestring, "MIXED-WPAPSK2") || !strcasecmp(item->valuestring, "WPAWPA2")){
                    param.ap[i].encrypt = ENCRYPT_WPA2_MIXED;
                    param.ap[i].wpa_auth = WPA_AUTH_PSK;    //by default
                    param.ap[i].wpa_key_fmt = KEY_ASCII;    //by default
                    param.ap[i].wpa_cypher = WPA_CIPHER_MIXED;  //by default
                    _item = cJSON_GetObjectItem(ap, "Pwd");
                    if(_item){
						if(!rtl_link_check_pwd(_item->valuestring, ENCRYPT_WPA2_MIXED, 0)){
							ERR_PRINT("ERROR ANDLINK(%s)%d invalid WPA2-MIXED Pwd value\n", __FUNCTION__, __LINE__);
							return INVALID_VALUE;
						}
                        snprintf(param.ap[i].key, sizeof(param.ap[i].key), "%s", _item->valuestring);
						pwdLen=strlen(_item->valuestring);
						if(pwdLen==64)
							param.ap[i].wpa_key_fmt = KEY_HEX;
                    }else{
                        ERR_PRINT("ERROR ANDLINK(%s)%d miss Pwd item\n", __FUNCTION__, __LINE__);
                        return LACK_OF_ARGUMENT;
                    }
                }else{
                    ERR_PRINT("ERROR ANDLINK(%s)%d unsupported SecurityMode: %s\n", __FUNCTION__, __LINE__, item->valuestring);
                    return INVALID_VALUE;
                }
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss SecurityMode item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }

            item = cJSON_GetObjectItem(ap, "MaxAssociateNum");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
				if(tmpvalue<0){
					ERR_PRINT("ERROR ANDLINK(%s)%d invalid MaxAssociateNum value\n", __FUNCTION__, __LINE__);
					return INVALID_VALUE;
				}
                param.ap[i].max_sta_num = tmpvalue;				
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss MaxAssociateNum item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
            }

            item = cJSON_GetObjectItem(ap, "SSIDAdvertisementEnabled");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
				if(tmpvalue!=0 && tmpvalue!=1){
					ERR_PRINT("ERROR ANDLINK(%s)%d invalid SSIDAdvertisementEnabled value\n", __FUNCTION__, __LINE__);
                	return INVALID_VALUE;
				}
                param.ap[i].broadcast_ssid = tmpvalue;
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d miss SSIDAdvertisementEnabled item\n", __FUNCTION__, __LINE__);
                return LACK_OF_ARGUMENT;
	        }
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Configurations item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }
    param.num = m;
	
#if 1
    for(i=0; i<m; i++)
	{
		printf("ap[%d].band = %d\n", i, param.ap[i].band);
		printf("ap[%d].channel = %d\n", i, param.ap[i].channel);
		printf("ap[%d].idx = %d\n", i, param.ap[i].idx);
		printf("ap[%d].enable = %d\n", i, param.ap[i].enable);
		printf("ap[%d].ssid = %s\n", i, param.ap[i].ssid);
		printf("ap[%d].encrypt = %d\n", i, param.ap[i].encrypt);
		printf("ap[%d].wpa_cypher = %d\n", i, param.ap[i].wpa_cypher);
		printf("ap[%d].wpa_auth = %d\n", i, param.ap[i].wpa_auth);
		printf("ap[%d].wpa_key_fmt = %d\n", i, param.ap[i].wpa_key_fmt);
		printf("ap[%d].wep_type = %d\n", i, param.ap[i].wep_type);
		printf("ap[%d].key = %s\n", i, param.ap[i].key);
		printf("ap[%d].max_sta_num = %d\n", i, param.ap[i].max_sta_num);
		printf("ap[%d].broadcast_ssid = %d\n", i, param.ap[i].broadcast_ssid);
	}
#endif
    ret |= rtl_link_syncWiFiConfig(param);
#ifdef UNIVERSAL_REPEATER
    ret |= rtl_link_syncRepeaterConfig(param);
#endif
    Andlink_set_restart_flag(ret);

#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
    if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
    	return SUCCESS;  
}

ERR_NUM_T Andlink_postWiFiSwitch(cJSON *data)
{   
    cJSON *item;
    int ret = 0;
    WiFiSwitch wifiswitch;

    item = cJSON_GetObjectItem(data, "Radio");
    if(item){
        if(!strcmp(item->valuestring, "2.4G"))
            wifiswitch.band = BAND2G;
        else if(!strcmp(item->valuestring, "5G"))
            wifiswitch.band = BAND5G;
        else if(!strcmp(item->valuestring, "all"))
            wifiswitch.band = DUALBAND;
        else{
            ERR_PRINT("ERROR ANDLINK(%s)%d unknown Radio: %s\n", __FUNCTION__, __LINE__, item->valuestring);
            return INVALID_VALUE;
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    item = cJSON_GetObjectItem(data, "Enable");
    if(item){
        wifiswitch.enable = item->valueint;
		if(wifiswitch.enable!=0 && wifiswitch.enable!=1){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid Enable value\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_syncWiFiSwitch(wifiswitch);
    Andlink_set_restart_flag(ret);
 
#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
		return SUCCESS;  
}

ERR_NUM_T Andlink_postRadioConfig(cJSON *data)
{   
    cJSON *item;
    int ret = 0;
    RadioConfig radio;

    item = cJSON_GetObjectItem(data, "Radio");
    if(item){
        if(!strcmp(item->valuestring, "2.4G"))
            radio.band = BAND2G;
        else if(!strcmp(item->valuestring, "5G"))
            radio.band = BAND5G;
        else if(!strcmp(item->valuestring, "all"))
            radio.band = DUALBAND;
        else{
            ERR_PRINT("ERROR ANDLINK(%s)%d unknown Radio: %s\n", __FUNCTION__, __LINE__, item->valuestring);
            return INVALID_VALUE;
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    item = cJSON_GetObjectItem(data, "TransmitPower");
    if(item){
        radio.power = atoi(item->valuestring);
		if(radio.power<0 || radio.power>100){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid TransmitPower value: %d\n", __FUNCTION__, __LINE__, radio.power);
            return INVALID_VALUE;
		}
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss TransmitPower item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_syncRadioConfig(radio);
    Andlink_set_restart_flag(ret);
	
#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif    
    	return SUCCESS;  
}

ERR_NUM_T Andlink_postWPS(cJSON *data)
{   
    cJSON *item;
    int ret = 0;
    WPSSwitch wps;

    item = cJSON_GetObjectItem(data, "Radio");
    if(item){
        if(!strcmp(item->valuestring, "2.4G"))
            wps.band = BAND2G;
        else if(!strcmp(item->valuestring, "5G"))
            wps.band = BAND5G;
        else if(!strcmp(item->valuestring, "all"))
            wps.band = DUALBAND;
        else{
            ERR_PRINT("ERROR ANDLINK(%s)%d unknown Radio: %s\n", __FUNCTION__, __LINE__, item->valuestring);
            return INVALID_VALUE;
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    wps.enable = 1; //bydefault

    ret = rtl_link_syncWPSSwitch(wps);
    Andlink_set_restart_flag(ret);

#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
    	return SUCCESS;   
}

ERR_NUM_T Andlink_postMacFilter(cJSON *data)
{   
    cJSON *item;
    ERR_NUM_T err = SUCCESS;
    int ret = 0, cnt=1, i=0;
    MacFilter macfilter;
    MacNode *node;
    char *delim = "/,";
    char *tmp_buf;

	memset(&macfilter, 0 ,sizeof(macfilter));
	list_init(&macfilter.mac_list);
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
	macfilter.v25=1;
#endif
	
    item = cJSON_GetObjectItem(data, "MacFilterEnable");
    if(item){
        macfilter.enable = item->valueint;
		if(macfilter.enable !=0 && macfilter.enable!=1){
			err = INVALID_VALUE;
	        ERR_PRINT("ERROR ANDLINK(%s)%d invalid MacFilterEnable value\n", __FUNCTION__, __LINE__);
	        goto EXIT;
		}
    }else{
        err = LACK_OF_ARGUMENT;
        ERR_PRINT("ERROR ANDLINK(%s)%d miss MacFilterEnable item\n", __FUNCTION__, __LINE__);
        goto EXIT;
    }

    item = cJSON_GetObjectItem(data, "MacFilterPolicy");
    if(item){
        macfilter.policy = item->valueint;
		if(macfilter.policy !=0 && macfilter.policy!=1){
			err = INVALID_VALUE;
	        ERR_PRINT("ERROR ANDLINK(%s)%d invalid MacFilterPolicy value\n", __FUNCTION__, __LINE__);
	        goto EXIT;
		}
		apmib_set(MIB_RTL_LINK_V25_MACAC_POLICY,(void *)&macfilter.policy);
    }
#ifndef CONFIG_ANDLINK_IF6_SUPPORT
	else{
        err = LACK_OF_ARGUMENT;
        ERR_PRINT("ERROR ANDLINK(%s)%d miss MacFilterPolicy item\n", __FUNCTION__, __LINE__);
        goto EXIT;
    }
#else
	else{
		apmib_get(MIB_RTL_LINK_V25_MACAC_POLICY,(void *)&macfilter.policy);
	}
#endif
    item = cJSON_GetObjectItem(data, "MacFilterEntries");
    if(item){
		macfilter.contain_entries=1;
        tmp_buf = strtok(item->valuestring, delim);
        if(tmp_buf){
            node = malloc(sizeof(MacNode));
            if(!node){
                ERR_PRINT("ERROR ANDLINK (%s)%d malloc failed!\n", __FUNCTION__, __LINE__);
                err = ERROR;
                goto EXIT;
            }
            list_insert(&macfilter.mac_list, node);
            memset(node, 0, sizeof(*node));
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
			snprintf(node->mac,sizeof(node->mac),tmp_buf);
#else
            Andlink_macFormat(node->mac, tmp_buf);
#endif

			err = Andlink_isLanDev(node);
			if(err != SUCCESS)
				goto EXIT;
			
            while((tmp_buf = strtok(NULL, delim)))
            {
                if(cnt % 2){
                    snprintf(node->name, sizeof(node->name), tmp_buf);
                }else{
                    node = malloc(sizeof(MacNode));
                    if(!node){
                        ERR_PRINT("ERROR ANDLINK (%s)%d malloc failed!\n", __FUNCTION__, __LINE__);
                        err = ERROR;
                        goto EXIT;
                    }
                    list_insert(&macfilter.mac_list, node);
                    memset(node, 0, sizeof(*node));
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
					snprintf(node->mac,sizeof(node->mac),tmp_buf);
#else
                    Andlink_macFormat(node->mac, tmp_buf);
#endif
					err = Andlink_isLanDev(node);
					if(err != SUCCESS)
						goto EXIT;
		       }
                cnt++;
            }
            
        }
    }
#ifndef CONFIG_ANDLINK_IF6_SUPPORT
	else{
        err = LACK_OF_ARGUMENT;
        ERR_PRINT("ERROR ANDLINK(%s)%d miss MacFilterEntries item\n", __FUNCTION__, __LINE__);
        goto EXIT;
    }
#else
	else{
		macfilter.contain_entries=0;
    }
#endif
    ret = rtl_link_syncMacFilterSetting(macfilter);
    Andlink_set_restart_flag(ret);
    
EXIT:
    list_destroy(&macfilter.mac_list, free);

	if( err==SUCCESS ){
#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
		if(Andlink_is_IF6())
			return RECEIVED_SUCCESS;
		else
#endif
			return SUCCESS;
	}else
    	return err;  
}
ERR_NUM_T Andlink_postSearchAP(cJSON *data)
{
	return SUCCESS;
}

ERR_NUM_T Andlink_postReboot(cJSON *data)
{   
    cJSON *item;
    int ret = 0;
    Reboot reboot;

    item = cJSON_GetObjectItem(data, "ControlType");
    if(item){
        if(!strcmp(item->valuestring, "Reboot"))
            reboot.band = DUALBAND;
        else if(!strcmp(item->valuestring, "2.4GChanReSelect"))
            reboot.band = BAND2G;
        else if(!strcmp(item->valuestring, "5GChanReSelect"))
            reboot.band = BAND5G;
        else{
            ERR_PRINT("ERROR ANDLINK(%s)%d unknown ControlType: %s\n", __FUNCTION__, __LINE__, item->valuestring);
            return INVALID_ARGUMENT;
        }    
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss ControlType item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    if(basic_info.rtl_link_mode == RTL_LINK_REPEATER && reboot.band != DUALBAND)
        return SUCCESS;

	ret = rtl_link_syncReboot(reboot);
	Andlink_set_restart_flag(ret);

#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
    	return SUCCESS;    
}

ERR_NUM_T Andlink_postLEDControl(cJSON *data)
{   
    cJSON *item;
    int ret = 0;
    LEDStatus led;

    item = cJSON_GetObjectItem(data, "LEDOnOff");
    if(item){
        led.on = item->valueint;  
		if(led.on !=0 && led.on !=1){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid LEDOnOff value\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss LEDOnOff item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_syncLEDOnOff(led);
    Andlink_set_restart_flag(ret);
	
#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif        
    	return SUCCESS;     
}

ERR_NUM_T Andlink_postUpgrade(cJSON *data)
{   
    cJSON *fileUrl, *downMode, *fileMode, *fileType;
    int ret;
    FWUpgrade fw;

    fileMode = cJSON_GetObjectItem(data, "fileMode");

    fileType = cJSON_GetObjectItem(data, "fileType");
    if(fileType){
        if(strcmp(fileType->valuestring, "FIRM"))
            return INVALID_VALUE;
    }

    fileUrl = cJSON_GetObjectItem(data, "fileUrl");
    if(fileUrl){
        snprintf(fw.url, sizeof(fw.url), fileUrl->valuestring);
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss UpgURL item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    downMode = cJSON_GetObjectItem(data, "downMode");
    if(downMode){
        fw.reboot = downMode->valueint;
		if(fw.reboot!=0 && fw.reboot!=1){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid downMode value\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Reboot item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_upgradeFirmware(fw);

#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
    	return SUCCESS;  
}

ERR_NUM_T Andlink_postRoamingConfig(cJSON *data)
{   
    cJSON *item, *qos;
    int ret;
    RoamingConfig roaming;

    memset(&roaming, 0, sizeof(roaming));

    item = cJSON_GetObjectItem(data, "RoamingSwitch");
    if(item){
        roaming.enable = item->valueint;
		if(roaming.enable!=0 && roaming.enable!=1){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid RoamingSwitch value\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss RoamingSwitch item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    item = cJSON_GetObjectItem(data, "LowRSSI2.4G");
    if(item){
        roaming.rssi_2g = item->valueint;
		if(roaming.rssi_2g > 0){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid LowRSSI2.4G value\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
    }

    item = cJSON_GetObjectItem(data, "LowRSSI5G");
    if(item){
        roaming.rssi_5g = item->valueint;
		if(roaming.rssi_5g > 0){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid LowRSSI5G value\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
    }

    qos = cJSON_GetObjectItem(data, "LowQOS");
    if(qos){
        roaming.qos = 1;
        item = cJSON_GetObjectItem(qos, "PacketLoss");
        if(item){
            roaming.fail_ratio = item->valueint;
        }
        item = cJSON_GetObjectItem(qos, "RetryRatio");
        if(item){
            roaming.retry_ratio = item->valueint;
        }
        item = cJSON_GetObjectItem(data, "DetectPeriod");
        if(item){
            roaming.detect_period = item->valueint/1000;
        }
        item = cJSON_GetObjectItem(data, "RSSISwitchDelta");
        if(item){
            roaming.rssi_delta= item->valueint;
        }
        item = cJSON_GetObjectItem(data, "DismissTime");
        if(item){
            roaming.dismiss_time= item->valueint/1000;
        }
    }
    
    ret = rtl_link_syncRoamingConfig(roaming);
    Andlink_set_restart_flag(ret);

#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
    	return SUCCESS; 
}

ERR_NUM_T Andlink_postUnbind(cJSON *data)
{
    char empty[1] = "";
    char gwip[16] = "0.0.0.0";
    int tmpvalue = 0;
    struct in_addr ia_val;

    inet_aton(gwip, &ia_val);

//unbind信息如果是if3的，需要清空deviceId和deviceToken，如果是if6的，则不需要
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
	if(!Andlink_is_IF6()){
	    apmib_set(MIB_RTL_LINK_DEVID, (void *)empty);
		memset(basic_info.deviceId, 0, sizeof(basic_info.deviceId));
	    apmib_set(MIB_RTL_LINK_DEVTOKEN, (void *)empty);
		memset(basic_info.deviceToken, 0, sizeof(basic_info.deviceToken));
	}
#endif
    apmib_set(MIB_RTL_LINK_HBTIME, (void *)&tmpvalue);
	basic_info.HBTime = 0;
    apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)empty);
	strcpy(basic_info.synccode, "0");
#ifdef CONFIG_ANDLINK_V3_SUPPORT
	//apmib_set(MIB_RTL_LINK_USER_KEY, (void *)&empty); 			// for if5
	//memset(basic_info.user_key, 0, sizeof(basic_info.user_key));	// for if5
	apmib_set(MIB_RTL_LINK_CGWTOKEN, (void *)empty);
	memset(basic_info.gwToken_cloud, 0, sizeof(basic_info.gwToken_cloud));
	apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_cloud, 0, sizeof(basic_info.gwIp_cloud));
	apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)empty);
	memset(basic_info.gwToken_host, 0, sizeof(basic_info.gwToken_host));
    apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_host, 0, sizeof(basic_info.gwIp_host));
#endif
#ifdef CONFIG_ANDLINK_IF6_SUPPORT   
    apmib_set(MIB_RTL_LINK_CGW_UDPPORT, (void *)empty);
    //memset(basic_info.udp_port, 0, sizeof(basic_info.udp_port));
	basic_info.udp_port = 0;
    apmib_set(MIB_RTL_LINK_CGW_TCPPORT, (void *)empty);
    //memset(basic_info.tcp_port, 0, sizeof(basic_info.tcp_port));
	basic_info.tcp_port = 0;
	apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_cloud_udp, 0, sizeof(basic_info.gwIp_cloud_udp));
	apmib_set(MIB_RTL_LINK_CGW_MS_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_cloud_tcp, 0, sizeof(basic_info.gwIp_cloud_tcp));
#endif
#if 1//CONFIG_ANDLINK_V2_SUPPORT
    apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp, 0, sizeof(basic_info.gwIp));
    apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)empty);
	memset(basic_info.gwToken, 0, sizeof(basic_info.gwToken));
#endif	

	Andlink_setPreferBssidMibToZero();
	
    apmib_update(CURRENT_SETTING); 

#ifdef CONFIG_ANDLINK_IF6_SUPPORT	
	if(Andlink_is_IF6())
		return RECEIVED_SUCCESS;
	else
#endif
    	return SUCCESS;
}

#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST) 
ERR_NUM_T Andlink_postAddGW(cJSON *data)
{
    cJSON *item;
    int CGW=0, HGW=0;
    char gwAddress[64], gwToken[RTL_LINK_MAX_INFO_LEN];
    char *pchar;

    item = cJSON_GetObjectItem(data, "gwType");
    if(item){
        if(!strcmp(item->valuestring, "CGW"))
            CGW=1;
        else if(!strcmp(item->valuestring, "HGW"))
            HGW=1;
        else{
            ERR_PRINT("ERROR ANDLINK(%s)%d Unkonwn gwType\n", __FUNCTION__, __LINE__);
            return INVALID_VALUE;
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss gwType item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    item = cJSON_GetObjectItem(data, "gwAddress");
    if(item){
        strcpy(gwAddress, item->valuestring);
        pchar = strstr(gwAddress, ":");
        if(pchar){
            *pchar = '\0';
        }
        if(CGW){
            strcpy(basic_info.gwIp_cloud, gwAddress);
            apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)gwAddress);
        }else{
            strcpy(basic_info.gwIp_host, gwAddress);
            apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)gwAddress);
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss gwAddress item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    item = cJSON_GetObjectItem(data, "gwToken");
    if(item){
        strcpy(gwToken, item->valuestring);
        if(CGW){
            strcpy(basic_info.gwToken_cloud, gwToken);
            apmib_set(MIB_RTL_LINK_CGWTOKEN, (void *)gwToken);
        }else{
            strcpy(basic_info.gwToken_host, gwAddress);
            apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)gwToken);
        }
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss gwToken item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }
        
    return SUCCESS; 

}

ERR_NUM_T Andlink_postAppSearchDev(cJSON *data)
{
	cJSON *item;
	int CGW=0, HGW=0;
	char gwAddress[64], gwToken[RTL_LINK_MAX_INFO_LEN];
	char *pchar;

	item = cJSON_GetObjectItem(data, "searchKey");
	if(item){
		if(strcmp(item->valuestring, "ANDLINK-APP")){
			ERR_PRINT("ERROR ANDLINK(%s)%d invalid searchKey\n", __FUNCTION__, __LINE__);
			return INVALID_VALUE;
		}
	}else{
		ERR_PRINT("ERROR ANDLINK(%s)%d miss searchKey item\n", __FUNCTION__, __LINE__);
		return LACK_OF_ARGUMENT;
	}

	item = cJSON_GetObjectItem(data, "AndlinkVersion");
	if(item){
		if(strcmp(item->valuestring, "V3")){
			ERR_PRINT("ERROR ANDLINK(%s)%d andlinkVersion: %s invalid\n", __FUNCTION__, __LINE__, item->valuestring);
			return INVALID_VALUE;
		}
	}else{
		ERR_PRINT("ERROR ANDLINK(%s)%d miss AndlinkVersion item\n", __FUNCTION__, __LINE__);
		return LACK_OF_ARGUMENT;
	}
	return SUCCESS; 
}
#endif

ERR_NUM_T Andlink_postInformData(cJSON *data)
{
}

ERR_NUM_T Andlink_postRoamingReq(cJSON *data)
{
}

ERR_NUM_T Andlink_getApWlanInfo(cJSON *data)
{
	int i;
	cJSON *Configurations,*Node;
	WiFiConfig config;
	
    memset(&config, 0, sizeof(config));
    
    rtl_link_getWiFiConfig(&config);
    
    Configurations = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "Configurations", Configurations);
   
    for(i=0; i<config.num; i++)
	{
        if(config.ap[i].idx==0 || (config.ap[i].idx!=0 && config.ap[i].enable && config.ap[i].idx!=4))  //andlink v3.1 index: 2.4g(1~4) 5g(5~8)
        {
            Node = cJSON_CreateObject();
            cJSON_AddItemToArray(Configurations, Node);
            if(config.ap[i].band == PHYBAND_2G)
                cJSON_AddStringToObject(Node, "Radio", "2.4G"); 
            else
                cJSON_AddStringToObject(Node, "Radio", "5G");
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)                
            if(config.ap[i].band == PHYBAND_2G)     //andlink v3.1 index: 2.4g(1~4) 5g(5~8) 
                cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 1);
            else
                cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 5);
#else
            cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 1);
#endif
            cJSON_AddStringToObject(Node, "SSID", config.ap[i].ssid);
            if(config.ap[i].encrypt == ENCRYPT_DISABLED)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "None");
                cJSON_AddStringToObject(Node, "Pwd", "");
            }
            else if(config.ap[i].encrypt == ENCRYPT_WEP)
            {
                if(config.ap[i].wep_type == WEP64)
                {
                    cJSON_AddStringToObject(Node, "SecurityMode", "WEP-64");
                    cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
                }
                else if(config.ap[i].wep_type == WEP128)
                {
                    cJSON_AddStringToObject(Node, "SecurityMode", "WEP-128");
                    cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
                }
            }
            else if(config.ap[i].encrypt == ENCRYPT_WPA)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "WPA-Personal");
                cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
            }
            else if(config.ap[i].encrypt == ENCRYPT_WPA2)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "WPA2-Personal");
                cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
            }
            else if(config.ap[i].encrypt == ENCRYPT_WPA2_MIXED)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "MIXED-WPAPSK2");
                cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
            }
			cJSON_AddNumberToObject(Node, "Channel", config.ap[i].channel);
        }
    }
}

ERR_NUM_T Andlink_getApConfigInfo(cJSON *data)
{
    RfStatus RF;
    WiFiConfig config;
    LEDStatus led;
    RoamingConfig roaming;
    MacFilter macfilter;
    MACFILTER_T *macEntry;
    struct sysinfo sys_info;
    cJSON *Radios, *Configurations, *Node;
    int i, ret=0;
    char power[4], uptime[33]; 
    char hwversion[33], swversion[33];
    char filter_string[2048]={0};
    char filter_item[64];
	SysMemInfo meminfo;
	
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    int workmode=0;
#endif

    if(version >= 0X210E){   //2.10e 2.10f
        //hw/sw version
        apmib_get(MIB_RTL_LINK_HWVERSION, (void *)hwversion);
        apmib_get(MIB_RTL_LINK_SWVERSION, (void *)swversion);

        //uptime
        sysinfo(&sys_info);

        //macfilter
        memset(&macfilter, 0, sizeof(macfilter));
        list_init(&macfilter.mac_list);
        rtl_link_getMacFilterSetting(&macfilter);
    }

    //radio
    memset(&RF, 0, sizeof(RF));
    ret = rtl_link_getRFInfo(&RF);
    if(ret){
        ERR_PRINT("ERROR ANDLINK(%s)%d error\n", __FUNCTION__, __LINE__);
        list_destroy(&macfilter.mac_list, free);
        return ERROR;
    }
    
    //config
    memset(&config, 0, sizeof(config));
    rtl_link_getWiFiConfig(&config);

    //led
    memset(&led, 0, sizeof(led));
    rtl_link_getLEDStatus(&led);

    //roaming
    memset(&roaming, 0, sizeof(roaming));
    rtl_link_getRoamingStatus(&roaming);

	//meminfo
	memset(&meminfo, 0, sizeof(meminfo));
	rtl_link_getSysMemInfo(&meminfo);
    
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    rtl_link_getWorkMode(&workmode);
#endif

    if(version >= 0X210E){
        cJSON_AddStringToObject(data, "hardwareVersion", hwversion);
        cJSON_AddStringToObject(data, "softwareVersion", swversion);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)        
        cJSON_AddNumberToObject(data, "WorkingMode", workmode);
#endif
		cJSON_AddNumberToObject(data, "FlashSize", meminfo.flashSize);
		cJSON_AddNumberToObject(data, "RamSize", meminfo.ramSize);
		cJSON_AddNumberToObject(data, "CPUOccupancyRate", meminfo.cpuUtility);
		cJSON_AddNumberToObject(data, "RamOccupancyRate", meminfo.ramUtility);		
        snprintf(uptime, sizeof(uptime), "%ld", sys_info.uptime);
        cJSON_AddStringToObject(data, "UpTime", uptime);
        cJSON_AddNumberToObject(data, "MacFilterEnable", macfilter.enable);
        cJSON_AddNumberToObject(data, "MacFilterPolicy", macfilter.policy);
        if(macfilter.mac_list.len > 32)
            macfilter.mac_list.len = 32;
        for(i=0; i<macfilter.mac_list.len; i++){
            macEntry = list_get_element(&macfilter.mac_list, i);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
			if(i == macfilter.mac_list.len-1)
			{				
				snprintf(filter_item, sizeof(filter_item), "%02x%02x%02x%02x%02x%02x/%s", 
					macEntry->macAddr[0], macEntry->macAddr[1], macEntry->macAddr[2], macEntry->macAddr[3],	macEntry->macAddr[4],
					macEntry->macAddr[5], macEntry->comment);			
			}else{
				snprintf(filter_item, sizeof(filter_item), "%02x%02x%02x%02x%02x%02x/%s,", 
	                macEntry->macAddr[0], macEntry->macAddr[1], macEntry->macAddr[2], macEntry->macAddr[3], 
	                macEntry->macAddr[4], macEntry->macAddr[5], macEntry->comment);				
			} 
#else
            snprintf(filter_item, sizeof(filter_item), "%02x:%02x:%02x:%02x:%02x:%02x/%s,", 
                macEntry->macAddr[0], macEntry->macAddr[1], macEntry->macAddr[2], macEntry->macAddr[3], 
                macEntry->macAddr[4], macEntry->macAddr[5], macEntry->comment);
#endif
            strcat(filter_string, filter_item);
        }
        cJSON_AddStringToObject(data, "MacFilterEntries", filter_string);
    }
    cJSON_AddNumberToObject(data, "LEDOnOff", led.on);
    cJSON_AddNumberToObject(data, "RoamingSwitch", roaming.enable);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    for(i=0; i<RF.num; i++){
        if(RF.rf[i].band == PHYBAND_2G)
            cJSON_AddNumberToObject(data, "LowRSSI2.4G", roaming.rssi_2g-100);
        else
            cJSON_AddNumberToObject(data, "LowRSSI5G", roaming.rssi_5g-100);
    }
#else
    if(roaming.enable){
        for(i=0; i<RF.num; i++){
            if(RF.rf[i].band == PHYBAND_2G)
                cJSON_AddNumberToObject(data, "LowRSSI2.4G", roaming.rssi_2g-100);
            else
                cJSON_AddNumberToObject(data, "LowRSSI5G", roaming.rssi_5g-100);
        }
        
    }
#endif
    cJSON_AddStringToObject(data, "SyncCode", config.synccode);

    Radios = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "Radios", Radios);
    for(i=0; i<RF.num; i++)
    {
        Node = cJSON_CreateObject();
        cJSON_AddItemToArray(Radios, Node);
        if(RF.rf[i].band == PHYBAND_2G)
            cJSON_AddStringToObject(Node, "Radio", "2.4G"); 
        else
            cJSON_AddStringToObject(Node, "Radio", "5G"); 
        cJSON_AddNumberToObject(Node, "Enable", RF.rf[i].enable);
        snprintf(power, sizeof(power), "%d", RF.rf[i].power);
        cJSON_AddStringToObject(Node, "TransmitPower", power);
        cJSON_AddNumberToObject(Node, "Channel", RF.rf[i].channel);
    }
    
    Configurations = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "Configurations", Configurations);
    for(i=0; i<config.num; i++)
    {
        if(config.ap[i].idx==0 || (config.ap[i].idx!=0 && config.ap[i].enable && config.ap[i].idx!=4 && !config.ap[i].root_disabled))  //andlink v3.1 index: 2.4g(1~4) 5g(5~8)
        {
            Node = cJSON_CreateObject();
            cJSON_AddItemToArray(Configurations, Node);
            if(config.ap[i].band == PHYBAND_2G)
                cJSON_AddStringToObject(Node, "Radio", "2.4G"); 
            else
                cJSON_AddStringToObject(Node, "Radio", "5G");
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)            
            if(config.ap[i].band == PHYBAND_2G)     //andlink v3.1 index: 2.4g(1~4) 5g(5~8) 
                cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 1);
            else
                cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 5);
#else
            cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 1);
#endif
            cJSON_AddNumberToObject(Node, "Enable", config.ap[i].enable);
            cJSON_AddStringToObject(Node, "SSID", config.ap[i].ssid);
            if(config.ap[i].encrypt == ENCRYPT_DISABLED)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "None");
                cJSON_AddStringToObject(Node, "Pwd", "");
            }
            else if(config.ap[i].encrypt == ENCRYPT_WEP)
            {
                if(config.ap[i].wep_type == WEP64)
                {
                    cJSON_AddStringToObject(Node, "SecurityMode", "WEP-64");
                    cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
                }
                else if(config.ap[i].wep_type == WEP128)
                {
                    cJSON_AddStringToObject(Node, "SecurityMode", "WEP-128");
                    cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
                }
            }
            else if(config.ap[i].encrypt == ENCRYPT_WPA)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "WPA-Personal");
                cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
            }
            else if(config.ap[i].encrypt == ENCRYPT_WPA2)
            {
                cJSON_AddStringToObject(Node, "SecurityMode", "WPA2-Personal");
                cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
            }
            else if(config.ap[i].encrypt == ENCRYPT_WPA2_MIXED)
            {
                //cJSON_AddStringToObject(Node, "SecurityMode", "WPA-WPA2-Personal");
                cJSON_AddStringToObject(Node, "SecurityMode", "MIXED-WPAPSK2");
                cJSON_AddStringToObject(Node, "Pwd", config.ap[i].key);
            }
            cJSON_AddNumberToObject(Node, "MaxAssociateNum", config.ap[i].max_sta_num);
            cJSON_AddNumberToObject(Node, "SSIDAdvertisementEnabled", config.ap[i].broadcast_ssid);
        }
    }

EXIT:
    list_destroy(&macfilter.mac_list, free);
    return SUCCESS;

}

ERR_NUM_T Andlink_getUplinkStatus(cJSON *data)
{
    UplinkStatus uplink;
	char cur_txrate[10], cur_rxrate[10];
    int ret=0;
    char port[10];
#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    char delim_mac[18]={0};
#endif

    memset(&uplink, 0, sizeof(uplink));

    ret = rtl_link_getUplinkStatus(&uplink);
    if(ret){
        ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
        return ERROR;
    }

    if(!uplink.cur_rxrate && !uplink.cur_txrate){
        if(uplink.type == WIRELESS){
            if(rtl_link_get_terminal_rate_by_mac(uplink.mac, &uplink.cur_txrate, &uplink.cur_rxrate)){
                ERR_PRINT("ERROR (%s)%d get_terminal_rate_by_mac error\n", __FUNCTION__, __LINE__);
                //res = ERROR;
                //goto EXIT;
            }
        }else{
#ifdef CONFIG_APP_RTK_ADAPTER
            if(rtl_link_get_port_speed(uplink.wan_port_num, &uplink.cur_rxrate, &uplink.cur_txrate)){
                ERR_PRINT("ERROR (%s)%d rtl_link_get_port_speed error\n", __FUNCTION__, __LINE__);
                //res = ERROR;
                //goto EXIT;
            }
#else
            sprintf(port, "Port%d", uplink.wan_port_num);
            if(rtl_link_get_terminal_rate_by_port(port, &uplink.cur_txrate, &uplink.cur_rxrate)){
                ERR_PRINT("ERROR (%s)%d get_terminal_rate_by_port error\n", __FUNCTION__, __LINE__);
                //res = ERROR;
                //goto EXIT;
            }
#endif
        }
    }

    if(uplink.type == WIRELESS){
        if(version >= 0X210E)
            cJSON_AddStringToObject(data, "UplinkType", "WLAN");
        else
            cJSON_AddStringToObject(data, "UplinkType", "Wireless");
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT             
//        Andlink_macFormat_add_delim(delim_mac, basic_info.wlan_mac_2g);
//        cJSON_AddStringToObject(data, "MacAddress", delim_mac);
//#else
        cJSON_AddStringToObject(data, "MacAddress", basic_info.wlan_mac_2g);
//#endif
        if(uplink.band == PHYBAND_2G)
            cJSON_AddStringToObject(data, "Radio", "2.4G");
        else
            cJSON_AddStringToObject(data, "Radio", "5G");
        cJSON_AddStringToObject(data, "SSID", uplink.ssid);
        cJSON_AddNumberToObject(data, "Channel", uplink.channel);
        cJSON_AddStringToObject(data, "Noise", uplink.noise);
        cJSON_AddStringToObject(data, "SNR", uplink.snr);
        cJSON_AddStringToObject(data, "RSSI", uplink.rssi);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)       
        cJSON_AddStringToObject(data, "RxRate", uplink.rxrate);
        cJSON_AddStringToObject(data, "TxRate", uplink.txrate);
		snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
        cJSON_AddStringToObject(data, "RxRate_rt", cur_rxrate);
        cJSON_AddStringToObject(data, "TxRate_rt", cur_txrate);
#else
        snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
        cJSON_AddStringToObject(data, "RxRate", cur_rxrate);
        cJSON_AddStringToObject(data, "TxRate", cur_txrate);
#endif
    }else{
        cJSON_AddStringToObject(data, "UplinkType", "Ethernet");
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)       
        //Andlink_macFormat_add_delim(delim_mac, basic_info.lan_mac);
        //cJSON_AddStringToObject(data, "MacAddress", delim_mac);
        cJSON_AddStringToObject(data, "MacAddress", basic_info.lan_mac);
        cJSON_AddStringToObject(data, "Radio", "");
        cJSON_AddStringToObject(data, "SSID", "");
        cJSON_AddNumberToObject(data, "Channel", 0);
        cJSON_AddStringToObject(data, "Noise", "");
        cJSON_AddStringToObject(data, "SNR", "");
        cJSON_AddStringToObject(data, "RSSI", "");
#else
        cJSON_AddStringToObject(data, "MacAddress", basic_info.lan_mac);
#endif
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
        cJSON_AddStringToObject(data, "RxRate", uplink.rxrate);
        cJSON_AddStringToObject(data, "TxRate", uplink.txrate);
		snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
        cJSON_AddStringToObject(data, "RxRate_rt", cur_rxrate);
        cJSON_AddStringToObject(data, "TxRate_rt", cur_txrate);
#else
        snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
        cJSON_AddStringToObject(data, "RxRate", cur_rxrate);
        cJSON_AddStringToObject(data, "TxRate", cur_txrate);
#endif
    }
    
#if defined(CONFIG_ANDLINK_V3_SUPPORT) && !defined(ANDLINK_IF356_COEXIST)
    cJSON_AddStringToObject(data, "IPAddress", uplink.ip);
#endif

    return SUCCESS;
}

ERR_NUM_T Andlink_getSTAInfo(cJSON *data)
{
    cJSON *Devices, *Node;
    List sta_list;
    STAInfo *sta;
    ERR_NUM_T res=SUCCESS;
    int i, ret=0, port;
    char cur_txrate[10] = {0}, cur_rxrate[10] = {0};
	char delete_delim_mac[13]={0},delete_delim_vmac[13]={0};

    list_init(&sta_list);
    ret = rtl_link_getWlanSTAInfo(&sta_list);
    if(ret==-1){
        ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
        res = ERROR;
        goto EXIT;
    }
    ret = rtl_link_getLanDevInfo(&sta_list);
    if(ret==-1){
        ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
        res = ERROR;
        goto EXIT;
    }

#ifndef CONFIG_APP_RTK_ADAPTER
    for(i=0; i<sta_list.len; i++)
    {
        sta = list_get_element(&sta_list, i);
        if(sta->uptype == WIRELESS){
            if(rtl_link_get_terminal_rate_by_mac(sta->mac, &sta->cur_rxrate, &sta->cur_txrate)){
                ERR_PRINT("ERROR (%s)%d get_terminal_rate_by_mac error\n", __FUNCTION__, __LINE__);
                //res = ERROR;
                //goto EXIT;
            }
        }else{
	        if(rtl_link_get_terminal_rate_by_port(sta->port, &sta->cur_rxrate, &sta->cur_txrate)){
                ERR_PRINT("ERROR (%s)%d get_terminal_rate_by_port error\n", __FUNCTION__, __LINE__);
                //res = ERROR;
                //goto EXIT;
            }
        }
    }  
#endif

    Devices = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "Devices", Devices);

    for(i=0; i<sta_list.len; i++)
    {
        sta = list_get_element(&sta_list, i);
#if 0
        DEBUG("sta[%d].uptype = %d\n", i, sta->uptype);
        DEBUG("sta[%d].mac = %s\n", i, sta->mac);
        DEBUG("sta[%d].clone_mac = %s\n", i, sta->clone_mac);
        DEBUG("sta[%d].uptime = %s\n", i, sta->uptime);
        if(sta[i].uptype == WIRELESS){
            DEBUG("sta[%d].band = %d\n", i, sta->band);
            DEBUG("sta[%d].ssid = %s\n", i, sta->ssid);
            DEBUG("sta[%d].rssi = %s\n", i, sta->rssi);
        }
        DEBUG("sta[%d].RxRate = %d\n", i, sta->cur_rxrate);
        DEBUG("sta[%d].TxRate = %d\n", i, sta->cur_txrate);
		DEBUG("sta[%d].hostname = %s\n", i, sta->hostname);
		DEBUG("sta[%d].ip = %s\n", i, sta->ip_addr);		
#endif        
        Node = cJSON_CreateObject();
        cJSON_AddItemToArray(Devices, Node);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
		Andlink_macFormat(delete_delim_mac, sta->clone_mac);
		cJSON_AddStringToObject(Node, "MacAddress", delete_delim_mac);
		Andlink_macFormat(delete_delim_vmac, sta->mac);
		 cJSON_AddStringToObject(Node, "VMacAddress", delete_delim_vmac);
#else
		cJSON_AddStringToObject(Node, "MacAddress", sta->clone_mac); 
		cJSON_AddStringToObject(Node, "VMacAddress", sta->mac);
#endif
        
        cJSON_AddStringToObject(Node, "UpTime", sta->uptime);
        if(!strcmp(sta->ip_addr, "0.0.0.0"))
            cJSON_AddStringToObject(Node, "IPAddress", "");
        else
		    cJSON_AddStringToObject(Node, "IPAddress", sta->ip_addr);
		cJSON_AddStringToObject(Node, "HostName", sta->hostname);
        if(sta->uptype == WIRELESS){
            if(sta->band == PHYBAND_2G)
                cJSON_AddStringToObject(Node, "Radio", "2.4G");
            else
                cJSON_AddStringToObject(Node, "Radio", "5G");
            cJSON_AddStringToObject(Node, "SSID", sta->ssid);
            cJSON_AddStringToObject(Node, "RSSI", sta->rssi);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST) 
			cJSON_AddStringToObject(Node, "RxRate", sta->rxrate);
		    cJSON_AddStringToObject(Node, "TxRate", sta->txrate);
			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
            snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
			cJSON_AddStringToObject(Node, "RxRate_rt", cur_rxrate);
        	cJSON_AddStringToObject(Node, "TxRate_rt", cur_txrate);
#else			
			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
            snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
			cJSON_AddStringToObject(Node, "RxRate", cur_rxrate);
        	cJSON_AddStringToObject(Node, "TxRate", cur_txrate);
#endif
        }else{
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST) 
            cJSON_AddStringToObject(Node, "Radio", "");
            cJSON_AddStringToObject(Node, "SSID", "");
            cJSON_AddStringToObject(Node, "RSSI", "");
		    cJSON_AddStringToObject(Node, "RxRate", sta->rxrate);
		    cJSON_AddStringToObject(Node, "TxRate", sta->txrate);
			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
			snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
		    cJSON_AddStringToObject(Node, "RxRate_rt", cur_rxrate);
		    cJSON_AddStringToObject(Node, "TxRate_rt", cur_txrate);
#else
			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
			snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
		    cJSON_AddStringToObject(Node, "RxRate", cur_rxrate);
		    cJSON_AddStringToObject(Node, "TxRate", cur_txrate);
#endif
        }
    }
EXIT:
    list_destroy(&sta_list, free);
    return res;
}

ERR_NUM_T Andlink_getWiFiStats(cJSON *data)
{
    WiFiTrafficStats wifi;
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    LanTrafficStats lan;
#endif 
    int i, ret=0;
    cJSON *Stats, *Node;

    memset(&wifi, 0, sizeof(wifi));

    ret = rtl_link_getWiFiTrafficStats(&wifi);
    if(ret){
        ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
        return ERROR;
    }
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    ret = rtl_link_getLanTrafficStats(&lan);
	if(ret){
		ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
		return ERROR;
	}
#endif    

    Stats = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "Stats", Stats);

    for(i=0; i<wifi.num; i++)
    {
        Node = cJSON_CreateObject();
        cJSON_AddItemToArray(Stats, Node);

        if(wifi.ap[i].band == PHYBAND_2G)
            cJSON_AddStringToObject(Node, "Radio", "2.4G");
        else
            cJSON_AddStringToObject(Node, "Radio", "5G");
        cJSON_AddNumberToObject(Node, "Index", wifi.ap[i].idx + 1);
        cJSON_AddStringToObject(Node, "SSID", wifi.ap[i].ssid);
        cJSON_AddNumberToObject(Node, "TotalBytesSent", wifi.ap[i].send_bytes);
        cJSON_AddNumberToObject(Node, "TotalBytesReceived", wifi.ap[i].recv_bytes);
        cJSON_AddNumberToObject(Node, "TotalPacketsSent", wifi.ap[i].send_packets);
        cJSON_AddNumberToObject(Node, "TotalPacketsReceived", wifi.ap[i].recv_packets);
        cJSON_AddNumberToObject(Node, "ErrorsSent", wifi.ap[i].err_send);
        cJSON_AddNumberToObject(Node, "ErrorsReceived", wifi.ap[i].err_recv);
        cJSON_AddNumberToObject(Node, "DiscardPacketsSent", wifi.ap[i].discard_send);
        cJSON_AddNumberToObject(Node, "DiscardPacketsReceived", wifi.ap[i].discard_recv);
    }
#ifdef CONFIG_ANDLINK_V25_SUPPORT
    //for(i=0; i<lan.num; i++)
	/* for HN100 */
#if defined (CONFIG_MODEL_HN100_340EA) || defined (CONFIG_MODEL_HN120_341FA) ||  defined (CONFIG_MODEL_HN120_341BA)
	for(i=0; i<4; i++)
#elif defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	for(i=0; i<2; i++)
#elif defined (CONFIG_MODEL_HN140_361EA)
	for(i=0; i<5; i++)
#endif
    {
        Node = cJSON_CreateObject();
        cJSON_AddItemToArray(Stats, Node);

        cJSON_AddStringToObject(Node, "Radio", "0");
        cJSON_AddNumberToObject(Node, "Index", lan.ap[i].idx + 1);
        cJSON_AddStringToObject(Node, "SSID","");
        cJSON_AddNumberToObject(Node, "TotalBytesSent", lan.ap[i].send_bytes);
        cJSON_AddNumberToObject(Node, "TotalBytesReceived", lan.ap[i].recv_bytes);
        cJSON_AddNumberToObject(Node, "TotalPacketsSent", lan.ap[i].send_packets);
        cJSON_AddNumberToObject(Node, "TotalPacketsReceived", lan.ap[i].recv_packets);
        cJSON_AddNumberToObject(Node, "ErrorsSent", lan.ap[i].err_send);
        cJSON_AddNumberToObject(Node, "ErrorsReceived", lan.ap[i].err_recv);
        cJSON_AddNumberToObject(Node, "DiscardPacketsSent", lan.ap[i].discard_send);
        cJSON_AddNumberToObject(Node, "DiscardPacketsReceived", lan.ap[i].discard_recv);
    }
#endif    
    return SUCCESS;
}

ERR_NUM_T Andlink_getRoamingConfig(cJSON *data)
{
    RoamingConfig roaming;
    cJSON *qos;

    memset(&roaming, 0, sizeof(roaming));

    rtl_link_getRoamingConfig(&roaming);

    cJSON_AddNumberToObject(data, "RoamingSwitch", roaming.enable);
    cJSON_AddNumberToObject(data, "LowRSSI2.4G", roaming.rssi_2g);
    cJSON_AddNumberToObject(data, "LowRSSI5G", roaming.rssi_5g);
    if(roaming.qos){
        qos = cJSON_CreateObject();
        cJSON_AddItemToObject(data, "LowQOS", qos);
        if(roaming.fail_ratio)
            cJSON_AddNumberToObject(qos, "PacketLoss", roaming.fail_ratio);
        if(roaming.retry_ratio)
            cJSON_AddNumberToObject(qos, "RetryRatio", roaming.retry_ratio);
    }
    if(roaming.detect_period)
        cJSON_AddNumberToObject(data, "DetectPeriod", roaming.detect_period*1000);
    if(roaming.rssi_delta)
        cJSON_AddNumberToObject(data, "RSSISwitchDelta", roaming.rssi_delta);
    if(roaming.dismiss_time)
        cJSON_AddNumberToObject(data, "DismissTime", roaming.dismiss_time*1000);

    return SUCCESS;
}

#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
ERR_NUM_T Andlink_getGWInfo(cJSON *data)
{
	cJSON *gwList, *node;

	gwList = cJSON_CreateArray();
	cJSON_AddItemToObject(data, "gwList", gwList);

	if(basic_info.gwIp_host[0] && basic_info.gwToken_host[0]){
		node = cJSON_CreateObject();
		cJSON_AddItemToArray(gwList,node);
		cJSON_AddStringToObject(node,"gwType", "HGW");
		cJSON_AddStringToObject(node,"gwAddress", basic_info.gwIp_host);
		cJSON_AddStringToObject(node,"gwToken", basic_info.gwToken_host);
	}

	if(basic_info.gwIp_cloud[0] && basic_info.gwToken_cloud[0]){
		node = cJSON_CreateObject();
		cJSON_AddItemToArray(gwList,node);
		cJSON_AddStringToObject(node,"gwType", "CGW");
		cJSON_AddStringToObject(node,"gwAddress", basic_info.gwIp_cloud);
		cJSON_AddStringToObject(node,"gwToken", basic_info.gwToken_cloud);
	}
	
	return SUCCESS;
}
#endif

void Andlink_storeGWIp(struct in_addr ip, Andlink_basicInfo *info)
{
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    Andlink_setHostGW();
    strcpy(info->gwIp_host, inet_ntoa(ip));
#else
    strcpy(info->gwIp, inet_ntoa(ip));
#endif
    apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip);
    DEBUG("ANDLINK (%s)%d store gw ip!\n", __FUNCTION__, __LINE__);
    apmib_update(CURRENT_SETTING);   
}

//parse ip and port from 一级家开平台
void Andlink_parseCloudGWInfo(cJSON *root, ANDLINK_CLOUD_GW_INFO *pcloud_gw_info)
{
    cJSON *item=NULL;
    char tmp_addr[512];
    char tmp_ip[32];
	char tmpBuff[128];
    char *pchar;
    struct in_addr ip;
    struct hostent *hp;
    int i=0, resolve_success=0, index=1, flag=0;
	int cnt=3, j=0;

    item = cJSON_GetObjectItem(root, "user_key");
	if(item){		
		ERR_PRINT("ERROR ANDLINK (%s)%d wrong rsp from CloudGW\n", __FUNCTION__, __LINE__);	
		return;
	}

    item = cJSON_GetObjectItem(root, "gwAddress");
	if(item){
	    strcpy(tmp_addr, item->valuestring);
	    pchar = strstr(tmp_addr, ":");
	    if(pchar){
	        *pchar = '\0';
	    }
	    if(is_valid_ipv4_addr(tmp_addr)){
	        strcpy(tmp_ip, tmp_addr);
	        resolve_success = 1;
	    }else{			
	        hp = gethostbyname(tmp_addr);	
	        if(hp != NULL){
	            while(hp->h_addr_list[i] != NULL){
	                strcpy(tmp_ip, inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
	                i++;
	            }
	            resolve_success = 1;
	        }else{
	            ERR_PRINT("ERROR ANDLINK (%s)%d resolve hostname \"%s\" fail\n", __FUNCTION__, __LINE__, tmp_addr);
	            resolve_success = 0;
	        }
	    }  

		if(resolve_success){
			//eg:"gwAddress2":"https://112.13.96.207:4443"
			item = cJSON_GetObjectItem(root, "gwAddress2");
			if(item){
				strcpy(tmp_ip, item->valuestring);				
				pchar = strtok(tmp_ip, "//");
				while(pchar){
					if(index == 1){
						pchar = strtok(NULL, "//");
						index++;
					}else{
						strcpy(tmpBuff, pchar);
						pchar = strstr(tmpBuff, ":");
						if(pchar){
							*pchar = '\0';
						}

						for(j=0; j<cnt; ++j){
							hp = NULL;
							i=0;
							hp = gethostbyname(tmpBuff);	
					        if(hp != NULL){
					            while(hp->h_addr_list[i] != NULL){
					                strcpy(tmp_ip, inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
					                i++;
					            }
								strcpy(pcloud_gw_info->gwip, tmp_ip);
								pcloud_gw_info->port = atoi(pchar+1);
								DEBUG("ANDLINK (%s)%d get CloudGW(%s, %d)!\n", __FUNCTION__, __LINE__, pcloud_gw_info->gwip, pcloud_gw_info->port);
								break;
					        }else{
					            ERR_PRINT("ERROR ANDLINK (%s)%d resolve hostname \"%s\" fail\n", __FUNCTION__, __LINE__, tmpBuff);				            
					        }	
						}
						pchar = strtok(NULL, ":"); //end while
					}
				}
			}else{
				ERR_PRINT("ERROR ANDLINK (%s)%d wrong rsp from CloudGW\n", __FUNCTION__, __LINE__);	
				return;
			}
		}
	}else{
		ERR_PRINT("ERROR ANDLINK (%s)%d wrong rsp from CloudGW\n", __FUNCTION__, __LINE__);	
		return;
	}

	return;
}

#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
void Andlink_storeRemoteGwInfo(cJSON *root, Andlink_basicInfo *info)
{
    cJSON *item=NULL;
    char tmp_addr[512];
    char tmp_ip[32];
	char tmpBuff[128];
    char *pchar;
    struct in_addr ip;
    struct hostent *hp;
    int i=0, resolve_success=0, index=1, flag=0;

    item = cJSON_GetObjectItem(root, "user_key");
	if(item){
	    DEBUG("ANDLINK (%s)%d store user_key of CGW by using app!\n", __FUNCTION__, __LINE__);
	    strncpy(info->user_key, item->valuestring, sizeof(info->user_key));
	    apmib_set(MIB_RTL_LINK_USER_KEY, (void *)info->user_key);

		//set autoboot flag as 2
		flag = 2;
		apmib_set(MIB_RTL_LINK_CGW_AUTOBOOT_FLAG, (void *)&flag);
	}else{
	    DEBUG("ANDLINK (%s)%d store user_key of CGW by using default value!\n", __FUNCTION__, __LINE__);
		apmib_get(MIB_RTL_LINK_CGW_AUTOBOOT_DEFAULT_USERKEY, (void *)info->user_key);
	    apmib_set(MIB_RTL_LINK_USER_KEY, (void *)info->user_key);
		
		//set autoboot flag as 1
		flag = 1;
		apmib_set(MIB_RTL_LINK_CGW_AUTOBOOT_FLAG, (void *)&flag);
	}

    item = cJSON_GetObjectItem(root, "gwAddress");
	if(!item){
		ERR_PRINT("ERROR ANDLINK (%s)%d wrong rsp from CloudGW\n", __FUNCTION__, __LINE__);	
		return;
	}
    strcpy(tmp_addr, item->valuestring);
    pchar = strstr(tmp_addr, ":");
    if(pchar){
        *pchar = '\0';
    }

    if(is_valid_ipv4_addr(tmp_addr)){
        strcpy(tmp_ip, tmp_addr);
        resolve_success = 1;
    }else{
        hp = gethostbyname(tmp_addr);
        if(hp != NULL){
            while(hp->h_addr_list[i] != NULL){
                strcpy(tmp_ip, inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
                i++;
            }
            resolve_success = 1;
        }else{
            ERR_PRINT("ERROR ANDLINK (%s)%d resolve hostname \"%s\" fail\n", __FUNCTION__, __LINE__, tmp_addr);
            resolve_success = 0;
        }
    }    

    if(resolve_success){
        strcpy(info->gwIp_cloud, tmp_ip);
        inet_aton(info->gwIp_cloud, &ip);
#if defined(CONFIG_ANDLINK_IF6_SUPPORT) && !defined(ANDLINK_IF356_COEXIST)
        strcpy(info->gwIp_cloud_udp, tmp_ip);
        inet_aton(info->gwIp_cloud_udp, &ip);
        if(pchar)
    	    info->udp_port=atoi(pchar+1);
#endif

#if 0
	    apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ip);
	    DEBUG("ANDLINK (%s)%d store gwAddress!\n", __FUNCTION__, __LINE__);

		DEBUG("ANDLINK (%s)%d Andlink_setCloudGW!\n", __FUNCTION__, __LINE__);
	    Andlink_setCloudGW();
#endif

		//for andlink_if5 added by elaine
		//"gwAddress2":"https://112.13.96.207:4443"
		//"gwAddress2":"https://112.13.96.208:4443"
#ifdef CONFIG_ANDLINK_IF5_SUPPORT
		item = cJSON_GetObjectItem(root, "gwAddress2");
		if(!item){
			ERR_PRINT("ERROR ANDLINK (%s)%d wrong rsp from CloudGW\n", __FUNCTION__, __LINE__); 
			return;
		}
		strcpy(tmp_ip, item->valuestring);
		pchar = strtok(tmp_ip, "//");
		while(pchar){
			if(index == 1){
				pchar = strtok(NULL, "//");
				index++;
			}else{
				strcpy(tmpBuff, pchar);
				pchar = strstr(tmpBuff, ":");
				if(pchar){
					*pchar = '\0';
				}
				strcpy(info->gwIp_if5, tmpBuff);
				inet_aton(info->gwIp_if5, &ip);
				apmib_set(MIB_RTL_LINK_CGW_IP_ADDR_IF5, (void *)&ip);

				info->port_if5=atoi(pchar+1);
				apmib_set(MIB_RTL_LINK_CGW_PORT_IF5, (void *)&info->port_if5);
			
				DEBUG("ANDLINK (%s)%d store gwAddress2!\n", __FUNCTION__, __LINE__);
				pchar = strtok(NULL, ":");
			}
		}
#endif
	}else{
		DEBUG("ANDLINK (%s)%d Andlink_setHostGW!\n", __FUNCTION__, __LINE__);
        Andlink_setHostGW();
	}

    apmib_update(CURRENT_SETTING);
}

int Andlink_wpsButtonPressed(void)
{
    char tmpbuf;
	FILE *fp;
	char line[20];
	int pressed=0;

	if ((fp = fopen("/proc/gpio", "r")) != NULL) {
		fgets(line, sizeof(line), fp);
		if (sscanf(line, "%c", &tmpbuf)) {
			if (tmpbuf == '0')
				pressed = 0;
			else
				pressed = 1;
		} else
			pressed = 0;
		fclose(fp);
	} else
		pressed = 0;

    return pressed;
}
#endif

void Andlink_handle_bootfail(Andlink_basicInfo *info)
{
    char empty[1] = "";
    char tmpstr[33];
    apmib_get(MIB_RTL_LINK_SYNCCODE, (void *)tmpstr);
    if(!strcmp(tmpstr, "0") || !tmpstr[0])
        return;
    strcpy(info->synccode, "0");
    apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)empty);
    apmib_update(CURRENT_SETTING);
}

int Andlink_switchRepeater(Andlink_basicInfo *info)
{
    int ret = 0;
    int i, band, enable, tmpvalue;
    int value, rpt_enabled = 1;
    int enable_mib, ssid_mib;
    int enable_back_mib, mode_back_mib, enable_back_bit, mode_back_bit;
    
    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
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

#if 0        
        if(info->rtl_link_mode == RTL_LINK_REPEATER){
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
                ret |= 0x1<<(i*(NUM_VWLAN_INTERFACE+1) + vwlan_idx);;
            }              
        }else 
#endif
        if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW){
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
    if(info->rtl_link_mode == RTL_LINK_REPEATER){
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
    
    return ret;
}

int Andlink_switchDHCP(Andlink_basicInfo *info)
{
    int dhcp, auto_dhcp, ret=0;
    
    apmib_get(MIB_DHCP, (void *)&dhcp);
    apmib_get(MIB_RTL_LINK_AUTO_DHCP, (void *)&auto_dhcp);

    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_REPEATER){
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
    }else if(info->rtl_link_mode == RTL_LINK_GW){
        //DHCP SERVER
        if(dhcp != DHCP_LAN_SERVER)
        {
            DEBUG("(%s)%d change DHCP MODE to DHCP SERVER\n", __FUNCTION__, __LINE__);
            rtl_link_setRecoverMib(MIB_RTL_LINK_DHCP_BACKUP, dhcp, DHCP_BIT);
            
            dhcp = DHCP_LAN_SERVER;
            apmib_set(MIB_DHCP, (void *)&dhcp);
            ret = 0x1<<30;
        }
    }else if(info->rtl_link_mode == WISP_MODE){
         ERR_PRINT("ERROR (%s)%d Donot support WISP MODE!\n", __FUNCTION__, __LINE__);
    }
    return ret;
}

int Andlink_switchDHCP_static(Andlink_basicInfo *info)
{
    int dhcp, ret=0;
    struct in_addr addr, addr_ori;

    apmib_get(MIB_DHCP, (void *)&dhcp);

    if(info->simulate_gw){
        if(!inet_aton(info->staticIp, &addr)){
            ERR_PRINT("ERROR (%s)%d Invalid IP Address!\n", __FUNCTION__, __LINE__);
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
            DEBUG("(%s)%d change IP ADDR to %s\n", __FUNCTION__, __LINE__, info->staticIp);
            apmib_set(MIB_IP_ADDR, (void *)&addr);
            ret = 0x1<<30;
        }
    }else{
        if(dhcp == DHCP_DISABLED){
            DEBUG("(%s)%d change DHCP MODE to DYNAMIC\n", __FUNCTION__, __LINE__);
            ret = Andlink_switchDHCP(info);
        }
    }

    return ret;
}

int Andlink_switchMode(Andlink_basicInfo *info)
{
    int opmode, ret=0;

	opmode = info->opmode;
    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_REPEATER){
        if(info->opmode != BRIDGE_MODE)
        {        	
            DEBUG("(%s)%d change OP MODE to BRIDGE MODE\n", __FUNCTION__, __LINE__);
            rtl_link_setRecoverMib(MIB_RTL_LINK_OPMODE_BACKUP, opmode, OPMODE_BIT);
            
            opmode = BRIDGE_MODE;
            apmib_set(MIB_OP_MODE, (void *)&opmode);
            info->opmode = BRIDGE_MODE;
            ret = 0x1<<30;
        }else{
            system("ifconfig eth1 0.0.0.0");    //清除路由表，否则可能造成br0无法联网
        }
    }else if(info->rtl_link_mode == RTL_LINK_GW){
        if(info->opmode != GATEWAY_MODE)
        {
            DEBUG("(%s)%d change OP MODE to GATEWAY MODE\n", __FUNCTION__, __LINE__);
            rtl_link_setRecoverMib(MIB_RTL_LINK_OPMODE_BACKUP, opmode, OPMODE_BIT);
            
            opmode = GATEWAY_MODE;
            apmib_set(MIB_OP_MODE, (void *)&opmode);
            info->opmode = GATEWAY_MODE;
            ret = 0x1<<30;
        }
    }

    return ret;
}

int Andlink_need_switch_pppoe()
{
	int mode=0, ret=0;	

	apmib_get(MIB_RTL_LINK_OPMODE_BACKUP, (void *)&mode);
	if(mode==GATEWAY_MODE)
		ret = 1;

	return ret;
}

int Andlink_switchMode_DHCP_ext(Andlink_basicInfo *info, int opmode, int enable_rpt, int err)
{
    int ret=0, ori_mode, repeater_mode=0, i=0, dhcp, auto_dhcp, rtl_link_mode;
	char ifname[15]={0};

	apmib_get(MIB_OP_MODE, (void *)&ori_mode);
	apmib_get(MIB_REPEATER_MODE, (void *)&repeater_mode);
	apmib_get(MIB_DHCP, (void *)&dhcp);	
	apmib_get(MIB_RTL_LINK_AUTO_DHCP, (void *)&auto_dhcp);
	
	/*change mode*/
	if(ori_mode != opmode){
		apmib_set(MIB_OP_MODE, (void *)&opmode);
		info->opmode = opmode;
		ret = 0x1<<30; 		
		DEBUG("(%s)%d change OP MODE to %s MODE due to %s\n", __FUNCTION__, __LINE__, (opmode==GATEWAY_MODE)?"GATEWAY":"Bridge", Err2str(err));
	}
	
	/*disable vxd*/
	if((repeater_mode) && (!enable_rpt)){
		DEBUG("(%s)%d disable repeater mode\n", __FUNCTION__, __LINE__);		
		repeater_mode = 0;
		apmib_set(MIB_REPEATER_MODE, (void *)&repeater_mode);

		for(i=0; i<NUM_WLAN_INTERFACE; ++i){
			snprintf(ifname, sizeof(ifname), "wlan%d-vxd", i);
			rtk_wlan_set_wlan_enable(ifname, repeater_mode);
		}
		apmib_set(MIB_REPEATER_ENABLED1, (void *)&repeater_mode);
		if(NUM_WLAN_INTERFACE>1)
			apmib_set(MIB_REPEATER_ENABLED2, (void *)&repeater_mode);
		ret = 0x1<<30;		
	}

	/*change dhcp*/   
    if(opmode==BRIDGE_MODE){
#ifdef CONFIG_AUTO_DHCP_CHECK        
        if(auto_dhcp){
            //DHCP SERVER for AUTO DHCP
            if(dhcp != DHCP_LAN_SERVER){
                DEBUG("(%s)%d change DHCP MODE to DHCP SERVER for Auto DHCP\n", __FUNCTION__, __LINE__);            
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
                dhcp = DHCP_LAN_CLIENT;
                apmib_set(MIB_DHCP, (void *)&dhcp);
                ret = 0x1<<30;
            }
        }
    }else if(opmode==GATEWAY_MODE){
        //DHCP SERVER
        if(dhcp != DHCP_LAN_SERVER)
        {
            DEBUG("(%s)%d change DHCP MODE to DHCP SERVER\n", __FUNCTION__, __LINE__);            
            dhcp = DHCP_LAN_SERVER;
            apmib_set(MIB_DHCP, (void *)&dhcp);
            ret = 0x1<<30;
        }
    }else if(opmode == WISP_MODE){
         ERR_PRINT("ERROR (%s)%d Donot support WISP MODE!\n", __FUNCTION__, __LINE__);
    }

#if 0
	/*change rtl_link mode*/
	apmib_get(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
	if(opmode==GATEWAY_MODE && rtl_link_mode!=RTL_LINK_GW){
		rtl_link_mode = RTL_LINK_GW;
		apmib_set(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
		ret = 0x1<<30;
		DEBUG("(%s)%d change RTL_LINK MODE to GATEWAY MODE\n", __FUNCTION__, __LINE__);  
	}else if(opmode==BRIDGE_MODE && rtl_link_mode!=RTL_LINK_BRIDGE){
		rtl_link_mode = RTL_LINK_BRIDGE;
		apmib_set(MIB_RTL_LINK_MODE, (void *)&rtl_link_mode);
		ret = 0x1<<30;
		DEBUG("(%s)%d change RTL_LINK MODE to BRIDGE MODE\n", __FUNCTION__, __LINE__);
	}	
#endif

    return ret;
}

int Andlink_getIF6CloudGW_Info(ANDLINK_CLOUD_GW_INFO *pcloud_gw_info)
{
	unsigned char area[RTL_LINK_MAX_INFO_LEN]={0};
	RTL_LINK_AREA_ENTRY_T entry;
	int i=0, entrynum=0;

	memset(pcloud_gw_info, 0, sizeof(ANDLINK_CLOUD_GW_INFO));
	memset(&entry, 0, sizeof(RTL_LINK_AREA_ENTRY_T));
	apmib_get(MIB_RTL_LINK_AREA, (void *)area);
	apmib_get(MIB_RTL_LINK_AREA_TBL_NUM, (void *)&entrynum);
	//printf("===[%s %d]area:%s, entrynum:%d\n", __FUNCTION__,__LINE__,area, entrynum);
	
	for(i=1; i<=entrynum; ++i){
		memset(&entry, 0, sizeof(RTL_LINK_AREA_ENTRY_T));
		*((char *)&entry) = (char)i;
		apmib_get(MIB_RTL_LINK_AREA_TBL, (void *)&entry);		
		//printf("===[%s %d][%d]area:%s, entry.area:%s, ip_domain:%s\n", __FUNCTION__,__LINE__, i,area, entry.rtl_link_area_code, entry.rtl_link_area_ip_domain);
		if(!strcmp(entry.rtl_link_area_code, area)){
			pcloud_gw_info->port = entry.rtl_link_area_port;
			if(is_valid_ipv4_addr(entry.rtl_link_area_ip_domain))
				strcpy(pcloud_gw_info->gwip, entry.rtl_link_area_ip_domain);
			else
				strcpy(pcloud_gw_info->domain, entry.rtl_link_area_ip_domain);			
			break;
		}
	}
	//printf("===[%s %d]pcloud_gw_info gw_ip:%s, domain:%s, port:%d\n", __FUNCTION__,__LINE__, pcloud_gw_info->gwip, pcloud_gw_info->domain, pcloud_gw_info->port);

	return 0;
}

static int Andlink_getBasicInfo(Andlink_basicInfo *info)
{
    struct in_addr ip;
    int enable1, enable2, band, value;
    char andlink_version[33], wlanif[16];
    int i;
	ANDLINK_CLOUD_GW_INFO if6_cloud_gw_info;
    
    apmib_get(MIB_RTL_LINK_VENDOR, (void *)info->vendor);
    apmib_get(MIB_RTL_LINK_DEVMODEL, (void *)info->model);
    apmib_get(MIB_RTL_LINK_HWVERSION, (void *)info->hwversion);
    apmib_get(MIB_RTL_LINK_SWVERSION, (void *)info->swversion);   
    apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)info->sn); //MIB_RTL_LINK_SN -> CUSTOMER_HW_SERIAL_NUM
    apmib_get(MIB_RTL_LINK_SYNCCODE, (void *)&info->synccode);
    if(!info->synccode[0]){
        strcpy(info->synccode, "0");
    }
    apmib_get(MIB_RTL_LINK_DEVTYPE, (void *)info->deviceType);
    apmib_get(MIB_RTL_LINK_PRODTOKEN, (void *)info->productToken);
    apmib_get(MIB_RTL_LINK_DEVID, (void *)info->deviceId);
    apmib_get(MIB_RTL_LINK_DEVTOKEN, (void *)info->deviceToken);
//#ifdef CONFIG_ANDLINK_V3_SUPPORT
    apmib_get(MIB_RTL_LINK_GWTOKEN, (void *)info->gwToken_host);
    apmib_get(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip);
    strcpy(info->gwIp_host, inet_ntoa(ip));
    apmib_get(MIB_RTL_LINK_CGWTOKEN, (void *)info->gwToken_cloud);
    apmib_get(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ip);
    strcpy(info->gwIp_cloud, inet_ntoa(ip));
	
	/*****add by elaine for andlink if5******/
	apmib_get(MIB_RTL_LINK_CGW_IP_ADDR_IF5, (void *)&ip);
    strcpy(info->gwIp_if5, inet_ntoa(ip));
	apmib_get(MIB_RTL_LINK_CGW_PORT_CLOUD, (void *)&info->port_cloud);
	apmib_get(MIB_RTL_LINK_CGW_PORT_IF5, (void *)&info->port_if5);
	apmib_get(MIB_RTL_LINK_CGWTOKEN_IF5, (void *)&info->gwToken_if5);
	apmib_get(MIB_RTL_LINK_CGW_MQTT_USER, (void *)info->mqttUser);
	apmib_get(MIB_RTL_LINK_CGW_MQTT_CLIENTID, (void *)info->mqttClientId);
//	apmib_get(MIB_RTL_LINK_CGW_MQTT_URL, (void *)info->mqttUrl);
	apmib_get(MIB_RTL_LINK_CGW_MQTT_IP_ADDR, (void *)info->mqtt_ip);
	apmib_get(MIB_RTL_LINK_CGW_MQTT_PORT, (void *)&info->mqtt_port);
	apmib_get(MIB_RTL_LINK_CGW_MQTT_PASSWORD, (void *)info->mqttPassword);
	apmib_get(MIB_RTL_LINK_CGW_MQTT_KEEPALIVE, (void *)&info->mqttKeepalive);
	apmib_get(MIB_RTL_LINK_CGW_AUTOBOOT_URL, (void *)info->autoBootUrl);
	apmib_get(MIB_RTL_LINK_CGW_AUTOBOOT_FLAG, (void *)&info->autoBootFlag);
	/*******************************/
	
    apmib_get(MIB_RTL_LINK_USER_KEY, (void *)info->user_key);
//#else
    apmib_get(MIB_RTL_LINK_GWTOKEN, (void *)info->gwToken);
    apmib_get(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip);
    strcpy(info->gwIp, inet_ntoa(ip));    
//#endif
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    apmib_get(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ip);
    strcpy(info->gwIp_cloud_udp, inet_ntoa(ip));
    apmib_get(MIB_RTL_LINK_CGW_MS_IP_ADDR, (void *)&ip);
	if(!strcmp(inet_ntoa(ip),"0.0.0.0"))//if message server ip NULL,use server ip
		strcpy(info->gwIp_cloud_tcp, info->gwIp_cloud_udp);
	else
    	strcpy(info->gwIp_cloud_tcp, inet_ntoa(ip));
    apmib_get(MIB_RTL_LINK_USER_KEY, (void *)info->user_key);
    apmib_get(MIB_RTL_LINK_CGW_UDPPORT, (void *)&info->udp_port);
    apmib_get(MIB_RTL_LINK_CGW_TCPPORT, (void *)&info->tcp_port);

	memset(&if6_cloud_gw_info, 0, sizeof(ANDLINK_CLOUD_GW_INFO));
	Andlink_getIF6CloudGW_Info(&if6_cloud_gw_info);
	strcpy(info->gwDomain_udp, if6_cloud_gw_info.domain);	 //maybe empty
	if(!strcmp(info->gwIp_cloud_udp, "0.0.0.0")){	
		strcpy(info->gwIp_cloud_udp, if6_cloud_gw_info.gwip); //maybe empty				
	}	
	if(!info->udp_port)
		info->udp_port = if6_cloud_gw_info.port;			//maybe empty	
	if(!strcmp(info->gwIp_cloud_tcp, "0.0.0.0"))
		strcpy(info->gwIp_cloud_tcp, info->gwIp_cloud_udp); //tcp & udp ip check is done when if6 boot
	
//#endif
    apmib_get(MIB_RTL_LINK_ANDLINKTOKEN, (void *)info->andlinkToken);
    apmib_get(MIB_RTL_LINK_ANDLINK_VER, (void *)andlink_version);
	
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(ANDLINK_IF356_COEXIST)
    if(!strcasecmp(andlink_version, "v3.0b")){
        info->version = 0X300B;
        version = 0X300B;
    }else if(!strcasecmp(andlink_version, "v3.10c")){
    	info->version = 0X310C;
        version = 0X310C;
    }else{
        info->version = 0X310C;
        version = 0X310C;
    }
#else 
    if(!strcasecmp(andlink_version, "v2.10f")){
        info->version = 0X210F;
        version = 0X210F;
    }else if(!strcasecmp(andlink_version, "v2.10e")){
        info->version = 0X210E;
        version = 0X210E;
    }else if(!strcasecmp(andlink_version, "v2.10b")){
        info->version = 0X210B;
        version = 0X210B;
    }else{
        info->version = 0X210F;
        version = 0X210F;
    }
#endif
    DEBUG("(%s)%d And-link version: v%x\n", __FUNCTION__, __LINE__, version);	
	
    apmib_get(MIB_RTL_LINK_MODE, (void *)&info->rtl_link_mode); //andlink的工作模式
    apmib_get(MIB_RTL_LINK_HBTIME, (void *)&info->HBTime);
    apmib_get(MIB_RTL_LINK_HBPORT, (void *)&info->HBPort);
    apmib_get(MIB_RTL_LINK_NEED_RECOVERY, (void *)&info->need_recover);
    apmib_get(MIB_RTL_LINK_SEARCH_GW_MAX_RETRY, (void *)&info->search_gw_max_retry);
    apmib_get(MIB_RTL_LINK_CONNECT_GW_TIMEOUT, (void *)&info->connect_gw_timeout);
    apmib_get(MIB_RTL_LINK_GETIP_TIMEOUT, (void *)&info->getip_timeout);
    apmib_get(MIB_RTL_LINK_SEARCH_ACK, (void *)&info->search_ack);
    apmib_get(MIB_RTL_LINK_SEARCH_ASYNC, (void *)&info->search_async);
    apmib_get(MIB_RTL_LINK_FW_UPGRADE, (void *)&info->notify_upgrade);
    apmib_get(MIB_RTL_LINK_ROAMING_SWITCH, (void *)&info->roaming_switch);
    apmib_get(MIB_RTL_LINK_ROAMING_QOS, (void *)&info->roaming_qos);
    apmib_get(MIB_RTL_LINK_ROAMING_RETRY_RATIO, (void *)&info->retry_ratio);
    apmib_get(MIB_RTL_LINK_ROAMING_FAIL_RATIO, (void *)&info->fail_ratio);
    apmib_get(MIB_RTL_LINK_ROAMING_DETEC_PERIOD, (void *)&info->detect_period);
    apmib_get(MIB_RTL_LINK_ROAMING_RSSI_DELTA, (void *)&info->rssi_delta);
    apmib_get(MIB_RTL_LINK_ROAMING_LOWQUALITY_TIME, (void *)&info->lowquality_check_time);
    apmib_get(MIB_RTL_LINK_ROAMING_NEIGHBOR_TIME, (void *)&info->neighbor_check_time);
    apmib_get(MIB_RTL_LINK_ROAMING_RATE_TH, (void *)&info->roaming_rate_th);
    apmib_get(MIB_RTL_LINK_ROAMING_DISMISS_TIME, (void *)&info->dismiss_time);
    apmib_get(MIB_RTL_LINK_AUTO_DHCP, (void *)&info->auto_dhcp);
    apmib_get(MIB_RTL_LINK_LOCAL_IP_ADDR, (void *)&ip);
    strcpy(info->staticIp, inet_ntoa(ip));
    if(strcmp(info->staticIp, "0.0.0.0"))
        info->simulate_gw=1;
    apmib_get(MIB_RTL_LINK_ROAMING_DEBUG_ON, (void *)&value);
    roaming_debug_on = value;
	apmib_get(MIB_RTL_LINK_AREA, (void *)info->area_code);
    apmib_get(MIB_OP_MODE, (void *)&info->opmode);
    apmib_get(MIB_WISP_WAN_ID, (void *)&info->wisp_idx);
    apmib_get(MIB_REPEATER_ENABLED1, (void *)&enable1);
    apmib_get(MIB_REPEATER_ENABLED2, (void *)&enable2);
    info->vxd_up = enable1 || enable2;

    if(rtl_link_getLanMacAddr(info->lan_mac))
    {
        DEBUG("(%s)%d get dut mac failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    info->idx_2g=-1;
    info->idx_5g=-1;

    apmib_save_wlanIdx();
    for(i=0; i<NUM_WLAN_INTERFACE; i++){
        wlan_idx = i;
        vwlan_idx = 0;
        apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
        sprintf(wlanif, "wlan%d", i);
        if(band == PHYBAND_2G){            
            apmib_get(MIB_WLAN_RSSI_TH, (void *)&info->rssi_th_2g);
            info->idx_2g = i;
            if(rtl_link_getWlanMacAddr(info->wlan_mac_2g, wlanif))
            {
                DEBUG("(%s)%d get dut mac failed\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }else{
            apmib_get(MIB_WLAN_RSSI_TH, (void *)&info->rssi_th_5g);
            info->idx_5g = i;
            if(rtl_link_getWlanMacAddr(info->wlan_mac_5g, wlanif))
            {
                DEBUG("(%s)%d get dut mac failed\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }
    }
    apmib_recov_wlanIdx();

	apmib_get(MIB_IS_DEFAULT_SETTING,(void *)&info->default_setting_poweron);
	info->poweron = 1;

	info->skip_coapping = 0;
	info->skip_online = 0;
	info->skip_inform_success = 0;
	info->skip_check_devId = 1;
	info->skip_hb_rspto = 1;
	info->skip_encrypt_reg = 0;

    info->basic_size = sizeof(Andlink_basicInfo);
}

static void show_usage(void)
{
    printf("Usage: andlink [argument]\n");
    printf("    Where arguments is optional as:\n");
    printf("\t-m andlink type val, 0: server mode, 1: client mode\n");
    printf("\t-d debug mode val, 0: no msg, 1: show error msg, 2: show error and debug msg\n");
    printf("\t-u uplink type val, 0: wired, 1:wireless\n");
    printf("\t-a ipaddr, the local interface address to use\n");
    printf("\t-o operation mode, 0: bridge mode, 1:gw mode\n");
    printf("\t-h show help msg\n");
}

void *IF3_thread_fn(void *arg)
{
    int ret;
    
    DEBUG("(%s)%d Create IF3 thread\n", __FUNCTION__, __LINE__);
    while(1){
        ret = Andlink_startClient(&basic_info);
        DEBUG("ANDLINK(%s)%d IF3 thread exit and restart ...\n", __FUNCTION__, __LINE__);
        sleep(5); 
    }
    return NULL;
}

int Andlink_support_if()
{
	int ret = 0;
	
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
	ret |= ANDLINK_SUPPORT_IF6;
#endif
#ifdef CONFIG_ANDLINK_IF5_SUPPORT
	ret |= ANDLINK_SUPPORT_IF5;
#endif

	return ret;	
}

#ifdef CONFIG_ANDLINK_IF6_SUPPORT
void *IF6_thread_fn(void *arg)
{
	int ret;

	DEBUG("(%s)%d Create IF6 thread\n", __FUNCTION__, __LINE__);	
	while(1){
    	ret = Andlink_start_cloude_SM(&basic_info);
    	DEBUG("ANDLINK(%s)%d cloude exit and restart ...\n", __FUNCTION__, __LINE__);
    	sleep(5);
	}	
	return NULL;
}

void Andlink_start_if6()
{
	pthread_t IF6_thread;
	
	if(!(basic_info.standingby & ANDLINK_IF6_RUNNING))
	{
    	basic_info.standingby |= ANDLINK_IF6_RUNNING;						
    	if(pthread_create(&IF6_thread,NULL,IF6_thread_fn,NULL) != 0){
       	 	ERR_PRINT("ERROR ANDLINK (%s)%d create IF6 thread failed!\n", __FUNCTION__, __LINE__);
    	}			            		
	}
	return;
}
#endif

#ifdef CONFIG_ANDLINK_IF5_SUPPORT
void Andlink_start_if5()
{						
	basic_info.standingby |= ANDLINK_IF5_RUNNING;
	if(!isFileExist(RTL_LINK_IF5_PID_FILE)){
		DEBUG("\n**********ANDLINK(%s)%d UP andlink_if5 daemon**********\n", __FUNCTION__, __LINE__);
		system("andlink_if5 -d3");
	}
}
#endif

int main(int argc, char **argv)
{
    int arg, pid_fd, mode=1, i, ret=0, ret1, value, wisp_idx=-1;
	char tmpbuf[64];
	int dev_mode;
#ifdef CONFIG_ANDLINK_IF6_SUPPORT
	pthread_t IF3_poll_thread;
	int start_if6=-1;
	char cmd_buf[128];	
	pthread_t IF6_poll_thread;
#endif

//#ifdef DMALLOC
//    dmalloc_debug_setup("debug=0x4000503,log=/var/ac_dmalloc.log");
//#endif    
    for(;;){
        arg = getopt(argc, argv, "m:d:h");
        if(arg < 0)
            break;
        switch(arg){
        case 'm':
            mode = atoi(optarg);
            break;
        case 'd':
            debug_mode = atoi(optarg);
            break;
        case 'h':
            show_usage();
            return 0;
        default:
            show_usage();
            return 0;
        }
    }
    
    if(daemon(0, 1)){
		ERR_PRINT("Daemon init error\n");
		return 0;
    }

    pid_fd = pidfile_acquire(RTL_LINK_PID_FILE);
    if(pid_fd < 0)
        return 0;
    pidfile_write_release(pid_fd);
    
    if (!apmib_init()) {
		ERR_PRINT("ERROR ANDLINK (%s)%d Initialize AP MIB failed!\n", __FUNCTION__, __LINE__);
		unlink(RTL_LINK_PID_FILE);
		return 0;
	}

	signal(SIGCHLD, SIG_IGN);
	memset(&basic_info, 0, sizeof(basic_info));

	if(mode == 1){
	    for(i=0; i<(sizeof(endpoints)/sizeof(Andlink_endpoint)); i++){
	        Andlink_register_endpoints(endpoints[i]);
	    }		

        while(1){
            ret = 0;
			
            //if(!basic_info.basic_size)
	        	Andlink_getBasicInfo(&basic_info);

			snprintf(tmpbuf, sizeof(tmpbuf), "echo 1 > %s", ANDLINK_SET_MIB_DIR);
            system(tmpbuf);
			
            /*switch dhcp/repeater/mode start*/
            if(basic_info.opmode==GATEWAY_MODE && basic_info.rtl_link_mode==RTL_LINK_REPEATER){
                //gateway -> repeater
                ret |= Andlink_switchMode(&basic_info);
                ret |= Andlink_switchRepeater(&basic_info);
                ret |= Andlink_switchDHCP(&basic_info);
            }else if(basic_info.opmode==BRIDGE_MODE && basic_info.vxd_up && basic_info.rtl_link_mode==RTL_LINK_GW){
                //repeater -> gateway
                ret |= Andlink_switchMode(&basic_info);
                ret |= Andlink_switchRepeater(&basic_info);
                ret |= Andlink_switchDHCP(&basic_info);
            }else{
                //bridge <-> repeater
                ret |= Andlink_switchRepeater(&basic_info);
            }
            //gateway <-> bridge do mode change after search gw

            //for simulated gw
            ret1 = Andlink_switchDHCP_static(&basic_info);
            if(ret1 == -1)
                return;
            else
                ret |= ret1;
	        if(basic_info.simulate_gw){
	            ret |= Andlink_switchMode(&basic_info);
	        }
			
            rtl_link_restart(ret);
            /*switch dhcp/repeater/mode end*/
			
			unlink(ANDLINK_SET_MIB_DIR);

/*marked by andlink new spec(20190329)*/
#if 0//defined(CONFIG_ANDLINK_IF6_SUPPORT) || defined(ANDLINK_IF356_COEXIST) 
			if(basic_info.opmode==GATEWAY_MODE && basic_info.rtl_link_mode == RTL_LINK_GW){			
#if 0//defined(ANDLINK_IF356_COEXIST)
				// what about if5 ip and port ???
				basic_info.standingby |= ANDLINK_IF5_RUNNING;
				if(!isFileExist(RTL_LINK_IF5_PID_FILE)){					
					DEBUG("\n**********ANDLINK(%s)%d UP andlink_if5 daemon**********\n", __FUNCTION__, __LINE__);
					system("andlink_if5 -d3");
				}
#endif
#if defined(CONFIG_ANDLINK_IF6_SUPPORT)
			    while(1){
				    if(!(basic_info.standingby & ANDLINK_IF6_RUNNING)){
	    			    basic_info.standingby |= ANDLINK_IF6_RUNNING;	
						basic_info.standingby |= ANDLINK_IF3_RUNNING;
	                    if(pthread_create(&IF3_poll_thread,NULL,IF3_thread_fn,NULL) != 0){
	                        ERR_PRINT("ERROR ANDLINK (%s)%d create IF3 thread failed!\n", __FUNCTION__, __LINE__);
	                    }				
	                }
					DEBUG("\n**********ANDLINK(%s)%d UP andlink_if6 daemon**********\n", __FUNCTION__, __LINE__);
				    ret = Andlink_start_cloude_SM(&basic_info);
				    DEBUG("ANDLINK(%s)%d cloude exit and restart ...\n", __FUNCTION__, __LINE__);
	                sleep(5);
	            }
#endif			
			}else
#endif			
			{				
	            ret = Andlink_startClient(&basic_info);
	            switch(ret){
					case IF5_START_DIRECT: 
					case IF5_GET_NETINFO_SUCCESS:
					case IF5_AUTO_BOOT_SUCCESS:
						/*just need start if5 */
#ifdef CONFIG_ANDLINK_IF5_SUPPORT						
						Andlink_start_if5();					
#endif
						basic_info.standingby |= ANDLINK_IF3_RUNNING;
						break;
                    case SEARCH_GW_TIMEOUT:			
						if(rtl_link_wan_access()==1){
							DEBUG("ANDLINK (%s)%d wan access ok\n", __FUNCTION__,__LINE__);
							rtl_link_set_file_value(RTL_LINK_SWITCH_PPPOE, INTERNET_ACCESS_SUCCESS_AFTER_SEARCH_GW_TO);	
							ret1 = Andlink_switchMode_DHCP_ext(&basic_info, BRIDGE_MODE, 0, INTERNET_ACCESS_SUCCESS_AFTER_SEARCH_GW_TO);
							rtl_link_restart(ret1);								                        	
						}else{
							printf("ANDLINK (%s)%d wan access fail\n", __FUNCTION__,__LINE__);
							if(Andlink_need_switch_pppoe()){
								rtl_link_set_file_value(RTL_LINK_SWITCH_PPPOE, INTERNET_ACCESS_FAIL_AFTER_SEARCH_GW_TO);	
								ret1 = Andlink_switchMode_DHCP_ext(&basic_info, GATEWAY_MODE, 0, INTERNET_ACCESS_FAIL_AFTER_SEARCH_GW_TO);
								rtl_link_restart(ret1);
							}else{
								DEBUG("ANDLINK (%s)%d not support pppoe\n", __FUNCTION__, __LINE__);
							}						
						}												
						break;						
					case OTHERS:                     
                    case SCAN_DEFSSID_TIMEOUT:
    				case CONNECT_WIRELESS_TIMEOUT:
                    case CONNECT_DEFSSID_TIMEOUT:
                    case COAP_RSP_TIMEOUT:
                    case ANDLINK_ERROR:
                    case ANDLINK_TOKEN_MISSMATCH:
    				case WAIT_APP_NETINFO_TIMEOUT:
					case PPPOE_CONNECT_FAIL:
            	        if(basic_info.need_recover)
                        {
                            DEBUG("ANDLINK (%s)%d exit and recover Gateway mode...\n", __FUNCTION__, __LINE__);
                            rtl_link_recoverSettings();
                            value = 0;
                            apmib_set(MIB_RTL_LINK_ENABLE, (void *)&value);
                            apmib_update(CURRENT_SETTING);
            				system("sysconf init gw all");
                            goto EXIT;
                        }else{
                            DEBUG("ANDLINK (%s)%d exit and restart ...\n", __FUNCTION__, __LINE__);
                            sleep(5);   
                        }
                        break;
					case GETIP_TIMEOUT:					
						if(Andlink_need_switch_pppoe()){
							rtl_link_set_file_value(RTL_LINK_SWITCH_PPPOE, ret);	
							ret1 = Andlink_switchMode_DHCP_ext(&basic_info, GATEWAY_MODE, 0, ret);
							rtl_link_restart(ret1);
						}else{
							DEBUG("ANDLINK (%s)%d not support pppoe\n", __FUNCTION__, __LINE__);
						}												
						break;	
					case PPPOE_CONNECT_SUCCESS: /*start if6 & if5*/	
					case START_IF_AFTER_SWITCH_BRIDGE:
						rtl_link_set_file_value(RTL_LINK_SWITCH_PPPOE, ret);	
#ifdef CONFIG_ANDLINK_IF6_SUPPORT						
						Andlink_start_if6();				
#endif
						break;					
                    case ANDLINK_UNBIND:
                        break;
					case COAP_RSP_TIMEOUT_HB: // if3 heartbeat rsp timeout						
						break; 	/* todo */
					default:
						break;
    	        }
    	        if(ret == ANDLINK_UNBIND){
    	            DEBUG("ANDLINK (%s)%d unbind exit andlink ...\n", __FUNCTION__, __LINE__);
    	            unlink(RTL_LINK_PID_FILE);
    	            break;
    	        }
	        }
	    }

	    Andlink_unregister_endpoints();
	} else {
        ;
        //Andlink_startServer();
	}

EXIT:
	
//#ifdef DMALLOC
//	dmalloc_shutdown();
//#endif
    return 0;
       
}


