#include <stdio.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <math.h>

#include "boa.h"
#include "access.h"
#include "apmib.h"
#include "cJSON.h"
#include "../../adapter-api/rtk_api/rtk_api.h"

#ifdef BOA_WITH_OPENSSL
#include <openssl/ssl.h>
#endif

#ifdef SUPPORT_ASP
	#include "asp_page.h"
#endif

#ifdef SUPER_NAME_SUPPORT
	#include "auth.h"
#endif 

typedef struct URLInfo_s {
	char  username[MAX_NAME_LEN];
	char password[MAX_NAME_LEN];
	char pppoename[MAX_NAME_LEN_LONG];
	char pppoepwd[MAX_NAME_LEN_LONG];
	char action[MAX_NAME_LEN_LONG];
	char area[MAX_NAME_LEN_LONG];
	int isconnectPPP;
}URLInfo;

int getFileValue(char *filename, int *value)
{
	FILE *fp = NULL;
	char buff[1024] = {0};

	fp = fopen(filename, "r");
	if(fp==NULL){
		printf("open %s fail \n", filename);
		return -1;
	}
	
	while (fgets(buff, sizeof(buff), fp)) {
		buff[strlen(buff)-1]='\0';
		*value = atoi(buff);
		break;
	}

	fclose(fp);
	
	return 0;
}

