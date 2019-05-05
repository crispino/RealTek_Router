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
#include <math.h>
#include <pthread.h>

#include "curl/curl.h"
#include "apmib.h"
#include "cJSON.h"

#include "coap_config.h"
#include "coap.h"
//#include "coap-client.h"

#include "rtl_link.h"
#include "rtl_link_utils.h"
#include "rtl_link_list.h"
#include "../andlink_interface.h"
//#include "andlink.h"
#include "andlink-if5.h"
//#include "andlink-if5-share.h"


#ifdef DMALLOC
#include "../../dmalloc-5.5.2/dmalloc.h"
#endif


int debug_mode = 2;
int roaming_debug_on = 0;
int version;
Andlink_basicInfo basic_info;
PHYBAND_TYPE_T qlink_band = PHYBAND_2G;
extern char *optarg;

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


int Andlink_if5_storeLoginInfo(cJSON *root, Andlink_basicInfo *info)
{
    cJSON *item;
    time_t tm;
	char tmp_ip[32], tmpBuff[128];
	char *pchar=NULL;
	struct in_addr ip;

    item = cJSON_GetObjectItem(root, "andlinkToken");
    if(item){
        if(version >= 0X210F) {//2.10f
            if(!strcmp(info->andlinkToken, item->valuestring)){
                DEBUG("ANDLINK_IF5 (%s)%d check andlinkToken OK!\n", __FUNCTION__, __LINE__);
                DEBUG("ANDLINK_IF5 register to cloud platform...\n");
            }else if(!strcmp(item->valuestring, "0000000000000000")){
                strcpy(info->andlinkToken, item->valuestring);
                //apmib_set(MIB_RTL_LINK_ANDLINKTOKEN, (void *)info->andlinkToken);
                DEBUG("ANDLINK_IF5 register to local GW...\n");
            }else{
                ERR_PRINT("ERROR ANDLINK(%s)%d andlinkToken missmatch\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }else{
            if(strcmp(info->andlinkToken, item->valuestring)){
                ERR_PRINT("ERROR ANDLINK(%s)%d andlinkToken missmatch\n", __FUNCTION__, __LINE__);
                return -1;
            }else{
                DEBUG("ANDLINK_IF5 (%s)%d check andlinkToken OK!\n", __FUNCTION__, __LINE__);
            }
        }
    }

    item = cJSON_GetObjectItem(root, "deviceId");
    if(item){
        strncpy(info->deviceId, item->valuestring, sizeof(info->deviceId));
        apmib_set(MIB_RTL_LINK_DEVID, (void *)info->deviceId);
        DEBUG("ANDLINK_IF5 (%s)%d store deviceId!\n", __FUNCTION__, __LINE__);
    }

    item = cJSON_GetObjectItem(root, "deviceToken");
    if(item){
        strncpy(info->deviceToken, item->valuestring, sizeof(info->deviceToken));
        apmib_set(MIB_RTL_LINK_DEVTOKEN, (void *)info->deviceToken);
        DEBUG("ANDLINK_IF5 (%s)%d store deviceToken!\n", __FUNCTION__, __LINE__);
    }

    item = cJSON_GetObjectItem(root, "heartBeatTime");
    if(item){
        info->HBTime = item->valueint/1000;
        apmib_set(MIB_RTL_LINK_HBTIME, (void *)&info->HBTime);
        DEBUG("ANDLINK_IF5 (%s)%d store HBTime!\n", __FUNCTION__, __LINE__);
    }

    item = cJSON_GetObjectItem(root, "timestamp");
    if(item){
        tm = item->valueint;
        stime(&tm);
    }

   
    item = cJSON_GetObjectItem(root, "gwToken");
    if(item){
	    strncpy(info->gwToken_if5, item->valuestring, sizeof(info->gwToken_if5));
	    apmib_set(MIB_RTL_LINK_CGWTOKEN_IF5, (void *)info->gwToken_if5);
        DEBUG("ANDLINK_IF5 (%s)%d store gwToken!\n", __FUNCTION__, __LINE__);
    }
    
	item = cJSON_GetObjectItem(root, "mqttUser");
	if(item){
		strncpy(info->mqttUser, item->valuestring, sizeof(info->mqttUser));
		apmib_set(MIB_RTL_LINK_CGW_MQTT_USER, (void *)info->mqttUser);
		DEBUG("ANDLINK_IF5 (%s)%d store mqttUser!\n", __FUNCTION__, __LINE__);
	}

	item = cJSON_GetObjectItem(root, "mqttClientId");
	if(item){
		strncpy(info->mqttClientId, item->valuestring, sizeof(info->mqttClientId));
		apmib_set(MIB_RTL_LINK_CGW_MQTT_CLIENTID, (void *)info->mqttClientId);
		DEBUG("ANDLINK_IF5 (%s)%d store mqttClientId!\n", __FUNCTION__, __LINE__);
	}

	item = cJSON_GetObjectItem(root, "mqttUrl");
	if(item){
//		strncpy(info->mqttUrl, item->valuestring, sizeof(info->mqttUrl));
//		apmib_set(MIB_RTL_LINK_CGW_MQTT_URL, (void *)info->mqttUrl);
//		DEBUG("ANDLINK_IF5 (%s)%d store mqttUrl!\n", __FUNCTION__, __LINE__);

		//ssl://cgw.komect.com:8883
		strncpy(tmp_ip, item->valuestring, sizeof(tmp_ip));
		pchar = strtok(tmp_ip, "//");
		pchar = strtok(NULL, "//");

		while(pchar){
			strncpy(tmpBuff, pchar, sizeof(tmpBuff));
			pchar = strstr(tmpBuff, ":");
			if(pchar){
				*pchar = '\0';
			}
			strcpy(info->mqtt_ip, tmpBuff);
			apmib_set(MIB_RTL_LINK_CGW_MQTT_IP_ADDR, (void *)info->mqtt_ip);

			info->mqtt_port=atoi(pchar+1);
			apmib_set(MIB_RTL_LINK_CGW_MQTT_PORT, (void *)&info->mqtt_port);
			DEBUG("ANDLINK (%s)%d store mqtt ip address and port!\n", __FUNCTION__, __LINE__);
			pchar = strtok(NULL, ":");
		}
	}

	item = cJSON_GetObjectItem(root, "mqttPassword");
	if(item){
		strncpy(info->mqttPassword, item->valuestring, sizeof(info->mqttPassword));
		apmib_set(MIB_RTL_LINK_CGW_MQTT_PASSWORD, (void *)info->mqttPassword);
		DEBUG("ANDLINK_IF5 (%s)%d store mqttPassword!\n", __FUNCTION__, __LINE__);
	}

	item = cJSON_GetObjectItem(root, "mqttKeepalive");
	if(item){
		info->mqttKeepalive = item->valueint;
		apmib_set(MIB_RTL_LINK_CGW_MQTT_KEEPALIVE, (void *)&info->mqttKeepalive);
		DEBUG("ANDLINK_IF5 (%s)%d store mqttKeepalive!\n", __FUNCTION__, __LINE__);
	}

    apmib_update(CURRENT_SETTING); 
      
    return 0;
}


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

ERR_NUM_T Andlink_if5_isLanDev(MacNode *node)
{
	MACFILTER_T entry, entry_tmp;
	ERR_NUM_T err = SUCCESS;
	char str_mac[32] = {0};
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

ERR_NUM_T Andlink_if5_getApConfigInfo(cJSON *data)
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
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    int workmode=0;
//#endif
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
    
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    rtl_link_getWorkMode(&workmode);
//#endif

    if(version >= 0X210E){
        cJSON_AddStringToObject(data, "hardwareVersion", hwversion);
        cJSON_AddStringToObject(data, "softwareVersion", swversion);
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT        
        cJSON_AddNumberToObject(data, "WorkingMode", workmode);
//#endif
		

        snprintf(uptime, sizeof(uptime), "%ld", sys_info.uptime);
        cJSON_AddStringToObject(data, "UpTime", uptime);
        cJSON_AddNumberToObject(data, "MacFilterEnable", macfilter.enable);
        cJSON_AddNumberToObject(data, "MacFilterPolicy", macfilter.policy);
        if(macfilter.mac_list.len > 32)
            macfilter.mac_list.len = 32;
        for(i=0; i<macfilter.mac_list.len; i++){
            macEntry = list_get_element(&macfilter.mac_list, i);
			/**added by elaine, 2019-02-20****/			
			if(i == macfilter.mac_list.len-1){
				snprintf(filter_item, sizeof(filter_item), "%02x%02x%02x%02x%02x%02x/%s", 
					macEntry->macAddr[0], macEntry->macAddr[1], macEntry->macAddr[2], macEntry->macAddr[3], 
					macEntry->macAddr[4], macEntry->macAddr[5], macEntry->comment);
			}else{
			/*******************************/
//#if defined(CONFIG_ANDLINK_V25_SUPPORT)
            	snprintf(filter_item, sizeof(filter_item), "%02x%02x%02x%02x%02x%02x/%s,", 
                macEntry->macAddr[0], macEntry->macAddr[1], macEntry->macAddr[2], macEntry->macAddr[3], 
                macEntry->macAddr[4], macEntry->macAddr[5], macEntry->comment);
			}
//#else
//            snprintf(filter_item, sizeof(filter_item), "%02x:%02x:%02x:%02x:%02x:%02x/%s,", 
//                macEntry->macAddr[0], macEntry->macAddr[1], macEntry->macAddr[2], macEntry->macAddr[3], 
//                macEntry->macAddr[4], macEntry->macAddr[5], macEntry->comment);
//#endif
            strcat(filter_string, filter_item);
        }
        cJSON_AddStringToObject(data, "MacFilterEntries", filter_string);
    }
    cJSON_AddNumberToObject(data, "LEDOnOff", led.on);
    cJSON_AddNumberToObject(data, "RoamingSwitch", roaming.enable);

//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    for(i=0; i<RF.num; i++){
        if(RF.rf[i].band == PHYBAND_2G)
            cJSON_AddNumberToObject(data, "LowRSSI2.4G", roaming.rssi_2g-100);
        else
            cJSON_AddNumberToObject(data, "LowRSSI5G", roaming.rssi_5g-100);
    }
//#else

//    if(roaming.enable){
//        for(i=0; i<RF.num; i++){
//            if(RF.rf[i].band == PHYBAND_2G)
//                cJSON_AddNumberToObject(data, "LowRSSI2.4G", roaming.rssi_2g-100);
//            else
//                cJSON_AddNumberToObject(data, "LowRSSI5G", roaming.rssi_5g-100);
//        }
        
//    }
//#endif

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
        if(config.ap[i].idx==0 || (config.ap[i].idx!=0 && config.ap[i].enable && config.ap[i].idx!=4))  //andlink v3.1 index: 2.4g(1~4) 5g(5~8)
        {
            Node = cJSON_CreateObject();
            cJSON_AddItemToArray(Configurations, Node);
            if(config.ap[i].band == PHYBAND_2G)
                cJSON_AddStringToObject(Node, "Radio", "2.4G"); 
            else
                cJSON_AddStringToObject(Node, "Radio", "5G");
//#ifdef CONFIG_ANDLINK_V3_SUPPORT                
            if(config.ap[i].band == PHYBAND_2G)     //andlink v3.1 index: 2.4g(1~4) 5g(5~8) 
                cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 1);
            else
                cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 5);
//#else
//            cJSON_AddNumberToObject(Node, "Index", config.ap[i].idx + 1);
//#endif
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

//EXIT:
    list_destroy(&macfilter.mac_list, free);
    return SUCCESS;

}

