#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <stdio.h>

#include "../rtk_api/rtk_api.h"
#include "cmcc_api.h"
#include "../../boa/apmib/apmib.h"

int debug_on = 0; // for debug 
extern int string_to_hex(char *string, unsigned char *key, int len);
extern int rtk_get_lan_device_info(unsigned int *num, RTK_LAN_DEVICE_INFO_Tp pdevinfo);

int cmcc_api_init(void)
{
	if(rtk_api_init() == RTK_SUCCESS)
		return RTK_SUCCESS;
	return RTK_FAILED;
} 

int cmcc_api_update(void)
{
    if(rtk_update() == RTK_SUCCESS)
        return RTK_SUCCESS;
    return RTK_FAILED;
}

void cmcc_validate_access_settings()
{
    system("sysconf firewall");
}

void cmcc_validate_qos_settings()
{
    system("sysconf setQos");
}

void cmcc_validate_wifi_settings()
{
    int i, wifi_restart;
    char cmd[64];
    
     for(i=0; i<NUM_WLAN_INTERFACE; i++){
        wifi_restart |= 0x1 << i;
    }
    
    sprintf(cmd, "sysconf rtl_link restart %d", wifi_restart);
    system(cmd);
}

void cmcc_validate_wan_settings()
{
#if defined(REINIT_WAN_SUPPORT)
	system("sysconf reinit_wan");
#else
	system("sysconf init gw all");
#endif
}

void cmcc_validate_settings()
{
    system("sysconf init gw all");
}

int cmcc_get_wan_status(int *status)
{
    int configured;
    
    if(!status){
		fprintf(stderr,"input error! %s:%d\n",__FILE__,__LINE__);
		return RTK_FAILED;
	}
	if(apmib_get(MIB_WAN_CONFIGURED,(void*)&configured)==0){
		fprintf(stderr,"Get MIB_WAN_CONFIGURED fail!\n");
		return RTK_FAILED;
	}
	*status=configured;
	return RTK_SUCCESS;
}

