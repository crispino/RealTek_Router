#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#ifdef CONFIG_MODEL_HN120_341FA
typedef enum {BAND2G=1, BAND5G, DUALBAND} WIFI_BAND_T;
#define RESET_GPIONUM "36" //gpioE2
#endif
#ifdef CONFIG_MODEL_HN120_341BA
#ifdef CONFIG_RTL_8196E
#define RESET_GPIONUM "5" //gpioa5
#define WPS_GPIONUM "5"   //gpioa5
#define LEDWPS_GPIONUM "9"  //gpioA6
#else
//#define RESET_GPIONUM "17" //gpioc1
//#define WPS_GPIONUM "17"   //gpioc1
#define RESET_GPIONUM "19" //gpioc3
#define WPS_GPIONUM "19"   //gpioc3
#define LED2G_GPIONUM "33" //gpioE1
#if defined (CONFIG_MODEL_HN100_340EA)
	#define REDLED2G_GPIONUM "34" //gpioE2
	//#define REDLED2G_GPIONUM "35" //gpioE3
	#define LEDWPS_GPIONUM "15"  //gpioB7
	//#define LEDWPS_GPIONUM "17"  //gpioc1
#else
	#define REDLED2G_GPIONUM "35" //gpioE3
	//#define LEDWPS_GPIONUM "15"  //gpioB7
	#define LEDWPS_GPIONUM "17"  //gpioc1
#endif
#endif
#endif

#ifdef CONFIG_MODEL_HN120_341FA
int do_wifi_led_on(char *str)
{
	int band = atoi(str);
	if (band == BAND2G) {
		//2.4G	Blue on
		system("echo 55 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio55/direction");
		system("echo 0 > /sys/class/gpio/gpio55/value");
		
		//2.4G Red off
		system("echo 56 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio56/direction");
		system("echo 1 > /sys/class/gpio/gpio56/value");
	} else if (band == BAND5G) {
		//5G Blue on
		system("echo 57 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio57/direction");
		system("echo 0 > /sys/class/gpio/gpio57/value");
		
		//5G Red off
		system("echo 58 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio58/direction");
		system("echo 1 > /sys/class/gpio/gpio58/value");
	} else if (band == DUALBAND) {
		//2.4G	Blue on
		system("echo 55 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio55/direction");
		system("echo 0 > /sys/class/gpio/gpio55/value");
		
		//2.4G Red off
		system("echo 56 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio56/direction");
		system("echo 1 > /sys/class/gpio/gpio56/value");
		
		//5G Blue on
		system("echo 57 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio57/direction");
		system("echo 0 > /sys/class/gpio/gpio57/value");
		
		//5G Red off
		system("echo 58 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio58/direction");
		system("echo 1 > /sys/class/gpio/gpio58/value");
	}
	return 0;
}
int do_wifi_led_off(char *str)
{
	int band = atoi(str);
	if (band == BAND2G) {
		//2.4G	Blue off
		system("echo 55 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio55/direction");
		system("echo 1 > /sys/class/gpio/gpio55/value");
		
		//2.4G Red off
		system("echo 56 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio56/direction");
		system("echo 1 > /sys/class/gpio/gpio56/value");

	} else if (band == BAND5G) {
		//5G Blue off
		system("echo 57 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio57/direction");
		system("echo 1 > /sys/class/gpio/gpio57/value");

		//5G Red off
		system("echo 58 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio58/direction");
		system("echo 1 > /sys/class/gpio/gpio58/value");
		
	} else if (band == DUALBAND) {
		//2.4G	Blue off
		system("echo 55 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio55/direction");
		system("echo 1 > /sys/class/gpio/gpio55/value");

		//2.4G Red off
		system("echo 56 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio56/direction");
		system("echo 1 > /sys/class/gpio/gpio56/value");

		//5G Blue off
		system("echo 57 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio57/direction");
		system("echo 1 > /sys/class/gpio/gpio57/value");

		//5G Red off
		system("echo 58 > /sys/class/gpio/export");
		system("echo out > /sys/class/gpio/gpio58/direction");
		system("echo 1 > /sys/class/gpio/gpio58/value");
	}

	return 0;
}
#endif
#ifdef CONFIG_MODEL_HN120_341BA
int do_wifi_led_on(char *str)
{
#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	system("echo 15  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio15/direction");
	system("echo 0	 > /sys/class/gpio/gpio15/value");
#endif

	return 0;
}