ERR_NUM_T Andlink_if5_getUplinkStatus(cJSON *data)
{
    UplinkStatus uplink;
	char cur_txrate[10], cur_rxrate[10];
    int ret=0;
#ifndef CONFIG_APP_RTK_ADAPTER
    char port[10];
#endif
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    char delim_mac[18]={0};
//#endif

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
        Andlink_macFormat_add_delim(delim_mac, basic_info.wlan_mac_2g);
        cJSON_AddStringToObject(data, "MacAddress", delim_mac);
//#else
//        cJSON_AddStringToObject(data, "MacAddress", basic_info.wlan_mac_2g);
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
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT       
        cJSON_AddStringToObject(data, "RxRate", uplink.rxrate);
        cJSON_AddStringToObject(data, "TxRate", uplink.txrate);
		snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
        cJSON_AddStringToObject(data, "RxRate_rt", cur_rxrate);
        cJSON_AddStringToObject(data, "TxRate_rt", cur_txrate);
//#else
//        snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
//        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
//        cJSON_AddStringToObject(data, "RxRate", cur_rxrate);
//        cJSON_AddStringToObject(data, "TxRate", cur_txrate);
//#endif
    }else{
        cJSON_AddStringToObject(data, "UplinkType", "Ethernet");
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT        
        //Andlink_macFormat_add_delim(delim_mac, basic_info.lan_mac);
        //cJSON_AddStringToObject(data, "MacAddress", delim_mac);
        cJSON_AddStringToObject(data, "MacAddress", basic_info.lan_mac);
        cJSON_AddStringToObject(data, "Radio", "");
        cJSON_AddStringToObject(data, "SSID", "");
        cJSON_AddNumberToObject(data, "Channel", 0);
        cJSON_AddStringToObject(data, "Noise", "");
        cJSON_AddStringToObject(data, "SNR", "");
        cJSON_AddStringToObject(data, "RSSI", "");
//#else
//        cJSON_AddStringToObject(data, "MacAddress", basic_info.lan_mac);
//#endif
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
        cJSON_AddStringToObject(data, "RxRate", uplink.rxrate);
        cJSON_AddStringToObject(data, "TxRate", uplink.txrate);
		snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
        cJSON_AddStringToObject(data, "RxRate_rt", cur_rxrate);
        cJSON_AddStringToObject(data, "TxRate_rt", cur_txrate);
//#else
//        snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)uplink.cur_rxrate*8/1000000);
//        snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)uplink.cur_txrate*8/1000000);
//        cJSON_AddStringToObject(data, "RxRate", cur_rxrate);
//        cJSON_AddStringToObject(data, "TxRate", cur_txrate);
//#endif
    }
    
