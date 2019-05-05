#include "onvif_config.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdarg.h>

struct onvif_cfg_t
{
	struct _onvif_channel*  channels;
    struct _onvif_app  app;

	int channelnum;
};

struct onvif_cfg_t onvif_cfg;


int  cfg_initilize(const char * cfgpath)
{
	int filesize = 0;
    FILE *hfile = fopen(cfgpath, "r");
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

    /*get system cofiguration*/

	//onvif_cfg.app.port= cJSON_GetObjectItem(json, "port")->valueint;
	//onvif_cfg.app.maxthread= cJSON_GetObjectItem(json, "maxthread")->valueint;

	/*
	 * Get onvif channels
	 */
	int j = cJSON_GetArraySize(cJSON_GetObjectItem(json, "onvif"));
	onvif_cfg.channelnum = j;
	onvif_cfg.channels = ont_platform_malloc(j * sizeof(struct _onvif_channel));


	for (i = 0; i < j; i++)
	{
		item = cJSON_GetArrayItem(cJSON_GetObjectItem(json, "onvif"), i);
		url = cJSON_GetObjectItem(item, "url")->valuestring;
		user = cJSON_GetObjectItem(item, "user")->valuestring;
		pass = cJSON_GetObjectItem(item, "passwd")->valuestring;

		onvif_cfg.channels[i].channelid = cJSON_GetObjectItem(item, "channel_id")->valueint;
        onvif_cfg.channels[i].defaultlevel = cJSON_GetObjectItem(item, "defaultlevel")->valueint;
		ont_platform_snprintf(onvif_cfg.channels[i].url, sizeof(onvif_cfg.channels[i].url), url);
		ont_platform_snprintf(onvif_cfg.channels[i].user, sizeof(onvif_cfg.channels[i].user), user);
		ont_platform_snprintf(onvif_cfg.channels[i].pass, sizeof(onvif_cfg.channels[i].pass), pass);

	}

	cJSON_Delete(json);
	fclose(hfile);
	ont_platform_free(dstaddr);
	return 0;
}


int  cfg_over_write(const char * cfgpath,const char* cfg_data,const uint32_t cfg_data_size)
{
	FILE *fp = fopen(cfgpath,"w+");
	if (fp == NULL)
	{
		return 2;
	}

	fwrite(cfg_data,cfg_data_size,1,fp);
	fclose(fp);
	return 0;
}

int  cfg_get_channel_num()
{
	return onvif_cfg.channelnum;
}


struct _onvif_channel * cfg_get_channels()
{
	return onvif_cfg.channels;
}

struct _onvif_channel * cfg_get_channel(int channelid)
{
    int i =0;
	for (; i < onvif_cfg.channelnum;i++)
	{
		if (onvif_cfg.channels[i].channelid == channelid)
		{
			return &onvif_cfg.channels[i];
		}
	}
	return NULL;
}


int cfg_get_port()
{
    return 9101;
}

int cfg_get_thread_num()
{
    return 4;
}

