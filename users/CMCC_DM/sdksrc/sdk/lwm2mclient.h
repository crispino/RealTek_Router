
#ifndef LWM2MCLIENT_H_
#define LWM2MCLIENT_H_

#include "liblwm2m.h"

#define LOG_M(FMT, ...) printf("[%s:%d] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#define LOG_S(STR) 		LOG_M("%s",STR)
#define TEST_OBJECT_ID 31024

lwm2m_object_t * get_object_device(void);
void free_object_device(lwm2m_object_t * objectP);
lwm2m_object_t * get_server_object(int serverId,int lifetime);
void free_server_object(lwm2m_object_t * object);
lwm2m_object_t * get_security_object(int serverId,const char* serverUri);
void free_security_object(lwm2m_object_t * objectP);
char * get_server_uri(lwm2m_object_t * objectP, uint16_t secObjInstID);

#endif /* LWM2MCLIENT_H_ */
