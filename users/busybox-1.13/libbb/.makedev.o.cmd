cmd_libbb/makedev.o := msdk-linux-gcc -Wp,-MD,libbb/.makedev.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -D"BB_VER=KBUILD_STR(1.13.4)" -DBB_BT=AUTOCONF_TIMESTAMP -DCONFIG_APP_RTK_ADAPTER -I/data/xiaoning/RealTek/HN120/target/tmpfs/include -DCONFIG_APP_RTK_ADAPTER -I/data/xiaoning/RealTek/HN120/target/tmpfs/include -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(makedev)"  -D"KBUILD_MODNAME=KBUILD_STR(makedev)" -c -o libbb/makedev.o libbb/makedev.c

deps_libbb/makedev.o := \
  libbb/makedev.c \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config/nommu.h) \
    $(wildcard include/config//nommu.h) \
    $(wildcard include/config//mmu.h) \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/byteswap.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/c95.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/en2k8.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/ile.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
    $(wildcard include/config/ix/implicitly.h) \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uClibc_config.h \
    $(wildcard include/config/mips/o32/abi//.h) \
    $(wildcard include/config/mips/n32/abi//.h) \
    $(wildcard include/config/mips/n64/abi//.h) \
    $(wildcard include/config/mips/isa/1//.h) \
    $(wildcard include/config/mips/isa/2//.h) \
    $(wildcard include/config/mips/isa/3//.h) \
    $(wildcard include/config/mips/isa/4//.h) \
    $(wildcard include/config/mips/isa/mips32//.h) \
    $(wildcard include/config/mips/isa/mips32r2//.h) \
    $(wildcard include/config/mips/isa/mips64//.h) \
    $(wildcard include/config/mips/isa/mips64r2//.h) \
    $(wildcard include/config//.h) \
    $(wildcard include/config/link//.h) \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/byteswap.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/byteswap-common.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/endian.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/endian.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/arpa/inet.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/netinet/in.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/stdint.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/wchar.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/wordsize.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/socket.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/uio.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/types.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/types.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/typesizes.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/time.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include/stddef.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/select.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/select.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sigset.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/time.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/sysmacros.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/pthreadtypes.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sgidefs.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uio.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/socket.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include-fixed/limits.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include-fixed/syslimits.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/limits.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/posix1_lim.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/local_lim.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/linux/limits.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uClibc_local_lim.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/posix2_lim.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/xopen_lim.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/stdio_lim.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/socket_type.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sockaddr.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm/socket.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm/sockios.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm/ioctl.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm-generic/ioctl.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/in.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include/stdbool.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/mount.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/ioctl.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/ioctls.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm/ioctls.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/ioctl-types.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/ttydefaults.h \

libbb/makedev.o: $(deps_libbb/makedev.o)

$(deps_libbb/makedev.o):
