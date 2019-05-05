#include <sys/stat.h>
#include <signal.h>
#include "prmt_landevice.h"
#include "prmt_landevice_eth.h"
//#include <config/autoconf.h> keith remove
#ifdef WLAN_SUPPORT  
#include "prmt_landevice_wlan.h"
#endif
#if defined(_PRMT_USB_ETH_) || defined(_PRMT_USB_)
#include "prmt_landevice_usb.h"
#else
#define CWMP_LANUSBIFNO		0
#endif

//#define _CHECK_DHCP_PREFIX_
#ifdef _CHECK_DHCP_PREFIX_
#define CheckDhcpPrefix(ip1,ip2,ip3,ip4) \
		do{\
			char lan_ip[32];\
			int  lanip1,lanip2, lanip3;\
			/*getMIB2Str(MIB_ADSL_LAN_DHCP_GATEWAY, lan_ip);*/\
			/*sscanf( lan_ip, "%d.%d.%d.%*d", &lanip1, &lanip2, &lanip3 );*/\
			getSYS2Str(SYS_DHCPS_IPPOOL_PREFIX, lan_ip);\
			sscanf(lan_ip, "%d.%d.%d.", &lanip1, &lanip2, &lanip3 );\
			if( (lanip1!=ip1) || (lanip2!=ip2) || (lanip3!=ip3)  )\
				return ERR_9007;\
		}while(0)

#else
#define CheckDhcpPrefix(ip1,ip2,ip3,ip4) do{}while(0)
#endif //_CHECK_DHCP_PREFIX_

#define GetDhcpPrefix(ipadd1,ipadd2,ipadd3,ipadd4) \
		do{\
			/*getMIB2Str(MIB_ADSL_LAN_DHCP_GATEWAY, buf);*/\
			/*sscanf( buf,"%d.%d.%d.%d", &ipadd1, &ipadd2, &ipadd3, &ipadd4 );*/\
			getSYS2Str(SYS_DHCPS_IPPOOL_PREFIX, buf);\
			sscanf( buf,"%d.%d.%d.", &ipadd1, &ipadd2, &ipadd3 );\
		}while(0);

#ifdef _CWMP_MAC_FILTER_
int getMACAddressList( char *buf, int len );
int setMACAddressList( char *buf );
#endif /*_CWMP_MAC_FILTER_*/

#ifdef SECONDARY_IP
unsigned int getIPItfInstNum( char *name );
#endif //SECONDARY_IP

/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#ifdef _PRMT_WT107_
struct CWMP_OP tLANInterfacesLeafOP = { getLANInterfaces, NULL };
struct CWMP_PRMT tLANInterfacesLeafInfo[] = 
{
/*(name,				type,		flag,			op)*/
{"LANEthernetInterfaceNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANInterfacesLeafOP},
{"LANUSBInterfaceNumberOfEntries",		eCWMP_tUINT,	CWMP_READ,		&tLANInterfacesLeafOP},
{"LANWLANConfigurationNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANInterfacesLeafOP}
};
enum eLANInterfacesLeaf
{
	eLANItfEthernetInterfaceNumberOfEntries,
	eLANItfUSBInterfaceNumberOfEntries,
	eLANItfWLANConfigurationNumberOfEntries
};
struct CWMP_LEAF tLANInterfacesLeaf[] =
{
{ &tLANInterfacesLeafInfo[eLANItfEthernetInterfaceNumberOfEntries] },
{ &tLANInterfacesLeafInfo[eLANItfUSBInterfaceNumberOfEntries] },
{ &tLANInterfacesLeafInfo[eLANItfWLANConfigurationNumberOfEntries] },
{ NULL }
};
#endif
/*ping_zhang:20081217 END*/

struct CWMP_OP tHostEntityLeafOP = { getHostEntity, NULL };
struct CWMP_PRMT tHostEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"IPAddress",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"AddressSource",		eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"LeaseTimeRemaining",		eCWMP_tINT,	CWMP_READ|CWMP_DENY_ACT,&tHostEntityLeafOP},
{"MACAddress",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{"Layer2Interface",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
#endif
/*ping_zhang:20081217 END*/
{"VendorClassID",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"ClientID",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"UserClassID",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"HostName",			eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"InterfaceType",		eCWMP_tSTRING,	CWMP_READ,		&tHostEntityLeafOP},
{"Active",			eCWMP_tBOOLEAN,	CWMP_READ,		&tHostEntityLeafOP}
};
enum eHostEntityLeaf
{
	eHost_IPAddress,
	eHost_AddressSource,	
	eHost_LeaseTimeRemaining,
	eHost_MACAddress,
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
	eHost_Layer2Interface,
#endif
/*ping_zhang:20081217 END*/	
	eHost_VendorClassID,
	eHost_ClientID,
	eHost_UserClassID,
	eHost_HostName,
	eHost_InterfaceType,
	eHost_Active
};
struct CWMP_LEAF tHostEntityLeaf[] =
{
{ &tHostEntityLeafInfo[eHost_IPAddress] },
{ &tHostEntityLeafInfo[eHost_AddressSource] },
{ &tHostEntityLeafInfo[eHost_LeaseTimeRemaining] },
{ &tHostEntityLeafInfo[eHost_MACAddress] },
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{ &tHostEntityLeafInfo[eHost_Layer2Interface] },
#endif
/*ping_zhang:20081217 END*/
{ &tHostEntityLeafInfo[eHost_VendorClassID] },
{ &tHostEntityLeafInfo[eHost_ClientID] },
{ &tHostEntityLeafInfo[eHost_UserClassID] },
{ &tHostEntityLeafInfo[eHost_HostName] },
{ &tHostEntityLeafInfo[eHost_InterfaceType] },
{ &tHostEntityLeafInfo[eHost_Active] },
{ NULL }
};


struct CWMP_PRMT tHostObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_LNKLIST,	NULL}
};
enum eHostObject
{
	eHost0
};
struct CWMP_LINKNODE tHostObject[] =
{
/*info,  			leaf,			next,		sibling,		instnum)*/
{&tHostObjectInfo[eHost0],	tHostEntityLeaf,	NULL,		NULL,			0},
};


struct CWMP_OP tHostsLeafOP =  { getHosts, NULL};
struct CWMP_PRMT tHostsLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"HostNumberOfEntries",		eCWMP_tUINT,	CWMP_READ,		&tHostsLeafOP}
};
enum eHostsLeaf
{
	eHostsNumberOfEntries
};
struct CWMP_LEAF tHostsLeaf[] =
{
{ &tHostsLeafInfo[eHostsNumberOfEntries] },
{ NULL }
};

struct CWMP_OP tHosts_Host_OP = { NULL, objHosts };
struct CWMP_PRMT tHostsObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Host",			eCWMP_tOBJECT,	CWMP_READ,		&tHosts_Host_OP}
};
enum eHostsObject
{
	eHostsHost
};
struct CWMP_NODE tHostsObject[] =
{
/*info,  			leaf,			node)*/
{&tHostsObjectInfo[eHostsHost],	NULL,			NULL},
{NULL,				NULL,			NULL}
};

struct CWMP_OP tIPInterfaceEntityLeafOP = { getIPItfEntity, setIPItfEntity };
struct CWMP_PRMT tIPInterfaceEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tIPInterfaceEntityLeafOP},
{"IPInterfaceIPAddress",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tIPInterfaceEntityLeafOP},
{"IPInterfaceSubnetMask",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tIPInterfaceEntityLeafOP},
{"IPInterfaceAddressingType",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tIPInterfaceEntityLeafOP}
};
enum eIPInterfaceEntityLeaf
{
	eIPIF_Enable,
	eIPIF_IPInterfaceIPAddress,
	eIPIF_IPInterfaceSubnetMask,
	eIPIF_IPInterfaceAddressingType
};
struct CWMP_LEAF tIPInterfaceEntityLeaf[] =
{
{ &tIPInterfaceEntityLeafInfo[eIPIF_Enable] },
{ &tIPInterfaceEntityLeafInfo[eIPIF_IPInterfaceIPAddress] },
{ &tIPInterfaceEntityLeafInfo[eIPIF_IPInterfaceSubnetMask] },
{ &tIPInterfaceEntityLeafInfo[eIPIF_IPInterfaceAddressingType] },
{ NULL }
};
struct CWMP_PRMT tIPInterfaceObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"1",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
#ifdef SECONDARY_IP
{"2",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif //SECONDARY_IP

};
enum eIPInterfaceObject
{
	eIPInterface1,
#ifdef SECONDARY_IP
	eIPInterface2,
#endif //SECONDARY_IP
};


struct CWMP_NODE tIPInterfaceObject[] =
{
/*info,  					leaf,				node)*/
{ &tIPInterfaceObjectInfo[eIPInterface1],	tIPInterfaceEntityLeaf,		NULL},
#ifdef SECONDARY_IP
{ &tIPInterfaceObjectInfo[eIPInterface2],	tIPInterfaceEntityLeaf,		NULL},
#endif //SECONDARY_IP
{ NULL,						NULL,				NULL}
};

/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPStaticAddress.{i}.*/
/*InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPStaticAddress.{i}.*/
struct CWMP_OP tDHCPStaticAddrENTITYLeafOP = { getDHCPStaticAddrEntity, setDHCPStaticAddrEntity };
struct CWMP_PRMT tDHCPStaticAddrENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPStaticAddrENTITYLeafOP},
{"Chaddr",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPStaticAddrENTITYLeafOP},
{"Yiaddr",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPStaticAddrENTITYLeafOP},
};
enum eDHCPStaticAddrDHCPOptionEntityLeaf
{
	eDHCPStaticAddrEnable,
	eDHCPStaticAddrChaddr,
	eDHCPStaticAddrYiaddr,
};
struct CWMP_LEAF tDHCPStaticAddrENTITYLeaf[] =
{
{ &tDHCPStaticAddrENTITYLeafInfo[eDHCPStaticAddrEnable] },
{ &tDHCPStaticAddrENTITYLeafInfo[eDHCPStaticAddrChaddr] },
{ &tDHCPStaticAddrENTITYLeafInfo[eDHCPStaticAddrYiaddr] },
{ NULL }
};

struct CWMP_PRMT tDHCPStaticAddrObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"0",			eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,		NULL}
};
enum eDHCPStaticAddrObject
{
	eDHCPStaticAddr0
};
struct CWMP_LINKNODE tDHCPStaticAddrObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tDHCPStaticAddrObjectInfo[eDHCPStaticAddr0],	tDHCPStaticAddrENTITYLeaf,	NULL,		NULL,			0}
};


/*InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.*/
/*InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.*/
struct CWMP_OP tDHCPOptionEntityLeafOP = { getDHCPOptionEntity, setDHCPOptionEntity };
struct CWMP_PRMT tDHCPOptionLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tDHCPOptionEntityLeafOP},
{"Tag",			eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tDHCPOptionEntityLeafOP},
{"Value",			eCWMP_tBASE64,	CWMP_WRITE|CWMP_READ,	&tDHCPOptionEntityLeafOP}
};
enum eDHCPOptionEntityLeaf
{
	eDHCPOptionEnable,
	eDHCPOptionTag,
	eDHCPOptionValue
};
struct CWMP_LEAF tDHCPOptionENTITYLeaf[] =
{
{ &tDHCPOptionLeafInfo[eDHCPOptionEnable] },
{ &tDHCPOptionLeafInfo[eDHCPOptionTag] },
{ &tDHCPOptionLeafInfo[eDHCPOptionValue] },
{ NULL }
};

struct CWMP_PRMT tDHCPOptionObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"0",			eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,		NULL}
};
enum eDHCPOptionObject
{
	eDHCPOption0
};
struct CWMP_LINKNODE tDHCPOptionObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tDHCPOptionObjectInfo[eDHCPOption0],	tDHCPOptionENTITYLeaf,	NULL,		NULL,			0}
};

/*InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.*/
struct CWMP_OP tDHCPConSPENTITYLeafOP = { getDHCPConSPEntity, setDHCPConSPEntity };
struct CWMP_PRMT tDHCPConSPENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"PoolOrder",		eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"SourceInterface",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"VendorClassID",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"VendorClassIDExclude",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"VendorClassIDMode",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"ClientID",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"ClientIDExclude",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"UserClassID",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"UserClassIDExclude",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"Chaddr",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"ChaddrMask",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"ChaddrExclude",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"LocallyServed",		eCWMP_tBOOLEAN,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"MinAddress",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"MaxAddress",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"ReservedAddresses",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"SubnetMask",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"DNSServers",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"DomainName",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"IPRouters",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"DHCPLeaseTime",		eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"UseAllocatedWAN",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"AssociatedConnection",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"DHCPServerIPAddress",		eCWMP_tSTRING,		CWMP_READ|CWMP_WRITE,	&tDHCPConSPENTITYLeafOP},
{"DHCPStaticAddressNumberOfEntries",		eCWMP_tUINT,		CWMP_READ,	&tDHCPConSPENTITYLeafOP},
{"DHCPOptionNumberOfEntries",		eCWMP_tUINT,		CWMP_READ,	&tDHCPConSPENTITYLeafOP},
};
enum eDHCPConSPDHCPOptionEntityLeaf
{
	eDHCPConSPEnable,
	eDHCPConSPPoolOrder,
	eDHCPConSPSourceInterface,
	eDHCPConSPVendorClassID,
	eDHCPConSPVendorClassIDExclude,
	eDHCPConSPVendorClassIDMode,
	eDHCPConSPClientID,
	eDHCPConSPClientIDExclude,
	eDHCPConSPUserClassID,
	eDHCPConSPUserClassIDExclude,
	eDHCPConSPChaddr,
	eDHCPConSPChaddrMask,
	eDHCPConSPChaddrExclude,
	eDHCPConSPLocallyServed,
	eDHCPConSPMinAddress,
	eDHCPConSPMaxAddress,
	eDHCPConSPReservedAddresses,
	eDHCPConSPSubnetMask,
	eDHCPConSPDNSServers,
	eDHCPConSPDomainName,
	eDHCPConSPIPRouters,
	eDHCPConSPDHCPLeaseTime,
	eDHCPConSPUseAllocatedWAN,
	eDHCPConSPAssociatedConnection,
	eDHCPConSPDHCPServerIPAddress,
	eDHCPConSPDHCPStaticAddressNumberOfEntries,
	eDHCPConSPDHCPOptionNumberOfEntries,
};
struct CWMP_LEAF tDHCPConSPENTITYLeaf[] =
{
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPEnable] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPPoolOrder] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPSourceInterface] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPVendorClassID] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPVendorClassIDExclude] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPVendorClassIDMode] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPClientID] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPClientIDExclude] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPUserClassID] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPUserClassIDExclude] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPChaddr] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPChaddrMask] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPChaddrExclude] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPLocallyServed] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPMinAddress] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPMaxAddress] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPReservedAddresses] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPSubnetMask] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPDNSServers] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPDomainName] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPIPRouters] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPDHCPLeaseTime] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPUseAllocatedWAN] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPAssociatedConnection] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPDHCPServerIPAddress] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPDHCPStaticAddressNumberOfEntries] },
{ &tDHCPConSPENTITYLeafInfo[eDHCPConSPDHCPOptionNumberOfEntries] },
{ NULL }
};


struct CWMP_OP tDHCPConSPDHCPOption_OP = { NULL, objDHCPOption};
struct CWMP_PRMT tDHCPConSPENTITYObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"DHCPOption",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPConSPDHCPOption_OP},
};
enum eDHCPConSPENTITYObject
{
	etDHCPConSPE_DHCPOption
};

struct CWMP_OP tDHCPConSPStaticAddr_OP = { NULL, objDHCPStaticAddr};
struct CWMP_PRMT tDHCPConSPEStaticAddObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"DHCPStaticAddress",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPConSPStaticAddr_OP}
};
enum tDHCPConSPEStaticAddObject
{
	etDHCPConSPE_StaticAddr
};

struct CWMP_NODE tDHCPConSPENTITYObject[] =
{
/*info,  							leaf,			node)*/
{&tDHCPConSPENTITYObjectInfo[etDHCPConSPE_DHCPOption],	NULL,	NULL},
{&tDHCPConSPEStaticAddObjectInfo[etDHCPConSPE_StaticAddr],	NULL,	NULL},
{NULL,								NULL,			NULL}
};

struct CWMP_PRMT tDHCPConSPObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"0",			eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,		NULL}
};
enum eDHCPConSPObject
{
	eDHCPConSP0
};
struct CWMP_LINKNODE tDHCPConSPObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tDHCPConSPObjectInfo[eDHCPConSP0],	tDHCPConSPENTITYLeaf,	tDHCPConSPENTITYObject,		NULL,			0}
};

#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/

