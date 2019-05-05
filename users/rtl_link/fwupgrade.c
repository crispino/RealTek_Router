#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "apmib.h"
#include "fwupgrade.h"
#include "rtl_link_utils.h"

#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
#include <sys/mman.h>
#endif

#ifdef DMALLOC
#include "../../../dmalloc-5.5.2/dmalloc.h"
#endif

static int fwChecksumOk(char *data, int len)
{
	unsigned short sum=0;
	int i;

	for (i=0; i<len; i+=2) {
#ifdef _LITTLE_ENDIAN_
		sum += WORD_SWAP( *((unsigned short *)&data[i]) );
#else
		sum += *((unsigned short *)&data[i]);
#endif

	}
	return( (sum==0) ? 1 : 0);
}

static int get_actvie_bank()
{
	FILE *fp;
	char buffer[2];
	int bootbank;
	fp = fopen("/proc/bootbank", "r");
	
	if (!fp) {
		ERR_PRINT("ERROR[%d] (%s)%d Read /proc/bootbank failed\n", errno, __FUNCTION__, __LINE__);
	}else
	{
			//fgets(bootbank, sizeof(bootbank), fp);
			fgets(buffer, sizeof(buffer), fp);
			fclose(fp);
	}
	bootbank = buffer[0] - 0x30;	
	if ( bootbank ==1 || bootbank ==2)
		return bootbank;
	else
		return 1;	
}


static void get_bank_info(int dual_enable,int *active,int *backup)
{
	int bootbank=0,backup_bank;
	
	bootbank = get_actvie_bank();	

	if(bootbank == 1 )
	{
		if( dual_enable ==0 )
			backup_bank =1;
		else
			backup_bank =2;
	}
	else if(bootbank == 2 )
	{
		if( dual_enable ==0 )
			backup_bank =2;
		else
			backup_bank =1;
	}
	else
	{
		bootbank =1 ;
		backup_bank =1 ;
	}	

	*active = bootbank;
	*backup = backup_bank;	

	//fprintf(stderr,"get_bank_info active_bank =%d , backup_bank=%d  \n",*active,*backup); //mark_debug	   
}

static int check_system_image(int fh,IMG_HEADER_Tp pHeader)
{
	// Read header, heck signature and checksum
	int ret=0;		
	char image_sig[4]={0};
	char image_sig_root[4]={0};
	
        /*check firmware image.*/
//	if ( read(fh, pHeader, sizeof(IMG_HEADER_T)) != sizeof(IMG_HEADER_T)) 
//     		return 0;	

	memcpy(image_sig, FW_HEADER, SIGNATURE_LEN);
	memcpy(image_sig_root, FW_HEADER_WITH_ROOT, SIGNATURE_LEN);

	if (!memcmp(pHeader->signature, image_sig, SIGNATURE_LEN))
		ret=1;
	else if  (!memcmp(pHeader->signature, image_sig_root, SIGNATURE_LEN))
		ret=2;
	else{
		ERR_PRINT("ERROR (%s)%d no sys signature at !\n", __FUNCTION__, __LINE__);
	}

	DEBUG("(%s)%d the active bankmark is %x\n", __FUNCTION__, __LINE__, pHeader->burnAddr);
	
    if(pHeader->burnAddr == FORCEBOOT_BANK_MARK){
	    pHeader->burnAddr = BASIC_BANK_MARK;
	    DEBUG("(%s)%d change the active bankmark to %x\n", __FUNCTION__, __LINE__, pHeader->burnAddr);
	} 
	
    //mark_dual , ignore checksum() now.(to do) 
	return (ret);
}

