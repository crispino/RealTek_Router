#include "update_cmd.h"
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

struct cmd_ctx
{
	int fd;
};

struct cmd_ctx cmd_ctx_;

void set_fifo_ready();
int update_cmd_inti(const char * cmd_file_fifo);

int update_cmd_inti(const char * cmd_file_fifo)
{
	int mode = 0666 | O_NONBLOCK | O_NDELAY;
	errno = 0;

	if (mkfifo(cmd_file_fifo, mode) < 0)
	{
		if (errno != EEXIST)
		{
			return 1;
		}
	}

	if (cmd_ctx_.fd == -1)
	{
		close(cmd_ctx_.fd);
		cmd_ctx_.fd = -1;
	}

	// O_NONBLOCK | O_NDELAY should be set here
	if ( (cmd_ctx_.fd = open(cmd_file_fifo, O_RDWR)) < 0)
	{
		return 2;
	}

	int value = fcntl(cmd_ctx_.fd, F_GETFL, 0);
	if (value == -1)
	{
		return 3;
	}

	if (value & O_NONBLOCK)
	{
		return 0;
	}

	if (fcntl(cmd_ctx_.fd, F_SETFL, value | O_NONBLOCK | O_NDELAY) < 0)
	{
		return 4;
	}
	return 0;
}

void set_fifo_ready()
{
	write(cmd_ctx_.fd, "\0", 1);
}

int update_cmd_push(const char * cmd_file_name,const char * package_name, const char* package_md5, const char* url)
{
	if (update_cmd_inti("update.fifo"))
	{
		return 1;
	}

	FILE *fp = fopen(cmd_file_name,"w+");
	if (fp == NULL)
	{
		return 2;
	}

	char cmd[1024] = {0};

	snprintf(cmd,sizeof(cmd),"%s#%s#%s",package_name,package_md5,url);

	fputs(cmd,fp);

	fclose(fp); 

	set_fifo_ready();

	return 0;
}