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
#include "sample_config.h"
#ifdef WIN32
#include <winsock.h>
#include <windows.h>
int
gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm.tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#else
#include <sys/time.h>
#endif
typedef  struct
{
	char playflag[32];
	char publishurl[255];
	char deviceid[16];
	int channel;
	char rvod;
	char closeflag; //0, nothing; 1 server closed,
	void *playctx;
}t_playlist;

ont_list_t *g_list = NULL;

#define VARAIBLE_CHECK if(!g_list) { g_list = ont_list_create(); play_env = ont_onvifdevice_create_playenv();}


static int compareliveFlag(t_playlist *d1, t_playlist *d2);

static void *play_env = NULL;


static int ont_video_live_stream_start_level(void *dev, int channel, const char *push_url, const char* deviceid, int level)
{
	ont_onvif_playctx *ctx = NULL;
	t_playlist *data = NULL;
	t_playlist data2 = { 0 };
	ont_list_node_t *node;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long ts = (tv.tv_usec / 1000 + tv.tv_sec * 1000);
	VARAIBLE_CHECK;

	data2.channel = channel;
	node = ont_list_find(g_list, &data2, compareliveFlag);
	if (node)
	{
		ONT_LOG1(ONTLL_INFO, "live already start %d", channel);
		return 1;
	}

	do
	{
		ctx = ont_onvifdevice_live_stream_start(play_env, channel, push_url, deviceid, level);
		if (!ctx)
		{
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
		ont_list_insert(g_list, data);
		return 0;
	} while (0);
	//error
	return -1;
}

void ont_video_live_stream_start(void *dev, int channel, const char *push_url, const char* deviceid)
{
	/* try the 4 streams.*/
	int level = 4;
	do
	{
		if (ont_video_live_stream_start_level(dev, channel, push_url, deviceid, level) >=0)
		{
			break;
		}
		else
		{
			level--;
		}
	} while (level>0);
    if(level<=0)
    {
	    ONT_LOG1(ONTLL_ERROR, "live channel start err, %d", channel);
    }
}


static int compareliveFlag(t_playlist *d1, t_playlist *d2)
{
	if (d1->channel == d2->channel)
	{
		return 0;
	}
	return -1;
}


void ont_video_live_stream_ctrl(void *dev, int channel, int stream)
{
	VARAIBLE_CHECK;
	ont_list_node_t *node;
	t_playlist data, *finddata;
	VARAIBLE_CHECK;
	data.channel = channel;
	node = ont_list_find(g_list, &data, compareliveFlag);
	if (!node)
	{
		return;
	}
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

void ont_video_vod_stream_start(void *dev, int channel, t_ont_video_file *fileinfo, const char *playflag, const char *push_url, const char *deviceid)
{
	VARAIBLE_CHECK;
	t_playlist data2 ={0};
	ont_list_node_t *node;
	ont_platform_snprintf(data2.playflag, sizeof(data2.playflag), "%s", playflag);
	node = ont_list_find(g_list, &data2, startfindFlag);
	t_playlist *data = NULL;
	if (node)
	{
        ONT_LOG1(ONTLL_INFO, "vod already start %s ", playflag);
		return;
	}
	data = ont_platform_malloc(sizeof(t_playlist));
    memset(data, 0x00, sizeof(t_playlist));
	data->closeflag = 0;
	data->rvod = 1;
    t_rtmp_vod_ctx *ctx = rtmp_rvod_createctx();

	do
	{
		struct _onvif_rvod * vod = cfg_get_rvod(channel, fileinfo->begin_time, fileinfo->end_time);
		if (!vod)
		{
			ONT_LOG3(ONTLL_ERROR, "vod not found channel %d, begin %s, end %s ", channel, fileinfo->begin_time, fileinfo->end_time);
			break;
		}

		if (rtmp_rvod_parsefile(ctx, vod->location) < 0)
		{
			break;
		}

		if (rtmp_rvod_start(ctx, push_url, deviceid) < 0)
		{
			break;
		}
		ont_platform_snprintf(data->playflag, sizeof data->playflag, "%s", playflag);
		data->playctx = ctx;
		ont_platform_snprintf(data->publishurl, sizeof(data->publishurl), "%s", push_url);
		ont_platform_snprintf(data->deviceid, sizeof(data->deviceid), "%s", deviceid);
		ont_list_insert(g_list, data);
		return;
	} while (0);
	ont_platform_free(data);
	rtmp_rvod_destoryctx(ctx);
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


void ont_video_dev_ptz_ctrl(void *dev, int channel, int mode, t_ont_video_ptz_cmd cmd, int speed)
{
	ont_onvifdevice_ptz(channel, cmd, speed, 2);
}

static int delta = 0;
void vodsinglestep(void *data, void *context)
{
	if (!data)
	{
		return;
	}
	t_playlist *play = data;
	int localDelta = 0;
	if (play->rvod == 1)
	{
		localDelta = rtmp_rvod_send_media_singlestep(play->playctx);
		if (localDelta < 0)
		{
			play->closeflag = 1;
		}
	}
	else
	{
		delta = 0;
	}

	if (localDelta>0 && localDelta < delta)
	{
		delta = localDelta;
	}
}


int _checkcloseFlag(t_playlist *d1, t_playlist *d2)
{
	struct timeval tv;
	ont_onvif_playctx *playctx = d1->playctx;
    gettimeofday(&tv, NULL);
    unsigned long ts = (tv.tv_usec / 1000 + tv.tv_sec * 1000);

	if (d1->rvod && d1->closeflag == 1)
	{
		return 0;
	}
	if (!d1->rvod && playctx->state)
	{
		return 0; /*need closed*/
	}

	if (!d1->rvod && playctx->last_sndts>0 && ts>playctx->last_sndts && ts - playctx->last_sndts >5000)
    {
        ONT_LOG2(ONTLL_INFO, "live end, ts %lu, last %lu", ts, playctx->last_sndts);
        return 0;
    }

	return -1;
}

static int _checkneedclose(void *dev)
{
	t_playlist data, *finddata;
	ont_list_node_t *node;
	VARAIBLE_CHECK;
	do
	{
		node = ont_list_find(g_list, &data, _checkcloseFlag);
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
		finddata->closeflag = 0;
		ont_list_remove(g_list, node, NULL);
		ont_platform_free(finddata);
	} while (1);
	return 0;
}

int ont_video_playlist_singlestep(void *dev)
{
	VARAIBLE_CHECK;
	delta = 10;
	ont_list_foreach(g_list, vodsinglestep, dev);
	ont_onvifdevice_live_stream_singlestep(play_env, 0);
	_checkneedclose(dev);
	return delta;
}
