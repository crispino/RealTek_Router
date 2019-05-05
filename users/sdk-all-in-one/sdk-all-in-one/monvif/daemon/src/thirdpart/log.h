/**
 * @file log.h
 * @brief write log
 * @author FENGWANFENG
 * @date 2013-10-22
 *
 *
 * @The usage of log, all of the following step is optional,
 *1. Setting the report log level, default value is logDEBUG4
 *2. Setting max size of log file , default value is 1024*1024*1024
 *3. Setting the prefix of log file name, default value is ./log
 *
 *here is an example
 *
 *#include "log.h"
 *#include <iostream>
 *
 *int main(int argc, char* argv[])
 *{
 *    try
 *    {
 *        FILELog::ReportingLevel() = FILELog::FromString(argv[1] ? argv[1] : "DEBUG1");
 *        Output2FILE::LogMaxSizeInByte() = 1024*1024;
 *        Output2FILE::NamePrefix()=std::string("mylog");
 *	      const int count = 1000003;
 *        FILE_LOG(logDEBUG) << "A loop with " << count << " iterations";
 *        for (int i = 0; i != count; ++i){
 *	        FILE_LOG(logDEBUG1) << "the counter i = " << i << "  "<< Output2FILE::CurrentSize();
 *        }
 *        return 0;
 *    }
 *    catch(const std::exception& e)
 *    {
 *        FILE_LOG(logERROR) << e.what();
 *    }
 *    return -1;
 *}
 *
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <iostream>
#include <string.h>
#include <string>
#include <cstdio>
#include <errno.h>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
inline string get_date()
{
	struct tm curr;
	time_t t;
	time(&t);
	curr = *localtime(&t);
	char sTmp[1024];
	strftime(sTmp,sizeof(sTmp),"%Y%m%d",&curr);
	return string(sTmp);
}

extern int errno;
inline std::string NowTime();

enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

template <typename T>
class Log
{
	public:
		Log();
		virtual ~Log();
		std::ostringstream& Get(TLogLevel level = logINFO);
	public:
		static TLogLevel& ReportingLevel();
		static std::string ToString(TLogLevel level);
		static TLogLevel FromString(const std::string& level);
	protected:
		std::ostringstream os;
	private:
		class MyLock{
			public:
				MyLock(){
					pthread_mutex_init(&mtx, NULL);
				}
				~MyLock(){
					pthread_mutex_destroy(&mtx);
				}
				pthread_mutex_t mtx;
		};
		static pthread_mutex_t* get_mutex(){
			//static pthread_mutex_t mt;
			static MyLock lk;
			return &lk.mtx;
		}

		/*
		static pthread_once_t* get_once_obj(){
			static pthread_once_t obj;
			return &obj;
		}
		static void once_fun(){
			pthread_mutex_init(get_mutex(),NULL);
		}
		*/
		Log(const Log&);
		Log& operator =(const Log&);
};

//1M
#define LOG_CACHE_SIZE (1<<20)
	template <typename T>
Log<T>::Log(){
	//pthread_once(Log<T>::get_once_obj(), Log<T>::once_fun);
	pthread_mutex_lock(Log<T>::get_mutex());
}
	template <typename T>
Log<T>::~Log(){
	os << std::endl;
	T::Output(os.str());
	pthread_mutex_unlock(Log<T>::get_mutex());
}

	template <typename T>
std::ostringstream& Log<T>::Get(TLogLevel level)
{
	if(os.str().size() > LOG_CACHE_SIZE){
		T::Output(os.str());
		os.str("");//clear os content
	}
	os << "[" << NowTime();
	os << "]<" << ToString(level) << "> ";
	//os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
	return os;
}

	template <typename T>
TLogLevel& Log<T>::ReportingLevel()
{
	static TLogLevel reportingLevel = logDEBUG4;
	return reportingLevel;
}

	template <typename T>
std::string Log<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
	return buffer[level];
}

	template <typename T>
TLogLevel Log<T>::FromString(const std::string& level)
{
	if (level == "DEBUG4")
		return logDEBUG4;
	if (level == "DEBUG3")
		return logDEBUG3;
	if (level == "DEBUG2")
		return logDEBUG2;
	if (level == "DEBUG1")
		return logDEBUG1;
	if (level == "DEBUG")
		return logDEBUG;
	if (level == "INFO")
		return logINFO;
	if (level == "WARNING")
		return logWARNING;
	if (level == "ERROR")
		return logERROR;
	Log<T>().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
	return logINFO;
}