struct CWMP_OP tLANHostConfLeafOP = { getLANHostConf, setLANHostConf };
struct CWMP_PRMT tLANHostConfLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{"MACAddress",			eCWMP_tSTRING,	CWMP_READ,		&tLANHostConfLeafOP},
#endif
/*ping_zhang:20081217 END*/
{"DHCPServerConfigurable",	eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"DHCPServerEnable",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"DHCPRelay",			eCWMP_tBOOLEAN,	CWMP_READ,		&tLANHostConfLeafOP},
{"MinAddress",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"MaxAddress",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"ReservedAddresses",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"SubnetMask",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"DNSServers",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"DomainName",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"IPRouters",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"DHCPLeaseTime",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"UseAllocatedWAN",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"AssociatedConnection",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"PassthroughLease",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
{"PassthroughMACAddress",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
#ifdef _CWMP_MAC_FILTER_
{"AllowedMACAddresses",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANHostConfLeafOP},
#endif /*_CWMP_MAC_FILTER_*/
{"IPInterfaceNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANHostConfLeafOP},
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
{"DHCPStaticAddressNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANHostConfLeafOP},
{"DHCPOptionNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANHostConfLeafOP},
{"DHCPConditionalPoolNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANHostConfLeafOP},
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
};
enum eLANHostConfLeaf
{
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
	eMACAddress,
#endif
/*ping_zhang:20081217 END*/
	eDHCPServerConfigurable,
	eDHCPServerEnable,
	eDHCPRelay,
	eMinAddress,
	eMaxAddress,
	eReservedAddresses,
	eSubnetMask,
	eDNSServers,
	eDomainName,
	eIPRouters,
	eDHCPLeaseTime,
	eUseAllocatedWAN,
	eAssociatedConnection,
	ePassthroughLease,
	ePassthroughMACAddress,
#ifdef _CWMP_MAC_FILTER_
	eAllowedMACAddresses,
#endif /*_CWMP_MAC_FILTER_*/
	eIPInterfaceNumberOfEntries,
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	eDHCPStaticAddressNumberOfEntries,
	eDHCPOptionNumberOfEntries,
	eDHCPConditionalPoolNumberOfEntries
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
};
struct CWMP_LEAF tLANHostConfLeaf[] =
{
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{ &tLANHostConfLeafInfo[eMACAddress] },
#endif
/*ping_zhang:20081217 END*/
{ &tLANHostConfLeafInfo[eDHCPServerConfigurable] },
{ &tLANHostConfLeafInfo[eDHCPServerEnable] },
{ &tLANHostConfLeafInfo[eDHCPRelay] },
{ &tLANHostConfLeafInfo[eMinAddress] },
{ &tLANHostConfLeafInfo[eMaxAddress] },
{ &tLANHostConfLeafInfo[eReservedAddresses] },
{ &tLANHostConfLeafInfo[eSubnetMask] },
{ &tLANHostConfLeafInfo[eDNSServers] },
{ &tLANHostConfLeafInfo[eDomainName] },
{ &tLANHostConfLeafInfo[eIPRouters] },
{ &tLANHostConfLeafInfo[eDHCPLeaseTime] },
{ &tLANHostConfLeafInfo[eUseAllocatedWAN] },
{ &tLANHostConfLeafInfo[eAssociatedConnection] },
{ &tLANHostConfLeafInfo[ePassthroughLease] },
{ &tLANHostConfLeafInfo[ePassthroughMACAddress] },
#ifdef _CWMP_MAC_FILTER_
{ &tLANHostConfLeafInfo[eAllowedMACAddresses] },
#endif /*_CWMP_MAC_FILTER_*/
{ &tLANHostConfLeafInfo[eIPInterfaceNumberOfEntries] },
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
{ &tLANHostConfLeafInfo[eDHCPStaticAddressNumberOfEntries] },
{ &tLANHostConfLeafInfo[eDHCPOptionNumberOfEntries] },
{ &tLANHostConfLeafInfo[eDHCPConditionalPoolNumberOfEntries] },
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
{ NULL }
};

/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
struct CWMP_OP tDHCPStaticAddr_OP = { NULL, objDHCPStaticAddr};
struct CWMP_OP tDHCPOption_OP = { NULL, objDHCPOption};
struct CWMP_OP tDHCPConSP_OP = { NULL, objDHCPConSP};
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
struct CWMP_PRMT tLANHostConfObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"IPInterface",			eCWMP_tOBJECT,	CWMP_READ,		NULL},
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
{"DHCPStaticAddress",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPStaticAddr_OP},
{"DHCPOption",					eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPOption_OP},
{"DHCPConditionalServingPool",	eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPConSP_OP},
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
};
enum eLANHostConfObject
{
	eIPInterface,
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	eDHCPStaticAddress,
	eDHCPOption,
	eDHCPConditionServingPool
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
};
struct CWMP_NODE tLANHostConfObject[] =
{
/*info,  					leaf,		node)*/
{ &tLANHostConfObjectInfo[eIPInterface],	NULL,		tIPInterfaceObject},
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
{ &tLANHostConfObjectInfo[eDHCPStaticAddress],	NULL,		NULL},
{ &tLANHostConfObjectInfo[eDHCPOption],	NULL,		NULL},
{ &tLANHostConfObjectInfo[eDHCPConditionServingPool],	NULL,		NULL},
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
{ NULL,						NULL,		NULL}
};

#ifdef WLAN_SUPPORT// #if 1//def CTCOM_WLAN_REQ
struct CWMP_OP tWLANConfiguration_OP = { NULL, objWLANConfiguration };
#endif

struct CWMP_OP tLANDeviceEntityLeafOP = { getLDEntity,	NULL };
struct CWMP_PRMT tLANDeviceEntityLeafInfo[] =
{
/*(name,				type,		flag,			op)*/
{"LANEthernetInterfaceNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANDeviceEntityLeafOP},
{"LANUSBInterfaceNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANDeviceEntityLeafOP},
{"LANWLANConfigurationNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLANDeviceEntityLeafOP}
};
enum eLANDeviceEntityLeaf
{
	eLANEthernetInterfaceNumberOfEntries,
	eLANUSBInterfaceNumberOfEntries,
	eLANWLANConfigurationNumberOfEntries
};
struct CWMP_LEAF tLANDeviceEntityLeaf[] =
{
{ &tLANDeviceEntityLeafInfo[eLANEthernetInterfaceNumberOfEntries] },
{ &tLANDeviceEntityLeafInfo[eLANUSBInterfaceNumberOfEntries] },
{ &tLANDeviceEntityLeafInfo[eLANWLANConfigurationNumberOfEntries] },
{ NULL }
};

struct CWMP_PRMT tLANDeviceEntityObjectInfo[] =
{
/*(name,				type,		flag,			op)*/
{"LANHostConfigManagement",		eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"LANEthernetInterfaceConfig",		eCWMP_tOBJECT,	CWMP_READ,		NULL},
#if defined(_PRMT_USB_ETH_) || defined(_PRMT_USB_)
{"LANUSBInterfaceConfig",			eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
#ifdef WLAN_SUPPORT
#if 1//def CTCOM_WLAN_REQ
{"WLANConfiguration",			eCWMP_tOBJECT,		CWMP_WRITE|CWMP_READ,	&tWLANConfiguration_OP},
#else
{"WLANConfiguration",			eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
#endif
{"Hosts",				eCWMP_tOBJECT,	CWMP_READ,		NULL}
};
enum eLANDeviceEntityObject
{
	eLANHostConfigManagement,
	eLANEthernetInterfaceConfig,
#if defined(_PRMT_USB_ETH_) || defined(_PRMT_USB_)
	eLANUSBInterfaceConfig,
#endif
#ifdef WLAN_SUPPORT
	eWLANConfiguration,
#endif
	eLANHosts
};
struct CWMP_NODE tLANDeviceEntityObject[] =
{
/*info,  							leaf,			node)*/
{ &tLANDeviceEntityObjectInfo[eLANHostConfigManagement],	tLANHostConfLeaf,	tLANHostConfObject},
{ &tLANDeviceEntityObjectInfo[eLANEthernetInterfaceConfig],	NULL,			tLANEthConfObject},
#if defined(_PRMT_USB_ETH_) || defined(_PRMT_USB_)
{ &tLANDeviceEntityObjectInfo[eLANUSBInterfaceConfig],	NULL,			tLANUSBConfObject},
#endif
#ifdef WLAN_SUPPORT
#if 1//def CTCOM_WLAN_REQ
{ &tLANDeviceEntityObjectInfo[eWLANConfiguration], 		NULL,			NULL},
#else
{ &tLANDeviceEntityObjectInfo[eWLANConfiguration], 		NULL,			tWLANConfigObject},
#endif
#endif
{ &tLANDeviceEntityObjectInfo[eLANHosts],			tHostsLeaf,		tHostsObject},
{ NULL,								NULL,			NULL}
};

/*
 * Here hard-code LANDevice.{i} to LANDevice.1
 * According to TR-098 spec: this data model, each instance models a LAN side layer 3 IP interface.
 */
struct CWMP_PRMT tLANDeviceObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"1",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
};
enum eLANDeviceObject
{
	eLANDev1
};
struct CWMP_NODE tLANDeviceObject[] =
{
/*info,  				leaf,			node)*/
{&tLANDeviceObjectInfo[eLANDev1],	tLANDeviceEntityLeaf,	tLANDeviceEntityObject},
{NULL,					NULL,			NULL}
};


/*****************************Hosts. begin*********************************/
#define _DHCPD_PID_FILE			"/var/run/udhcpd.pid"
#define _DHCPD_LEASES_FILE		"/var/lib/misc/udhcpd.leases"
#define DHCPD_HOST_OPTION_FILE	"/var/lib/misc/dhcp_host_options"	

#define MAX_STA_NUM			64	// max support sta number

char* gDHCPHosts = NULL;
unsigned int	gDHCPHostsNum = 0;
struct CWMP_LINKNODE **gDHCPTable = NULL;

struct dhcpOfferedAddr {
	u_int8_t chaddr[16];
	u_int32_t yiaddr;		/* network order */
	u_int32_t expires;		/* host order */
};

int updateDHCPList()
{
	FILE 	*fp = NULL;
	struct stat status;
	
	// siganl DHCP server to update lease file
	int pid = getPid(_DHCPD_PID_FILE);
	if ( pid > 0)
		kill(pid, SIGUSR1);
	usleep(1000);

	if ( stat(_DHCPD_LEASES_FILE, &status) < 0 )
		goto err;
	
	if(gDHCPHosts) free(gDHCPHosts);
	gDHCPHosts = malloc(status.st_size);
	if(gDHCPHosts == NULL) goto err;
	
	fp = fopen(_DHCPD_LEASES_FILE, "r");
	if ( fp == NULL ) goto err;
	fread(gDHCPHosts, 1, status.st_size, fp);
	fclose(fp);
	
	gDHCPHostsNum = status.st_size / sizeof( struct dhcpOfferedAddr );		
	return 0;

err:
	if(gDHCPHosts)
	{
		free(gDHCPHosts);
		gDHCPHosts = NULL;
	}
	gDHCPHostsNum = 0;
	if(fp != NULL){
		fclose(fp);
		fp = NULL;
	}
	return -1;
}

int getDHCPClient( int id,  char *ip, char *mac, unsigned int *liveTime )
{
	struct dhcpOfferedAddr *p = NULL;
	//id starts from 0
	if( (id<0) || (id>=gDHCPHostsNum) ) return -1;
	if( (ip==NULL) || (mac==NULL) || (liveTime)==0 ) return -1;
	if( (gDHCPHosts==NULL) || (gDHCPHostsNum==0) ) return -1;
	
	p = (struct dhcpOfferedAddr *) (gDHCPHosts + id * sizeof( struct dhcpOfferedAddr ));
	*liveTime = 0;
	strcpy(ip, inet_ntoa(*((struct in_addr *)&p->yiaddr)) );
	sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
			p->chaddr[0],p->chaddr[1],p->chaddr[2],
			p->chaddr[3],p->chaddr[4],p->chaddr[5]);
	if(p->expires != 0xffffffff)
		*liveTime = ntohl(p->expires);
	return 0;
}

int findDHCPStaticAssign( char *mac, char *ip )
{
	int i, total = 0;
	int ret = 0;
	DHCPRSVDIP_T Entry;
	char m[6];
	
	if( mac==NULL || ip==NULL ) return ret;
	if( sscanf( mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
		&m[0],&m[1],&m[2],&m[3],&m[4],&m[5] )!=6 )
		return ret;
	
	mib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&total);
	for( i=1;i<=total;i++ )
	{
		*((char *)&Entry) = (char)i;
		if(!mib_get(MIB_DHCPRSVDIP_TBL, (void *)&Entry))
			continue;
		
		if( memcmp(Entry.macAddr, m, 6 )==0  && strcmp( inet_ntoa(*((struct in_addr *)Entry.ipAddr)), ip)==0 )
		{
			ret=1;
			break;
		}
	}
	
	return ret;	
}

int isWirelessSta(int id){
	int w_idx, vw_idx;
	char wlan_ifname[10] = {0};
	char wlanStas[ sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1) ];
	int  has_sta_flag;
	struct dhcpOfferedAddr *p = (struct dhcpOfferedAddr *) (gDHCPHosts + id * sizeof( struct dhcpOfferedAddr ));

	for(w_idx=0; w_idx<NUM_WLAN_INTERFACE; w_idx++){
		for(vw_idx=0; vw_idx<NUM_VWLAN_INTERFACE; vw_idx++)
		{
			memset(wlan_ifname, 0x00, sizeof(wlan_ifname));
			if(vw_idx == 0)
				sprintf(wlan_ifname, "wlan%d", w_idx);
			else
				sprintf(wlan_ifname, "wlan%d-va%d", w_idx, vw_idx-1);	

			has_sta_flag = 1;
			memset( wlanStas, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1) );	
			if ( getWlStaInfo( wlan_ifname,  (WLAN_STA_INFO_T *)wlanStas ) < 0 ){		
				memset( wlanStas, 0, sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1) );
				has_sta_flag=0;
			}
			if(has_sta_flag){
				WLAN_STA_INFO_T* pInfo;
				int i;
				for (i=1; i<=MAX_STA_NUM; i++)
				{
					pInfo = (WLAN_STA_INFO_T*)&wlanStas[i*sizeof(WLAN_STA_INFO_T)];
					if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC)){				
						if(memcmp(p->chaddr, pInfo->addr, 6) == 0)
							return vw_idx+1+ (NUM_VWLAN_INTERFACE)*w_idx;
					}
				}
			}
		}

	}
	return -1;
}

int parseDhcpClientOptionValue(char* buf, char *mac_addr, char * tag, char *value){
	if(!buf || !mac_addr || !tag || !value)	return -1;
	char *p;
	int index, len;
	p = strstr(buf, tag);
	if(p == NULL || strstr(buf, mac_addr) == NULL )	return -1;
	if(strstr(p, "#") == NULL  || strstr(p , ":") == NULL || strstr(p, "#") == NULL)
		return -1;
	//5404a6a9e3df:60#MSFT 5.0:61#015404a6a9e3df:77#none:12#realsil-PC:
	index = strstr(p, "#")- strstr(buf, mac_addr);
	len = strstr(p , ":")- strstr(p, "#");
	memcpy(value, buf+index+1, len-1);
	return 1;		
}

int getDhcpClientOptionsValue(int id, char *vendorClass, char *clientClass, char *userClass, char *hostName){
	int found = -1;
	char *lineBuf;
	char mac[20] = {0};
	FILE *fp;
	struct dhcpOfferedAddr *p = NULL;
	
	if(!vendorClass || !clientClass || !userClass || !hostName)	return -1;	
	//id starts from 0
	if( (id<0) || (id>=gDHCPHostsNum) ) return -1;
	if(gDHCPHostsNum == 0) return -1;

	if( (fp = fopen(DHCPD_HOST_OPTION_FILE, "r")) == NULL )	return -1;
		
	lineBuf = (char*)malloc(1024);
	if( lineBuf == NULL ){
		fclose(fp);
		fp = NULL;
		printf("[%s:%d] malloc fail!\n",__FILE__,__LINE__);
		return -1;	
	}
	bzero(lineBuf,1024);	
	
	p = (struct dhcpOfferedAddr *) (gDHCPHosts + id * sizeof( struct dhcpOfferedAddr ));

	snprintf(mac, 20, "%02x%02x%02x%02x%02x%02x",
		p->chaddr[0], p->chaddr[1], p->chaddr[2],
		p->chaddr[3], p->chaddr[4], p->chaddr[5]);

	while(fgets(lineBuf, 1024, fp))
	{
		//5404a6a9e3df:60#MSFT 5.0:61#015404a6a9e3df:77#none:12#realsil-PC:
		if(strstr(lineBuf, mac)){
			found = 1;
			parseDhcpClientOptionValue(lineBuf, mac, "60#", vendorClass);
			parseDhcpClientOptionValue(lineBuf, mac, "61#", clientClass);
			parseDhcpClientOptionValue(lineBuf, mac, "77#", userClass);
			parseDhcpClientOptionValue(lineBuf, mac, "12#", hostName);
			break;
		}
		bzero(lineBuf, 1024);
	}
	
	if(fp){
		fclose(fp);
		fp = NULL;
	}
	
	if(lineBuf){
		free(lineBuf);
		lineBuf = NULL;
	}
		
	return found;
}


int getHostEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char	*tok=NULL;
	char	ipAddr[40], macAddr[40], leaseTime[40];
	unsigned int livetime = 0;
	int	id;

	int wlanStaNum, isActive;
	char vendorClass[256] = {0};
	char clientClass[256] = {0};
	char userClass[256] = {0};
	char hostName[64] = {0};
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	updateDHCPList();
	//the name should be like "xxxxx.Hosts.Host.{i}.XXXXX"
	tok = strstr( name, "Host." );
	if( tok == NULL ) return ERR_9005;
	sscanf( tok, "Host.%d.%*s", &id );
	
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d>(id = %d)\n", __FUNCTION__, __LINE__, id ) );

	if( (id<1) || (id>gDHCPHostsNum) ) return ERR_9005;

	getDHCPClient( id-1, ipAddr, macAddr, &livetime );

	wlanStaNum = isWirelessSta(id-1);

	snprintf(vendorClass, sizeof(vendorClass), "%s", "none");
	snprintf(clientClass, sizeof(clientClass),"%s", "none");
	snprintf(userClass, sizeof(userClass), "%s", "none");
	snprintf(hostName, sizeof(hostName), "%s", "none");

	isActive = getDhcpClientOptionsValue( id-1, vendorClass, clientClass, userClass, hostName);
	
	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "IPAddress" )==0 )
	{
		*data = strdup( ipAddr );
	}else if( strcmp( lastname, "AddressSource" )==0 )
	{
		if( findDHCPStaticAssign( macAddr, ipAddr )>0 )
			*data = strdup( "Static" );
		else
			*data = strdup( "DHCP" );
	}else if( strcmp( lastname, "LeaseTimeRemaining" )==0 )
	{
		//the value must be 0 if the AddressSource is not DHCP
		if( findDHCPStaticAssign( macAddr, ipAddr )>0 )
			*data = intdup( 0 );
		else
			*data = uintdup( livetime );
	}else if( strcmp( lastname, "MACAddress" )==0 )
	{
		*data = strdup( macAddr );
	}else if( strcmp( lastname, "Layer2Interface" )==0 )
	{
		/*currently we don't known which ethernet interface the host connect with*/	
		if(isActive < 0)
			*data = strdup("");
		else if(wlanStaNum < 0)
			*data = strdup( "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig." );
		else{
			char tmpBuf[256] = {0};
			snprintf(tmpBuf, sizeof(tmpBuf), "%s.%d.","InternetGatewayDevice.LANDevice.1.WLANConfiguration", wlanStaNum);
			*data = strdup(tmpBuf);
		}
	}else if( strcmp( lastname, "VendorClassID" )==0 )
	{
		if(strcmp( vendorClass, "none") == 0)
			*data = strdup( "" );
		else
			*data = strdup( vendorClass );
	}else if( strcmp( lastname, "ClientID" )==0 )
	{
		if(strcmp( clientClass, "none") == 0)
			*data = strdup( "" );
		else{		
			*data = strdup(clientClass);
		}
	}else if( strcmp( lastname, "UserClassID" )==0 )
	{
		if(strcmp( userClass, "none") == 0)
			*data = strdup( "" );
		else
			*data = strdup( userClass );
	}else if( strcmp( lastname, "HostName" )==0 )
	{
		if(strcmp( hostName, "none") == 0)
			*data = strdup( "" );
		else
			*data = strdup( hostName );
	}else if( strcmp( lastname, "InterfaceType" )==0 )
	{
		if(isActive < 0)
			*data = strdup("");
		else if(wlanStaNum < 0)
			*data = strdup( "Ethernet" ); //or Ethernet
		else
			*data = strdup( "802.11" ); //or 802.11	
	}else if( strcmp( lastname, "Active" )==0 )
	{
		if(isActive < 0)
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
	}else{
		return ERR_9005;
	}

	if(gDHCPHosts){
		free(gDHCPHosts);
		gDHCPHosts = NULL;
	}
	return 0;
}

int getHosts(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "HostNumberOfEntries" )==0 )
	{
		updateDHCPList();
		*data = uintdup( gDHCPHostsNum );
	}else{
		return ERR_9005;
	}

	if(gDHCPHosts){
		free(gDHCPHosts);
		gDHCPHosts = NULL;
	}
	return 0;
}

int objHosts(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>type=%d\n", __FUNCTION__, __LINE__,type ) );
	switch( type )
	{
	case eCWMP_tINITOBJ:
		{
		struct CWMP_LINKNODE **ptable = (struct CWMP_LINKNODE **)data;

		if(data==NULL) return -1;

		gDHCPTable = ptable;
		*gDHCPTable = NULL;
		gDHCPHostsNum=0;

		updateDHCPList();
		if( gDHCPHostsNum > 0 )
			return create_Object( gDHCPTable, tHostObject, sizeof(tHostObject), gDHCPHostsNum, 1 );
		else
			return 0;
	     	break;
		}
	case eCWMP_tUPDATEOBJ:
	 	{
		 	unsigned int num,i;
	     	struct CWMP_LINKNODE *old_table;
			
			updateDHCPList();		
			num = gDHCPHostsNum;
			CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>num=%d\n", __FUNCTION__, __LINE__,num ) );	
			
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;
	     	for( i=0; i<num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;

				remove_entity = remove_SiblingEntity( &old_table, i+1 );
				if( remove_entity!=NULL )
				{
					add_SiblingEntity( (struct CWMP_LINKNODE**)&entity->next, remove_entity );
				}else{ 
					unsigned int InstNum=i+1;
					add_Object( name, (struct CWMP_LINKNODE**)&entity->next,  tHostObject, sizeof(tHostObject), &InstNum );
				}
	     	}
	     	if( old_table )
	     		destroy_ParameterTable( (struct CWMP_NODE*)old_table );
			return 0;
	     }
	}

	return -1;
}

