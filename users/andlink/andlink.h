#ifndef __ANDLINK_H
#define __ANDLINK_H

#define QLINK_DHCP_IP       "192.168.188.1"
#define QLINK_DHCP_START    "192.168.188.100"
#define QLINK_DHCP_END      "192.168.188.200"

ERR_NUM_T Andlink_postGWSearchAck(cJSON *data);
ERR_NUM_T Andlink_postQlinkInfo(cJSON *data);
ERR_NUM_T Andlink_postWiFiParameterSync(cJSON *data);
ERR_NUM_T Andlink_postWiFiSwitch(cJSON *data);
ERR_NUM_T Andlink_postRadioConfig(cJSON *data);
ERR_NUM_T Andlink_postWPS(cJSON *data);
ERR_NUM_T Andlink_postMacFilter(cJSON *data);
ERR_NUM_T Andlink_postReboot(cJSON *data);
ERR_NUM_T Andlink_postSearchAP(cJSON *data);
ERR_NUM_T Andlink_postLEDControl(cJSON *data);
ERR_NUM_T Andlink_postUpgrade(cJSON *data);
ERR_NUM_T Andlink_postRoamingConfig(cJSON *data);
ERR_NUM_T Andlink_postUnbind(cJSON *data);
#ifdef CONFIG_ANDLINK_V3_SUPPORT
ERR_NUM_T Andlink_postAddGW(cJSON *data);
ERR_NUM_T Andlink_postAppSearchDev(cJSON *data);
#endif
ERR_NUM_T Andlink_postInformData(cJSON *data);
ERR_NUM_T Andlink_postRoamingReq(cJSON *data);
ERR_NUM_T Andlink_getApConfigInfo(cJSON *data);
ERR_NUM_T Andlink_getUplinkStatus(cJSON *data);
ERR_NUM_T Andlink_getSTAInfo(cJSON *data);
ERR_NUM_T Andlink_getWiFiStats(cJSON *data);
ERR_NUM_T Andlink_getRoamingConfig(cJSON *data);
#ifdef CONFIG_ANDLINK_V3_SUPPORT
ERR_NUM_T Andlink_getGWInfo(cJSON *data);
#endif

const Andlink_endpoint endpoints[] =
{
    {COAP_REQUEST_POST,  PATH_GWSEARCHACK,      Andlink_postGWSearchAck},
    {COAP_REQUEST_POST,  PATH_QLINKINFO,        Andlink_postQlinkInfo},
    {COAP_REQUEST_POST,  PATH_WIFISYNC,         Andlink_postWiFiParameterSync},
    {COAP_REQUEST_POST,  PATH_WIFISWITCH,       Andlink_postWiFiSwitch},
    {COAP_REQUEST_POST,  PATH_RADIOCONFIG,      Andlink_postRadioConfig},
    {COAP_REQUEST_POST,  PATH_WPS,              Andlink_postWPS},
    {COAP_REQUEST_POST,  PATH_MACFILTER,        Andlink_postMacFilter},
    {COAP_REQUEST_POST,  PATH_REBOOT,           Andlink_postReboot},
    {COAP_REQUEST_POST,  PATH_SEARCHAP,         Andlink_postSearchAP},
    {COAP_REQUEST_POST,  PATH_LED,              Andlink_postLEDControl},
    {COAP_REQUEST_POST,  PATH_UPGRADE,          Andlink_postUpgrade},
    {COAP_REQUEST_POST,  PATH_ROAMING,          Andlink_postRoamingConfig},
    {COAP_REQUEST_POST,  PATH_UNBIND,           Andlink_postUnbind},
#ifdef CONFIG_ANDLINK_V3_SUPPORT    
    {COAP_REQUEST_POST,  PATH_ADDGW,        	Andlink_postAddGW},
    {COAP_REQUEST_POST,  PATH_APPSEARCHDEV,     Andlink_postAppSearchDev},
#endif
    {COAP_REQUEST_POST,  PATH_INFORMDATA,       Andlink_postInformData},
    {COAP_REQUEST_POST,  PATH_ROAMING_REQ,      Andlink_postRoamingReq},
    {COAP_REQUEST_GET,   PATH_APCONFIG,         Andlink_getApConfigInfo},
    {COAP_REQUEST_GET,   PATH_UPLINK,           Andlink_getUplinkStatus},
    {COAP_REQUEST_GET,   PATH_STAINFO,          Andlink_getSTAInfo},
    {COAP_REQUEST_GET,   PATH_WIFISTATS,        Andlink_getWiFiStats},
    {COAP_REQUEST_GET,   PATH_ROAMING_QUERY,    Andlink_getRoamingConfig},
#ifdef CONFIG_ANDLINK_V3_SUPPORT
    {COAP_REQUEST_GET,   PATH_QLINKQUERYGW,    	Andlink_getGWInfo},
#endif
};

typedef enum _Capability {
    cESS 		= 0x01,
    cIBSS		= 0x02,
    cPollable		= 0x04,
    cPollReq		= 0x01,
    cPrivacy		= 0x10,
    cShortPreamble	= 0x20,
} Capability;

#endif
