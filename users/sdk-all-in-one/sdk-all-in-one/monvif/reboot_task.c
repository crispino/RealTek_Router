#include "reboot_task.h"
#include "time.h"
#include <sys/wait.h>
#include <sys/types.h>
#include "stdio.h"
#include "stdlib.h"


time_t GetTodayTimeOf(time_t tTime,int hour, int min, int sec)
{
	struct tm stTempTm;
	struct tm *pTempTm = localtime_r(&tTime, &stTempTm);

	if(!pTempTm)
	{
		return tTime;
	}

	stTempTm.tm_sec = sec;
	stTempTm.tm_min = min;
	stTempTm.tm_hour = hour;

	return mktime(&stTempTm);
}

int  ExecuteAndWaitCmd(const char* cmd)
{
	pid_t status;
	status = system(cmd);
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

void  sys_check_and_reboot_at(int hour, int min, int sec)
{
	time_t _now = time(NULL);
    time_t _temp = GetTodayTimeOf(_now,hour,min,sec);
	if (_temp -20 < _now  && _temp+20>_now )
	{
		ExecuteAndWaitCmd("exit");
	}
}