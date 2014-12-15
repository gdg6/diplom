
#include "sqlSnmp.cpp"
#include "sqlite3IO.cpp"
#include "pgSql.cpp"
#include <memory>
#include <thread>
#include <string>

#define __SQLITE3__

class App {
private:
	const char * db_name = "snmp_db";
	const char * user_table = "user";
	std::shared_ptr<SqlSnmp> sqlService; 
	
	void initDB()
	{
#ifdef __SQLITE3__
	std::shared_ptr<SqlService> service = std::shared_ptr<SqlService>(new Sqlite3IO(db_name));
#else 
	std::shared_ptr<SqlService> service = std::shared_ptr<SqlService>(new PgSql(db_name));
#endif
		sqlService = std::shared_ptr<SqlSnmp>(new SqlSnmp(service));
		std::cout << "initDB: ok" << std::endl;
	}
	
	void initTable()
	{
		
	}
public:
	App(){
		initDB();
        initTalbes();
		
	}
};


int main()
{
	App app;
	return 0;
}
