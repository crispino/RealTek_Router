#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <ont/mqtt.h>
#include <ont/video.h>
#include <ont/video_rvod.h>
#include <ont/log.h>
#include "cJSON.h"
#include "ont_list.h"
#include "update_cmd.h"




typedef void(*t_ont_onvif_live_stream_start)(void *dev, int tindex, int channel, const char *push_url, const char* deviceid);

typedef void(*t_ont_onvif_live_stream_ctrl)(void *dev, int tindex,  int channel, int stream);

typedef void(*t_ont_onvif_vod_stream_start)(void *dev, int tindex, int channel, t_ont_video_file *fileinfo, const char *playflag, const char *push_url, const char *deviceid);

typedef void(*t_ont_onvif_stream_make_keyframe)(void *dev, int tindex,  int channel);

typedef void(*t_ont_onvif_dev_ptz_ctrl)(void *dev, int tindex,  int channel, int mode, t_ont_video_ptz_cmd cmd, int speed);


typedef struct _t_ont_onvif_dev_ctrl_callbacks
{
	t_ont_onvif_live_stream_ctrl  stream_ctrl;
	t_ont_onvif_live_stream_start live_start;
	t_ont_onvif_stream_make_keyframe make_keyframe;
	t_ont_onvif_vod_stream_start     vod_start;
	t_ont_onvif_dev_ptz_ctrl      ptz_ctrl;
} t_ont_onvif_dev_ctrl_callbacks;




void ont_video_live_stream_start(void *dev, int tindex,  int channel, const char *push_url, const char* deviceid);
void ont_video_live_stream_ctrl(void *dev, int tindex,  int channel, int stream);
void ont_video_vod_stream_start(void *dev, int tindex,  int channel, t_ont_video_file *fileinfo, const char *playflag, const char *push_url, const char *deviceid);
void ont_video_stream_make_keyframe(void *dev, int tindex,  int channel);
void ont_video_dev_ptz_ctrl(void *dev, int tindex, int channel, int mode, t_ont_video_ptz_cmd cmd, int speed);

t_ont_onvif_dev_ctrl_callbacks _gdevonvifCallback = {
	ont_video_live_stream_ctrl,
	ont_video_live_stream_start,
	ont_video_stream_make_keyframe,
	ont_video_vod_stream_start,
	ont_video_dev_ptz_ctrl
};



int ont_video_dev_set_channels(void *_dev, int channels)
{
	ont_device_t *dev = _dev;
	char dsname[32];
	int i = 0;
	cJSON *json = cJSON_CreateObject();

	cJSON_AddItemToObject(json, "dst", cJSON_CreateString("video"));
	cJSON_AddItemToObject(json, "beginTime", cJSON_CreateString(""));
	cJSON_AddItemToObject(json, "endTime", cJSON_CreateString(""));
	cJSON_AddItemToObject(json, "vedioDesc", cJSON_CreateString("Nothing to show"));
	char *jsonValue = cJSON_PrintUnformatted(json);
	for (i = 0; i < channels; i++)
	{
		ont_platform_snprintf(dsname, sizeof(dsname), "ont_video_%d_mqtt_test_video", i + 1);
		ont_device_add_dp_object(dev, dsname, jsonValue);
	}
	ont_device_send_dp(dev);
	ont_platform_free(jsonValue);
	cJSON_Delete(json);

	return 0;
}


int ont_video_dev_fileinfo_upload(void *_dev, int channel, t_ont_video_file *list, int n)
{
	ont_device_t *dev = _dev;
	char dsname[64];
	int i = 0;

	ont_platform_snprintf(dsname, sizeof(dsname), "ont_video_%d_mqtttestvideo", channel);

	cJSON *json = NULL;
	char *jsonValue = NULL;
	//"beginTime" : "2016-10-19 16:30:30",
	//"endTime" : "2016-10-20 16:30:30",
	//"vedioDesc" : "video2"
	for (i = 0; i < n; i++)
	{
		json = cJSON_CreateObject();
		cJSON_AddItemToObject(json, "dst", cJSON_CreateString("video"));
		cJSON_AddItemToObject(json, "beginTime", cJSON_CreateString(list[i].begin_time));
		cJSON_AddItemToObject(json, "endTime", cJSON_CreateString(list[i].end_time));
		cJSON_AddItemToObject(json, "vedioDesc", cJSON_CreateString(list[i].descrtpion));
		jsonValue = cJSON_PrintUnformatted(json);
		ont_device_add_dp_object(dev, dsname, jsonValue);
		ont_platform_free(jsonValue);
		cJSON_Delete(json);
	}
	ont_device_send_dp(dev);
	return 0;
}


static int _ont_videocmd_stream_ctrl(ont_device_t *dev, int tindex, cJSON *cmd)
{
	ONT_LOG2(ONTLL_INFO, "channel %d, level %d ", cJSON_GetObjectItem(cmd, "channel_id")->valueint,
	         cJSON_GetObjectItem(cmd, "level")->valueint);

	_gdevonvifCallback.stream_ctrl(dev, tindex,
	                               cJSON_GetObjectItem(cmd, "channel_id")->valueint, cJSON_GetObjectItem(cmd, "level")->valueint);
	return 0;
}

