#ifndef _STATUS_SYNC_H_
#define _STATUS_SYNC_H_



# ifdef __cplusplus
extern "C" {
# endif



int  status_synced(uint32_t chnnum);
int  status_gethwaddr(char addr[32]);


# ifdef __cplusplus
}
# endif

#endif
