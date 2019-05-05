#include "SysCmdUtil.h"

#include <sys/wait.h>
#include <sys/types.h>
#include "stdio.h"
#include "stdlib.h"

int  SysCmdUtil::ExecuteCmd(const std::string& cmd)
{
	pid_t status;
	status = system(cmd.c_str());
	if (status == -1)
	{
		return 1;
	}
	return 0;
}

int  SysCmdUtil::ExecuteAndWaitCmd(const std::string& cmd)
{
	pid_t status;
	status = system(cmd.c_str());
	if (status == -1)
	{
		return 1;
	}

	if (WIFEXITED(status))
	{
		if (0 == WEXITSTATUS(status))
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	return 0;
}

bool  SysCmdUtil::IsProcessExist(const std::string& process)
{
	FILE* fp;
	int count;
	char buf[4096];
	char command[150];

	if (process.empty())
	{
		return false;
	}

	sprintf(command, "ps aux|grep %s|grep -v grep|wc -l", process.c_str());
	if ((fp = popen(command, "r")) == NULL)
	{
		return false;
	}

	bool ret = false;
	if ( (fgets(buf, 4096, fp)) != NULL )
	{
		count = atoi(buf);
		if ((count - 1) == 0)
		{
			ret = false;
		}
		else
		{
			ret = true;
		}	
	}

	pclose(fp);
	return ret;
	
}