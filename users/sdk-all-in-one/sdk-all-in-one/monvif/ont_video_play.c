#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <ont/mqtt.h>
#include <ont/video.h>
#include <ont/video_rvod.h>
#include <ont/log.h>
#include "ont_list.h"
#include "ont_onvif_if.h"
#include "device_onvif.h"
#include "onvif_config.h"
#include "message.h"


extern threadinfo_t *g_threads;
typedef  struct
{
	char playflag[32];
	char publishurl[255];
	char deviceid[16];
	int channel;
	char rvod;
	char closeflag; //0 nothing; 1 server closed,
	void *playctx;
}t_playlist;


static int compareliveFlag(t_playlist *d1, t_playlist *d2);


static int ont_video_live_stream_start_level(void *dev, int tindex, int channel, const char *push_url, const char* deviceid, int level)
{
	ont_onvif_playctx *ctx = NULL;
	t_playlist *data = NULL;
	t_playlist data2 = { 0 };
	ont_list_node_t *node;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long ts = (tv.tv_usec / 1000 + tv.tv_sec * 1000);
    
	data2.channel = channel;
	node = ont_list_find(g_threads[tindex].list, &data2, compareliveFlag);
	if (node)
	{
		ONT_LOG1(ONTLL_INFO, "live already start %d", channel);
		return 1;
	}

	do
	{
		//test-begin
		
		//test-end
		//ctx = ont_onvifdevice_live_stream_start(g_threads[tindex].play_env, channel, "rtmp://127.0.0.1:1935/hls/teststream", "1069895", level);
		ctx = ont_onvifdevice_live_stream_start(g_threads[tindex].play_env, channel, push_url, deviceid, level);
		if (!ctx)
		{
		    ONT_LOG1(ONTLL_INFO, "live start error %d", channel);            
			break;
		}
		data = ont_platform_malloc(sizeof(t_playlist));
		memset(data, 0x00, sizeof(t_playlist));
		data->closeflag = 0;
		data->rvod = 0;
		data->playctx = ctx;
		data->channel = channel;
        ctx->last_sndts = ts;
		ont_platform_snprintf(data->publishurl, sizeof(data->publishurl), "%s", push_url);
		ont_platform_snprintf(data->deviceid, sizeof(data->deviceid), "%s", deviceid);
		ont_list_insert(g_threads[tindex].list, data);
		return 0;
	} while (0);
	//error
	return -1;
}

void ont_video_live_stream_start(void *dev, int tindex,  int channel, const char *push_url, const char* deviceid)
{
	/* try the 4 streams.*/
    if (!cfg_get_channel(channel))
    {
		ONT_LOG1(ONTLL_ERROR, "not found the channel %d", channel);        
        return ;
    }
	int level =cfg_get_channel(channel)->defaultlevel;
    ont_video_live_stream_start_level(dev, tindex, channel, push_url, deviceid, level);
}


static int compareliveFlag(t_playlist *d1, t_playlist *d2)
{
	if (d1->channel == d2->channel)
	{
		return 0;
	}
	return -1;
}


void ont_video_live_stream_ctrl(void *dev, int tindex, int channel, int stream)
{
	ont_list_node_t *node;
	t_playlist data, *finddata;
	data.channel = channel;
	node = ont_list_find(g_threads[tindex].list, &data, compareliveFlag);
	if (!node)
	{
		return;
	}
    cfg_get_channel(channel)->defaultlevel = stream; /*set the cache to the stream level*/
	finddata = ont_list_data(node);
	ont_onvifdevice_stream_ctrl(finddata->playctx, stream);
	return;
}



int startfindFlag(t_playlist *d1, t_playlist *d2)
{
	if (strcmp(d1->playflag, d2->playflag) == 0)
	{
		return 0;
	}
	return -1;
}

void ont_video_vod_stream_start(void *dev, int tindex, int channel, t_ont_video_file *fileinfo, const char *playflag, const char *push_url, const char *deviceid)
{
    ONT_LOG1(ONTLL_ERROR, "rvod not support", channel);            
	return ;
}

int compareFlag(t_playlist *d1, t_playlist *d2)
{
	if (strcmp(d1->playflag, d2->playflag) == 0)
	{
		return 0;
	}
	return -1;
}


void ont_video_stream_make_keyframe(void *dev, int channel)
{
	//need implement.
}


void ont_video_dev_ptz_ctrl(void *dev, int tindex, int channel, int mode, t_ont_video_ptz_cmd cmd, int speed)
{
    //TODO: if not play, not contrl the PTZ.
	ont_onvifdevice_ptz(channel, cmd, speed, 2);
}


int _checkcloseFlag(t_playlist *d1, t_playlist *d2)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long ts = (tv.tv_usec / 1000 + tv.tv_sec * 1000);

    /*check onvif if need close*/
    
    ont_onvif_playctx *playctx= (ont_onvif_playctx*)(d1->playctx);
	if (!d1->rvod && playctx->state == 1)
	{
		return 0;
	}
    /*not send data 10 secondes, need close*/
    if ( playctx->last_sndts>0&& ts>playctx->last_sndts && ts-playctx->last_sndts >5000)
    {
        ONT_LOG2(ONTLL_INFO, "live end, ts %ld, last %ld", ts, playctx->last_sndts);            
        return 0;
    }
	return -1;
}

int checkneedclose(ont_list_t *list)
{
	t_playlist data, *finddata;
	ont_list_node_t *node;
	do
	{
		node = ont_list_find(list, &data, _checkcloseFlag);
		if (!node)
		{
			return 0;
		}
		finddata = ont_list_data(node);
		if (finddata->rvod)
		{
			ONT_LOG1(ONTLL_INFO, "vod end %s ", finddata->playflag);
			rtmp_rvod_stop(finddata->playctx);
			rtmp_rvod_destoryctx(finddata->playctx);
		}
		else
		{
            ONT_LOG1(ONTLL_INFO, "live end, channel %d", finddata->channel);
			ont_onvifdevice_live_stream_stop(finddata->playctx);
		}
		finddata->playctx = NULL;
		ont_list_remove(list, node, NULL);
		ont_platform_free(finddata);
	} while (1); 
	return 0;
}





