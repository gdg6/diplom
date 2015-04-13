#ifndef __LOG_ORM__
#define __LOG_ORM__ 

#include "ORM.cpp"
#include "../model/log.cpp"
#include <iostream>

class LogORM : public ORM {
private:


	std::string sql_insert = "insert into \'logs\' (\'l_type\', \'context\', \'created_at\', \'updated_at\') VALUES(?, ?, ?, ?)";


	int rc;
	sqlite3 * db;

public:
	LogORM(sqlite3 *db) : rc(0)
	{
		this -> db = db;
	}


	// int insertLog(Log log)
	// {
	// 	return insertLog(log.getType(), log.getContext());
	// }


	int insertLog(std::string l_type, std::string context)
	{
		sqlite3_stmt * stmt;
		std::string time  = currentDateTime();
		int rc = sqlite3_prepare_v2(db, sql_insert.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}

		rc |= sqlite3_bind_text(stmt, 1, l_type.c_str(), l_type.length(), SQLITE_STATIC);    
		rc |= sqlite3_bind_text(stmt, 2, context.c_str(), context.length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 3, time.c_str(), time.length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 4, time.c_str(), time.length(), SQLITE_STATIC); 

		if (rc != SQLITE_OK) 
		{                 
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return rc;
		}
 
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	~LogORM()
	{

	}
};

#endif

