#include "RemoteUpdate.h"
#include "SysCmdUtil.h"
#include "thirdpart/xml_conf.h"
#include "thirdpart/log.h"
#include "thirdpart/md5/md5.h"


#define   GET_INT32_CFG_ITEM(xmlcfgobj, xml_path)     \
	      atoi(xmlcfgobj[xml_path].c_str())	          \

#define   GET_STRING_CFG_ITEM(xmlcfgobj, xml_path)    \
		  xmlcfgobj[xml_path]						  \


/*******************************************RemoteUpdateCtx*************************************************/
int RemoteUpdateCtx::Initilize(const std::string & conf)
{
	/*
	 * load update cfg file
	 */
	ConfFile cf;
	if (!cf.init(conf)) {
		FILE_LOG(logERROR) << "Initilize file failed ";
		return 1;
	}

	service_path_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_path");
	FILE_LOG(logDEBUG) << "service_path is  " << service_path_;

	service_name_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_name");
	FILE_LOG(logDEBUG) << "service_name is  " << service_name_;

	service_update_cmd_file_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_update_cmd_file");
	FILE_LOG(logDEBUG) << "service_update_cmd_file is  " << service_update_cmd_file_;

	service_updates_save_path_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_updates_save_path");
	FILE_LOG(logDEBUG) << "service_updates_save_path is  " << service_updates_save_path_;

	service_update_log_file_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_update_log_file");
	FILE_LOG(logDEBUG) << "service_update_log_file is  " << service_update_log_file_;

	service_stop_cmd_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_stop_cmd");
	FILE_LOG(logDEBUG) << "service_stop_cmd is  " << service_stop_cmd_;

	service_start_cmd_ = GET_STRING_CFG_ITEM(cf, "/root/update/service_start_cmd");
	FILE_LOG(logDEBUG) << "service_start_cmd is  " << service_start_cmd_;

	update_cmd_.Initilize(service_path_, service_update_cmd_file_);
	return 0;
}


/**********************************************RemoteUpdate**************************************************/
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)
#include "stdio.h"
std::string RemoteUpdate::GetFileMd5(const std::string & file_name)
{
	std::ifstream in(file_name.c_str(), ios::binary);
	if (!in)
		return "";

	MD5_CTX md5;
	MD5Init(&md5);
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
			MD5Update(&md5, (unsigned char*)&buffer[0], length);
	}
	in.close();

	unsigned char md5_value[MD5_SIZE];
	MD5Final(&md5, md5_value);

	char md5_str[1024];
	for (int i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i * 2, 2 + 1, "%02x", md5_value[i]);
	}

	return std::string(md5_str);
}

int RemoteUpdate::UpdateCheck(BaseUpdateCtx* ctx)
{
	/*
	 * 1. if update.cmd is empty, return 1
	 * 2. if update.cmd is not empty, get cmd and check if need to update(check md5 of current file and md5 in cmd)
	 * 3. if md5 is the same clear cmd return 1
	 */
	//FILE_LOG(logDEBUG) << "update cmd check..." ;
	if (GetCtx(ctx)->GetUpdateCmd().PollingWaitAndReadCmd())
	{
		//FILE_LOG(logDEBUG) << "no valid cmd, need not to update";
		return 1;
	}

	FILE_LOG(logDEBUG) << "new version filename = " << GetCtx(ctx)->GetUpdateCmd().GetUpdatePackNameFromCmd();
	FILE_LOG(logDEBUG) << "new version md5  = " << GetCtx(ctx)->GetUpdateCmd().GetUpdateFileMd5FromCmd();
	FILE_LOG(logDEBUG) << "new version host = " << GetCtx(ctx)->GetUpdateCmd().GetUpdateHostFromCmd();

	if (GetCtx(ctx)->GetUpdateCmd().IsUpdateCmdEmpty())
	{
		FILE_LOG(logDEBUG) << "update cmd check is empty, do not need to update" ;
		return 2;
	}

	if (!GetCtx(ctx)->GetUpdateCmd().IsPackNameValid())
	{
		FILE_LOG(logDEBUG) << "packname=" <<  GetCtx(ctx)->GetUpdateCmd().GetUpdatePackNameFromCmd() << " is not valid" ;
		return 3;
	}

	return 0;
}