#ifdef CONFIG_ANDLINK_V3_SUPPORT
   // cJSON_AddStringToObject(data, "IPAddress", uplink.ip); //IF5协议中不需要上报IPAddress
#endif

    return SUCCESS;
}

ERR_NUM_T Andlink_if5_getSTAInfo(cJSON *data)
{
    cJSON *Devices=NULL, *Node=NULL;
    List sta_list;
    STAInfo *sta;
    ERR_NUM_T res=SUCCESS;
    int i, ret=0;
#ifndef CONFIG_APP_RTK_ADAPTER
	int port;
#endif
    char cur_txrate[10], cur_rxrate[10];
//#if defined(CONFIG_ANDLINK_IF6_SUPPORT)
	char delete_delim_mac[13]={0},delete_delim_vmac[13]={0};
//#endif

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
//#if defined(CONFIG_ANDLINK_IF6_SUPPORT)
		Andlink_macFormat(delete_delim_mac, sta->clone_mac);
		cJSON_AddStringToObject(Node, "MacAddress", delete_delim_mac);
		Andlink_macFormat(delete_delim_vmac, sta->mac);
		 cJSON_AddStringToObject(Node, "VMacAddress", delete_delim_vmac);
//#else
//		cJSON_AddStringToObject(Node, "MacAddress", sta->clone_mac); 
//		cJSON_AddStringToObject(Node, "VMacAddress", sta->mac);
//#endif
        
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
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT 
			cJSON_AddStringToObject(Node, "RxRate", sta->rxrate);
		    cJSON_AddStringToObject(Node, "TxRate", sta->txrate);
			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
            snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
			cJSON_AddStringToObject(Node, "RxRate_rt", cur_rxrate);
        	cJSON_AddStringToObject(Node, "TxRate_rt", cur_txrate);
//#else			
//			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
//            snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
//			cJSON_AddStringToObject(Node, "RxRate", cur_rxrate);
//        	cJSON_AddStringToObject(Node, "TxRate", cur_txrate);
//#endif
        }else{
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT 
            cJSON_AddStringToObject(Node, "Radio", "");
            cJSON_AddStringToObject(Node, "SSID", "");
            cJSON_AddStringToObject(Node, "RSSI", "");
//#endif    
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT 
		    cJSON_AddStringToObject(Node, "RxRate", sta->rxrate);
		    cJSON_AddStringToObject(Node, "TxRate", sta->txrate);
			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
			snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
		    cJSON_AddStringToObject(Node, "RxRate_rt", cur_rxrate);
		    cJSON_AddStringToObject(Node, "TxRate_rt", cur_txrate);
//#else
//			snprintf(cur_rxrate, sizeof(cur_rxrate), "%.6f", (float)sta->cur_rxrate*8/1000000);
//			snprintf(cur_txrate, sizeof(cur_txrate), "%.6f", (float)sta->cur_txrate*8/1000000);
//		    cJSON_AddStringToObject(Node, "RxRate", cur_rxrate);
//		    cJSON_AddStringToObject(Node, "TxRate", cur_txrate);
//#endif
        }
    }
	
EXIT:
    list_destroy(&sta_list, free);
    return res;
}

ERR_NUM_T Andlink_if5_getWiFiStats(cJSON *data)
{
    WiFiTrafficStats wifi;
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    LanTrafficStats lan;
//#endif 
    int i, ret=0;
    cJSON *Stats, *Node;

    memset(&wifi, 0, sizeof(wifi));

    ret = rtl_link_getWiFiTrafficStats(&wifi);
    if(ret){
        ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
        return ERROR;
    }
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    ret = rtl_link_getLanTrafficStats(&lan);
	if(ret){
		ERR_PRINT("ERROR (%s)%d error\n", __FUNCTION__, __LINE__);
		return ERROR;
	}
//#endif    

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
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT
    for(i=0; i<lan.num; i++)
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
//#endif    
    return SUCCESS;
}

