#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ont/log.h>
#include <curl/curl.h>
#include "isp_info.h"
#include "cJSON.h"



static char *url_getip = "http://members.3322.org/dyndns/getip";
static char url_getip_info[256] = "http://ip.taobao.com/service/getIpInfo.php?ip=";

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


int  isp_curl(char *content, size_t size,  char *url)
{
    int n = 0;
    int err_no = 0; //0-正确
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK) {err_no = ONTISP_CURL_ERR_BAD;}
    else {
        n = ont_platform_snprintf(content, size, "%s", chunk.memory);
        if (n < 0) {err_no = ONTISP_CURL_ERR_OVER_RANGE;}
    }

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();
    return err_no;
}


/**
 * 删除尾部空白、\t、换行符
 */
void rtrim(char *s)
{
    int i;
    i = strlen(s)-1;
    while ((s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && i >= 0) {i--;};
    s[i+1] = '\0';
}


void isp_get_info(char *isp_info, size_t size)
{
    char rip[20] = "";
    int res = 0;
    int times = 5; //请求获取isp接口信息总次数

    res = isp_curl(rip, 20, url_getip);
    if (ONTISP_CURL_ERR_OK == res) {
        rtrim(rip);

        strcat(url_getip_info, rip);

        for (; times > 0; times--) {
            res = isp_curl(isp_info, size, url_getip_info);
            if (ONTISP_CURL_ERR_OK == res) { break; }
        }
    }
}



int isp_parse_info() {
    char isp_info[1024] = "";
    isp_get_info(isp_info, 1024);
	int ont_isp_id = ONTISP_UK;
    uint64_t isp_id;

	cJSON *json = NULL;
    cJSON *data = NULL;

	json = cJSON_Parse(isp_info);
    if (!json) { return ONTISP_CM;}

    if (cJSON_HasObjectItem(json, "data")) {
        data = cJSON_GetObjectItem(json, "data");

        isp_id = (uint64_t)atoll(cJSON_GetObjectItem(data, "isp_id")->valuestring);

        if (isp_id == ISP_CM) {
            ont_isp_id = ONTISP_CM;
        } else if (isp_id == ISP_CU) {
            ont_isp_id = ONTISP_CU;
        } else if (isp_id == ISP_CT) {
            ont_isp_id = ONTISP_CT;
        } else if (isp_id == ISP_CE) {
            ont_isp_id = ONTISP_CE;
        }
    }
    cJSON_Delete(json);

    return ont_isp_id;
}