int do_wifi_led_off(char *str)
{
#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	system("echo 1   > /sys/class/gpio/gpio15/value");
#endif

	return 0;
}
#endif
#ifdef CONFIG_MODEL_HN120_341FA
int do_power_led_on(char *str)
{
#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 0	 > /sys/class/gpio/gpio33/value");
#endif
	return 0;
}

int do_led_on(char *str)
{
	char ledwps_active_low[50] = {0}, ledwps_value[50] = {0};

#if defined (CONFIG_MODEL_HN100_340EA)
	system("echo 0 > /sys/class/gpio/gpio33/value");
	system("echo 1 > /sys/class/gpio/gpio34/value");
#endif

#ifdef CONFIG_MODEL_HN350_339EA
	//´ò¿ªPOWERµÆ
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 0	 > /sys/class/gpio/gpio33/value");  //´ò¿ª
	
	//´ò¿ªWLANµÆ
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction");
	system("echo 0	 > /sys/class/gpio/gpio15/value");  //´ò¿ª
	
	//´ò¿ªWPSµÆ
	system("echo 17  > /sys/class/gpio/export"); 
	system("echo 1	 > /sys/class/gpio/gpio17/value");  //´ò¿ª
#endif

#ifdef CONFIG_MODEL_HN330_331EA	
	//´ò¿ªPOWERµÆ
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	//system("echo 1	 > /sys/class/gpio/gpio33/value");	//¹Ø±Õ
	system("echo 0	 > /sys/class/gpio/gpio33/value");	//´ò¿ª
	
	//´ò¿ªWLANµÆ
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	//system("echo 1	 > /sys/class/gpio/gpio15/value");	//¹Ø±Õ
	system("echo 0	 > /sys/class/gpio/gpio15/value");	//´ò¿ª
	
	//´ò¿ªLANµÆ
	system("echo 55  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio55/direction");
	system("echo 0	 > /sys/class/gpio/gpio55/value");	//´ò¿ª
	//system("echo 1	 > /sys/class/gpio/gpio55/value");	//¹Ø±Õ	
#endif

#ifdef CONFIG_MODEL_HN120_341FA
	//Power
	system("echo 35 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio35/direction");
	system("echo 0 > /sys/class/gpio/gpio35/value");

	//2.4G	Blue on
	system("echo 55 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio55/direction");
	system("echo 0 > /sys/class/gpio/gpio55/value");

	//2.4G Red off
	system("echo 56 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio56/direction"); 
	system("echo 1 > /sys/class/gpio/gpio56/value");

	//5G Blue on
	system("echo 57 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio57/direction");
	system("echo 0 > /sys/class/gpio/gpio57/value");

	//5G Red off
	system("echo 58 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio58/direction"); 
	system("echo 1 > /sys/class/gpio/gpio58/value");
#endif

	return;
}

