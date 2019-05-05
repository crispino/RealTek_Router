0.编译
原环境 linux_2.6_x86_64
依赖于库pthread;

makefile编译命令:
make all
make clean
make testsdk
make install

makefile编译说明:
-DLWM2M_CLIENT_MODE: 使用lwm2m客户端模式
-DLWM2M_SUPPORT_JSON: 在程序中支持json数据类型
-DLWM2M_LITTLE_ENDIAN: 编译平台的字节顺序为小端; //若为大端,请使用宏-DLWM2M_BIG_ENDIAN
-DLWM2M_WITH_LOGS: 显示lwm2m底层日志;


1.目录
core：lwm2m sources
sdksrc/sdk: sdk sources
sdksrc/testsdk:use sdk 
sdksrc/shared:common func

2.函数参数说明
LWM2M_SDK_INIT传入参数
typedef struct
{
	char szCMEI_IMEI[64]; 	//CMEI/IMEI
	char szIMSI[64];		//IMSI
	char szDMv[16]; 		//DM版本号
	char szAppKey[64]; 		//appkey 
	char szPwd[64];			//pwd秘钥
	
	int  nAddressFamily; 	//4或6  ipv4，ipv6  ;//目前填4
	char szSrvIP[32];		//127.0.0.1 //dm服务地址
    int  nSrvPort;   		//5683
	int  nLifetime;    		//300 //秒
	
	int  nBootstrap; 		//是否booststrap 0,not use; other n,use;//目前填0
	
} Options;

//初始化Options:参数,optfuncs:回调函数
int LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs); 
//启动sdk，nMode=0,阻塞在当前线程运行；nMode!=0,在新线程中运行
int LWM2M_SDK_RUN(int nMode);
//关闭sdk，退出SDK的循环；注销lwm2m服务
int LWM2M_SDK_STOP();
//清理sdk,参数等
int LWM2M_SDK_FINI();


