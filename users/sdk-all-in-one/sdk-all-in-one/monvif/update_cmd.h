#ifndef __UPDATE_CMD_H_
#define __UPDATE_CMD_H_

#ifdef __cplusplus

extern "C"{

int update_cmd_push(const char * cmd_file_name,
					const char * package_name,
					const char* package_md5,
					const char* url);

}
#endif


#endif // __UPDATE_CMD_H_