int do_led_off(char *str)
{
	char ledwps_active_low[50] = {0}, ledwps_value[50] = {0};

#if defined (CONFIG_MODEL_HN100_340EA)
	system("echo 1 > /sys/class/gpio/gpio33/value");
	system("echo 0 > /sys/class/gpio/gpio34/value");
#endif
	
#ifdef CONFIG_MODEL_HN350_339EA
	//¹Ø±ÕPOWERµÆ
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 1	 > /sys/class/gpio/gpio33/value");	//¹Ø±Õ
	
	//¹Ø±ÕWLANµÆ
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	system("echo 1	 > /sys/class/gpio/gpio15/value");	//¹Ø±Õ
	
	//¹Ø±ÕWPSµÆ
	system("echo 17  > /sys/class/gpio/export"); 
	system("echo 0	 > /sys/class/gpio/gpio17/value");	//¹Ø±Õ
#endif

#ifdef CONFIG_MODEL_HN330_331EA
	//¹Ø±ÕPOWERµÆ
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 1	 > /sys/class/gpio/gpio33/value");	//¹Ø±Õ
	
	//¹Ø±ÕWLANµÆ
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	system("echo 1	 > /sys/class/gpio/gpio15/value");	//¹Ø±Õ
	
	//¹Ø±ÕLANµÆ
	system("echo 55  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio55/direction");
	system("echo 1	 > /sys/class/gpio/gpio55/value");	//¹Ø±Õ	
#endif

#ifdef CONFIG_MODEL_HN120_341FA
	//Power
	system("echo 35 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio35/direction");
	system("echo 1 > /sys/class/gpio/gpio35/value");

	//wps
	system("echo 34 > /sys/class/gpio/export");
	//system("echo out > /sys/class/gpio/gpio34/direction");
	system("echo 0 > /sys/class/gpio/gpio34/value");

	//2.4G	Blue off
	system("echo 55 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio55/direction");
	system("echo 1 > /sys/class/gpio/gpio55/value");

	//2.4G Red off
	system("echo 56 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio56/direction"); 
	system("echo 1 > /sys/class/gpio/gpio56/value");

	//5G Blue off
	system("echo 57 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio57/direction");
	system("echo 1 > /sys/class/gpio/gpio57/value");

	//5G Red off
	system("echo 58 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio58/direction"); 
	system("echo 1 > /sys/class/gpio/gpio58/value");
#endif

	return;
}
#endif
#ifdef CONFIG_MODEL_HN120_341BA
int do_power_led_on(char *str)
{
#if defined (CONFIG_MODEL_HN350_339EA) || defined (CONFIG_MODEL_HN330_331EA)
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 0	 > /sys/class/gpio/gpio33/value");
#endif
	return 0;
}

int do_led_on(char *str)
{
	char ledwps_active_low[50] = {0}, ledwps_value[50] = {0};

#ifdef CONFIG_RTL_8196E
	system("ew 0xb8000044 0x000036DB;"
		"ew 0xb8003500 0xFFFF838B;"
		"ew 0xb8003508 0xFF007C40;"
		"ew 0xb800350c 0x00005C7C;");

#else					
	system("ew 0xb8000834 0x33320000;"
		"ew 0xb8000838 0x20000000;"
		"ew 0xb800351c 0xF83FFFFF;"
		"ew 0xb8003524 0x07C00000;"
		"ew 0xb8003528 0x07C00000;");
#endif

#if defined (CONFIG_MODEL_HN100_340EA)
	system("echo 0 > /sys/class/gpio/gpio33/value");
	system("echo 1 > /sys/class/gpio/gpio34/value");
#endif

#ifdef CONFIG_MODEL_HN350_339EA
	//打开POWER灯
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 0	 > /sys/class/gpio/gpio33/value");  //打开
	
	//打开WLAN灯
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction");
	system("echo 0	 > /sys/class/gpio/gpio15/value");  //打开
	
	//打开WPS灯
	system("echo 17  > /sys/class/gpio/export"); 
	system("echo 1	 > /sys/class/gpio/gpio17/value");  //打开
#endif

#ifdef CONFIG_MODEL_HN330_331EA	
	//打开POWER灯
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	//system("echo 1	 > /sys/class/gpio/gpio33/value");	//关闭
	system("echo 0	 > /sys/class/gpio/gpio33/value");	//打开
	
	//打开WLAN灯
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	//system("echo 1	 > /sys/class/gpio/gpio15/value");	//关闭
	system("echo 0	 > /sys/class/gpio/gpio15/value");	//打开
	
	//打开LAN灯
	system("echo 55  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio55/direction");
	system("echo 0	 > /sys/class/gpio/gpio55/value");	//打开
	//system("echo 1	 > /sys/class/gpio/gpio55/value");	//关闭	
#endif

#ifdef CONFIG_MODEL_HN120_341BA
	//Power
	system("echo 15 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio15/direction");
	system("echo 0 > /sys/class/gpio/gpio15/value");

	//wps on
	system("echo 1 > /proc/gpio");

	//2.4G	Green on
	system("echo 33 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction"); 
	system("echo 0 > /sys/class/gpio/gpio33/value");

	//2.4G Red off
	system("echo 34 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio34/direction"); 
	system("echo 1 > /sys/class/gpio/gpio34/value");

	//5G Green on
	system("echo 35 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio35/direction"); 
	system("echo 0 > /sys/class/gpio/gpio35/value");

	//5G Red off
	system("echo 36 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio36/direction"); 
	system("echo 1 > /sys/class/gpio/gpio36/value");
#endif

	return;
}

