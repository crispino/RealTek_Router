include ../busybox/.config
include ../../linux-3.10/.config
include ../.config

APMIBLIB=$(shell pwd)/../boa/apmib/libapmib.so
RTLLINKLIB=$(shell pwd)/../rtl_link/liblink.a
CFLAGS += -I$(shell pwd)/../boa/apmib/
CFLAGS += -I$(shell pwd)/../rtl_link/
ifeq ($(CONFIG_APP_RTK_ADAPTER),y)
CFLAGS += -I$(shell pwd)/../adapter-api/rtk_api/
LDFLAGS += -L$(shell pwd)/../adapter-api/rtk_api -lrtk_adapter
endif
CFLAGS += $(COMMON_CFLAGS)
CFLAGS += -DWITH_POSIX	
CFLAGS += -lm
CFLAGS += -Wall
CFLAGS += -lpthread

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

SRC = main.c
OBJ = $(SRC:%.c=%.o)

all: online_update

online_update: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@	$^ $(APMIBLIB) $(RTLLINKLIB) 
	$(STRIP) $@
clean:
	rm -rf *.o online_update

romfs:
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
	$(ROMFSINST) $(DIR_ROOT)/users/lib/lib/libmtdapi.so /lib/libmtdapi.so
endif
	$(ROMFSINST) /bin/online_update
	
