TARGET = libont_crypto.so
SRC = aes_core.c aes_cbc.c md5.c ont_aes_ecb.c base64.c 
OBJ = $(SRC:.c=.o)
COMMON_CFLAGS+= -I$(DIR_USERS)/sdk-all-in-one/sdk-all-in-one/include -I./


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -s -shared -o $@ $(OBJ)

.c.o:
	$(CC) $(COMMON_CFLAGS)  -c -o $@ -fpic $<	

clean:
	@rm -f $(OBJ) $(TARGET)

romfs:
	$(ROMFSINST) /lib/$(TARGET)
