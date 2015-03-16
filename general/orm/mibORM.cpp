#ifndef __MIB_ORM__
#define __MIB_ORM__

#include "ORM.cpp"

class MibORM : ORM
{
private:
// ============== MIBS ===============
	sqlite3 * db;
public:
	MibORM(sqlite3 * db)
	{
		this -> db = db;
	}

};

#endif

