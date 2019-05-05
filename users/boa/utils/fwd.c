#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>

//keith_fwd
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
//keith_fwd
#include <unistd.h>
#ifdef KERNEL_3_10
#include <linux/fs.h>
#endif

#if defined(CONFIG_ANDLINK_SUPPORT)||defined(CONFIG_ELINK_SUPPORT)
#include "../../rtl_link/fwupgrade.h"
#define RTL_LINK_FW_DIR "/var/rtl_link_fw_dir"
#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
#include <sys/mman.h>
#endif
#endif


//--------------------------------------------------------------------
//#define CHECK_BURN_SERIAL 1

#if 0
  #define BDBG_BSN(format, arg...) 	  \
		  printf(format , ## arg)
#else
  #define BDBG_BSN(format, arg...)
#endif

#if CHECK_BURN_SERIAL
#define FW_SIGNATURE			((char *)"cs6c")	// fw signature
#define FW_SIGNATURE_WITH_ROOT	((char *)"cr6c")	// fw signature with root fs
#define ROOT_SIGNATURE			((char *)"r6cr")

#define SQSH_SIGNATURE			((char *)"sqsh")
#define SQSH_SIGNATURE_LE       ((char *)"hsqs")

#define SIG_LEN			4

#define SQUASHFS_MAGIC			0x73717368
#define SQUASHFS_MAGIC_SWAP 	0x68737173

#define SIZE_OF_SQFS_SUPER_BLOCK 640
#define SIZE_OF_CHECKSUM 2
#define OFFSET_OF_LEN 2

/* Firmware image file header */
typedef struct img_header {
	unsigned char signature[SIG_LEN];
	unsigned int startAddr;
	unsigned int burnAddr;
	unsigned int len;
}__attribute__ ((packed)) IMG_HEADER_T, *IMG_HEADER_Tp;

struct squashfs_super_block_partial {
	unsigned int		s_magic;
	unsigned int		inodes;
	unsigned int		mkfs_time /* time of filesystem creation */;
	unsigned int		block_size;
#if 0
	unsigned int		fragments;
	unsigned short		compression;
	unsigned short		block_log;
	unsigned short		flags;
	unsigned short		no_ids;
	unsigned short		s_major;
	unsigned short		s_minor;
	long long		root_inode;//squashfs_inode_t	root_inode;
	long long		bytes_used;
	long long		id_table_start;
	long long		xattr_table_start;
	long long		inode_table_start;
	long long		directory_table_start;
	long long		fragment_table_start;
	long long		lookup_table_start;
#endif
}__attribute__ ((packed));

struct _rootfs_padding {
	unsigned char  zero_pad[2];
	unsigned short chksum;
	unsigned char  signature[SIG_LEN];
	unsigned long  len;
}__attribute__ ((packed));

unsigned long gen_burn_serial()
{
	unsigned long burn_serial = 0;

	/* generate burn_serial */
	srand(time(NULL));
	burn_serial=rand() | (1<<31);

	return burn_serial;
}
#endif /* #if CHECK_BURN_SERIAL */
//--------------------------------------------------------------------
#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
static int isFileExist(char *file_name)
{
	struct stat status;

	if ( stat(file_name, &status) < 0)
		return 0;

	return 1;
}
#endif

#if defined(CONFIG_APP_CMCC_ADAPTER)||defined(CONFIG_APP_BOA_HE_UI)||(CONFIG_ANDLINK_SUPPORT)||(CONFIG_ELINK_SUPPORT)

static int daemonKilled = 0;
//static void killDaemon()
void upgrade_killDaemons()
{
 		
    if (daemonKilled)
    	return;

    daemonKilled = 1;

#if 1	
	FILE *stream;
	system("ps > /var/alive_daemons");
	stream = fopen ( "/var/alive_daemons", "r" );
	if ( stream != NULL ) { 	
		char *strtmp, *pid_token=NULL;
		char line[100], cmdBuf[50];

		while (fgets(line, sizeof(line), stream))
		{
			strtmp = line;
			while(*strtmp == ' ')
				strtmp++;
            if(	(strstr(strtmp,"boa") != 0) 
            		|| (strstr(strtmp,"/bin/sh") != 0) 
            		|| (strstr(strtmp,"ps") != 0) 
            		|| (strstr(strtmp,"testcase") != 0) 
            		|| (strstr(strtmp,"edp_client") != 0) 
            		|| (strstr(strtmp, "root") == 0)
            		|| (strstr(strtmp,"andlink") != 0) 
#if defined(CONFIG_APP_FWD) // fwd is daemon to write firmware in flash at last.
            		|| (strstr(strtmp, "fwd") != 0)
#endif            		
#if defined(APP_WATCHDOG)
					|| (strstr(strtmp,"watchdog") != 0)
#endif
            )
				continue;
			else
			{

				pid_token = strtok(strtmp, " ");
				if((pid_token != NULL) && (strlen(pid_token) >= 3))
				{
					sprintf(cmdBuf,"kill -9 %s > /dev/null", pid_token);
					system(cmdBuf);
				}
			}
		}
		fclose(stream);
	}
	system("rm -rf /var/alive_daemons");

#endif
	
// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
	system("killall -9 snmpd 2> /dev/null");
	system("killall -9 solar_monitor 2> /dev/null");
	system("killall -9 solar 2> /dev/null");
	system("killall -9 dns_task 2> /dev/null");
	system("killall -9 ivrserver 2> /dev/null");
	system("killall -9 fwupdate 2> /dev/null");
	system("killall -9 netlink 2> /dev/null");
#endif

#ifdef CONFIG_SNMP
	system("killall -9 snmpd 2> /dev/null");
#endif
}

char *get_ifname(char *name, char *p)
{
	while (isspace(*p))
		p++;
	while (*p) {
		if (isspace(*p))
			break;
		if (*p == ':') {	/* could be an alias */
			char *dot = p, *dotname = name;

			*name++ = *p++;
			while (isdigit(*p))
				*name++ = *p++;
			if (*p != ':') {	/* it wasn't, backup */
				p = dot;
				name = dotname;
			}
			if (*p == '\0')
				return NULL;
			p++;
			break;
		}
		*name++ = *p++;
	}
	*name++ = '\0';
	return p;
}

#define _PATH_PROCNET_DEV "/proc/net/dev"

int shutdown_netdev(void)
{
	FILE *fh;
	char buf[512];
	char *s, name[16], tmp_str[64];
	int iface_num=0;
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		return 0;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);
	while (fgets(buf, sizeof buf, fh)) {
		s = get_ifname(name, buf);
		if(strstr(name, "eth") || strstr(name, "wlan") || strstr(name, "ppp")|| strstr(name, "sit")){
			iface_num++;
			snprintf(tmp_str,sizeof(tmp_str),"ifconfig %s down",name);
			system(tmp_str);
		}
	}
	
	fclose(fh);
	return iface_num;
}
pid_t find_pid_by_name( char* pidName)
{
	DIR *dir;
	struct dirent *next;
	pid_t pid;
	
	if ( strcmp(pidName, "init")==0)
		return 1;
	
	dir = opendir("/proc");
	if (!dir) {
		printf("Cannot open /proc");
		return 0;
	}

	while ((next = readdir(dir)) != NULL) {
		FILE *status;
		char filename[64];
		char buffer[64];
		char name[64];

		/* Must skip ".." since that is outside /proc */
		if (strcmp(next->d_name, "..") == 0)
			continue;

		/* If it isn't a number, we don't want it */
		if (!isdigit(*next->d_name))
			continue;

		sprintf(filename, "/proc/%s/status", next->d_name);
		if (! (status = fopen(filename, "r")) ) {
			continue;
		}
		if (fgets(buffer, 63, status) == NULL) {
			fclose(status);
			continue;
		}
		fclose(status);

		/* Buffer should contain a string like "Name:   binary_name" */
		sscanf(buffer, "%*s %s", name);
		if (strcmp(name, pidName) == 0) {
		//	pidList=xrealloc( pidList, sizeof(pid_t) * (i+2));
			pid=(pid_t)strtol(next->d_name, NULL, 0);
			closedir(dir);
			return pid;
		}
	}	
	closedir(dir);
	return 0;
}

