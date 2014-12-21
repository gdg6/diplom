#ifndef __DEVICE_ORM__
#define __DECICE_ORM__

#include "ORM.cpp"
#include "../model/device.cpp"
#include <memory>
#include <thread>
#include <string>
#include <string.h> // for strcpy
#include <iostream>

#define __SQLITE3__

class DeviceORM  : public ORM
{
	//~ std::string sql_select_by_id = "SELECT id, name, description, mac, serial_number, model, peername, port, login, password, mid_id FROM devices WHERE Id = ?";
	std::string sql_select_by_id = "SELECT Name FROM devices WHERE Id = ?";
	int rc; //status for db;
	
#ifdef __SQLITE3__
	  sqlite3 * db;
#else

#endif

	//for output,  maybe in future deleted
    static int callback( void * data, int argc, char ** argv, char ** azColName)
    {
       int i = 0;
       std::cerr << ((const char * ) data) << ":" ;
       while(i < argc )
      {
         std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
         i++;
      }
      std::cout << std::endl;
      return 0;
    }
   
    int sql_exec(const char * sql, char * description, int (*callback)(void *, int, char **, char **))
    {
      char * errMsg = 0;
      rc = sqlite3_exec(db, sql, callback, (void * ) description, & errMsg);
      if(rc != SQLITE_OK) std::cout << errMsg << std::endl;
      return rc;
    }  
	
public:
	DeviceORM(const char * name_db)
	{
#ifdef __SQLITE3__
		rc = sqlite3_open(name_db, & db);
#else

#endif
	}
	
	std::shared_ptr<std::list<Device>> getAll()
	{
		std::shared_ptr<std::list<Device>> list = std::shared_ptr<std::list<Device>>(new std::list<Device>());
		return list;
	}
	
	
	std::shared_ptr<Device> getByPK(int id)
	{

		std::shared_ptr<Device> device = std::shared_ptr<Device>(new Device);
#ifdef __SQLITE3__		
		sqlite3_stmt *stmt;

		int rc = sqlite3_prepare_v2(db, sql_select_by_id.c_str(), -1, &stmt, NULL);
		if (rc != SQLITE_OK)
			throw std::string(sqlite3_errmsg(db));

		rc = sqlite3_bind_int(stmt, 1, id);    // Using parameters ("?") is not
		if (rc != SQLITE_OK) {                 // really necessary, but recommended
			std::string errmsg(sqlite3_errmsg(db)); // (especially for strings) to avoid
			sqlite3_finalize(stmt);            // formatting problems and SQL
			throw errmsg;                      // injection attacks.
		}

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			std::string errmsg(sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			throw errmsg;
		}
		if (rc == SQLITE_DONE) {
			sqlite3_finalize(stmt);
			throw std::string("customer not found");
		}
		
		device -> setId(id);
		device -> setName( std::string((const char *)sqlite3_column_text(stmt, 0)));
		

		sqlite3_finalize(stmt);
#else

#endif
		return device;
	}
};

#endif

int main()
{
	DeviceORM d("../../snmp_db");
	std::shared_ptr<Device> device = d.getByPK(1);
	std::cout << device->getName();
	return 0;
}
