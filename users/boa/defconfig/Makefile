ifeq ($(wildcard $(DIR_LINUX)/.config),)
include ../../../linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../../.config
else
include $(DIR_USERS)/.config
endif

.PHONY: clean all

#------------------------------------------------------------------------------
# CONFIG_FILE and CONFIG_DAT
#------------------------------------------------------------------------------
# pocket AP and biwlan must defined after 96C and 98, 
# otherwise the CONFIG_FILE and CONFIG_DAT values are wrong.
CONFIG_FILE = 
CONFIG_DAT = 
DUAL_BAND_SUPPORT = n

ifeq ($(CONFIG_RTL_8196C_GW),y)
CONFIG_FILE = config-gw-96c.txt
CONFIG_DAT  = config-gw-96c.dat
endif

ifeq ($(CONFIG_RTL_8198_GW),y)
CONFIG_FILE = config-gw-98.txt
CONFIG_DAT  = config-gw-98.dat
endif

ifeq ($(CONFIG_DOMAIN_NAME_QUERY_SUPPORT),y)
CONFIG_FILE = config-gw-96c-pocket-domain.txt
CONFIG_DAT  = config-gw-96c-pocket-domain.dat
endif

#ifeq ($(CONFIG_RTL_DUAL_PCIESLOT_BIWLAN),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_1),y)
ifeq ($(CONFIG_SLOT_0_92C),y)
ifeq ($(CONFIG_SLOT_1_92C),y)
CONFIG_FILE = config-gw-98-biwlan.txt
CONFIG_DAT  = config-gw-98-biwlan.dat
endif
endif
endif
endif

ifeq ($(CONFIG_RTL_8198_GW),y)
ifeq ($(CONFIG_RTL_92D_DMDP),y)
CONFIG_FILE = config-gw-98-92d.txt 
CONFIG_DAT  = config-gw-98-92d.dat
endif
endif

#ifeq ($(CONFIG_RTL_DUAL_PCIESLOT_BIWLAN_D), y)
ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_1),y)
ifeq ($(CONFIG_RTL_92D_SUPPORT),y)
CONFIG_FILE = config-gw-98-92c-92d.txt
CONFIG_DAT  = config-gw-98-92c-92d.dat
endif
endif
endif

ifeq ($(CONFIG_RTL_8198_AP_ROOT),y)
ifeq ($(CONFIG_RTL_92D_SUPPORT),y)
CONFIG_FILE = config-ap-98-92d.txt
CONFIG_DAT  = config-ap-98-92d.dat
endif
ifeq ($(CONFIG_RTL_92C_SUPPORT),y)
CONFIG_FILE = config-ap-98-92c.txt
CONFIG_DAT  = config-ap-98-92c.dat
ifeq ($(CONFIG_RTL_92D_SUPPORT),y)
CONFIG_FILE = config-ap-98-92c92d.txt
CONFIG_DAT  = config-ap-98-92c-92d.dat
endif
endif
endif

ifeq ($(CONFIG_RTL_8196C_GW),y)
ifeq ($(CONFIG_RTL_92D_DMDP),y)
CONFIG_FILE = config-gw-96c-92d.txt 
CONFIG_DAT  = config-gw-96c-92d.dat
endif
endif

ifeq ($(CONFIG_POCKET_ROUTER_SUPPORT),y)
ifeq ($(CONFIG_DOMAIN_NAME_QUERY_SUPPORT),y)
ifeq ($(CONFIG_RTL_92D_DMDP),y)
CONFIG_FILE = config-gw-96c-pocket-92d.txt 
CONFIG_DAT  = config-gw-96c-pocket-92d.dat
endif
endif
endif

ifeq ($(CONFIG_RTL_8196D_GW),y)
CONFIG_FILE = config-gw-96d-92d.txt
CONFIG_DAT  = config-gw-96d-92d.dat
endif

ifeq ($(CONFIG_RTL_8197D_GW),y)
CONFIG_FILE = config-gw-97d-92c-92d.txt
CONFIG_DAT  = config-gw-97d-92c-92d.dat
endif

ifeq ($(CONFIG_RTL_8197DL_GW),y)
CONFIG_FILE = config-gw-97d-92c-92d.txt
CONFIG_DAT  = config-gw-97d-92c-92d.dat
endif

ifeq ($(CONFIG_RTL_8197D_AP),y)
CONFIG_FILE = config-ap-97d-8367r.txt
CONFIG_DAT = config-ap-97d-8367r.dat
endif


