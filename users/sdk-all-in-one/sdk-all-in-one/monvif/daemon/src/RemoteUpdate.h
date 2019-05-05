#ifndef REMOTE_UPDATE_H
#define REMOTE_UPDATE_H

#include "assert.h"
#include "BaseUpdate.h"
#include "UpdateCmd.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

class RemoteUpdateCtx : public BaseUpdateCtx
{
public:
	virtual int Initilize(const std::string & conf);
	
	UpdateCmd & GetUpdateCmd(){return update_cmd_;}

	std::string GetServicePath()const {return service_path_;}

	std::string GetServiceName()const {return service_name_;}

	std::string GetUpdatesSavePath()const {return service_updates_save_path_;}

	std::string GetUpdateCmdPath()const {return service_update_cmd_file_;}

	std::string GetUpdateStartCmd()const {return service_start_cmd_;}

	std::string GetUpdateStopCmd()const {return service_stop_cmd_;}

	std::string GetNewVersionCopyCmd(){
		std::string UpdatePackDirName(update_cmd_.GetUpdatePackNameFromCmd().c_str(),update_cmd_.GetUpdatePackNameFromCmd().size() - 4);

		return "mv " + GetUpdatesSavePath()  + UpdatePackDirName + "/* " + GetServicePath();
	}

	std::string GetNewVersionSavePath(){
		return service_updates_save_path_ + update_cmd_.GetUpdatePackNameFromCmd();
	}

	std::string GetNewVersionUnPackCmd(){
		// tar xcf pack.tgz -C target_dir 
		// tar xcf ../updates/xxx.tgz -C ../updates
		return "tar -xvf " + GetNewVersionSavePath() + " -C " + service_updates_save_path_;
	}
private:
	std::string  service_path_;

	std::string  service_name_;

	std::string  service_update_cmd_file_;

	std::string  service_updates_save_path_;
	
	std::string  service_update_log_file_;

	std::string  service_start_cmd_;

	std::string  service_stop_cmd_;

	UpdateCmd    update_cmd_;
};

class RemoteUpdate : public BaseServiceUpdate
{
public:
	virtual  int  UpdateCheck(BaseUpdateCtx* ctx);

	virtual  int  UpdateGetNewVersion(BaseUpdateCtx* ctx);

	virtual  int  UpdateValidNewVersion(BaseUpdateCtx* ctx);

	virtual  int  UpdateStopService(BaseUpdateCtx*);

	virtual  int  UpdateReplaceWithNewVersion(BaseUpdateCtx*);

	virtual  int  UpdateStartService(BaseUpdateCtx*);

	virtual  int  UpdateDone(BaseUpdateCtx* ctx);

private:
	std::string GetFileMd5(const std::string & file_name);

	RemoteUpdateCtx * GetCtx(BaseUpdateCtx* ctx){
		assert(ctx);
		return dynamic_cast<RemoteUpdateCtx *>(ctx);
	}
};

#endif // REMOTE_UPDATE_H