int cmcc_set_wan_status(int status)
{
    if(apmib_set(MIB_WAN_CONFIGURED,(void*)&status)==0){
		fprintf(stderr,"Set MIB_WAN_CONFIGURED fail!\n");
		return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

int cmcc_get_qos_info(QOS_REQ_Tp req)
{
	int count = 0, index = 0, enable=0;
	unsigned char str_mac[32] = {0};
	IPQOS_T entry;
	
	if(req == NULL)
		return RTK_FAILED;

    apmib_get( MIB_QOS_ENABLED, (void *)&enable);
	apmib_get( MIB_QOS_RULE_TBL_NUM, (void *)&count);
	if(enable && count){
	    for(index = 1; index <= count; index++){
    		memset(&entry, '\0', sizeof(IPQOS_T));
    		*((char *)&entry) = (char)index;
    		apmib_get(MIB_QOS_RULE_TBL, (void *)&entry);

    		snprintf(str_mac, sizeof(str_mac), "%02x:%02x:%02x:%02x:%02x:%02x",
    			entry.mac[0], entry.mac[1], entry.mac[2], entry.mac[3], entry.mac[4], entry.mac[5]);

    		if(!memcmp(req->clientMAC, entry.mac, 6)){
    			snprintf(req->qosSwitch, sizeof(req->qosSwitch), "%d", entry.enabled);
    			snprintf(req->maxUplinkRate, sizeof(req->maxUplinkRate), "%d", entry.bandwidth/8);
    			snprintf(req->maxDownlinkRate, sizeof(req->maxDownlinkRate), "%d", entry.bandwidth_downlink/8);
    			return RTK_SUCCESS;
    		}
    	}
    	snprintf(req->qosSwitch, sizeof(req->qosSwitch), "0");
	}else{
	    snprintf(req->qosSwitch, sizeof(req->qosSwitch), "0");
	}

	return RTK_SUCCESS;
}

void cmcc_get_encrpt_string(unsigned int enc, unsigned char *str)
{
	if(enc==TYPE_NONE)
		snprintf(str, 50, "%s", "TYPE_NONE");
	else if(enc==TYPE_WEP)
		snprintf(str, 50, "%s", "TYPE_WEP");
	else if(enc==TYPE_WPA_PSK)
		snprintf(str, 50, "%s", "TYPE_WPA_PSK");
	else if(enc==TYPE_WPA_TKIP)
		snprintf(str, 50, "%s", "TYPE_WPA_TKIP");
	else if(enc==TYPE_WPA2_PSK)
		snprintf(str, 50, "%s", "TYPE_WPA2_PSK");
	else if(enc==TYPE_WPA2_AES)
		snprintf(str, 50, "%s", "TYPE_WPA2_AES");
	else if(enc==TYPE_WPA_PSK_WPA2_PSK)
		snprintf(str, 50, "%s", "TYPE_WPA_PSK_WPA2_PSK");
	else if(enc==TYPE_WPA_TKIP_WPA2_PSK)
		snprintf(str, 50, "%s", "TYPE_WPA_TKIP_WPA2_PSK");
	else if(enc==TYPE_WPA_PSK_WPA2_AES)
		snprintf(str, 50, "%s", "TYPE_WPA_PSK_WPA2_AES");
	else if(enc==TYPE_WPA_TKIP_WPA2_AES)
		snprintf(str, 50, "%s", "TYPE_WPA_TKIP_WPA2_AES");
	else if(enc==TYPE_WAPI)
		snprintf(str, 50, "%s", "TYPE_WAPI");	
	else
		snprintf(str, 50, "%s", "---");
	return;
}

int cmcc_get_encrypt_type(unsigned char *ifname, unsigned int enc, unsigned int *out)
{
	int ret = RTK_SUCCESS;
	unsigned int cipher_suite, cipher2_suite, auth_type;

	/* ENC_TYPE_GET: get customer encrypt by rtk value */	
	if(enc==ENCRYPT_DISABLED)
		*out = TYPE_NONE;
	else if(enc==ENCRYPT_WEP)
		*out = TYPE_WEP;
	else if(enc==ENCRYPT_WPA || enc==ENCRYPT_WPA2 || enc==ENCRYPT_WPA2_MIXED){   
		rtk_wlan_get_cipher_suite(ifname, &auth_type, &cipher_suite, &cipher2_suite);
		if(1){
			if(enc==ENCRYPT_WPA){ // WPA
				if(cipher_suite==WPA_CIPHER_TKIP)
					*out = TYPE_WPA_TKIP;
				else if(cipher_suite==(WPA_CIPHER_TKIP | WPA_CIPHER_AES))
					*out = TYPE_WPA_PSK;
			}else if(enc==ENCRYPT_WPA2){ // WPA2
				if(cipher2_suite==WPA_CIPHER_AES)
					*out = TYPE_WPA2_AES;
				else if(cipher2_suite==(WPA_CIPHER_TKIP | WPA_CIPHER_AES))
					*out = TYPE_WPA2_PSK;
			}else {	// WPA-WPA2
				if((cipher_suite==WPA_CIPHER_TKIP) && (cipher2_suite==WPA_CIPHER_AES))
					*out = TYPE_WPA_TKIP_WPA2_AES;
				else if((cipher_suite==WPA_CIPHER_TKIP) && (cipher2_suite==(WPA_CIPHER_TKIP | WPA_CIPHER_AES)))
					*out = TYPE_WPA_TKIP_WPA2_PSK;
				else if((cipher_suite==(WPA_CIPHER_TKIP | WPA_CIPHER_AES)) && (cipher2_suite==(WPA_CIPHER_AES)))
					*out = TYPE_WPA_PSK_WPA2_AES;
				else if((cipher_suite==(WPA_CIPHER_TKIP | WPA_CIPHER_AES)) && (cipher2_suite==(WPA_CIPHER_TKIP | WPA_CIPHER_AES)))
					*out = TYPE_WPA_PSK_WPA2_PSK;
			}				
		}			
	}		
	else if(enc==ENCRYPT_WAPI)
		*out = TYPE_WAPI;	
		
	return RTK_SUCCESS;
}

int cmcc_set_encrypt_type(unsigned char *ifname, unsigned int enc, unsigned char *code)
{
    unsigned int encrypt = 0, wpa_cipher_suite = 0, wpa2_cipher_suite = 0, wpa_auth_type=0, wep_auth_type=0;
    unsigned int wlan_wep = 0, keyfmt = 0;
    int key_len = 0;	
	
    switch(enc){
        case TYPE_NONE:
            encrypt = ENCRYPT_DISABLED;            
            break;
        case TYPE_WEP:
            encrypt = ENCRYPT_WEP;
            break;
        case TYPE_WPA_TKIP:
            encrypt = ENCRYPT_WPA;
            wpa_cipher_suite = WPA_CIPHER_TKIP;
            break;            
        case TYPE_WPA_PSK:
            encrypt = ENCRYPT_WPA;
            wpa_cipher_suite = (WPA_CIPHER_TKIP | WPA_CIPHER_AES);
            break;
        case TYPE_WPA2_AES:
            encrypt = ENCRYPT_WPA2;
            wpa2_cipher_suite = WPA_CIPHER_AES;
            break; 
        case TYPE_WPA2_PSK:
            encrypt = ENCRYPT_WPA2;
            wpa2_cipher_suite = (WPA_CIPHER_TKIP | WPA_CIPHER_AES);
            break;
        case TYPE_WPA_TKIP_WPA2_AES:
            encrypt = ENCRYPT_WPA2_MIXED;
            wpa_cipher_suite = WPA_CIPHER_TKIP;
            wpa2_cipher_suite = WPA_CIPHER_AES;
            break; 
        case TYPE_WPA_TKIP_WPA2_PSK:
            encrypt = ENCRYPT_WPA2_MIXED;
            wpa_cipher_suite = WPA_CIPHER_TKIP;
            wpa2_cipher_suite = (WPA_CIPHER_TKIP | WPA_CIPHER_AES);
            break; 
        case TYPE_WPA_PSK_WPA2_AES:
            encrypt = ENCRYPT_WPA2_MIXED;
            wpa_cipher_suite = (WPA_CIPHER_TKIP | WPA_CIPHER_AES);
            wpa2_cipher_suite = WPA_CIPHER_AES;
            break; 
        case TYPE_WPA_PSK_WPA2_PSK:
            encrypt = ENCRYPT_WPA2_MIXED;
            wpa_cipher_suite = (WPA_CIPHER_TKIP | WPA_CIPHER_AES);
            wpa2_cipher_suite = (WPA_CIPHER_TKIP | WPA_CIPHER_AES);                       
            break;
        case TYPE_WAPI:
            encrypt = ENCRYPT_WAPI;
            break;
        default:
            printf("[%s %d]Invalid encrpty [%d] when set encrpty.\n", __FUNCTION__,__LINE__,enc);
		    return RTK_FAILED;                
    }
	
    rtk_wlan_set_encryption(ifname, encrypt); 
    
    rtk_wlan_get_wep_auth(ifname, &wep_auth_type);
    rtk_wlan_get_wpa_auth(ifname, &wpa_auth_type);
    rtk_wlan_get_key_type_by_encrypt(ifname, encrypt, code, &wlan_wep, &keyfmt);

	if(strstr(ifname, "-va")){
		if((encrypt == ENCRYPT_WPA) || (encrypt == ENCRYPT_WPA2) || (encrypt == ENCRYPT_WPA2_MIXED))
			wpa_auth_type = WPA_AUTH_PSK;
		if(encrypt == ENCRYPT_WEP)
			wep_auth_type = AUTH_BOTH;
	}
	
	CMCC_DEBUG("ifname:%s, cust encType:%d, rtk encrypt:%d, wep_auth:%d, wlan_wep:%d, wpa_auth:%d, keyfmt:%d\n",		
						ifname, enc, encrypt, wep_auth_type, wlan_wep, wpa_auth_type, keyfmt);

	if(encrypt == ENCRYPT_DISABLED){
		rtk_wlan_set_wep_auth(ifname, wep_auth_type);
		rtk_wlan_set_wpa_auth(ifname, wpa_auth_type);
	}
    else if(encrypt == ENCRYPT_WEP){
		CMCC_DEBUG("==> rtk_wlan_set_security_wep\n");
        rtk_wlan_set_security_wep(ifname, wep_auth_type, wlan_wep, keyfmt, 0, code);
    }else if(encrypt == ENCRYPT_WPA){ 
    	CMCC_DEBUG("wpa_cipher_suite:%d\n", wpa_cipher_suite);
        rtk_wlan_set_security_wpa(ifname, wpa_auth_type, wpa_cipher_suite, keyfmt, code);
    }else if(encrypt == ENCRYPT_WPA2){
    	CMCC_DEBUG("wpa2_cipher_suite:%d\n", wpa2_cipher_suite);
        rtk_wlan_set_security_wpa2(ifname, wpa_auth_type, wpa2_cipher_suite, keyfmt, code);
    }else if(encrypt == ENCRYPT_WPA2_MIXED){
    	CMCC_DEBUG("wpa_cipher_suite:%d, wpa2_cipher_suite:%d\n", wpa_cipher_suite, wpa2_cipher_suite);
        rtk_wlan_set_security_wpamix(ifname, wpa_auth_type, wpa_cipher_suite, wpa2_cipher_suite, keyfmt, code);
    }else if(encrypt == ENCRYPT_WAPI){

    }      

    return RTK_SUCCESS;
}

void cmcc_get_terminal_access_info(unsigned char *mac, char *access)
{
    int i, entryNum=0, enable=0;
    MACFILTER_T entry;
    
    apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
    apmib_get(MIB_MACFILTER_ENABLED, (void *)&enable);
    if(enable == 1 && entryNum>0){
        for(i = 1; i <= entryNum; i++){
            memset(&entry, '\0', sizeof(entry));
            *((char *)&entry) = (char)i;
            apmib_get(MIB_MACFILTER_TBL, (void *)&entry);
            if(! memcmp(entry.macAddr, mac, 6)){
                strcpy(access, "1");
                return;
            }
        }
        strcpy(access, "0");
    }else{
        strcpy(access, "0");
    }
    return;
}

int cmcc_get_INITINFO_REQ(struct initinfo_req *req)
{
    struct rtk_dut_info info;
    struct rtk_static_config config;
    struct in_addr ip_addr;
    
    if(!rtk_get_dut_info(&info))
        return RTK_FAILED;
    snprintf(req->devID, sizeof(req->devID), info.sn);
    if(info.opmode == GATEWAY_MODE)
        snprintf(req->deviceType, sizeof(req->deviceType), "1");
    else if(info.opmode == BRIDGE_MODE){
        if(info.repeater_mode)
            snprintf(req->deviceType, sizeof(req->deviceType), "2");
        else
            snprintf(req->deviceType, sizeof(req->deviceType), "3");
    }
    
    if(!rtk_get_lan_ip(&config))
        return RTK_FAILED;
    ip_addr.s_addr = config.ip;
    snprintf(req->IP, sizeof(req->IP), inet_ntoa(ip_addr));
    return RTK_SUCCESS;
}


int cmcc_set_INITINFO_RSP(struct initinfo_rsp rsp)
{
    struct rtk_time time;

    time.tm = (time_t)atol(rsp.time);
    snprintf(time.tm_zone, sizeof(time.tm_zone), rsp.timeZone);
    strcat(time.tm_zone, " 1");
    if(!rtk_set_time(time))
        return RTK_FAILED;

	if (apmib_set(MIB_DEVICE_PASSWORD, (void *)(rsp.password)) == 0) {
		printf("[%s:%d] set mib MIB_DEVICE_PASSWORD error!\n", __FUNCTION__, __LINE__);
		return RTK_FAILED;
	}
    return RTK_SUCCESS;
}

int cmcc_get_ABLITITY_NOTIFY_REQ_init(char* devId,char* fac,char* softVersion)
{
#ifdef CUSTOMER_HW_SETTING_SUPPORT
	if(apmib_set(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)devId) == 0){
		printf("set MIB_RTL_LINK_SN fail\n");
		return RTK_FAILED;
	}
#else
	if(apmib_set(MIB_RTL_LINK_SN, (void *)devId) == 0){
		printf("set MIB_RTL_LINK_SN fail\n");
		return RTK_FAILED;
	}
#endif

	if(apmib_set(MIB_RTL_LINK_VENDOR,(void*)fac)==0){
		printf("set MIB_RTL_LINK_SN fail\n");
		return RTK_FAILED;
	}

	if(apmib_set(MIB_RTL_LINK_SWVERSION,(void*)softVersion)==0){
		printf("set MIB_RTL_LINK_SN fail\n");
		return RTK_FAILED;
	}

	return RTK_SUCCESS;
}


int cmcc_get_ABLITITY_NOTIFY_REQ(struct ability_notify_req *req)
{
    struct rtk_dut_info info;
    
    if(!rtk_get_dut_info(&info))
        return RTK_FAILED;
    
    snprintf(req->devID, sizeof(req->devID), info.sn);
    if(info.enable_2g == WIFI_STATUS_ENABLE){
        rtk_wlan_get_ssid(WLAN_2G_IFNAME, req->devName);
    }else if(info.enable_5g == WIFI_STATUS_ENABLE){
        rtk_wlan_get_ssid(WLAN_5G_IFNAME, req->devName);
    }else{
        rtk_wlan_get_ssid(WLAN_2G_IFNAME, req->devName);
    }
    //snprintf(req->devName, sizeof(req->devName), info.model);
    snprintf(req->fac, sizeof(req->fac), info.vendor);
    snprintf(req->type, 2, &info.sn[2]);
    snprintf(req->softVersion, sizeof(req->softVersion), info.swversion);
    if(info.band == BAND2G){
        strcpy(req->capacitySet.enable2GAccess, "1");
        strcpy(req->capacitySet.enable5GAccess, "0");
    }else if(info.band == BAND5G){
        strcpy(req->capacitySet.enable2GAccess, "0");
        strcpy(req->capacitySet.enable5GAccess, "1");
    }else{
        strcpy(req->capacitySet.enable2GAccess, "1");
        strcpy(req->capacitySet.enable5GAccess, "1");
    }
    return RTK_SUCCESS;
}

int cmcc_get_ROUTE_OVERVIEW_RSP(struct route_overview_rsp *rsp)
{
    int repeater_connect_2g = 0, repeater_connect_5g = 0;
    int ip_filter, mac_filter, nas_filter, port_filter, port_forward, url_filter, qos_enable;
    int num_connect, channel, i, m=0, n=0;
    unsigned char mac[6];
    unsigned char wlan_if[16], wlan_repeater_if[16];
    unsigned char repeater_ssid[MAX_SSID_LEN], ssid_2g[MAX_SSID_LEN], ssid_5g[MAX_SSID_LEN];
    struct rtk_dut_info info;
    struct sysinfo sys_info;
    struct rtk_static_config config;
    struct rtk_link_type_info sta_info[MAX_STA_NUM*2+4]; //2.4G/5G/Ethernet
    struct cmcc_sta_info *psta;
    struct in_addr ip_addr;
    RTK_WLAN_STA_INFO_T repeater_info[MAX_STA_NUM+1];
    QOS_REQ_T qos_info;
    RTK_DOWNLOAD_STATICS download;
    RTK_UPLOAD_STATICS upload;
    
    if(rsp == NULL){
        printf("[%s %d]rsp is NULL !!! \n", __FUNCTION__,__LINE__);
        return RTK_FAILED;
    }
    memset(rsp, 0, sizeof(struct route_overview_rsp));

    if(!rtk_get_dut_info(&info))
        return RTK_FAILED;
    //type
    snprintf(rsp->type, 2, &info.sn[2]);

    //mac
    rtk_get_default_mac_address(mac, "eth0");
    sprintf(rsp->MAC, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    //ip
    if(!rtk_get_lan_ip(&config))
        return RTK_FAILED;
    ip_addr.s_addr = config.ip; 
    snprintf(rsp->IP, sizeof(rsp->IP), "%s", inet_ntoa(ip_addr));

    //channel
    if(info.enable_2g == WIFI_STATUS_ENABLE)
        rtk_wlan_get_real_channel(WLAN_2G_IFNAME, &channel);
    else if(info.enable_5g == WIFI_STATUS_ENABLE)
        rtk_wlan_get_real_channel(WLAN_5G_IFNAME, &channel);
    else{
        printf("[%s, %d] wlan disabled, can not get channel\n", __FUNCTION__, __LINE__);
        channel = 0;
    }
    snprintf(rsp->channel, sizeof(rsp->channel), "%d", channel);

    //softwareVersion
    snprintf(rsp->softwareVersion, sizeof(rsp->softwareVersion), info.swversion);

    //onlineDuration
    sysinfo(&sys_info);
    snprintf(rsp->onlineDuration, sizeof(rsp->onlineDuration), "%ld", sys_info.uptime);

    //uplineSignal & uplinkSSID
    if(info.opmode==BRIDGE_MODE && info.repeater_mode){
        if(info.enable_2g && info.repeater_enable_2g){
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_2G_IFNAME);
            getWlStaNum(wlan_repeater_if, &repeater_connect_2g);
        }
        if(info.enable_5g && info.repeater_enable_5g){
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_5G_IFNAME);
            getWlStaNum(wlan_repeater_if, &repeater_connect_5g);
        }

        //prior to use 2G when 2g/5g both conncet
        if(repeater_connect_2g)
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_2G_IFNAME);
        else if(repeater_connect_5g)
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_5G_IFNAME);
        else
            printf("[%s %d]Repeater Mode but not connect yet !!! \n", __FUNCTION__,__LINE__);

        if(repeater_connect_2g || repeater_connect_5g){
            rtk_get_wlan_sta(wlan_repeater_if, repeater_info);
            if(repeater_info[1].aid){
                //upLineSignal
                if(repeater_info[1].rssi >= 60)
                    snprintf(rsp->upLineSignal, sizeof(rsp->upLineSignal), "1");
                else if(repeater_info[1].rssi < 60 && repeater_info[1].rssi >= 40)
                    snprintf(rsp->upLineSignal, sizeof(rsp->upLineSignal), "2");
                else
                    snprintf(rsp->upLineSignal, sizeof(rsp->upLineSignal), "3");
            }else{
                printf("[%s %d]Get repeater STA failed !!! \n", __FUNCTION__,__LINE__);
            }
            //uplinkSSID
            rtk_wlan_get_repeater_ssid(wlan_repeater_if, repeater_ssid);
            snprintf(rsp->upLinkSSID, sizeof(rsp->upLinkSSID), "%s", repeater_ssid);
        }
    }

    //firewall
    rtk_get_ip_filter_enable(&ip_filter);
    rtk_get_mac_filter_enable(&mac_filter);
    rtk_get_qos_enable(&qos_enable);
    //rtk_get_nas_filter_enable(&nas_filter);
    //rtk_get_port_filter_enable(&port_filter);
    //rtk_get_port_forward_enable(&port_forward);
    //rtk_get_url_filter_enable(&url_filter);
    //if(ip_filter || mac_filter || nas_filter || port_filter || port_forward || url_filter || qos_enable)
    if(ip_filter || mac_filter || qos_enable)
        snprintf(rsp->firewallStatus, sizeof(rsp->firewallStatus), "1");
    else
        snprintf(rsp->firewallStatus, sizeof(rsp->firewallStatus), "2");

    //wifistatus
    if(info.enable_2g == WIFI_STATUS_ENABLE)
        snprintf(rsp->WifiStatus, sizeof(rsp->WifiStatus), "1");
    else if(info.enable_2g == WIFI_STATUS_DISABLE)
        snprintf(rsp->WifiStatus, sizeof(rsp->WifiStatus), "2");
    else if(info.enable_2g == WIFI_STATUS_NOT_SUPPORT)
        snprintf(rsp->WifiStatus, sizeof(rsp->WifiStatus), "0");
    //5Gwifistatus
    if(info.enable_5g == WIFI_STATUS_ENABLE)
        snprintf(rsp->WifiStatus5G, sizeof(rsp->WifiStatus5G), "1");
    else if(info.enable_5g == WIFI_STATUS_DISABLE)
        snprintf(rsp->WifiStatus5G, sizeof(rsp->WifiStatus5G), "2");
    else if(info.enable_5g == WIFI_STATUS_NOT_SUPPORT)
        snprintf(rsp->WifiStatus5G, sizeof(rsp->WifiStatus5G), "0");

    //downlinkRate & uplinkRate
    if(info.opmode == GATEWAY_MODE){
        rtk_get_download_statics(&download);
        rtk_get_upload_statics(&upload);
    }else if(info.opmode == WISP_MODE || (info.opmode==BRIDGE_MODE && info.repeater_mode)){
        if(!rtk_get_terminal_rate_by_mac(repeater_info[1].addr, &download.cur_download_speed, &upload.cur_upload_speed)){
            printf("[%s %d]Get repeater rate failed !!! \n", __FUNCTION__,__LINE__);
        }
    }else if(info.opmode == BRIDGE_MODE){
        // FIXME: get repeater's real uplink port ,currently use port4 by default
#ifdef CONFIG_RTL_8367R_SUPPORT
        if(!rtk_get_terminal_rate_by_port(0, &download.cur_download_speed, &upload.cur_upload_speed)){
            printf("[%s %d]Get repeater rate failed !!! \n", __FUNCTION__,__LINE__);
        }
#else
        if(!rtk_get_terminal_rate_by_port(4, &download.cur_download_speed, &upload.cur_upload_speed)){
            printf("[%s %d]Get repeater rate failed !!! \n", __FUNCTION__,__LINE__);
        }
#endif
    }
    snprintf(rsp->downlinkRate, sizeof(rsp->downlinkRate), "%ld", download.cur_download_speed);
    snprintf(rsp->uplinkRate, sizeof(rsp->uplinkRate), "%ld", upload.cur_upload_speed);

    //connectDevices & connect5GDevices
    memset(sta_info, 0, sizeof(struct rtk_link_type_info)*(MAX_STA_NUM*2+4));
    rtk_get_terminal_info(&num_connect, sta_info, MAX_STA_NUM*2+4);
    for(i=0; i<num_connect; i++){
        if(sta_info[i].type == RTK_24G)
            psta = &rsp->connectDevices[n++];
        else if(sta_info[i].type == RTK_5G)
            psta = &rsp->connect5GDevices[m++];
        else
            continue;
    
        //clientUplinkRate
        snprintf(psta->clientUplinkRate, sizeof(psta->clientUplinkRate), "%d", sta_info[i].upload_speed);
        //clientDownlinkRate
        snprintf(psta->clientDownlinkRate, sizeof(psta->clientDownlinkRate), "%d", sta_info[i].download_speed);
        // TODO: deviceStatus
        //MAC
        snprintf(psta->MAC, sizeof(psta->MAC), "%02X:%02X:%02X:%02X:%02X:%02X", 
            sta_info[i].mac[0],sta_info[i].mac[1],sta_info[i].mac[2],sta_info[i].mac[3],sta_info[i].mac[4],sta_info[i].mac[5]);
        //IP & name
#ifdef CONFIG_RECORD_CLIENT_HOST
        snprintf(psta->IP, sizeof(psta->IP), "%s", sta_info[i].client_host_ip);
        snprintf(psta->name, sizeof(psta->name), "%s", sta_info[i].client_host_name);
#else
        snprintf(psta->IP, sizeof(psta->IP), "%s", inet_ntoa(sta_info[i].ip));
        snprintf(psta->name, sizeof(psta->name), "%s", sta_info[i].brand);
#endif
        //onlineDuration
        snprintf(psta->onlineDuration, sizeof(psta->onlineDuration), "%d", sta_info[i].link_time);
        //accessInternet
        cmcc_get_terminal_access_info(sta_info[i].mac, psta->accessInternet);
        //qosSwitch &  maxUplinkRate & maxDownlinkRate
        memset(&qos_info, 0, sizeof(qos_info));
        memcpy(qos_info.clientMAC, sta_info[i].mac, 6);
        cmcc_get_qos_info(&qos_info);
        snprintf(psta->qosSwitch, sizeof(psta->qosSwitch), qos_info.qosSwitch);
        snprintf(psta->maxDownlinkRate, sizeof(psta->maxDownlinkRate), qos_info.maxDownlinkRate);
        snprintf(psta->maxUplinkRate, sizeof(psta->maxUplinkRate), qos_info.maxUplinkRate);
        //SSID
        snprintf(psta->SSID, sizeof(psta->SSID), "%s", sta_info[i].ssid);
    }
    rsp->connect_num = n;
    rsp->connect5G_num = m;
    
    return RTK_SUCCESS;
}