int createRsp(cJSON *jsonRsp, char *rspCont)
{

	cJSON_AddStringToObject(jsonRsp, "Result", rspCont);
	
}
void Get_pppoeStatus(URLInfo *urlInfo)
{
	int isWanPhy_Link=0;

	isWanPhy_Link=getWanLink("eth1");
	if ( isConnectPPP()){
		if(isWanPhy_Link < 0){
			//return req_format_write(wp, "fmget_pppoe_disconnect");
			printf("(%s) %d PPPOE disconnect!\n",__FUNCTION__,__LINE__);
			urlInfo->isconnectPPP = 0;
		}else{
			//return req_format_write(wp, "fmget_pppoe_connet");
			printf("(%s) %d PPPOE connect!\n",__FUNCTION__,__LINE__);
			urlInfo->isconnectPPP = 1;
		}
	}else{
		printf("(%s) %d PPPOE disconnect!\n",__FUNCTION__,__LINE__);
		urlInfo->isconnectPPP = 0;
	}

}
int Set_pppoe(URLInfo *urlInfo){
	int wan_type=3;
	
	if(!apmib_set(MIB_WAN_DHCP,(void *)&wan_type)) 
	{
		printf("(%s) %d Set WAN DHCP error!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	if(!apmib_set(MIB_PPP_USER_NAME,(void *)urlInfo->pppoename)) 
	{
		printf("(%s) %d Set PPP user name MIB error!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	if(!apmib_set(MIB_PPP_PASSWORD, (void *)urlInfo->pppoepwd))
	{
		printf("(%s) %d Set PPP user password MIB error!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	if(rtk_start_pppoe() == -1){
		printf("(%s) %d Start PPPOE fail!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	return 0;
		
}

int Get_pppoeInfo(URLInfo *urlInfo, request *req)
{
	//http://192.168.10.1/itms?username=CMCCAdmin&pwd=aDmA&pppoename=15979102612&pppoepass=1234567890
	//http://192.168.10.1/itms?username=CMCCAdmin&pwd=aDmA&action=GetRegStat
	//http://192.168.10.1/itms?username=xxxx&userpwd=xxxx&PPPOEuser=xxx&PPPOEpassword=xxx&area=xxx
	//http://192.168.10.1/itms?username=xxx&ueserpwd=xxx&action=GetRegStat
	char buffer[128], tmpbuff[128];
	char *pchar=NULL, *pchar_tmp=NULL;
	char *savestr1=NULL;

	snprintf(buffer, sizeof(buffer), "%s", req->post_data);

	//username=CMCCAdmin&pwd=aDmA&pppoename=15979102612&pppoepass=1234567890
	pchar = strtok_r(buffer, "&", &savestr1);
	while(pchar){
		strncpy(tmpbuff, pchar, sizeof(tmpbuff));
		
		pchar_tmp = strstr(tmpbuff, "=");
		if(pchar_tmp){
			*pchar_tmp = '\0';
		}
		if(strstr(tmpbuff, "username")){
			strncpy(urlInfo->username, pchar_tmp+1, sizeof(urlInfo->username));
		}else if(strstr(tmpbuff, "userpwd")){
			strncpy(urlInfo->password, pchar_tmp+1, sizeof(urlInfo->password));
		}else if(strstr(tmpbuff, "PPPOEuser")){
			strncpy(urlInfo->pppoename, pchar_tmp+1, sizeof(urlInfo->pppoename));
		}else if(strstr(tmpbuff, "PPPOEpassword")){
			strncpy(urlInfo->pppoepwd, pchar_tmp+1, sizeof(urlInfo->pppoepwd));
		}else if(strstr(tmpbuff, "action")){
			strncpy(urlInfo->action, pchar_tmp+1, sizeof(urlInfo->action));
		}else if(strstr(tmpbuff, "area")){
			strncpy(urlInfo->area, pchar_tmp+1, sizeof(urlInfo->area));
			apmib_set(MIB_RTL_LINK_AREA, (void *)urlInfo->area);
		}
		
		pchar = strtok_r(NULL, "&", &savestr1);
	}
	
	return 0;
}

void Pppoe_handleJson(request *req)
{
	//jsonReq = req;
	//ont_rsp_ops(req->post_data,req->post_data_len);	
//	pre0: get passwd/pppoe info from req->request_uri;
	URLInfo urlInfo;
	char user_name[MAX_NAME_LEN], user_password[MAX_NAME_LEN];
	cJSON *jsonRsp = NULL;
	char *jsonData = NULL;
	int opMode=0, tmpValue=0;

	send_r_request_ok2(req);		/* All's well */	
	
	memset(&urlInfo, 0, sizeof(URLInfo));
	jsonRsp = cJSON_CreateObject();
	
	Get_pppoeInfo(&urlInfo, req);

	//compare username and password
	apmib_get(MIB_USER_NAME, (void *)user_name);
	apmib_get(MIB_USER_PASSWORD, (void *)user_password);
	if(strncmp(user_name, urlInfo.username, sizeof(user_name)) || strncmp(user_password, urlInfo.password, sizeof(user_password))){
		//response Adminauth Failed
		printf("(%s) %d Device Adminauth Failed!\n",__FUNCTION__,__LINE__);
		createRsp(jsonRsp, "Auth Failed");
		jsonData = cJSON_PrintUnformatted(jsonRsp);
		req_format_write(req,jsonData);
		goto EXIT;
	}
	
	//set pppoe,call reinit
	if(!urlInfo.action[0]){
		apmib_get(MIB_OP_MODE,(void *)&opMode);
		if(opMode == BRIDGE_MODE){
			printf("(%s) %d Device is in bridge mode, can't initialize PPPOE!\n",__FUNCTION__,__LINE__);
			createRsp(jsonRsp, "Bridge Mode Failed");
			jsonData = cJSON_PrintUnformatted(jsonRsp);
			req_format_write(req,jsonData);
			goto EXIT;
		}

		if(!find_pid_by_name("ppp_inet")){
			if(!Set_pppoe(&urlInfo)){
			//response Success
				printf("(%s) %d Device Successfully initializing PPPOE!\n",__FUNCTION__,__LINE__);
				createRsp(jsonRsp, "Default Configuration Succ");
				jsonData = cJSON_PrintUnformatted(jsonRsp);
				req_format_write(req,jsonData);
				goto EXIT;
			}
		}else{
			if(!Set_pppoe(&urlInfo)){
				printf("(%s) %d Device is running PPPOE!\n",__FUNCTION__,__LINE__);
				createRsp(jsonRsp, "Register is running");
				jsonData = cJSON_PrintUnformatted(jsonRsp);
				req_format_write(req,jsonData);
				goto EXIT;
			}
		}
		//what about "SN error" and "Over Limit"
			
	}else{
		Get_pppoeStatus(&urlInfo);
		if(find_pid_by_name("ppp_inet")){
			if(urlInfo.isconnectPPP){
				//TODO: internet connected or not
				if(checkWanping("www.baidu.com","www.sohu.com","www.163.com")==1){
					//TODO: judge if6 connection status
					getFileValue(ANDLINK_IF6_STATUS, &tmpValue);
					if(tmpValue == REGISTERING){
						//reponse Registering Intelligent Platform
						createRsp(jsonRsp, "Registering Intelligent Platform");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}else if(tmpValue == REGISTER_SUCCESS){
						//response Reg And Service Succ
						createRsp(jsonRsp, "Reg And Service Succ");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}else if(tmpValue == REGISTER_ERROR){
						//response Intelligent Platform Normal Error
						createRsp(jsonRsp, "Intelligent Platform Normal Error");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}else if(tmpValue == AUTH_SUCCESS){
						//response Intelligent Platform Auth Succ
						createRsp(jsonRsp, "Intelligent Platform Auth Succ");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}else if(tmpValue == AUTH_FAIL){
						//response Intelligent Platform Auth Failed
						createRsp(jsonRsp, "Intelligent Platform Auth Failed");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}else if(tmpValue == RE_REGISTER){
						//response Re-Registering Intelligent Platform
						createRsp(jsonRsp, "Re-Registering Intelligent Platform");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}else if(tmpValue == REGISTER_ANOTHER_SRV){
						//response Intelligent Platform Register to Another Server
						createRsp(jsonRsp, "Intelligent Platform Register to Another Server");
						jsonData = cJSON_PrintUnformatted(jsonRsp);
						req_format_write(req,jsonData);
						goto EXIT;
					}
					
				}else{
					//reponse Getting Address Failed
					printf("(%s) %d Device Getting Address Failed!\n",__FUNCTION__,__LINE__);
					createRsp(jsonRsp, "Getting Address Failed");
					jsonData = cJSON_PrintUnformatted(jsonRsp);
					req_format_write(req,jsonData);
					goto EXIT;
				}
			}else{
				//response Getting WAN Address
				printf("(%s) %d Device is getting WAN Address!\n",__FUNCTION__,__LINE__);
				createRsp(jsonRsp, "Getting WAN Address");
				jsonData = cJSON_PrintUnformatted(jsonRsp);
				req_format_write(req,jsonData);
				goto EXIT;
			}
		}else{
			printf("(%s) %d Device PPPOE Fail!\n",__FUNCTION__,__LINE__);
			createRsp(jsonRsp, "PPPOE Fail");
			jsonData = cJSON_PrintUnformatted(jsonRsp);
			req_format_write(req,jsonData);
			goto EXIT;
		}
	}

EXIT:
	if(jsonRsp != NULL)
		cJSON_Delete(jsonRsp);
	if(jsonData != NULL)
		free(jsonData);
		
	update_content_length(req);
	freeAllTempStr();
	//jsonReq = NULL;
	return;
}




int Pppoe_init_json(request * req)
{
#ifndef NEW_POST
	struct stat statbuf;
#endif
	SQUASH_KA(req);
	complete_env(req);
	
#ifndef NEW_POST
	fstat(req->post_data_fd, &statbuf);
	if (req->method == M_POST)
	{
		lseek(req->post_data_fd, SEEK_END, 0);	
	}
#else
	if (req->method == M_POST)
		req->post_data_idx = req->post_data_len;		
#endif	
	Pppoe_handleJson(req);
	return 0;

}