static int get_image_header(int fh,IMG_HEADER_Tp header_p)
{
	int ret=0;
	//check 	CODE_IMAGE_OFFSET2 , CODE_IMAGE_OFFSET3 ?
	//ignore check_image_header () for fast get header , assume image are same offset......	
	// support CONFIG_RTL_FLASH_MAPPING_ENABLE ? , scan header ...
#ifndef CONFIG_MTD_NAND
	lseek(fh, CODE_IMAGE_OFFSET, SEEK_SET);		
#else
	lseek(fh, CODE_IMAGE_OFFSET-NAND_BOOT_SETTING_SIZE, SEEK_SET);	
#endif

    if ( read(fh, header_p, sizeof(IMG_HEADER_T)) != sizeof(IMG_HEADER_T)){
        ERR_PRINT("ERROR (%s)%d read img header error!\n", __FUNCTION__, __LINE__);
     		return 0;
    }

	ret = check_system_image(fh,header_p);

	//assume , we find the image header in CODE_IMAGE_OFFSET
#ifndef CONFIG_MTD_NAND
	lseek(fh, CODE_IMAGE_OFFSET, SEEK_SET);	
#else
	lseek(fh, CODE_IMAGE_OFFSET-NAND_BOOT_SETTING_SIZE, SEEK_SET);	
#endif

    if ( write(fh, header_p, sizeof(IMG_HEADER_T)) != sizeof(IMG_HEADER_T)){
        ERR_PRINT("ERROR (%s)%d write img header error!\n", __FUNCTION__, __LINE__);
        return 0;
    }

	return ret;	
}

static unsigned long header_to_mark(int  flag, IMG_HEADER_Tp pHeader)
{
	unsigned long ret_mark=NO_IMAGE_BANK_MARK;
	//mark_dual ,  how to diff "no image" "image with no bank_mark(old)" , "boot with lowest priority"
	if(flag) //flag ==0 means ,header is illegal
	{
		if( (pHeader->burnAddr & GOOD_BANK_MARK_MASK) )
			ret_mark=pHeader->burnAddr;	
		else
			ret_mark = OLD_BURNADDR_BANK_MARK;
	}
	return ret_mark;
}

static unsigned long get_next_bankmark(char *kernel_dev,int dual_enable)
{
    unsigned long bankmark=NO_IMAGE_BANK_MARK;
    int ret=0,fh;
    IMG_HEADER_T header; 	
	
	fh = open(kernel_dev, O_RDWR);
	if ( fh == -1 ) {
	    ERR_PRINT("ERROR[%d] (%s)%d Open file failed!\n", errno, __FUNCTION__, __LINE__);
		return NO_IMAGE_BANK_MARK;
	}
	ret = get_image_header(fh,&header);	

	bankmark= header_to_mark(ret, &header);	
	close(fh);
	//get next boot mark

	if( bankmark < BASIC_BANK_MARK)
		return BASIC_BANK_MARK;
	else if( (bankmark ==  FORCEBOOT_BANK_MARK) || (dual_enable == 0)) //dual_enable = 0 ....
	{
		return FORCEBOOT_BANK_MARK;//it means dual bank disable
	}
	else{
		return bankmark+1;
    }
	
}

int rtl_link_download_fw(char *url, char *fw_dir)
{    
    char cmdbuf[URL_SIZE_MAX+128]={0};
    char *fw_name = NULL;
	char tmpstr[10]={0};
    
    fw_name=strrchr(url, '/');
	if (fw_name)
    	fw_name++;
    sprintf(fw_dir, "%s%s", STORE_PATH, fw_name);
    unlink(fw_dir);

	snprintf(tmpstr, sizeof(tmpstr), "%s", fw_name);
	if(!strstr(tmpstr, ".bin"))
		return -1;
	
	snprintf(cmdbuf, sizeof(cmdbuf), "wget -P %s -o %s %s &", STORE_PATH, DOWNLOAD_LOG, url);
	system(cmdbuf);
//	snprintf(cmdbuf, sizeof(cmdbuf), "wget -q -P %s %s", STORE_PATH, url);
//  system(cmdbuf);  

	return 0;
}

