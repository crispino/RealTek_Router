#include <stdio.h>
#include "gpiopin.h"

void usage()
{
	printf("\n Usage: led_control all|portnum on|off|recovery \n");

	return; 
}

void record_port(int portnum)
{
	char cmdbuf[128];
	
	/*set pinmux to GPIO*/
	snprintf(cmdbuf, sizeof(cmdbuf), "echo %d > /sys/class/gpio/export", portnum); 
	system(cmdbuf);	
}

void set_port(int portnum, int value)
{
	char cmdbuf[128];
	
	record_port(portnum);/*auto record port pinmux*/
	
	/*output mode*/
	snprintf(cmdbuf, sizeof(cmdbuf), "sh -c \"echo \"low\" > /sys/class/gpio/gpio%d/direction\" 2> /dev/null", portnum);
	system(cmdbuf);
	
	/*active_low*/
	snprintf(cmdbuf, sizeof(cmdbuf), "echo 1 > /sys/class/gpio/gpio%d/active_low", portnum);
	system(cmdbuf);
	
	if (value == 1) {
		/*on*/
		snprintf(cmdbuf, sizeof(cmdbuf), "echo 1 > /sys/class/gpio/gpio%d/value", portnum);
		system(cmdbuf);
	} else {
		/*off*/
		snprintf(cmdbuf, sizeof(cmdbuf), "echo 0 > /sys/class/gpio/gpio%d/value", portnum);
		system(cmdbuf);
	}
	//printf("%s\n",cmdbuf);
}

void recovery_port(int portnum)
{
	char cmdbuf[128];
	
	/*Recovery*/
	snprintf(cmdbuf, sizeof(cmdbuf), "echo %d > /sys/class/gpio/unexport", portnum); 
	system(cmdbuf);	
}

void recovery_all_port(void)
{
	system("echo 5 > /proc/gpio"); /*recovery wps action*/
	
#ifdef CONFIG_RTL_8197F
	recovery_port(WIFILED);
	recovery_port(INTERNET_LED);
	
	recovery_port(SIGNAL_SMALL);
	recovery_port(SIGNAL_MIDDLE);
	recovery_port(SIGNAL_LARGE);
	recovery_port(SIGNAL_GIANT);
	
	recovery_port(LINK_LED1);

	/*recovery phy led*/
	system("echo \"8367write 1b03 0x936\" >/proc/rtl865x/phyReg");/*0x936 or 0x222*/
	
	system("ifconfig wlan1 down");
	system("iwpriv wlan1 set_mib led_type=3"); /*recovery wifi gpio control*/
	system("ifconfig wlan1 up");
#else 
	/*96e*/
	system("ifconfig wlan0 down");
	system("iwpriv wlan0 set_mib led_type=3"); /*recovery wifi gpio control*/
	system("ifconfig wlan0 up");
#endif	
}

void power_on_all_port(void)
{
	system("echo 6 > /proc/gpio"); /*set wps gpio control*/
#ifdef CONFIG_RTL_8197F
	set_port(WIFILED, POWER_ON);
	set_port(INTERNET_LED, POWER_ON);
	
	set_port(SIGNAL_SMALL, POWER_ON);
	set_port(SIGNAL_MIDDLE, POWER_ON);
	set_port(SIGNAL_LARGE, POWER_ON);
	set_port(SIGNAL_GIANT, POWER_ON);
	
	set_port(LINK_LED1, POWER_ON);
#endif
}

void power_off_all_port(void)
{
	system("echo 6 > /proc/gpio"); /*set wps gpio control*/
	
#if CONFIG_RTL_8197F
	system("ifconfig wlan1 down");
	system("iwpriv wlan1 set_mib led_type=0"); /*recovery wifi gpio control*/
	system("ifconfig wlan1 up");
	
	set_port(WIFILED, POWER_OFF);
	set_port(INTERNET_LED, POWER_OFF);
	set_port(SIGNAL_SMALL, POWER_OFF);
	set_port(SIGNAL_MIDDLE, POWER_OFF);
	set_port(SIGNAL_LARGE, POWER_OFF);
	set_port(SIGNAL_GIANT, POWER_OFF);
	
	set_port(LINK_LED1, POWER_OFF);

	/*power off phy led*/
	system("echo \"8367write 1b03 0\" >/proc/rtl865x/phyReg");
#else 
	/*96e*/
	system("ifconfig wlan0 down");
	system("iwpriv wlan0 set_mib led_type=0"); /*set wifi gpio control*/
	system("ifconfig wlan0 up");
#endif
}

int main(int argc, char *argv[])
{
	unsigned char portnum = 0;
	
	if (argc != 3) {
		usage();
		return -1;
	}
	
	if (memcmp(argv[1], "all", strlen("all")) == 0) {
		if (memcmp(argv[2], "on", strlen("on")) == 0)
			power_on_all_port();
		else if (memcmp(argv[2], "off", strlen("off")) == 0)
			power_off_all_port();
		else if (memcmp(argv[2], "recovery", strlen("recovery")) == 0)
			recovery_all_port();
		else
			usage();
		
		return 0;
	}
	
	portnum = atoi(argv[1]);
	//printf("num=%d\n", portnum);
	if (memcmp(argv[2], "on", strlen("on")) == 0)
		set_port(portnum, POWER_ON);
	else if (memcmp(argv[2], "off", strlen("off")) == 0)
		set_port(portnum, POWER_OFF);
	else if (memcmp(argv[2], "recovery", strlen("recovery")) == 0)
		recovery_port(portnum);
	else
		usage();
	
	return 0;
}