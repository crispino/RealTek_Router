#CC		= mips-uclibc-gcc
COPT		= -Os 
CFLAGS		= ${COPT} 
ifeq ($(CONFIG_RSDK_rsdk-1.5.0-4181-EB-2.6.30-0.9.30.2),y)
CFLAGS 		+= -mips16
endif
ifeq ($(CONFIG_RSDK_rsdk-1.5.0-4181-EB-2.6.30-0.9.30.3),y)
CFLAGS 		+= -mips16
endif
LDFLAGS		= 
#STRIP		= mips-uclibc-strip
ifeq ($(wildcard $(DIR_LINUX)/.config),)
include $(DIR_ROOT)/linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif
#include ../../linux-2.6.30/.config
ifeq ($(RTL8186_TR),1)
CFLAGS+=-DCONFIG_RTL8186_TR 
endif

ifeq ($(CONFIG_RTL865X_SC),1)
CFLAGS+=-DCONFIG_RTL865X_SC 
endif

ifeq ($(RTL8186_KB),1)
CFLAGS+=-DCONFIG_RTL8186_KB
endif

ifeq ($(CONFIG_RTL_8196B),y)
CFLAGS+=-DCONFIG_RTL_8196B
endif

ifeq ($(CONFIG_RTL_8196C),y)
CFLAGS+=-DCONFIG_RTL_8196C
endif

ifeq ($(CONFIG_RTL_8198),y)
CFLAGS+=-DCONFIG_RTL_8198
endif

ifeq ($(CONFIG_RTL_819XD),y)
CFLAGS+=-DCONFIG_RTL_819XD
endif

ifeq ($(CONFIG_RTL_8196E),y)
CFLAGS+=-DCONFIG_RTL_8196E
endif

ifeq ($(CONFIG_RTL_8198B),y)
CFLAGS+=-DCONFIG_RTL_8198B
endif

ifeq ($(CONFIG_RTL_8198C),y)
CFLAGS+=-DCONFIG_RTL_8198C
endif

ifeq ($(CONFIG_RTL_819X),y)
CFLAGS+=-DCONFIG_RTL_819X
endif

ifeq ($(CONFIG_RTL_8197F),y)
CFLAGS+=-DCONFIG_RTL_8197F
endif

ifeq ($(CONFIG_RTL_92D_DMDP),y)
CFLAGS  += -DCONFIG_RTL_92D_DMDP
endif
ifeq ($(CONFIG_APP_ADAPTER_API),y)
CFLAGS  += -DTRAFFIC_METER_SUPPORT
endif
ifeq ($(CONFIG_ANDLINK_SUPPORT),y)
CFLAGS  += -DTRAFFIC_METER_SUPPORT
endif
ifeq ($(CONFIG_ELINK_SUPPORT),y)
CFLAGS  += -DTRAFFIC_METER_SUPPORT
endif
ifeq ($(CONFIG_APP_ADAPTER_API),y)
CFLAGS  += -DCONFIG_APP_ADAPTER_API
endif

CFLAGS += $(COMMON_CFLAGS)

CFLAGS += $(EXTRA_CFLAGS)

all:    reload
#	-ctags *.[ch]

clean:
	rm -f reload tags *.o 

sources = reload.c

reload:	$(sources:.c=.o)
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@	
	$(STRIP) $@

romfs:
	$(ROMFSINST) reload /bin/reload
