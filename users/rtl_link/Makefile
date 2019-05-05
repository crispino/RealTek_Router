include ../busybox-1.13/.config
include ../../linux-3.10/.config
include ../.config

CFLAGS += -I$(shell pwd)/../boa/apmib/
CFLAGS += $(COMMON_CFLAGS)
ifeq ($(CONFIG_APP_RTK_ADAPTER), y)
CFLAGS += -I$(shell pwd)/../adapter-api/rtk_api/
endif
#LDFLAGS += ../adapter-api/rtk_api
#LIBS += -lrtk_adapter.so
CFLAGS += -Wall

ifeq ($(CONFIG_WGET), y)
CFLAGS += -DCONFIG_WGET
endif

ifeq ($(CONFIG_AUTO_DHCP_CHECK), y)
CFLAGS += -DCONFIG_AUTO_DHCP_CHECK
endif

ifeq ($(CONFIG_RTL_REPEATER_MODE_SUPPORT), y)
CFLAGS += -DCONFIG_RTL_REPEATER_MODE_SUPPORT
endif

ifeq ($(CONFIG_RTL_MULTI_CLONE_SUPPORT), y)
CFLAGS += -DMULTI_MAC_CLONE
endif

ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
CFLAGS += -DMTD_NAME_MAPPING
endif
ifeq ($(CONFIG_APP_RTK_ADAPTER),y)
CFLAGS += -DCONFIG_APP_RTK_ADAPTER
endif

ifeq ($(CONFIG_APP_FWD),y)
CFLAGS += -DCONFIG_APP_FWD
endif

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

all: liblink.a

liblink.a: $(OBJ)
	$(AR) rcs $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS) $(LIBS)

clean:
	rm -rf *.o *.a

romfs:
	@echo "do nothing here"