ERR_NUM_T Andlink_if5_postWiFiParams(cJSON *data)
{   
    int m, i, j;
    int band, ret = 0;
    int tmpvalue, disabled;
    WiFiConfig param;
    cJSON *ap, *conf, *item, *_item;

    memset(&param, 0, sizeof(param));
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
#if defined(CONFIG_ANDLINK_V3_SUPPORT) || defined(CONFIG_ANDLINK_IF6_SUPPORT)
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
            
            item = cJSON_GetObjectItem(ap, "Enable");
            if(item){
                if(item->type == cJSON_Number){
                    tmpvalue = item->valueint;
                }else if(item->type == cJSON_String){
                    sscanf(item->valuestring, "%d", &tmpvalue);
                }
				//enable guest wifi on wlan1-va1, if 2.4G disable, report INVALID_VALUE
				if(tmpvalue == 1){
					if(param.ap[i].idx == 2){  
						apmib_save_wlanIdx();
						for(j=0; j<NUM_WLAN_INTERFACE; j++){
							wlan_idx = j;
							vwlan_idx = 0;
							apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&band);
							if(band == PHYBAND_2G){
								apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disabled);
								if(disabled == 1){
									ERR_PRINT("ERROR ANDLINK(%s)%d wlan%d disabled, can't enable wlan%d-va%d\n", __FUNCTION__, __LINE__, wlan_idx, wlan_idx, vwlan_idx);
									return INVALID_VALUE;
								}
							}
						}
						apmib_recov_wlanIdx();
					}
				}
                param.ap[i].enable = tmpvalue;
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
    ret |= rtl_link_if5_syncWiFiConfig(param);
#ifdef UNIVERSAL_REPEATER
    ret |= rtl_link_syncRepeaterConfig(param);
#endif
    Andlink_set_restart_flag(ret);

    return SUCCESS;  
}

ERR_NUM_T Andlink_if5_postWiFiSwitch(cJSON *data)
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
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Radio item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_syncWiFiSwitch(wifiswitch);
    Andlink_set_restart_flag(ret);
    
    return SUCCESS;  
    //return RECEIVED_SUCCESS;
}

ERR_NUM_T Andlink_if5_postRadioConfig(cJSON *data)
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
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss TransmitPower item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_syncRadioConfig(radio);
    Andlink_set_restart_flag(ret);
    
    return SUCCESS;  
}

ERR_NUM_T Andlink_if5_postWPS(cJSON *data)
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
    
    return SUCCESS;   
}

ERR_NUM_T Andlink_if5_postMacFilter(cJSON *data)
{   
    cJSON *item;
    ERR_NUM_T err = SUCCESS;
    int ret = 0, cnt=1, i=0;
    MacFilter macfilter;
    MacNode *node;
    char *delim = "/,";
    char *tmp_buf;
	char tmp_str[2048];

	memset(&macfilter, 0 ,sizeof(macfilter));
	list_init(&macfilter.mac_list);
	macfilter.v25=1;

    item = cJSON_GetObjectItem(data, "MacFilterEnable");
    if(item){
        macfilter.enable = item->valueint;
    }else{
        err = LACK_OF_ARGUMENT;
        ERR_PRINT("ERROR ANDLINK_IF5 (%s)%d miss MacFilterEnable item\n", __FUNCTION__, __LINE__);
        goto EXIT;
    }

    item = cJSON_GetObjectItem(data, "MacFilterPolicy");
    if(item){
        macfilter.policy = item->valueint;
		apmib_set(MIB_RTL_LINK_V25_MACAC_POLICY,(void *)&macfilter.policy);
    }
	else{
		apmib_get(MIB_RTL_LINK_V25_MACAC_POLICY,(void *)&macfilter.policy);
	}
	
    item = cJSON_GetObjectItem(data, "MacFilterEntries");
    if(item){
		macfilter.contain_entries=1;
		strncpy(tmp_str, item->valuestring, sizeof(tmp_str));
        tmp_buf = strtok(tmp_str, delim);
        if(tmp_buf){
            node = malloc(sizeof(MacNode));
            if(!node){
                ERR_PRINT("ERROR ANDLINK_IF5 (%s)%d malloc failed!\n", __FUNCTION__, __LINE__);
                err = ERROR;
                goto EXIT;
            }
            list_insert(&macfilter.mac_list, node);
            memset(node, 0, sizeof(*node));
			snprintf(node->mac,sizeof(node->mac),tmp_buf);

			err = Andlink_if5_isLanDev(node);
			if(err != SUCCESS)
				goto EXIT;
			
            while((tmp_buf = strtok(NULL, delim)))
            {
                if(cnt % 2){
                    snprintf(node->name, sizeof(node->name), tmp_buf);
                }else{
                    node = malloc(sizeof(MacNode));
                    if(!node){
                        ERR_PRINT("ERROR ANDLINK_IF5 (%s)%d malloc failed!\n", __FUNCTION__, __LINE__);
                        err = ERROR;
                        goto EXIT;
                    }
                    list_insert(&macfilter.mac_list, node);
                    memset(node, 0, sizeof(*node));
					snprintf(node->mac,sizeof(node->mac),tmp_buf);

					err = Andlink_if5_isLanDev(node);
					if(err != SUCCESS)
						goto EXIT;

                }
                cnt++;
            }
            
        }
    }
	else{
        err = LACK_OF_ARGUMENT;
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss MacFilterEntries item\n", __FUNCTION__, __LINE__);
        goto EXIT;
    }

    ret = rtl_link_syncMacFilterSetting(macfilter);
    Andlink_set_restart_flag(ret);
    
EXIT:
    list_destroy(&macfilter.mac_list, free);
    return err;  
}


ERR_NUM_T Andlink_if5_postReboot(cJSON *data)
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

    return SUCCESS;    
}