/*****************************Hosts. end*********************************/

/*********************LANHostConfigManagement. begin********************/
int getLanDNSList( char *buf )
{
	unsigned char tmp[64];
	char *zeroip="0.0.0.0";

	if( buf==NULL ) return -1;

	buf[0]=0;
	tmp[0]=0;
	getMIB2Str(MIB_LAN_DNS1, tmp);
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
		strcat( buf, tmp );
	
	tmp[0]=0;
	getMIB2Str(MIB_LAN_DNS2, tmp);
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
	{
		if( strlen(buf) > 0 )
			strcat( buf, ",");
		strcat( buf, tmp );
	}
	
	tmp[0]=0;
	getMIB2Str(MIB_LAN_DNS3, tmp);
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
	{
		if( strlen(buf) > 0 )
			strcat( buf, ",");
		strcat( buf, tmp );
	}
	
	return 0;
}

int setLanDNSList( char *buf)
{
	char *tok1, *tok2, *tok3;
	int  ret=0;
	struct in_addr in1,in2,in3,emp_in;

	if( buf==NULL ) return -1;
	tok1 = NULL;
	tok2 = NULL;
	tok3 = NULL;

	tok1=strtok( buf, "," );
	tok2=strtok( NULL, "," );
	tok3=strtok( NULL, "," );
	if( (tok1==NULL)&&(tok2==NULL)&&(tok3==NULL) )
		return -1;
	
	if(tok1) 
		if(  inet_aton( tok1, &in1 )==0  ) ret=-1;
	if(tok2) 
		if(  inet_aton( tok2, &in2 )==0  ) ret=-1;
	if(tok3) 
		if(  inet_aton( tok3, &in3 )==0  ) ret=-1;

	memset( &emp_in, 0, sizeof(struct in_addr) );
	if(ret==0)
	{
		if( tok1!=NULL )
			mib_set(MIB_LAN_DNS1, (void *)&in1);
		else
			mib_set(MIB_LAN_DNS1, (void *)&emp_in);
	
		if(tok2!=NULL)
			mib_set(MIB_LAN_DNS2, (void *)&in2);
		else
			mib_set(MIB_LAN_DNS2, (void *)&emp_in);
	
		if(tok3!=NULL)
			mib_set(MIB_LAN_DNS3, (void *)&in3);
		else
			mib_set(MIB_LAN_DNS3, (void *)&emp_in);
	}
	return ret;
}

#ifdef _CWMP_MAC_FILTER_
int getMACAddressList( char *buf, int len )
{
#if 1
	int i, j, total;
	int wlanMacType;
	MACFILTER_T macEntry;

	if (buf == NULL) return -1;
	if (len <= 0) return -1;

	buf[0] = '\0';

	mib_get(MIB_MACFILTER_TBL_NUM, (void *)&total);

	//tr098_printf("total %d", total);

	for (i=1; i<=total; i++)
	{
		char mac[19];
	
		*( (char *) &macEntry ) = (char) i;
		
		if (!mib_get(MIB_MACFILTER_TBL, (void *)&macEntry))
			break;

		if ((strlen(buf)+19) > len) break;

		snprintf(mac, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
				macEntry.macAddr[0], macEntry.macAddr[1], macEntry.macAddr[2],
				macEntry.macAddr[3], macEntry.macAddr[4], macEntry.macAddr[5]);

		//tr098_printf("mac %s", mac);

		if (strstr(buf, mac) == NULL)
		{
			//tr098_printf("mac %s", mac);
			if (buf[0]) strcat(buf, ",");
			strcat(buf, mac);
		}
	}
	
	//tr098_printf("buf %s", buf);

	for (i=0; i<NUM_WLAN_INTERFACE; i++)
	{
		/* 0: disable, 1: allow list, 2: deny list */
		getWlanMib(i, 0, MIB_WLAN_MACAC_ENABLED, (void *)&wlanMacType);

		//tr098_printf("wlanMacType %d", wlanMacType);

		if (wlanMacType != 2) continue;
		
		getWlanMib(i, 0, MIB_WLAN_MACAC_NUM, (void *)&total);

		//tr098_printf("total %d", total);

		for (j=1; j<=total; j++)
		{
			char mac[19];

			*( (char *) &macEntry ) = (char) j;

			getWlanMib(i, 0, MIB_WLAN_MACAC_ADDR, (void *)&macEntry);

			if ((strlen(buf)+19) > len) break;

			snprintf(mac, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
					macEntry.macAddr[0], macEntry.macAddr[1], macEntry.macAddr[2],
					macEntry.macAddr[3], macEntry.macAddr[4], macEntry.macAddr[5]);

			//tr098_printf("mac %s", mac);

			if (strstr(buf, mac) == NULL)
			{
				//tr098_printf("mac %s", mac);
				if (buf[0]) strcat(buf, ",");
				strcat(buf, mac);		
			}
		}
	}

	//tr098_printf("buf %s", buf);
	
	return 0;
#else
	int total,i;
	CWMP_MAC_FILTER_T MacEntry;
	
	if(buf==NULL || len<=0) return -1;
	buf[0]='\0';

	mib_get(MIB_CWMP_MACFILTER_TBL_NUM, (void *)&total);

	for (i = 1; i <= total; i++)
	{
		*((char *)&MacEntry) = (char)i;
		if ( !mib_get(MIB_CWMP_MACFILTER_TBL, (void *)&MacEntry))
			continue;
		//action==allow, dstMac==0, dir==outgoing
		if( (MacpDHCPStaticEntry->action==1)    && (MacpDHCPStaticEntry->dir==DIR_OUT) &&
		    (MacpDHCPStaticEntry->dstMac[0]==0) && (MacpDHCPStaticEntry->dstMac[1]==0) &&
		    (MacpDHCPStaticEntry->dstMac[2]==0) && (MacpDHCPStaticEntry->dstMac[3]==0) &&
		    (MacpDHCPStaticEntry->dstMac[4]==0) && (MacpDHCPStaticEntry->dstMac[5]==0)
		  )
		{
			char tmp[19];
			if(strlen(buf)+19>len) break;
			
			if(buf[0]) strcat(buf,",");
			snprintf( tmp, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
				MacpDHCPStaticEntry->srcMac[0], MacpDHCPStaticEntry->srcMac[1],
				MacpDHCPStaticEntry->srcMac[2], MacpDHCPStaticEntry->srcMac[3],
				MacpDHCPStaticEntry->srcMac[4], MacpDHCPStaticEntry->srcMac[5]);
			strcat(buf,tmp);
	}
	}
	return 0;
#endif
}

int setMACAddressList( char *buf )
{
#if 1
	int i;
	char *token;
	int wlanMacType;
	MACFILTER_T macEntry;

	if (buf == NULL) return -1;

	//tr098_trace();

	// clear all allow lists
	mib_set(MIB_MACFILTER_DELALL, (void *)&macEntry);

	for (i=0; i<NUM_WLAN_INTERFACE; i++)
	{
		setWlanMib(i, 0, MIB_WLAN_AC_ADDR_DELALL, (void *)&macEntry);
	}

	// set new allow lists
	token = strtok(buf, ", \n\r");

	while (token != NULL)
	{
		char m0, m1, m2, m3, m4, m5;

		//tr098_printf("token %s", token);

		if (sscanf(token, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m0, &m1, &m2, &m3, &m4, &m5 ) == 6)
		{
			if (m0<256 && m1<256 && m2<256 && m3<256 && m4<256 && m5<256)
			{
				//tr098_trace();
				memset(&macEntry, 0, sizeof(MACFILTER_T));

				macEntry.macAddr[0] = (unsigned char) m0;
				macEntry.macAddr[1] = (unsigned char) m1;
				macEntry.macAddr[2] = (unsigned char) m2;
				macEntry.macAddr[3] = (unsigned char) m3;
				macEntry.macAddr[4] = (unsigned char) m4;
				macEntry.macAddr[5] = (unsigned char) m5;

				//tr098_printf("%d:%d:%d:%d:%d:%d",
				//	macpDHCPStaticEntry->macAddr[0], macpDHCPStaticEntry->macAddr[1], macpDHCPStaticEntry->macAddr[2],
				//	macpDHCPStaticEntry->macAddr[3], macpDHCPStaticEntry->macAddr[4], macpDHCPStaticEntry->macAddr[5]);

				mib_set(MIB_MACFILTER_ADD, (void *)&macEntry);

				for (i=0; i<NUM_WLAN_INTERFACE; i++)
				{
					setWlanMib(i, 0, MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry);
				}
			}
			else
			{
				//tr098_trace();
				return -1;
			}
		}
		else
		{
			//tr098_trace();
			return -1;
		}

		token = strtok(NULL, ", \n\r");
	}

	/* 0: disable, 1: allow list, 2: deny list */
	wlanMacType = 2;
	for (i=0; i<NUM_WLAN_INTERFACE; i++)
	{
		setWlanMib(i, 0, MIB_WLAN_MACAC_ENABLED, (void *)&wlanMacType);
	}

	return 0;
#else
	int total,i;
	CWMP_MAC_FILTER_T MacEntry;
	
	if(buf==NULL) return -1;

	//clear all allow list entries
	mib_get(MIB_CWMP_MACFILTER_TBL_NUM, (void *)&total);
	for (i = 1; i <= total; i++)
	{
		*((char *)&MacEntry) = (char)i;
		if ( !mib_get(MIB_CWMP_MACFILTER_TBL, (void *)&MacEntry))
			continue;

		//action==allow, dstMac==0, dir==outgoing
		if( (MacpDHCPStaticEntry->action==1)    && (MacpDHCPStaticEntry->dir==DIR_OUT) &&
		    (MacpDHCPStaticEntry->dstMac[0]==0) && (MacpDHCPStaticEntry->dstMac[1]==0) &&
		    (MacpDHCPStaticEntry->dstMac[2]==0) && (MacpDHCPStaticEntry->dstMac[3]==0) &&
		    (MacpDHCPStaticEntry->dstMac[4]==0) && (MacpDHCPStaticEntry->dstMac[5]==0)
		  )
		{
			mib_set(MIB_CWMP_MACFILTER_DEL, (void *)&MacEntry);
		}
	}
	
	//set a new allow list
	{
		char *tok;
		
		tok = strtok( buf, ", \n\r" );
		while(tok!=NULL)
		{
			unsigned int m0,m1,m2,m3,m4,m5;
			if( sscanf( tok, "%x:%x:%x:%x:%x:%x", &m0, &m1, &m2, &m3, &m4, &m5 )==6 )
			{
				if( m0<256 && m1<256 && m2<256 && m3<256 && m4<256 && m5<256)
				{
					memset( &MacEntry, 0, sizeof( CWMP_MAC_FILTER_T ) );
					MacpDHCPStaticEntry->action=1;
					MacpDHCPStaticEntry->dir=DIR_OUT;
					MacpDHCPStaticEntry->srcMac[0]=(unsigned char)m0;
					MacpDHCPStaticEntry->srcMac[1]=(unsigned char)m1;
					MacpDHCPStaticEntry->srcMac[2]=(unsigned char)m2;
					MacpDHCPStaticEntry->srcMac[3]=(unsigned char)m3;
					MacpDHCPStaticEntry->srcMac[4]=(unsigned char)m4;
					MacpDHCPStaticEntry->srcMac[5]=(unsigned char)m5;
					mib_set(MIB_CWMP_MACFILTER_ADD, (void *)&MacEntry);
				}else
					return -1;
			}else
				return -1;
			
			
			tok=strtok( NULL, ", \n\r" );
		}
	}

	return 0;
#endif
}
#endif /*_CWMP_MAC_FILTER_*/

#ifdef SUPPORT_RESERVED_DHCP_IPADDR
int getDHCPReservedIPAddr(char *out, unsigned int *in)
{
	int i = 0;
	if(!out || !in) return -1;
	while(in[i]){
		char *ipStr;
		ipStr = inet_ntoa(*((struct in_addr *)&(in[i])));		
		CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d > (i=%d, ipStr=%s)\n", __FUNCTION__, __LINE__, i,ipStr) );
		if(strlen(out) == 0)
			sprintf(out, "%s", ipStr);
		else{
			strcat(out, ",");
			strcat(out, ipStr);
		}
		i++;
	}
	
	return 1;
}

int setDHCPReservedIPAddr(unsigned int *out, char *in)
{
	struct in_addr	inaddr;
	int i = 0;
	if(!out || !in) return -1;
	if(in && strlen(in)){
		char *tok;
		tok = strtok(in, ",");
		while(tok){	
//			printf("[%s:%d] tok=%s\n",__FUNCTION__,__LINE__,tok);	
			if(inet_aton(tok, &inaddr) == 0)
				return -1;
			out[i++] = (unsigned int)inaddr.s_addr;
			tok = strtok(NULL, ",");
		}
	}
	return 1;
}

int check_reserveIP_invalid(struct in_addr *IpAddr, struct in_addr *start_ip,struct in_addr *end_ip,struct in_addr *router_ip,struct in_addr *netmask){
	if(!IpAddr || !start_ip || !end_ip || !router_ip || !netmask) return -1;

	if(((IpAddr->s_addr & netmask->s_addr) != (router_ip->s_addr & netmask->s_addr)))
		return -1;
	
	if(((IpAddr->s_addr) & (~netmask->s_addr))<((start_ip->s_addr) & (~netmask->s_addr)) 
		|| ((IpAddr->s_addr) & (~netmask->s_addr)) >((end_ip->s_addr) & (~netmask->s_addr)) 
		|| ((IpAddr->s_addr) & (~netmask->s_addr))==((router_ip->s_addr) & (~netmask->s_addr))
		)
		return -1;

	return 1;
}

int check_def_poor_reserveIP(char *setBuf){
	struct in_addr start_ip, end_ip, router_ip, netmask, set_ip;
	char tmpBuf1[256],*tok;
	struct in_addr backup_ip[16] = {0};
	int i = 0, num = 0;
	if(!setBuf) return -1;
	
	apmib_get(MIB_DHCP_CLIENT_START,  (void *)&start_ip);
	apmib_get(MIB_DHCP_CLIENT_END,  (void *)&end_ip);	
	apmib_get(MIB_IP_ADDR,  (void *)&router_ip);
	apmib_get(MIB_SUBNET_MASK,  (void *)&netmask);
	
	strcpy(tmpBuf1, setBuf);
	tok = strtok(tmpBuf1, ",");
	while(tok && strlen(tok)){
		if(++num > 16) return -1;//max set 16 nums, for mib size
		if(inet_aton(tok, &set_ip) == 0 ) return -1;
		
		i = 0;
		while(backup_ip[i].s_addr != 0){/* check same ip*/
			int j = 0;
			for(j = 0; j <= i; j++){
				if(memcmp(&set_ip, &backup_ip[j], sizeof(struct in_addr)) == 0) return -1;
			}
			i++;
		}
		memcpy(&backup_ip[i], &set_ip, sizeof(struct in_addr));
		
		if(check_reserveIP_invalid(&set_ip,&start_ip, &end_ip, &router_ip, &netmask) < 0)
			return -1;
		tok = strtok(NULL, ",");
	}
	return 1;
}

int check_conf_poor_reserveIP(char *setBuf, DHCPS_SERVING_POOL_T *pDHCPSPEntry){
	struct in_addr start_ip, end_ip, router_ip, netmask, set_ip;
	char tmpBuf1[256], *tok;
	struct in_addr backup_ip[16] = {0};
	int i = 0, num = 0;
	if(!setBuf || !pDHCPSPEntry) return -1;

	apmib_get(MIB_IP_ADDR,  (void *)&router_ip);
	memcpy(&start_ip, &pDHCPSPEntry->startaddr, sizeof(pDHCPSPEntry->startaddr));
	memcpy(&end_ip, &pDHCPSPEntry->endaddr, sizeof(pDHCPSPEntry->endaddr));
	memcpy(&netmask, &pDHCPSPEntry->subnetmask, sizeof(pDHCPSPEntry->subnetmask));

	if(start_ip.s_addr == 0 || end_ip.s_addr == 0 || netmask.s_addr == 0) return -1;
	
	strcpy(tmpBuf1, setBuf);
	tok = strtok(tmpBuf1, ",");
	while(tok && strlen(tok)){
		if(++num > 16) return -1;//max set 16 nums, for mib size
		if(inet_aton(tok, &set_ip) == 0 ) return -1;
		
		i = 0;
		while(backup_ip[i].s_addr != 0){/* check same ip*/
			int j = 0;
			for(j = 0; j <= i; j++){
				if(memcmp(&set_ip, &backup_ip[j], sizeof(struct in_addr)) == 0) return -1;
			}
			i++;
		}
		memcpy(&backup_ip[i], &set_ip, sizeof(struct in_addr));
		
		if(check_reserveIP_invalid(&set_ip,&start_ip, &end_ip, &router_ip, &netmask) < 0)
			return -1;
		tok = strtok(NULL, ",");
	}
	return 1;
}
#endif


#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
unsigned int getDHCPStaticAddrNums(unsigned int usedFor, unsigned int dhcpConSPInstNum);
unsigned int getDHCPOptionNums( unsigned int usedFor, unsigned int dhcpConSPInstNum);
#endif

int getLANHostConf(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned char buf[256]="";
	unsigned char vChar=0;
	int  vInt=0;
	int dhcp_enynum;
	int i, entryNum;

	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;

	if( strcmp( lastname, "MACAddress" )==0 )
	{
		unsigned char buffer[64];
		unsigned char macadd[MAC_ADDR_LEN];
		mib_get(MIB_ELAN_MAC_ADDR, (void *)macadd);
		sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x", macadd[0], macadd[1],						
			macadd[2], macadd[3], macadd[4], macadd[5]);
		*data=strdup(buffer);
	}else if( strcmp( lastname, "DHCPServerConfigurable" )==0 )
	{		
		mib_get( MIB_LAN_DHCP_CONFIGURABLE, (void *)&vInt);
		*data = booldup( (vInt!=0) );
	}
	else if( strcmp( lastname, "DHCPServerEnable" )==0 )
	{
		mib_get( MIB_DHCP, (void *)&vInt);
		*data = booldup( (vInt!=DHCP_DISABLED) );
	}
	else if( strcmp( lastname, "DHCPRelay" )==0 )
	{
		mib_get( MIB_DHCP, (void *)&vInt);
		*data = booldup( (vInt==DHCP_RELAY) ); //default 'server' when disable
	}
	else if( strcmp( lastname, "MinAddress" )==0 )
	{
		getMIB2Str(MIB_DHCP_CLIENT_START, buf);
		*data=strdup( buf );
	}
	else if( strcmp( lastname, "MaxAddress" )==0 )
	{
		getMIB2Str(MIB_DHCP_CLIENT_END, buf);
		*data=strdup( buf );
	}
	else if( strcmp( lastname, "ReservedAddresses" )==0 )
	{
#ifdef SUPPORT_RESERVED_DHCP_IPADDR
		unsigned int reserveIp[16] = {0};
		mib_get(MIB_DHCP_RESERVED_IPADDR, (void *)reserveIp);
		if( getDHCPReservedIPAddr(buf, reserveIp)< 0)
			*data = strdup("");
		else
			*data = strdup(buf);
#else
		*data = strdup("");
#endif
	}
	else if( strcmp( lastname, "SubnetMask" )==0 )
	{
#ifdef SECONDARY_IP
		mib_get(MIB_ADSL_LAN_DHCP_POOLUSE, (void *)&vChar);
#else
		vChar = 0;
#endif //SECONDARY_IP
		getMIB2Str(MIB_SUBNET_MASK, buf);
		*data=strdup( buf );
	}
	else if( strcmp( lastname, "DNSServers" )==0 )
	{
		getLanDNSList(buf);			
		if(strlen(buf) == 0 )
			getMIB2Str(MIB_IP_ADDR, (void *)buf);
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "DomainName" )==0 )
	{
		mib_get(MIB_DOMAIN_NAME, (void *)buf);
		*data=strdup( buf );
	}
	else if( strcmp( lastname, "IPRouters" )==0 )
	{
		getMIB2Str(MIB_DEFAULT_GATEWAY, buf);
		if(strcmp(buf, "0.0.0.0") == 0){
			getMIB2Str(MIB_IP_ADDR, buf);
		}
		*data=strdup( buf );
	}
	else if( strcmp( lastname, "DHCPLeaseTime" )==0 )
	{
		mib_get(MIB_DHCP_LEASE_TIME, (void *)&vInt);
		*data=intdup( vInt * 60 );
	}
	else if( strcmp( lastname, "UseAllocatedWAN" )==0 )
	{
		*data=strdup( "Normal" );
	}
	else if( strcmp( lastname, "AssociatedConnection" )==0 )
	{
		*data=strdup( "" );
	}
	else if( strcmp( lastname, "PassthroughLease" )==0 )
	{
		*data=uintdup( 0 );
	}
	else if( strcmp( lastname, "PassthroughMACAddress" )==0 )
	{
		*data=strdup( "" );
	}
