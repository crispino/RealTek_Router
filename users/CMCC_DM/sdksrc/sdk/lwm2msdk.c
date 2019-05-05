#include "liblwm2m.h"
#include "connection.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>

#include "j_aes.h"
#include "j_base64.h"
#include <pthread.h>
#include "lwm2msdk.h"
#include "lwm2mclient.h"

#define MAX_PACKET_SIZE 1024
#define OBJ_COUNT 3

//lwm2m_object_t * objArray[OBJ_COUNT];
int g_reboot = 0;
static int g_quit = -1;//

pthread_t 	g_lwsdktid;
static 		s_thrd_flag = 0;
int 		g_pip_ios_fd[2] ;
unsigned char g_szEPI[256]="";
Options  g_opt = {"CMEI_IMEI","IMSI", "v1.0","appkey123456","",4,"127.0.0.1",5683,300,0};
OptFuncs g_sdk_opt_funcs = {NULL};



typedef struct
{
    lwm2m_object_t * securityObjP;
    int sock;
    connection_t * connList;
    int addressFamily;
} client_data_t;


void handle_sigint(int signum)
{
    g_quit = 1;
}

void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) return NULL;

    fprintf(stdout, "Connecting to %s\r\n", uri);

    // parse uri in the form "coaps://[host]:[port]"
    if (0 == strncmp(uri, "coaps://", strlen("coaps://")))
    {
        host = uri+strlen("coaps://");
    }
    else if (0 == strncmp(uri, "coap://", strlen("coap://")))
    {
        host = uri+strlen("coap://");
    }
    else
    {
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    newConnP = connection_create(dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        fprintf(stderr, "Connection creation failed.\r\n");
    }
    else {
        dataP->connList = newConnP;
    }

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
    connection_t * targetP;

    app_data = (client_data_t *)userData;
    targetP = (connection_t *)sessionH;

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
        connection_t * parentP;

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }
}


void print_state(lwm2m_context_t * lwm2mH)
{
    lwm2m_server_t * targetP;

    fprintf(stderr, "State: ");
    switch(lwm2mH->state)
    {
    case STATE_INITIAL:
        fprintf(stderr, "STATE_INITIAL");
        break;
    case STATE_BOOTSTRAP_REQUIRED:
        fprintf(stderr, "STATE_BOOTSTRAP_REQUIRED");
        break;
    case STATE_BOOTSTRAPPING:
        fprintf(stderr, "STATE_BOOTSTRAPPING");
        break;
    case STATE_REGISTER_REQUIRED:
        fprintf(stderr, "STATE_REGISTER_REQUIRED");
        break;
    case STATE_REGISTERING:
        fprintf(stderr, "STATE_REGISTERING");
        break;
    case STATE_READY:
        fprintf(stderr, "STATE_READY");
        break;
    default:
        fprintf(stderr, "Unknown !");
        break;
    }
    fprintf(stderr, "\r\n");

    targetP = lwm2mH->bootstrapServerList;

    if (lwm2mH->bootstrapServerList == NULL)
    {
        fprintf(stderr, "No Bootstrap Server.\r\n");
    }
    else
    {
        fprintf(stderr, "Bootstrap Servers:\r\n");
        for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
            fprintf(stderr, " - Security Object ID %d", targetP->secObjInstID);
            fprintf(stderr, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
            fprintf(stderr, "\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                fprintf(stderr, "DEREGISTERED\r\n");
                break;
            case STATE_BS_HOLD_OFF:
                fprintf(stderr, "CLIENT HOLD OFF\r\n");
                break;
            case STATE_BS_INITIATED:
                fprintf(stderr, "BOOTSTRAP INITIATED\r\n");
                break;
            case STATE_BS_PENDING:
                fprintf(stderr, "BOOTSTRAP PENDING\r\n");
                break;
            case STATE_BS_FINISHED:
                fprintf(stderr, "BOOTSTRAP FINISHED\r\n");
                break;
            case STATE_BS_FAILED:
                fprintf(stderr, "BOOTSTRAP FAILED\r\n");
                break;
            default:
                fprintf(stderr, "INVALID (%d)\r\n", (int)targetP->status);
            }
            fprintf(stderr, "\r\n");
        }
    }

    if (lwm2mH->serverList == NULL)
    {
        fprintf(stderr, "No LWM2M Server.\r\n");
    }
    else
    {
        fprintf(stderr, "LWM2M Servers:\r\n");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
            fprintf(stderr, " - Server ID %d", targetP->shortID);
            fprintf(stderr, "\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                fprintf(stderr, "DEREGISTERED\r\n");
                break;
            case STATE_REG_PENDING:
                fprintf(stderr, "REGISTRATION PENDING\r\n");
                break;
            case STATE_REGISTERED:
                fprintf(stderr, "REGISTERED\tlocation: \"%s\"\tLifetime: %lus\r\n", targetP->location, (unsigned long)targetP->lifetime);
                break;
            case STATE_REG_UPDATE_PENDING:
                fprintf(stderr, "REGISTRATION UPDATE PENDING\r\n");
                break;
            case STATE_REG_UPDATE_NEEDED:
                fprintf(stderr, "REGISTRATION UPDATE REQUIRED\r\n");
                break;
            case STATE_DEREG_PENDING:
                fprintf(stderr, "DEREGISTRATION PENDING\r\n");
                break;
            case STATE_REG_FAILED:
                fprintf(stderr, "REGISTRATION FAILED\r\n");
                break;
            default:
                fprintf(stderr, "INVALID (%d)\r\n", (int)targetP->status);
            }
            fprintf(stderr, "\r\n");
        }
    }
}


