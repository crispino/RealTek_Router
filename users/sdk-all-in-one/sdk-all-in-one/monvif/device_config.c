#include "device_config.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdarg.h>

struct device_cfg_t
{
    struct _device_profile  profile;
};

struct device_cfg_t device_cfg;


int  devcfg_initilize(const char * cfgpath)
{
	int filesize = 0;
#ifdef ONT_DEBUG
    FILE *hfile = fopen(cfgpath, "r");
#else
    FILE *hfile = fopen("device_config.json", "r");
#endif

	int i = 0;
	cJSON *json = NULL;
    cJSON *item = NULL;
	char *url, *user, *pass;

	char *end = NULL;
	if (!hfile)
	{
		printf("not found the config file[config.json]\n");
        return 0;
	}

	fseek(hfile, 0, SEEK_END);
	filesize = ftell(hfile);
	fseek(hfile, 0, SEEK_SET);
	char *dstaddr = ont_platform_malloc(filesize+4);
	while (!feof(hfile))
	{
		fread(dstaddr + i, 1, 1, hfile);
		i++;
	}
	dstaddr[i] = '\0';

	json = cJSON_ParseWithOpts(dstaddr, &end, 0);
    if (!json)
    {
        return -1;
    }

    /*
     * Get device profile
     */
    item = cJSON_GetObjectItem(json, "profile");
    if (!item)
    {
        printf("config file error\n");
        getchar();
        exit(0);
    }
    ont_platform_snprintf(device_cfg.profile.svraddr, sizeof(device_cfg.profile.svraddr), cJSON_GetObjectItem(item, "svraddr")->valuestring);
    ont_platform_snprintf(device_cfg.profile.regcode, sizeof(device_cfg.profile.regcode), cJSON_GetObjectItem(item, "regcode")->valuestring);
    device_cfg.profile.svrport = cJSON_GetObjectItem(item, "svrport")->valueint;
    device_cfg.profile.productid = cJSON_GetObjectItem(item, "productid")->valueint;

	cJSON_Delete(json);
	fclose(hfile);
	ont_platform_free(dstaddr);
	return 0;
}

struct _device_profile * devcfg_get_profile()
{
    return &device_cfg.profile;
}

