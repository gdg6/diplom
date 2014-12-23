#ifndef __APP__
#define _APP__

#include <memory>
#include <thread>
#include <string>

#include "../serviceImpl/servicePack.h"

#define __SQLITE3__

#define COUNT_USER_FIELD 4
#define COUNT_REPORT_FIELD 3

class App {
private:

	int rc;
	ReportService reportService;
	DeviceService deviceService;
	UserService userService;

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
	
public:
	App(sqlite3 * db) : reportService(db), deviceService(db), userService(db), db(db)
	{
        initTables();
        
	}

	void insertReport(int device_id, std::string context)
	{
		
	}
		
};


#endif

int main()
{
}