void lwm2m_sdk_notify(SDK_NOTIFY_TYPE type,SDK_NOTIFY_CODE code,int ret,const char *notify_msg)
{
    if(g_sdk_opt_funcs.NotifyMsg==NULL)
    return ;
    OptNotifyParam optNotifyParam;
    optNotifyParam.notify_type = type;
    optNotifyParam.notify_code = code;
    if(code == NOTIFY_CODE_UNKNOWEN)
    {
        if(ret==0)
        optNotifyParam.notify_code=NOTIFY_CODE_OK;
        else if(ret<0)
        optNotifyParam.notify_code=NOTIFY_CODE_FAILED;
    }
    
    char szmsg[1024];
    memset(szmsg,0,sizeof(szmsg));
    sprintf(szmsg,"{ret=%0x,msg=%s}",ret,notify_msg);
    optNotifyParam.notify_msg = szmsg;
    g_sdk_opt_funcs.NotifyMsg(&optNotifyParam);
}

void lwm2m_sdk_notify_sys(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,notify_msg);
}

void lwm2m_sdk_notify_reg_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REGISTER,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_reg_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REGISTER,NOTIFY_CODE_FAILED,ret,notify_msg);
}
void lwm2m_sdk_notify_dereg_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_DEREGISTER,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_dereg_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_DEREGISTER,NOTIFY_CODE_FAILED,ret,notify_msg);
}
void lwm2m_sdk_notify_regup_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REG_UPDATE,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_regup_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REG_UPDATE,NOTIFY_CODE_FAILED,ret,notify_msg);
}


int to_encode(unsigned char *szin,unsigned char *szout)
{
    unsigned char *name = szin;
    //AES CBC
	/* A 256 bit key */  
	unsigned char *passwd = "00000000000000000000000000000000";
	if(strlen(g_opt.szPwd)>0)
	{
		passwd = g_opt.szPwd;
	}
	else
	{
		printf("pwd is null,use default pwd is:%s~\n", passwd);
	}
	//
	unsigned char key[64]={0};
	memset(key,0,sizeof(key));
	char shaout[256];
	memset(shaout,0,sizeof(shaout));
	StrSHA256(passwd,strlen(passwd), shaout);
	//printf("pwd=%s\n",passwd);
	//printf("shaout=%s\n",shaout);
	HexStrToByte(shaout,strlen(shaout),key);
	//hex_print("16sha:",key,32);
	
	/* A 128 bit IV */  
	unsigned char iv[16]={0};
	memset(iv,0,16);
	int i=0;
	for (i=0; i<16; ++i) {  
        iv[i] = 0;  
    } 
	unsigned char ciphertext[264]={0};   
	/* Buffer for the decrypted text */  
	unsigned char decryptedtext[264]={0}; 

	int decryptedtext_len, ciphertext_len=0;
	unsigned char *plaintext =  "The quick brown fox jumps over the lazy dog1234";
	plaintext = name;
	//printf("Plaintext is:\n%s~\n", plaintext);
	//加密

	ciphertext_len=EncryptionAES( plaintext, strlen(plaintext), key,ciphertext);	
 
	//printf("Ciphertext is %d bytes long:\n", ciphertext_len);
	//hex_print("Ciphertext:",ciphertext,ciphertext_len);
	
	name = ciphertext; //是否使用加密后的值
	
	//base64
	unsigned char *testbase64="123456789";//MTIzNDU2Nzg5
	testbase64=name;
	
	unsigned char *encData=0;
	unsigned char *decData=0;
	int encDataLen=0;
	int decDataLen=0;
	//printf("base64 inputlen=%d,strlen=%d\n",ciphertext_len,strlen(testbase64) );
	int ret= j_base64_encode(testbase64, ciphertext_len, &encData, &encDataLen);
	//printf("base64 encData=%s~\n", encData);
	memcpy(szout,encData,encDataLen);
	
    j_base64_free(encData, encDataLen);
	
    return encDataLen;
	

}


