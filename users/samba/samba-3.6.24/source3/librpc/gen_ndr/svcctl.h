/* header auto-generated by pidl */

#ifndef _PIDL_HEADER_svcctl
#define _PIDL_HEADER_svcctl

#include <stdint.h>

#include "libcli/util/ntstatus.h"

#include "librpc/gen_ndr/misc.h"
#include "librpc/gen_ndr/security.h"
#ifndef _HEADER_svcctl
#define _HEADER_svcctl

#define SVCCTL_ACCEPT_NONE	( 0x00000000 )
#define SERVICE_TYPE_KERNEL_DRIVER	( 0x01 )
#define SERVICE_TYPE_FS_DRIVER	( 0x02 )
#define SERVICE_TYPE_ADAPTER	( 0x04 )
#define SERVICE_TYPE_RECOGNIZER_DRIVER	( 0x08 )
#define SERVICE_TYPE_DRIVER	( SERVICE_TYPE_KERNEL_DRIVER|SERVICE_TYPE_FS_DRIVER|SERVICE_TYPE_RECOGNIZER_DRIVER )
#define SERVICE_TYPE_WIN32_OWN_PROCESS	( 0x10 )
#define SERVICE_TYPE_WIN32_SHARE_PROCESS	( 0x20 )
#define SERVICE_TYPE_WIN32	( SERVICE_TYPE_WIN32_OWN_PROCESS|SERVICE_TYPE_WIN32_SHARE_PROCESS )
#define SERVICE_TYPE_INTERACTIVE_PROCESS	( 0x100 )
#define SV_TYPE_ALL	( 0xFFFFFFFF )
#define SC_MANAGER_READ_ACCESS	( (SEC_STD_READ_CONTROL|SC_RIGHT_MGR_CONNECT|SC_RIGHT_MGR_ENUMERATE_SERVICE|SC_RIGHT_MGR_QUERY_LOCK_STATUS) )
#define SC_MANAGER_EXECUTE_ACCESS	( SC_MANAGER_READ_ACCESS )
#define SC_MANAGER_WRITE_ACCESS	( (SEC_STD_REQUIRED|SC_MANAGER_READ_ACCESS|SC_RIGHT_MGR_CREATE_SERVICE|SC_RIGHT_MGR_LOCK|SC_RIGHT_MGR_MODIFY_BOOT_CONFIG) )
#define SC_MANAGER_ALL_ACCESS	( SC_MANAGER_WRITE_ACCESS )
#define SERVICE_READ_ACCESS	( (SEC_STD_READ_CONTROL|SC_RIGHT_SVC_ENUMERATE_DEPENDENTS|SC_RIGHT_SVC_INTERROGATE|SC_RIGHT_SVC_QUERY_CONFIG|SC_RIGHT_SVC_QUERY_STATUS|SC_RIGHT_SVC_USER_DEFINED_CONTROL) )
#define SERVICE_EXECUTE_ACCESS	( (SERVICE_READ_ACCESS|SC_RIGHT_SVC_START|SC_RIGHT_SVC_STOP|SC_RIGHT_SVC_PAUSE_CONTINUE) )
#define SERVICE_WRITE_ACCESS	( (SEC_STD_REQUIRED|SERVICE_READ_ACCESS|SERVICE_EXECUTE_ACCESS|SC_RIGHT_SVC_CHANGE_CONFIG) )
#define SERVICE_ALL_ACCESS	( SERVICE_WRITE_ACCESS )
#define SC_MAX_ARGUMENT_LENGTH	( 1024 )
#define SC_MAX_ARGUMENTS	( 1024 )
struct SERVICE_LOCK_STATUS {
	uint32_t is_locked;
	const char *lock_owner;/* [charset(UTF16),unique] */
	uint32_t lock_duration;
};

enum svcctl_ServiceStatus
#ifndef USE_UINT_ENUMS
 {
	SVCCTL_STATE_UNKNOWN=(int)(0x00000000),
	SVCCTL_STOPPED=(int)(0x00000001),
	SVCCTL_START_PENDING=(int)(0x00000002),
	SVCCTL_STOP_PENDING=(int)(0x00000003),
	SVCCTL_RUNNING=(int)(0x00000004),
	SVCCTL_CONTINUE_PENDING=(int)(0x00000005),
	SVCCTL_PAUSE_PENDING=(int)(0x00000006),
	SVCCTL_PAUSED=(int)(0x00000007)
}
#else
 { __donnot_use_enum_svcctl_ServiceStatus=0x7FFFFFFF}
