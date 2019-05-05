include ../busybox/.config
include ../../linux-3.10/.config
include ../.config
COAPLIB=$(shell pwd)/../libcoap-master/.libs/libcoap-1.a
CRYPTOLIB=$(shell pwd)/../openssl-1.0.2d/libcrypto.a
APMIBLIB=$(shell pwd)/../boa/apmib/libapmib.so
RTLLINKLIB=$(shell pwd)/../rtl_link/liblink.a
CURLLIB=$(shell pwd)/../curl-7.36.0/lib/.libs/libcurl.a
ANDLINKLIB_V2=libandlinkv2.a
ANDLINKLIB_V3=libandlinkv3.a

ifeq ($(CONFIG_ANDLINK_V2_SUPPORT), y)
ANDLINKLIB=$(ANDLINKLIB_V2)
CFLAGS += -DCONFIG_ANDLINK_V2_SUPPORT
else ifeq ($(CONFIG_ANDLINK_V3_SUPPORT), y)
ANDLINKLIB=$(ANDLINKLIB_V3)
CFLAGS += -DCONFIG_ANDLINK_V3_SUPPORT
endif
ifeq ($(CONFIG_ANDLINK_IF6_SUPPORT), y)
CFLAGS += -DCONFIG_ANDLINK_IF6_SUPPORT
endif
ifeq ($(CONFIG_ANDLINK_IF5_SUPPORT), y)
CFLAGS += -DCONFIG_ANDLINK_IF5_SUPPORT
endif

CFLAGS += -I$(shell pwd)/../libcoap-master/include/coap
CFLAGS += -I$(shell pwd)/../libcoap-master/
CFLAGS += -I$(shell pwd)/../boa/apmib/
CFLAGS += -I$(shell pwd)/../openssl-1.0.2d/include/
CFLAGS += -I$(shell pwd)/../rtl_link/
CFLAGS += -I$(shell pwd)/../curl-7.36.0/include
ifeq ($(CONFIG_APP_RTK_ADAPTER),y)
CFLAGS += -I$(shell pwd)/../adapter-api/rtk_api/
LDFLAGS += -L$(shell pwd)/../adapter-api/rtk_api -lrtk_adapter
endif
CFLAGS += $(COMMON_CFLAGS)
CFLAGS += -DWITH_POSIX	
CFLAGS += -lm
CFLAGS += -Wall
CFLAGS += -lpthread

LDFLAGS += -L$(DIR_USERS)/openssl-1.0.2d -lcrypto -lssl

MTDAPI_DIR=$(DIR_ROOT)/users/lib/lib
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
CFLAGS += -DMTD_NAME_MAPPING
CFLAGS += -L$(MTDAPI_DIR) -lmtdapi
endif

ifeq ($(CONFIG_STA_ROAMING_CHECK), y)
CFLAGS += -DSTA_ROAMING_CHECK
endif

ifeq ($(CONFIG_RTK_SMART_ROAMING), y)
CFLAGS += -DRTK_SMART_ROAMING
endif

ifeq ($(CONFIG_AUTO_DHCP_CHECK), y)
CFLAGS += -DCONFIG_AUTO_DHCP_CHECK
endif

ifeq ($(CONFIG_WGET), y)
CFLAGS += -DCONFIG_WGET
endif

SRC = andlink.c
OBJ = $(SRC:%.c=%.o)

all: andlink andlink_if5

andlink: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(COAP_LIB) -o $@	$^ $(APMIBLIB) $(ANDLINKLIB) $(RTLLINKLIB) $(CURLLIB) $(CRYPTOLIB) $(COAPLIB)
	$(STRIP) $@

andlink_if5:
	$(CC) -c andlink-if5

clean:
	rm -rf *.o andlink

romfs:
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
	$(ROMFSINST) $(DIR_ROOT)/users/lib/lib/libmtdapi.so /lib/libmtdapi.so
endif
	$(ROMFSINST) /bin/andlink
ifeq ($(CONFIG_ANDLINK_IF5_SUPPORT), y)
	$(ROMFSINST) ./andlink-if5/andlink_if5 /bin/andlink_if5
	$(ROMFSINST) ./ca.crt /etc/ca.crt
endif
	
