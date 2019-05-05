#ifndef _CWMP_NOTIFY_H_
#define _CWMP_NOTIFY_H_

#include "parameter_api.h"
extern int gBringLanMacAddrInInform;

#ifdef __cplusplus
extern "C" {
#endif


/*define the attribute's notification*/
#define CWMP_NTF_OFF	0x00
#define CWMP_NTF_PAS	0x01
#define CWMP_NTF_ACT	0x02
#define CWMP_NTF_FORCED	0x04 /*forced notification*/
#define CWMP_NTF_MASK 	0xff

/*define the attribute's accesslist*/
#define CWMP_ACS_OFF	0x00
#define CWMP_ACS_SUB	0x01 /*subscriber*/
#define CWMP_ACS_MASK	0xff

/* ID for Notifications - Magician: Special use. No need to take it into account in normal situation. */
#define CWMP_NTF_CNT_REQ_URL 1627
#define CWMP_NTF_PARAM_KEY 1430

struct CWMP_NOTIFY{
	char			*name;
	unsigned short		type;
	unsigned char		notifx;
	unsigned char		accesslist;
	void			*value;
	struct CWMP_NOTIFY	*next;
	int id; // Magician: Special use. No need to take it into account in normal situation.
};

/* used for flash storage */
struct cwmp_notify_t {
	char				name[256+4];
	unsigned char		notifx;
	unsigned char		accesslist;
};

extern struct CWMP_NOTIFY *pCWMPNotifyRoot;

extern int notify_init( int flag );
extern int notify_uninit( void );
extern int notify_set_attributes( char *name, unsigned char notify_mode, unsigned char access_mode );
extern int notify_get_attributes( char *name, unsigned char *notifx, unsigned char *access );
extern int notify_update_value( char *name );
extern int notify_check_active( void );
extern int notify_check_passive( void );
extern int notify_check_all( void );
extern int notify_create_update_info( struct node **node_root );
extern int notify_save( void );
void notify_clean_wan_with_forced_attribute(void);

struct CWMP_NOTIFY *notify_find( struct CWMP_NOTIFY **root, char *name );
extern void notify_set_wan_changed(void);
extern void notify_clear_wan_changed(void);
#ifdef __cplusplus
}
#endif

#endif /*_CWMP_NOTIFY_H_*/
