#include <stdio.h>
#include <unistd.h>

#include "cJSON.h"
#include "apmib.h"


#define NO_NEED_RESET		0
#define NEED_RESET			1

char json_url[288] = {0};
char server[256] = {"ftp://192.168.234.234"};

int debug_mode = 2;
int action = NEED_RESET;
char *URL = NULL;


static void get_path()
{
	char *Json_name = NULL;

#if defined (CONFIG_MODEL_HN100_340EA)
	Json_name = "/HN100_Json.txt";
#elif defined (CONFIG_MODEL_HN120_341FA) || defined (CONFIG_MODEL_HN120_341BA)
	Json_name = "/HN120_Json.txt";
#elif defined (CONFIG_MODEL_HN350_339EA)
	Json_name = "/HN350_Json.txt";
#elif defined (CONFIG_MODEL_HN330_331EA)
	Json_name = "/HN330_Json.txt";
#elif defined (CONFIG_MODEL_HN140_361EA)
	Json_name = "/HN140_Json.txt";
#else
	Json_name = "/ZOWEE_Json.txt";
#endif

	snprintf(json_url, sizeof(json_url), "%s%s", server, Json_name);
}

static int get_version()
{
	FILE *fp;
	int version = 0;

	fp = fopen("/etc/version", "r");
	if (fp == NULL)
	{
		printf("Open version failed\n");
		return -1;
	}

	/* Revision: 9206 */
	fscanf(fp, "%*s%d", &version);
	fclose(fp);

	return version;
}

static int match_sn(char *sn)
{
	char my_sn[32] = {0};
	char *delimiter1, *delimiter2;
	apmib_get(MIB_CUSTOMER_HW_SERIAL_NUM, (void *)my_sn);
	printf("local sn: %s\n", my_sn);

	delimiter1 = strchr(sn, '-');
	delimiter2 = strchr(sn, ',');

	//eg. 211118110000001
	if (delimiter1 == NULL && delimiter2 == NULL)
	{
		if (!strncmp(sn, my_sn, strlen(my_sn)))
			return 1; //matched.
	}

	//eg. 211118110000001,211118110000010,211118110000020
	if (delimiter1 == NULL && delimiter2)
	{
		if (strstr(sn, my_sn))
			return 1;
	}

	//eg. 211118110000001-211118110000010
	if (delimiter1 && delimiter2 == NULL)
	{
		if (strstr(sn, my_sn))
			return 1;

		char *ptr1, *ptr2, *ptr3;
		ptr1 = sn;
		ptr2 = strchr(sn, '-');
		ptr2[0] = '\0';
		ptr2+=1;
		if (ptr2[0] == '\0')
			return 0;

		if (!strncmp(my_sn, ptr1, 8) && !strncmp(my_sn, ptr2, 8))
		{
			ptr1+=8;
			ptr2+=8;
			ptr3 = &my_sn[0]+8;
			if (atoi(ptr1) < atoi(ptr3) && atoi(ptr3) < atoi(ptr2))
				return 1; //matched.
		}
	}

	//eg. 211118060002060,,,211118060002039-211118060002050,
	if (delimiter1 && delimiter2)
	{
		if (strstr(sn, my_sn))
			return 1;

		char *ptr1, *ptr2, *ptr3, *ptr4;
		ptr1 = sn;
		ptr3 = strchr(sn, ',');
		while(ptr3)
		{
			ptr3[0] = '\0';
			ptr3+=1;
			ptr2 = strchr(ptr1, '-');
			if (ptr2)
			{
				ptr2[0] = '\0';
				ptr2+=1;
				if (!strncmp(my_sn, ptr1, 8) && !strncmp(my_sn, ptr2, 8))
				{
					ptr1+=8;
					ptr2+=8;
					ptr4 = &my_sn[0]+8;
					if (atoi(ptr1) < atoi(ptr4) && atoi(ptr4) < atoi(ptr2))
						return 1; //matched.
				}
			}
			else
			{
				if (!strncmp(sn, my_sn, strlen(my_sn)))
					return 1; //matched.
			}

			ptr1 = ptr3;
			ptr3 = strchr(ptr1, ',');
		}

		/* once again */
		ptr2 = strchr(ptr1, '-');
		if (ptr2)
		{   
			ptr2[0] = '\0';
			ptr2+=1;
			if (!strncmp(my_sn, ptr1, 8) && !strncmp(my_sn, ptr2, 8)) 
			{   
				ptr1+=8;
				ptr2+=8;
				ptr4 = &my_sn[0]+8;
				if (atoi(ptr1) < atoi(ptr4) && atoi(ptr4) < atoi(ptr2))
					return 1; //matched.
			}   
		}   
		else
		{   
			if (!strncmp(sn, my_sn, strlen(my_sn)))
				return 1; //matched.
		}
	}

	return 0;
}