static int daemonKilled = 0;
//static void killDaemon()
static void killDaemon(int wait)
{
#ifndef WIFI_SIMPLE_CONFIG
    int pid;
#endif
		
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
            		|| (strstr(strtmp,"elink") != 0) 
            		|| (strstr(strtmp,"andlink") != 0)
            		|| (strstr(strtmp, "root") == 0)
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

static void kill_processes(void)
{


	printf("upgrade: killing tasks...\n");
	
	kill(1, SIGTSTP);		/* Stop init from reforking tasks */
	kill(1, SIGSTOP);		
	kill(2, SIGSTOP);		
	kill(3, SIGSTOP);		
	kill(4, SIGSTOP);		
	kill(5, SIGSTOP);		
	kill(6, SIGSTOP);		
	kill(7, SIGSTOP);		
	//atexit(restartinit);		/* If exit prematurely, restart init */
	sync();

	signal(SIGTERM,SIG_IGN);	/* Don't kill ourselves... */
	setpgrp(); 			/* Don't let our parent kill us */
	sleep(1);
	signal(SIGHUP, SIG_IGN);	/* Don't die if our parent dies due to
					 * a closed controlling terminal */
	
}

int rtl_link_do_upgrade(char *fw_dir)
{
    char *buff=NULL;
    int fd = -1, fileLen, readLen;
    int len, locWrite, numLeft, numWrite=0, head_offset=0, ret=0;
    int flag, isValidfw, isIncludeRoot;
    int fwSizeLimit = CONFIG_FLASH_SIZE;
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
    int active_bank,backup_bank;
    int dual_enable =0;
#endif
    int fh = -1;
    int startAddr=-1, startAddrWeb=-1;
    struct stat fileStat={0};
    IMG_HEADER_Tp pHeader;

#ifdef MTD_NAME_MAPPING
    char webpageMtd[16],linuxMtd[16],rootfsMtd[16];
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
    char webpageMtd2[16],linuxMtd2[16],rootfsMtd2[16];
#endif
#endif

#if defined(CONFIG_APP_FWD)
#define FWD_CONF "/var/fwd.conf"
    int newfile = 1;
    int shm_id = 0;
    char *shm_memory=NULL;
#else
    killDaemon(1);
	#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
	    // do nothing
	#else
		shutdown_netdev();
		system("ifconfig br0 down 2> /dev/null");
	#endif
#endif

#ifdef MTD_NAME_MAPPING
    if(rtl_name_to_mtdblock(FLASH_WEBPAGE_NAME,webpageMtd) == 0
            || rtl_name_to_mtdblock(FLASH_LINUX_NAME,linuxMtd) == 0
            || rtl_name_to_mtdblock(FLASH_ROOTFS_NAME,rootfsMtd) == 0){
        ERR_PRINT("ERROR (%s)%d cannot find webpage/linux/rootfs mtdblock!", __FUNCTION__, __LINE__);
        goto ret_upload;
    }
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
    if(rtl_name_to_mtdblock(FLASH_WEBPAGE_NAME2,webpageMtd2) == 0
            || rtl_name_to_mtdblock(FLASH_LINUX_NAME2,linuxMtd2) == 0
            || rtl_name_to_mtdblock(FLASH_ROOTFS_NAME2,rootfsMtd2) == 0){
        ERR_PRINT("ERROR (%s)%d cannot find webpage/linux/rootfs mtdblock!", __FUNCTION__, __LINE__);
        goto ret_upload;
    }
#endif
#endif    

#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
    apmib_get(MIB_DUALBANK_ENABLED,(void *)&dual_enable);   
    get_bank_info(dual_enable,&active_bank,&backup_bank); 
    DEBUG("(%s)%d active_bank = %d, backup_bank = %d\n", __FUNCTION__, __LINE__, active_bank, backup_bank);
#endif

#ifdef MTD_NAME_MAPPING
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
    Kernel_dev_name[0] = linuxMtd;
    Kernel_dev_name[1] = linuxMtd2;
    Rootfs_dev_name[0] = rootfsMtd;
    Rootfs_dev_name[1] = rootfsMtd2;
    printf("Kernel_dev_name[0]=%s\n", Kernel_dev_name[0]);
    printf("Kernel_dev_name[1]=%s\n", Kernel_dev_name[1]);
    printf("Rootfs_dev_name[0]=%s\n", Rootfs_dev_name[0]);
    printf("Rootfs_dev_name[1]=%s\n", Rootfs_dev_name[1]);
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_WEB_BACKUP_ENABLE
    Web_dev_name[0] = webpageMtd;
    Web_dev_name[1] = webpageMtd2;
#endif
#endif
#endif

    //read firmware
    stat(fw_dir, &fileStat);
    fileLen = fileStat.st_size;
    fd = open(fw_dir, O_RDONLY);
    if(fd == -1){
        ERR_PRINT("ERROR[%d] (%s)%d open fw %s failed\n", errno, __FUNCTION__, __LINE__, fw_dir);
        ret = -1;
        goto ret_upload;
    }

#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
    buff = (char *)mmap(NULL, fileLen, PROT_READ, MAP_SHARED, fd, 0);
    if(buff == (void*)-1){
        ERR_PRINT("ERROR[%d] (%s)%d mmap failed\n", errno, __FUNCTION__, __LINE__);
        goto ret_upload;
    }
#else
#if defined(CONFIG_APP_FWD)
    /* Generate a System V IPC key */ 
    key_t key;
    key = ftok("/bin/fwd", 0xE04);

    /* Allocate a shared memory segment */
    shm_id = shmget(key, fileLen, IPC_CREAT | IPC_EXCL | 0666); 

    if (shm_id == -1) {
        ERR_PRINT("ERROR(%d) (%s)%d get sharememory error!\n",  errno, __FUNCTION__, __LINE__);
        ret = -1;
        goto ret_upload;
    }
    /* Attach the shared memory segment */
    shm_memory = (char *)shmat(shm_id, NULL, 0);
    buff=shm_memory;
#else
    buff=malloc(fileLen);
    if(!buff){
        ERR_PRINT("ERROR[%d] (%s)%d malloc\n", errno, __FUNCTION__, __LINE__);
        ret = -1;
        goto ret_upload;
    }
    bzero(buff,fileLen);
#endif

    readLen=read(fd,buff,fileLen);
    if(readLen!=fileLen){
        ERR_PRINT("ERROR (%s)%d read %d but file len is %d, read fail!\n",  __FUNCTION__, __LINE__, readLen,fileLen);
        ret = -1;
        goto ret_upload;
    }
#endif

    while ((head_offset+sizeof(IMG_HEADER_T)) <  fileLen){
        //check firmware
        locWrite = 0;
        pHeader = (IMG_HEADER_Tp) &buff[head_offset];
        len = pHeader->len;
#ifdef _LITTLE_ENDIAN_
        len  = DWORD_SWAP(len);
#endif
        numLeft = len + sizeof(IMG_HEADER_T);
        if (!memcmp(&buff[head_offset], FW_HEADER, SIGNATURE_LEN) ||
                !memcmp(&buff[head_offset], FW_HEADER_WITH_ROOT, SIGNATURE_LEN)) {
            isValidfw = 1;
            flag = 1;
        }
        else if (!memcmp(&buff[head_offset], WEB_HEADER, SIGNATURE_LEN)) {
            isValidfw = 1;
            flag = 2;
        }
        else if (!memcmp(&buff[head_offset], ROOT_HEADER, SIGNATURE_LEN)) {
            isValidfw = 1;
            flag = 3;
            isIncludeRoot = 1;
        }else{
            ERR_PRINT("ERROR (%s)%d Invalid file !\n",  __FUNCTION__, __LINE__);
            ret = -1;
            goto ret_upload;
        }

        if (len > fwSizeLimit) { //len check by sc_yang 
            ERR_PRINT("ERROR (%s)%d Image len exceed max size 0x%x ! len=0x%x", __FUNCTION__, __LINE__, fwSizeLimit, len);
            ret = -1;
            goto ret_upload;
        }
        if ( (flag == 1) || (flag == 3)) {
            if ( !fwChecksumOk(&buff[sizeof(IMG_HEADER_T)+head_offset], len)) {
                ERR_PRINT("ERROR (%s)%d Image checksum mismatched! len=0x%x, checksum=0x%x", __FUNCTION__, __LINE__, 
                        len, *((unsigned short *)&buff[len-2]) );
                ret = -1;
                goto ret_upload;
            }
        }
        else {
            char *ptr = &buff[sizeof(IMG_HEADER_T)+head_offset];
            if ( !CHECKSUM_OK((unsigned char *)ptr, len) ) {
                ERR_PRINT("ERROR (%s)%d Image checksum mismatched! len=0x%x", __FUNCTION__, __LINE__, len);
                ret = -1;
                goto ret_upload;
            }
        }
        //set mtd dev
#ifndef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
        if (flag == 3)  //rootfs
        {
#ifndef MTD_NAME_MAPPING
            fh = open(FLASH_DEVICE_NAME1, O_RDWR);
#else
            fh = open(rootfsMtd, O_RDWR);
#endif

#if defined(CONFIG_APP_FWD)
#ifndef MTD_NAME_MAPPING
            write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME1);
#else
            write_line_to_file(FWD_CONF, (newfile==1?1:2), rootfsMtd);
#endif
            newfile = 2;
#endif			
        }
        else if(flag == 1)  // linux
        {
#ifndef MTD_NAME_MAPPING
            fh = open(FLASH_DEVICE_NAME, O_RDWR);
#else
            fh = open(linuxMtd, O_RDWR);
#endif
#if defined(CONFIG_APP_FWD)	
#ifndef MTD_NAME_MAPPING
            write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME);
#else
            write_line_to_file(FWD_CONF, (newfile==1?1:2), linuxMtd);
#endif
            newfile = 2;
#endif			
        }
        else    // web
        {
#ifndef MTD_NAME_MAPPING
            fh = open(FLASH_DEVICE_NAME, O_RDWR);
#else
            fh = open(webpageMtd, O_RDWR);
#endif
#if defined(CONFIG_APP_FWD)	
#ifndef MTD_NAME_MAPPING
            write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME);
#else
            write_line_to_file(FWD_CONF, (newfile==1?1:2), webpageMtd);
#endif
            newfile = 2;
#endif			
        }