int do_led_off(char *str)
{
	char ledwps_active_low[50] = {0}, ledwps_value[50] = {0};

#ifdef CONFIG_RTL_8196E
	system("ew 0xb8000044 0x000036DB;"
		"ew 0xb8003500 0xFFFF838B;"
		"ew 0xb8003508 0xFF007C40;"
		"ew 0xb800350c 0x00005C7C;");

#else					
	system("ew 0xb8000834 0x33320000;"
		"ew 0xb8000838 0x20000000;"
		"ew 0xb800351c 0xF83FFFFF;"
		"ew 0xb8003524 0x07C00000;"
		"ew 0xb8003528 0x07C00000;");
#endif

#if defined (CONFIG_MODEL_HN100_340EA)
	system("echo 1 > /sys/class/gpio/gpio33/value");
	system("echo 0 > /sys/class/gpio/gpio34/value");
#endif
	
#ifdef CONFIG_MODEL_HN350_339EA
	//关闭POWER灯
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 1	 > /sys/class/gpio/gpio33/value");	//关闭
	
	//关闭WLAN灯
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	system("echo 1	 > /sys/class/gpio/gpio15/value");	//关闭
	
	//关闭WPS灯
	system("echo 17  > /sys/class/gpio/export"); 
	system("echo 0	 > /sys/class/gpio/gpio17/value");	//关闭
#endif

#ifdef CONFIG_MODEL_HN330_331EA
	//关闭POWER灯
	system("echo 33  > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 1	 > /sys/class/gpio/gpio33/value");	//关闭
	
	//关闭WLAN灯
	system("echo 15  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio15/direction"); 
	system("echo 1	 > /sys/class/gpio/gpio15/value");	//关闭
	
	//关闭LAN灯
	system("echo 55  > /sys/class/gpio/export"); 
	system("echo out > /sys/class/gpio/gpio55/direction");
	system("echo 1	 > /sys/class/gpio/gpio55/value");	//关闭	
#endif

#ifdef CONFIG_MODEL_HN120_341BA
	//Power
	system("echo 15 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio15/direction");
	system("echo 1 > /sys/class/gpio/gpio15/value");

	//wps
	system("echo 0 > /proc/gpio");

	//2.4G	Green off
	system("echo 33 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio33/direction");
	system("echo 1 > /sys/class/gpio/gpio33/value");

	//5G Green off
	system("echo 35 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio35/direction");
	system("echo 1 > /sys/class/gpio/gpio35/value");

	//2.4G Red off
	system("echo 34 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio34/direction"); 
	system("echo 1 > /sys/class/gpio/gpio34/value");

	//5G Red off
	system("echo 36 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio36/direction"); 
	system("echo 1 > /sys/class/gpio/gpio36/value");
#endif

	return;
}
#endif
int do_log_on(char *str)
{
    int tty = -1;
    char *tty_name = NULL;

    /* »ñÈ¡µ±Ç°ttyÃû³Æ */
    tty_name = ttyname(STDOUT_FILENO);
	printf("tty_name: %s\n", tty_name);

	/* ÖØ¶¨Ïòconsoleµ½µ±Ç°tty */
	tty = open(tty_name, O_RDONLY | O_WRONLY);
	ioctl(tty, TIOCCONS);
	perror("ioctl TIOCCONS");
	close(tty);
	
	return 0;
}