//format: "B0:D5:9D:00:00:01- B0:D5:9D:00:00:10, B0:D5:9D:00:12:01- B0:D5:9D:00:12:10"
static inline int match_mac(char *mac)
{
	char tmp1[64] = {0}, tmp2[64] = {0};

	apmib_get(MIB_HW_NIC0_ADDR, (void *)&tmp1);
	convert_bin_to_str(tmp1, 6, tmp2);
	printf("localmac: %s\n", tmp2);

	return 0;
}

/*
 * Desc: 解析文件内容为json格式，生成升级策略
 * Input: 
 	buf: 读取的文件buf。
 	buf_len: buf整个长度。
 * Ouput:
	return value 返回是否需要升级。
 	action: 全局变量返回升级后的动作。
 * Format:
 {
	 "fwVersion": "8099", //服务器中固件的内部版本号（svn号），设备本身的版本号与服务器中的不相同，则需要更新。
	 "SN": "211118110000001-211118110000010,211118110000100-211118110000180,", //可选。允许升级的SN（SN段）。使用"-"表示一个范围的SN，多个SN（SN段）间使用"，"隔开。空白表示无SN策略，只匹配MAC即可。
	 "MAC": "B0:D5:9D:00:00:01- B0:D5:9D:00:00:10, B0:D5:9D:00:12:01- B0:D5:9D:00:12:10", //可选。允许升级的MAC（MAC段）。使用"-"表示一个范围的MAC，多个MAC（MAC段）间使用"，"隔开。空白表示无MAC策略，只匹配SN即可。
	 "URL": "http://www.updatetest.com/firmware/HN100_V1.0.1_svn8099.bin", //升级固件路径，IP、域名均可。
	 "action": "0" //固件升级完成后，是否需要恢复出厂设置。0:不需要；1:需要。
 }
 * 
 */
static int get_json(cJSON *json, int buf_len)
{
	cJSON *item;
	int fwVersion = 0, localVersion;
	char *SN, *MAC;

	if (json == NULL)
	{
		printf("Json format error!\n");
		return 0;
	}

	item = cJSON_GetObjectItem(json, "fwVersion");
	if(item){
		if(item->type == cJSON_Number){
			fwVersion = item->valueint;
		}else if(item->type == cJSON_String){
			fwVersion = atoi(item->valuestring);
		}
	}
	else
	{
		printf("Json fwVersion format error!\n");
		return 0;
	}

	item = cJSON_GetObjectItem(json, "action");
	if(item){
		if(item->type == cJSON_Number){
			action = item->valueint;
		}else if(item->type == cJSON_String){
			action = atoi(item->valuestring);
		}
	}
	else
	{
		printf("Json action format error!\n");
		return 0;
	}

	item = cJSON_GetObjectItem(json, "SN");
	if(item){
		SN = item->valuestring;
	}
	else
	{
		printf("Json SN format error!\n");
		return 0;
	}

	item = cJSON_GetObjectItem(json, "MAC");
	if(item){
		MAC = item->valuestring;
	}
	else
	{
		printf("Json MAC format error!\n");
		return 0;
	}

	item = cJSON_GetObjectItem(json, "URL");
	if(item){
		URL = item->valuestring;
	}
	else
	{
		printf("Json URL format error!\n");
		return 0;
	}

	localVersion = get_version();
	printf("fwVersion: %d, localVersion: %d, SN: %s, MAC: %s, URL: %s, action: %d\n",
			fwVersion, localVersion, SN, MAC, URL, action);

	if (fwVersion > 0 && localVersion > 0 && fwVersion != localVersion)
	{
		if ((SN == NULL || 0 == strlen(SN)) && (MAC == NULL || 0 == strlen(MAC)))
			return 1; //need to be updated.
		if (match_sn(SN) || match_mac(MAC))
			return 1; //need to be updated.
	}
	
	return 0;
}