#else
        if (flag == 3) //rootfs
        {
            fh = open(Rootfs_dev_name[backup_bank-1], O_RDWR);

#if defined(CONFIG_APP_FWD)			
            write_line_to_file(FWD_CONF, (newfile==1?1:2), Rootfs_dev_name[backup_bank-1]);
            newfile = 2;
#endif			
        }        
        else if (flag == 1) //linux
        {
            fh = open(Kernel_dev_name[backup_bank-1], O_RDWR);
#if defined(CONFIG_APP_FWD)			
            write_line_to_file(FWD_CONF, (newfile==1?1:2), Kernel_dev_name[backup_bank-1]);
            newfile = 2;
#endif			
        }
        else //web
        {
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_WEB_BACKUP_ENABLE)
            fh = open(Web_dev_name[backup_bank-1],O_RDWR);
#if defined(CONFIG_APP_FWD)			
            write_line_to_file(FWD_CONF, (newfile==1?1:2), Web_dev_name[backup_bank-1]);
            newfile = 2;
#endif			
#else		
#ifndef MTD_NAME_MAPPING
            fh = open(FLASH_DEVICE_NAME, O_RDWR);
#else
            fh = open(webpageMtd,O_RDWR);
#endif
#if defined(CONFIG_APP_FWD)			
#ifndef MTD_NAME_MAPPING
            write_line_to_file(FWD_CONF, (newfile==1?1:2), FLASH_DEVICE_NAME);
