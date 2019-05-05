#ifndef SERVICE_WATCHER_H
#define SERVICE_WATCHER_H

#include <string>
using std::string;

class BaseServiceWatcher
{
public:
	virtual  int Do(const std::string& service_name,const std::string & start_cmd )
	{
		if (!IsServiceRunning(service_name))
		{
			return StartService(start_cmd);
		}
		return 0;
	}
protected:
	virtual  bool IsServiceRunning(const std::string& service_name) = 0;

	virtual  int  StartService(const std::string & start_cmd ) = 0;
};

class VideoServiceWatcher : public BaseServiceWatcher
{
protected:
	virtual  bool IsServiceRunning(const std::string& service_name);

	virtual  int  StartService(const std::string & start_cmd );
};

#endif // SERVICE_WATCHER_H
