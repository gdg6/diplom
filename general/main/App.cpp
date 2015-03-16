#ifndef __APP__
#define __APP__

#include <memory>
#include <thread>
#include <string>
#include <thread>
#include "../serviceImpl/servicePack.h"
#include "../utils/schema.cpp"
#include "asyncSnmpManager.cpp"
#include "serverThreadPool.cpp"

class App {
private:


	sqlite3 * db;

	int rc;
	ReportService reportService;
	DeviceService deviceService;
	UserService userService;

	
	std::shared_ptr<AsyncSnmpManager> asyncSnmpManager;


	
	void initTables()
	{
		delete (new Schema(db));
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
		
	
public:
	App(sqlite3 * db) : db(db), reportService(db), deviceService(db), userService(db)
	{
        initTables();
        if(initManager() != 0)
        {
			//~ std::cerr << "Can't be run SNMP manager!!!" << std::endl;
			exit(1);
		}
	}

	void Run()
	{
		pthread_t t1;
		pthread_t t2;

		pthread_create(&t1, NULL, starterThreadPoolServer, (void*)db);
		pthread_create(&t2, NULL, starterAsyncSnmpManager, (void*)db);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		
	}
	~App()
	{
		sqlite3_close(db);
	}
		
};

#endif