#ifdef _CWMP_MAC_FILTER_
	else if( strcmp( lastname, "AllowedMACAddresses" )==0 )
	{
		buf[0]=0;
		getMACAddressList( buf,256 );
		*data=strdup( "" );
	}
#endif /*_CWMP_MAC_FILTER_*/
	
	else if( strcmp( lastname, "IPInterfaceNumberOfEntries" )==0 )
	{
#if defined(CONFIG_X_CWMP_MULTI_LAN)	
		unsigned int IPIFCount=0;
		IPIFCount = getLANConDevIPIFCount(chainid);
		*data = uintdup(IPIFCount);
#else
		*data = uintdup(1);
#endif
	}
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	else if( strcmp( lastname, "DHCPStaticAddressNumberOfEntries" )==0 )
	{
		unsigned int usedFor=eUsedFor_DHCPServer;
		*data = uintdup(getDHCPStaticAddrNums(usedFor, 0));
	}
	else if( strcmp( lastname, "DHCPOptionNumberOfEntries" )==0 )
	{
		unsigned int usedFor=eUsedFor_DHCPServer;
		*data = uintdup(getDHCPOptionNums(usedFor, 0));
	}
	else if( strcmp( lastname, "DHCPConditionalPoolNumberOfEntries" )==0 )
	{
		int var=0;
		apmib_get(MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&var);
		*data = uintdup(var);
	}	
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*ping_zhang:20080919 END*/
	else{
		return ERR_9005;
	}
	
	return 0;
}

static int isValidDomainName(char *str)
{
	int i, len=strlen(str);
	for(i = 0; i < len; i++)
	{
		if((str[i] >= '0' && str[i] <= '9') ||
			(str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] >= 'A' && str[i] <= 'Z') ||
			str[i] == '-')
			continue;
		
		return 0;
	}
	
	if( (str[0] >= '0' && str[0] <= '9') ||
		str[0] == '-' || 
		str[len-1] == '-')
		return 0;

	return 1;
}

int setLANHostConf(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int	vInt=0;
	unsigned int	ServerEn = 0, ServerConfigurable = 0;
	struct in_addr in;
	
	if( (name==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;
	//if( data==NULL ) return ERR_9007;

	mib_get( MIB_DHCP, (void *)&ServerEn);
	mib_get( MIB_LAN_DHCP_CONFIGURABLE, (void *)&ServerConfigurable);
	if( strcmp( lastname, "DHCPServerConfigurable" )==0 )
	{
		int *i = data;

		if( i==NULL ) return ERR_9007;
		vInt = (*i==0)?0:1;
		mib_set(MIB_LAN_DHCP_CONFIGURABLE, (void *)&vInt);
		return 0;
	}
	else if( strcmp( lastname, "DHCPServerEnable" )==0 )
	{
		int *i = data;
		
		if( i==NULL ) return ERR_9007;
		if( ServerConfigurable==0 ) return ERR_9001;
		vInt = (*i==0)?0:DHCP_SERVER; /*default:DHCP Server, not relay*/
		mib_set(MIB_DHCP, (void *)&vInt);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "MinAddress" )==0 )
	{		
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		mib_set( MIB_DHCP_CLIENT_START, (void *)&in);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "MaxAddress" )==0 )
	{		
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		mib_set( MIB_DHCP_CLIENT_END, (void *)&in);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "ReservedAddresses" )==0 )
	{
		unsigned int reserveIp[16] = {0};
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;	
#ifdef SUPPORT_RESERVED_DHCP_IPADDR
		if(check_def_poor_reserveIP(buf) < 0) return ERR_9007;
		if( setDHCPReservedIPAddr(reserveIp, buf)< 0) return ERR_9007;
		mib_set(MIB_DHCP_RESERVED_IPADDR, (void *)reserveIp);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
#else
		return ERR_9001;
#endif
	}
	else if( strcmp( lastname, "SubnetMask" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
	//	if( ServerEn==0 ) return ERR_9001;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;

		mib_set(MIB_SUBNET_MASK, (void *)&in);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;	
#endif
	}
	else if( strcmp( lastname, "DNSServers" )==0 )
	{
		char *tok;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if(setLanDNSList( buf ) < 0)
			return ERR_9007;
		
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_SL, apply_DNS, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;		
#endif
	}
	else if( strcmp( lastname, "DomainName" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		//mib defines the length of domainname=30
		if( strlen(buf)>=30 ) return ERR_9001;
		if(!isValidDomainName(buf)) return ERR_9007;

		mib_set(MIB_DOMAIN_NAME, (void *)buf);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "IPRouters" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
	//	if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( inet_aton( buf, &in )==0 ) return ERR_9007;		
		mib_set( MIB_DEFAULT_GATEWAY, (void *)&in);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "DHCPLeaseTime" )==0 )
	{
		int *pInt = data;

		if(pInt==NULL) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if(*pInt<-1) return ERR_9007;
		int mins = (*pInt)/60;
		mib_set(MIB_DHCP_LEASE_TIME, (void *)&mins);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "UseAllocatedWAN" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "Normal" )!=0 ) return ERR_9007;
		return 0;
	}
	else if( strcmp( lastname, "AssociatedConnection" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "" )!=0 ) return ERR_9001;
		return 0;
	}
	else if( strcmp( lastname, "PassthroughLease" )==0 )
	{
		unsigned int *pUint = data;

		if( pUint==NULL ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( *pUint!=0 ) return ERR_9001;
		return 0;
	}
	else if( strcmp( lastname, "PassthroughMACAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( ServerEn==0 || ServerConfigurable == 0) return ERR_9001;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "" )!=0 ) return ERR_9001;
		return 0;
	}
#ifdef _CWMP_MAC_FILTER_
	else if( strcmp( lastname, "AllowedMACAddresses" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		else return ERR_9001;
	//	if( setMACAddressList(buf)< 0) return ERR_9007;
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_MACFILTER, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 0;
#endif
	}
#endif /*_CWMP_MAC_FILTER_*/
	else{
		return ERR_9005;
	}
	
	return 0;
}

/*********************LANHostConfigManagement. end********************/

/**********************IPInterface begin******************************/

int getIPItfEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned char buf[256];
	unsigned int vInt=0;
#ifdef SECONDARY_IP
	unsigned char instnum=0;
#endif //SECONDARY_IP	

	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;
#ifdef SECONDARY_IP
	instnum = getIPItfInstNum(name);
	if(instnum<=0) return ERR_9005;
#endif //SECONDARY_IP

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
#ifdef SECONDARY_IP
	    if(instnum==2)
		mib_get(MIB_ADSL_LAN_ENABLE_IP2, (void *)&vChar);
	    else
#endif //SECONDARY_IP
		mib_get(MIB_CWMP_LAN_IPIFENABLE, (void *)&vInt);
		*data = booldup( vInt!=0 );
	}
	else if( strcmp( lastname, "IPInterfaceIPAddress" )==0 )
	{
#ifdef SECONDARY_IP
	    if(instnum==2)
		getMIB2Str(MIB_ADSL_LAN_IP2, buf);
	    else
#endif //SECONDARY_IP
		getMIB2Str(MIB_IP_ADDR, buf);
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "IPInterfaceSubnetMask" )==0 )
	{
#ifdef SECONDARY_IP
	    if(instnum==2)
		getMIB2Str(MIB_ADSL_LAN_SUBNET2, buf);
	    else
#endif //SECONDARY_IP
		getMIB2Str(MIB_SUBNET_MASK, buf);
		*data=strdup( buf );
	}
	else if( strcmp( lastname, "IPInterfaceAddressingType" )==0 )
	{
		apmib_get(MIB_DHCP,(void *)&vInt);
		if(vInt == DHCP_CLIENT)
			*data = strdup( "DHCP" );
		else
			*data = strdup( "Static" );
			
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setIPItfEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int vInt=0, configurable = 0;
	struct in_addr in;
#ifdef SECONDARY_IP
	unsigned char instnum=0;
#endif //SECONDARY_IP

	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;

	if( data==NULL ) return ERR_9007;
#ifdef SECONDARY_IP
	instnum = getIPItfInstNum(name);
	if(instnum<=0) return ERR_9005;
#endif //SECONDARY_IP

	mib_get(MIB_CWMP_LAN_IPIFENABLE, (void *)&configurable);

	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i = data;
		
		if( i==NULL ) return ERR_9007;
		vInt = (*i==0)?0:1;
#ifdef SECONDARY_IP
	    if(instnum==2)
		mib_set(MIB_ADSL_LAN_ENABLE_IP2, (void *)&vChar);
	    else
#endif //SECONDARY_IP
		mib_set(MIB_CWMP_LAN_IPIFENABLE, (void *)&vInt);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_LANIP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else		
		return 1;		
#endif
	}
	else if( strcmp( lastname, "IPInterfaceIPAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( inet_aton( buf, &in )==0  ) return ERR_9007;
		if( configurable == 0) return ERR_9001;
/*for telefonica, when modifying this parameter, do not modify the dhcp gateway at the same time*/
#ifndef TELEFONICA_DEFAULT_CFG
		/*web behavior => after setting lan ip, change the dhcp gateway with the same ip*/
#ifdef SECONDARY_IP
		if(instnum==1)
#endif //SECONDARY_IP
		{
		    /*lan ip & dhcp gateway setting should be set independently*/
			#if 0
		    #if 1
			struct in_addr dhcp_gw, origIp;
			mib_get(MIB_IP_ADDR, (void *)&origIp);
		
			mib_get(MIB_DEFAULT_GATEWAY, (void *)&dhcp_gw);
			if(dhcp_gw.s_addr==origIp.s_addr)
				mib_set(MIB_DEFAULT_GATEWAY, (void *)&in);
		    #else
			mib_set( MIB_ADSL_LAN_DHCP_GATEWAY, (void *)&in);
		    #endif
			#endif
		}
#endif //TELEFONICA_DEFAULT_CFG


#ifdef SECONDARY_IP
	    if(instnum==2)
		mib_set(MIB_ADSL_LAN_IP2, (void *)&in);	
	    else
#endif //SECONDARY_IP
		mib_set(MIB_IP_ADDR, (void *)&in);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_LANIP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else		
		return 1;			
#endif
	}
	else if( strcmp( lastname, "IPInterfaceSubnetMask" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		if(configurable == 0) return ERR_9001;
		
#ifdef SECONDARY_IP
	    if(instnum==2)
		mib_set(MIB_ADSL_LAN_SUBNET2, (void *)&in);
	    else
#endif //SECONDARY_IP
		mib_set(MIB_SUBNET_MASK, (void *)&in);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_LANIP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "IPInterfaceAddressingType" )==0 )
	{
		if( (buf!=NULL) && (strlen(buf)==0) ) return ERR_9007;
		if(configurable == 0) return ERR_9001;
		if( strcmp(buf, "DHCP")==0 )
			vInt = DHCP_CLIENT; 
		else if( strcmp(buf, "Static")==0 )
			vInt = DHCP_SERVER;
		else if( strcmp(buf, "AutoIP")==0 )
			return ERR_9001;
		else
			return ERR_9007;	
		apmib_set(MIB_DHCP,(void *)&vInt);
		
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_LANIP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else{
		return ERR_9005;
	}

	return 0;
}
/**********************IPInterface end******************************/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_

unsigned int getDHCPConSPInstNum( char *name )
{
	return getInstNum( name, "DHCPConditionalServingPool" );
}

/*********************DHCPStaticAddress begin*******************************/


unsigned int getDHCPStaticAddrInstNum( char *name )
{
	return getInstNum( name, "DHCPStaticAddress" );
}

unsigned int getDHCPStaticAddrNums(unsigned int usedFor, unsigned int dhcpConSPInstNum)
{
	unsigned int ret=0, i,num;
	DHCPRSVDIP_T *p,DHCPStaticAddr_entity;

	apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
	p = &DHCPStaticAddr_entity;
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(DHCPRSVDIP_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)p ))
			continue;
		if(p->usedFor!=usedFor || dhcpConSPInstNum != p->dhcpConSPInstNum)
			continue;
		ret++;
	}

	return ret;
}

int checkSameIp(struct in_addr *IpAddr){
	int i, entryNum;
	DHCPRSVDIP_T entry;
	struct in_addr start_ip, end_ip, router_ip, netmask;
	if(!IpAddr) return -1;
		
	if(!apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entryNum)) return 0;
	
	for(i = 1; i <= entryNum; i++){
		*((char *)&entry) = i;
		if(!apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry)) return 0;
		if(memcmp(IpAddr, entry.ipAddr, 4)==0)
			return -1;
	}

	apmib_get(MIB_DHCP_CLIENT_START,  (void *)&start_ip);
	apmib_get(MIB_DHCP_CLIENT_END,  (void *)&end_ip);	
	apmib_get(MIB_IP_ADDR,  (void *)&router_ip);
	apmib_get(MIB_SUBNET_MASK,  (void *)&netmask);
	if((IpAddr->s_addr & netmask.s_addr) != (router_ip.s_addr & netmask.s_addr)) return -1;

	if(((IpAddr->s_addr) & (~netmask.s_addr))<((start_ip.s_addr) & (~netmask.s_addr)) 
	|| ((IpAddr->s_addr) & (~netmask.s_addr)) >((end_ip.s_addr) & (~netmask.s_addr)) 
	|| ((IpAddr->s_addr) & (~netmask.s_addr))==((router_ip.s_addr) & (~netmask.s_addr))
	) return -1;

	return 1;
}

int checkSameMac(char *macAddr){
	int i, entryNum;
	DHCPRSVDIP_T entry ;	
	if(!macAddr) return -1;
		
	if(!apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entryNum)) return 0;
	for(i = 1; i <= entryNum; i++){
		*((char *)&entry) = i;
		if(!apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry)) return 0;
		if(memcmp(macAddr, entry.macAddr, 6)==0)
			return -1;
	}
	return 1;
}

int getDHCPStaticAddrByOptInstNum( unsigned int dhcpstaticAddrNum, unsigned int dhcpSPNum, unsigned int usedFor, DHCPRSVDIP_T *p, unsigned int *id )
{
	int ret = -1;
	unsigned int i,num;

	apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&num);
	for (i=1; i<=num; i++) {
		memset(p, '\0', sizeof(DHCPRSVDIP_T));
		*((char *)p) = (char)i;
		if (!apmib_get(MIB_DHCPRSVDIP_TBL, (void *)p))
			continue;

		if( (p->usedFor==usedFor) && (p->InstanceNum==dhcpstaticAddrNum) 
			&& (p->dhcpConSPInstNum==dhcpSPNum) )
		{
			*id = i;
			ret = 0;
			break;
		}
	}
	return 0;
}

int getDHCPStaticAddrEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;;
	unsigned int chainid;
	unsigned int dhcpStaticAddrInstNum,dhcpConSPOptNum;
	int num,i;
	int ret=0;
	char buf[256];
	unsigned int uInt;
	DHCPRSVDIP_T *pDHCPStaticEntry, DhcpStaticAddrEntry;
	unsigned char usedFor;	

	dhcpStaticAddrInstNum = getDHCPStaticAddrInstNum( name );
	dhcpConSPOptNum = getDHCPConSPInstNum( name );
	
	pDHCPStaticEntry = &DhcpStaticAddrEntry;
	if(dhcpConSPOptNum != 0)  						//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPStaticAddress.{i}.
	{
		usedFor = eUsedFor_DHCPServer_ServingPool;
		if( getDHCPStaticAddrByOptInstNum(dhcpStaticAddrInstNum,dhcpConSPOptNum, usedFor, pDHCPStaticEntry, &chainid) < 0 )
		return ERR_9002;
	}
	else if(dhcpConSPOptNum==0 && dhcpStaticAddrInstNum!=0)	 //for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPStaticAddress.{i}.
	{
		usedFor = eUsedFor_DHCPServer;
		if( getDHCPStaticAddrByOptInstNum(dhcpStaticAddrInstNum,dhcpConSPOptNum, usedFor, pDHCPStaticEntry, &chainid) < 0)
		return ERR_9002;
	}
	else
		return ERR_9005;
	
	*type = entity->info->type;
	*data = NULL;

	
	//ret=getDHCPStaticAddrByInstNum(dhcpStaticAddrInstNum,pDHCPStaticEntry,&chainid);
	//if(ret<0) return ERR_9002;
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	if( strcmp( lastname, "Enable" )==0 )
	{
		*data = booldup(pDHCPStaticEntry->dhcpRsvdIpEntryEnabled);
	}
#else
	if( strcmp( lastname, "Enable" )==0 )
	{
		mib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&uInt);
		*data = booldup(uInt);
	}
#endif
	else if( strcmp( lastname, "Chaddr" )==0 )
	{
		char macaddr[30];

		sprintf(macaddr," %02x:%02x:%02x:%02x:%02x:%02x",
			pDHCPStaticEntry->macAddr[0], pDHCPStaticEntry->macAddr[1],
			pDHCPStaticEntry->macAddr[2], pDHCPStaticEntry->macAddr[3],
			pDHCPStaticEntry->macAddr[4], pDHCPStaticEntry->macAddr[5]);
		
		*data = strdup(macaddr);
	}else if( strcmp( lastname, "Yiaddr" )==0 )
	{
		*data = strdup(inet_ntoa(*((struct in_addr*)pDHCPStaticEntry->ipAddr)));
	}else
	{
		return ERR_9005;
	}
	
	return 0;
}

int setDHCPStaticAddrEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	unsigned int chainid;
	unsigned char usedFor;
	unsigned int dhcpStaticAddrInstNum,dhcpConSPOptNum;
	int num,i;
	int ret=0;
	char *buf=data;
	DHCPRSVDIP_T *pDHCPStaticAddrEntry, DhcpStaticAddrEntry[2];
	char tmpbuf[30]={0};
	char *tok, del[] = ", ", *pstr;

	if( (name==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	dhcpStaticAddrInstNum = getDHCPStaticAddrInstNum( name );
	dhcpConSPOptNum = getDHCPConSPInstNum( name );
	
	pDHCPStaticAddrEntry = &DhcpStaticAddrEntry[0];
	if(dhcpConSPOptNum != 0)	 //for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPStaticAddress.{i}.
	{
		usedFor = eUsedFor_DHCPServer_ServingPool;
		if( getDHCPStaticAddrByOptInstNum(dhcpStaticAddrInstNum, dhcpConSPOptNum, usedFor, pDHCPStaticAddrEntry, &chainid) < 0 )
		return ERR_9002;
	}
	else if(dhcpConSPOptNum==0 && dhcpStaticAddrInstNum!=0)	//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPStaticAddress.{i}.
	{
		usedFor = eUsedFor_DHCPServer;
		if( getDHCPStaticAddrByOptInstNum(dhcpStaticAddrInstNum, dhcpConSPOptNum, usedFor, pDHCPStaticAddrEntry, &chainid) < 0)
		return ERR_9002;
	}
	else
		return ERR_9005;
		
	memcpy(&DhcpStaticAddrEntry[1], &DhcpStaticAddrEntry[0], sizeof(DHCPRSVDIP_T));
	pDHCPStaticAddrEntry = &DhcpStaticAddrEntry[1];


	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i = data;
		int iVal;
		if( i==NULL ) return ERR_9007;
		iVal = (*i==0) ? 0:1;		

		pDHCPStaticAddrEntry->dhcpRsvdIpEntryEnabled=iVal;	
		apmib_set( MIB_DHCPRSVDIP_MOD, (void*)&DhcpStaticAddrEntry );

		return 1;
	}
	else if( strcmp( lastname, "Chaddr" )==0 )
	{
		char m0, m1, m2, m3, m4, m5;

		//tr098_printf("buf %s", buf);

		if (sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m0, &m1, &m2, &m3, &m4, &m5 ) == 6)
		{
			pDHCPStaticAddrEntry->macAddr[0] = (unsigned char) m0;
			pDHCPStaticAddrEntry->macAddr[1] = (unsigned char) m1;
			pDHCPStaticAddrEntry->macAddr[2] = (unsigned char) m2;
			pDHCPStaticAddrEntry->macAddr[3] = (unsigned char) m3;
			pDHCPStaticAddrEntry->macAddr[4] = (unsigned char) m4;
			pDHCPStaticAddrEntry->macAddr[5] = (unsigned char) m5;

			int ret = checkSameMac(pDHCPStaticAddrEntry->macAddr);
			if(ret == -1) 
				return ERR_9007;
			else if(ret == 0) 
				return ERR_9002;
			
			apmib_set( MIB_DHCPRSVDIP_MOD, (void*)&DhcpStaticAddrEntry );

		/*	CWMPDBG( 1, ( stderr, "<%s:%d > (enable = %hhx:%hhx:%hhx:%hhx:%hhx:%hhx)\n", __FUNCTION__, __LINE__,
				pDHCPStaticAddrEntry->macAddr[0],pDHCPStaticAddrEntry->macAddr[1],
				pDHCPStaticAddrEntry->macAddr[2],pDHCPStaticAddrEntry->macAddr[3],
				pDHCPStaticAddrEntry->macAddr[4],pDHCPStaticAddrEntry->macAddr[5]) );
		*/
		}else
		{
			return ERR_9007;
		}		

		return 1;			
	}
	else if( strcmp( lastname, "Yiaddr" )==0 )
	{
		struct in_addr inIp;
		struct in_addr inLanaddr_orig, inLanmask_orig;
		if(!inet_aton(buf, &inIp)) return ERR_9007;

		int retval = checkSameIp(&inIp);
		if(retval == -1) 
			return ERR_9007;
		else if(retval == 0) {
			return ERR_9002;
		}
		
		memcpy(pDHCPStaticAddrEntry->ipAddr, &inIp, 4);
	
		apmib_set( MIB_DHCPRSVDIP_MOD, (void*)&DhcpStaticAddrEntry );

		return 1;
	}else
	{
		return ERR_9005;
	}
	
	return 0;
}


void compact_staticAddrNode(int usedFor, int dhcpSPNum, int instanceNum)
{
	int i, num;
	DHCPRSVDIP_T *pDHCPRsvdEntry, DhcpRsvdEntry[2];
	pDHCPRsvdEntry = &DhcpRsvdEntry[1];
	
	apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
	for(i = 1; i <= num; i++){
		*(char *)&DhcpRsvdEntry[0] = (char)i;
		mib_get(MIB_DHCPRSVDIP_TBL, (void *)&DhcpRsvdEntry[0]);

		printf("[%s:%d]DhcpRsvdEntry[0].InstanceNum = %d\n",__FILE__, __LINE__, DhcpRsvdEntry[0].InstanceNum);
			
		if(DhcpRsvdEntry[0].usedFor == usedFor && 
			DhcpRsvdEntry[0].dhcpConSPInstNum == dhcpSPNum &&
			DhcpRsvdEntry[0].InstanceNum > instanceNum){
			memcpy(pDHCPRsvdEntry, &DhcpRsvdEntry[0], sizeof(DHCPRSVDIP_T));
			--pDHCPRsvdEntry->InstanceNum;
			printf("[%s:%d]pDHCPRsvdEntry->InstanceNum = %d\n",__FILE__, __LINE__, pDHCPRsvdEntry->InstanceNum);
					
			mib_set(MIB_DHCPRSVDIP_MOD, (void *)DhcpRsvdEntry);
		}
	}
}

unsigned int findMaxDHCPStaticAddrNum(unsigned int usedFor, unsigned int dhcpConSPInstNum)
{
	unsigned int ret=0, i,num;
	DHCPRSVDIP_T *p,DHCPStaticAddr_entity;

	apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
	p = &DHCPStaticAddr_entity;
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(DHCPRSVDIP_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)p ))
			continue;
		if(p->usedFor!=usedFor || dhcpConSPInstNum != p->dhcpConSPInstNum)
			continue;
		if(p->InstanceNum>ret)
			ret=p->InstanceNum;
	}

	return ret;
}

int objDHCPStaticAddr(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	DHCPRSVDIP_T *pDHCPRsvdEntry, DhcpRsvdEntry[2];
	unsigned int chainid;
	unsigned int dhcpConSPInstNum, usedFor, num, i;

	dhcpConSPInstNum = getDHCPConSPInstNum( name );
	if(dhcpConSPInstNum != 0)//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPStaticAddress.{i}.
	{
		usedFor = eUsedFor_DHCPServer_ServingPool;
		switch( type )
		{
			case eCWMP_tINITOBJ:
			{
				int MaxInstNum;
				struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				MaxInstNum = findMaxDHCPStaticAddrNum(usedFor,dhcpConSPInstNum);
				apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
				pDHCPRsvdEntry = &DhcpRsvdEntry[1];
				
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPRsvdEntry, '\0', sizeof(DHCPRSVDIP_T));
					*((char *)pDHCPRsvdEntry) = (char)i;
					if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)pDHCPRsvdEntry ))
						continue;
					else
						memcpy(&DhcpRsvdEntry[0], &DhcpRsvdEntry[1], sizeof(DHCPRSVDIP_T));

					if(pDHCPRsvdEntry->usedFor != usedFor)
						continue;

					if( pDHCPRsvdEntry->dhcpConSPInstNum == dhcpConSPInstNum)
					{
						if( pDHCPRsvdEntry->InstanceNum==0 ) //maybe createn by web or cli
						{
							MaxInstNum++;
							pDHCPRsvdEntry->InstanceNum = MaxInstNum;
							apmib_set( MIB_DHCPRSVDIP_MOD, (void *)&DhcpRsvdEntry);
						}
						if( create_Object( c, tDHCPStaticAddrObject, sizeof(tDHCPStaticAddrObject), 1, pDHCPRsvdEntry->InstanceNum ) < 0 )
							return -1;
					}
				}
				add_objectNum( name, MaxInstNum );
				return 0;
			}	
			case eCWMP_tADDOBJ:
			{
			//	int ret;
			//	char tmpbuf[128];

				int ret, found=0;
				DHCPS_SERVING_POOL_T *pDHCPSPOptEntry, DhcpSPOptEntry;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
				pDHCPSPOptEntry = &DhcpSPOptEntry;
				for (i=1; i<=num; i++)
				{	
					memset(pDHCPSPOptEntry, '\0', sizeof(DHCPS_SERVING_POOL_T));
					*((char *)pDHCPSPOptEntry) = (char)i;
					if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)pDHCPSPOptEntry ) )
						continue;
					if(pDHCPSPOptEntry->InstanceNum == dhcpConSPInstNum )
					{
						found = 1;
						break;
					}
				}
				if(found ==0) return ERR_9005;
				ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPStaticAddrObject, sizeof(tDHCPStaticAddrObject), data );
				if( ret >= 0 )
				{
					DHCPRSVDIP_T entry;
					memset( &entry, 0, sizeof( DHCPRSVDIP_T ) );
					{ //default values for this new entry
						memset(entry.ipAddr, 0, sizeof(entry.ipAddr));
						memset(entry.macAddr, 0, sizeof(entry.macAddr));
						memset(entry.hostName, 0, sizeof(entry.hostName));
						
						entry.dhcpConSPInstNum = pDHCPSPOptEntry->InstanceNum;
						entry.usedFor = usedFor;
						entry.InstanceNum = *(int *)data;
	
					}
					if ( apmib_set( MIB_DHCPRSVDIP_ADD, (void*)&entry) == 0)
					{
						fprintf(stderr,"\r\n Add DHCPRSVDIP table entry error!");
						return -1;
					}						
				}
				return ret;
			}
			case eCWMP_tDELOBJ:
			{
				int ret, num, i;
				int found = 0;
				unsigned int *pUint=data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
				pDHCPRsvdEntry = &DhcpRsvdEntry[1];
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPRsvdEntry, '\0', sizeof(DHCPRSVDIP_T));
					*((char *)pDHCPRsvdEntry) = (char)i;				
					if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)pDHCPRsvdEntry ) )
						continue;
					if(pDHCPRsvdEntry->usedFor == usedFor
							&& pDHCPRsvdEntry->dhcpConSPInstNum==dhcpConSPInstNum
							&& pDHCPRsvdEntry->InstanceNum==*pUint)
					{
						found =1;
						apmib_set( MIB_DHCPRSVDIP_DEL, (void *)DhcpRsvdEntry );
						break;
					}
				}

				if(found==0) return ERR_9005;
				ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
			#if 0
				apply_DHCP(CWMP_RESTART,0,NULL);
			#endif
				//if( ret==0 )	ret=1;
				return ret;
			}
			case eCWMP_tUPDATEOBJ:
			{
				int num,i;
				struct CWMP_LINKNODE *old_table;

				apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
				old_table = (struct CWMP_LINKNODE *)entity->next;
				entity->next = NULL;
				pDHCPRsvdEntry = &DhcpRsvdEntry[1];
				for( i=1; i<=num;i++ )
				{
					struct CWMP_LINKNODE *remove_entity=NULL;
					memset(pDHCPRsvdEntry, '\0', sizeof(DHCPRSVDIP_T));
					*((char *)pDHCPRsvdEntry) = (char)i;
					if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)pDHCPRsvdEntry ))
						continue;
					else
						memcpy(&DhcpRsvdEntry[0], &DhcpRsvdEntry[1], sizeof(DHCPRSVDIP_T));

					if( (pDHCPRsvdEntry->usedFor == usedFor) &&
							(pDHCPRsvdEntry->dhcpConSPInstNum == dhcpConSPInstNum) ) // &&
							//(pDHCPOptEntry->dhcpOptInstNum!=0))
						{
							remove_entity = remove_SiblingEntity( &old_table, pDHCPRsvdEntry->InstanceNum );
							if( remove_entity!=NULL )
							{
								add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
							}
							else
							{
								if( find_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, pDHCPRsvdEntry->InstanceNum )==NULL )
								{
									unsigned int MaxInstNum = pDHCPRsvdEntry->InstanceNum;
									add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPOptionObject, sizeof(tDHCPOptionObject), &MaxInstNum );
									if(MaxInstNum!=pDHCPRsvdEntry->InstanceNum)
									{
										pDHCPRsvdEntry->InstanceNum = MaxInstNum;
										apmib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpRsvdEntry);
									}
								}//else already in next_table
							}
						}	
				}

				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );

				return 0;
			}
		}
	}
	else
	{
		usedFor = eUsedFor_DHCPServer;
		CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d > (type = %d)\n", __FUNCTION__, __LINE__, type) );
		
		switch(type)
		{
			case eCWMP_tINITOBJ:
			{	
				struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
				int MaxInstNum;
		
				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
				
				MaxInstNum = findMaxDHCPStaticAddrNum(usedFor, dhcpConSPInstNum);
				apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
				pDHCPRsvdEntry = &DhcpRsvdEntry[1];
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPRsvdEntry, '\0', sizeof(DHCPRSVDIP_T));
					*((char *)pDHCPRsvdEntry) = (char)i;
					if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)pDHCPRsvdEntry ))
						continue;
					else
						memcpy(&DhcpRsvdEntry[0], &DhcpRsvdEntry[1], sizeof(DHCPRSVDIP_T));

					if(pDHCPRsvdEntry->usedFor != usedFor )
						continue;
					if( pDHCPRsvdEntry->InstanceNum==0 )  //maybe createn by web or cli
					{
						MaxInstNum++;
						pDHCPRsvdEntry->InstanceNum = MaxInstNum;
						apmib_set( MIB_DHCPRSVDIP_MOD, (void *)&DhcpRsvdEntry);
					}
					if( create_Object( c, tDHCPStaticAddrObject, sizeof(tDHCPStaticAddrObject), 1, pDHCPRsvdEntry->InstanceNum ) < 0 )
						return -1;
				}
				add_objectNum( name, MaxInstNum );
				return 0;
			}
			case eCWMP_tADDOBJ:
			{
				int ret;
					
				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
	
				ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPStaticAddrObject, sizeof(tDHCPStaticAddrObject), data );
				if( ret >= 0 )
				{
					DHCPRSVDIP_T entry;
					memset( &entry, 0, sizeof( DHCPRSVDIP_T ) );
					{ //default values for this new entry
						memset(entry.ipAddr, 0, sizeof(entry.ipAddr));
						memset(entry.macAddr, 0, sizeof(entry.macAddr));
						memset(entry.hostName, 0, sizeof(entry.hostName));
						entry.InstanceNum = *(int *)data;
						entry.dhcpConSPInstNum = 0;
						entry.usedFor = usedFor;
					}
					if ( apmib_set( MIB_DHCPRSVDIP_ADD, (void*)&entry) == 0)
					{
						fprintf(stderr,"\r\n Add DHCPRSVDIP table entry error!");
						return -1;
					}						
				}
				return ret;
			}
			case eCWMP_tDELOBJ:
			{
				int ret, num, i;
				int found = 0;
				unsigned int *pUint=data;
	
				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
				
				apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
				pDHCPRsvdEntry = &DhcpRsvdEntry[1];
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPRsvdEntry, '\0', sizeof(DHCPRSVDIP_T));
					*((char *)pDHCPRsvdEntry) = (char)i;				
					if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)pDHCPRsvdEntry ) )
						continue;
					if(pDHCPRsvdEntry->usedFor == eUsedFor_DHCPServer && pDHCPRsvdEntry->InstanceNum==*pUint)
					{
						found =1;
						//clearOptTbl(pDHCPRsvdEntry->InstanceNum);
		#ifdef SUPPORT_DHCP_RESERVED_IPADDR
					//	clearDHCPReservedIPAddrByInstNum( pDHCPRsvdEntry->InstanceNum );
		#endif //SUPPORT_DHCP_RESERVED_IPADDR
						apmib_set( MIB_DHCPRSVDIP_DEL, (void *)pDHCPRsvdEntry );
						compact_staticAddrNode(usedFor, dhcpConSPInstNum, *pUint);
						//compact_poolorder();
						break;
					}
				}
	
				if(found==0) return ERR_9005;
				ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
#if 0
				apply_DHCP(CWMP_RESTART,0,NULL);
#endif
				return ret;
			}
			case eCWMP_tUPDATEOBJ:
			{
				int num,i;
				struct CWMP_LINKNODE *old_table;
	
				apmib_get( MIB_DHCPRSVDIP_TBL_NUM, (void *)&num );
				old_table = (struct CWMP_LINKNODE *)entity->next;
				entity->next = NULL;
				pDHCPRsvdEntry = &DhcpRsvdEntry[1];
				for( i=1; i<=num;i++ )
				{
					struct CWMP_LINKNODE *remove_entity=NULL;
					memset(pDHCPRsvdEntry, '\0', sizeof(DHCPRSVDIP_T));
					*((char *)pDHCPRsvdEntry) = (char)i;
					if( !apmib_get( MIB_DHCPRSVDIP_TBL, (void*)pDHCPRsvdEntry ))
						continue;
					else
						memcpy(&DhcpRsvdEntry[0], &DhcpRsvdEntry[1], sizeof(DHCPRSVDIP_T));

					if( (pDHCPRsvdEntry->usedFor == usedFor) )
					{
						remove_entity = remove_SiblingEntity( &old_table, pDHCPRsvdEntry->InstanceNum );
						if( remove_entity!=NULL )
						{
							add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
						}
						else
						{
							if( find_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, pDHCPRsvdEntry->InstanceNum )==NULL )
							{
								unsigned int MaxInstNum = pDHCPRsvdEntry->InstanceNum;
								add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPStaticAddrObject, sizeof(tDHCPStaticAddrObject), &MaxInstNum );
								if(MaxInstNum!=pDHCPRsvdEntry->InstanceNum)
								{
									pDHCPRsvdEntry->InstanceNum = MaxInstNum;
									apmib_set( MIB_DHCPRSVDIP_MOD, (void *)&DhcpRsvdEntry );
								}
							}//else already in next_table
						}
					}			
				}
	
				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );
	
				return 0;
			}
		}
	}
	return -1;	
}

/*********************DHCPStaticAddress end*******************************/



/*********************DHCPOption begin*******************************/

unsigned int getDHCPOptInstNum( char *name )
{
	return getInstNum( name, "DHCPOption" );
}

unsigned int getDHCPOptionNums( unsigned int usedFor, unsigned int dhcpConSPInstNum)
{
	unsigned int ret=0, i,num;
	MIB_CE_DHCP_OPTION_T *p,DHCPOption_entity;

	apmib_get(MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num);
	p = &DHCPOption_entity;
	for(i=1; i<=num;i++) {
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;
		if (!apmib_get(MIB_DHCP_SERVER_OPTION_TBL, (void *)p))
			continue;
		if(p->usedFor!=usedFor || dhcpConSPInstNum != p->dhcpConSPInstNum)
			continue;
		ret++;
	}

	return ret;
}


int getDHCPOptionByOptInstNum( unsigned int dhcpOptNum, unsigned int dhcpSPNum, unsigned int usedFor, MIB_CE_DHCP_OPTION_T *p, unsigned int *id )
{
	int ret = -1;
	unsigned int i,num;

	apmib_get(MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num);
	for (i=1; i<=num; i++) {
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;
		if (!apmib_get(MIB_DHCP_SERVER_OPTION_TBL, (void *)p))
			continue;

		if( (p->usedFor==usedFor) && (p->dhcpOptInstNum==dhcpOptNum) && (p->dhcpConSPInstNum==dhcpSPNum) )
		{
			*id = i;
			ret = 0;
			break;
		}
	}
	return 0;
}


unsigned int DHCPServerReservedOption[]={
	53,  //DHCP Message type
	54,  //Server id
	51,  //Leasetime
	1,   //subnet mask
	3,   //router
	6,   //DNS Servers
	15,  //Domain name
	0
};//star: I think some important options can't be changed by user, because they are necessary or are managed by other parameters.