#endif

int main(int argc, char *argv[])
{
	unsigned char *shm_memory=NULL;
	int shm_id = 0;
	FILE *fp, *fp2;
	FILE *fp_watchdog;
	char  buf[150]={0};
#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
    struct stat status;
    int fileLen, fd;
    char fwdir[256];
#endif

				
#if CHECK_BURN_SERIAL
	struct _rootfs_padding  rootfs_padding;
	IMG_HEADER_T Header;
	unsigned long burn_serial = 0;
	int padding_len = 0;
	unsigned char* read_buf = NULL;
	int trueorfalse = 0;
#endif
#ifdef CONFIG_APP_CMCC_ADAPTER
	pid_t pid=0;
#endif
	while(1)
	{  	  


		sleep(3);
		

		fp = fopen("/var/fwd.ready", "r");
    if (fp)
		{
#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
            if(isFileExist("/var/rtl_link_shrinkmem")){
                fgets(fwdir,sizeof(fwdir),fp);
                stat(fwdir, &status);
                fileLen = status.st_size;

                fd = open(fwdir, O_RDONLY);
                if(fd == -1){
                    printf("FWD (%s)%d open fw %s failed\n", __FUNCTION__, __LINE__, fwdir);
                }

                shm_memory = (char *)mmap(NULL, fileLen, PROT_READ, MAP_SHARED, fd, 0);
                if(shm_memory == (void *)-1){
                    printf("FWD (%s)%d mmap failed\n", __FUNCTION__, __LINE__);
                }
            }else
#endif
            {
       		    fgets(buf,150,fp);
    			shm_id = atoi(buf);
    			
    			/* Attach the shared memory segment */
    			shm_memory = (char *)shmat(shm_id, NULL, 0);
    	    }		
    	    
#ifdef CONFIG_APP_CMCC_ADAPTER
			system("flash set UPGRADED_FW 1");
			pid=find_pid_by_name("edp_client");
			//pid=find_pid_by_name("udhcpc"); //for test
			if(pid>0)
				kill(pid,SIGUSR1);
			sleep(5); //tw need sleep for reply status change
#endif
#if defined(CONFIG_APP_CMCC_ADAPTER)||defined(CONFIG_APP_BOA_HE_UI)||(CONFIG_ANDLINK_SUPPORT)||(CONFIG_ELINK_SUPPORT)
	upgrade_killDaemons();
#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
    // do nothing
#else
	shutdown_netdev();
	system("ifconfig br0 down 2> /dev/null");
#endif
#endif			
			fp2 = fopen("/var/fwd.conf", "r");
			if(fp2)
			{
				int head_offset=0;
				unsigned char mtd_name[20]={0};
				unsigned char offset_str[20]={0};
				unsigned char size_str[20]={0};
				unsigned char fw_offset_str[20]={0};
				
				int numWrite;
				int fw_offset=0;
				
				
				fgets(mtd_name,20,fp2);
				fgets(offset_str,20,fp2);
				fgets(size_str,20,fp2);
				fgets(fw_offset_str,20,fp2);
								
				do{
					
					int fh;
					int offset = atoi(offset_str);
				 	int size = atoi(size_str);
				 	int fw_offset = atoi(fw_offset_str);				 	

				 	mtd_name[strlen(mtd_name)-1]='\0';
				 	
				 	fh = open(mtd_name, O_RDWR);
				 	if(fh == -1)
				 	{
						//printf("\r\n Open [%s] fail.__[%s-%u]\r\n",mtd_name,__FILE__,__LINE__);				 		
						fp_watchdog = fopen("/proc/watchdog_reboot","w+"); //reboot
						if(fp_watchdog)
						{
							fputs("111", fp_watchdog);
							fclose(fp_watchdog);
						}
						
						for(;;);
				 	}

#if CHECK_BURN_SERIAL
#ifndef CONFIG_MTD_NAND
					if (size > sizeof(IMG_HEADER_T))
					{
				 		memcpy((void *)(&Header), (void *)(&(shm_memory[fw_offset])), sizeof(IMG_HEADER_T));

						if (!memcmp(Header.signature, FW_SIGNATURE, SIG_LEN) ||
							!memcmp(Header.signature, FW_SIGNATURE_WITH_ROOT, SIG_LEN))
						{
							Header.burnAddr = burn_serial = gen_burn_serial();
							memcpy((void *)(&(shm_memory[fw_offset])), (void *)(&Header), sizeof(IMG_HEADER_T));
							memcpy((void *)(&(shm_memory[fw_offset+size])), (void *)(&burn_serial), sizeof(burn_serial));
							size += sizeof(burn_serial);
						}
						else if (!memcmp(Header.signature, SQSH_SIGNATURE, SIG_LEN) ||
							!memcmp(Header.signature, SQSH_SIGNATURE_LE, SIG_LEN))
						{
							struct squashfs_super_block_partial sblk;
						 	lseek(fh, offset, SEEK_SET);
							read(fh, (char *)(&sblk), sizeof (sblk));

							if (size == sblk.mkfs_time+SIZE_OF_SQFS_SUPER_BLOCK+SIZE_OF_CHECKSUM)
								padding_len = sizeof(rootfs_padding.zero_pad);

							memset(&rootfs_padding, 0, sizeof(rootfs_padding));
							memcpy((void *)(&rootfs_padding.chksum), (void *)(&(shm_memory[fw_offset]) + size - SIZE_OF_CHECKSUM), SIZE_OF_CHECKSUM);
							memcpy((char *)(&rootfs_padding.signature), ROOT_SIGNATURE, SIG_LEN);
							rootfs_padding.chksum -= padding_len;
							rootfs_padding.len = sblk.mkfs_time = burn_serial = size + padding_len - SIZE_OF_SQFS_SUPER_BLOCK - SIZE_OF_CHECKSUM;
							memcpy((void *)(&(shm_memory[fw_offset])), (void *)(&sblk), sizeof(struct squashfs_super_block_partial));
						}
						else {
							BDBG_BSN("\n[%s:%d] not handle!!!\n", __FUNCTION__, __LINE__);
						}
					}
#endif
#endif /* #if CHECK_BURN_SERIAL */

				 	lseek(fh, offset, SEEK_SET);
				 	numWrite = write(fh, &(shm_memory[fw_offset]), size);
				 	
#if CHECK_BURN_SERIAL
#ifndef CONFIG_MTD_NAND
					if (burn_serial) {
						if (!memcmp(Header.signature, SQSH_SIGNATURE, SIG_LEN) ||
							!memcmp(Header.signature, SQSH_SIGNATURE_LE, SIG_LEN))
						{						
							lseek(fh, offset+size-SIZE_OF_CHECKSUM, SEEK_SET);

							if (padding_len) {
								BDBG_BSN("[%s:%d] pad\n", __FUNCTION__, __LINE__);
								write(fh, &rootfs_padding, sizeof(rootfs_padding));
							}
							else {
								BDBG_BSN("[%s:%d] nopad\n", __FUNCTION__, __LINE__);
								write(fh, (void *)(&rootfs_padding)+sizeof(rootfs_padding.zero_pad), sizeof(rootfs_padding)-sizeof(rootfs_padding.zero_pad));
							}
						}
					}
#endif
#endif /* #if CHECK_BURN_SERIAL */

				 	if(numWrite != size)
				 	{
						//printf("\r\n Write firmware size incorrect.__[%s-%u]\r\n",__FILE__,__LINE__);				 		
						fp_watchdog = fopen("/proc/watchdog_reboot","w+"); //reboot
						if(fp_watchdog)
						{
							fputs("111", fp_watchdog);
							fclose(fp_watchdog);
						}
						
						for(;;);			 		
				 	}
				 	sync();	
					#ifdef KERNEL_3_10
					if(ioctl(fh,BLKFLSBUF,NULL) < 0){
						printf("flush mtd system cache error\n");
					}
					#endif
#if CHECK_BURN_SERIAL
#ifndef CONFIG_MTD_NAND
					/* compare flash/ram data */
					if (!memcmp(Header.signature, FW_SIGNATURE, SIG_LEN) ||
							!memcmp(Header.signature, FW_SIGNATURE_WITH_ROOT, SIG_LEN)){
						//printf("[%s]%d\n",__func__,__LINE__);
						read_buf = (unsigned char*)malloc(size);
						if(read_buf == NULL){
							/* error */
							trueorfalse = 1;
							goto FINISH_COMPARE;
						}
							
						lseek(fh, offset, SEEK_SET);
						read(fh,read_buf,size);
						//if(memcmp(read_buf,&(shm_memory[fw_offset]),size+0x100) !=0){
						if(memcmp(read_buf,&(shm_memory[fw_offset]),size) !=0){
							/* error */
							trueorfalse = 1;
							goto FINISH_COMPARE;
						}
					}else if (!memcmp(Header.signature, SQSH_SIGNATURE, SIG_LEN) ||
							!memcmp(Header.signature, SQSH_SIGNATURE_LE, SIG_LEN)){
						//printf("[%s]%d\n",__func__,__LINE__);
						read_buf = (unsigned char*)malloc(size+sizeof(rootfs_padding));
						if(read_buf == NULL){
							/* error */
							trueorfalse = 1;
							goto FINISH_COMPARE;
						}

						lseek(fh, offset, SEEK_SET);
						read(fh,read_buf,size+sizeof(rootfs_padding));

						if(memcmp(read_buf,&(shm_memory[fw_offset]),size-SIZE_OF_CHECKSUM) != 0){
							/* error */
							trueorfalse = 1;
							goto FINISH_COMPARE;
						
						}

						if (padding_len) {
							BDBG_BSN("[%s:%d] pad\n", __FUNCTION__, __LINE__);
							if(memcmp(read_buf+size-SIZE_OF_CHECKSUM,&rootfs_padding,sizeof(rootfs_padding)) != 0){
								/* error */
								trueorfalse =1;
								goto FINISH_COMPARE;
							}
						}
						else {
							BDBG_BSN("[%s:%d] nopad\n", __FUNCTION__, __LINE__);
							if(memcmp(read_buf+size-SIZE_OF_CHECKSUM,(void *)(&rootfs_padding)+sizeof(rootfs_padding.zero_pad),
								sizeof(rootfs_padding)-sizeof(rootfs_padding.zero_pad)) != 0){
								/* error */
								trueorfalse = 1;
								goto FINISH_COMPARE;
							}
						}
					}else {
						//printf("[%s]%d\n",__func__,__LINE__);
						BDBG_BSN("\n[%s:%d] not handle!!!\n", __FUNCTION__, __LINE__);
					}

FINISH_COMPARE:
					if(read_buf){
						free(read_buf);
						read_buf = NULL;
					}
		
					if(trueorfalse == 1){
						printf("[%s]:%d,flash/memory data is different\n",__func__,__LINE__);
						memset(Header.signature,0,SIG_LEN);
						lseek(fh, offset, SEEK_SET);
						write(fh,&Header,sizeof(Header));

						sync();	
						#ifdef KERNEL_3_10
						if(ioctl(fh,BLKFLSBUF,NULL) < 0){
							printf("flush mtd system cache error\n");
						}
						#endif

						fp_watchdog = fopen("/proc/watchdog_reboot","w+"); //reboot
						if(fp_watchdog)
						{
							fputs("111", fp_watchdog);
							fclose(fp_watchdog);
						}
						
						for(;;);
					}
#endif
#endif				
					close(fh);					 
				 	
				 	memset(mtd_name,0x00,sizeof(mtd_name));
				 	memset(offset_str,0x00,sizeof(offset_str));
				 	memset(size_str,0x00,sizeof(size_str));
				 	memset(fw_offset_str,0x00,sizeof(fw_offset_str));
				 	
				 	fgets(mtd_name,20,fp2);
					fgets(offset_str,20,fp2);
					fgets(size_str,20,fp2);
					fgets(fw_offset_str,20,fp2);
					
				}while( !feof(fp2) );
				
				fclose(fp2);
#if 0//defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)
                if(stat("/var/rtl_link_restart_block", &status) < 0){
                    fp_watchdog = fopen("/proc/watchdog_reboot","w+"); //reboot
                    if(fp_watchdog)
                    {
                        fputs("111", fp_watchdog);
                        fclose(fp_watchdog);
                    }
                    for(;;);
                }else{
                    unlink("/var/rtl_link_restart_block");
                }
#else	
												
				fp_watchdog = fopen("/proc/watchdog_reboot","w+"); //reboot
				if(fp_watchdog)
				{
					fputs("111", fp_watchdog);
					fclose(fp_watchdog);
				}
				for(;;);
#endif				
			}
			else
			{
				fp_watchdog = fopen("/proc/watchdog_reboot","w+"); //reboot
				if(fp_watchdog)
				{
					fputs("111", fp_watchdog);
					fclose(fp_watchdog);
				}
				
				for(;;);
				
			}
			
#if 0//defined(CONFIG_ELINK_SUPPORT) || defined(CONFIG_ANDLINK_SUPPORT)		
                         unlink("/var/fwd.ready");
                         if(shm_memory){
                             shmdt(shm_memory);
                             shm_memory = NULL;
                         }
                         if(shm_id){
                             shmctl(shm_id, IPC_RMID, 0);
                             shm_id = 0;
                         }
#endif			
				
			fclose(fp);
		}
#ifdef CONFIG_ELINK_SUPPORT
        unlink("/var/fwd.ready");
#endif			
	}
}
	
