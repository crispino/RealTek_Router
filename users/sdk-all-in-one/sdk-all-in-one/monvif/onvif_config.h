#ifndef _ONVIF_CONFIG_H_
#define _ONVIF_CONFIG_H_

#include "ont_list.h"



# ifdef __cplusplus
extern "C" {
# endif


struct _onvif_channel
{
	int   channelid;
    int   defaultlevel;
	char  url[1024];
	char  user[128];
	char  pass[128];

};


struct _onvif_app
{
    unsigned int  port;
    unsigned int  maxthread;
};

int  cfg_initilize(const char * cfgpath);

int  cfg_over_write(const char * cfgpath,const char* cfg_data,const uint32_t cfg_data_size);

int  cfg_get_channel_num();
struct _onvif_channel * cfg_get_channels();
struct _onvif_channel * cfg_get_channel(int channelid);

int cfg_get_port();
int cfg_get_thread_num();


# ifdef __cplusplus
}
# endif

#endif