static void show_options(void)
{
	printf("\t-s Server address\n");
	printf("\t-d debug mode val, 0: no msg, 1: show error msg, 2: show error and debug msg\n");
	printf("\t-h Show this message\n");
}

int main(int argc, char *argv[])
{
	int arg, pid;
	FILE *fp;
	char file_dir[256] = {0};
	char *buf = NULL;
	int need_update = 0; //1: need to be updated.
	size_t buf_len, bufread;

	while ((arg = getopt(argc, argv, "s:d:h")) != -1)
	switch (arg)
	{
		case 's':
			strncpy(server, optarg, sizeof(server));
			break;
		case 'd':
			debug_mode = atoi(optarg);
			break;
		case 'h':
			show_options();
			return 0;
		default:
			printf("other option :%c\n", arg);
			show_options();
			return 0;
	}

    if (!apmib_init()) {
		printf("ERROR online_update (%s)%d Initialize AP MIB failed!\n", __FUNCTION__, __LINE__);
		return 0;
	}

	get_path();

	pid = fork();
	if (pid == 0)
	{
		int try_times = 24;
		while(1)
		{
			rtl_link_download_fw(json_url, file_dir);
			
			fp = fopen(file_dir, "r");
			if (fp == NULL)
			{
				//printf("Open file %s failed\n", file_dir);
				if (try_times-- <= 0)
					return -1;
				else
					sleep(5);
			}
			else
				break;
		}
		if (fseek(fp, 0L, SEEK_END) != 0)
		{
			fclose(fp);
			return -1;
		}
		buf_len = ftell(fp);
		if (buf_len <= 0)
		{
			fclose(fp);
			return -1;
		}
		if (fseek(fp, 0L, SEEK_SET) != 0)
		{
			fclose(fp);
			return -1;
		}

		buf = malloc(sizeof(char) * (buf_len + 1));
		if (buf == NULL)
		{
			printf("malloc failed\n");
			return -1;
		}
		buf[0] = '\0';

		bufread = fread(buf, sizeof(char), buf_len, fp);
		if (bufread != buf_len)
		{
			printf("bufread != bufled, %u %u\n%s\n", bufread, buf_len, buf);
			fclose(fp);
			return -1;
		}
		fclose(fp);

		
		cJSON *json = cJSON_Parse(buf);
		need_update = get_json(json, buf_len);

		printf("need_update: %d\n", need_update);
		if (need_update)
		{
			rtl_link_download_fw(URL, file_dir);
			if (!isFileExist(file_dir))
			{
				printf("Download failed.\n");
				return -1;
			}

			if (-1 == rtl_link_do_upgrade(file_dir))
			{
				printf("Upgrade failed.\n");
				sleep(2);
				system("reboot");
			}

			switch(action)
			{
				default:
				case NEED_RESET:
					printf("flash reset");
					system("flash reset");
				case NO_NEED_RESET:
					break;
			}
		}
		else
			printf("No need to be updated.\n");

		cJSON_Delete(json);
		free(buf);

		exit(0);
	}

	return 0;
}

