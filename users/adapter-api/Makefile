ifeq ($(wildcard $(DIR_LINUX)/.config),)
include ../../linux-2.6.30/.config
DIR_LINUX_KERNEL = ../../linux-2.6.30
else
include $(DIR_LINUX)/.config
DIR_LINUX_KERNEL = $(DIR_LINUX)
endif

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../.config
else
include $(DIR_USERS)/.config
endif

#COMMON_CFLAGS =
SUBDIR=
ifeq ($(CONFIG_APP_RTK_ADAPTER),y)
SUBDIR += rtk_api
endif
ifeq ($(CONFIG_APP_360_ADAPTER),y)
SUBDIR += 360_api
endif
ifeq ($(CONFIG_APP_HAIER_ADAPTER),y)
SUBDIR += haier_api
endif
ifeq ($(CONFIG_APP_CMCC_ADAPTER),y)
SUBDIR += cmcc
endif
ifeq ($(CONFIG_APP_RTK_API_TEST_CASE),y)
SUBDIR += test_case
endif


all: $(SUBDIR)
	for i in $(SUBDIR) ; do \
       $(MAKE) -C $$i || exit "$$?"; \
	done

clean: $(SUBDIR)
	for i in $(SUBDIR) ; do \
       $(MAKE) -C $$i clean || exit "$$?"; \
	done

romfs: $(SUBDIR)
	for i in $(SUBDIR) ; do \
       $(MAKE) -C $$i romfs || exit "$$?"; \
	done