#else
            write_line_to_file(FWD_CONF, (newfile==1?1:2), webpageMtd);
#endif
            newfile = 2;
#endif	
#endif		
        }
#endif

        //set startaddr     
        if ( fh == -1 ) {
            ERR_PRINT("ERROR(%d) (%s)%d File open failed, flag = %d!\n", errno, __FUNCTION__, __LINE__, flag);
            ret = -1;
            goto ret_upload;
        } else {
            if (flag == 1) {
                if (startAddr == -1) {
                    //startAddr = CODE_IMAGE_OFFSET;
                    startAddr = pHeader->burnAddr ;
#ifdef _LITTLE_ENDIAN_
                    startAddr = DWORD_SWAP(startAddr);
#endif
#ifdef CONFIG_MTD_NAND
                    startAddr = startAddr - NAND_BOOT_SETTING_SIZE;
#endif
                }
            }
            else if (flag == 3) {
                if (startAddr == -1) {
                    startAddr = 0; // always start from offset 0 for 2nd FLASH partition
                }
            }
            else {
                if (startAddrWeb == -1) {
                    //startAddr = WEB_PAGE_OFFSET;
                    startAddr = pHeader->burnAddr ;
#ifdef _LITTLE_ENDIAN_
                    startAddr = DWORD_SWAP(startAddr);
#endif
#ifdef CONFIG_MTD_NAND
                    startAddr = startAddr - NAND_BOOT_SETTING_SIZE;     
#endif
                }
                else
                    startAddr = startAddrWeb;
            }
            lseek(fh, startAddr, SEEK_SET);

#if defined(CONFIG_APP_FWD)			
            {
                char tmpStr[20]={0};
                snprintf(tmpStr, sizeof(tmpStr), "\n%d", startAddr);
                write_line_to_file(FWD_CONF, (newfile==1?1:2), tmpStr);

                newfile = 2;
            }
#endif	
            if (flag == 3) {
                locWrite += sizeof(IMG_HEADER_T); // remove header
                numLeft -=  sizeof(IMG_HEADER_T);
#ifndef CONFIG_APP_FWD		
                kill_processes();
                sleep(2);
#endif
            }
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
            if (flag == 1) {  //kernel image
                pHeader->burnAddr = get_next_bankmark(Kernel_dev_name[active_bank-1],dual_enable);  //replace the firmware header with new bankmark //mark_debug        
                DEBUG("(%s)%d next bankmark is %x\n", __FUNCTION__, __LINE__, pHeader->burnAddr);
            }
#endif

            //install fw by fwd
#if defined(CONFIG_APP_FWD)
            {
                char tmpStr[20]={0};

                snprintf(tmpStr, sizeof(tmpStr), "\n%d", numLeft);
                write_line_to_file(FWD_CONF, (newfile==1?1:2), tmpStr);
                snprintf(tmpStr, sizeof(tmpStr), "\n%d\n", locWrite+head_offset);
                write_line_to_file(FWD_CONF, (newfile==1?1:2), tmpStr);                 
                newfile = 2;
            }
#else
            //write to flash directly

            numWrite = write(fh, &(buff[locWrite+head_offset]), numLeft);
            if (numWrite < numLeft) {
                ERR_PRINT("ERROR (%s)%d File write failed. locWrite=%d numLeft=%d numWrite=%d Size=%d bytes.", \
                        __FUNCTION__, __LINE__, locWrite, numLeft, numWrite, fileLen);
                ret = -1;
                goto ret_upload;
            }
#endif //#if defined(CONFIG_APP_FWD)

            locWrite += numWrite;
            numLeft -= numWrite;
            sync();
            close(fh);

            head_offset += len + sizeof(IMG_HEADER_T) ;
            startAddr = -1 ; //by sc_yang to reset the startAddr for next image 
        }
    }