ifeq ($(CONFIG_RTL_8196E_GW),y)
ifeq ($(CONFIG_RTL_92D_SUPPORT),y)
CONFIG_FILE = config-gw-96e-92d.txt
CONFIG_DAT  = config-gw-96e-92d.dat
endif

ifeq ($(CONFIG_RTL_92C_SUPPORT),y)
CONFIG_FILE = config-gw-96e-92c.txt
CONFIG_DAT  = config-gw-96e-92c.dat
endif

ifeq ($(CONFIG_RTL_88E_SUPPORT),y)
CONFIG_FILE = config-gw-96e-88e.txt
CONFIG_DAT  = config-gw-96e-88e.dat
endif

ifeq ($(CONFIG_SLOT_0_8192EE),y)
CONFIG_FILE = config-gw-96e-88e.txt
CONFIG_DAT  = config-gw-96e-92e.dat
endif
endif

ifeq ($(CONFIG_RTL_8196EU_ULINKER),y)
ifeq ($(CONFIG_RTL_88E_SUPPORT),y)
ifeq ($(CONFIG_RTL_ULINKER),y)
CONFIG_FILE = config-gw-96eu-88e-ulinker.txt
CONFIG_DAT  = config-gw-96eu-88e-ulinker.dat
endif
endif
endif
ifeq ($(CONFIG_RTL_8812_SUPPORT),y)
ifeq ($(CONFIG_RTL_8197D_GW),y)
CONFIG_FILE = config-gw-97d-92c-8812.txt
CONFIG_DAT  = config-gw-97d-92c-8812.dat
endif
ifeq ($(CONFIG_RTL_8197DL_GW),y)
CONFIG_FILE = config-gw-97d-92c-8812.txt
CONFIG_DAT  = config-gw-97d-92c-8812.dat
endif
ifeq ($(CONFIG_RTL_8197F),y)
CONFIG_FILE = config-gw-97f-8812.txt
CONFIG_DAT  = config-gw-97f-8812.dat
endif
endif
ifeq ($(CONFIG_WLAN_HAL_8881A),y)
CONFIG_FILE = config-gw-8881A.txt
CONFIG_DAT  = config-gw-8881A.dat

ifeq ($(CONFIG_APPLE_MFI_SUPPORT), y)
CONFIG_FILE = config-gw-8881A-wac.txt
CONFIG_DAT  = config-gw-8881A-wac.dat
else
ifeq ($(CONFIG_RTL_8881A_SELECTIVE),y)
CONFIG_FILE = config-gw-8881A-selective.txt
CONFIG_DAT  = config-gw-8881A-selective.dat
endif
endif

endif

ifeq ($(CONFIG_RTL_8197F),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_0), y)
CONFIG_FILE = config-gw-97f-8812.txt
CONFIG_DAT  = config-gw-97f-8812.dat
else
CONFIG_FILE = config-gw-96e-88e.txt
CONFIG_DAT  = config-gw-97f.dat
endif
ifeq ($(CONFIG_RTL_8363NB_SUPPORT),y)
ifeq ($(CONFIG_CMCC),y)
ifeq ($(CONFIG_RTL_8197F_GW),y)
CONFIG_FILE = config-gw-97f-8812-8363NB.txt 
CONFIG_DAT = config-gw-97f-8812-8363NB.dat
else
ifeq ($(CONFIG_RTL_8197F_AP),y)
CONFIG_FILE = config-ap-97f-8812-8363NB.txt
CONFIG_DAT = config-ap-97f-8812-8363NB.dat
else
ifeq ($(CONFIG_RTL_8197F_RPT),y)
CONFIG_FILE = config-rpt-97f-8812-8363NB.txt
CONFIG_DAT = config-rpt-97f-8812-8363NB.dat
endif
endif
endif
endif
endif
endif

ifeq ($(CONFIG_RTL_DEF_SETTING_IN_FW),y)
#ifneq ($(CONFIG_RTL_8198C),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_1),y)
DUAL_BAND_SUPPORT = y
endif
endif

ifeq ($(CONFIG_RTL_92D_DMDP),y)
DUAL_BAND_SUPPORT = y
endif