#define SVCCTL_STATE_UNKNOWN ( 0x00000000 )
#define SVCCTL_STOPPED ( 0x00000001 )
#define SVCCTL_START_PENDING ( 0x00000002 )
#define SVCCTL_STOP_PENDING ( 0x00000003 )
#define SVCCTL_RUNNING ( 0x00000004 )
#define SVCCTL_CONTINUE_PENDING ( 0x00000005 )
#define SVCCTL_PAUSE_PENDING ( 0x00000006 )
#define SVCCTL_PAUSED ( 0x00000007 )
#endif
;

/* bitmap svcctl_ControlsAccepted */
#define SVCCTL_ACCEPT_STOP ( 0x00000001 )
#define SVCCTL_ACCEPT_PAUSE_CONTINUE ( 0x00000002 )
#define SVCCTL_ACCEPT_SHUTDOWN ( 0x00000004 )
#define SVCCTL_ACCEPT_PARAMCHANGE ( 0x00000008 )
#define SVCCTL_ACCEPT_NETBINDCHANGE ( 0x00000010 )
#define SVCCTL_ACCEPT_HARDWAREPROFILECHANGE ( 0x00000020 )
#define SVCCTL_ACCEPT_POWEREVENT ( 0x00000040 )

struct SERVICE_STATUS {
	uint32_t type;
	enum svcctl_ServiceStatus state;
	uint32_t controls_accepted;
	WERROR win32_exit_code;
	uint32_t service_exit_code;
	uint32_t check_point;
	uint32_t wait_hint;
};

struct SERVICE_STATUS_PROCESS {
	struct SERVICE_STATUS status;
	uint32_t process_id;
	uint32_t service_flags;
}/* [public] */;

struct ENUM_SERVICE_STATUSW {
	const char * service_name;/* [flag(LIBNDR_FLAG_STR_NULLTERM|LIBNDR_FLAG_ALIGN2),relative] */
	const char * display_name;/* [flag(LIBNDR_FLAG_STR_NULLTERM|LIBNDR_FLAG_ALIGN2),relative] */
	struct SERVICE_STATUS status;
}/* [gensize,public] */;

struct ENUM_SERVICE_STATUSA {
	const char * service_name;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative] */
	const char * display_name;/* [relative,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	struct SERVICE_STATUS status;
}/* [public,gensize] */;

/* bitmap svcctl_ServerType */
#define SV_TYPE_WORKSTATION ( 0x00000001 )
#define SV_TYPE_SERVER ( 0x00000002 )
#define SV_TYPE_SQLSERVER ( 0x00000004 )
#define SV_TYPE_DOMAIN_CTRL ( 0x00000008 )
#define SV_TYPE_DOMAIN_BAKCTRL ( 0x00000010 )
#define SV_TYPE_TIME_SOURCE ( 0x00000020 )
#define SV_TYPE_AFP ( 0x00000040 )
#define SV_TYPE_NOVELL ( 0x00000080 )
#define SV_TYPE_DOMAIN_MEMBER ( 0x00000100 )
#define SV_TYPE_PRINTQ_SERVER ( 0x00000200 )
#define SV_TYPE_DIALIN_SERVER ( 0x00000400 )
#define SV_TYPE_SERVER_UNIX ( 0x00000800 )
#define SV_TYPE_NT ( 0x00001000 )
#define SV_TYPE_WFW ( 0x00002000 )
#define SV_TYPE_SERVER_MFPN ( 0x00004000 )
#define SV_TYPE_SERVER_NT ( 0x00008000 )
#define SV_TYPE_POTENTIAL_BROWSER ( 0x00010000 )
#define SV_TYPE_BACKUP_BROWSER ( 0x00020000 )
#define SV_TYPE_MASTER_BROWSER ( 0x00040000 )
#define SV_TYPE_DOMAIN_MASTER ( 0x00080000 )
#define SV_TYPE_SERVER_OSF ( 0x00100000 )
#define SV_TYPE_SERVER_VMS ( 0x00200000 )
#define SV_TYPE_WIN95_PLUS ( 0x00400000 )
#define SV_TYPE_DFS_SERVER ( 0x00800000 )
#define SV_TYPE_ALTERNATE_XPORT ( 0x20000000 )
#define SV_TYPE_LOCAL_LIST_ONLY ( 0x40000000 )
#define SV_TYPE_DOMAIN_ENUM ( 0x80000000 )

