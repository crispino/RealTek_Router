ifeq ($(DIR_LINUX),)
include ../../linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

RSDK_LIB_DIR=$(shell  echo $(PATH) | cut -f1 -d:)

SUBDIR_ARPTABLES := arptables-v0.0.4


all:
	if [ ! -e ./$(SUBDIR_ARPTABLES)/Makefile ]; then \
		cd ./$(SUBDIR_ARPTABLES); \
		LD=$(CROSS_COMPILE)ld CC="$(CROSS_COMPILE)gcc -fpic -O3" ./configure --build=i686-pc-linux-gnu --host=mips-linux; \
	fi
	make -C $(SUBDIR_ARPTABLES)

	
romfs:
	$(ROMFSINST) $(SUBDIR_ARPTABLES)/arptables /bin/arptables



clean:
	make -C $(SUBDIR_ARPTABLES) distclean
	
distclean:
	make -C $(SUBDIR_ARPTABLES) distclean