ifeq ($(CONFIG_WLAN_HAL_8881A),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
DUAL_BAND_SUPPORT = y
endif
endif

ifeq ($(CONFIG_WLAN_HAL_8197F),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
DUAL_BAND_SUPPORT = y
endif
endif

ifeq ($(DUAL_BAND_SUPPORT), y)
CONFIG_FILE = config-example.txt
else
CONFIG_FILE = config-example-2g.txt
endif

ifeq ($(CONFIG_MODEL_HN330_331EA),y)
CONFIG_FILE = config-HN330-331EA.txt
endif

ifeq ($(CONFIG_MODEL_HN350_339EA),y)
CONFIG_FILE = config-HN350-339EA.txt
endif

ifeq ($(CONFIG_MODEL_HN100_340EA),y)
CONFIG_FILE = config-HN100-340EA.txt
endif

ifeq ($(CONFIG_MODEL_HN120_341FA),y)
CONFIG_FILE = config-HN120-341FA.txt
endif

ifeq ($(CONFIG_MODEL_HN120_341BA),y)
CONFIG_FILE = config-HN120-341BA.txt
endif

ifeq ($(CONFIG_MODEL_HN140_361EA),y)
CONFIG_FILE = config-HN140-361EA.txt
endif

CONFIG_DAT=config.dat
#endif
endif

ifeq ($(CONFIG_WLAN_HAL_8814AE),y)
ifeq ($(CONFIG_RTL_8196D),y)
CONFIG_FILE = config-gw-96d-8194.txt
CONFIG_DAT = config-gw-96d-8194.dat
else
CONFIG_FILE = config-gw-98C-8814.txt
CONFIG_DAT = config-gw-98C-8814.dat
endif
endif
#------------------------------------------------------------------------------
# all
#------------------------------------------------------------------------------	
all:
	rm -rf *.dat
ifneq ($(CONFIG_FILE),)
ifeq ($(CONFIG_RTL_DEF_SETTING_IN_FW),y)
	cp $(CONFIG_FILE) config_bak
ifeq ($(CONFIG_CMCC),y)
	sed -i 's/^IP_ADDR=192.168.1.254/IP_ADDR=192.168.8.1/g' $(CONFIG_FILE)
	sed -i 's/^DHCP_CLIENT_START=192.168.1.100/DHCP_CLIENT_START=192.168.8.100/g' $(CONFIG_FILE)
	sed -i 's/^DHCP_CLIENT_END=192.168.1.200/DHCP_CLIENT_END=192.168.8.200/g' $(CONFIG_FILE)
endif
ifeq ($(CONFIG_APP_BOA_HE_UI),y)
	sed -i 's/^IP_ADDR=192.168.1.254/IP_ADDR=192.168.8.1/g' $(CONFIG_FILE)
	sed -i 's/^DHCP_CLIENT_START=192.168.1.100/DHCP_CLIENT_START=192.168.8.100/g' $(CONFIG_FILE)
	sed -i 's/^DHCP_CLIENT_END=192.168.1.200/DHCP_CLIENT_END=192.168.8.200/g' $(CONFIG_FILE)
	sed -i 's/^USER_NAME=""/USER_NAME="user"/g' $(CONFIG_FILE)
	sed -i 's/^USER_PASSWORD=""/USER_PASSWORD="user"/g' $(CONFIG_FILE)
endif
endif
ifneq ($(CONFIG_RTL_8196C_iNIC),y)
ifneq ($(CONFIG_RTL_8198_INBAND_AP),y)
ifeq ($(CONFIG_RTL_8198_AP_ROOT),y)
	../tools/cvcfg-ap -no_hw $(CONFIG_FILE) $(CONFIG_DAT)
else
ifeq ($(CONFIG_RTL_8197D_AP),y)
	../tools/cvcfg-ap -no_hw $(CONFIG_FILE) $(CONFIG_DAT)
else
ifeq ($(CONFIG_RTL_AP_PACKAGE),y)
	../tools/cvcfg-ap -no_hw $(CONFIG_FILE) $(CONFIG_DAT)
else
	../tools/cvcfg-gw -no_hw $(CONFIG_FILE) $(CONFIG_DAT)
endif	#CONFIG_RTL_AP_PACKAGE
endif   #CONFIG_RTL_8197D_AP_ROOT
endif	#CONFIG_RTL_8198_AP_ROOT
endif	#CONFIG_RTL_8198_INBAND_AP
endif	#CONFIG_RTL_8196C_iNIC
endif
ifeq ($(CONFIG_RTL_DEF_SETTING_IN_FW),y)
	./bin2c.pl def_setting_data < $(CONFIG_DAT) > ../apmib/def_setting_config.h
	mv config_bak $(CONFIG_FILE)
endif
#------------------------------------------------------------------------------
# clean
#------------------------------------------------------------------------------
clean:
	rm -rf *.dat
