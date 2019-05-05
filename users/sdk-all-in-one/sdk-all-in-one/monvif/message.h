
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "ont_list.h"

# ifdef __cplusplus
extern "C" {
# endif


typedef struct threadinfo_t
{
    pthread_t thread_id;
    pthread_attr_t attr;
    int index;
    
    int sock_srv; /*inter thread communication*/ 
    int sock_cli;
    ont_list_t *list;
    void *play_env;
}threadinfo_t;

extern threadinfo_t *g_threads;

typedef struct message_t
{
    int size; /*message size*/
    int type; /*message type*/
    void *dev;
    void *data;
}message;


typedef void HandlerProc(void* clientData, int mask);


int subconnect_wait();
int subconnect();
int messagehandle_init();

int messagesend(void *dev, int channel, void*cmd, int size);

void *messagehandle_proc(void *arg);


int checkneedclose(ont_list_t *list);

int ont_setmessage_handle(void *env, int sock, HandlerProc proc, void *arg);

int ont_videocmd_handle(void *dev, ont_list_t *list, const char*str);


# ifdef __cplusplus
}
# endif



#endif
