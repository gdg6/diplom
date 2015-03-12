#ifndef __SQL_BUFFER__
#define __SQL_BUFFER__

#include <string>
#define MAX_SQL_LENGTH 31000

class SqlBuffer {
private:
	std::string buffer;
	int count_sql;
	std::string rez;

public: 

	SqlBuffer() : buffer(""), count_sql(0)
	{
	}

	//Fixme - sql must be validate
	void addSql(std::string sql) {
		buffer += sql; 
		count_sql++;
	}
	
	std::string & getSql()
	{
		buffer+=";";
		return buffer;
	}
	
	std::string & pushSql()
	{
		rez = buffer;
		this->bufferClear();
		return rez;
	}
	
	int getLengthBuffer()
	{
		return buffer.length();
	}
	
	void bufferClear()
	{
		buffer.clear();
		count_sql = 0;
		buffer = "";
	}
	
	int getCountSql()
	{
		return count_sql;
	}
	
	bool maxSqlLength()
	{
		return buffer.length() < MAX_SQL_LENGTH ? false : true;
	}
	
	~SqlBuffer()
	{
		buffer.clear();
	}
};

#endif
