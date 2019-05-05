#ifndef __COMMON_H__
#define __COMMON_H__
#include <ont/edp.h>

typedef struct szJsonReqRspHeader
{
	unsigned char	version[7];
	unsigned char	msgType[65];
	unsigned char	msgSeq[33];
	unsigned char	erroCode[5];
	unsigned char	description[257];
}szJsonReqRspHeader_T,*szJsonReqRspHeader_Tp;

#define RTK_SUCCESS 	(1)
#define RTK_FAILED    	(0)

#ifndef ONT_DECREASE_SIZE
#define ONT_ENCRYPT			Zuwang_encrypt		
#define ONT_DECRYPT			Zuwang_decrypt
#else
#define ONT_ENCRYPT			ont_aes128_ecb_pkcs5_encrypt		
#define ONT_DECRYPT			ont_aes128_ecb_pkcs5_decrypt		
#endif

#endif