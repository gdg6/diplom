#ifndef __APP__
#define _APP__

#include <memory>
#include <thread>
#include <string>

#include "../serviceImpl/servicePack.h"
#include "asyncSnmpManager.cpp"

#define __SQLITE3__


class App {
private:

	int rc;
	ReportService reportService;
	DeviceService deviceService;
	UserService userService;
	
	std::shared_ptr<AsyncSnmpManager> asyncSnmpManager;

#ifdef __SQLITE3__
	   sqlite3 * db;
#else 

#endif
	
	void initTables()
	{
		//~ //create users table
		//~ sqlService -> sqlExec(sql_user_create_table);
		//~ 
		//~ //create devices table
		//~ sqlService -> sqlExec(sql_devices_create_table); 
		//~ 
		//~ //create reports table
		//~ sqlService -> sqlExec(sql_reports_create_table);
		//~ 
		//~ //create mibs table
		//~ sqlService -> sqlExec(sql_mib_create_table);
		//~ 
		//~ std::cout << "initTalbe: ok" << std::endl;
	}
	
	int initManager()
	{
		int r = 0;
		try
		{
			asyncSnmpManager = std::shared_ptr<AsyncSnmpManager>(new AsyncSnmpManager(db));
		}
		catch (std::bad_alloc& ba)
		{
			r = -1;
		}
		
		return r;
	}
	
	int runManager()
	{
		
		return 0;
	}
	
	int initServer()
	{
		return 0;
	}
	
public:
	App(sqlite3 * db) : reportService(db), deviceService(db), userService(db), db(db)
	{
        initTables();
        if(initManager() != 0)
        {
			std::cerr << "Can't be run SNMP manager!!!" << std::endl;
			exit(1);
		} 
	}

	void Run()
	{
		asyncSnmpManager -> Run();
		
	}
	~App()
	{
		sqlite3_close(db);
	}
		
};


#endif

int main(int argc, char *argv[])
{
	sqlite3 * db;
	sqlite3_open("../snmp_db", &db);
	App app(db);
	app.Run();
	
	return 0;
}