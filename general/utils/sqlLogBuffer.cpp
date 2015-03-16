#ifndef __SQL_LOG_BUFFER__
#define __SQL_LOG_BUFFER__

#include "SqlBuffer.cpp"
#include <sqlite3.h>
#include <mutex>

class SqlLogBuffer {
private: 
	std::string currentSql;
	SqlBuffer sqlBuffer;
	std::mutex mt;


	const char  * valid_chars  = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM:1234567890- ";
	const std::set<char> set_pattern ;

	bool ValidateContext(std::string context)
	{

		if(context.length() > 255) {
			return false;
		}

		std::set<char>::iterator it;		
		
		for(int i = 0; i < context.length(); i++) {
			it = set_pattern.find(context[i]);
			if(it == set_pattern.end()) {
				return false;
			}
		}
		
		return true;
	}

	static const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		return buf;
	}

public:
	SqlLogBuffer(): set_pattern (valid_chars,  valid_chars + sizeof(valid_chars) - 1)
	{

	}

	void addInsert(int  id, std::string  type, std::string  context)
	{
		std::unique_lock<std::mutex> lock(mt);
		currentSql = "";
		if(id <=  0) { 
			return;
		}

		if(!type.length()) {
			return;
		}
		
		if(!context.length() || context.length() > 1024) {
			return;
		}
		
		if(! ValidateContext(context) ) {
			return;
		}
		
		if(!sqlBuffer.getCountSql()) {
			currentSql = "INSERT INTO logs (\'type\', \'context\', \'created_at\', \'updated_at\') VALUES ";
			currentSql += "(";
		} else {
			currentSql += ", (";
		}
		
		std::string c_t = currentDateTime();
		currentSql += "\'" + type + "\', \'" + context + "\', \'" + c_t + "\', \'" + c_t +"\' )";
		
		sqlBuffer.addSql(currentSql);
	}

	std::string  & getSql()
	{
		std::unique_lock<std::mutex> lock(mt);
		return sqlBuffer.getSql();
	}
	
	int getCountSql()
	{
		std::unique_lock<std::mutex> lock(mt);
		return sqlBuffer.getCountSql();
	}
	
	std::string & pushSql()
	{
		std::unique_lock<std::mutex> lock(mt);
		return sqlBuffer.pushSql();
	}
	
	bool isMustBePush()
	{
		std::unique_lock<std::mutex> lock(mt);
		return sqlBuffer.maxSqlLength();
	}
	
	int getLengthBuffer()
	{
		std::unique_lock<std::mutex> lock(mt);
		return sqlBuffer.getLengthBuffer();
	}

};

#endif