#ifndef __SQL_SNMP
#define __SQL_SNMP

#include "sqlite3IO.cpp"
#include "pgSql.cpp"
#include <memory>
#include <thread>
#include <string>

//helpers
char SQL[] = "EXEC SQL";
char INSERT[] =  "INSERT";
char SELECT[] = "SELECT";
char CREATE_TABLE[] = "CREATE_TABLE";

class SqlSnmp {
   std::shared_ptr<SqlService> sqlService;
public: 
	SqlSnmp(std::shared_ptr<SqlService> sqlService) {
		this->sqlService = sqlService;
	}
		
	void insertInto(char * table_name, char * column_names, char * data, int (*callback)(void*, int, char **, char **)) {
		std::string sql = "INSERT INTO \'";
		std::string h1 = "\' (";
		std::string h2 = ") VALUES (";
		std::string h3 = ");";
		
		std::string t_name  = table_name;
		std::string c_names = column_names;
		std::string argv = data;
		
		sql += t_name + h1 + c_names + h2 + argv + h3;
		std::cout << sql << std::endl;  
		sqlService ->sqlExec(sql.c_str(), INSERT);
		if(!callback) {
		    sqlService ->sqlExec(sql.c_str(), INSERT);
		} else {
			sqlService ->sqlExec(sql.c_str(), INSERT, callback);
		}
	}

	void select(char * table_name, char * column_names, char * condition, char * order_by, bool sort, int (*callback)(void*, int, char **, char **)) {
		std::string sql = "SELECT ";
		std::string columns = column_names == NULL ? "*" : column_names;
		std::string from = " FROM ";
		std::string t_name = table_name;
		std::string cond = (condition == NULL ? "" : condition);
		std::string orderBy = (order_by == NULL ? "" : order_by);
		std::string sort_desc = (order_by == NULL ? "" : (sort ? "ASC" : "DESC"));
		std::string end = ";";
		
		sql += columns + from + t_name + " " + cond + " " + orderBy + " " + sort_desc + " " + end;
		std::cout << sql << std::endl;
		char * description = SELECT;
		if(!callback) {
			sqlService->sqlExec(sql.c_str(), description);
		} else {
			sqlService->sqlExec(sql.c_str(), description, callback);
		}
	}
	
	void createTable(char * table_name, char *fields[], int size) {
		std::string sql = "CREATE TABLE ";
		sql += table_name; sql += " (";
		int i = 0;
		for(;i < size - 1; i++) {
			sql += fields[i];
			sql += ", "; 
		}
		sql += fields[i];
		sql += ");";
		std::cout << sql.c_str() << std::endl;
		sqlService ->sqlExec(sql.c_str(), CREATE_TABLE);		
	}
	
	void sqlExec(std::string sql) 
	{
		sqlService -> sqlExec(sql.c_str(), SQL);
	}
	
	void sqlExec(std::string sql, int (*callback)(void*, int, char **, char **)) 
	{
		sqlService -> sqlExec(sql.c_str(), SQL);
	}
};



#endif

//~ SqlService * factory(int type, const char * nameDatabase) {
  //~ return new Sqlite3IO(nameDatabase);
//~ }


//~ int main()
//~ {
	//~ const char name_db [] = "snmp_db";
	//~ std::shared_ptr<SqlService> sql = std::shared_ptr<SqlService>(factory(1, name_db));
	//~ SqlSnmp s(sql); 
	//~ s.writeLog("hello");
	//~ s.insertInto("Transport", "ID, NAME", "1, 'vagon'");
	//~ s.select("tran", "name", "WHERE id == 1", "ORDER BY id", true);
	//~ char *t[] ={"ID INTEGER PRIMARY KEY AUTOINCREMENT","NAME text not null", "PASSWORD text"};
	//~ s.createTable("USER", t, 3);
	//~ s.insertInto("USER", "NAME, PASSWORD", "'DENIS', '123456'", NULL);
	//~ s.select("USER", NULL, NULL, NULL, false, NULL);
	//~ return 0;
//~ }

