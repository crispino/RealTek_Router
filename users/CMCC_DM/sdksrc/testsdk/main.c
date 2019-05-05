#include "lwm2msdk.h"
#include <signal.h>
#include <unistd.h>

#define DEFAULT_LIFETIME	(15*60)
//Options opt = {"861111003929109","460041850403690", "v1.0","M100000006","",4,"127.0.0.1",5683,60,0};
/* CIOT */
Options opt = {"211418093261559", "211418093261559", "v1.0","M100000052",
		"n525A97z0M7Vyh91b0508l7j0U5g2g9Y",4,"m.fxltsbl.com",5683,DEFAULT_LIFETIME,0};

int nrunmode = 0; // default: thread

void handle_sig(int signum)
{
    LWM2M_SDK_STOP();
}


void  myNotifyMsg(OptNotifyParam *optNotifyParam)
{
   if(optNotifyParam==NULL)
    return ;

    printf("myNotifyMsg:type=%0x,code=%d,msg=%s\n",optNotifyParam->notify_type,optNotifyParam->notify_code,optNotifyParam->notify_msg);
    switch (optNotifyParam->notify_type)
    {
    case NOTIFY_TYPE_SYS:
    break;
    case NOTIFY_TYPE_REGISTER:
    break;
    case NOTIFY_TYPE_DEREGISTER:
    break;
    case NOTIFY_TYPE_REG_UPDATE:
    break;
    default:
    break;  
    }
    
}

static void show_help(void)
{
    printf("Usage: CMCC_DM [arguments]\n");
    printf("\t-c CMEI\n");
    printf("\t-m CMCC_DM work mode, 0: run in main thread, 1: run in created thread\n");
    printf("\t-s Server address\n");
    printf("\t-k Key of encrypt\n");
    printf("\t-t lifetime\n");
    printf("\t-h show help msg\n");
}

void CMCC_DM_main()
{
	OptFuncs optfuncs={NULL};
    optfuncs.NotifyMsg = myNotifyMsg;

	//init config
	int ret = LWM2M_SDK_INIT(&opt,&optfuncs);

	if(nrunmode==0)
	{
		printf("[%s:%d]to run in main thread\n",__func__,__LINE__);
		ret = LWM2M_SDK_RUN(0);
	}
	else
	{
		printf("to run in created thread\n");
		ret = LWM2M_SDK_RUN(1);

		unsigned int ntimes = 1;
		while(ntimes++ > 0)
		{
		    printf("main thread sleep[%d]\n",ntimes);
			sleep(60);
		}
		
		printf("main thread to stop workthread\n");
		LWM2M_SDK_STOP();
		printf("main thread has stopped workthread \n");
	}
	
	LWM2M_SDK_FINI();
	printf("CMCC_DM main quit \n");

	return ;
}

int main(int argc, char *argv[])
{
	int arg;

    for(;;){
        arg = getopt(argc, argv, "c:m:s:k:t:h");
        if(arg < 0)
            break;
        switch(arg){
		case 'c':
			strcpy(opt.szCMEI_IMEI ,optarg);
			/* IMSI same as CMEI_IMEI */
			strcpy(opt.szIMSI ,optarg);
			break;
        case 'm':
            nrunmode = atoi(optarg);
            break;
        case 's':
            strcpy(opt.szSrvIP, optarg);
            break;
        case 'k':
            strcpy(opt.szPwd, optarg);
            break;
		case 't':
			opt.nLifetime = atoi(optarg);
			break;
		case 'h':
            show_help();
			return 0;
        default:
            show_help();
            return 0;
        }
    }

	signal(SIGINT, handle_sig);

	while (1)
	{
		printf("CMCC_DM main start!\n");
		CMCC_DM_main();
		sleep(120);
	}

	return 0;
}

