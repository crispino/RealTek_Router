#include "boa.h"
#include "access.h"

#ifdef BOA_WITH_OPENSSL
#include <openssl/ssl.h>
#endif

#ifdef SUPPORT_ASP
	#include "asp_page.h"
#endif

#ifdef SUPER_NAME_SUPPORT
	#include "auth.h"
#endif 

static request* jsonReq = NULL;

void *ont_platform_malloc(size_t size)
{
    return malloc(size);
}

void ont_platform_free(void *ptr)
{
    free(ptr);
}



int _ont_send_rsp(char* data,int datalen)
{	
	req_format_write(jsonReq,data);
	return 0;
}

int _ont_req_send(char* data,int datalen)
{
	/* do nothing */
	return 0;
}

void handleJson(request *req)
{
	send_r_request_ok2(req);		/* All's well */	

	jsonReq = req;
	ont_rsp_ops(req->post_data,req->post_data_len);

	update_content_length(req);
	freeAllTempStr();
	jsonReq = NULL;
	return;
}



int init_json(request * req)
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
//-------------------------------------	
	ont_rtk_set_debugLevel(3);
	
	handleJson(req);
	return 0;

}

