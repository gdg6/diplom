#ifndef __WALK_RAPORT_SERVICE__
#define __WALK_RAPORT_SERVICE__

#include "../orm/walkRaportORM.cpp"
#include <memory>
#include <thread>
#include <string>

class WalkRaportService
{
private:
	WalkRaportORM walkRaportORM;
public: 
	
	WalkRaportService(sqlite3 * db) : walkRaportORM(db)
	{

	}

	int save(WalkRaport walkRaport)
	{
		return walkRaportORM.insertWalkRaport(walkRaport);
	}

	std::shared_ptr<std::vector<std::shared_ptr<WalkRaport>>> getListByDeviceId(int device_id)
	{
		return walkRaportORM.getListByDeviceId(device_id);
	}

	int destroy(int id)
	{
		return walkRaportORM.deleteWalkRaport(id);
	}

	~WalkRaportService()
	{

	}
};

#endif