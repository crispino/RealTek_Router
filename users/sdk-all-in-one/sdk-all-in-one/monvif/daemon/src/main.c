#include "RemoteUpdate.h"
#include "time.h"
#include <unistd.h>
#include "ServiceWatcher.h"

int main(int argc, char const *argv[])
{
	RemoteUpdateCtx cxt;
	VideoServiceWatcher service_watcher;

	if(!cxt.Initilize("../conf/update_cfg.xml"))
	{
		RemoteUpdate remote_update;
		while(1) {
			service_watcher.Do(cxt.GetServiceName(),cxt.GetUpdateStartCmd());

		    remote_update.Do(&cxt);
		    
		    usleep( 1000 ); 
		}	
	}

	return 0;
}