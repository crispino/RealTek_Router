TARGET = libcmcc_adapter.so
SRC = cmcc_api.c
OBJ = $(SRC:.c=.o)

ifeq ($(CONFIG_RTL_8367R_SUPPORT),y)
COMMON_CFLAGS += -DCONFIG_RTL_8367R_SUPPORT
endif

#LIB += -L../../boa/apmib/
#LIB += -L../rtk_api/
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -s -shared -o $@ $(OBJ)

.c.o:
	$(CC) $(COMMON_CFLAGS)  -c -o $@ -fpic $<	

clean:
	@rm -f $(OBJ) $(TARGET)

romfs:
	$(ROMFSINST) /lib/$(TARGET)
