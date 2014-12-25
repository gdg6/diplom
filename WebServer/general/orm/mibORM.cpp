#ifndef __MIB_ORM__
#define __MIB_ORM__

#include "ORM.cpp"

class MibORM : ORM
{
private:
// ============== MIBS ===============
    std::string sql_mib_create_table = "CREATE TABLE \'mibs\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'file_name_mib\' varchar(255), \'created_at\' datetime, \'updated_at\' datetime);";
	sqlite3 * db;
public:
	MibORM(sqlite3 * db)
	{
		this -> db = db;
	}

};

#endif