int do_log_off(char *str)
{
    int tty = -1;
    char *tty_name = NULL;
	
    tty = open("/dev/console", O_RDONLY | O_WRONLY);
	ioctl(tty, TIOCCONS);
	perror("ioctl TIOCCONS");
	close(tty);
	
	return 0;
}

typedef struct cmd_node {
	char *cmd_name;
	struct cmd_node *next;
	int next_count;
	int (*call_back)(char *str);
}cmd_node_t;

cmd_node_t cmd_led[] = {
	{"off", NULL, 0,  do_led_off},
	{"on", NULL,0, do_led_on},
};

cmd_node_t cmd_wifi[] = {
	{"off", NULL, 0,  do_wifi_led_off},
	{"on", NULL,0, do_wifi_led_on},
};

cmd_node_t cmd_power[] = {
	{"on", NULL,0, do_power_led_on},
};

cmd_node_t cmd_logs[] = {
	{"on", NULL,0, do_log_on},
	{"off", NULL,0, do_log_off},
};

cmd_node_t cmds[] = {
	{"led", (struct cmd_node *)&cmd_led, 2, NULL},
	{"wifi", (struct cmd_node *)&cmd_wifi, 2, NULL},
	{"power", (struct cmd_node *)&cmd_power, 1, NULL},
	{"log", (struct cmd_node *)&cmd_logs, 2, NULL},	
};

int print_hint()
{
	int ret = 0;
	printf("\n\r");
	printf("------------------------------------------------------------------------------\n\r"); 
	printf("You have to disable the button default action before starting test,\nwill you disable the button default action? Y(yes)/N(no)/E(exit)\n");
	printf("------------------------------------------------------------------------------\n\r");

	char str[20];
	scanf("%s", str);
	if(strcmp("Y", str) == 0 || strcmp("y", str) == 0 || strcmp("yes", str) == 0 || strcmp("Yes", str) == 0)
		ret = 1;
	else if (strcmp("N", str) == 0 || strcmp("n", str) == 0 || strcmp("no", str) == 0 || strcmp("No", str) == 0)
		ret = 0;
	else if (strcmp("E", str) == 0 || strcmp("e", str) == 0 || strcmp("Exit", str) == 0 || strcmp("exit", str) == 0)
		ret = 2;
	else 
		ret = 0;
	
	return ret;
}

void print_help()
{
	printf("\n\r");
	printf("------------------------------------------------------------------------------\n\r");    
	printf("| 1:get button status\n\r");
	printf("| 2:power on all LEDs\n\r");	
	printf("| 3:power off all LEDs\n\r");	
	printf("| 4:enable button default action\n\r");	
	printf("------------------------------------------------------------------------------\n\r");
	return; 
}

int parse_cmd(int argc, char **argv)
{
	int ctr = 0;
	unsigned char flg_find = 0;
	int cmd1_ctr = sizeof(cmds)/sizeof(cmd_node_t);
	int len;
	int ret = -1;
	
	for (ctr = 0; ctr < cmd1_ctr; ctr++) {
		printf("compare %s and %s\n", argv[1], cmds[ctr].cmd_name);
		if (!strcmp(argv[1], cmds[ctr].cmd_name)) {
			flg_find = 1;
			break;
		}
	}
	printf("flg_find = %d, ctr = %d\n", flg_find, ctr);
	if (flg_find) {
		flg_find = 0;
		len = cmds[ctr].next_count;
		cmd_node_t *p_cmd = (cmd_node_t*)cmds[ctr].next;
		for (ctr = 0; ctr < len; ctr++) {
			//printf("compare %s and %s\n", argv[2], (p_cmd+ctr)->cmd_name);
			if (!strcmp(argv[2], (p_cmd+ctr)->cmd_name)) {
				flg_find = 1;
				if ((p_cmd+ctr)->call_back) {
					if (argc == 4) {
						ret = (p_cmd+ctr)->call_back(argv[3]);
					} else {
						char *p_void;
						ret = (p_cmd+ctr)->call_back(p_void);
					}
				}
				break; 
			}
		}
	}
	if (!flg_find) {
		printf("input command is invalid\n");
		print_help();
	}
	return 0; 
}


