#ifndef __OID_ORM__
#define __OID_ORM__

#include "ORM.cpp"
#include "../model/oid.cpp"

#include <iostream>
#include <string>

class OidORM : public ORM
{
private:
	std::string sql_create = "CREATE TABLE \'oids\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'oid\' varchar(255), \'translate\' varchar(255), \'created_at\' datetime, \'updated_at\' datetime);";
	std::string sql_select_by_id = "select Oid, Translate FROM oids WHERE Id = ?";
	std::string sql_select_all  = "SELECT * FROM oids;";

	int rc; //status for db;
	sqlite3 * db;

public:
	OidORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}

	std::shared_ptr<std::vector<std::shared_ptr<Oid>>> getAll()
	{

		std::shared_ptr<std::vector<std::shared_ptr<Oid>>> list(new std::vector<std::shared_ptr<Oid>>());
		sqlite3_stmt *stmt;
		
		int rc = sqlite3_prepare_v2(db, sql_select_all.c_str(), -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			throw std::string (sqlite3_errmsg(db));
		}


		std::shared_ptr<Oid> oid;
		while((rc = sqlite3_step(stmt) ==  100)) { // до тех пор пока запрос возращяет результат
			oid = std::shared_ptr<Oid>(new Oid);
			oid -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			oid -> setOid( std::string((const char *)sqlite3_column_text(stmt, 1)));
			oid -> setTranslate( std::string((const char *)sqlite3_column_text(stmt, 2)));
			oid -> setActive( (atoi((const char *)sqlite3_column_text(stmt, 3)) == 0 ? false : true ));
			list->push_back(oid);
		}

		sqlite3_finalize(stmt);

		return list;
	}
	
	std::shared_ptr<Oid> getByPK(int id)
	{

		std::shared_ptr<Oid> oid = std::shared_ptr<Oid>(new Oid);
		sqlite3_stmt *stmt;

		int rc = sqlite3_prepare_v2(db, sql_select_by_id.c_str(), -1, &stmt, NULL);
		if (rc != SQLITE_OK){
			oid->setStatus(oid ->INIT_BAD);
			return oid;
		}
		rc = sqlite3_bind_int(stmt, 1, id);    // Using parameters ("?") is not
		if (rc != SQLITE_OK) {                 // really necessary, but recommended
			sqlite3_finalize(stmt);            // formatting problems and SQL
			oid->setStatus(oid ->INIT_BAD);
			return oid;            // injection attacks.
		}

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			sqlite3_finalize(stmt);
			oid->setStatus(oid ->INIT_BAD);
			return oid;
		}
		if (rc == SQLITE_DONE) {
			sqlite3_finalize(stmt);
		    oid->setStatus(oid ->INIT_BAD);
			return oid;
		}
	
		if(oid -> getStatus() == oid -> INIT_SUCCESS) 
		{
			oid = std::shared_ptr<Oid>(new Oid);
			oid -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			oid -> setOid( std::string((const char *)sqlite3_column_text(stmt, 1)));
			oid -> setTranslate( std::string((const char *)sqlite3_column_text(stmt, 2)));
			oid -> setActive( (atoi((const char *)sqlite3_column_text(stmt, 3)) == 0 ? false : true ));
		}

		sqlite3_finalize(stmt);
		return oid;
	}

	~OidORM() {
	}

};

#endif