static int _ont_videcmd_stream_live_start(ont_device_t *dev, int tindex, cJSON *cmd)
{
	//   "channel_id": 1,            //Êý×Ö
	//   "pushUrl":   "rtmp://xxx"  //×Ö·û´®
	uint8_t devid[32];
	ont_platform_snprintf(devid, 32, "%d", dev->device_id);
	ONT_LOG3(ONTLL_INFO, "live channel %d ttl %d,url%s", cJSON_GetObjectItem(cmd, "channel_id")->valueint, cJSON_GetObjectItem(cmd, "ttl")->valueint,
	         cJSON_GetObjectItem(cmd, "pushUrl")->valuestring);

	_gdevonvifCallback.live_start(dev, tindex,
	                              cJSON_GetObjectItem(cmd, "channel_id")->valueint,
	                              cJSON_GetObjectItem(cmd, "pushUrl")->valuestring,
	                              devid);
	return 0;
}

static int _ont_videcmd_stream_make_keyframe(ont_device_t *dev, int tindex, cJSON *cmd)
{

	_gdevonvifCallback.make_keyframe(dev, tindex,
	                                 cJSON_GetObjectItem(cmd, "channel_id")->valueint);
	return 0;
}

static int _ont_videcmd_stream_vodstart(ont_device_t *dev, int tindex, cJSON *cmd)
{
	ONT_LOG0(ONTLL_ERROR, "rvod not support");
	return 0;
}

static int _ont_videcmd_ptz_ctrl(ont_device_t *dev, int tindex, cJSON *cmd)
{
	ONT_LOG4(ONTLL_INFO, "live channel %d, cmd %d, stop%d, speed",
	         cJSON_GetObjectItem(cmd, "channel_id")->valueint,
	         cJSON_GetObjectItem(cmd, "cmd")->valueint,
	         cJSON_GetObjectItem(cmd, "stop")->valueint,
	         cJSON_GetObjectItem(cmd, "speed")->valueint);

	_gdevonvifCallback.ptz_ctrl(dev, tindex,
	                            cJSON_GetObjectItem(cmd, "channel_id")->valueint,
	                            cJSON_GetObjectItem(cmd, "stop")->valueint,
	                            cJSON_GetObjectItem(cmd, "cmd")->valueint,
	                            cJSON_GetObjectItem(cmd, "speed")->valueint);
	return 0;
}

int ont_get_channleid(void *dev, const char*str)
{
	cJSON *json = NULL;
	cJSON *cmd = NULL;
	int channelid = 0;
	json = cJSON_Parse(str);
	do
	{
		if (cJSON_HasObjectItem(json, "type"))
		{
			if (strcmp(cJSON_GetObjectItem(json, "type")->valuestring, "video"))
			{
				break;
			}
		}
		else
		{
			break;
		}
		if (!cJSON_HasObjectItem(json, "cmdId"))
		{
			break;
		}

		if (!cJSON_HasObjectItem(json, "cmd"))
		{
			break;
		}
		cmd = cJSON_GetObjectItem(json, "cmd");
		if (cmd)
		{
			channelid = cJSON_GetObjectItem(cmd, "channel_id")->valueint;
		}
	} while (0);

	cJSON_Delete(json);

	return channelid;
}

int ont_videocmd_update(cJSON* cmd_json)
{
	cJSON *cmd = NULL;
	if (!cJSON_HasObjectItem(cmd_json, "cmd"))
	{
		return 1;
	}
	
	cmd = cJSON_GetObjectItem(cmd_json, "cmd");

	return update_cmd_push("./update.cmd"
					   ,cJSON_GetObjectItem(cmd,"package_name")->valuestring
					   ,cJSON_GetObjectItem(cmd, "md5")->valuestring
					   ,cJSON_GetObjectItem(cmd, "url")->valuestring);
}

int ont_videocmd_update_cfg(cJSON* cmd_json)
{
	cJSON *cmd = NULL;
	if (!cJSON_HasObjectItem(cmd_json, "onvif"))
	{
		return 1;
	}
	
	cmd = cJSON_GetObjectItem(cmd_json, "onvif");

	if (!cfg_over_write("config.json",cJSON_Print(cmd_json),strlen(cJSON_Print(cmd_json))))
	{
		exit(0);
	}
	return 0;
}

int ont_videocmd_handle(void *dev, int tindex, const char*str)
{
	cJSON *json = NULL;
	cJSON *cmd = NULL;
	int cmdid = 0;
	json = cJSON_Parse(str);
	do
	{
		if (cJSON_HasObjectItem(json, "type"))
		{
			//!for update
			if (0 == strcmp(cJSON_GetObjectItem(json, "type")->valuestring, "update"))
			{
				return ont_videocmd_update(json);
			}

			if (0 == strcmp(cJSON_GetObjectItem(json, "type")->valuestring, "update_cfg"))
			{
				return ont_videocmd_update_cfg(json);
			}

			if (strcmp(cJSON_GetObjectItem(json, "type")->valuestring, "video"))
			{
				break;
			}
		}
		else
		{
			break;
		}
		if (!cJSON_HasObjectItem(json, "cmdId"))
		{
			break;
		}
		cmdid = cJSON_GetObjectItem(json, "cmdID")->valueint;
		if (!cJSON_HasObjectItem(json, "cmd"))
		{
			break;
		}
		cmd = cJSON_GetObjectItem(json, "cmd");
		switch (cmdid)
		{
		case 1:
			_ont_videcmd_stream_live_start(dev, tindex, cmd);
			break;
		case 2:
			_ont_videcmd_stream_vodstart(dev, tindex, cmd);
			break;
		case 5:
			_ont_videcmd_stream_make_keyframe(dev, tindex, cmd);
			break;
		case 6:
			_ont_videocmd_stream_ctrl(dev, tindex, cmd);
			break;
		case 7:
			_ont_videcmd_ptz_ctrl(dev, tindex, cmd);
			break;
		default:
			break;
		}
	} while (0);
	cJSON_Delete(json);
	return 0;
}

