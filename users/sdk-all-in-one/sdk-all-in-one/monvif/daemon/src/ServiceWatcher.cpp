#include "ServiceWatcher.h"
#include "SysCmdUtil.h"

bool VideoServiceWatcher::IsServiceRunning(const std::string& service_name)
{
	return SysCmdUtil::IsProcessExist(service_name);
}

int  VideoServiceWatcher::StartService(const std::string & start_cmd )
{
	return SysCmdUtil::ExecuteCmd(start_cmd);
}