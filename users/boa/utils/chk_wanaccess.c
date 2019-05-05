#include <stdio.h>
#include <stdlib.h>
#include "apmib.h"
#define SLEEP_INTERVAL (10) 
int checkWanping(char *server1, char *server2, char *server3)
{
	int ret=-1;
	char buffer[128]={0};

	snprintf(buffer,sizeof(buffer),"ping %s -c 1 > /dev/null",server1);
	ret=system(buffer);
	if(ret) {		
		return 0;
	}	

	snprintf(buffer,sizeof(buffer),"ping %s -c 1 > /dev/null",server2);
	ret=system(buffer);
	if(ret) {		
		return 0;
	}	

	snprintf(buffer,sizeof(buffer),"ping %s -c 1 > /dev/null",server3);
	ret=system(buffer);
	if(ret) {		
		return 0;
	}	

	return 1;
}

int main()
{
	int access=0;
	if ( !apmib_init()) {
		printf("Initialize AP MIB failed in setSystemTime_flash!\n");
		return -1;
	}
	while(true)
	{
		access=0;
		if(checkWanping("www.baidu.com","www.sohu.com","www.163.com")==1)
			access=1;
#ifdef CONFIG_APP_BOA_HE_UI
		apmib_set(MIB_SITE_ACCESS,(void*)&access);
#endif
		sleep(SLEEP_INTERVAL);
	}


}
