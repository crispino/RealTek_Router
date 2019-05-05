#ifndef _DEV_PROFILE_CONFIG_H_
#define _DEV_PROFILE_CONFIG_H_


#include "ont_list.h"

# ifdef __cplusplus
extern "C" {
# endif

struct _device_profile
{
	char  svraddr[1024];
	unsigned short  svrport;

	unsigned int  productid;
	char  regcode[1024];
};

int  devcfg_initilize(const char * cfgpath);

struct _device_profile * devcfg_get_profile();

# ifdef __cplusplus
}
# endif

#endif