int checkDHCPOptionTag(unsigned int tag, unsigned int dhcpOptNum, unsigned int dhcpSPNum, unsigned int usedFor)
{
	int i,num, found = 0;
	MIB_CE_DHCP_OPTION_T *p;
	MIB_CE_DHCP_OPTION_T entry = {0};
	unsigned int *tmp=DHCPServerReservedOption;
	if(tag > 255) return -1;
	
	while(*tmp!=0){
		if(tag == *tmp)
			return -1;
		tmp++;
	}

	p = &entry;
	apmib_get(MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num);
	for (i=1; i<=num; i++) {
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;
		if (!apmib_get(MIB_DHCP_SERVER_OPTION_TBL, (void *)p))
			continue;

		if( (p->usedFor==usedFor) && (p->dhcpConSPInstNum==dhcpSPNum) )
		{
			if(tag == p->tag){
				found = 1; 
				break;
			}
		}
	}
	if(found == 1){
		if(p->dhcpOptInstNum != dhcpOptNum)
			return -1;
	}

	return 0;
}
/*ping_zhang:20090319 START:replace ip range with serving pool of tr069*/

int getDHCPOptionEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	MIB_CE_DHCP_OPTION_T *pDHCPOptEntry, DhcpOptEntry;
	unsigned int dhcpOptNum,dhcpConSPOptNum;
	unsigned char usedFor;
	unsigned int chainid;

	dhcpOptNum = getDHCPOptInstNum( name );
	dhcpConSPOptNum = getDHCPConSPInstNum( name );

	pDHCPOptEntry = &DhcpOptEntry;
	if(dhcpConSPOptNum != 0)  					//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.	
	{
		usedFor = eUsedFor_DHCPServer_ServingPool;
		if( getDHCPOptionByOptInstNum(dhcpOptNum,dhcpConSPOptNum, usedFor, pDHCPOptEntry, &chainid) < 0 )
		return ERR_9002;
	}
	else if(dhcpConSPOptNum==0 && dhcpOptNum!=0)	//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	{
		usedFor = eUsedFor_DHCPServer;
		if( getDHCPOptionByOptInstNum(dhcpOptNum,dhcpConSPOptNum, usedFor, pDHCPOptEntry, &chainid) < 0)
		return ERR_9002;
	}
	else
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;

	if( strcmp( lastname, "Enable" )==0 )
	{
		if(pDHCPOptEntry->enable)
			*data = booldup(1);
		else
			*data = booldup(0);
	}
	else if( strcmp( lastname, "Tag" )==0 )
	{
		*data = uintdup(pDHCPOptEntry->tag);
	}
	else if( strcmp( lastname, "Value" )==0 )
	{
		struct xsd__base64 tmp;
		tmp.__ptr=pDHCPOptEntry->value;
		tmp.__size=pDHCPOptEntry->len;		
		*data=base64dup( tmp );
	}
	else
	{
		return ERR_9005;
	}
	return 0;
}


int setDHCPOptionEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char *buf = data;
	MIB_CE_DHCP_OPTION_T *pDHCPOptEntry, DhcpOptEntry[2];
	unsigned int dhcpOptNum,dhcpConSPOptNum;
	unsigned char usedFor;
	unsigned int chainid;

	if( (name==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	dhcpOptNum = getDHCPOptInstNum( name );
	dhcpConSPOptNum = getDHCPConSPInstNum( name );

	pDHCPOptEntry = &DhcpOptEntry[0];
	if(dhcpConSPOptNum != 0)  			//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.			
	{
		usedFor = eUsedFor_DHCPServer_ServingPool;
		if( getDHCPOptionByOptInstNum(dhcpOptNum, dhcpConSPOptNum, usedFor, pDHCPOptEntry, &chainid) < 0 )
		return ERR_9002;
	}
	else if(dhcpConSPOptNum==0 && dhcpOptNum!=0)	 //for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	{
		usedFor = eUsedFor_DHCPServer;
		if( getDHCPOptionByOptInstNum(dhcpOptNum, dhcpConSPOptNum, usedFor, pDHCPOptEntry, &chainid) < 0)
		return ERR_9002;
	}
	else
		return ERR_9005;

	memcpy(&DhcpOptEntry[1], &DhcpOptEntry[0], sizeof(MIB_CE_DHCP_OPTION_T));
	pDHCPOptEntry = &DhcpOptEntry[1];
	
	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		pDHCPOptEntry->enable = (*i==0) ? 0:1;
		apmib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpOptEntry);	
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else		
		return 1;				
#endif
	}
	else if( strcmp( lastname, "Tag" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(*i<1 || *i>254) return ERR_9007;
		if(checkDHCPOptionTag(*i,dhcpOptNum, dhcpConSPOptNum, usedFor)<0)  return ERR_9001;
		pDHCPOptEntry->tag = *i;
		apmib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpOptEntry);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;			
#endif
	}
	else if( strcmp( lastname, "Value" )==0 )
	{
		if( buf==NULL ) return ERR_9007;

		if(data)
		{
		   int i;
		   struct xsd__base64 *b=data;
		   fprintf( stderr, "<xsd__base64:size %d>", b->__size );
		   for( i=0; i<b->__size; i++ )
		   {
		    fprintf( stderr, "%u(%c) ", b->__ptr[i], b->__ptr[i]  );
		   }
		   fprintf( stderr, "\n" );
		   if(b->__size>DHCP_OPT_VAL_LEN) return ERR_9001;
		   pDHCPOptEntry->len=b->__size;
		   memcpy(pDHCPOptEntry->value,b->__ptr,b->__size);
		}
		apmib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpOptEntry);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;				
#endif
	}
	else
	{
		return ERR_9005;
	}
	return 0;
}

unsigned int findMaxDHCPOptionEntryNum( unsigned int usedFor, unsigned int dhcpConSPInstNum)
{
	unsigned int ret=0, i,num;
	MIB_CE_DHCP_OPTION_T *p,DHCPOption_entity;

	apmib_get(MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num);
	p = &DHCPOption_entity;
	for(i=1; i<=num;i++) {
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;
		if (!apmib_get(MIB_DHCP_SERVER_OPTION_TBL, (void *)p))
			continue;
		if(p->usedFor!=usedFor || dhcpConSPInstNum != p->dhcpConSPInstNum)
			continue;
		if(p->dhcpOptInstNum>ret)
			ret=p->dhcpOptInstNum;
	}

	return ret;
}

void compact_DHCPOptionNode(int usedFor, int dhcpSPNum, int instanceNum)
{
	int i, num;
	MIB_CE_DHCP_OPTION_T *pDHCPOptionEntry, DHCPOptionEntry[2];
	pDHCPOptionEntry = &DHCPOptionEntry[1];
	
	apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
	for(i = 1; i <= num; i++){
		*(char *)&DHCPOptionEntry[0] = (char)i;
		mib_get(MIB_DHCP_SERVER_OPTION_TBL, (void *)&DHCPOptionEntry[0]);

		printf("[%s:%d]DhcpRsvdEntry[0].InstanceNum = %d\n",__FILE__, __LINE__, DHCPOptionEntry[0].dhcpOptInstNum);
			
		if(DHCPOptionEntry[0].usedFor == usedFor && 
			DHCPOptionEntry[0].dhcpConSPInstNum == dhcpSPNum &&
			DHCPOptionEntry[0].dhcpOptInstNum > instanceNum){
			memcpy(pDHCPOptionEntry, &DHCPOptionEntry[0], sizeof(MIB_CE_DHCP_OPTION_T));
			--pDHCPOptionEntry->dhcpOptInstNum;
			printf("[%s:%d]pDHCPOptionEntry->dhcpOptInstNum = %d\n",__FILE__, __LINE__, pDHCPOptionEntry->dhcpOptInstNum);
					
			mib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)DHCPOptionEntry);
		}
	}
}

int objDHCPOption(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	MIB_CE_DHCP_OPTION_T *pDHCPOptEntry, DhcpOptEntry[2];
	unsigned int dhcpConSPInstNum, usedFor,num,i;
	unsigned int chainid;

	dhcpConSPInstNum = getDHCPConSPInstNum( name );
	if(dhcpConSPInstNum != 0)   //for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.
	{
		usedFor = eUsedFor_DHCPServer_ServingPool;
		switch( type )
		{
		case eCWMP_tINITOBJ:
			{
				int MaxInstNum;
				struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				MaxInstNum = findMaxDHCPOptionEntryNum(usedFor,dhcpConSPInstNum);
				apmib_get(MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num);
				pDHCPOptEntry = &DhcpOptEntry[1];
				for (i=1; i<=num; i++) {
					memset(pDHCPOptEntry, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
					*((char *)pDHCPOptEntry) = (char)i;
					if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void*)pDHCPOptEntry ))
						continue;
					else
						memcpy(&DhcpOptEntry[0], &DhcpOptEntry[1], sizeof(MIB_CE_DHCP_OPTION_T));

					if(pDHCPOptEntry->usedFor != usedFor || pDHCPOptEntry->dhcpConSPInstNum != dhcpConSPInstNum)
						continue;

					if( pDHCPOptEntry->dhcpOptInstNum==0 ) //maybe createn by web or cli
					{
						MaxInstNum++;
						pDHCPOptEntry->dhcpOptInstNum = MaxInstNum;
						apmib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpOptEntry);
					}
					if( create_Object( c, tDHCPOptionObject, sizeof(tDHCPOptionObject), 1, pDHCPOptEntry->dhcpOptInstNum ) < 0 )
						return -1;

				}
				add_objectNum( name, MaxInstNum );
				return 0;
			}
		case eCWMP_tADDOBJ:
			{
				int ret, found=0;
				DHCPS_SERVING_POOL_T *pDHCPSPOptEntry, DhcpSPOptEntry;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
				pDHCPSPOptEntry = &DhcpSPOptEntry;
				for (i=1; i<=num; i++)
				{	
					memset(pDHCPSPOptEntry, '\0', sizeof(DHCPS_SERVING_POOL_T));
					*((char *)pDHCPSPOptEntry) = (char)i;
					if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)pDHCPSPOptEntry ) )
						continue;
					if(pDHCPSPOptEntry->InstanceNum == dhcpConSPInstNum )
					{
						found = 1;
						break;
					}
				}
				if(found ==0) return ERR_9005;
				ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPOptionObject, sizeof(tDHCPOptionObject), data );
				if( ret >= 0 )
				{
					MIB_CE_DHCP_OPTION_T entry;
					memset( &entry, 0, sizeof( MIB_CE_DHCP_OPTION_T ) );
					{ //default values for this new entry
						entry.enable = 0;
						entry.dhcpConSPInstNum = pDHCPSPOptEntry->InstanceNum;
						entry.usedFor = usedFor;
						entry.dhcpOptInstNum =*(int*)data;
					}
					apmib_set( MIB_DHCP_SERVER_OPTION_ADD, (void *)&entry);
				}
				return ret;
			}
		case eCWMP_tDELOBJ:
			{
				int ret, num, i;
				int found = 0;
				unsigned int *pUint=data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
				pDHCPOptEntry = &DhcpOptEntry[1];
				for( i=1;i<=num;i++ )
				{
					memset(pDHCPOptEntry, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
					*((char *)pDHCPOptEntry) = (char)i;					
					if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void*)pDHCPOptEntry ) )
						continue;
					if(pDHCPOptEntry->usedFor == usedFor
						&& pDHCPOptEntry->dhcpConSPInstNum==dhcpConSPInstNum
						&& pDHCPOptEntry->dhcpOptInstNum==*pUint)
					{
						found =1;
						apmib_set( MIB_DHCP_SERVER_OPTION_DEL, (void *)pDHCPOptEntry );
						break;
					}
				}

				if(found==0) return ERR_9005;
				ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
			#if 0
				apply_DHCP(CWMP_RESTART,0,NULL);
			#endif
				//if( ret==0 )	ret=1;
				return ret;
			}
		case eCWMP_tUPDATEOBJ:
			{
				int num,i;
				struct CWMP_LINKNODE *old_table;

				 apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
				old_table = (struct CWMP_LINKNODE *)entity->next;
				entity->next = NULL;
				pDHCPOptEntry = &DhcpOptEntry[1];
				for( i=1; i<=num;i++ )
				{
					struct CWMP_LINKNODE *remove_entity=NULL;				
					memset(pDHCPOptEntry, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
					*((char *)pDHCPOptEntry) = (char)i;
					if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void*)pDHCPOptEntry ))
						continue;
					else
						memcpy(&DhcpOptEntry[0], &DhcpOptEntry[1], sizeof(MIB_CE_DHCP_OPTION_T));

					if( (pDHCPOptEntry->usedFor == usedFor) &&
						(pDHCPOptEntry->dhcpConSPInstNum == dhcpConSPInstNum) ) // &&
						//(pDHCPOptEntry->dhcpOptInstNum!=0))
					{
						remove_entity = remove_SiblingEntity( &old_table, pDHCPOptEntry->dhcpOptInstNum );
						if( remove_entity!=NULL )
						{
							add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
						}
						else
						{
							if( find_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, pDHCPOptEntry->dhcpOptInstNum )==NULL )
							{
								unsigned int MaxInstNum = pDHCPOptEntry->dhcpOptInstNum;
								add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPOptionObject, sizeof(tDHCPOptionObject), &MaxInstNum );
								if(MaxInstNum!=pDHCPOptEntry->dhcpOptInstNum)
								{
									pDHCPOptEntry->dhcpOptInstNum = MaxInstNum;
									apmib_set(MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpOptEntry);
								}
							}//else already in next_table
						}
					}
				}

				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );

				return 0;
			}
		}
	}
	else					//for IGD.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	{
		usedFor = eUsedFor_DHCPServer;
		
		switch( type )
		{
		case eCWMP_tINITOBJ:
			{		
				struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
				int MaxInstNum;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
				MaxInstNum = findMaxDHCPOptionEntryNum(usedFor,0);
				apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
				pDHCPOptEntry = &DhcpOptEntry[1];
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPOptEntry, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
					*((char *)pDHCPOptEntry) = (char)i;
					if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void*)pDHCPOptEntry ))
						continue;
					else
						memcpy(&DhcpOptEntry[0], &DhcpOptEntry[1], sizeof(MIB_CE_DHCP_OPTION_T));

					if(pDHCPOptEntry->usedFor != usedFor )
						continue;
					if( pDHCPOptEntry->dhcpOptInstNum==0 ) //maybe createn by web or cli
					{
						MaxInstNum++;
						pDHCPOptEntry->dhcpOptInstNum = MaxInstNum;
						apmib_set( MIB_DHCP_SERVER_OPTION_MOD, (void*)&DhcpOptEntry );
					}
					if( create_Object( c, tDHCPOptionObject, sizeof(tDHCPOptionObject), 1, pDHCPOptEntry->dhcpOptInstNum ) < 0 )
						return -1;

				}
				add_objectNum( name, MaxInstNum );
				return 0;
			}
		case eCWMP_tADDOBJ:
			{
				int ret;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
				if(num >= MAX_EACH_POOL_DHCP_OPTION_NUM) return ERR_9001;
				
				ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPOptionObject, sizeof(tDHCPOptionObject), data );
				if( ret >= 0 )
				{
					MIB_CE_DHCP_OPTION_T entry;
					memset( &entry, 0, sizeof( MIB_CE_DHCP_OPTION_T ) );
					{ //default values for this new entry
						entry.enable = 0;
						entry.usedFor = usedFor;
						entry.dhcpOptInstNum = *(int*)data;
						entry.dhcpConSPInstNum = 0;
					}
					apmib_set( MIB_DHCP_SERVER_OPTION_ADD, (void*)&entry);
				}
				return ret;
			}
		case eCWMP_tDELOBJ:
			{
				int ret, num, i;
				int found = 0;
				unsigned int *pUint=data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
				pDHCPOptEntry = &DhcpOptEntry[1];
				for( i=1;i<=num;i++ )
				{
					memset(pDHCPOptEntry, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
					*((char *)pDHCPOptEntry) = (char)i;					
					if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void*)pDHCPOptEntry ) )
						continue;
					if(pDHCPOptEntry->usedFor == eUsedFor_DHCPServer && pDHCPOptEntry->dhcpOptInstNum==*pUint)
					{
						found =1;
						apmib_set( MIB_DHCP_SERVER_OPTION_DEL, (void *)pDHCPOptEntry );
						break;
					}
				}

				if(found==0) return ERR_9005;
				ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
				compact_DHCPOptionNode(eUsedFor_DHCPServer, 0, pDHCPOptEntry->dhcpOptInstNum);
				
				return ret;
			}
		case eCWMP_tUPDATEOBJ:
			{
				int num,i;
				struct CWMP_LINKNODE *old_table;

				apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
				old_table = (struct CWMP_LINKNODE *)entity->next;
				entity->next = NULL;
				pDHCPOptEntry = &DhcpOptEntry[1];
				for( i=1; i<=num;i++ )
				{
					struct CWMP_LINKNODE *remove_entity=NULL;
					memset(pDHCPOptEntry, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
					*((char *)pDHCPOptEntry) = (char)i;

					if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void*)pDHCPOptEntry ))
						continue;
					else
						memcpy(&DhcpOptEntry[0], &DhcpOptEntry[1], sizeof(MIB_CE_DHCP_OPTION_T));

					if( (pDHCPOptEntry->usedFor == usedFor) ) //&& (pDHCPOptEntry->dhcpOptInstNum!=0))
					{
						remove_entity = remove_SiblingEntity( &old_table, pDHCPOptEntry->dhcpOptInstNum );
						if( remove_entity!=NULL )
						{
							add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
						}
						else
						{
							if( find_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, pDHCPOptEntry->dhcpOptInstNum )==NULL )
							{
								unsigned int MaxInstNum = pDHCPOptEntry->dhcpOptInstNum;
								add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPOptionObject, sizeof(tDHCPOptionObject), &MaxInstNum );
								if(MaxInstNum!=pDHCPOptEntry->dhcpOptInstNum)
								{
									pDHCPOptEntry->dhcpOptInstNum = MaxInstNum;
									apmib_set( MIB_DHCP_SERVER_OPTION_MOD, (void *)&DhcpOptEntry );
								}
							}//else already in next_table
						}
					}
				}

				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );

				return 0;
			}
		}
	}



	return -1;
}

/*********************DHCPOption end*******************************/



// add function
//---------------------------------------------------------------------------------------
#if 0
unsigned int findDHCPOptionNum(int usedFor, unsigned int ifIndex)
{
	unsigned int ret=0, i,num;
	MIB_CE_DHCP_OPTION_T *p,DHCPOption_entity;
#if 1//tmp
	//num = mib_chain_total( MIB_DHCP_CLIENT_OPTION_TBL );
	apmib_get( MIB_DHCP_CLIENT_OPTION_TBL_NUM, (void *)&num );
	p = &DHCPOption_entity;
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCP_CLIENT_OPTION_TBL, (void*)p ))
			continue;
		if(p->usedFor==usedFor && p->ifIndex==ifIndex)
			ret++;
	}
#else
	num = mib_chain_total( MIB_DHCP_CLIENT_OPTION_TBL );
	for( i=0; i<num;i++ )
	{
		p = &DHCPOption_entity;
		if( !mib_chain_get( MIB_DHCP_CLIENT_OPTION_TBL, i, (void*)p ))
			continue;
		if(p->usedFor==usedFor && p->ifIndex==ifIndex)
			ret++;
	}
#endif
	return ret;
}
#endif
int getATMVCEntry(char *name, MIB_CE_ATM_VC_T *p, unsigned int *idx)
{
	int ret = -1;
	unsigned int i,num, wandev, devcon, ip, ppp;

	if( name == NULL )
		return ret;
#if 0//tmp
	wandev = getWANDevInstNum(name);
	devcon= getWANConDevInstNum(name);
	ip = getWANIPConInstNum(name);
	ppp = getWANPPPConInstNum(name);

	if(wandev == 0 || devcon == 0 || (ip == 0 && ppp == 0))
		return ret;

	num = mib_chain_total( MIB_ATM_VC_TBL );
	for( i=0; i<num;i++ )
	{
		if( !mib_chain_get( MIB_ATM_VC_TBL, i, (void*)p ) )
			continue;

		if(!isWANDevModeMatch(wandev, p->ifIndex))
			continue;

		//fix for multi conppp/ip per ConDevInstNum
		//if((p->ConDevInstNum == devcon) && (p->ConPPPInstNum == ppp || p->ConIPInstNum == ip))
		if((p->ConDevInstNum == devcon) && (p->ConPPPInstNum == ppp) && (p->ConIPInstNum == ip))
		{
			*idx = i;
			ret = 0;
			break;
		}
	}
#endif
	return ret;
}

