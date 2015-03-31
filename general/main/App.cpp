#ifndef __APP__
#define __APP__

#include <memory>
#include <thread>
#include <string>
#include <thread>
#include "../serviceImpl/servicePack.h"
#include "../lib/liblog.h"
#include "../utils/schema.cpp"
#include "asyncSnmpManager.cpp"
#include "serverThreadPool.cpp"

#define APP_OK 0
#define BAD_ALLOC -1

class App {
private:


	sqlite3 * db;
	std::shared_ptr<LogService> logService;
	std::shared_ptr<AsyncSnmpManager> asyncSnmpManager;

	
	void initTables()
	{
		delete (new Schema(db));
	}

	
	int initManager()
	{
		try
		{
			asyncSnmpManager = std::shared_ptr<AsyncSnmpManager>(new AsyncSnmpManager(db, logService));
		}
		catch (std::bad_alloc& ba)
		{
			return BAD_ALLOC;
		}		
		return APP_OK;
	}
		
	
public:


	App(sqlite3 * db) : db(db)
	{
		logService = std::shared_ptr<LogService>(new LogService(db));
        initTables();
        if(initManager() != 0)
        {
			logService -> save(BAD_ALLOC_LOG, "can't malloc snmp manager");
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
