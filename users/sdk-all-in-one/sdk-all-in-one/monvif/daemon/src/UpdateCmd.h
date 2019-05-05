#ifndef UPDATE_CMD_H
#define UPDATE_CMD_H

#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
#include "string.h"

/*********************************fifo***************************************/
int open_fifo(const char *path);


class FiFo {
public:
	FiFo(): _fd(-1) {}
	~FiFo() {
		if (_fd != -1) {
			close(_fd);
		}
	}

	int init(const char* fpath)
	{
		int mode = 0666 | O_NONBLOCK | O_NDELAY;
		errno = 0;

		if (mkfifo(fpath, mode) < 0)
		{
			if (errno != EEXIST)
			{
				return -1;
			}
		}

		if (_fd != -1)
		{
			close(_fd);
			_fd = -1;
		}

		// O_NONBLOCK | O_NDELAY should be set here
		if ( (_fd = open(fpath, O_RDWR)) < 0) {
			return -1;
		}
		//unlink(fpath);//fix me if unlink;

		int value = fcntl(_fd, F_GETFL, 0);
		if (value == -1) {
			return -2;
		}
		if (value & O_NONBLOCK) {
			return 0;
		}
		if (fcntl(_fd, F_SETFL, value | O_NONBLOCK | O_NDELAY) < 0) {
			return -1;
		}
		return 0;
	}
	int fd() {
		return _fd;
	}
private:
	int _fd;
};

class UpdateCmd
{
public:
	int Initilize(const std::string& service_path, const std::string & cmd_file_path) {
		cmd_file_ = service_path + cmd_file_path;
		std::string filo_file_path = service_path + "update.fifo";
		file_fifo_.init(filo_file_path.c_str());
		//set_ready_flag();
		return 0;
	}

	int PushCmd(const std::string& pack_name, const std::string& md5, const std::string &url ) {
		std::string cmd = pack_name + "#" + md5 + "#" + url;
		std::fstream cmd_file_ss;
		std::string strFileName = "update.cmd";
		cmd_file_ss.open(strFileName.c_str(), std::fstream::out | std::ios_base::trunc);
		cmd_file_ss << cmd;
		cmd_file_ss.close();
		set_ready_flag();
	}

	int PollingWaitAndReadCmd() {
		char buffer[128];
		int result, nread;
		fd_set  readfds;
		struct timeval timeout;

		FD_ZERO(&readfds);
		FD_SET(file_fifo_.fd(), &readfds);

		timeout.tv_sec = 2;
		timeout.tv_usec = 500000;
		result = select(FD_SETSIZE, &readfds, (fd_set *)0, (fd_set *)0, &timeout);
		switch (result)
		{
		case 0:
		{
			//cout << "select time out "<< endl;
			return 1; //time out
		}
		case -1:
		{
			//cout << "select no ready fd" << endl;
			return 2; //
		}
		default:
			break;
		}

		clear_ready_flag();
		if (Load())
		{
			return 3;
		}
		
		return 0;
	}

	int Load() {
		ifstream fin(cmd_file_.c_str(), std::ios::in);
		if (!fin.is_open())
		{
			return 1;
		}

		char line[1024] = {0};
		while (fin.getline(line, sizeof(line)))
		{
			cmd_.clear();
			cmd_.assign(&line[0], strlen(&line[0]));
		}

		if (!cmd_.empty())
		{
			ParseCmd();
		}
		else
		{
			return 2;
		}

		fin.clear();
		fin.close();
		return 0;
	}

	int ResetCmd() {
		cmd_.clear();
		cmd_fields_.clear();
	}

	bool IsPackNameValid(){
		if (GetUpdatePackNameFromCmd().find(".tgz") == string::npos)
		{
			return false;
		}
		return true;
	}

	bool IsUpdateCmdEmpty() {
		return cmd_.empty();
	}

	std::string GetUpdateHostFromCmd() {
		return GetCmdFiled(2);
	}

	std::string GetUpdateFileMd5FromCmd() {
		return GetCmdFiled(1);
	}

	std::string GetUpdatePackNameFromCmd() {
		return GetCmdFiled(0);
	}

	std::string GetFullDownLoadPath() {
		return GetCmdFiled(2) + GetCmdFiled(0);
	}

	std::string GetCmdFile() {
		return cmd_file_;
	}

	void clear_ready_flag() {
		static char buf[1];
		read(file_fifo_.fd(), buf, 1);
	}
	void set_ready_flag() {
		write(file_fifo_.fd(), "\0", 1);
	}
private:
	std::string GetCmdFiled(int index) {
		if (cmd_fields_.size() == 0)
		{
			return "";
		}
		return cmd_fields_[index % cmd_fields_.size()];
	}

 	void ParseCmd(){
		string::size_type pos = std::string::npos;
		cmd_fields_.clear();
		do
		{
			pos = cmd_.find_first_of("#");
			if (pos != std::string::npos)
			{
				cmd_fields_.push_back(cmd_.substr(0, pos));
				cmd_ = cmd_.substr(pos + 1, cmd_.size() - pos - 1);
			}
			else
			{
				if (cmd_.size() != 0 )
				{
					cmd_fields_.push_back(cmd_);
				}
			}

		} while (pos != std::string::npos);
 	}
private:
	std::string cmd_;

	std::string cmd_file_;

	std::vector<std::string> cmd_fields_;

	FiFo file_fifo_;
};

#endif // UPDATE_CMD_H
