#ifndef __DEVIE_SERVICE__
#define __DEVIE_SERVICE__


#include ".,/orm/sqlSnmp.cpp"
#include "sqlite3IO.cpp"
#include "pgSql.cpp"
#include "snmpSessionDevice.cpp"
#include <memory>
#include <thread>
#include <string>

#include "device.cpp"

class DeviceService 
{

private:
	std::string sql_select_by_id = "SELECT * FROM devices WHERE id = ";

	std::shared_ptr<Device> device;
	std::shared_ptr<SqlSnmp> sqlService;

    int callback( void * data, int argc, char ** argv, char ** azColName)
	{
		for(int i = 0; i < argc; i++)
		{
			 if(strcmp(azColName[i], "id")) device -> setId(atoi(argv[i]));
			 if(strcmp(azColName[i], "login")) device -> setLogin(std::string(argv[i]));
			 if(strcmp(azColName[i], "password")) device -> setPassword(std::string(argv[i]));
			 if(strcmp(azColName[i], "peername")) device -> setPeername(std::string(argv[i]));	
		}
		return 0;
	}

    int sql_exec(const char * sql, char * description, int (*callback)(void *, int, char **, char **))
    {
      char * errMsg = 0;
      int rc = sqlite3_exec(sqlService->getDb(), sql, callback, (void * ) description, & errMsg);
      if(rc != SQLITE_OK) std::cout << errMsg << std::endl;
      return rc;
    }  

public:
	DeviceService()
	{
		
	}

	DeviceService(std::shared_ptr<SqlSnmp> sqlService)
	{
		this -> sqlService = sqlService;
	}

	std::shared_ptr<Device>  getByPk(int id) 
	{
		device = std::shared_ptr<Device>(new Device);
		sql_select_by_id += std::to_string(id) + ";";
		char  c[] = "qqq";
		// sqlService->sqlExec(sql_select_by_id.c_str(), callback);
		// sqlService -> sqlExec(sql_select_by_id, callback);
		sql_exec(sql_select_by_id.c_str(), "qqq", this->callback);
		return device;
	}
};


#endif


int callback( void * data, int argc, char ** argv, char ** azColName)
{
   int i = 0;
   std::cerr << ((const char * ) data) << ":" << std::endl;
   while(i < argc )
   {
     std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
     i++;
   }
   std::cout << std::endl;
   return 0;
}

#define __SQLITE3__

int main()
{
	char db_name []  = "../snmp_db";
#ifdef __SQLITE3__
	std::shared_ptr<SqlService> service = std::shared_ptr<SqlService>(new Sqlite3IO(db_name));
#else 
	std::shared_ptr<SqlService> service = std::shared_ptr<SqlService>(new PgSql(db_name));
#endif
	std::shared_ptr<SqlSnmp> sqlService = std::shared_ptr<SqlSnmp>(new SqlSnmp(service));
	std::cout << "initDB: ok" << std::endl;

	DeviceService * deviceService = new DeviceService(sqlService);
	std::shared_ptr<Device>  device = deviceService -> getByPk(1);
	
	//=== print Device parametres
	std::cout << device -> getLogin() << std::endl;
	std::cout << device -> getPassword() << std::endl;
	std::cout << device -> getPassword() << std::endl;
}
