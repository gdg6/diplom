#ifndef __DEVIE_SERVICE__
#define __DEVIE_SERVICE__

#include "../orm/deviceORM.cpp"
#include <memory>
#include <thread>
#include <string>

class DeviceService 
{
private:
	DeviceORM deviceORM;

public:
	
	DeviceService(sqlite3 * db) : deviceORM(db)
	{
	}
	
	int save(Device device)
	{
		return deviceORM.insertDevice(device);
	}

	std::shared_ptr<Device> getByPK(int id)
	{
		return deviceORM.getByPK(id);
	}
		
	std::shared_ptr<std::vector<std::shared_ptr<Device>>> getAll()
	{
		return deviceORM.getAll();
	}
	
};


#endif