enum SERVICE_CONTROL
#ifndef USE_UINT_ENUMS
 {
	SVCCTL_CONTROL_STOP=(int)(0x00000001),
	SVCCTL_CONTROL_PAUSE=(int)(0x00000002),
	SVCCTL_CONTROL_CONTINUE=(int)(0x00000003),
	SVCCTL_CONTROL_INTERROGATE=(int)(0x00000004),
	SVCCTL_CONTROL_SHUTDOWN=(int)(0x00000005)
}
#else
 { __donnot_use_enum_SERVICE_CONTROL=0x7FFFFFFF}
#define SVCCTL_CONTROL_STOP ( 0x00000001 )
#define SVCCTL_CONTROL_PAUSE ( 0x00000002 )
#define SVCCTL_CONTROL_CONTINUE ( 0x00000003 )
#define SVCCTL_CONTROL_INTERROGATE ( 0x00000004 )
#define SVCCTL_CONTROL_SHUTDOWN ( 0x00000005 )
#endif
;

enum svcctl_ErrorControl
#ifndef USE_UINT_ENUMS
 {
	SVCCTL_SVC_ERROR_IGNORE=(int)(0x00000000),
	SVCCTL_SVC_ERROR_NORMAL=(int)(0x00000001),
	SVCCTL_SVC_ERROR_CRITICAL=(int)(0x00000002),
	SVCCTL_SVC_ERROR_SEVERE=(int)(0x00000003)
}
#else
 { __donnot_use_enum_svcctl_ErrorControl=0x7FFFFFFF}
#define SVCCTL_SVC_ERROR_IGNORE ( 0x00000000 )
#define SVCCTL_SVC_ERROR_NORMAL ( 0x00000001 )
#define SVCCTL_SVC_ERROR_CRITICAL ( 0x00000002 )
#define SVCCTL_SVC_ERROR_SEVERE ( 0x00000003 )
#endif
;

enum svcctl_StartType
#ifndef USE_UINT_ENUMS
 {
	SVCCTL_BOOT_START=(int)(0x00000000),
	SVCCTL_SYSTEM_START=(int)(0x00000001),
	SVCCTL_AUTO_START=(int)(0x00000002),
	SVCCTL_DEMAND_START=(int)(0x00000003),
	SVCCTL_DISABLED=(int)(0x00000004)
}
#else
 { __donnot_use_enum_svcctl_StartType=0x7FFFFFFF}
#define SVCCTL_BOOT_START ( 0x00000000 )
#define SVCCTL_SYSTEM_START ( 0x00000001 )
#define SVCCTL_AUTO_START ( 0x00000002 )
#define SVCCTL_DEMAND_START ( 0x00000003 )
#define SVCCTL_DISABLED ( 0x00000004 )
#endif
;

enum svcctl_ServiceState
#ifndef USE_UINT_ENUMS
 {
	SERVICE_STATE_ACTIVE=(int)(0x00000001),
	SERVICE_STATE_INACTIVE=(int)(0x00000002),
	SERVICE_STATE_ALL=(int)((SERVICE_STATE_ACTIVE|SERVICE_STATE_INACTIVE))
}
#else
 { __donnot_use_enum_svcctl_ServiceState=0x7FFFFFFF}
#define SERVICE_STATE_ACTIVE ( 0x00000001 )
#define SERVICE_STATE_INACTIVE ( 0x00000002 )
#define SERVICE_STATE_ALL ( (SERVICE_STATE_ACTIVE|SERVICE_STATE_INACTIVE) )
#endif
;

/* bitmap svcctl_MgrAccessMask */
#define SC_RIGHT_MGR_CONNECT ( 0x0001 )
#define SC_RIGHT_MGR_CREATE_SERVICE ( 0x0002 )
#define SC_RIGHT_MGR_ENUMERATE_SERVICE ( 0x0004 )
#define SC_RIGHT_MGR_LOCK ( 0x0008 )
#define SC_RIGHT_MGR_QUERY_LOCK_STATUS ( 0x0010 )
#define SC_RIGHT_MGR_MODIFY_BOOT_CONFIG ( 0x0020 )