int cmcc_get_ROUTE_BASE_INFO_RSP(struct route_base_info_rsp *rsp)
{
    struct rtk_dut_info info;
    RTK_WAN_INFO wan_info;
    RTK_LAN_INFO lan_info;
    int repeater_connect_2g, repeater_connect_5g;
    unsigned char wlan_repeater_if[16];
    unsigned char mac[6];
    struct sockaddr hwaddr;

    if(!rtk_get_dut_info(&info))
        return RTK_FAILED;
    snprintf(rsp->deviceID, sizeof(rsp->deviceID), info.sn);
    
    if(!rtk_get_lan_info(&lan_info))
        return RTK_FAILED;
    snprintf(rsp->lanIP, sizeof(rsp->lanIP), inet_ntoa(lan_info.ip));

    if(info.opmode == GATEWAY_MODE){
        if(!rtk_get_wan_info(&wan_info))
            return RTK_FAILED;
    }
    
    if(info.opmode == GATEWAY_MODE || info.opmode == WISP_MODE){
        snprintf(rsp->wanIP, sizeof(rsp->wanIP), inet_ntoa(wan_info.ip));
        snprintf(rsp->wanMAC, sizeof(rsp->wanMAC), "%02X:%02X:%02X:%02X:%02X:%02X", 
            wan_info.mac[0],wan_info.mac[1],wan_info.mac[2],wan_info.mac[3],wan_info.mac[4],wan_info.mac[5]);
    }else if(info.opmode==BRIDGE_MODE && info.repeater_mode){
        snprintf(rsp->wanIP, sizeof(rsp->wanIP), inet_ntoa(lan_info.ip));
        
        if(info.enable_2g && info.repeater_enable_2g){
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_2G_IFNAME);
            getWlStaNum(wlan_repeater_if, &repeater_connect_2g);
        }
        if(info.enable_5g && info.repeater_enable_5g){
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_5G_IFNAME);
            getWlStaNum(wlan_repeater_if, &repeater_connect_5g);
        }

        if(repeater_connect_2g)
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_2G_IFNAME);
        else if(repeater_connect_5g)
            snprintf(wlan_repeater_if, sizeof(wlan_repeater_if), "%s-vxd", WLAN_5G_IFNAME);
        else
            printf("[%s %d]Repeater Mode but not connect yet !!! \n", __FUNCTION__,__LINE__);

        rtk_getInAddr(wlan_repeater_if, HW_ADDR_T, (void *)&hwaddr);
        memcpy(mac, (unsigned char *)hwaddr.sa_data, 6);
        
        snprintf(rsp->wanMAC, sizeof(rsp->wanMAC), "%02X:%02X:%02X:%02X:%02X:%02X", 
            mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        
    }else if(info.opmode == BRIDGE_MODE){
        snprintf(rsp->wanIP, sizeof(rsp->wanIP), inet_ntoa(lan_info.ip));
        snprintf(rsp->wanMAC, sizeof(rsp->wanMAC), "%02X:%02X:%02X:%02X:%02X:%02X", 
            lan_info.mac[0],lan_info.mac[1],lan_info.mac[2],lan_info.mac[3],lan_info.mac[4],lan_info.mac[5]);
    }

    return RTK_SUCCESS;
}

int cmcc_set_DEVICE_OPERATE_REQ(struct device_operate_req req)
{
    char cmd[64], wlan_if[16];
    int i, wifi_restart=0, enable;

    for(i=0; i<NUM_WLAN_INTERFACE; i++){
        wifi_restart |= 0x1 << i;
    }
    
    if(!strcmp(req.opType, "1")){
        sprintf(cmd, "sysconf rtl_link restart %d", wifi_restart);
        system(cmd);
    }if(!strcmp(req.opType, "2")){
        cmcc_api_update();
        system("reboot");
    }if(!strcmp(req.opType, "3")){
        system("flash default-sw");
        system("reboot");
    }if(!strcmp(req.opType, "4")){
        if(!strcmp(req.opPara, "1")){
            for(i = 0; i<NUM_WLAN_INTERFACE; i++){
                sprintf(wlan_if, "wlan%d", i);
                rtk_wlan_enable(wlan_if);
            }
        }else{
            for(i = 0; i<NUM_WLAN_INTERFACE; i++){
                sprintf(wlan_if, "wlan%d", i);
                rtk_wlan_disable(wlan_if);
            }
        }
        sprintf(cmd, "sysconf rtl_link restart %d", wifi_restart);
        system(cmd);
    }if(!strcmp(req.opType, "5")){
        cmcc_WIFI_ACCELERATE(1); 
    }if(!strcmp(req.opType, "6")){
        if(!strcmp(req.opPara, "1"))
            enable = 1;
        else
            enable = 0;
        rtk_set_ip_filter_enable(enable);
        rtk_set_mac_filter_enable(enable);
        rtk_set_qos_enable(enable);
        //rtk_set_nas_filter_enable(enable);
        //rtk_set_port_filter_enable(enable);
        //rtk_set_port_forward_enable(enable);
        //rtk_set_url_filter_enable(enable);
        system("sysconf firewall");
        system("sysconf setQos");
    }

    return RTK_SUCCESS;
}

//===================== access control start ============================