ERR_NUM_T Andlink_if5_postLEDControl(cJSON *data)
{   
    cJSON *item;
    int ret = 0;
    LEDStatus led;

    item = cJSON_GetObjectItem(data, "LEDOnOff");
    if(item){
        led.on = item->valueint;    
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss LEDOnOff item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_syncLEDOnOff(led);
    Andlink_set_restart_flag(ret);
        
    return SUCCESS;     
}

ERR_NUM_T Andlink_if5_postRoamingConfig(cJSON *data)
{   
    cJSON *item; 
	cJSON *qos;
    int ret;
    RoamingConfig roaming;

    memset(&roaming, 0, sizeof(roaming));

    item = cJSON_GetObjectItem(data, "RoamingSwitch");
    if(item){
        roaming.enable = item->valueint;
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss RoamingSwitch item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    item = cJSON_GetObjectItem(data, "LowRSSI2.4G");
    if(item){
        roaming.rssi_2g = item->valueint;
    }

    item = cJSON_GetObjectItem(data, "LowRSSI5G");
    if(item){
        roaming.rssi_5g = item->valueint;
    }
//#if 0
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
//#endif
    
    ret = rtl_link_syncRoamingConfig(roaming);
    Andlink_set_restart_flag(ret);
        
    return SUCCESS; 
}

ERR_NUM_T Andlink_if5_postDeviceLimit(cJSON *data)
{
	int m=0, i=0, j=0, ret=0;
	ERR_NUM_T err = SUCCESS;
	DeviceLimit D_Limit;
	NodeLimit *N_limit=NULL;
	cJSON *devices, *device, *item;
	List lan_sta_list;
    STAInfo *lan_sta;
	char delete_delim_mac[13]={0};

	memset(&D_Limit, 0 ,sizeof(D_Limit));
	list_init(&D_Limit.limit_list);
	
	list_init(&lan_sta_list);
	ret = rtl_link_getLanDevInfo(&lan_sta_list);
    if(ret==-1){
        ERR_PRINT("ERROR (%s)%d error getLanDevInfo\n", __FUNCTION__, __LINE__);
		err = ERROR;
        goto EXIT;
    }

	devices = cJSON_GetObjectItem(data, "Devices");
	if(devices){
		m = cJSON_GetArraySize(devices);
		for(i=0;i<m;i++){
			device = cJSON_GetArrayItem(devices, i);

			N_limit = malloc(sizeof(NodeLimit));
        	if(!N_limit){
                ERR_PRINT("ERROR ANDLINK_IF5 (%s)%d malloc failed!\n", __FUNCTION__, __LINE__);
                err = ERROR;
                goto EXIT;
        	}
       		list_insert(&D_Limit.limit_list, N_limit);
        	memset(N_limit, 0, sizeof(*N_limit));
				
			item = cJSON_GetObjectItem(device, "DeviceMac");
			if(item){
				snprintf(N_limit->mac,sizeof(N_limit->mac),item->valuestring);
			}else{
                ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss DeviceMac item\n", __FUNCTION__, __LINE__);
                err = LACK_OF_ARGUMENT;
				goto EXIT;
			}

			N_limit->is_lan_sta = 0;
			for(j=0; j<lan_sta_list.len; j++){
				lan_sta = list_get_element(&lan_sta_list, j);
				Andlink_macFormat(delete_delim_mac, lan_sta->mac);
				if(!strcmp(delete_delim_mac, N_limit->mac)){
					DEBUG("(%s)%d sta %s is lan sta\n", __FUNCTION__, __LINE__,N_limit->mac);
					N_limit->is_lan_sta = 1;
				}
			}

			item = cJSON_GetObjectItem(device, "RxRate");
			if(item){
				snprintf(N_limit->limit_rxrate,sizeof(N_limit->limit_rxrate),item->valuestring);
			}else{
                ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss RxRate item\n", __FUNCTION__, __LINE__);
                err = LACK_OF_ARGUMENT;
				goto EXIT;
			}

			item = cJSON_GetObjectItem(device, "TxRate");
			if(item){
				snprintf(N_limit->limit_txrate,sizeof(N_limit->limit_txrate),item->valuestring);
			}else{
                ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss TxRate item\n", __FUNCTION__, __LINE__);
                err = LACK_OF_ARGUMENT;
				goto EXIT;
			}
#if 0
			DEBUG("N_limit->mac = %s\n", N_limit->mac);
			DEBUG("N_limit->limit_rxrate = %s\n", N_limit->limit_rxrate);
			DEBUG("N_limit->limit_txrate = %s\n", N_limit->limit_txrate);		
#endif
			
		}
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss Devices item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

	
	
	ret = rtl_link_syncDeviceLimit(D_Limit);
	if(ret == -1)
		goto EXIT;
	else
		Andlink_set_restart_flag(ret);

EXIT:
	list_destroy(&D_Limit.limit_list, free);
	list_destroy(&lan_sta_list, free);
	return err;  
}

ERR_NUM_T Andlink_if5_postTimerTask(cJSON *data)
{
	int ret=0;
	ERR_NUM_T err = SUCCESS;
	TimerTask T_Task;
	cJSON *item;

	memset(&T_Task, 0 ,sizeof(T_Task));
			
	item = cJSON_GetObjectItem(data, "Operation");
	if(item){
		if((!strcmp(item->valuestring, "Add")) && (!strcmp(item->valuestring, "Delete")) &&
			(!strcmp(item->valuestring, "Update"))){
			ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid Operation item\n", __FUNCTION__, __LINE__);
			err = INVALID_VALUE;
			goto EXIT;
		}
		snprintf(T_Task.operation,sizeof(T_Task.operation),item->valuestring);
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss Operation item\n", __FUNCTION__, __LINE__);
        err = LACK_OF_ARGUMENT;
		goto EXIT;
	}

	item = cJSON_GetObjectItem(data, "TaskId");
	if(item){
		if(atoi(item->valuestring)<0){
			ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid TaskId item\n", __FUNCTION__, __LINE__);
			err = INVALID_VALUE;
			goto EXIT;
		}
		snprintf(T_Task.taskid,sizeof(T_Task.taskid),item->valuestring);
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss TaskId item\n", __FUNCTION__, __LINE__);
        err = LACK_OF_ARGUMENT;
		goto EXIT;
	}

	item = cJSON_GetObjectItem(data, "TimeOffset");
	if(item){
		if((item->valueint < 0) || (item->valueint > SECONDS_OF_A_DAY)){
			ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid TimeOffset item\n", __FUNCTION__, __LINE__);
			err = INVALID_VALUE;
			goto EXIT;
		}
		T_Task.timeoffset = item->valueint;
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss TimeOffset item\n", __FUNCTION__, __LINE__);
        err = LACK_OF_ARGUMENT;
		goto EXIT;
	}

	item = cJSON_GetObjectItem(data, "Week");
	if(item){
		if((item->valueint < 0) || (item->valueint > 255)){
			ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid Week item\n", __FUNCTION__, __LINE__);
			err = INVALID_VALUE;
			goto EXIT;
		}
		T_Task.week = item->valueint;
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss Week item\n", __FUNCTION__, __LINE__);
        err = LACK_OF_ARGUMENT;
		goto EXIT;
	}

	item = cJSON_GetObjectItem(data, "Enable");
	if(item){
		if((item->valueint != 0) && (item->valueint != 1)){
			ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid Enable item\n", __FUNCTION__, __LINE__);
			err = INVALID_VALUE;
			goto EXIT;
		}
		T_Task.enable = item->valueint;
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss Enable item\n", __FUNCTION__, __LINE__);
        err = LACK_OF_ARGUMENT;
		goto EXIT;
	}

	item = cJSON_GetObjectItem(data, "Action");
	if(item){
		snprintf(T_Task.action,sizeof(T_Task.action),item->valuestring);
		if(strcmp(T_Task.action, "ToSetHealthMode") == 0){
			item = cJSON_GetObjectItem(data, "TimeOffset2");
			if((item->valueint < 0) || (item->valueint > SECONDS_OF_A_DAY)){
				ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid TimeOffset2 item\n", __FUNCTION__, __LINE__);
				err = INVALID_VALUE;
				goto EXIT;
			}
			T_Task.timeoffset2 = item->valueint;
			
			item = cJSON_GetObjectItem(data, "Index");
			if((item->valueint < 0) || (item->valueint > 8)){
				ERR_PRINT("ERROR ANDLINK_IF5(%s)%d invalid Index item\n", __FUNCTION__, __LINE__);
				err = INVALID_VALUE;
				goto EXIT;
			}
			T_Task.index = item->valueint;
		}
	}else{
        ERR_PRINT("ERROR ANDLINK_IF5(%s)%d miss Action item\n", __FUNCTION__, __LINE__);
        err = LACK_OF_ARGUMENT;
		goto EXIT;
	}
	
#if 0
		DEBUG("T_Task->operation = %s\n", T_Task.operation);
		DEBUG("T_Task->taskid = %s\n", T_Task.taskid);
		DEBUG("T_Task->timeoffset = %d\n", T_Task.timeoffset);		
		DEBUG("T_Task->week = %d\n", T_Task.week);		
		DEBUG("T_Task->enable = %d\n", T_Task.enable);		
		DEBUG("T_Task->action = %s\n", T_Task.action);
		DEBUG("T_Task->timeoffset2 = %d\n", T_Task.timeoffset2);
		DEBUG("T_Task->index = %d\n", T_Task.index);
#endif
	
	ret = rtl_link_syncTimerTask(T_Task);
	if(!ret)
		err = INVALID_VALUE;
	Andlink_set_restart_flag(ret);

EXIT:
	return err;
	
}

ERR_NUM_T Andlink_if5_postUpgrade(cJSON *data)
{   
    cJSON *fileUrl, *downMode, *fileMode, *fileType;
    int ret;
    FWUpgrade fw;

    fileMode = cJSON_GetObjectItem(data, "fileMode");

    fileType = cJSON_GetObjectItem(data, "fileType");

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
    }else{
        ERR_PRINT("ERROR ANDLINK(%s)%d miss Reboot item\n", __FUNCTION__, __LINE__);
        return LACK_OF_ARGUMENT;
    }

    ret = rtl_link_upgradeFirmware(fw);
	
   	return RECEIVED_SUCCESS; 
 
}

ERR_NUM_T Andlink_if5_postUnbind(cJSON *data)
{
    char empty[1] = "";
    char gwip[16] = "0.0.0.0";
    int tmpvalue = 0;
    struct in_addr ia_val;

    inet_aton(gwip, &ia_val);
    
    apmib_set(MIB_RTL_LINK_DEVID, (void *)empty);
	memset(basic_info.deviceId, 0, sizeof(basic_info.deviceId));
    apmib_set(MIB_RTL_LINK_DEVTOKEN, (void *)empty);
	memset(basic_info.deviceToken, 0, sizeof(basic_info.deviceToken));
    apmib_set(MIB_RTL_LINK_HBTIME, (void *)&tmpvalue);
	basic_info.HBTime = 0;
    apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)empty);
	strcpy(basic_info.synccode, "0");
//#ifdef CONFIG_ANDLINK_V3_SUPPORT
	apmib_set(MIB_RTL_LINK_USER_KEY, (void *)&empty);
	memset(basic_info.user_key, 0, sizeof(basic_info.user_key));
	apmib_set(MIB_RTL_LINK_CGWTOKEN, (void *)empty);
	memset(basic_info.gwToken_cloud, 0, sizeof(basic_info.gwToken_cloud));
	apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_cloud, 0, sizeof(basic_info.gwIp_cloud));