/* bitmap svcctl_ServiceAccessMask */
#define SC_RIGHT_SVC_QUERY_CONFIG ( 0x0001 )
#define SC_RIGHT_SVC_CHANGE_CONFIG ( 0x0002 )
#define SC_RIGHT_SVC_QUERY_STATUS ( 0x0004 )
#define SC_RIGHT_SVC_ENUMERATE_DEPENDENTS ( 0x0008 )
#define SC_RIGHT_SVC_START ( 0x0010 )
#define SC_RIGHT_SVC_STOP ( 0x0020 )
#define SC_RIGHT_SVC_PAUSE_CONTINUE ( 0x0040 )
#define SC_RIGHT_SVC_INTERROGATE ( 0x0080 )
#define SC_RIGHT_SVC_USER_DEFINED_CONTROL ( 0x0100 )

struct QUERY_SERVICE_CONFIG {
	uint32_t service_type;
	enum svcctl_StartType start_type;
	enum svcctl_ErrorControl error_control;
	const char *executablepath;/* [range(0,8192),unique,charset(UTF16)] */
	const char *loadordergroup;/* [range(0,8192),unique,charset(UTF16)] */
	uint32_t tag_id;
	const char *dependencies;/* [charset(UTF16),unique,range(0,8192)] */
	const char *startname;/* [range(0,8192),charset(UTF16),unique] */
	const char *displayname;/* [unique,charset(UTF16),range(0,8192)] */
}/* [public,gensize] */;

struct svcctl_ArgumentString {
	const char *string;/* [range(0,SC_MAX_ARGUMENT_LENGTH),unique,charset(UTF16)] */
};

enum svcctl_ConfigLevel
#ifndef USE_UINT_ENUMS
 {
	SERVICE_CONFIG_DESCRIPTION=(int)(0x00000001),
	SERVICE_CONFIG_FAILURE_ACTIONS=(int)(0x00000002)
}
#else
 { __donnot_use_enum_svcctl_ConfigLevel=0x7FFFFFFF}
#define SERVICE_CONFIG_DESCRIPTION ( 0x00000001 )
#define SERVICE_CONFIG_FAILURE_ACTIONS ( 0x00000002 )
#endif
;

struct SERVICE_DESCRIPTION {
	const char * description;/* [flag(LIBNDR_FLAG_STR_NULLTERM|LIBNDR_FLAG_ALIGN2),relative] */
}/* [gensize,public] */;

enum SC_ACTION_TYPE
#ifndef USE_UINT_ENUMS
 {
	SC_ACTION_NONE=(int)(0),
	SC_ACTION_RESTART=(int)(1),
	SC_ACTION_REBOOT=(int)(2),
	SC_ACTION_RUN_COMMAND=(int)(3)
}
#else
 { __donnot_use_enum_SC_ACTION_TYPE=0x7FFFFFFF}
#define SC_ACTION_NONE ( 0 )
#define SC_ACTION_RESTART ( 1 )
#define SC_ACTION_REBOOT ( 2 )
#define SC_ACTION_RUN_COMMAND ( 3 )
#endif
;

struct SC_ACTION {
	enum SC_ACTION_TYPE type;
	uint32_t delay;
};

struct SERVICE_FAILURE_ACTIONS {
	uint32_t reset_period;
	const char * rebootmsg;/* [flag(LIBNDR_FLAG_STR_NULLTERM|LIBNDR_FLAG_ALIGN2),relative] */
	const char * command;/* [flag(LIBNDR_FLAG_STR_NULLTERM|LIBNDR_FLAG_ALIGN2),relative] */
	uint32_t num_actions;/* [range(0,1024)] */
	struct SC_ACTION *actions;/* [relative,size_is(num_actions)] */
}/* [public,gensize] */;

enum svcctl_StatusLevel
#ifndef USE_UINT_ENUMS
 {
	SVC_STATUS_PROCESS_INFO=(int)(0x00000000)
}
#else
 { __donnot_use_enum_svcctl_StatusLevel=0x7FFFFFFF}
#define SVC_STATUS_PROCESS_INFO ( 0x00000000 )
#endif
;


