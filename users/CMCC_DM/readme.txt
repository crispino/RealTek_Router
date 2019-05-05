0.����
ԭ���� linux_2.6_x86_64
�����ڿ�pthread;

makefile��������:
make all
make clean
make testsdk
make install

makefile����˵��:
-DLWM2M_CLIENT_MODE: ʹ��lwm2m�ͻ���ģʽ
-DLWM2M_SUPPORT_JSON: �ڳ�����֧��json��������
-DLWM2M_LITTLE_ENDIAN: ����ƽ̨���ֽ�˳��ΪС��; //��Ϊ���,��ʹ�ú�-DLWM2M_BIG_ENDIAN
-DLWM2M_WITH_LOGS: ��ʾlwm2m�ײ���־;


1.Ŀ¼
core��lwm2m sources
sdksrc/sdk: sdk sources
sdksrc/testsdk:use sdk 
sdksrc/shared:common func

2.��������˵��
LWM2M_SDK_INIT�������
typedef struct
{
	char szCMEI_IMEI[64]; 	//CMEI/IMEI
	char szIMSI[64];		//IMSI
	char szDMv[16]; 		//DM�汾��
	char szAppKey[64]; 		//appkey 
	char szPwd[64];			//pwd��Կ
	
	int  nAddressFamily; 	//4��6  ipv4��ipv6  ;//Ŀǰ��4
	char szSrvIP[32];		//127.0.0.1 //dm�����ַ
    int  nSrvPort;   		//5683
	int  nLifetime;    		//300 //��
	
	int  nBootstrap; 		//�Ƿ�booststrap 0,not use; other n,use;//Ŀǰ��0
	
} Options;

//��ʼ��Options:����,optfuncs:�ص�����
int LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs); 
//����sdk��nMode=0,�����ڵ�ǰ�߳����У�nMode!=0,�����߳�������
int LWM2M_SDK_RUN(int nMode);
//�ر�sdk���˳�SDK��ѭ����ע��lwm2m����
int LWM2M_SDK_STOP();
//����sdk,������
int LWM2M_SDK_FINI();