//#ifdef CONFIG_ANDLINK_IF5_SUPPORT
	/*****add by elaine for andlink if5******/
	apmib_set(MIB_RTL_LINK_CGW_PORT_CLOUD, (void *)&tmpvalue);
    basic_info.port_cloud = 0;
	apmib_set(MIB_RTL_LINK_CGW_IP_ADDR_IF5, (void *)&ia_val);
	memset(basic_info.gwIp_if5, 0, sizeof(basic_info.gwIp_if5));
	apmib_set(MIB_RTL_LINK_CGW_PORT_IF5, (void *)&tmpvalue);
    basic_info.port_if5 = 0;
	apmib_set(MIB_RTL_LINK_CGWTOKEN_IF5, (void *)empty);
	memset(basic_info.gwToken_if5, 0, sizeof(basic_info.gwToken_if5));
	apmib_set(MIB_RTL_LINK_CGW_MQTT_USER, (void *)empty);
	memset(basic_info.mqttUser, 0, sizeof(basic_info.mqttUser));
	apmib_set(MIB_RTL_LINK_CGW_MQTT_CLIENTID, (void *)empty);
	memset(basic_info.mqttClientId, 0, sizeof(basic_info.mqttClientId));
	//apmib_set(MIB_RTL_LINK_CGW_MQTT_URL, (void *)empty);
	//memset(basic_info.mqttUrl, 0, sizeof(basic_info.mqttUrl));
	apmib_set(MIB_RTL_LINK_CGW_MQTT_IP_ADDR, (void *)empty);
	memset(basic_info.mqtt_ip, 0, sizeof(basic_info.mqtt_ip));
	apmib_set(MIB_RTL_LINK_CGW_MQTT_PORT, (void *)&tmpvalue);
    basic_info.mqtt_port = 0;
	apmib_set(MIB_RTL_LINK_CGW_MQTT_PASSWORD, (void *)empty);
	memset(basic_info.mqttPassword, 0, sizeof(basic_info.mqttPassword));
	apmib_set(MIB_RTL_LINK_CGW_MQTT_KEEPALIVE, (void *)&tmpvalue);
    basic_info.mqttKeepalive = 0;
	apmib_set(MIB_RTL_LINK_CGW_AUTOBOOT_FLAG, (void *)&tmpvalue);
	basic_info.autoBootFlag = 0;
	/*******************************/