int LWM2M_SDK_RUN_PARAM(int argc, char *argv[])
{
    client_data_t data;
    lwm2m_context_t * lwm2mH = NULL;
    lwm2m_object_t * objArray[OBJ_COUNT];
	char * server = "127.0.0.1";
	char * serverPort = "5683";
    const char * localPort = "56830";
	unsigned char * initname = "testlwm2mclient";
	unsigned char * name = "";
	int lifetime=300;
    int result;
    int opt;

    memset(&data, 0, sizeof(client_data_t));

    data.addressFamily = AF_INET6;
	//localPort = argv[opt];
	//param set by optional	
	if (strlen(g_opt.szSrvIP)>=0) server=g_opt.szSrvIP;
	
	char szsrvport[10]={0};
	if (g_opt.nSrvPort > 80 ) 
	{
		sprintf(szsrvport,"%d",g_opt.nSrvPort);
		serverPort = szsrvport;		
	}
	
	if(g_opt.nAddressFamily==4) data.addressFamily = AF_INET;
	
	if ( g_opt.nLifetime > 10 ) lifetime=g_opt.nLifetime ;
	
	unsigned char *szStars="****";
	unsigned char szEpname[256]={0};
	sprintf(szEpname,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,	
	strlen(g_opt.szDMv)>0?g_opt.szDMv:szStars
	);

    unsigned char szEPIsrc[256]={0};
    sprintf(szEPIsrc,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,
	szStars
	);

    unsigned char szEpnameCode[256]={0};    
    memset(szEpnameCode,0,sizeof(szEpnameCode));
    memset(g_szEPI,0,sizeof(g_szEPI));
    name=szEpname;


    int nEpnamecodeLen = 0;
    nEpnamecodeLen = to_encode(szEpname,szEpnameCode);	
	unsigned char epnametmp[256];
	memset(epnametmp,0,sizeof(epnametmp));
	sprintf(epnametmp,"L@#@%s@#@%s",szEpnameCode,g_opt.szAppKey);

    //strcpy(epnametmp,"L@#@M+D2vyO9kk/IPVyzehd6Biypy8R69yTlHdaIQiTp6klbXqJmESZJ6QXoDlQkqOC8@#@M100000006");
	printf("epname=%s~\n", epnametmp);
	name = epnametmp;

		
    unsigned char epitmp[256];
	memset(epitmp,0,sizeof(epitmp));
    to_encode(szEPIsrc,epitmp); 
	sprintf(g_szEPI,"L@#@%s@#@%s",epitmp,g_opt.szAppKey);
    //strcpy(g_szEPI,"L@#@M+D2vyO9kk/IPVyzehd6Bs2GeOZOGcEHYX1mWBROKSg=@#@M100000006");
	
    printf("szEPI=%s~\n", g_szEPI);  

///////////////

    /*
     *This call an internal function that create an IPv6 socket on the port 5683.
     */
    fprintf(stderr, "Trying to bind LWM2M Client to port %s\r\n", localPort);
    data.sock = create_socket(localPort, data.addressFamily);
    if (data.sock < 0)
    {
        fprintf(stderr, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
        return -1;
    }
	
	char serverUri[50];
    int serverId = 123;
	sprintf (serverUri, "coap://%s:%s", server, serverPort);

    //objArray[0] = get_security_object();
	objArray[0] = get_security_object(serverId, serverUri);
    if (NULL == objArray[0])
    {
        fprintf(stderr, "Failed to create security object\r\n");
        return -1;
    }
    data.securityObjP = objArray[0];

    objArray[1] = get_server_object(serverId, lifetime);
    if (NULL == objArray[1])
    {
        fprintf(stderr, "Failed to create server object\r\n");
        return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2])
    {
        fprintf(stderr, "Failed to create Device object\r\n");
        return -1;
    }

    lwm2mH = lwm2m_init(&data);
    if (NULL == lwm2mH)
    {
        fprintf(stderr, "lwm2m_init() failed\r\n");
        return -1;
    }

   
    result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
    if (result != 0)
    {
        fprintf(stderr, "lwm2m_configure() failed: 0x%X\r\n", result);
        return -1;
    }


    //signal(SIGINT, handle_sigint);
    fprintf(stdout, "LWM2M Client \"%s\" started on port %s.\r\nUse Ctrl-C to exit.\r\n\n", name, localPort);
    g_quit = 0;
    
    /*
     * We now enter in a while loop that will handle the communications from the server
     */
    while (0 == g_quit)
    {
        struct timeval tv;
        fd_set readfds;

        tv.tv_sec = 60;
        tv.tv_usec = 0;

        FD_ZERO(&readfds);
        FD_SET(data.sock, &readfds);
		FD_SET(g_pip_ios_fd[0],&readfds);

        print_state(lwm2mH);
        
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));
        if (result != 0)
        {
            fprintf(stderr, "lwm2m_step() failed: 0x%X\r\n", result);
            g_quit = 1;//退出
            break;
            //continue;
            return -1;
        }

        /*
         * This part wait for an event on the socket until "tv" timed out (set
         * with the precedent function)
         */
        result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);
        if (result < 0)
        {
            if (errno != EINTR)
            {
              fprintf(stderr, "Error in select(): %d %s\r\n", errno, strerror(errno));
            }
        }
        else if (result > 0)
        {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            /*
             * If an event happens on the socket
             */
            if (FD_ISSET(data.sock, &readfds))
            {
                struct sockaddr_storage addr;
                socklen_t addrLen;

                addrLen = sizeof(addr);

                /*
                 * We retrieve the data received
                 */
                numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

                if (0 > numBytes)
                {
                    fprintf(stderr, "Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
                }
                else if (0 < numBytes)
                {
                    connection_t * connP;

                    connP = connection_find(data.connList, &addr, addrLen);
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
                    }
                    else
                    {
                        /*
                         * This packet comes from an unknown peer
                         */
                        fprintf(stderr, "received bytes ignored!\r\n");
                    }
                }
            }
			else if(FD_ISSET(g_pip_ios_fd[0], &readfds))
			{
				char c[2];  
				read(g_pip_ios_fd[0], c, 2);
				g_quit = 1;//退出
			}
        }
    }

    /*
     * Finally when the loop is left, we unregister our client from it
     */
    lwm2m_close(lwm2mH);
    lwm2m_sdk_notify_dereg_success(0,"dereg");
    close(data.sock);
    connection_free(data.connList);

    free_security_object(objArray[0]);
    free_server_object(objArray[1]);
    free_object_device(objArray[2]);

    fprintf(stdout, "loop func quit\r\n\n");

    return 0;
}