class Output2FILE
{
	public:
		static FILE*& Stream();
		static void Output(const std::string& msg);
		static unsigned int& LogMaxSizeInByte();
		static std::string& NamePrefix();
		static std::string& NameSubfix();
		static long int& CurrentSize();
		static unsigned int& DayMaxLogNum();
		//static pthread_mutex_t* get_mutex();
		//static bool& mutex_init_falg();
		//static pthread_once_t* get_once_obj();
		//static void once_fun();
	private:
		static FILE* OpenFileStream();
};
/*
   inline pthread_mutex_t* Output2FILE::get_mutex(){
   static pthread_mutex_t mt;
   return &mt;
   }
   inline pthread_once_t* Output2FILE::get_once_obj(){
   static pthread_once_t obj;
   return &obj;
   }
   inline void Output2FILE::once_fun(){
   std::cout << "init one fun call." << std::endl;
   pthread_mutex_init(get_mutex(),NULL);
   }
   inline bool& mutex_init_falg(){
   static bool is_init = false;
   return is_init;
   }*/
inline unsigned int& Output2FILE::DayMaxLogNum()
{
	static unsigned int maxfilenum = 10;
	return maxfilenum;
}
inline unsigned int& Output2FILE::LogMaxSizeInByte()
{
	static unsigned int maxsize= 1024*1024*1024;
	return maxsize;
}
inline long int& Output2FILE::CurrentSize()
{
	static long int cursize=0;
	return cursize;
}
inline std::string& Output2FILE::NamePrefix()
{
	static std::string nameprefix= "./log";
	return nameprefix;
}
inline std::string& Output2FILE::NameSubfix()
{
	static std::string namesubfix = get_date();
	return namesubfix;
}
inline FILE* Output2FILE::OpenFileStream()
{
	FILE* pStream = NULL;
	NameSubfix()=get_date();
	std::string FilePath = NamePrefix()+string("_")+NameSubfix();
	std::replace(FilePath.begin(),FilePath.end(),' ','_');
	FilePath.append(".log");
	if(( pStream = fopen(FilePath.c_str(),"a+") )==NULL)
	{
		perror("access file failed ");
		perror(FilePath.c_str());
		pStream = stderr;
	}
	//set close fd on exec
	int fd = fileno(pStream);
	if(fd >= 0){
		fcntl(fd, F_SETFD, FD_CLOEXEC);
	}
	//get file len
	fseek(pStream, 0L, SEEK_END);
	CurrentSize() = ftell(pStream);

	return pStream;

}
inline FILE*& Output2FILE::Stream()
{

	static FILE* pStream = stderr;
	if( pStream == stderr )
	{
		pStream = OpenFileStream();
	}

	if(NameSubfix() != get_date()){
		fclose(pStream);
		pStream = OpenFileStream();
	}


	if(CurrentSize() >= LogMaxSizeInByte())
			//(0 != NameSubfix().compare(0,10,NowTime(),0,10)))
	{
		// new file must be created in the following
		// two situation: 1. the size of log is equal
		// or large than LogMaxSizeInByte() 2.The log
		// file can only record the logs which are generated
		// in current day.
		if(EOF == fclose(pStream)){
			perror("close file failed");
			/*
		} else {
			CurrentSize()=0;
			*/
		}
		string _filename = NamePrefix()+string("_")+NameSubfix();
		std::replace(_filename.begin(),_filename.end(),' ','_');
		char sTmp[32]="";
		string from,to;
		//rename other
		for (int i = DayMaxLogNum()-2; i >= 0; i--) {
			sprintf(sTmp,"_%d",i);
			if (i == 0) from = _filename+".log";
			else from = _filename+sTmp+".log";
			sprintf(sTmp,"_%d",i+1);
			to = _filename+sTmp+".log";
			if (access(from.c_str(), F_OK) == 0) {
				if (rename(from.c_str(), to.c_str()) < 0) {
					std::cerr << "rename " << from << " --> " << to << " fail!!!" << std::endl;
				}
			}
		}
		pStream = OpenFileStream();
	}
	return pStream;
}

inline void Output2FILE::Output(const std::string& msg)
{
	FILE* pStream = Stream();
	int len;
	if (!pStream)
		return;
	len=fprintf(pStream, "%s", msg.c_str());
	if(len > 0 )
	{
		CurrentSize()+=len;
	}
	fflush(pStream);
}

class FILELog : public Log<Output2FILE> {};
//typedef Log<Output2FILE> FILELog;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG4
#endif

#define FILE_LOG(level) \
	if (level > FILELOG_MAX_LEVEL) ;\
else if (level > FILELog::ReportingLevel()) ; \
else FILELog().Get(level)<<basename(__FILE__)<<":"<<__LINE__<<":"

    
//else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream()) ;

#include <sys/time.h>



inline std::string NowTime()
{
	char buffer[22];
	time_t t;
	time(&t);
	//tm r = {0};
	tm r;
	memset(&r, 0, sizeof(tm));
	strftime(buffer, sizeof(buffer), "%F %T", localtime_r(&t, &r));
	struct timeval tv;
	gettimeofday(&tv, 0);
	char result[100] = {0};
	std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
	return result;
}

#endif //__LOG_H__
