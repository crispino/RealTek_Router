EXEC = led_control
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

#CFLAGS += -DNO_PROTOTYPES=1 -Os
#LDLIBS += -lm
#CC	= mips-uclibc-gcc
#STRIP	= mips-uclibc-strip

IFLAGS	=
LDFLAGS =

all: $(EXEC)

$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
	
$(OBJS):gpiopin.h

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.o