void * runthreadfunc(void * arg)
{
	int ret = LWM2M_SDK_RUN_PARAM(0,NULL);	
	lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,"LWM2M_SDK_RUN_PARAM");
	LOG_S("runthreadfunc end");
	s_thrd_flag = 0;
	return 0;
}

int LWM2M_SDK_RUN(int nMode)
{
	//LWM2M_SDK_RUN(0,NULL);
	int ret = 0;
	if(g_quit==0)
	{
	    LOG_S("has runned");
	    return 0;
	}
	
	if(nMode==0)
	{
		ret = LWM2M_SDK_RUN_PARAM(0,NULL);
		lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,"LWM2M_SDK_RUN_PARAM");
		s_thrd_flag = 0;
	}
	else
	{
		if(s_thrd_flag==0)
		{
			if (0 != pthread_create(&g_lwsdktid, NULL, runthreadfunc,NULL))
			{
				LOG_S("msg=failed to start up thread.|");
				s_thrd_flag = 0;
				
				ret = -1;
				return -1;
			}
			s_thrd_flag = 1;
			
		}
		else
		{
			LOG_S("msg=hss started up thread.|");
		}
			
		
		
	}
	
	//pthread_detach(g_lwsdktid); 	
	return ret;
	
}

int  LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs)
{	
	int ret = 0;
	strcpy(g_opt.szCMEI_IMEI,options->szCMEI_IMEI);
	strcpy(g_opt.szIMSI,options->szIMSI);	
	strcpy(g_opt.szDMv,options->szDMv);
	strcpy(g_opt.szAppKey,options->szAppKey);
	strcpy(g_opt.szPwd,options->szPwd);
	
	strcpy(g_opt.szSrvIP,options->szSrvIP);
	g_opt.nAddressFamily = options->nAddressFamily;
	g_opt.nSrvPort = options->nSrvPort;
	g_opt.nLifetime = options->nLifetime;
	g_opt.nBootstrap = options->nBootstrap;	

    g_sdk_opt_funcs = *optfuncs;
//other valid check
	//
	int pip_ret = pipe(g_pip_ios_fd);
	
	g_quit = -1;
	s_thrd_flag = 0;
	LOG_S("init sucess");
	return ret;
	
}



int LWM2M_SDK_STOP()
{	
    int ret = 0;

	if(g_quit != 0)
	{
		LOG_S("is stopped");
		return 0;
	}
	
	int wret = write(g_pip_ios_fd[1], "x", 1);
	 
	//wait for thread quit	
	if(s_thrd_flag==1)
	{	
		pthread_join(g_lwsdktid,NULL);
	}
	s_thrd_flag = 0;
	
	return ret;
	
}

int LWM2M_SDK_FINI()
{	
	
	g_quit = -1;
	s_thrd_flag = 0;
	
	close(g_pip_ios_fd[0]);
	close(g_pip_ios_fd[1]);
	
	LOG_S("end");
	return 0;
	
}

