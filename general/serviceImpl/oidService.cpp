#ifndef __OID_SERVICE__
#define __OID_SERVICE__

#include "../orm/oidORM.cpp"


class OidService
{
private: 
	OidORM oidORM; 
public:

	OidService(sqlite3 * db) : oidORM(db)
	{
		
	}
		
	std::shared_ptr<Oid> getByPK(int id)
	{
		return oidORM.getByPK(id);
	}
	
	std::shared_ptr<std::vector<std::shared_ptr<Oid>>> getByDeviceId(int device_id)
	{
		return oidORM.getListByDeviceId(device_id);
	}

	int save(Oid oid)
	{
		return oidORM.insertOid(oid);
	}

	std::shared_ptr<std::vector<std::shared_ptr<Oid>>> getAll()
	{
		return oidORM.getAll();
	}
	
};

#endif