int cmcc_get_CLIENT_ACCESS_CONTROL_INFO_RSP(int *deviceNum, CLIENT_ACCESS_CONTROL_INFO_RSP_Tp rsp)
{
	int lanDeviceNum = 0, entryNum = 0, enable = 0;
	int i = 0, j = 0;
	char mac_list[MAX_FILTER_NUM][32] = {{0}}; 
	RTK_LAN_DEVICE_INFO_T devinfo[MAX_STA_NUM]={0};	
	MACFILTER_T entry;

	if(rsp == NULL)
		return RTK_FAILED;
		
	if(rtk_get_lan_device_info(&lanDeviceNum, devinfo) == RTK_SUCCESS){
		memset(rsp, '\0', sizeof(CLIENT_ACCESS_CONTROL_INFO_RSP_T));

		//printf("[%s:%d] lanDeviceNum: %d\n", __FUNCTION__, __LINE__, lanDeviceNum);
		apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
		apmib_get(MIB_MACFILTER_ENABLED, (void *)&enable);
		if(enable==1 && entryNum>0){
			for(i = 1; i <= entryNum; i++) {
				memset(&entry, '\0', sizeof(entry));
				*((char *)&entry) = (char)i;
				apmib_get(MIB_MACFILTER_TBL, (void *)&entry);

				snprintf(mac_list[i-1], 32, "%02x:%02x:%02x:%02x:%02x:%02x", 
					entry.macAddr[0], entry.macAddr[1], entry.macAddr[2], entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
	
				//printf("[%s:%d] macfilter%d-- %s\n", __FUNCTION__, __LINE__, i-1, mac_list[i-1]);
			}
		}

		*deviceNum = lanDeviceNum;
		for(i = 0; i < lanDeviceNum; i++){
			CLIENT_ACCESS_CONTROL_INFO_Tp str_tmp = &((rsp->caInfo)[i]);
			
			snprintf(str_tmp->clientMAC, sizeof(str_tmp->clientMAC), "%02x:%02x:%02x:%02x:%02x:%02x",
				devinfo[i].mac[0], devinfo[i].mac[1], devinfo[i].mac[2], 
				devinfo[i].mac[3], devinfo[i].mac[4], devinfo[i].mac[5]); 
			snprintf(str_tmp->clientIP, sizeof(str_tmp->clientIP), "%s", 
				inet_ntoa((*((struct in_addr *)&(devinfo[i].ip)))));
			
			snprintf(str_tmp->clientName, sizeof(str_tmp->clientName), "%s", devinfo[i].hostname);
			snprintf(str_tmp->accessRight, sizeof(str_tmp->accessRight), "%d", ALLOW);

			if(enable==1 && entryNum>0){
				for(j = 0; j < entryNum; j++){
					if(strncmp(str_tmp->clientMAC, mac_list[j], 32) == 0){
						snprintf(str_tmp->accessRight, sizeof(str_tmp->accessRight), "%d", DENY);
						break;
					}
				}
			}
			//printf("[%s:%d] info%d--mac:%s, ip: %s, name: %s, access: %s\n", __FUNCTION__, __LINE__, 
			//i, tmp.clientMAC, tmp.clientIP, tmp.clientName, tmp.accessRight);
		}
		
		return RTK_SUCCESS;
	}
	return RTK_SUCCESS;
}

int cmcc_set_CLIENT_ACCESS_CONTROL_REQ(CLIENT_ACCESS_CONTROL_REQ_Tp req)
{
	MACFILTER_T entry;
	char str_mac[32] = {0}, str_allow[8] = {0}, str_deny[8] = {0};
	int entryNum = 0, i = 0;
	
	if(req == NULL)
		return RTK_FAILED;
	
	snprintf(str_allow, sizeof(str_allow), "%d", ALLOW);
	snprintf(str_deny, sizeof(str_deny), "%d", DENY);

	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);
	for(i = 1; i <= entryNum; i++) {
		memset(&entry, '\0', sizeof(entry));
		*((char *)&entry) = (char)i;
	
		apmib_get(MIB_MACFILTER_TBL, (void *)&entry);
		snprintf(str_mac, 32, "%02x:%02x:%02x:%02x:%02x:%02x", 
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2], entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		
		if(strncasecmp(req->clientMAC, str_mac, strlen(str_mac)) == 0){
			if(strncmp(req->accessRight, str_allow, strlen(str_allow)) == 0){
				if (apmib_set(MIB_MACFILTER_DEL, (void *)&entry) == 0) {
					printf("[%s:%d] Delete mac filter table entry error!\n", __FUNCTION__, __LINE__);
					return RTK_FAILED;
				}
			}
			return RTK_SUCCESS;
		}
	}
	
	if((i == entryNum+1) && strncmp(req->accessRight, str_deny, strlen(str_deny)) == 0){
		if(entryNum < MAX_FILTER_NUM){
			char str_all[32] = {0}, str_mac[32] = {0};
			char *ptr = NULL, *strptr = NULL;
			int count = 0, index = 0;

			snprintf(str_all, sizeof(str_all), "%s", req->clientMAC);

			if(strlen(str_all) != 17 || str_all[2] != ':' || str_all[5] != ':' || 
			str_all[8] != ':' || str_all[11] != ':' || str_all[14] != ':' || str_all[17] != '\0'){
				printf("[%s:%d] Invalid Mac Address!\n", __FUNCTION__, __LINE__);
				return RTK_FAILED;
			}

			strptr = str_all;
			while(count < 5){
				ptr = strsep(&strptr, ":");
				if(ptr == NULL){
					printf("[%s:%d] Invalid Mac Address!\n", __FUNCTION__, __LINE__);
					return RTK_FAILED;
				}

				str_mac[index] = *ptr;
				str_mac[index+1] = *(ptr+1);
				
				count++;
				index += 2;
			}

			str_mac[index] = *strptr;
			str_mac[index+1] = *(strptr+1);

			//printf("[%s:%d] str_mac: %s\n", __FUNCTION__, __LINE__, str_mac);
			
			memset(&entry, '\0', sizeof(entry));
			if(!string_to_hex(str_mac, entry.macAddr, 12)){
				printf("[%s:%d] Invalid Mac Address!\n", __FUNCTION__, __LINE__);
				return RTK_FAILED;
			}

			apmib_set(MIB_MACFILTER_DEL, (void *)&entry);
			if(apmib_set(MIB_MACFILTER_ADD, (void *)&entry) == 0) {
				printf("[%s:%d] Add mac filter table entry error!\n", __FUNCTION__, __LINE__);
				return RTK_FAILED;
			}
		}
		else{
			printf("[%s:%d] Mac filter table is already full!\n", __FUNCTION__, __LINE__);
			return RTK_FAILED;
		}
	}
	
	return RTK_SUCCESS;
}

//===================== access control end ==============================


//===================== shaping control start ============================

int cmcc_set_QOS_REQ(QOS_REQ_Tp req)
{
	unsigned char str_mac[32] = {0}, str_tmp[32] = {0}, str_mac_hex[MAC_ADDR_LEN] = {0};
	char *ptr = NULL, *strptr = NULL;
	int count = 0, index = 0;
	int qos_switch = 0, up_rate = 0, down_rate = 0;
	IPQOS_T entry;

	if(req == NULL)
		return RTK_FAILED;

	// mac
	snprintf(str_tmp, sizeof(str_tmp), "%s", req->clientMAC);
	strptr = str_tmp;
	
	while(count < 5){
		ptr = strsep(&strptr, ":");
		if(ptr == NULL){
			printf("[%s:%d] Invalid Mac Address!\n", __FUNCTION__, __LINE__);
			return RTK_FAILED;
		}

		str_mac[index] = *ptr;
		str_mac[index+1] = *(ptr+1);
		
		count++;
		index += 2;
	}

	str_mac[index] = *strptr;
	str_mac[index+1] = *(strptr+1);
//	printf("[%s:%d] str_mac: %s, req->clientMAC: %s\n", __FUNCTION__, __LINE__, 
//		str_mac, req->clientMAC);

	if(!string_to_hex(str_mac, str_mac_hex, 12)){
		printf("[%s:%d] Invalid Mac Address!\n", __FUNCTION__, __LINE__);
		return RTK_FAILED;
	}
//	printf("[%s:%d] str_mac_hex: %02x:%02x:%02x:%02x:%02x:%02x\n", __FUNCTION__, __LINE__, 
//		str_mac_hex[0], str_mac_hex[1], str_mac_hex[2], 
//		str_mac_hex[3], str_mac_hex[4], str_mac_hex[5]);

	// switch
	qos_switch = atoi(req->qosSwitch);
	if(qos_switch != QOS_DISABLE && qos_switch != QOS_ENABLE){
		printf("[%s:%d] Invalid Qos Swtich!\n", __FUNCTION__, __LINE__);
		return RTK_FAILED;
	}
	
	up_rate = atoi(req->maxUplinkRate);
	if(up_rate > 0){
		up_rate = up_rate*8;			// KBps -> Kbps
		if(up_rate > MAX_LINK_RATE){	// exceed max rate
			snprintf(req->maxUplinkRate, sizeof(req->maxUplinkRate), "%d", QOS_RATE_OUT_RANGE);
			return RTK_FAILED;
		}
	}
	else{
		if(up_rate != NO_RATE_LIMIT){
			printf("[%s:%d] Invalid Qos Uplink Rate Input!\n", __FUNCTION__, __LINE__);
			return RTK_FAILED;
		}
	}
	
	down_rate = atoi(req->maxDownlinkRate);	
	if(down_rate > 0){
		down_rate = down_rate*8;		// KBps -> Kbps
		if(down_rate > MAX_LINK_RATE){	// exceed max rate
			snprintf(req->maxDownlinkRate, sizeof(req->maxDownlinkRate), "%d", QOS_RATE_OUT_RANGE);
			return RTK_FAILED;
		}
	}
	else{
		if(down_rate != NO_RATE_LIMIT){
			printf("[%s:%d] Invalid Qos Downlink Rate Input!\n", __FUNCTION__, __LINE__);
			return RTK_FAILED;
		}
	}

	// obtain qos table
	apmib_get( MIB_QOS_RULE_TBL_NUM, (void *)&count);
	for(index = 1; index <= count; index++){
		memset(&entry, '\0', sizeof(IPQOS_T));
		*((char *)&entry) = (char)index;
		apmib_get(MIB_QOS_RULE_TBL, (void *)&entry);

		if(memcmp(entry.mac, str_mac_hex, MAC_ADDR_LEN) == 0){
			if(entry.enabled==qos_switch && up_rate==entry.bandwidth && down_rate==entry.bandwidth_downlink){
				return RTK_SUCCESS;		// exactly the same
			}
			else{
				IPQOS_T qosArray[2];
				memset(&qosArray[0], 0, sizeof(IPQOS_T));
	    		memset(&qosArray[1], 0, sizeof(IPQOS_T));
				memcpy(&qosArray[0], &entry, sizeof(IPQOS_T));

				entry.bandwidth = up_rate;
				entry.bandwidth_downlink = down_rate;
				entry.enabled = qos_switch;
				memcpy(&qosArray[1], &entry, sizeof(IPQOS_T));

				if(!apmib_set(MIB_QOS_MOD, (void *)qosArray)){
		        	printf("[%s:%d] Modify Qos Entry Error!\n", __FUNCTION__, __LINE__);
					return RTK_FAILED;
		        }

				return RTK_SUCCESS;
			}
		}
	}
		
	// Add a new entry
	memset(&entry, '\0', sizeof(IPQOS_T));
	entry.bandwidth 			= up_rate;
	entry.bandwidth_downlink	= down_rate;
	entry.enabled 				= qos_switch;
	entry.mode 					= (QOS_RESTRICT_MAC|QOS_RESTRICT_MAX);
#if defined(ADVANCED_IPQOS)
	// init params
	entry.phyport			= ADVANCED_IPQOS_DEF_CHAR_VALUE;
	entry.dscp				= ADVANCED_IPQOS_DEF_CHAR_VALUE;
	entry.vlan_pri			= ADVANCED_IPQOS_DEF_CHAR_VALUE;
	entry.remark_dscp		=ADVANCED_IPQOS_DEF_CHAR_VALUE;
	entry.remark_vlan_pri	=ADVANCED_IPQOS_DEF_CHAR_VALUE;
#endif	
	memcpy(entry.mac, str_mac_hex, MAC_ADDR_LEN);

	if(!apmib_set(MIB_QOS_ADD, (void *)&entry)){
        printf("[%s:%d] Add Qos Entry Error!\n", __FUNCTION__, __LINE__);
		return RTK_FAILED;
    }
	
	return RTK_SUCCESS;
}
//===================== shaping control end ==============================


