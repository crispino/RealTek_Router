#
# Realtek Semiconductor Corp.
#
# winfred_wang (winfred_wang@realsil.com.cn)
# July. 10, 2017
#

DIR_USERS := ..
include $(DIR_USERS)/.config
include $(DIR_LINUX)/.config

DIR_SOURCE := mosquitto-1.4.14
CLEAN := clean
DISTCLEAN := distclean
OPENSSL_DIR=$(DIR_ROOT)/users/openssl-1.0.2d

all: configCheck mqtt

configCheck:
	@echo "mtd-utils configCheck"

mqtt:
	$(MAKE) -C $(DIR_SOURCE) \
		CC=gcc \
		AR=ar \
		CXX=g++ \
		STRIP=strip \
		LD=$(CROSS_COMPILE)ld \
		CFLAGS="-I$(OPENSSL_DIR)/include" \
		LDFLAGS="-L$(OPENSSL_DIR) -lcrypto -lssl"

romfs:
	$(MAKE) -C $(DIR_SOURCE) install DESTDIR=$(DIR_ROMFS)

clean:
	if [ -f $(DIR_SOURCE)/Makefile ]; then \
		$(MAKE) -C $(DIR_SOURCE) $(CLEAN); \
	fi

distclean:
	if [ -f $(DIR_SOURCE)/Makefile ]; then \
                $(MAKE) -C $(DIR_SOURCE) $(DISTCLEAN); \
        fi