int RemoteUpdate::UpdateGetNewVersion(BaseUpdateCtx* ctx)
{
	/*
	 * download new version and save it through wget -O save_target -c download_url
	 */

	//clear save path
	std::string clear_cmd = "rm -rf " + GetCtx(ctx)->GetUpdatesSavePath() + "/*";
	int iret = SysCmdUtil::ExecuteAndWaitCmd(clear_cmd);
	if (iret)
	{
		FILE_LOG(logDEBUG) << "clear_cmd execute failed for " << iret;
		return 1;
	}

	std::string download_cmd = "wget -t3 -T10 -O " + GetCtx(ctx)->GetNewVersionSavePath();
	download_cmd += " -c " + GetCtx(ctx)->GetUpdateCmd().GetFullDownLoadPath();
	FILE_LOG(logDEBUG) << "download_cmd=" << download_cmd;
	iret = SysCmdUtil::ExecuteAndWaitCmd(download_cmd);
	if (iret)
	{
		FILE_LOG(logDEBUG) << "cmd execute failed for " << iret;
		return 1;
	}

	return 0;
}

int RemoteUpdate::UpdateValidNewVersion(BaseUpdateCtx* ctx)
{
	/*
	 * check md5 of new version and md5 in cmd
	 */
	FILE_LOG(logDEBUG) << "UpdateValidNewVersion..";
	FILE_LOG(logDEBUG) << "GetNewVersionSavePath = " << GetCtx(ctx)->GetNewVersionSavePath();
	std::string md5_of_newversion =  GetFileMd5(GetCtx(ctx)->GetNewVersionSavePath());

	FILE_LOG(logDEBUG) << "md5 from cmd = " << GetCtx(ctx)->GetUpdateCmd().GetUpdateFileMd5FromCmd();
	FILE_LOG(logDEBUG) << "md5 of newversion = " << md5_of_newversion;

	if (GetCtx(ctx)->GetUpdateCmd().GetUpdateFileMd5FromCmd() == md5_of_newversion)
	{
		return 0;
	}
	return 1;
}

int  RemoteUpdate::UpdateStopService(BaseUpdateCtx* ctx)
{
	FILE_LOG(logDEBUG) << "UpdateStopService...";
	int iret = SysCmdUtil::ExecuteAndWaitCmd(GetCtx(ctx)->GetUpdateStopCmd());
	if (iret)
	{
		FILE_LOG(logDEBUG) << "stop cmd execute failed for " << iret;
		return 1;
	}

	int retry_count = 20;
	while (retry_count-- > 0) {
		if (SysCmdUtil::IsProcessExist(GetCtx(ctx)->GetServiceName()))
		{
			if (SysCmdUtil::ExecuteAndWaitCmd(GetCtx(ctx)->GetUpdateStopCmd()))
			{
				return 2;
			}
		}
		usleep( 1000 ); 
	}


	return 0 ;
}

int  RemoteUpdate::UpdateReplaceWithNewVersion(BaseUpdateCtx* ctx)
{
	FILE_LOG(logDEBUG) << "UpdateReplaceWithNewVersion...";


	int iret = SysCmdUtil::ExecuteAndWaitCmd(GetCtx(ctx)->GetNewVersionUnPackCmd());
	if (iret)
	{
		FILE_LOG(logDEBUG) << "tar cmd execute failed for " << iret;
		return 1;
	}

	iret = SysCmdUtil::ExecuteAndWaitCmd(GetCtx(ctx)->GetNewVersionCopyCmd());
	if (iret)
	{
		FILE_LOG(logDEBUG) << "mv cmd execute failed for " << iret;
		return 1;
	}
	return 0;
}

int  RemoteUpdate::UpdateStartService(BaseUpdateCtx* ctx)
{
	FILE_LOG(logDEBUG) << "UpdateStartService...";
	int iret = SysCmdUtil::ExecuteAndWaitCmd(GetCtx(ctx)->GetUpdateStartCmd());
	if (iret)
	{
		FILE_LOG(logDEBUG) << "start cmd execute failed for " << iret;
		return 1;
	}

	return 0;
}

int RemoteUpdate::UpdateDone(BaseUpdateCtx* ctx)
{
	FILE_LOG(logDEBUG) << "UpdateDone";
	GetCtx(ctx)->GetUpdateCmd().ResetCmd();
	return 0;
}