unsigned int findMaxDHCPReqOptionOrder(void )
{
	unsigned int ret=0, i,num;
	MIB_CE_DHCP_OPTION_T *p,DHCPSP_entity;

	//num = mib_chain_total( MIB_DHCP_CLIENT_OPTION_TBL );
	apmib_get( MIB_DHCP_CLIENT_OPTION_TBL_NUM, (void *)&num );
	p = &DHCPSP_entity;	
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;		
		if( !apmib_get( MIB_DHCP_CLIENT_OPTION_TBL, (void*)p ))
			continue;
		if(p->usedFor!=eUsedFor_DHCPClient_Req)
			continue;
		if(p->order>ret)
			ret=p->order;
	}

	return ret;
}

void clearOptTbl(unsigned int instnum)
{
	unsigned int  i,num,found;
	MIB_CE_DHCP_OPTION_T *p,DHCPOption_entity;

delOpt:
	//num = mib_chain_total( MIB_DHCP_SERVER_OPTION_TBL );
	apmib_get( MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num );
	p = &DHCPOption_entity;
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(MIB_CE_DHCP_OPTION_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCP_SERVER_OPTION_TBL, (void *)p ))
			continue;
		if(p->usedFor!=eUsedFor_DHCPServer_ServingPool)
			continue;
		if(p->dhcpConSPInstNum==instnum){
			apmib_set(MIB_DHCP_SERVER_OPTION_DEL, (void *)p);
			break;
		}
	}
	if(i<num)
		goto delOpt;
	return;

}



unsigned int findMaxDHCPConSPInsNum(void )
{
	unsigned int ret=0, i,num;
	DHCPS_SERVING_POOL_T *p,DHCPSP_entity;

	//num = mib_chain_total( MIB_DHCPS_SERVING_POOL_TBL );
	apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
	p = &DHCPSP_entity;
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(DHCPS_SERVING_POOL_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)p ))
			continue;
		if(p->InstanceNum>ret)
			ret=p->InstanceNum;
	}

	return ret;
}

int getDHCPConSPByInstNum( unsigned int dhcpspNum,  DHCPS_SERVING_POOL_T *p, unsigned int *id )
{
	int ret=-1;
	unsigned int i,num;

	if( (dhcpspNum==0) || (p==NULL) || (id==NULL) )
		return ret;
#if 1//tmp
	//num = mib_chain_total( MIB_DHCPS_SERVING_POOL_TBL );
	apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(DHCPS_SERVING_POOL_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)p ) )
			continue;

		if( p->InstanceNum==dhcpspNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}
#endif
	return ret;
}


unsigned int findMaxDHCPConSPOrder(void )
{
	unsigned int ret=0, i,num;
	DHCPS_SERVING_POOL_T *p,DHCPSP_entity;

	//num = mib_chain_total( MIB_DHCPS_SERVING_POOL_TBL );
	apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
	p = &DHCPSP_entity;
	for( i=1; i<=num;i++ )
	{
		memset(p, '\0', sizeof(DHCPS_SERVING_POOL_T));
		*((char *)p) = (char)i;
		if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)p ))
			continue;
		if(p->poolorder>ret)
			ret=p->poolorder;
	}

	return ret;
}






//---------------------------------------------------------------------------------------


/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#ifdef _PRMT_WT107_
int getLANInterfaces(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "LANEthernetInterfaceNumberOfEntries" )==0 )
	{
	#if 1
		*data = uintdup( 0 );
	#else
		int flags=0;
		if( getInFlags("eth0", &flags)==1 )
		{
			if (flags & IFF_UP)
				*data = uintdup( 0 );
			else
				*data = uintdup(CWMP_LANETHIFNO);
		}else
			return ERR_9002;
	#endif
	}else if( strcmp( lastname, "LANUSBInterfaceNumberOfEntries" )==0 )
	{
		*data = uintdup(0);
	}else if( strcmp( lastname, "LANWLANConfigurationNumberOfEntries" )==0 )
	{
	#if 1
		*data = uintdup( 0 );
	#else
	   #ifdef WLAN_SUPPORT
		int flags=0,i;
		unsigned int count = WLAN_IF_NUM;
		for(i=0; i<WLAN_IF_NUM; i++)
		{
			if( getInFlags(wlan_name[i], &flags)==1 )
			{
				if (flags & IFF_UP)
					count --;
			}else
                                continue;
				//return ERR_9002;
		}
		*data = uintdup(count);
	   #else
		*data = uintdup(WLAN_IF_NUM);
	   #endif
	#endif
	}else{
		return ERR_9005;
	}
	
	return 0;
}
#endif
/*ping_zhang:20081217 END*/



/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
//#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_

/*ping_zhang:20090319 START:replace ip range with serving pool of tr069*/
//move to utility.c
#if 0
unsigned int getSPDHCPOptEntryNum(unsigned int usedFor, unsigned int instnum)
{
	unsigned int ret=0, i,num;
	MIB_CE_DHCP_OPTION_T *p,DHCPOPT_entity;

	num = mib_chain_total( MIB_DHCP_SERVER_OPTION_TBL );
	for( i=0; i<num;i++ )
	{
		p = &DHCPOPT_entity;
		if( !mib_chain_get( MIB_DHCP_SERVER_OPTION_TBL, i, (void*)p ))
			continue;
		if(p->usedFor==usedFor && p->dhcpConSPInstNum==instnum)
			ret++;
	}

	return ret;
}
#endif
/*ping_zhang:20090319 END*/


int getSPDNSList( DHCPS_SERVING_POOL_T *p, char *buf )
{
	unsigned char tmp[64];
	char *zeroip="0.0.0.0";

	if( buf==NULL ) return -1;

	buf[0]=0;
	tmp[0]=0;
	strcpy(tmp,inet_ntoa(*((struct in_addr *)p->dnsserver1)));
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
		strcat( buf, tmp );

	tmp[0]=0;
	strcpy(tmp,inet_ntoa(*((struct in_addr *)p->dnsserver2)));
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
	{
		if( strlen(buf) > 0 )
			strcat( buf, ",");
		strcat( buf, tmp );
	}

	tmp[0]=0;
	strcpy(tmp,inet_ntoa(*((struct in_addr *)p->dnsserver3)));
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
	{
		if( strlen(buf) > 0 )
			strcat( buf, ",");
		strcat( buf, tmp );
	}

	return 0;
}
int setSPDNSList( DHCPS_SERVING_POOL_T *p, char *buf )
{
	char *tok1, *tok2, *tok3;
	int  ret=0;
	struct in_addr in1,in2,in3,emp_in;

	if( buf==NULL ) return -1;
	tok1 = NULL;
	tok2 = NULL;
	tok3 = NULL;

	tok1=strtok( buf, "," );
	tok2=strtok( NULL, "," );
	tok3=strtok( NULL, "," );
	if( (tok1==NULL)&&(tok2==NULL)&&(tok3==NULL) )
		return -1;

	if(tok1)
		if(  inet_aton( tok1, &in1 )==0  ) ret=-1;
	if(tok2)
		if(  inet_aton( tok2, &in2 )==0  ) ret=-1;
	if(tok3)
		if(  inet_aton( tok3, &in3 )==0  ) ret=-1;

	memset( &emp_in, 0, sizeof(struct in_addr) );
	if(ret==0)
	{
		if( tok1!=NULL )
			memcpy(p->dnsserver1, &in1, 4);
		else
			memcpy(p->dnsserver1, &emp_in, 4);

/*ping_zhang:20081104 START:telefonica tr069 new request verify*/
#if 0
		if( tok2!=NULL )
			memcpy(p->dnsserver1, &in2, 4);
		else
			memcpy(p->dnsserver1, &emp_in, 4);

		if( tok3!=NULL )
			memcpy(p->dnsserver1, &in3, 4);
		else
			memcpy(p->dnsserver1, &emp_in, 4);
#else
		if( tok2!=NULL )
			memcpy(p->dnsserver2, &in2, 4);
		else
			memcpy(p->dnsserver2, &emp_in, 4);

		if( tok3!=NULL )
			memcpy(p->dnsserver3, &in3, 4);
		else
			memcpy(p->dnsserver3, &emp_in, 4);
#endif
/*ping_zhang:20081104 END*/
	}
	return ret;
}


int checkandmodify_poolorder(unsigned int order, int chainid)
{
	int ret=-1;
	int num,i;
	int maxorder;
	DHCPS_SERVING_POOL_T *p,pentry[2];

	p=&pentry[1];
	maxorder=findMaxDHCPConSPOrder();
	if(order>maxorder+1)
		goto checkresult;
	else{
		apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
		for( i=1; i<=num;i++ )
		{
			memset(p, '\0', sizeof(DHCPS_SERVING_POOL_T));
			*((char *)p) = (char)i;		
			if(i==chainid)
				continue;
			if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)p ))
				continue;
			else
				memcpy(&pentry[0], &pentry[1], sizeof(DHCPS_SERVING_POOL_T));

			if(p->poolorder>=order){
				(p->poolorder)++;
				apmib_set(MIB_DHCPS_SERVING_POOL_MOD,(void*)&pentry);
			}
		}
		ret=0;
	}

checkresult:
	return ret;
}

void compact_poolorder( )
{
	int ret=-1;
	int num,i,j;
	int maxorder;
	DHCPS_SERVING_POOL_T *p,pentry[2];
	char *orderflag;

	while(1){
		p=&pentry[1];
		maxorder=findMaxDHCPConSPOrder();
		orderflag=(char*)malloc(maxorder+1);
		if(orderflag==NULL) return;
		memset(orderflag,0,maxorder+1);

		apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
		for( i=1; i<=num;i++ )
		{
			memset(p, '\0', sizeof(DHCPS_SERVING_POOL_T));
			*((char *)p) = (char)i;
			if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)p ))
				continue;
			orderflag[p->poolorder]=1;
		}
		for(j=1;j<=maxorder;j++){
			if(orderflag[j]==0)
				break;
		} //star: there only one 0 in orderflag array
		if(j==(maxorder+1))
			break;
		for( i=1; i<=num;i++ )
		{
			memset(p, '\0', sizeof(DHCPS_SERVING_POOL_T));
			*((char *)p) = (char)i;		
			if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)p ))
				continue;
			else			
				memcpy(&pentry[0], &pentry[1], sizeof(DHCPS_SERVING_POOL_T));

			if(p->poolorder>j){
				(p->poolorder)--;
				apmib_set(MIB_DHCPS_SERVING_POOL_MOD,(void*)&pentry);
			}
		}

		if(orderflag)
		{
			free(orderflag);
			orderflag=NULL;
		}
	}

}
static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

static int string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}
int isDigitalStr(char *str)
{   
    int len = strlen(str);
    char *s = str;
    int i = 0;
 
    while( '0' <= *s && *s <= '9' && i < len){
        s++;
        i++;
    }
 
    if(i == len)
        return 1;
    else 
        return 0;       
}

int compactmacaddr(char *newmac, char* oldmac){
	int i,maclen;
	char *p;
	char tmp[10];
	unsigned char vChar;

	maclen=strlen(oldmac);

	p=strtok(oldmac,":");
	while(p){
		strcat(newmac,p);
		p=strtok(NULL,":");
	}

	printf("newmac=%s,oldmac=%s",newmac,oldmac);
	return 1;
}


int getDHCPConSPEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;;
	unsigned int chainid;
	unsigned int dhcpConSPInstNum;
	int num,i;
	int ret=0;
	char buf[256];
	DHCPS_SERVING_POOL_T *pDHCPSPEntry, DhcpSPEntry;

	*type = entity->info->type;
	*data = NULL;
		
	pDHCPSPEntry = &DhcpSPEntry;
	dhcpConSPInstNum = getDHCPConSPInstNum( name );
	ret=getDHCPConSPByInstNum(dhcpConSPInstNum,pDHCPSPEntry,&chainid);
	if(ret<0) return ERR_9002;

	if( strcmp( lastname, "Enable" )==0 )
	{
		*data = booldup(pDHCPSPEntry->enable);
	}else if( strcmp( lastname, "PoolOrder" )==0 )
	{
		*data = uintdup(pDHCPSPEntry->poolorder);
	}else if( strcmp( lastname, "SourceInterface" )==0 )
	{
		char qstr[302];
		char focr = 0;

		qstr[0] = 0;
			
		if(!pDHCPSPEntry->sourceinterface)
			*data = strdup("");
		else
		{
			strcpy(buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.");

			for( i = 0; i < 4; i++ )
			{
				if((pDHCPSPEntry->sourceinterface >> i) & 1)
				{
					sprintf(qstr, "%s%s%s%d", qstr, focr? ",": "", buf, i+1);
					focr = 1;
				}
			}

#ifdef WLAN_SUPPORT
#define MAX_WLAN_NUM 12
			strcpy(buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.");

			for(i = 0; i < MAX_WLAN_NUM; i++)//dual band, max num: 12
			{
				if((pDHCPSPEntry->sourceinterface >> (4+i)) & 1){
					sprintf(qstr, "%s%s%s%d", qstr, focr? ",": "", buf, i+1);
					focr = 1;
				}				
			}
#endif

			*data = strdup(qstr);
		}
	}else if( strcmp( lastname, "VendorClassID" )==0 )
	{
		*data = strdup(pDHCPSPEntry->vendorclass);
	}else if( strcmp( lastname, "VendorClassIDExclude" )==0 )
	{
		*data = booldup(pDHCPSPEntry->vendorclassflag);
	}else if( strcmp( lastname, "VendorClassIDMode" )==0 )
	{
		*data = strdup(pDHCPSPEntry->vendorclassmode);
	}else if( strcmp( lastname, "ClientID" )==0 )
	{
		*data = strdup(pDHCPSPEntry->clientid);
	}else if( strcmp( lastname, "ClientIDExclude" )==0 ){
		*data = booldup(pDHCPSPEntry->clientidflag);
	}else if( strcmp( lastname, "UserClassID" )==0 )
	{
		*data = strdup(pDHCPSPEntry->userclass);
	}else if( strcmp( lastname, "UserClassIDExclude" )==0 )
	{
		*data = booldup(pDHCPSPEntry->userclassflag);
	}else if( strcmp( lastname, "Chaddr" )==0 )
	{
		//00:00:00:00:00:00 returns an empty string, not used for conditional serving
		if( (pDHCPSPEntry->chaddr[0]==0) &&
			(pDHCPSPEntry->chaddr[1]==0) &&
			(pDHCPSPEntry->chaddr[2]==0) &&
			(pDHCPSPEntry->chaddr[3]==0) &&
			(pDHCPSPEntry->chaddr[4]==0) &&
			(pDHCPSPEntry->chaddr[5]==0) )
		  	buf[0]=0;
		else
			sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pDHCPSPEntry->chaddr[0],pDHCPSPEntry->chaddr[1],pDHCPSPEntry->chaddr[2],
				pDHCPSPEntry->chaddr[3],pDHCPSPEntry->chaddr[4],pDHCPSPEntry->chaddr[5]);
		*data = strdup(buf);
	}else if( strcmp( lastname, "ChaddrMask" )==0 )
	{
		sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pDHCPSPEntry->chaddrmask[0],pDHCPSPEntry->chaddrmask[1],pDHCPSPEntry->chaddrmask[2],
			pDHCPSPEntry->chaddrmask[3],pDHCPSPEntry->chaddrmask[4],pDHCPSPEntry->chaddrmask[5]);
		*data = strdup(buf);
	}else if( strcmp( lastname, "ChaddrExclude" )==0 )
	{
		*data = booldup(pDHCPSPEntry->chaddrflag);
	}else if( strcmp( lastname, "LocallyServed" )==0 )
	{
		*data = booldup(pDHCPSPEntry->localserved);
	}else if( strcmp( lastname, "MinAddress" )==0 )
	{
		strcpy(buf,inet_ntoa(*((struct in_addr *)pDHCPSPEntry->startaddr)));
		*data=strdup( buf );
	}else if( strcmp( lastname, "MaxAddress" )==0 )
	{
		strcpy(buf,inet_ntoa(*((struct in_addr *)pDHCPSPEntry->endaddr)));
		*data=strdup( buf );
	}else if( strcmp( lastname, "ReservedAddresses" )==0 )
	{
#ifdef SUPPORT_RESERVED_DHCP_IPADDR
		if( getDHCPReservedIPAddr(buf, pDHCPSPEntry->reservedDhcpIp)< 0)
			*data = strdup("");
		else
			*data = strdup(buf);
#else
		*data = strdup("");
#endif
	}else if( strcmp( lastname, "SubnetMask" )==0 )
	{
		strcpy(buf,inet_ntoa(*((struct in_addr *)pDHCPSPEntry->subnetmask)));
		*data=strdup( buf );
	}else if( strcmp( lastname, "DNSServers" )==0 )
	{
		if( pDHCPSPEntry->dnsservermode==0 ) //automatically attain DNS
			*data=strdup("");
		else
		{
			getSPDNSList(pDHCPSPEntry,buf);
			*data = strdup( buf );
		}
	}else if( strcmp( lastname, "DomainName" )==0 )
	{
		*data=strdup( pDHCPSPEntry->domainname );
	}else if( strcmp( lastname, "IPRouters" )==0 )
	{
		strcpy(buf,inet_ntoa(*((struct in_addr *)pDHCPSPEntry->iprouter)));
		*data=strdup( buf );
	}else if( strcmp( lastname, "DHCPLeaseTime" )==0 )
	{
		*data=intdup((pDHCPSPEntry->leasetime)*60);
	}else if( strcmp( lastname, "UseAllocatedWAN" )==0 )
	{
		*data=strdup( "Normal" );
	}else if( strcmp( lastname, "AssociatedConnection" )==0 )
	{
		*data=strdup( "" );
	}else if( strcmp( lastname, "DHCPServerIPAddress" )==0 )
	{
		strcpy(buf,inet_ntoa(*((struct in_addr *)pDHCPSPEntry->dhcprelayip)));
		*data=strdup( buf );
	}else if( strcmp( lastname, "DHCPStaticAddressNumberOfEntries" )==0 )
	{		
		unsigned int usedFor=eUsedFor_DHCPServer_ServingPool;
		*data = uintdup(getDHCPStaticAddrNums(usedFor, dhcpConSPInstNum));
	}else if( strcmp( lastname, "DHCPOptionNumberOfEntries" )==0 )
	{
		unsigned int usedFor=eUsedFor_DHCPServer_ServingPool;
		*data = uintdup(getDHCPOptionNums(usedFor, dhcpConSPInstNum));
	}else
	{
		return ERR_9005;
	}
	return 0;
}

int setDHCPConSPEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	unsigned int chainid;
	unsigned int dhcpConSPInstNum;
	int num,i;
	int ret=0;
	char *buf=data;
	DHCPS_SERVING_POOL_T *pDHCPSPEntry, DhcpSPEntry[2];
	char tmpbuf[30]={0};
	char *tok, del[] = ", ", *pstr;

	if( (name==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	pDHCPSPEntry = &DhcpSPEntry[1];
	dhcpConSPInstNum = getDHCPConSPInstNum( name );
	ret=getDHCPConSPByInstNum(dhcpConSPInstNum,pDHCPSPEntry,&chainid);
	if(ret<0) return ERR_9002;

	memcpy(&DhcpSPEntry[0], &DhcpSPEntry[1], sizeof(DHCPS_SERVING_POOL_T));
	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		pDHCPSPEntry->enable = (*i==0) ? 0:1;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
		
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_L, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "PoolOrder" )==0 )
	{
		unsigned int *i=data;
		if(i==NULL) return ERR_9007;
		if(*i<1) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		if(checkandmodify_poolorder(*i,chainid)<0) return ERR_9007;
		pDHCPSPEntry->poolorder = *i;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
		compact_poolorder();
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "SourceInterface" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;

		if( strlen(buf)==0 )
		{
			pDHCPSPEntry->sourceinterface = 0;
#ifdef _CWMP_APPLY_
			apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
			return 0;
#else
			return 1;
#endif
		}

		pDHCPSPEntry->sourceinterface = 0;
		
		for( tok = strtok(buf, del); tok; tok = strtok(NULL, del) )
		{

			if(pstr = strstr(tok, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig."))
			{
				pstr += 61;  // Length of "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig."

				if( *pstr < '1' || *pstr > '4' )
					return ERR_9007;
			
				pDHCPSPEntry->sourceinterface |= 1 << (*pstr - '0' - 1);
			}
#ifdef WLAN_SUPPORT
#define MAX_WLAN_NUM 12
			else if(pstr = strstr(tok, "InternetGatewayDevice.LANDevice.1.WLANConfiguration."))
			{
				pstr += strlen("InternetGatewayDevice.LANDevice.1.WLANConfiguration.");
				
				if( !isDigitalStr(pstr) )
					return ERR_9007;
				
				if(atoi(pstr) < 1 || atoi(pstr) > MAX_WLAN_NUM )
					return ERR_9007;
				
				pDHCPSPEntry->sourceinterface |= 0x10 << (atoi(pstr) - 1);
			}
#endif
		else
				return ERR_9007;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
		
		//printf("source interface=%d%d%d%d%d\n", (pDHCPSPEntry->sourceinterface & 0x10) >> 4, (pDHCPSPEntry->sourceinterface & 0x8) >> 3,
		//	(pDHCPSPEntry->sourceinterface & 0x4) >> 2, (pDHCPSPEntry->sourceinterface & 0x2) >> 1,	pDHCPSPEntry->sourceinterface & 0x1);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "VendorClassID" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
/*ping_zhang:20090319 START:replace ip range with serving pool of tr069*/
#if 0
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->vendorclass,0,OPTION_LEN);
		else{
			strncpy(pDHCPSPEntry->vendorclass,buf,OPTION_LEN-1);
			pDHCPSPEntry->vendorclass[OPTION_LEN-1]=0;
		}
#else
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->vendorclass,0,OPTION_60_LEN+1);
		else{
			strncpy(pDHCPSPEntry->vendorclass,buf,OPTION_60_LEN);
			pDHCPSPEntry->vendorclass[OPTION_60_LEN]=0;
		}
#endif
/*ping_zhang:20090319 END*/
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "VendorClassIDExclude" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		pDHCPSPEntry->vendorclassflag = (*i==0) ? 0:1;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "VendorClassIDMode" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		if(strcmp(buf,"Exact")&&strcmp(buf,"Prefix")&&strcmp(buf,"Suffix")&&strcmp(buf,"Substring"))
			return ERR_9007;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->vendorclassmode,0,MODE_LEN);
		else{
			strncpy(pDHCPSPEntry->vendorclassmode,buf,MODE_LEN-1);
			pDHCPSPEntry->vendorclassmode[MODE_LEN-1]=0;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "ClientID" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->clientid,0,OPTION_LEN);
		else{
			strncpy(pDHCPSPEntry->clientid,buf,OPTION_LEN-1);
			pDHCPSPEntry->clientid[OPTION_LEN-1]=0;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "ClientIDExclude" )==0 ){
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		pDHCPSPEntry->clientidflag = (*i==0) ? 0:1;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "UserClassID" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->userclass,0,OPTION_LEN);
		else{
			strncpy(pDHCPSPEntry->userclass,buf,OPTION_LEN-1);
			pDHCPSPEntry->userclass[OPTION_LEN-1]=0;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "UserClassIDExclude" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		pDHCPSPEntry->userclassflag = (*i==0) ? 0:1;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "Chaddr" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		//00:00:00:00:00:00 or an empty string means "not used for conditional serving"
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->chaddr,0,MAC_ADDR_LEN);
		else{
			if( strlen(buf)!=17) return ERR_9007;
			if(compactmacaddr(tmpbuf,buf)==0) return ERR_9007;
			if(string_to_hex(tmpbuf,pDHCPSPEntry->chaddr,12)==0) return ERR_9007;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "ChaddrMask" )==0 )
	{
		if( buf==NULL ) return ERR_9007;	
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		
		//FF:FF:FF:FF:FF:FF or an empty string indicates all bits of the Chaddr are to be used for conditional serving classification
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->chaddrmask,0xff,MAC_ADDR_LEN);
		else{
			if(strlen(buf)!=17) return ERR_9007;
			if(compactmacaddr(tmpbuf,buf)==0) return ERR_9007;
			if(string_to_hex(tmpbuf,pDHCPSPEntry->chaddrmask,12)==0) return ERR_9007;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "ChaddrExclude" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		pDHCPSPEntry->chaddrflag = (*i==0) ? 0:1;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "LocallyServed" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;		
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		pDHCPSPEntry->localserved = (*i==0) ? 0:1;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "MinAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;		
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->startaddr,0,IP_ADDR_LEN);
		else
			if(!inet_aton(buf, (struct in_addr *)&pDHCPSPEntry->startaddr)) return ERR_9007;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "MaxAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;		
		if(pDHCPSPEntry->enable == 0) return ERR_9001;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->endaddr,0,IP_ADDR_LEN);
		else
			if(!inet_aton(buf, (struct in_addr *)&pDHCPSPEntry->endaddr)) return ERR_9007;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "ReservedAddresses" )==0 )
	{		
		if( buf==NULL ) return ERR_9007;		
		if(pDHCPSPEntry->enable == 0) return ERR_9001;		
#ifdef SUPPORT_RESERVED_DHCP_IPADDR
		if( check_conf_poor_reserveIP(buf, pDHCPSPEntry) < 0) return ERR_9007;
		if( setDHCPReservedIPAddr(pDHCPSPEntry->reservedDhcpIp, buf)< 0) return ERR_9007;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
#else
		return ERR_9001;
#endif
	}else if( strcmp( lastname, "SubnetMask" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->subnetmask,0,IP_ADDR_LEN);
		else
			if(!inet_aton(buf, (struct in_addr *)&pDHCPSPEntry->subnetmask)) return ERR_9007;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "DNSServers" )==0 )
	{
		char *tok;
		if( (buf==NULL) || (strlen(buf)==0) )
		{ 	//automatically attain DNS
			pDHCPSPEntry->dnsservermode=0;
		}else if( setSPDNSList( pDHCPSPEntry, buf ) == 0 )
		{
			pDHCPSPEntry->dnsservermode=1;
		}else
			 return ERR_9007;
		
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "DomainName" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->domainname,0,GENERAL_LEN);
		else{
			strncpy(pDHCPSPEntry->domainname,buf,GENERAL_LEN-1);
			pDHCPSPEntry->domainname[GENERAL_LEN-1]=0;
		}
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "IPRouters" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->iprouter,0,IP_ADDR_LEN);
		else
			if(!inet_aton(buf, (struct in_addr *)&pDHCPSPEntry->iprouter)) return ERR_9007;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "DHCPLeaseTime" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(*i<-1) return ERR_9007;
		pDHCPSPEntry->leasetime= (*i)/60;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "UseAllocatedWAN" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "Normal" )!=0 ) return ERR_9007;
		return 0;
	}else if( strcmp( lastname, "AssociatedConnection" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		//if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "" )!=0 ) return ERR_9001;
		return 0;
	}else if( strcmp(lastname, "DHCPServerIPAddress") == 0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0)
			memset(pDHCPSPEntry->dhcprelayip,0,IP_ADDR_LEN);
		else
			if(!inet_aton(buf, (struct in_addr *)&pDHCPSPEntry->dhcprelayip)) return ERR_9007;
		apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void*)&DhcpSPEntry );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DHCP, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}else
	{
		return ERR_9005;
	}
	return 0;
}


void compact_DHCPCOnfSPNode(int instanceNum)
{
	int i, num;
	DHCPS_SERVING_POOL_T *pDHCPSPEntry, DhcpSPEntry[2];
	pDHCPSPEntry = &DhcpSPEntry[1];

	for( i=1; i<=num;i++ )
	{
		memset(pDHCPSPEntry, '\0', sizeof(DHCPS_SERVING_POOL_T));
		*((char *)pDHCPSPEntry) = (char)i;
		if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)pDHCPSPEntry ))
			continue;
		else
			memcpy(&DhcpSPEntry[0], &DhcpSPEntry[1], sizeof(DHCPS_SERVING_POOL_T));

		if( pDHCPSPEntry->InstanceNum > instanceNum )
		{
			pDHCPSPEntry->InstanceNum--;
			pDHCPSPEntry->poolorder--;
			apmib_set(MIB_DHCPS_SERVING_POOL_MOD, (void *)&DhcpSPEntry);
		}
	}

}

int objDHCPConSP(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	DHCPS_SERVING_POOL_T *pDHCPSPEntry, DhcpSPEntry[2];
	unsigned int chainid;
	unsigned int num,i;
	pDHCPSPEntry = &DhcpSPEntry[1];

	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d>(type = %d)\n", __FUNCTION__, __LINE__, type ) );

	switch(type)
		{
		case eCWMP_tINITOBJ:
			{
				int MaxInstNum;
				struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				MaxInstNum = findMaxDHCPConSPInsNum();
				apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPSPEntry, '\0', sizeof(DHCPS_SERVING_POOL_T));
					*((char *)pDHCPSPEntry) = (char)i;
					if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)pDHCPSPEntry ))
						continue;
					else
						memcpy(&DhcpSPEntry[0], &DhcpSPEntry[1], sizeof(DHCPS_SERVING_POOL_T));
	
					if( pDHCPSPEntry->InstanceNum==0 )
					{
						MaxInstNum++;
						pDHCPSPEntry->InstanceNum = MaxInstNum;
						apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void *)&DhcpSPEntry);
					}
					if( create_Object( c, tDHCPConSPObject, sizeof(tDHCPConSPObject), 1, pDHCPSPEntry->InstanceNum ) < 0 )
						return -1;
				}
				add_objectNum( name, MaxInstNum );
				return 0;
			}
		case eCWMP_tADDOBJ:
			{
				int ret;
				char tmpbuf[128];

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
				apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
				if(num >= MAX_DHCPS_SERVING_POOL_NUM) return ERR_9001;

				ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPConSPObject, sizeof(tDHCPConSPObject), data );
				if( ret >= 0 )
				{
					DHCPS_SERVING_POOL_T entry;
					memset( &entry, 0, sizeof( DHCPS_SERVING_POOL_T ) );
					{ //default values for this new entry
						entry.enable = 0;
						entry.poolorder = findMaxDHCPConSPOrder() + 1;
						sprintf(tmpbuf,"servingpool%d",*(int*)data);
						strcpy(entry.poolname,tmpbuf);
						entry.leasetime=86400/60;
						entry.InstanceNum = *(int *)data;
						entry.localserved = 1;//default: locallyserved=true;
						memset(entry.chaddrmask,0xff,MAC_ADDR_LEN);//default to all 0xff
						strncpy(entry.vendorclassmode,"Substring",MODE_LEN-1);
						entry.vendorclassmode[MODE_LEN-1]=0;
					}
					if ( apmib_set( MIB_DHCPS_SERVING_POOL_ADD, (void*)&entry) == 0)
					{
						fprintf(stderr,"\r\n Add DHCPS_SERVING_POOL table entry error!");
						return -1;
					}						
				}
				return ret;
			}
		case eCWMP_tDELOBJ:
			{
				int ret, num, i;
				int found = 0;
				unsigned int *pUint=data;

				if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

				apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
				pDHCPSPEntry = &DhcpSPEntry[1];
				for( i=1; i<=num;i++ )
				{
					memset(pDHCPSPEntry, '\0', sizeof(DHCPS_SERVING_POOL_T));
					*((char *)pDHCPSPEntry) = (char)i;				
					if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)pDHCPSPEntry ) )
						continue;
					if(pDHCPSPEntry->InstanceNum==*pUint)
					{
						found =1;
						clearOptTbl(pDHCPSPEntry->InstanceNum);
					#ifdef SUPPORT_DHCP_RESERVED_IPADDR
					//	clearDHCPReservedIPAddrByInstNum( pDHCPSPEntry->InstanceNum );
					#endif //SUPPORT_DHCP_RESERVED_IPADDR
						apmib_set( MIB_DHCPS_SERVING_POOL_DEL, (void *)pDHCPSPEntry );
						//compact_poolorder();
						break;
					}
				}

				if(found==0) return ERR_9005;
				ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
				compact_DHCPCOnfSPNode(pDHCPSPEntry->InstanceNum);
				if(ret == 0)
					ret = 1;
				return ret;
			}
		case eCWMP_tUPDATEOBJ:
			{
				int num,i;
				struct CWMP_LINKNODE *old_table;

				apmib_get( MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&num );
				old_table = (struct CWMP_LINKNODE *)entity->next;
				entity->next = NULL;
				pDHCPSPEntry = &DhcpSPEntry[1];
				for( i=1; i<=num;i++ )
				{
					struct CWMP_LINKNODE *remove_entity=NULL;
					memset(pDHCPSPEntry, '\0', sizeof(DHCPS_SERVING_POOL_T));
					*((char *)pDHCPSPEntry) = (char)i;
					if( !apmib_get( MIB_DHCPS_SERVING_POOL_TBL, (void*)pDHCPSPEntry ))
						continue;
					else
						memcpy(&DhcpSPEntry[0], &DhcpSPEntry[1], sizeof(DHCPS_SERVING_POOL_T));

					remove_entity = remove_SiblingEntity( &old_table, pDHCPSPEntry->InstanceNum );
					if( remove_entity!=NULL )
					{
							add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
					}
					else
					{
							if( find_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, pDHCPSPEntry->InstanceNum )==NULL )
							{
								unsigned int MaxInstNum = pDHCPSPEntry->InstanceNum;
								add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tDHCPConSPObject, sizeof(tDHCPConSPObject), &MaxInstNum );
								if(MaxInstNum!=pDHCPSPEntry->InstanceNum)
								{
									pDHCPSPEntry->InstanceNum = MaxInstNum;
									apmib_set( MIB_DHCPS_SERVING_POOL_MOD, (void *)&DhcpSPEntry );
								}
							}//else already in next_table
					}
				}

				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );

				return 0;
			}
		}
}
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_

int getLDEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "LANEthernetInterfaceNumberOfEntries" )==0 )
	{
		*data = uintdup(CWMP_LANETHIFNO);
	}else if( strcmp( lastname, "LANUSBInterfaceNumberOfEntries" )==0 )
	{
		*data = uintdup(CWMP_LANUSBIFNO);
	}else if( strcmp( lastname, "LANWLANConfigurationNumberOfEntries" )==0 )
	{
#ifdef WLAN_SUPPORT
		int num, i;
		CWMP_WLANCONF_T *pwlanConf, wlanconf_entity;
		int wlanCount = 0;		
		mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);		
		pwlanConf = &wlanconf_entity;
		for( i=1; i<=num;i++ )
		{
			*((char *)pwlanConf) = (char)i;
			if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
				continue;

			if(pwlanConf->IsConfigured == 1 ) //skip root interface
				wlanCount++;
		}
	
		*data = uintdup( wlanCount );
#else
		*data = uintdup( 0 );
#endif		
	}
	else
	{
		return ERR_9005;
	}
	
	return 0;
}


/**************************************************************************************/
/* utility functions*/
/**************************************************************************************/
int getPid(char *filename)
{
	struct stat status;
	char buff[100];
	FILE *fp;

	if ( stat(filename, &status) < 0)
		return -1;
	fp = fopen(filename, "r");
	if (!fp) {
        	error(__FILE__, __LINE__, 0x2, "Read pid file error!\n");
		return -1;
   	}
	fgets(buff, 100, fp);
	fclose(fp);

	return (atoi(buff));
}
/*copy from fmmgmt.c in boa dir*/
static char *get_name(char *name, char *p)
{
    while (isspace(*p))
	p++;
    while (*p) {
	if (isspace(*p))
	    break;
	if (*p == ':') {	/* could be an alias */
	    char *dot = p, *dotname = name;
	    *name++ = *p++;
	    while (isdigit(*p))
		*name++ = *p++;
	    if (*p != ':') {	/* it wasn't, backup */
		p = dot;
		name = dotname;
	    }
	    if (*p == '\0')
		return NULL;
	    p++;
	    break;
	}
	*name++ = *p++;
    }
    *name++ = '\0';
    return p;
}

int getInterfaceStat(
	char *ifname,
	unsigned long *bs,
	unsigned long *br,
	unsigned long *ps,
	unsigned long *pr )
{
	int	ret=-1;
	FILE 	*fh;
	char 	buf[512];
	unsigned long rx_pkt, rx_err, rx_drop, tx_pkt, tx_err, tx_drop;

	if( (bs==NULL) || (br==NULL) || (ps==NULL) || (pr==NULL) )	return ret;
	*bs=0; *br=0; *ps=0; *pr=0;
	
	fh = fopen("/proc/net/dev", "r");
	if (!fh) return ret;
	
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);
	while (fgets(buf, sizeof buf, fh))
	{
		char *s, name[128];
		s = get_name(name, buf);    
		if (!strcmp(ifname, name)) {
			sscanf(s,
			"%lu %lu %*lu %*lu %*lu %*lu %*lu %*lu %lu %lu %*lu %*lu %*lu %*lu %*lu %*lu",
			br, pr, bs, ps);
			ret=0;
			break;
		}
	}
	fclose(fh);
	return ret;
}

/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
int getInterfaceStat1(
	char *ifname,
	unsigned long *es, unsigned long *er,
	unsigned long *ups, unsigned long *upr, 
	unsigned long *dps, unsigned long *dpr, 
	unsigned long *mps, unsigned long *mpr, 
	unsigned long *bps, unsigned long *bpr, 
	unsigned long *uppr)
{
	int	ret=-1;
	FILE 	*fh;
	char 	buf[512];
	unsigned long rx_pkt, rx_err, rx_drop, tx_pkt, tx_err, tx_drop;

	if( (es==NULL) || (er==NULL) || (ups==NULL) || (upr==NULL) 
		|| (dps==NULL) || (dpr==NULL) || (mps==NULL) || (mpr==NULL)
		|| (bps==NULL) || (bpr==NULL) || (uppr==NULL))	
		return ret;
	*es=0; *er=0; *ups=0; *upr=0;
	*dps=0; *dpr=0; *mps=0; *mpr=0;
	*bps=0; *bpr=0; *uppr=0;
	
	fh = fopen("/proc/net/dev", "r");
	if (!fh) return ret;
	
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);
	while (fgets(buf, sizeof buf, fh))
	{
		char *s, name[128];
		s = get_name(name, buf);    
		if (!strcmp(ifname, name)) {
			sscanf(s,
			"%*lu %*lu %lu %lu %*lu %*lu %*lu %lu %*lu %*lu %lu %lu %*lu %*lu %*lu %*lu",
			er, dpr, mpr,es, dps);
			ret=0;
			break;
		}
	}
	fclose(fh);
	return ret;
}

//#endif
/*ping_zhang:20081217 END*/
/*
int getMIBDefaultValue( int id, void *data )
{
	int i;
	
	if( data==NULL ) return -1;
	
	for (i=0; mib_table[i].id; i++)
		if ( mib_table[i].id == id ) break;

	return 0;
}
*/






#ifdef SECONDARY_IP
unsigned int getIPItfInstNum( char *name )
{
	return getInstNum( name, "IPInterface" );
}
#endif //SECONDARY_IP




