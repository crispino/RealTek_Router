cmd_coreutils/libcoreutils/cp_mv_stat.o := msdk-linux-gcc -Wp,-MD,coreutils/libcoreutils/.cp_mv_stat.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -D"BB_VER=KBUILD_STR(1.13.4)" -DBB_BT=AUTOCONF_TIMESTAMP -DCONFIG_APP_RTK_ADAPTER -I/data/xiaoning/RealTek/HN120/target/tmpfs/include -DCONFIG_APP_RTK_ADAPTER -I/data/xiaoning/RealTek/HN120/target/tmpfs/include -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(cp_mv_stat)"  -D"KBUILD_MODNAME=KBUILD_STR(cp_mv_stat)" -c -o coreutils/libcoreutils/cp_mv_stat.o coreutils/libcoreutils/cp_mv_stat.c

deps_coreutils/libcoreutils/cp_mv_stat.o := \
  coreutils/libcoreutils/cp_mv_stat.c \
    $(wildcard include/config/feature/verbose/cp/message.h) \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/o.h) \
    $(wildcard include/config/ntf.h) \
    $(wildcard include/config/t.h) \
    $(wildcard include/config/l.h) \
    $(wildcard include/config/wn.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/feature/devfs.h) \
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
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/ctype.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uClibc_touplow.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/dirent.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/dirent.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/errno.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/errno.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/linux/errno.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm/errno.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm-generic/errno-base.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/syscall.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sysnum.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/fcntl.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/fcntl.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/stat.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/stat.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/inttypes.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/rpc/netdb.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/siginfo.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/netdb.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/setjmp.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/setjmp.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/signal.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/signum.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sigaction.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sigcontext.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sigstack.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/ucontext.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/sigthread.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/stdio.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uClibc_stdio.h \
    $(wildcard include/config/io/futexes//.h) \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/wchar.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include/stdarg.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/stdlib.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/waitflags.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/waitstatus.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/alloca.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/string.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/poll.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/poll.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/mman.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/mman.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/time.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/wait.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/resource.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/resource.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/termios.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/termios.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uClibc_clk_tck.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/unistd.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/posix_opt.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/uClibc_posix_opt.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/environments.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/confname.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/getopt.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/utime.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/param.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/linux/param.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm/param.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/asm-generic/param.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/mntent.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/paths.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/sys/statfs.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/bits/statfs.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/pwd.h \
  /data/xiaoning/RealTek/HN120/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/../../../../mips-linux-uclibc/include/grp.h \
  include/xatonum.h \
  coreutils/libcoreutils/coreutils.h \

coreutils/libcoreutils/cp_mv_stat.o: $(deps_coreutils/libcoreutils/cp_mv_stat.o)

$(deps_coreutils/libcoreutils/cp_mv_stat.o):
