// Обертка для работы с SQLITE3
// Разработан для дипломной работы  


#include <sqlite3.h>
#include <iostream>
#include <string>


class ModelSqlite3 {
private:
    sqlite3 * db;
    int rc;

    static int callback( void * data, int argc, char ** argv, char ** azColName)
    {
       int i = 0;
       std::cerr << ((const char * ) data) << ":" ;
       while(i < argc )
      {
         std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
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
    ModelSqlite3(const char * name_db)
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

   ~ModelSqlite3()
   {
     if (db) sqlite3_close(db);
   }

};


