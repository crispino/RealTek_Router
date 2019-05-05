#
# Realtek Semiconductor Corp.
#
# winfred_wang (winfred_wang@realsil.com.cn)
# July. 10, 2017
#

DIR_USERS := ..
include $(DIR_USERS)/.config
include $(DIR_LINUX)/.config

DIR_SOURCE := sdk-all-in-one
CLEAN := clean
DISTCLEAN := distclean

COMPILE_OPTION=OFF
ifeq ($(CONFIG_APP_ONT_CRYPTO),y)
CRYPTO_DIR=ont_crypto
CRYPTO_INC=$(CRYPTO_DIR)
DECREASE_OPTION=ON
else
CRYPTO_DIR=$(openssl_dir)
CRYPTO_INC=$(CRYPTO_DIR)/include
endif


all: configCheck sdk-onenet

configCheck:
	@echo "sdk_onenet configCheck"

prepare:
ifeq ($(CONFIG_RTL_8196E),y)
	cp $(DIR_SOURCE)/realtek/edp/libedp_96e.so $(DIR_SOURCE)/realtek/edp/libedp.so
endif
ifeq ($(CONFIG_RTL_8197F),y)
	cp $(DIR_SOURCE)/realtek/edp/libedp_97f.so $(DIR_SOURCE)/realtek/edp/libedp.so
endif


sdk-onenet: prepare
	cd $(DIR_SOURCE); \
	cmake 	\
		-DCMAKE_INSTALL_PREFIX=$(DIR_ROMFS) \
		-DCMAKE_C_COMPILER:FILEPATH=$(DIR_RSDK)/bin/$(CROSS_COMPILE)gcc \
		-DCMAKE_CXX_COMPILER:FILEPATH=$(DIR_RSDK)/bin/$(CROSS_COMPILE)g++ \
		-DCMAKE_FIND_ROOT_PATH:PATH=$(DIR_RSDK) \
		-DCRYPTO_DIR:STRING=$(CRYPTO_DIR) \
		-DCRYPTO_INC:STRING=$(CRYPTO_INC) \
		-DDECREASE_OPTION:BOOL=$(DECREASE_OPTION); \
	make

romfs:
	$(ROMFSINST) $(DIR_SOURCE)/bin/edp_client /bin
	$(ROMFSINST) $(DIR_SOURCE)/realtek/edp/libedp.so /lib

clean:
	if [ -f $(DIR_SOURCE)/Makefile ]; then \
		$(MAKE) -C $(DIR_SOURCE) $(CLEAN); \
	fi

distclean:
	if [ -f $(DIR_SOURCE)/Makefile ]; then \
                $(MAKE) -C $(DIR_SOURCE) $(DISTCLEAN); \
        fi