//===================== wlan security start ==============================
/* just for debug */


/******************************************************************************/
/* SSIDList idx: 
		0:2G ROOT; 1:2G-VAP0; 2:2G-VAP1; 3:2G-VAP2; 4:2G-VAP3 
		5:5G ROOT; 6:5G-VAP0; 7:5G-VAP1; 8:5G-VAP2; 9:5G-VAP3;
*/
int cmcc_get_DEVICE_WIFI_INFO_NEW_RSP(CUST_WLAN_INFOp pwlan_info)
{
	unsigned int wlan_enable_2g = 0, wlan_enable_5g = 0;
	unsigned int wlanBand2G5GSelect, rf_band, encrypt;
	unsigned int enc_type;
	unsigned char ssid[MAX_SSID_LEN] = {0}, ifname[10] = {0}, tmpbuf[CUST_WIFI_CODE_LEN] = {0};
	int i, j, ssid_idx=0;
	unsigned char enc_str[50] = {0};

	if(pwlan_info==NULL) {
		printf("[%s %d]pwlan_info NULL !!! \n", __FUNCTION__,__LINE__);
		return RTK_FAILED;
	}
		
	memset(pwlan_info, 0, sizeof(CUST_WLAN_INFO));
	
	rtk_wlan_get_2g5g_support(&wlanBand2G5GSelect);
	if(wlanBand2G5GSelect==BANDMODESINGLE){
		rtk_wlan_get_rf_band("wlan0", &rf_band);		
		CMCC_DEBUG("rf_band:%d\n", rf_band);
	}

	CMCC_DEBUG("wlanBand2G5GSelect:%d\n", wlanBand2G5GSelect);
	
	if(wlanBand2G5GSelect==BANDMODEBOTH){
		rtk_wlan_get_enable(WLAN_2G_IFNAME, &wlan_enable_2g);
		rtk_wlan_get_enable(WLAN_5G_IFNAME, &wlan_enable_5g);
		wlan_enable_2g = (wlan_enable_2g ? STATUS_ENABLE : STATUS_DISABLE);			
		wlan_enable_5g = (wlan_enable_5g ? STATUS_ENABLE : STATUS_DISABLE);	
		CMCC_DEBUG("wlan_enable_2g:%d, wlan_enable_5g:%d\n",wlan_enable_2g, wlan_enable_5g);
	}else if((wlanBand2G5GSelect==BANDMODE5G) || 
					((wlanBand2G5GSelect==BANDMODESINGLE) && (rf_band==PHYBAND_5G))){
		wlan_enable_2g = STATUS_NOT_SUPPORT;
		rtk_wlan_get_enable(WLAN_5G_IFNAME, &wlan_enable_5g);
		wlan_enable_5g = (wlan_enable_5g ? STATUS_ENABLE : STATUS_DISABLE);	
		CMCC_DEBUG("wlan_enable_2g:%d, wlan_enable_5g:%d\n",wlan_enable_2g, wlan_enable_5g);
	}else if((wlanBand2G5GSelect==BANDMODE2G) ||
					((wlanBand2G5GSelect==BANDMODESINGLE) && (rf_band==PHYBAND_2G))){
		wlan_enable_5g = STATUS_NOT_SUPPORT;
		rtk_wlan_get_enable(WLAN_2G_IFNAME, &wlan_enable_2g);
		wlan_enable_2g = (wlan_enable_2g ? STATUS_ENABLE : STATUS_DISABLE);		
		CMCC_DEBUG("wlan_enable_2g:%d, wlan_enable_5g:%d\n",wlan_enable_2g, wlan_enable_5g);
	}else {
		printf("[%s %d]Invalid band[%d].\n",__FUNCTION__,__LINE__,wlanBand2G5GSelect);
		return RTK_FAILED;
	}

	/* get wlan status */
	snprintf(pwlan_info->WifiStatus, CUST_WIFI_STATUS_LEN, "%d", wlan_enable_2g);
	snprintf(pwlan_info->WifiStatus_5G, CUST_WIFI_STATUS_LEN, "%d", wlan_enable_5g);	
	CMCC_DEBUG("WifiStatus:%s, WifiStatus_5G:%s\n", pwlan_info->WifiStatus, pwlan_info->WifiStatus_5G);

	/* get wlan ssid list , 2g first*/
	if(wlan_enable_2g){ // support 2G	
		for(i=0; i<=NUM_VWLAN; ++i) {
			if(i)
				snprintf(ifname, sizeof(ifname), "%s-va%d", WLAN_2G_IFNAME, i-1);				
			else
				snprintf(ifname, sizeof(ifname), "%s", WLAN_2G_IFNAME);	
			CMCC_DEBUG("====>>>> ifname:%s\n", ifname);
			
			/* ssid */ 
			rtk_wlan_get_ssid(ifname, ssid);			
			snprintf(pwlan_info->SSIDList[ssid_idx].SSID, CUST_WIFI_SSID_LEN, "%s", ssid);
			CMCC_DEBUG("idx:%d, ssid:%s\n", ssid_idx, pwlan_info->SSIDList[ssid_idx].SSID);
			
			/* encrypt */
			rtk_wlan_get_encryption(ifname, &encrypt);
			cmcc_get_encrypt_type(ifname, encrypt, &enc_type);			
			snprintf(pwlan_info->SSIDList[ssid_idx].encryptType, CUST_WIFI_ENCRPTY_TYPE_LEN, "%d", enc_type);				
			cmcc_get_encrpt_string(enc_type, enc_str);			
			CMCC_DEBUG("idx:%d, encrypt:%d, enc_type:%s, enc_str:%s\n", 
							ssid_idx, encrypt, pwlan_info->SSIDList[ssid_idx].encryptType, enc_str);

			/* encrypt code */	
			rtk_wlan_get_enc_code(ifname, encrypt, tmpbuf, CUST_WIFI_CODE_LEN);
			snprintf(pwlan_info->SSIDList[ssid_idx].CODE, CUST_WIFI_CODE_LEN, "%s", tmpbuf);
			CMCC_DEBUG("idx:%d, CODE:%s\n", ssid_idx, pwlan_info->SSIDList[ssid_idx].CODE);

			/* frequency */
			snprintf(pwlan_info->SSIDList[ssid_idx].frequency, CUST_WIFI_FREQUENCY_LEN, "%d", FREQUENCY_BAND_2G);			
			CMCC_DEBUG("idx:%d, frequency:%s\n\n", ssid_idx, pwlan_info->SSIDList[ssid_idx].frequency);
			ssid_idx++;
		}
	}
	
	if(wlan_enable_5g){		
		for(i=0; i<=NUM_VWLAN; ++i) {
			if(i)
				snprintf(ifname, sizeof(ifname), "%s-va%d", WLAN_5G_IFNAME, i-1);				
			else
				snprintf(ifname, sizeof(ifname), "%s", WLAN_5G_IFNAME);	
			CMCC_DEBUG("====>>>> ifname:%s\n", ifname);
			
			/* ssid */ 
			rtk_wlan_get_ssid(ifname, ssid);
			
			snprintf(pwlan_info->SSIDList[ssid_idx].SSID, CUST_WIFI_SSID_LEN, "%s", ssid);			
			CMCC_DEBUG("idx:%d, ssid:%s\n", ssid_idx, pwlan_info->SSIDList[ssid_idx].SSID);

			/* encrypt */
			rtk_wlan_get_encryption(ifname, &encrypt);
			cmcc_get_encrypt_type(ifname, encrypt, &enc_type);			
			snprintf(pwlan_info->SSIDList[ssid_idx].encryptType, CUST_WIFI_ENCRPTY_TYPE_LEN, "%d", enc_type);
			cmcc_get_encrpt_string(enc_type, enc_str);			
			CMCC_DEBUG("idx:%d, encrypt:%d, enc_type:%s, enc_str:%s\n", 
							ssid_idx, encrypt, pwlan_info->SSIDList[ssid_idx].encryptType, enc_str);

			/* encrypt code */	
			rtk_wlan_get_enc_code(ifname, encrypt, tmpbuf, CUST_WIFI_CODE_LEN);
			snprintf(pwlan_info->SSIDList[ssid_idx].CODE, CUST_WIFI_CODE_LEN, "%s", tmpbuf);			
			CMCC_DEBUG("idx:%d, CODE:%s\n", ssid_idx, pwlan_info->SSIDList[ssid_idx].CODE);			

			/* frequency */
			snprintf(pwlan_info->SSIDList[ssid_idx].frequency, CUST_WIFI_FREQUENCY_LEN, "%d", FREQUENCY_BAND_5G);
			CMCC_DEBUG("idx:%d, frequency:%s\n", ssid_idx, pwlan_info->SSIDList[ssid_idx].frequency);			
			ssid_idx++;
		}
	}	
	
	snprintf(pwlan_info->SSIDNum, CUST_SSID_NUM, "%d", ssid_idx);	
	CMCC_DEBUG("SSIDNum:%s\n", pwlan_info->SSIDNum); 		
	return RTK_SUCCESS;
}

/******************************************************************************/
/* SSIDList idx: 
		0:2G ROOT; 
*/
int cmcc_get_WIFI_SWITCH_STATUS_RSP(unsigned char *status)
{
	unsigned int wlan_enable = 0;
	unsigned char ifname[10] = {0};

	if(NUM_WLAN_INTERFACE==2)
		snprintf(ifname, sizeof(ifname), "%s", "wlan1");
	else
		snprintf(ifname, sizeof(ifname), "%s", "wlan0");

	rtk_wlan_get_enable(ifname, &wlan_enable);	
	if(wlan_enable==0)
		sprintf(status, "%d", 2);
	else
		sprintf(status, "%d", 1);

	CMCC_DEBUG("disable:%d, status:%s\n", wlan_enable, status);
	
	return RTK_SUCCESS;
}

/* SSIDList idx: 
		0:2G ROOT; 1:2G-VAP0; 2:2G-VAP1; 3:2G-VAP2; 4:2G-VAP3 
		5:5G ROOT; 6:5G-VAP0; 7:5G-VAP1; 8:5G-VAP2; 9:5G-VAP3;
*/