int main(int argc, char **argv)
{
	if (argc >= 2) {
		parse_cmd(argc, argv);
		return;
	}

	int cmd;
	int action = 0;
	int i;
	FILE *fp;
	#ifdef CONFIG_MODEL_HN120_341BA
	int ledwps_active = 0;
	char reset_export[50], reset_in[50], reset_active_low[50], reset_value[50];
	char wps_export[50], wps_in[50], wps_active_low[50], wps_value[50];
	
	sprintf(reset_export, "echo %s > /sys/class/gpio/export", RESET_GPIONUM);
	sprintf(reset_in, "echo in > /sys/class/gpio/gpio%s/direction", RESET_GPIONUM);
	sprintf(reset_active_low, "echo 1 > /sys/class/gpio/gpio%s/active_low", RESET_GPIONUM);
	sprintf(reset_value, "/sys/class/gpio/gpio%s/value", RESET_GPIONUM);
	
	sprintf(wps_export, "echo %s > /sys/class/gpio/export", WPS_GPIONUM);
	sprintf(wps_in, "echo in > /sys/class/gpio/gpio%s/direction", WPS_GPIONUM);
	sprintf(wps_active_low, "echo 1 > /sys/class/gpio/gpio%s/active_low", WPS_GPIONUM);
	sprintf(wps_value, "/sys/class/gpio/gpio%s/value", WPS_GPIONUM);
	
#ifdef CONFIG_RTL_8197F
	char led2g_export[50], led2g_out[50], redled2g_export[50], redled2g_out[50];
	sprintf(led2g_export, "echo %s > /sys/class/gpio/export", LED2G_GPIONUM);
	sprintf(led2g_out, "echo out > /sys/class/gpio/gpio%s/direction", LED2G_GPIONUM);
	//only for zowee, Red LED
	sprintf(redled2g_export, "echo %s > /sys/class/gpio/export", REDLED2G_GPIONUM);
	sprintf(redled2g_out, "echo out > /sys/class/gpio/gpio%s/direction", REDLED2G_GPIONUM);	
#endif

	char ledwps_export[50], ledwps_out[50], ledwps_active_low[50], ledwps_value[50];
	sprintf(ledwps_export, "echo %s > /sys/class/gpio/export", LEDWPS_GPIONUM);
	sprintf(ledwps_out, "echo out > /sys/class/gpio/gpio%s/direction", LEDWPS_GPIONUM);
	sprintf(ledwps_active_low, "/sys/class/gpio/gpio%s/active_low", LEDWPS_GPIONUM);
	system(ledwps_export);
	system(ledwps_out);

	//you should check reset button and wps button
	fp=fopen(ledwps_active_low,"r");
	if(fp){
		fscanf(fp,"%d",&ledwps_active);		
	}else{
		//printf("<%s>LZQ: Open  ledwps_active_low file error!\n", __FUNCTION__);
	}
	#endif

#ifdef CONFIG_MODEL_HN120_341FA
	char reset_export[50], reset_in[50], reset_active_low[50], reset_value[50];
	
	sprintf(reset_export, "echo %s > /sys/class/gpio/export", RESET_GPIONUM);
	//sprintf(reset_in, "echo in > /sys/class/gpio/gpio%s/direction", RESET_GPIONUM);
	sprintf(reset_value, "/sys/class/gpio/gpio%s/value", RESET_GPIONUM);
#endif
HINT:
	cmd = print_hint();
	switch(cmd){
		case 0:
			goto HINT;
			break;
		case 1:
			system("echo 0 > /proc/enable_rstbtn");
			system("kill -s 9 `ps -aux | grep timelycheck | awk '{print $1}'`");
			system(reset_export);
			//system(reset_in);
			#ifdef CONFIG_MODEL_HN120_341BA
			system(reset_in);
			system(wps_export);
			system(wps_in);
			#endif
TESTLOOP:
			print_help();
			char str[32] = {0};
			scanf("%s", str);
			if (!strcmp(str, "e") || !strcmp(str, "E") || !strcmp(str, "exit") || !strcmp(str, "Exit") || !strcmp(str, "EXIT"))
				action = 5;
			else
				action = atoi(str);

			switch(action){
				case 1:
					while(1)
					{
			#ifdef CONFIG_MODEL_HN120_341BA
						/* 因为339BA产品上，RESET，WPS按钮复用，所以只检测一个GPIO值。若是不同的GPIO值，则需要按不同的按键。 */
						system(reset_active_low);		
			#endif
						fp = fopen(reset_value, "r");
						if(fp){
							fscanf(fp,"%d",&i);
							if (i == 1){  //1:pressed, 0:unpressed
								printf("reset button is pressed.\n");
								printf("wps button is pressed.\n");
								goto TESTLOOP;
							}else{
								//printf("reset button error.\n");
								//goto TESTLOOP;
							}
						}else{
							//printf("button error. \n");
							//goto TESTLOOP;
						}

						fclose(fp);
						usleep(100*1000);
					}
				case 2:
					system("kill -s 9 `ps -aux | grep timelycheck | awk '{print $1}'`");
					do_led_on(NULL);

					#ifdef CONFIG_MODEL_HN120_341FA
						//wps on
						system("echo 34 > /sys/class/gpio/export");
						system("echo 1 > /sys/class/gpio/gpio34/value");
					#endif
					
					goto TESTLOOP;
				case 3:
					system("kill -s 9 `ps -aux | grep timelycheck | awk '{print $1}'`");
					do_led_off(NULL);
					#ifdef CONFIG_MODEL_HN120_341FA
						//2.4G Red on
						system("echo 56 > /sys/class/gpio/export");
						system("echo out > /sys/class/gpio/gpio56/direction");
						system("echo 0 > /sys/class/gpio/gpio56/value");

						//5G Red on
						system("echo 58 > /sys/class/gpio/export");
						system("echo out > /sys/class/gpio/gpio58/direction"); 
						system("echo 0 > /sys/class/gpio/gpio58/value");
					#endif
					#ifdef CONFIG_MODEL_HN120_341BA
						//2.4G Red on
						system("echo 34 > /sys/class/gpio/export");
						system("echo out > /sys/class/gpio/gpio34/direction"); 
						system("echo 0 > /sys/class/gpio/gpio34/value");

						//5G Red on
						system("echo 36 > /sys/class/gpio/export");
						system("echo out > /sys/class/gpio/gpio36/direction"); 
						system("echo 0 > /sys/class/gpio/gpio36/value");
					#endif
					goto TESTLOOP;
				case 4:
					system("echo 1 > /proc/enable_rstbtn");

					#ifdef CONFIG_MODEL_HN120_341FA
					//wps off
					system("echo 34 > /sys/class/gpio/export");
					system("echo 0 > /sys/class/gpio/gpio34/value");
					#endif

					system("timelycheck &");
					do_led_on(NULL);
					
					goto HINT;
					break;
				case 5:
					exit(0);
					break;

				default:
					goto TESTLOOP;
			}
			break;
		case 2:
			exit(0);
			break;
		default:
			goto HINT;
			break;
	}

	return 0;
}      
