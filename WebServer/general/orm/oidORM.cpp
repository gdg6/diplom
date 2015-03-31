#ifndef __OID_ORM__
#define __OID_ORM__

#include "ORM.cpp"
#include "../model/oid.cpp"
#include <iostream>
#include <string>

#define ALL_ACTIVE 1
#define ALL 0

class OidORM : public ORM
{
private:
	std::string sql_select_by_id = "select Id, Device_id, Oid, Translate, Active FROM oids WHERE Id = ?";
	std::string sql_select_all_by_device_id = "SELECT Id, Device_id, Oid, Translate, Active FROM oids WHERE device_id = ?";
	std::string sql_select_all  = "SELECT * FROM oids;";
	std::string sql_insert = "INSERT INTO \'oids\' (\'device_id\', \'oid\', \'translate\', \'active\', \'created_at\', \'updated_at\') VALUES (?, ?, ?, ?, ?, ?)";

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
			//FIXME Не забыть сделать запись в таблицу логирования
			// std::cout <<  std::string (sqlite3_errmsg(db));
			// perror("");
			return list;
		}

		std::shared_ptr<Oid> oid;
		while((rc = sqlite3_step(stmt) !=  SQLITE_DONE)) { 
			oid = std::shared_ptr<Oid>(new Oid);
			oid -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			oid -> setDeviceId(atoi( (const char *)sqlite3_column_text(stmt, 1)));
			oid -> setOid(std::string((const char *)sqlite3_column_text(stmt, 2)));
			oid -> setTranslate(std::string((const char *)sqlite3_column_text(stmt, 3)));
			oid -> setActive((*(const char *)sqlite3_column_text(stmt, 4) == 't' ) ? true : false );
			list->push_back(oid);
		}

		sqlite3_finalize(stmt);

		return list;
	}
	

	std::shared_ptr<std::vector<std::shared_ptr<Oid>>> getListByDeviceId(int device_id, int must_be_active)
	{

		std::shared_ptr<std::vector<std::shared_ptr<Oid>>> list(new std::vector<std::shared_ptr<Oid>>());
		sqlite3_stmt *stmt;
		
		int rc = sqlite3_prepare_v2(db, sql_select_all_by_device_id.c_str(), sql_select_all_by_device_id.length(), &stmt, 0);
		if (rc != SQLITE_OK) {
			//FIXME Не забыть сделать запись в таблицу логирования
			// std::cout <<  std::string (sqlite3_errmsg(db));
			return list;
		}

		rc = sqlite3_bind_int(stmt, 1, device_id);
		if (rc != SQLITE_OK) {
			sqlite3_finalize(stmt);
			return list;           
		}

		std::shared_ptr<Oid> oid;
		while((rc = sqlite3_step(stmt) !=  SQLITE_DONE)) {
			oid = std::shared_ptr<Oid>(new Oid);
			oid -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			oid -> setDeviceId(atoi( (const char *)sqlite3_column_text(stmt, 1)));
			oid -> setOid(std::string((const char *)sqlite3_column_text(stmt, 2)));
			oid -> setTranslate(std::string((const char *)sqlite3_column_text(stmt, 3)));
			oid -> setActive((*(const char *)sqlite3_column_text(stmt, 4) == 't' ) ? true : false );
			if(must_be_active) {
				if(oid -> getActive()) {
					list -> push_back(oid);				
				}
			} else {
				list -> push_back(oid);				
			}
		}

		sqlite3_finalize(stmt);
		return list;
	}


	std::shared_ptr<std::vector<std::shared_ptr<Oid>>> getActiveOidsByDeviceId(int device_id)
	{
		return getListByDeviceId(device_id, ALL_ACTIVE);
	}


	int insertOid(Oid oid) 
	{

		sqlite3_stmt * stmt;
		std::string time  = currentDateTime();
		int rc = sqlite3_prepare_v2(db, sql_insert.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		// (\'device_id\', \'oid\', \'translate\', \'active\', \'created_at\', \'updated_at\')
		rc |= sqlite3_bind_int(stmt, 1, oid.getDeviceId()); 
		rc |= sqlite3_bind_text(stmt, 2, oid.getOid().c_str(), oid.getOid().length(), SQLITE_STATIC);    
		rc |= sqlite3_bind_text(stmt, 3, oid.getTranslate().c_str(), oid.getTranslate().length(), SQLITE_STATIC);    
		rc |= sqlite3_bind_text(stmt, 4, oid.getActive() ? "t" : "f", 1, SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 5, time.c_str(), time.length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 6, time.c_str(), time.length(), SQLITE_STATIC);

		if (rc != SQLITE_OK) 
		{                 
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return rc;
		}
 
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
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
			oid -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			oid -> setDeviceId( (atoi( (const char *)sqlite3_column_text(stmt, 1)) ));
			oid -> setOid( std::string((const char *)sqlite3_column_text(stmt, 2)));
			oid -> setTranslate( std::string((const char *)sqlite3_column_text(stmt, 3)));
			oid -> setActive( (*(const char *)sqlite3_column_text(stmt, 4) == 't' )? true : false );
		}

		sqlite3_finalize(stmt);
		return oid;
	}

	~OidORM() {
	}

};

#endif