int cmcc_set_DEVICE_WIFI_NEW_RSP(CUST_WLAN_INFO wlan_info)
{
	unsigned int WLAN_STATUS_REINIT = 2;
	unsigned int WLAN_STATUS_INVALID = 3;
	unsigned int wlan_enable_2g = 0, wlan_enable_5g = 0;
	unsigned int support_2g = 0, support_5g = 0;
	unsigned int setValue;
	unsigned int wlanBand2G5GSelect, rf_band, encryptType, encrypt, frequency;
	int i, ssid_len, ssid_idx=0;
	unsigned char ifname[10] = {0}, ssid[MAX_SSID_LEN] = {0};
	unsigned char enc_str[50] = {0};
	WLAN_SSID_INFO ssid_info;
	CUST_WLAN_INFO wlan_orig_info;
	int kk = 0, mm = 0;
	int key_length = 0;
	int total_ssid_num = 0;
	
	memset(&ssid_info, 0, sizeof(WLAN_SSID_INFO));
	
	rtk_wlan_get_2g5g_support(&wlanBand2G5GSelect);
	if(wlanBand2G5GSelect==BANDMODESINGLE){
		rtk_wlan_get_rf_band("wlan0", &rf_band);		
		CMCC_DEBUG("rf_band:%d\n", rf_band);
	}

	CMCC_DEBUG("wlanBand2G5GSelect:%d\n", wlanBand2G5GSelect);
	
	if(wlanBand2G5GSelect==BANDMODEBOTH){
		support_2g = 1;
		support_5g = 1;
		CMCC_DEBUG("support_2g:%d, support_5g:%d\n", support_2g, support_5g);
	}else if((wlanBand2G5GSelect==BANDMODE5G) || 
					((wlanBand2G5GSelect==BANDMODESINGLE) && (rf_band==PHYBAND_5G))){
		support_5g = 1;
		CMCC_DEBUG("support_2g:%d, support_5g:%d\n", support_2g, support_5g);
	}else if((wlanBand2G5GSelect==BANDMODE2G) ||
					((wlanBand2G5GSelect==BANDMODESINGLE) && (rf_band==PHYBAND_2G))){
		support_2g = 1;	
		CMCC_DEBUG("support_2g:%d, support_5g:%d\n", support_2g, support_5g);
	}else {
		printf("[%s %d]Invalid wlanBand2G5GSelect[%d].\n",__FUNCTION__,__LINE__,wlanBand2G5GSelect);
		return RTK_FAILED;
	}
	
	memset(&wlan_orig_info, 0, sizeof(CUST_WLAN_INFO));
	cmcc_get_DEVICE_WIFI_INFO_NEW_RSP(&wlan_orig_info);

	total_ssid_num = atoi(wlan_orig_info.SSIDNum);
	CMCC_DEBUG("(get)total_ssid_num:%d\n", total_ssid_num);

	for(kk=0; kk<total_ssid_num; ++kk){	
		if(atoi(wlan_info.SSIDList[kk].encryptType)==TYPE_WEP){				
			key_length = strlen(wlan_info.SSIDList[kk].CODE);			
			if(key_length==10 || key_length==26){
				for(mm=0; mm<key_length;++mm){
					wlan_info.SSIDList[kk].CODE[mm] = tolower(wlan_info.SSIDList[kk].CODE[mm]);					
				}				
			}			
		}
	}		
	
	if(memcmp(&wlan_orig_info, &wlan_info, sizeof(CUST_WLAN_INFO)) == 0)
	{
		printf("====%s(%d) same !!!!\n", __FUNCTION__, __LINE__);		
		return 2;
	}else{
		int j;
		printf("====%s(%d) not same\n", __FUNCTION__, __LINE__);
		
		if(memcmp(wlan_orig_info.WifiStatus, wlan_info.WifiStatus, sizeof(wlan_info.WifiStatus)))
			CMCC_DEBUG("ori WifiStatus:%s, cur WifiStatus:%s\n", wlan_orig_info.WifiStatus, wlan_info.WifiStatus);

		if(memcmp(wlan_orig_info.WifiStatus_5G, wlan_info.WifiStatus_5G, sizeof(wlan_info.WifiStatus_5G)))
			CMCC_DEBUG("ori WifiStatus_5G:%s, cur WifiStatus_5G:%s\n", wlan_orig_info.WifiStatus_5G, wlan_info.WifiStatus_5G);

		if(memcmp(wlan_orig_info.SSIDNum, wlan_info.SSIDNum, CUST_SSID_NUM))
			CMCC_DEBUG("ori SSIDNum:%s, cur SSIDNum:%s\n", wlan_orig_info.SSIDNum,  wlan_info.SSIDNum);
		
		for(i=0; i<total_ssid_num; ++i){
			
			if(memcmp(wlan_orig_info.SSIDList[i].SSID, wlan_info.SSIDList[i].SSID, CUST_WIFI_SSID_LEN))
				CMCC_DEBUG("idx:%d, ori ssid:%s, cur ssid:%s\n", i, wlan_orig_info.SSIDList[i].SSID,  wlan_info.SSIDList[i].SSID);

			if(memcmp(wlan_orig_info.SSIDList[i].encryptType, wlan_info.SSIDList[i].encryptType, CUST_WIFI_ENCRPTY_TYPE_LEN))
				CMCC_DEBUG("idx:%d, ori enc:%s, cur enc:%s\n", i, wlan_orig_info.SSIDList[i].encryptType,  wlan_info.SSIDList[i].encryptType);

			if(memcmp(wlan_orig_info.SSIDList[i].CODE, wlan_info.SSIDList[i].CODE, CUST_WIFI_CODE_LEN))
				CMCC_DEBUG("idx:%d, ori code:%s, cur code:%s\n", i, wlan_orig_info.SSIDList[i].CODE,  wlan_info.SSIDList[i].CODE);

			if(memcmp(wlan_orig_info.SSIDList[i].frequency, wlan_info.SSIDList[i].frequency, CUST_WIFI_FREQUENCY_LEN))
				CMCC_DEBUG("idx:%d, ori fre:%s, cur fre:%s\n", i, wlan_orig_info.SSIDList[i].frequency,  wlan_info.SSIDList[i].frequency);			
		}
	}		

	wlan_enable_2g = atoi(wlan_info.WifiStatus);
	wlan_enable_5g = atoi(wlan_info.WifiStatus_5G);
	
	if(wlan_enable_2g==STATUS_ENABLE)
		wlan_enable_2g = 1;
	else if(wlan_enable_2g==STATUS_DISABLE)
		wlan_enable_2g = 0;
	else if(wlan_enable_2g==STATUS_REINIT)
		wlan_enable_2g = WLAN_STATUS_REINIT;
	else {
		wlan_enable_2g = WLAN_STATUS_INVALID;
		printf("[%s %d]Invalid WifiStatus %d\n", __FUNCTION__,__LINE__,wlan_enable_2g);		
	}
	
	if(wlan_enable_5g==STATUS_ENABLE)
		wlan_enable_5g = 1;
	else if(wlan_enable_5g==STATUS_DISABLE)
		wlan_enable_5g = 0;
	else if(wlan_enable_5g==STATUS_REINIT)
		wlan_enable_5g = WLAN_STATUS_REINIT;
	else {
		wlan_enable_5g = WLAN_STATUS_INVALID;
		printf("[%s %d]Invalid WifiStatus_5G %d\n", __FUNCTION__,__LINE__,wlan_enable_5g);		
	}
	
	if(support_2g){ // support 2G	
		for(i=0; i<=NUM_VWLAN; ++i) {		
			if(i)
				snprintf(ifname, sizeof(ifname), "%s-va%d", WLAN_2G_IFNAME, i-1);				
			else
				snprintf(ifname, sizeof(ifname), "%s", WLAN_2G_IFNAME);	
			
			CMCC_DEBUG("====>>>> ifname:%s\n", ifname);
			
			ssid_info = wlan_info.SSIDList[ssid_idx];
			frequency = atoi(ssid_info.frequency);
			encryptType = atoi(ssid_info.encryptType);
			cmcc_get_encrpt_string(encryptType, enc_str);  // just for debug
			
			CMCC_DEBUG("ssid_idx:%d, ssid:%s\n", ssid_idx, ssid_info.SSID);
			CMCC_DEBUG("ssid_idx:%d, encryptType:%s,%d\n", ssid_idx, ssid_info.encryptType, encryptType);			
			CMCC_DEBUG("ssid_idx:%d, encStr:%s\n", ssid_idx, enc_str);
			CMCC_DEBUG("ssid_idx:%d, CODE:%s\n", ssid_idx, ssid_info.CODE);
			CMCC_DEBUG("ssid_idx:%d, frequency:%s,%d\n", ssid_idx, ssid_info.frequency, frequency);
			CMCC_DEBUG("wlan_enable_2g:%d\n",wlan_enable_2g);
			
			if( frequency==FREQUENCY_BAND_2G ){
				/* status */
				if(i==0) // just root
				{
					if(wlan_enable_2g == WLAN_STATUS_REINIT){
						rtk_wlan_down_wlan(WLAN_2G_IFNAME);
						rtk_wlan_up_wlan(WLAN_2G_IFNAME);
					}else if(wlan_enable_2g==0 || wlan_enable_2g==1){
						rtk_wlan_set_wlan_enable(WLAN_2G_IFNAME, wlan_enable_2g);	
					}		
				}
				
				/* ssid */ 
				ssid_len = strlen(ssid_info.SSID);
				ssid_len = ((ssid_len>MAX_SSID_LEN)?MAX_SSID_LEN:ssid_len);
				snprintf(ssid, ssid_len+1, "%s", ssid_info.SSID);
				rtk_wlan_set_ssid(ifname, ssid);			
				CMCC_DEBUG("ssid_len:%d, ssid:%s\n", ssid_len, ssid);
				
				/* encrypt and encrypt code */			
				cmcc_set_encrypt_type(ifname, encryptType, ssid_info.CODE);					

				/* frequency :do nothing */			
				printf("rf frequency cannot be changed. \n");			
			}else
				printf("the board just support 2G, cannot set 5G information\n");
			
			ssid_idx++;
		}
	}
	
	if(support_5g){		
		for(i=0; i<=NUM_VWLAN; ++i){
			if(i)
				snprintf(ifname, sizeof(ifname), "%s-va%d", WLAN_5G_IFNAME, i-1);				
			else
				snprintf(ifname, sizeof(ifname), "%s", WLAN_5G_IFNAME);	
			
			CMCC_DEBUG("====>>>> ifname:%s\n", ifname);
			
			ssid_info = wlan_info.SSIDList[ssid_idx];
			encryptType = atoi(ssid_info.encryptType);
			cmcc_get_encrpt_string(encryptType, enc_str);  // just for debug
			frequency = atoi(ssid_info.frequency);
			
			CMCC_DEBUG("ssid_idx:%d, ssid:%s\n", ssid_idx, ssid_info.SSID);
			CMCC_DEBUG("ssid_idx:%d, encryptType:%s,%d\n", ssid_idx, ssid_info.encryptType,encryptType);
			CMCC_DEBUG("ssid_idx:%d, encStr:%s\n", ssid_idx,  enc_str);
			CMCC_DEBUG("ssid_idx:%d, CODE:%s\n", ssid_idx, ssid_info.CODE);
			CMCC_DEBUG("ssid_idx:%d, frequency:%s,%d\n", ssid_idx, ssid_info.frequency, frequency);
			CMCC_DEBUG("wlan_enable_5g:%d\n",wlan_enable_5g);

			if( frequency==FREQUENCY_BAND_5G ){				
				/* status */
				if(i==0) // just root
				{
					if(wlan_enable_5g == WLAN_STATUS_REINIT){
						rtk_wlan_down_wlan(WLAN_5G_IFNAME);
						rtk_wlan_up_wlan(WLAN_5G_IFNAME);
					}else if(wlan_enable_5g==0 || wlan_enable_5g==1){
						rtk_wlan_set_wlan_enable(WLAN_5G_IFNAME, wlan_enable_5g);	
					}		
				}				
				
				/* ssid */ 
				ssid_len = strlen(ssid_info.SSID);
				ssid_len = ((ssid_len>MAX_SSID_LEN)?MAX_SSID_LEN:ssid_len);
				snprintf(ssid, ssid_len+1, "%s", ssid_info.SSID);
				rtk_wlan_set_ssid(ifname, ssid);			
				CMCC_DEBUG("idx:%d, ssid_len:%d, ssid:%s\n", ssid_idx, ssid_len, ssid);
				
				/* encrypt and encrypt code */			
				cmcc_set_encrypt_type(ifname, encryptType, ssid_info.CODE);			

				/* frequency:do nothing */
				printf("no change band frequency\n");	
			}else
				printf("the board just support 5G, cannot set 2G information\n");
			
			ssid_idx++;
		}
	}
			
	return RTK_SUCCESS;
}
//===================== wlan security end ==============================