//#endif
	apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)empty);
	memset(basic_info.gwToken_host, 0, sizeof(basic_info.gwToken_host));
    apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_host, 0, sizeof(basic_info.gwIp_host));
//#else
#if 0//CONFIG_ANDLINK_IF6_SUPPORT   
	//if5解绑不能清空if6相关mib
    apmib_set(MIB_RTL_LINK_CGW_UDPPORT, (void *)&tmpvalue);
    basic_info.udp_port = 0;
    apmib_set(MIB_RTL_LINK_CGW_TCPPORT, (void *)&tmpvalue);
    basic_info.tcp_port = 0;
	apmib_set(MIB_RTL_LINK_CGW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_cloud_udp, 0, sizeof(basic_info.gwIp_cloud_udp));
	apmib_set(MIB_RTL_LINK_CGW_MS_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp_cloud_tcp, 0, sizeof(basic_info.gwIp_cloud_tcp));
#endif
    apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ia_val);
	memset(basic_info.gwIp, 0, sizeof(basic_info.gwIp));
    apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)empty);
	memset(basic_info.gwToken, 0, sizeof(basic_info.gwToken));
//#endif
    apmib_update(CURRENT_SETTING);
	
    return SUCCESS;
}

int Andlink_if5_formFwDownloadSuccessInform(cJSON *root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *data;
    int tmpvalue=0;
    
    time(&tm);

    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    cJSON_AddStringToObject(root, "eventType", "XData_File");
    cJSON_AddNumberToObject(root, "timestamp", tm);

    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(data, "respCode", FILE_TRANSFER_COMPLETE);
    cJSON_AddStringToObject(data, "restCont", "File Transfer Complete");
	
    return 0;
}

int Andlink_if5_formFwDownloadPercentInform(cJSON *root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *data;
    char tmpBuff[10]={0};
    
    time(&tm);

    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    cJSON_AddStringToObject(root, "eventType", "XData_File");
    cJSON_AddNumberToObject(root, "timestamp", tm);

    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(data, "respCode", FILE_TRANSFERRING);
	snprintf(tmpBuff, sizeof(tmpBuff), "%d", info->fwdownload_percent);
    cJSON_AddStringToObject(data, "restCont", tmpBuff);
	
    return 0;
}

int Andlink_if5_formUpGradeInform(cJSON *root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *data;
    int tmpvalue=0;
    
    time(&tm);

    cJSON_AddStringToObject(root, "deviceId", info->deviceId);
    cJSON_AddStringToObject(root, "eventType", "XData_File");
	cJSON_AddNumberToObject(root, "timestamp", tm);
	
    data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(data, "respCode", FILE_UPGRADE_SUCCESS);
    cJSON_AddStringToObject(data, "restCont", "File Upgrade Success");

    apmib_set(MIB_RTL_LINK_FW_UPGRADE, (void *)&tmpvalue);
    apmib_update(CURRENT_SETTING);
    return 0;
}

int Andlink_if5_formBootstrap(cJSON * root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *XData;

    time(&tm);
   
    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
        cJSON_AddStringToObject(root, "deviceMac", info->lan_mac);
    else
        cJSON_AddStringToObject(root, "deviceMac", info->wlan_mac_2g);	
    cJSON_AddStringToObject(root, "productToken", info->productToken);
    cJSON_AddStringToObject(root, "deviceType", info->deviceType);
    cJSON_AddNumberToObject(root, "timestamp", tm);
		
	return 0;
}

int Andlink_if5_formBoot(cJSON * root, Andlink_basicInfo *info)
{
    time_t tm;
    cJSON *XData;
    LoginInfo login;
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    char delim_mac[18]={0};
//#endif

    time(&tm);

    memset(&login, 0, sizeof(login));
    if(rtl_link_getLoginInfo(&login)){
        ERR_PRINT("ERROR ANDLINK(%s)%d error\n", __FUNCTION__, __LINE__);
        return -1;
    }


    cJSON_AddStringToObject(root, "deviceId", info->deviceId);

    cJSON_AddStringToObject(root, "deviceType", info->deviceType);
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT    
    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
		cJSON_AddStringToObject(root, "deviceMac", info->lan_mac);
    else
		cJSON_AddStringToObject(root, "deviceMac", info->wlan_mac_2g);
//#endif
    cJSON_AddStringToObject(root, "firmwareVersion", info->hwversion);
    cJSON_AddStringToObject(root, "softwareVersion", info->swversion);
    cJSON_AddStringToObject(root, "ipAddress", login.localIp);
    cJSON_AddNumberToObject(root, "timestamp", tm);

    XData = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "XData", XData);
    cJSON_AddStringToObject(XData, "deviceVendor", info->vendor);
    cJSON_AddStringToObject(XData, "deviceModel", info->model);
