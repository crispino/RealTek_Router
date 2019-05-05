#ifndef _BASE_UPDATE_H
#define _BASE_UPDATE_H

#include <string>
class BaseUpdateCtx
{
public:
	virtual  int  Initilize(const std::string & conf) = 0;
};

class BaseServiceUpdate
{
public:
	int Do(BaseUpdateCtx* ctx)
	{
		if (UpdateCheck(ctx))
		{
			return 1;
		}

		if (UpdateGetNewVersion(ctx))
		{
			return 2;
		}

		if (UpdateValidNewVersion(ctx))
		{
			return 3;
		}

		if (UpdateStopService(ctx))
		{
			return 4;
		}

		if (UpdateReplaceWithNewVersion(ctx))
		{
			return 5;
		}

		if (UpdateStartService(ctx))
		{
			return 6;
		}

		return UpdateDone(ctx);
	}
public:
	virtual  int  UpdateCheck(BaseUpdateCtx*) = 0;

	virtual  int  UpdateGetNewVersion(BaseUpdateCtx*) = 0;

	virtual  int  UpdateValidNewVersion(BaseUpdateCtx*) = 0;

	virtual  int  UpdateStopService(BaseUpdateCtx*) = 0;

	virtual  int  UpdateReplaceWithNewVersion(BaseUpdateCtx*) = 0;

	virtual  int  UpdateStartService(BaseUpdateCtx*) = 0;

	virtual  int  UpdateDone(BaseUpdateCtx*) = 0;
};




#endif // _BASE_UPDATE_H