//===================== wlan accelerate start ============================
/*
	accelerate : 		
		1: wlan_accelerate
		0: not wlan_accelerate	
*/
#if 0 
/* game accelerate */
//int cmcc_WIFI_ACCELERATE(unsigned char *ifname, unsigned int accelerate)
int cmcc_WIFI_ACCELERATE(unsigned int accelerate)
{
	int ret = RTK_SUCCESS, i;
	unsigned char cmd[50] = {0};
	unsigned char root[10] = {0};
	int wlan_sta_num = 0;

#if 0
	if(strstr(ifname, "wlan0-va"))
		snprintf(root, sizeof(root), "%s", "wlan0");
	else if(strstr(ifname, "wlan1-va"))
		snprintf(root, sizeof(root), "%s", "wlan1");	
	else
		snprintf(root, sizeof(root), "%s", ifname);
#endif
	
	for(i=0; i<NUM_WLAN_INTERFACE; i++){
		snprintf(root, sizeof(root), "wlan%d", i);
		if (getWlStaNum(root, &wlan_sta_num) < 0)
			continue;
		CMCC_DEBUG("root:%s, accelerate:%d\n", root, accelerate);			
		if(rtk_wlan_set_wlan_accelerate(root, accelerate)==RTK_FAILED)
			return RTK_FAILED;

		snprintf(cmd, sizeof(cmd), "iwpriv %s set_mib qos_enhance_enable=%d", root, accelerate);
		CMCC_DEBUG("cmd:%s\n", cmd);		
		system(cmd);
	}		
	
	return RTK_SUCCESS;
}
#else
/* auto channel */
int cmcc_WIFI_ACCELERATE(unsigned int accelerate) 
{
	int ret = RTK_SUCCESS, i;
	unsigned char cmd[50] = {0};
	unsigned char root[10] = {0};
	int wlan_sta_num = 0;
	unsigned int channel = 0;

	if(accelerate){
		for(i=0; i<NUM_WLAN_INTERFACE; i++){
			snprintf(root, sizeof(root), "wlan%d", i);
			if (getWlStaNum(root, &wlan_sta_num) < 0)
				continue;
			
			CMCC_DEBUG("root:%s, accelerate:%d\n", root, accelerate);	
			
			rtk_wlan_get_channel(root, (void *)&channel);
			if( channel!=0 ){ // fix channel
				ret  = RTK_FAILED;
				continue;
			}		
			snprintf(cmd, sizeof(cmd), "iwpriv %s autoch", root);
			CMCC_DEBUG("cmd:%s\n", cmd);		
			system(cmd);
		}		
	}	
	return ret;
}
#endif
//===================== wlan accelerate end ==============================

//===================== upgrade firmware begin ==============================
int cmcc_get_DEVUPDATE_INFO_REQ(struct devUpdate_info_req *req)
{
	struct rtk_dut_info info={0};
	char errmsg[128]={0};
	if(!req){
		snprintf(errmsg,sizeof(errmsg),"invalid input!%s %d\n",__FILE__,__LINE__);
		goto DEVUPDATE_INFO_REQ_FAIL;
	}
	if(rtk_get_dut_info(&info)!=RTK_SUCCESS){
		snprintf(errmsg,sizeof(errmsg),"rtk_get_dut_info fail!%s %d\n",__FILE__,__LINE__);
		goto DEVUPDATE_INFO_REQ_FAIL;
	}
    strncpy(req->devID, info.sn,sizeof(req->devID));
	switch(info.opmode){
		case GATEWAY_MODE:
			snprintf(req->type,sizeof(req->type),"2");
			break;
		case BRIDGE_MODE:
		    if(info.repeater_mode)
		        snprintf(req->type,sizeof(req->type),"16");
		    else
			    snprintf(req->type,sizeof(req->type),"8");
			break;
		default:
			snprintf(errmsg,sizeof(errmsg),"opmode is %s invalid! %s %d\n",info.opmode,__FILE__,__LINE__);
			goto DEVUPDATE_INFO_REQ_FAIL;
	}

	strncpy(req->currentVersion,info.swversion,sizeof(req->currentVersion));
	
	return RTK_SUCCESS;
DEVUPDATE_INFO_REQ_FAIL:
	fprintf(stderr,"%s",errmsg);
	return RTK_FAILED;
}

int cmcc_upgradeImage_via_url(char* url)
{
	char errmsg[128]={0};
	struct stat fileStat={0};
	char * updateData=NULL;
	int updateLen=0;
	FILE *fp=NULL;
	if(!url||!url[0]) return RTK_SUCCESS;

	if(rtl_get_file_via_url(url,UPDATE_IMAGE_FILE)!=RTK_SUCCESS)
		return RTK_FAILED;
	if(stat(UPDATE_IMAGE_FILE,&fileStat)<0){
		fprintf(stderr,"stat %s fail! %s:%d\n",UPDATE_IMAGE_FILE,__FILE__,__LINE__);
		return RTK_FAILED;
	}
	updateLen=fileStat.st_size;
	//fprintf(stderr,"%s:%d updateLen=%d\n",__FUNCTION__,__LINE__,updateLen);
	if((updateData=malloc(updateLen))==NULL){
		fprintf(stderr,"malloc %d fail! %s:%d\n",updateLen,__FILE__,__LINE__);
		return RTK_FAILED;
	}
	if((fp=fopen(UPDATE_IMAGE_FILE,"r"))==NULL){
		snprintf(errmsg,sizeof(errmsg),"open %s fail %s:%d\n",UPDATE_IMAGE_FILE,__FILE__,__LINE__);
		goto CMCC_UPGRADEIMAGE_VIA_URL_FAIL;
	}
	if(fread(updateData,updateLen,1,fp)!=1){
		snprintf(errmsg,sizeof(errmsg),"read %s fail %s:%d\n",UPDATE_IMAGE_FILE,__FILE__,__LINE__);
		goto CMCC_UPGRADEIMAGE_VIA_URL_FAIL;
	}
	fclose(fp);
	fp=NULL;
	if(upgrade_firmware(updateData,updateLen,errmsg,sizeof(errmsg))!=RTK_SUCCESS){
		goto CMCC_UPGRADEIMAGE_VIA_URL_FAIL;
	}
	free(updateData);
#ifndef CONFIG_APP_FWD
	REBOOT_WAIT_COMMAND(2);
#endif
	return RTK_SUCCESS;
CMCC_UPGRADEIMAGE_VIA_URL_FAIL:
	if(updateData) free(updateData);
	if(fp) fclose(fp);	
	fprintf(stderr,"%s\n",errmsg);
	return RTK_FAILED;
}
int cmcc_set_DEVUPDATE_INFO_RSP(struct devUpdate_info_rsp *rsp)
{
	char errmsg[128]={0};
	if(!rsp){
		snprintf(errmsg,sizeof(errmsg),"invalid input!%s %d\n",__FILE__,__LINE__);
		goto DEVUPDATE_INFO_RSP_FAIL;
	}
	if(strcmp(rsp->needUpdate,"0")==0){
		//need no update
	}
	else if(strcmp(rsp->needUpdate,"1")==0){
		//must update
		if(cmcc_upgradeImage_via_url(rsp->updateUrl)==RTK_FAILED){
			snprintf(errmsg,sizeof(errmsg),"cmcc_upgradeImage_via_url fail!%s %d\n",__FILE__,__LINE__);
			goto DEVUPDATE_INFO_RSP_FAIL;
		}
	}
	else if(strcmp(rsp->needUpdate,"2")==0){
		//option update
	}
	else{
		snprintf(errmsg,sizeof(errmsg),"rsp needUpdate value invalid!%s %d\n",__FILE__,__LINE__);
		goto DEVUPDATE_INFO_RSP_FAIL;
	}
	
	return RTK_SUCCESS;
DEVUPDATE_INFO_RSP_FAIL:
	fprintf(stderr,"%s\n",errmsg);
	return RTK_FAILED;
}
int cmcc_set_DEVUPDATE_REQ(struct devUpdate_req *req)
{
	char errmsg[128]={0};
	if(!req){
		snprintf(errmsg,sizeof(errmsg),"invalid input!%s %d\n",__FILE__,__LINE__);
		goto DEVUPDATE_REQ_FAIL;
	}
	if(cmcc_upgradeImage_via_url(req->updateUrl)==RTK_FAILED){
			snprintf(errmsg,sizeof(errmsg),"cmcc_upgradeImage_via_url fail!%s %d\n",__FILE__,__LINE__);
			goto DEVUPDATE_REQ_FAIL;
		}
	return RTK_SUCCESS;
DEVUPDATE_REQ_FAIL:
	fprintf(stderr,"%s\n",errmsg);
	return RTK_FAILED;
}
int cmcc_get_DEVUPDATE_RSP()
{
	//nothing to prepare
	return RTK_SUCCESS;
}

int cmcc_get_DEVUPDATE_STATUS(int * status)
{
	int upgradeFw=0;
	if(!status){
		fprintf(stderr,"input error! %s:%d\n",__FILE__,__LINE__);
		return RTK_FAILED;
	}
	if(apmib_get(MIB_UPGRADED_FW,(void*)&upgradeFw)==0){
		fprintf(stderr,"Get MIB_UPGRADED_FW fail!\n");
		return RTK_FAILED;
	}
	*status=upgradeFw;
	return RTK_SUCCESS;
}
int cmcc_set_DEVUPDATE_STATUS(int        status)
{
	if(apmib_set(MIB_UPGRADED_FW,(void*)&status)==0){
		fprintf(stderr,"Set MIB_UPGRADED_FW fail!\n");
		return RTK_FAILED;
	}
	return RTK_SUCCESS;
}

void cmcc_get_DEVICE_STATUS_CHANG_REQ(struct device_status_change *req)
{	
	int status;
	struct rtk_dut_info info;
    
    rtk_get_dut_info(&info);
    strcpy(req->deviceID, info.sn);

	cmcc_get_DEVUPDATE_STATUS(&status);
	if(status == 1)
		strcpy(req->Status,"2");
	else
		strcpy(req->Status,"1");
}