struct svcctl_CloseServiceHandle {
	struct {
		struct policy_handle *handle;/* [ref] */
	} in;

	struct {
		struct policy_handle *handle;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_ControlService {
	struct {
		struct policy_handle *handle;/* [ref] */
		enum SERVICE_CONTROL control;
	} in;

	struct {
		struct SERVICE_STATUS *service_status;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_DeleteService {
	struct {
		struct policy_handle *handle;/* [ref] */
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_LockServiceDatabase {
	struct {
		struct policy_handle *handle;/* [ref] */
	} in;

	struct {
		struct policy_handle *lock;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_QueryServiceObjectSecurity {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t security_flags;
		uint32_t offered;/* [range(0,0x40000)] */
	} in;

	struct {
		uint8_t *buffer;/* [size_is(offered),ref] */
		uint32_t *needed;/* [ref,range(0,0x40000)] */
		WERROR result;
	} out;

};


struct svcctl_SetServiceObjectSecurity {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t security_flags;
		uint8_t *buffer;/* [size_is(offered),ref] */
		uint32_t offered;
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_QueryServiceStatus {
	struct {
		struct policy_handle *handle;/* [ref] */
	} in;

	struct {
		struct SERVICE_STATUS *service_status;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_SetServiceStatus {
	struct {
		WERROR result;
	} out;

};


struct svcctl_UnlockServiceDatabase {
	struct {
		struct policy_handle *lock;/* [ref] */
	} in;

	struct {
		struct policy_handle *lock;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_NotifyBootConfigStatus {
	struct {
		WERROR result;
	} out;

};


struct svcctl_SCSetServiceBitsW {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t bits;
		uint32_t bitson;
		uint32_t immediate;
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_ChangeServiceConfigW {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t type;
		enum svcctl_StartType start_type;
		enum svcctl_ErrorControl error_control;
		const char *binary_path;/* [charset(UTF16),unique] */
		const char *load_order_group;/* [charset(UTF16),unique] */
		const char *dependencies;/* [charset(UTF16),unique] */
		const char *service_start_name;/* [unique,charset(UTF16)] */
		const char *password;/* [unique,charset(UTF16)] */
		const char *display_name;/* [charset(UTF16),unique] */
	} in;

	struct {
		uint32_t *tag_id;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_CreateServiceW {
	struct {
		struct policy_handle *scmanager_handle;/* [ref] */
		const char *ServiceName;/* [charset(UTF16)] */
		const char *DisplayName;/* [unique,charset(UTF16)] */
		uint32_t desired_access;
		uint32_t type;
		enum svcctl_StartType start_type;
		enum svcctl_ErrorControl error_control;
		const char *binary_path;/* [charset(UTF16)] */
		const char *LoadOrderGroupKey;/* [unique,charset(UTF16)] */
		uint8_t *dependencies;/* [size_is(dependencies_size),unique] */
		uint32_t dependencies_size;
		const char *service_start_name;/* [unique,charset(UTF16)] */
		uint8_t *password;/* [size_is(password_size),unique] */
		uint32_t password_size;
		uint32_t *TagId;/* [unique] */
	} in;

	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t *TagId;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_EnumDependentServicesW {
	struct {
		struct policy_handle *service;/* [ref] */
		enum svcctl_ServiceState state;
		uint32_t offered;/* [range(0,0x40000)] */
	} in;

	struct {
		uint8_t *service_status;/* [size_is(offered),ref] */
		uint32_t *needed;/* [ref,range(0,0x40000)] */
		uint32_t *services_returned;/* [ref,range(0,0x40000)] */
		WERROR result;
	} out;

};


struct svcctl_EnumServicesStatusW {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t type;
		enum svcctl_ServiceState state;
		uint32_t offered;/* [range(0,0x40000)] */
		uint32_t *resume_handle;/* [unique] */
	} in;

	struct {
		uint8_t *service;/* [size_is(offered),ref] */
		uint32_t *needed;/* [range(0,0x40000),ref] */
		uint32_t *services_returned;/* [range(0,0x40000),ref] */
		uint32_t *resume_handle;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_OpenSCManagerW {
	struct {
		const char *MachineName;/* [unique,charset(UTF16)] */
		const char *DatabaseName;/* [charset(UTF16),unique] */
		uint32_t access_mask;
	} in;

	struct {
		struct policy_handle *handle;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_OpenServiceW {
	struct {
		struct policy_handle *scmanager_handle;/* [ref] */
		const char *ServiceName;/* [charset(UTF16)] */
		uint32_t access_mask;
	} in;

	struct {
		struct policy_handle *handle;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_QueryServiceConfigW {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t offered;/* [range(0,8192)] */
	} in;

	struct {
		struct QUERY_SERVICE_CONFIG *query;/* [ref] */
		uint32_t *needed;/* [ref,range(0,8192)] */
		WERROR result;
	} out;

};


struct svcctl_QueryServiceLockStatusW {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t offered;
	} in;

	struct {
		struct SERVICE_LOCK_STATUS *lock_status;/* [ref] */
		uint32_t *needed;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_StartServiceW {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t NumArgs;/* [range(0,SC_MAX_ARGUMENTS)] */
		struct svcctl_ArgumentString *Arguments;/* [unique,size_is(NumArgs)] */
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_GetServiceDisplayNameW {
	struct {
		struct policy_handle *handle;/* [ref] */
		const char *service_name;/* [charset(UTF16),unique] */
		uint32_t *display_name_length;/* [unique] */
	} in;

	struct {
		const char **display_name;/* [ref,charset(UTF16)] */
		uint32_t *display_name_length;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_GetServiceKeyNameW {
	struct {
		struct policy_handle *handle;/* [ref] */
		const char *service_name;/* [unique,charset(UTF16)] */
		uint32_t *display_name_length;/* [unique] */
	} in;

	struct {
		const char **key_name;/* [ref,charset(UTF16)] */
		uint32_t *display_name_length;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_SCSetServiceBitsA {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t bits;
		uint32_t bitson;
		uint32_t immediate;
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_ChangeServiceConfigA {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t type;
		enum svcctl_StartType start_type;
		enum svcctl_ErrorControl error_control;
		const char *binary_path;/* [charset(UTF16),unique] */
		const char *load_order_group;/* [charset(UTF16),unique] */
		const char *dependencies;/* [charset(UTF16),unique] */
		const char *service_start_name;/* [charset(UTF16),unique] */
		const char *password;/* [unique,charset(UTF16)] */
		const char *display_name;/* [charset(UTF16),unique] */
	} in;

	struct {
		uint32_t *tag_id;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_CreateServiceA {
	struct {
		struct policy_handle *handle;/* [ref] */
		const char *ServiceName;/* [unique,charset(UTF16)] */
		const char *DisplayName;/* [charset(UTF16),unique] */
		uint32_t desired_access;
		uint32_t type;
		enum svcctl_StartType start_type;
		enum svcctl_ErrorControl error_control;
		const char *binary_path;/* [charset(UTF16),unique] */
		const char *LoadOrderGroupKey;/* [charset(UTF16),unique] */
		const char *dependencies;/* [unique,charset(UTF16)] */
		const char *service_start_name;/* [charset(UTF16),unique] */
		const char *password;/* [charset(UTF16),unique] */
	} in;

	struct {
		uint32_t *TagId;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_EnumDependentServicesA {
	struct {
		struct policy_handle *service;/* [ref] */
		enum svcctl_ServiceState state;
		uint32_t offered;
	} in;

	struct {
		struct ENUM_SERVICE_STATUSA *service_status;/* [unique] */
		uint32_t *needed;/* [ref] */
		uint32_t *services_returned;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_EnumServicesStatusA {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t type;
		enum svcctl_ServiceState state;
		uint32_t offered;
		uint32_t *resume_handle;/* [unique] */
	} in;

	struct {
		uint8_t *service;/* [size_is(offered)] */
		uint32_t *needed;/* [ref] */
		uint32_t *services_returned;/* [ref] */
		uint32_t *resume_handle;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_OpenSCManagerA {
	struct {
		const char *MachineName;/* [charset(UTF16),unique] */
		const char *DatabaseName;/* [charset(UTF16),unique] */
		uint32_t access_mask;
	} in;

	struct {
		struct policy_handle *handle;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_OpenServiceA {
	struct {
		struct policy_handle *scmanager_handle;/* [ref] */
		const char *ServiceName;/* [charset(UTF16),unique] */
		uint32_t access_mask;
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_QueryServiceConfigA {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t offered;
	} in;

	struct {
		uint8_t *query;
		uint32_t *needed;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_QueryServiceLockStatusA {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t offered;
	} in;

	struct {
		struct SERVICE_LOCK_STATUS *lock_status;/* [ref] */
		uint32_t *needed;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_StartServiceA {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t NumArgs;
		const char *Arguments;/* [unique,charset(UTF16)] */
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_GetServiceDisplayNameA {
	struct {
		struct policy_handle *handle;/* [ref] */
		const char *service_name;/* [unique,charset(UTF16)] */
		uint32_t *display_name_length;/* [unique] */
	} in;

	struct {
		const char **display_name;/* [charset(UTF16),ref] */
		uint32_t *display_name_length;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_GetServiceKeyNameA {
	struct {
		struct policy_handle *handle;/* [ref] */
		const char *service_name;/* [unique,charset(UTF16)] */
		uint32_t *display_name_length;/* [unique] */
	} in;

	struct {
		const char **key_name;/* [ref,charset(UTF16)] */
		uint32_t *display_name_length;/* [unique] */
		WERROR result;
	} out;

};


struct svcctl_GetCurrentGroupeStateW {
	struct {
		WERROR result;
	} out;

};


struct svcctl_EnumServiceGroupW {
	struct {
		WERROR result;
	} out;

};


struct svcctl_ChangeServiceConfig2A {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t info_level;
		uint8_t *info;/* [unique] */
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_ChangeServiceConfig2W {
	struct {
		struct policy_handle *handle;/* [ref] */
		uint32_t info_level;
		uint8_t *info;/* [unique] */
	} in;

	struct {
		WERROR result;
	} out;

};


struct svcctl_QueryServiceConfig2A {
	struct {
		struct policy_handle *handle;/* [ref] */
		enum svcctl_ConfigLevel info_level;
		uint32_t offered;
	} in;

	struct {
		uint8_t *buffer;
		uint32_t *needed;/* [ref] */
		WERROR result;
	} out;

};


struct svcctl_QueryServiceConfig2W {
	struct {
		struct policy_handle *handle;/* [ref] */
		enum svcctl_ConfigLevel info_level;
		uint32_t offered;/* [range(0,8192)] */
	} in;

	struct {
		uint8_t *buffer;/* [ref,size_is(offered)] */
		uint32_t *needed;/* [range(0,8192),ref] */
		WERROR result;
	} out;

};


struct svcctl_QueryServiceStatusEx {
	struct {
		struct policy_handle *handle;/* [ref] */
		enum svcctl_StatusLevel info_level;
		uint32_t offered;/* [range(0,8192)] */
	} in;

	struct {
		uint8_t *buffer;/* [ref,size_is(offered)] */
		uint32_t *needed;/* [range(0,8192),ref] */
		WERROR result;
	} out;

};


struct EnumServicesStatusExA {
	struct {
		struct policy_handle *scmanager;/* [ref] */
		uint32_t info_level;
		uint32_t type;
		enum svcctl_ServiceState state;
		uint32_t offered;
		uint32_t *resume_handle;/* [unique] */
	} in;

	struct {
		uint8_t *services;
		uint32_t *needed;/* [ref] */
		uint32_t *service_returned;/* [ref] */
		const char **group_name;/* [charset(UTF16),ref] */
		uint32_t *resume_handle;/* [unique] */
		WERROR result;
	} out;

};


struct EnumServicesStatusExW {
	struct {
		struct policy_handle *scmanager;/* [ref] */
		uint32_t info_level;
		uint32_t type;
		enum svcctl_ServiceState state;
		uint32_t offered;/* [range(0,0x40000)] */
		const char *group_name;/* [unique,charset(UTF16)] */
		uint32_t *resume_handle;/* [range(0,0x40000),unique] */
	} in;

	struct {
		uint8_t *services;/* [ref,size_is(offered)] */
		uint32_t *needed;/* [ref,range(0,0x40000)] */
		uint32_t *service_returned;/* [range(0,0x40000),ref] */
		uint32_t *resume_handle;/* [range(0,0x40000),unique] */
		WERROR result;
	} out;

};


struct svcctl_SCSendTSMessage {
	struct {
		WERROR result;
	} out;

};

#endif /* _HEADER_svcctl */
#endif /* _PIDL_HEADER_svcctl */
