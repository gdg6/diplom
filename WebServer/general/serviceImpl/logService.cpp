#ifndef __LOG_SERVICE__
#define __LOG_SERVICE__

#include "../orm/logORM.cpp"
#include <memory>
#include <thread>
#include <string>

class LogService {
private:
	LogORM logORM;
	
public:

	LogService(sqlite3 * db) : logORM(db)
	{

	}

	int save(Log log)
	{
		return logORM.insertLog(log.getType(), log.getContext());
	}

	int save(std::string type, std::string context)
	{
		return logORM.insertLog(type, context);
	}

	~LogService()
	{

	}
};

#endif