//===================== upgrade firmware end ==============================
//===================== get network info start ============================
int cmcc_get_BROADBAND_SETTING_REQ()
{
	//nothing to prepare
	return RTK_SUCCESS;
}
int cmcc_get_BROADBAND_SETTING_RSP(struct boardBand_setting* rsp)
{
	char errmsg[128]={0};
	int wan_dhcp=0;
	char pppoeUser[MAX_NAME_LEN_LONG]={0};
	char pppoepsw[MAX_NAME_LEN_LONG]={0};
	char wanIpAddr[4]={0};
	char wanSubnetMask[4]={0};
	char wanDefaultGateway[4]={0};
	char dns1[4]={0};
	char dns2[4]={0};
	if(!rsp){
		snprintf(errmsg,sizeof(errmsg),"invalid input!%s %d\n",__FILE__,__LINE__);
		goto BROADBAND_SETTING_RSP_FAIL;
	}
	if(apmib_get(MIB_WAN_DHCP,(void*)&wan_dhcp)==0){
		snprintf(errmsg,sizeof(errmsg),"get MIB_WAN_DHCP fail!%s %d\n",__FILE__,__LINE__);
		goto BROADBAND_SETTING_RSP_FAIL;
	}
	
	if(wan_dhcp==DHCP_DISABLED){
		if(apmib_get(MIB_WAN_IP_ADDR,(void*)wanIpAddr)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_WAN_IP_ADDR fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		if(apmib_get(MIB_WAN_DEFAULT_GATEWAY,(void*)wanDefaultGateway)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_WAN_DEFAULT_GATEWAY fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		if(apmib_get(MIB_WAN_SUBNET_MASK,(void*)wanSubnetMask)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_WAN_SUBNET_MASK fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		if(apmib_get(MIB_DNS1,(void*)dns1)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_DNS1 fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		if(apmib_get(MIB_DNS2,(void*)dns2)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_DNS2 fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		snprintf(rsp->type,sizeof(rsp->type),"4");
		snprintf(rsp->IP,sizeof(rsp->IP),"%s",inet_ntoa(*((struct in_addr*)wanIpAddr)));
		snprintf(rsp->gatewayIP,sizeof(rsp->gatewayIP),"%s",inet_ntoa(*((struct in_addr*)wanDefaultGateway)));
		snprintf(rsp->submask,sizeof(rsp->submask),"%s",inet_ntoa(*((struct in_addr*)wanSubnetMask)));
		snprintf(rsp->DNS1,sizeof(rsp->DNS1),"%s",inet_ntoa(*((struct in_addr*)dns1)));
		snprintf(rsp->DNS2,sizeof(rsp->DNS2),"%s",inet_ntoa(*((struct in_addr*)dns2)));		
	}
	else if(wan_dhcp==DHCP_CLIENT)
		snprintf(rsp->type,sizeof(rsp->type),"2");
	else if(wan_dhcp==PPPOE){
		if(apmib_get(MIB_PPP_USER_NAME,(void*)pppoeUser)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_PPP_USER_NAME fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		if(apmib_get(MIB_PPP_PASSWORD,(void*)pppoepsw)==0){
			snprintf(errmsg,sizeof(errmsg),"get MIB_PPP_PASSWORD fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_RSP_FAIL;
		}
		snprintf(rsp->type,sizeof(rsp->type),"1");
		snprintf(rsp->PPPoEName,sizeof(rsp->PPPoEName),"%s",pppoeUser);
		snprintf(rsp->PPPoECode,sizeof(rsp->PPPoECode),"%s",pppoepsw);
	}
	else
		snprintf(rsp->type,sizeof(rsp->type),"0");
	return RTK_SUCCESS;
BROADBAND_SETTING_RSP_FAIL:
	fprintf(stderr,"%s\n",errmsg);
	return RTK_FAILED;
}
int cmcc_set_BROADBAND_SETTING_REQ(struct boardBand_setting* req)
{
	char errmsg[128]={0};
	int wan_dhcp=0;
	char pppoeUser[MAX_NAME_LEN_LONG]={0};
	char pppoepsw[MAX_NAME_LEN_LONG]={0};
	char wanIpAddr[4]={0};
	char wanSubnetMask[4]={0};
	char wanDefaultGateway[4]={0};
	char dns1[4]={0};
	char dns2[4]={0};
	if(!req){
		snprintf(errmsg,sizeof(errmsg),"invalid input!%s %d\n",__FILE__,__LINE__);
		goto BROADBAND_SETTING_REQ_FAIL;
	}
	if(strcmp(req->type,"1")==0){
		wan_dhcp=PPPOE;
		strcpy(pppoeUser,req->PPPoEName);
		strcpy(pppoepsw,req->PPPoECode);
		if(apmib_set(MIB_PPP_USER_NAME,(void*)pppoeUser)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_PPP_USER_NAME fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(apmib_set(MIB_PPP_PASSWORD,(void*)pppoepsw)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_PPP_PASSWORD fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
	}
	else if(strcmp(req->type,"2")==0){
		wan_dhcp=DHCP_CLIENT;
	}
	else if(strcmp(req->type,"4")==0){
		wan_dhcp=DHCP_DISABLED;
		if(inet_aton(req->IP, &wanIpAddr)==0){
			snprintf(errmsg,sizeof(errmsg),"invalid input ipaddr=%s!%s %d\n",req->IP,__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(inet_aton(req->gatewayIP, &wanDefaultGateway)==0){
			snprintf(errmsg,sizeof(errmsg),"invalid input gatewayIP=%s!%s %d\n",req->gatewayIP,__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(inet_aton(req->submask, &wanSubnetMask)==0){
			snprintf(errmsg,sizeof(errmsg),"invalid input submask=%s!%s %d\n",req->submask,__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(inet_aton(req->DNS1, &dns1)==0){
			snprintf(errmsg,sizeof(errmsg),"invalid input DNS1=%s!%s %d\n",req->DNS1,__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(inet_aton(req->DNS2, &dns2)==0){
			snprintf(errmsg,sizeof(errmsg),"invalid input DNS2=%s!%s %d\n",req->DNS2,__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(apmib_set(MIB_WAN_IP_ADDR,(void*)wanIpAddr)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_WAN_IP_ADDR fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(apmib_set(MIB_WAN_DEFAULT_GATEWAY,(void*)wanDefaultGateway)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_WAN_DEFAULT_GATEWAY fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(apmib_set(MIB_WAN_SUBNET_MASK,(void*)wanSubnetMask)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_WAN_SUBNET_MASK fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(apmib_set(MIB_DNS1,(void*)dns1)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_DNS1 fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
		if(apmib_set(MIB_DNS2,(void*)dns2)==0){
			snprintf(errmsg,sizeof(errmsg),"set MIB_DNS2 fail!%s %d\n",__FILE__,__LINE__);
			goto BROADBAND_SETTING_REQ_FAIL;
		}
	}else{
		snprintf(errmsg,sizeof(errmsg),"invalid input type=%s!%s %d\n",req->type,__FILE__,__LINE__);
		goto BROADBAND_SETTING_REQ_FAIL;
	}
	if(apmib_set(MIB_WAN_DHCP,(void*)&wan_dhcp)==0){
		snprintf(errmsg,sizeof(errmsg),"set MIB_WAN_DHCP fail!%s %d\n",__FILE__,__LINE__);
		goto BROADBAND_SETTING_REQ_FAIL;
	}
	return RTK_SUCCESS;
BROADBAND_SETTING_REQ_FAIL:
	fprintf(stderr,"%s\n",errmsg);
	return RTK_FAILED;
}
int cmcc_set_BROADBAND_SETTING_RSP()
{//nothing to prepare
	return RTK_SUCCESS;
}
//===================== get network info end ===============================

/*
*Note :
*	1.if(req->log != NULL) -> Got new log messages!
*	2. Free req->log after use it.
*
*/
void cmcc_get_LOG_UPLOAD_REQ(struct log_upload_req *req)
{
	struct rtk_dut_info info;
	struct stat file_stat;
	static time_t last_mtime=0;
	static unsigned int last_size=0;
	FILE *fp;
	unsigned int len;
	
	if(req == NULL) {
		printf("[%s:%d] Error,req is NULL!", __FUNCTION__, __LINE__);
		return;
	}
	memset(req, 0, sizeof(struct log_upload_req));
	
	rtk_get_dut_info(&info);
    memcpy(req->deviceID, info.sn, sizeof(info.sn));
	
    if(stat(SYSLOG_FILE, &file_stat) != 0) {
		printf("Stat file %s failed :%s\n", SYSLOG_FILE, strerror(errno));
		return;
	}
	
	if(last_mtime != file_stat.st_mtime) {
		last_mtime = file_stat.st_mtime;
		
		if(file_stat.st_size > last_size) {
					
			fp = fopen(SYSLOG_FILE, "r");		
			if(fp == NULL) {
				printf("Fopen file %s failed :%s\n", SYSLOG_FILE, strerror(errno));
				return;
			}
			if( fseek(fp, last_size, SEEK_SET) != 0) {
				printf("Fseek %s failed :%s\n", SYSLOG_FILE, strerror(errno));
				fclose(fp);
				return;
			}	
			
			len = file_stat.st_size - last_size;
			req->log = (char *)calloc(sizeof(char), len+1);
			if(req->log == NULL) {
				printf("Calloc %d failed :%s\n", len, strerror(errno));
				fclose(fp);
				return;
			}
	
			if(fread(req->log, 1, len, fp) != len) {
				printf("Fread %s failed :%s\n", SYSLOG_FILE, strerror(errno));
				fclose(fp);
				return;		
			}
			*(req->log + len) = 0;
			last_size = file_stat.st_size;
			
			fclose(fp);
		} else {
			printf("The %s has been modified, but file size wasn't bigger!\n", SYSLOG_FILE);
		}
	}

	return;
}

int cmcc_GET_DEVICE_PASSWORD_RSP(struct device_password* rsp)
{
	unsigned char password[129] = {0};
	
	if(apmib_get(MIB_DEVICE_PASSWORD,(void*)password)==0){
		printf("get MIB_DEVICE_PASSWORD fail!%s %d\n",__FILE__,__LINE__);
		return RTK_FAILED;
	}

	snprintf(rsp->password,sizeof(rsp->password),"%s",password);
	return RTK_SUCCESS;
}

int cmcc_GET_DEVICE_INFO_RSP(struct device_info_rsp* rsp)
{
	unsigned char mac[6] = {0};
	struct rtk_dut_info info;
	memset(&info,0,sizeof(struct rtk_dut_info));

	if(!rtk_get_dut_info(&info))
        return RTK_FAILED;

    snprintf(rsp->deviceID, sizeof(rsp->deviceID),"%s",info.sn);
	
	rtk_get_default_mac_address(mac, "eth0");
    snprintf(rsp->MAC,sizeof(rsp->MAC), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	snprintf(rsp->name,sizeof(rsp->name),"CMCC-%02X%02X%02X",mac[3],mac[4],mac[5]);

	return RTK_SUCCESS;
}

int cmcc_WTP_ONLINE_NOTIFICATION_REQ(struct online_notification_rsp* req)
{
	unsigned char mac[6] = {0};
	struct rtk_dut_info info;
	RTK_LAN_INFO lan_info;
	memset(&info,0,sizeof(struct rtk_dut_info));

	if(!rtk_get_dut_info(&info))
        return RTK_FAILED;

    snprintf(req->deviceID, sizeof(req->deviceID),"%s",info.sn);
	
	rtk_get_default_mac_address(mac, "eth0");
    snprintf(req->MAC,sizeof(req->MAC), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	snprintf(req->name,sizeof(req->name),"CMCC_%s",req->MAC);

	if(!rtk_get_lan_info(&lan_info))
        return RTK_FAILED;
    snprintf(req->IP,sizeof(req->IP), inet_ntoa(lan_info.ip));

	return RTK_SUCCESS;
}

