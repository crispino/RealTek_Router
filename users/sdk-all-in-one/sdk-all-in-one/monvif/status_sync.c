
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>


#include <stdio.h>
#include <stdarg.h>
#include <ont/log.h>
#include "ont/platform.h"
#include "status_sync.h"


#define CHANNELINFO_SYNC_FILE "channelsync"


int  status_synced(uint32_t nchn)
{
	uint32_t chnnum;
	size_t rsize;
	long fsize = 0;
	FILE *hfile = fopen(CHANNELINFO_SYNC_FILE, "r+");
	if (hfile)
	{
		fseek(hfile, 0, SEEK_END);
		fsize = ftell(hfile);
		if (fsize >= sizeof(chnnum))
		{
			fseek(hfile, 0, SEEK_SET);
			rsize = fread(&chnnum, sizeof(chnnum), 1, hfile);
			if (nchn == chnnum)
			{
				return 1;
			}
			else
			{
				fwrite(&nchn, sizeof(nchn), 1, hfile);
				return 0;
			}
		}
		else
		{
			fwrite(&nchn, sizeof(nchn), 1, hfile);
		}
	}
	else
	{
		hfile = fopen(CHANNELINFO_SYNC_FILE, "w+");
		fwrite(&nchn, sizeof(nchn), 1, hfile);
		return 0;
	}

	fclose(hfile);
	return 0;
}


int  status_gethwaddr(char mac[32])
{
    struct ifreq ifreq;
    int sock = 0;
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
		ONT_LOG0(ONTLL_ERROR, "error sock");
        return 2;
    }
    /*need change*/
    strcpy(ifreq.ifr_name, "eth0");
    if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0)
    {
        snprintf(mac, 32, "dummy");
		ONT_LOG0(ONTLL_ERROR, "error ioctl");
        return 3;
    }

    int i = 0;
    for(i = 0; i < 6; i++){
        sprintf(mac+3*i, "%02X:", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
    }
    mac[strlen(mac) - 1] = 0;
    ONT_LOG1(ONTLL_INFO,"MAC: %s\n", mac);    
}