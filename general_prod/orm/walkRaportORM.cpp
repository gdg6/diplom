#ifndef __WALK_RAPORT_ORM__
#define __WALK_RAPORT_ORM__

#include "ORM.cpp"
#include "../model/walkRaport.cpp"
#include <iostream>

class WalkRaportORM : public ORM 
{
private:
	std::string sql_select_by_device_id = "SELECT \'path\', \'device_id\' FROM walk_raports WHERE walk_raports.device_id = ?";
	std::string sql_select_by_id = "SELECT \'path\', \'device_id\' FROM walk_raports WHERE walk_raports.id = ?";
	std::string sql_insert = "INSERT INTO \'walk_raports\' (\'path\', \'device_id\', \'created_at\', \'updated_at\') VALUES(?, ?, ?, ?)";
	std::string sql_delete_by_id = "DELETE FROM walk_raports WHERE id = ?";

	int rc;
	sqlite3 * db;

public:
	WalkRaportORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}

	int insertWalkRaport(WalkRaport & walkRaport)
	{
		sqlite3_stmt * stmt;
		std::string time  = currentDateTime();
		int rc = sqlite3_prepare_v2(db, sql_insert.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}

		rc |= sqlite3_bind_text(stmt, 1, walkRaport.getPath().c_str(), walkRaport.getPath().length(), SQLITE_STATIC);    
		rc |= sqlite3_bind_int(stmt, 2, walkRaport.getDeviceId()); 
		rc |= sqlite3_bind_text(stmt, 3, time.c_str(), time.length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 4, time.c_str(), time.length(), SQLITE_STATIC); 

		if (rc != SQLITE_OK) 
		{                 
			sqlite3_finalize(stmt);            
			return rc;
		}
 
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}


	std::shared_ptr<std::vector<std::shared_ptr<WalkRaport>>> getListByDeviceId(int & device_id)
	{

		std::shared_ptr<std::vector<std::shared_ptr<WalkRaport>>> list(new std::vector<std::shared_ptr<WalkRaport>>());
		sqlite3_stmt *stmt;
		
		int rc = sqlite3_prepare_v2(db, sql_select_by_device_id.c_str(), -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			// throw std::string (sqlite3_errmsg(db));
			return list;
		}

		rc = sqlite3_bind_int(stmt, 1, device_id);  
		if (rc != SQLITE_OK) {                 
			sqlite3_finalize(stmt);            
			return list;           
		}

		std::shared_ptr<WalkRaport> walkRaport;
		while((rc = sqlite3_step(stmt) ==  100)) { 
			walkRaport = std::shared_ptr<WalkRaport>(new WalkRaport);
			walkRaport -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			walkRaport -> setPath( std::string((const char *)sqlite3_column_text(stmt, 1)));
			list->push_back(walkRaport);
		}

		sqlite3_finalize(stmt);
		return list;
	}
	

	int deleteWalkRaport(int id)
	{
		sqlite3_stmt * stmt;
		std::string time  = currentDateTime();
		int rc = sqlite3_prepare_v2(db, sql_delete_by_id.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}

		//FIXME надо бы проверять на существование такое устройство
		rc |= sqlite3_bind_int(stmt, 1, id);

		if (rc != SQLITE_OK) 
		{                 
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return rc;
		}
 
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	~WalkRaportORM()
	{

	}
};


#endif