//#ifndef CONFIG_ANDLINK_IF6_SUPPORT 
//    if(info->rtl_link_mode == RTL_LINK_BRIDGE || info->rtl_link_mode == RTL_LINK_GW)
//        cJSON_AddStringToObject(XData, "deviceMac", info->lan_mac);
//    else
//        cJSON_AddStringToObject(XData, "deviceMac", info->wlan_mac_2g);
//#endif  
    cJSON_AddStringToObject(XData, "deviceSn", info->sn);
    if(login.uptype == ETHERNET)
        cJSON_AddStringToObject(XData, "apUplinkType", "Ethernet");
    else if(login.uptype == WIRELESS)
        cJSON_AddStringToObject(XData, "apUplinkType", "Wireless");
    if(login.band == DUALBAND || login.band == BAND5G)
        cJSON_AddNumberToObject(XData, "radio5", 1);
    else
        cJSON_AddNumberToObject(XData, "radio5", 0);
//#ifdef CONFIG_ANDLINK_IF6_SUPPORT         
    //cJSON_AddStringToObject(XData, "user key", info->user_key);
//#endif
#if 0
    cJSON_AddStringToObject(XData, "SyncCode", info->synccode);
#endif
    return 0;
}

int Andlink_if5_formOffline_message(cJSON * root, Andlink_basicInfo *info)
{
	int ret=0;
	time_t tm;

	time(&tm);

	cJSON_AddStringToObject(root, "eventType", "Offline");
	cJSON_AddStringToObject(root, "deviceId", info->deviceId);
	cJSON_AddNumberToObject(root, "timestamp", tm);
	
	return ret;
}

static int Andlink_if5_getBasicInfo(Andlink_basicInfo *info)
{
    struct in_addr ip;
    int enable1, enable2, band, value;
    char andlink_version[33], wlanif[16];
    int i;
    
    apmib_get(MIB_RTL_LINK_VENDOR, (void *)info->vendor);
    apmib_get(MIB_RTL_LINK_DEVMODEL, (void *)info->model);
    apmib_get(MIB_RTL_LINK_HWVERSION, (void *)info->hwversion);
    apmib_get(MIB_RTL_LINK_SWVERSION, (void *)info->swversion);   
    apmib_get(MIB_RTL_LINK_SN, (void *)info->sn);
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
	//apmib_get(MIB_RTL_LINK_CGW_MQTT_URL, (void *)info->mqttUrl);
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
//#endif
    apmib_get(MIB_RTL_LINK_ANDLINKTOKEN, (void *)info->andlinkToken);
    apmib_get(MIB_RTL_LINK_ANDLINK_VER, (void *)andlink_version);
#ifdef CONFIG_ANDLINK_V3_SUPPORT
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
//	apmib_get(MIB_RTL_LINK_BOOTSTRAP_MAX_RETRY, (void *)&info->bootstrap_max_retry);
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
    info->poweron=1;

	info->skip_coapping = 0;
	info->skip_online = 0;
	info->skip_inform_success = 0;
	info->skip_check_devId = 1;
	info->skip_encrypt_reg = 0;
	
    info->basic_size = sizeof(Andlink_basicInfo);

	return 0;
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

int main(int argc, char **argv)
{
    int arg, pid_fd, mode=1, ret=0, silent_time, getip_timeout;
	unsigned char cmd[128];

	FILE* fp=NULL;
	char line_buffer[256];
	int tmp_value;

#ifdef DMALLOC
    dmalloc_debug_setup("debug=0x4000503,log=/var/ac_dmalloc.log");
#endif    
    for(;;){
        //arg = getopt(argc, argv, "m:d:h");
		arg = getopt(argc, argv, "d:h");
        if(arg < 0)
            break;
        switch(arg){
       // case 'm':
       //     mode = atoi(optarg);
       //     break;
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

    pid_fd = pidfile_acquire(RTL_LINK_IF5_PID_FILE);
    if(pid_fd < 0)
        return 0;
    pidfile_write_release(pid_fd);
    if (!apmib_init()) {
		ERR_PRINT("ERROR ANDLINK_IF5 (%s)%d Initialize AP MIB failed!\n", __FUNCTION__, __LINE__);
		unlink(RTL_LINK_IF5_PID_FILE);
		return 0;
	}

	signal(SIGCHLD, SIG_IGN);
	memset(&basic_info, 0, sizeof(basic_info));
	Andlink_if5_getBasicInfo(&basic_info);
	
	while(1){

			ret = Andlink_if5_start_SM(&basic_info);
			DEBUG("ANDLINK_IF5 (%s)%d after Andlink_start_if5_SM ret is %d\n", __FUNCTION__,__LINE__,ret);
			
			switch(ret){
				
				case OTHERS:
				case GETIP_TIMEOUT:
				case COAP_RSP_TIMEOUT:
	            case ANDLINK_IF5_ERROR:
				case ANDLINK_TOKEN_MISSMATCH:
				case ANDLINK_IF5_INFORMFWDOWNLOADSTATUS_FAILED:
				case ANDLINK_IF5_INFORMUPGRADE_FAILED:
					DEBUG("ANDLINK_IF5 (%s)%d, (%s)exit and restart ...\n", __FUNCTION__, __LINE__, If5Err2str(ret));
					//unlink(RTL_LINK_IF5_PID_FILE);
	                sleep(5);
					//Andlink_start_if5_SM(p_if5);
					break;
				
				case ANDLINK_IF5_MQTT_DISCONNECT:
				case ANDLINK_IF5_AUTOBOOT_DISCONNECT:
					break;
				default:
					DEBUG("ANDLINK_IF5 (%s)%d unkonwn error value %d\n", __FUNCTION__, __LINE__, If5Err2str(ret));
			}
			
			if(ret == ANDLINK_IF5_MQTT_DISCONNECT || ret == ANDLINK_IF5_AUTOBOOT_DISCONNECT){
				if(ret == ANDLINK_IF5_MQTT_DISCONNECT){
					//mqtt断连之后，WiFi组网设备解绑后进入SoftAP配网状态，有线组网设备解绑后应恢复到允许被发现状态
					rtl_link_set_file_value(ANDLINK_IF5_EXIT, 1);
				}else{
					rtl_link_set_file_value(ANDLINK_IF5_EXIT, 2);
				}
					
				unlink(RTL_LINK_IF5_PID_FILE);
        		break;
			}								
		}	

EXIT:
#ifdef DMALLOC
	dmalloc_shutdown();
#endif
	DEBUG("ANDLINK_IF5 (%s)%d EXIT ANDLINK IF5 DAEMON!\n", __FUNCTION__, __LINE__);
	return 0;
}



