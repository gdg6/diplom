#ifndef __REPORT_ORM__
#define __REPORT_ORM__

#include "ORM.cpp"
#include <iostream>
class ReportORM : ORM
{
private:
	sqlite3 * db;
	int rc;
	std::string sql_create_table = "CREATE TABLE \'reports\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'device_id\' integer, \'type\', \'context\' text, \'created_at\' datetime, \'updated_at\' datetime);";
	std::string sql_insert = "INSERT INTO \'reports\'  (\'device_id\', \'type\', \'context\', \'created_at\', \'updated_at\') VALUES (?, ?, ?, ?, ?);";
	
public:
	ReportORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}
	
	// return 101 if success
	int insertReport(int device_id, std::string type, std::string context)
	{
		sqlite3_stmt *stmt;
		std::string time  = ORM::currentDateTime();
		int rc = sqlite3_prepare_v2(db, sql_insert.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		
		rc = sqlite3_bind_int(stmt, 1, device_id);    
		rc = sqlite3_bind_text(stmt, 2, type.c_str(), type.length(), SQLITE_STATIC);    
		rc = sqlite3_bind_text(stmt, 3, context.c_str(), context.length(), SQLITE_STATIC);    // Using parameters ("?") is not
		rc = sqlite3_bind_text(stmt, 4, time.c_str(), time.length(), SQLITE_STATIC);    // Using parameters ("?") is not
		rc = sqlite3_bind_text(stmt, 5, time.c_str(), time.length(), SQLITE_STATIC);    // Using parameters ("?") is not
		if (rc != SQLITE_OK) 
		{                 	
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return rc;
		}
 
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}
	
	int sqlExec(std::string & sql)
	{
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}
	
};

#endif

