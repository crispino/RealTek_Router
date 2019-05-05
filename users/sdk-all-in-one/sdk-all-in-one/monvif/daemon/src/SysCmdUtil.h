#ifndef SYS_CMD_UTIL_H
#define SYS_CMD_UTIL_H

#include <string>
using std::string;

class SysCmdUtil
{
public:
	static int  ExecuteCmd(const std::string& cmd);

	static int  ExecuteAndWaitCmd(const std::string& cmd);

	static bool  IsProcessExist(const std::string& process);
};

#endif // SYS_CMD_UTIL_H