#if defined(CONFIG_APP_FWD)
    {           
        char tmpStr[256]={0};
        int tmpvalue;

        tmpvalue = 1;
        apmib_set(MIB_RTL_LINK_FW_UPGRADE, (void *)&tmpvalue);
        apmib_update(CURRENT_SETTING);
#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
        snprintf(tmpStr, sizeof(tmpStr), "%s", fw_dir);
        write_line_to_file("/var/rtl_link_shrinkmem", 1, " ");
#else
        snprintf(tmpStr, sizeof(tmpStr), "%d", shm_id);
#endif

        write_line_to_file("/var/fwd.ready", 1, tmpStr);

        sync();
    }
#endif	//#if defined(CONFIG_APP_FWD)

ret_upload: 
    if(fd != -1)
        close(fd);
    if(fh != -1)
        close(fh);
#if defined(RTL_LINK_UPGRADE_SHRINKMEM)
    if(buff)
        munmap(buff, fileLen); 
#else
#if defined(CONFIG_APP_FWD)
    if(shm_memory){
        if (shmdt(shm_memory) == -1)
            ERR_PRINT("ERROR[%d] (%s)%d shmdt failed\n", errno, __FUNCTION__, __LINE__);
    }

    if (shm_id != 0 && ret == -1) {     //when ret == -1 do shmctl RMID here, or do shmctl RMID in fwd
        if(shmctl(shm_id, IPC_RMID, 0) == -1)
            ERR_PRINT("ERROR[%d] (%s)%d shmctl(IPC_RMID) failed\n", errno, __FUNCTION__, __LINE__);
    }
#else 
    if(buff)
        free(buff);
#endif
#endif
    return ret;

}

