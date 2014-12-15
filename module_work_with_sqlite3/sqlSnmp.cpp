#ifndef __SQL_SNMP
#define __SQL_SNMP

#include "sqlite3IO.cpp"
#include "pgSql.cpp"
#include <memory>
#include <thread>
#include <string>



class SqlSnmp {
   std::shared_ptr<SqlService> sqlService;
public: 
	SqlSnmp(std::shared_ptr<SqlService> sqlService) {
		this->sqlService = sqlService;
	}
	
	void writeLog(char * oid, char * command) {
		
	}
	
	void writeLog(char * data) {
		std::string sql  = "INSERT INTO \'";
		std::string h1 = "\' VALUES (\'";
		std::string h2 = "');";
		std::string h3 = data;
	    const char nameTable []  = "Report";
		std::string s(nameTable);
		sql+=s + h1 + h3  + h2;;
		std::cout << sql.c_str();
	}
	
};



#endif

SqlService * factory(int type, const char * nameDatabase) {
  return new Sqlite3IO(nameDatabase);
}


int main()
{
	const char name_db [] = "snmp_db";
	std::shared_ptr<SqlService> sql = std::shared_ptr<SqlService>(factory(1, name_db));
	SqlSnmp s(sql); 
	s.writeLog("hello");
	return 0;
}

