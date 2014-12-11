// Обертка для работы с SQLITE3
// Разработан для дипломной работы  


#include <sqlite3.h>
#include <iostream>


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


class SnmpSqlite{
  SnmpSqlite * snmp_sqlite;
public:
  



};

int callback( void * data, int argc, char ** argv, char ** azColName)
{
    int i = 0;
    std::cerr << ((const char * ) data) << ":" ;
    while(i < argc )
   {
       std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << " ";
       i++;
    }
    std::cout << std::endl;
    return 0;
}

#include <stdio.h>
#include <string.h>
int main()
{
   const char * name_db = "snmp_db";
   int (*my_callback)(void *, int, char **, char **) = & callback;
   ModelSqlite3 *  m = new ModelSqlite3(name_db);
   std::cout << m -> getStatus() << std::endl;
   char * sql = "create table company (\
                `ID` INTEGER PRIMARY KEY AUTOINCREMENT , \
                `name` text not null);";
   m -> sqlExec(sql, "test");
   std:: cout << "status:" << m -> getStatus() << std::endl;
   sql = "INSERT INTO COMPANY ( NAME) VALUES ('Paul'), ( 'KRISTINA');";
   m->sqlExec(sql, "test2");
   std::cout << "status: " << m->getStatus() << std::endl;
   sql = "select * from company";
   m->sqlExec(sql, "test3", my_callback);
   delete m;
   return 0;
}

