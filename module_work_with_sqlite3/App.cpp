
#include "sqlSnmp.cpp"
#include "sqlite3IO.cpp"
#include "pgSql.cpp"
#include <memory>
#include <thread>
#include <string>

#define __SQLITE3__

#define COUNT_USER_FIELD 4
#define COUNT_REPORT_FIELD 3

class App {
private:

	const char * db_name = "snmp_db";
// ============== USERS ===============
    std::string sql_user_create_table = "CREATE TABLE \'users\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'login\' varchar(255), \'password_digest\' varchar(255), \'role\' integer, \'created_at\' datetime, \'updated_a\' datetime);";

// ============== DEVICES ============
    std::string sql_devices_create_table = "CREATE TABLE \'devices\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'name\' varchar(255), \'description\' varchar(1024), \'room\' varchar(25), \'mac\' varchar(255), \'serial_number\' varchar(255), \'model\' varchar(1024), \'port\' integer, \'mib_id\' integer, \'created_at\' datetime, \'updated_at\' datetime);";


// ============== REPORTS ============
	std::string sql_reports_create_table = "CREATE TABLE \'reports\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'device_id\' integer, \'context\' text, \'created_at\' datetime, \'updated_at\' datetime);";

// ============== MIBS ===============
    std::string sql_mib_create_table = "CREATE TABLE \'mibs\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'file_name_mib\' varchar(255), \'created_at\' datetime, \'updated_at\' datetime);";
    

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
	
	void initTables()
	{
		//create users table
		sqlService -> sqlExec(sql_user_create_table);
		
		//create devices table
		sqlService -> sqlExec(sql_devices_create_table); 
		
		//create reports table
		sqlService -> sqlExec(sql_reports_create_table);
		
		//create mibs table
		sqlService -> sqlExec(sql_mib_create_table);
		
		std::cout << "initTalbe: ok" << std::endl;
	}
	
	
public:
	App(){
		initDB();
        initTables();
	}
	
	void AuthorizationUser()
	{
		std::cout << "Enter name";
		
	}
	
	void insertReport(int device_id, std::string context)
	{
		if(device_id < 0) {
			std::cerr << "Err: device_id < 0; SQL FAIL!" << std::endl;
		}
		std::string sql = "INSERT INTO \'reports\'  (\'device_id\', \'context\') VALUES (" + std::to_string(device_id) + ", \'" + context + "\');" ;
		sqlService -> sqlExec(sql.c_str()); 
	}
	
};


int main()
{
    App p;

	return 0;
}
