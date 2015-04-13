#ifndef __WALK_REQUEST_SERVICE__
#define __WALK_REQUEST_SERVICE__

#include "../orm/walkRequestORM.cpp"
#include <memory>
#include <thread>
#include <string>

class WalkRequestService 
{
private:
	WalkRequestORM walkRequestORM;
public:
	WalkRequestService(sqlite3 * db) : walkRequestORM(db)
	{

	}	

	~WalkRequestService()
	{

	}
};


#endif