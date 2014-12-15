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
		
	void insertInto(char * table_name, char * column_names, char * data) {
		std::string sql = "INSERT INTO \'";
		std::string h1 = "\' (";
		std::string h2 = ") VALUES (";
		std::string h3 = ");";
		
		std::string t_name  = table_name;
		std::string c_names = column_names;
		std::string argv = data;
		
		sql += t_name + h1 + c_names + h2 + argv + h3;
		std::cout << sql << std::endl;  
	}

	void select(char * table_name, char * column_names, char * condition, char * order_by, bool sort) {
		std::string sql = "SELECT ";
		std::string columns = column_names == NULL ? "*" : column_names;
		std::string from = " FROM ";
		std::string t_name = table_name;
		std::string cond = (condition == NULL ? "" : condition);
		std::string orderBy = (order_by == NULL ? "" : order_by);
		std::string sort_desc = (order_by == NULL ? "" : (sort ? "ASC" : "DESC"));
		std::string end = ";";
		
		sql += columns + from + t_name + " " + cond + " " + orderBy + " " + sort_desc + " " + end;
		std::cout << sql;
	}
	
};



#endif

//~ SqlService * factory(int type, const char * nameDatabase) {
  //~ return new Sqlite3IO(nameDatabase);
//~ }
//~ 
//~ 
//~ int main()
//~ {
	//~ const char name_db [] = "snmp_db";
	//~ std::shared_ptr<SqlService> sql = std::shared_ptr<SqlService>(factory(1, name_db));
	//~ SqlSnmp s(sql); 
	//~ s.writeLog("hello");
	//~ s.insertInto("Transport", "ID, NAME", "1, 'vagon'");
	//~ s.select("tran", "name", "WHERE id == 1", "ORDER BY id", true);
	//~ return 0;
//~ }

