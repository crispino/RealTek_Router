include ../../busybox/.config
include ../../../linux-3.10/.config
include ../../.config
COAPLIB=$(shell pwd)/../../libcoap-master/.libs/libcoap-1.a
CRYPTOLIB=$(shell pwd)/../../openssl-1.0.2d/libcrypto.a
APMIBLIB=$(shell pwd)/../../boa/apmib/libapmib.so
RTLLINKLIB=$(shell pwd)/../../rtl_link/liblink.a
CURLLIB=$(shell pwd)/../../curl-7.36.0/lib/.libs/libcurl.a
MQTTLIB=$(shell pwd)/../../mqtt/mosquitto-1.4.14/lib/libmosquitto.a
ANDLINKLIB_IF5=libandlinkif5_$(TOOLCHAIN).a

CFLAGS += -I$(shell pwd)/../../libcoap-master/include/coap
CFLAGS += -I$(shell pwd)/../../libcoap-master/
CFLAGS += -I$(shell pwd)/../../boa/apmib/
CFLAGS += -I$(shell pwd)/../../openssl-1.0.2d/include/
CFLAGS += -I$(shell pwd)/../../rtl_link/
CFLAGS += -I$(shell pwd)/../../mqtt/mosquitto-1.4.14/lib
CFLAGS += -I$(DIR_USERS)/curl-7.36.0/include

ifeq ($(CONFIG_ANDLINK_IF6_SUPPORT), y)
CFLAGS += -DCONFIG_ANDLINK_IF6_SUPPORT
endif
ifeq ($(CONFIG_ANDLINK_IF5_SUPPORT), y)
CFLAGS += -DCONFIG_ANDLINK_IF5_SUPPORT
endif

ifeq ($(CONFIG_APP_RTK_ADAPTER),y)
CFLAGS += -I$(shell pwd)/../../adapter-api/rtk_api/
LDFLAGS += -L$(shell pwd)/../../adapter-api/rtk_api -lrtk_adapter
endif

LDFLAGS += -lpthread
#LDFLAGS += -L$(DIR_USERS)/curl-7.36.0/lib/.libs -lcurl
LDFLAGS += -L$(DIR_USERS)/openssl-1.0.2d -lcrypto -lssl

CFLAGS += $(COMMON_CFLAGS)
CFLAGS += -DWITH_POSIX	
CFLAGS += -lm
CFLAGS += -Wall

MTDAPI_DIR=$(DIR_ROOT)/users/lib/lib
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
CFLAGS += -DMTD_NAME_MAPPING
CFLAGS += -L$(MTDAPI_DIR) -lmtdapi
endif

ifeq ($(CONFIG_APP_DMALLOC),y)
LDFLAGS += ../../dmalloc-5.5.2/libdmallocth.a
CFLAGS += -DDMALLOC
endif

ifeq ($(CONFIG_STA_ROAMING_CHECK), y)
CFLAGS += -DSTA_ROAMING_CHECK
endif

ifeq ($(CONFIG_RTK_SMART_ROAMING), y)
CFLAGS += -DRTK_SMART_ROAMING
endif

ifeq ($(CONFIG_WGET), y)
CFLAGS += -DCONFIG_WGET
endif

SRC = andlink-if5.c
OBJ = $(SRC:%.c=%.o)

all: andlink_if5

andlink_if5: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(COAP_LIB) -o $@	$^ $(APMIBLIB) $(ANDLINKLIB_IF5) $(RTLLINKLIB) $(CRYPTOLIB) $(COAPLIB) $(CURLLIB) $(MQTTLIB)
	$(STRIP) $@

romfs:
	@echo "do nothing"

clean:
	rm -rf *.o andlink_if5

