EXEC = FTY_TEST
OBJS = main.o 

CROSS_COMPILE ?= rsdk-linux-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc

ifeq ($(CONFIG_RTL_8197F),y)
        CFLAGS += -DCONFIG_RTL_8197F
endif

ifeq ($(CONFIG_RTL_8196E),y)
        CFLAGS += -DCONFIG_RTL_8196E
endif

ifeq ($(CONFIG_MODEL_HN330_331EA),y)
CFLAGS += -DCONFIG_MODEL_HN330_331EA
endif

ifeq ($(CONFIG_MODEL_HN350_339EA),y)
CFLAGS += -DCONFIG_MODEL_HN350_339EA
endif

ifeq ($(CONFIG_MODEL_HN100_340EA),y)
CFLAGS += -DCONFIG_MODEL_HN100_340EA
endif

ifeq ($(CONFIG_MODEL_HN120_341FA),y)
CFLAGS += -DCONFIG_MODEL_HN120_341FA
endif

ifeq ($(CONFIG_MODEL_HN120_341BA),y)
CFLAGS += -DCONFIG_MODEL_HN120_341BA
endif

CFLAGS += -DNO_PROTOTYPES=1 -Os
LDLIBS += -lm
#CC	= mips-uclibc-gcc
#STRIP	= mips-uclibc-strip

IFLAGS	=
LDFLAGS =

ifeq ($(SLINK),1)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += --static -Wl,--gc-sections
endif
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/FTY_TEST

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

