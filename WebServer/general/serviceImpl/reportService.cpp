#ifndef __REPORT_SERVICE__
#define __REPORT_SERVICE__

#include "../orm/reportORM.cpp"

class ReportService
{
private:
	ReportORM reportORM;
	
public:	
	ReportService(sqlite3 * db) : reportORM(db)
	{
		
	}
	
	int insertReport(int device_id, std::string type, std::string context)
	{
		return reportORM.insertReport(device_id, type, context);
	}
	
	//sql must be checked before execute
	int sqlExec(std::string & sql)
	{
		return reportORM.sqlExec(sql);
	}
};

#endif
