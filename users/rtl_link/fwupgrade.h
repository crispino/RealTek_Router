#define URL_SIZE_MAX 256
#define STORE_PATH "/var/tmp/"

#define DOWNLOAD_LOG "/var/download.log"
#define DOWNLOAD_LOG_REPORT "/var/downloadreport.txt"

#define FWD_CONF "/var/fwd.conf"
#define GOOD_BANK_MARK_MASK 0x80000000  //goo abnk mark must set bit31 to 1
#define NO_IMAGE_BANK_MARK 0x80000000  
#define OLD_BURNADDR_BANK_MARK 0x80000001 
#define BASIC_BANK_MARK 0x80000002           
#define FORCEBOOT_BANK_MARK 0xFFFFFFF0  //means always boot/upgrade in this bank

//#define RTL_LINK_UPGRADE_SHRINKMEM

#ifndef CONFIG_MTD_NAND
static char *Kernel_dev_name[2]=
 {
   "/dev/mtdblock0", "/dev/mtdblock2"
 };
static char *Rootfs_dev_name[2]=
 {
   "/dev/mtdblock1", "/dev/mtdblock3"
 };

#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_WEB_BACKUP_ENABLE)
static char *Web_dev_name[2]=
{
	"/dev/mtdblock0", "/dev/mtdblock2"
};
#endif
#endif
#else
static char *Kernel_dev_name[2]=
 {
   "/dev/mtdblock2", "/dev/mtdblock5"
 };
static char *Rootfs_dev_name[2]=
 {
   "/dev/mtdblock3", "/dev/mtdblock6"
 };

#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_WEB_BACKUP_ENABLE)
static char *Web_dev_name[2]=
{
	"/dev/mtdblock2", "/dev/mtdblock5"
};
#endif
#endif

#endif

