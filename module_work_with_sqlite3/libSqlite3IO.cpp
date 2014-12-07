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
   
    int sql_exec(const char * sql, char * description)
    {
      char * errMsg = 0;
      rc = sqlite3_exec(db, sql, callback, (void * ) description, & errMsg);
      if(rc != SQLITE_OK) std::cout << errMsg << std::endl;
      return rc;
    }  


public:  

    ModelSqlite3(const char * name_db)
    {
	rc = sqlite3_open(name_db, & db);
    }

    int sqlExec(const char * sql, char * description)
    {
      return sql_exec(sql, description);
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


#include <stdio.h>
#include <string.h>
int main()
{
   const char * name_db = "snmp_db";
   ModelSqlite3 *  m = new ModelSqlite3(name_db);
   std::cout << m -> getStatus();
   char sql[255];   
 
   scanf("%s", & sql);
   sql[strlen(sql)] = '\0';
   m -> sqlExec(sql, "test");
   m -> getStatus();
   delete m;
   return 0;
}

