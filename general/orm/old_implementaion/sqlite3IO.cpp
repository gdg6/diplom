// Обертка для работы с SQLITE3
// Разработан для дипломной работы  

#ifndef __SQLITE3
#define __SQLITE3

#include <iostream>
#include "sqlService.cpp"

class Sqlite3IO  : public  SqlService {
private:
    sqlite3 * db;
    int rc;

    static int callback( void * data, int argc, char ** argv, char ** azColName)
    {
       int i = 0;
       std::cerr << ((const char * ) data) << ":" ;
       while(i < argc )
      {
         std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
         i++;
      }
      std::cout << std::endl;
      return 0;
    }
   
    int sql_exec(const char * sql, char * description, int (*callback)(void *, int, char **, char **))
    {
      char * errMsg = 0;
      rc = sqlite3_exec(db, sql, callback, (void * ) description, & errMsg);
      if(rc != SQLITE_OK) std::cout << errMsg << std::endl;
      return rc;
    }  

public:  
    //необходимо будет сделать обработку ошибки и желательно иметь разный доступ к бд. 
    // см мануал
    Sqlite3IO(const char * name_db)
    {
		  rc = sqlite3_open(name_db, & db);
    }

    int sqlExec(const char * sql, char * description)
    {
      return sql_exec(sql, description, this->callback);
    }    

    int sqlExec(const char * sql, char * description, int (*callback)(void*, int, char **, char **))
    {
	    return sql_exec(sql, description, callback);
    }

    int getStatus()
    {
      return rc;
    }

    sqlite3 * getDb()
    {
      return db;
    }

   ~Sqlite3IO()
   {
     if (db) sqlite3_close(db);
   }


};


#endif

int main()
{
	Sqlite3IO sqlite3IO("../../snmp_db");
	char sql [] = "select * from devices;";
	sqlite3IO.sqlExec(sql, "ww");
}
