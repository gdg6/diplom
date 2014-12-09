//пример использования libSqlite3IO.cpp
#include "libSqlite3IO.cpp"
#include <string.h>

int callback( void * data, int argc, char ** argv, char ** azColName)
{
   int i = 0;
   std::cerr << ((char * ) data) << ":" ;
   while(i < argc )
  {
	  
	  std::cout << azColName[i] << " = " << argv[i] << std::endl;
	  if(strncpy(azColName[i], "PASSWORD", strlen(azColName[i]))) std::cout << "=-)" << std::endl;
	 //~ if(strcpy(azColName[i], "PASSWORD")) {
		//~ std::cout << "argv[i]   " << strlen(azColName[i]) << " " << strlen("PASSWORD") << argv[++i] << std::endl; 
		//~ std::cout << "argv[i]   " << strlen(azColName[i]) << " " << strlen("PASSWORD") << argv[++i] << std::endl; 
		//~ if(strcpy(argv[i], (char * ) data ))
							//~ return 1;
	  //~ 
	 //~ }
	i++;
  }
  return 0;
}

int main()
{
	int (* m_callback)(void *, int, char**, char**) =  &callback;
	const char * sql1 = "CREATE TABLE USER("  \
         "ID INT PRIMARY KEY  UNIQUE   NOT NULL ," \
         "NAME           TEXT    NOT NULL," \
         "PASSWORD       char(50)   NOT NULL);";
    ModelSqlite3 * m = new ModelSqlite3("snmp_db");
    m->sqlExec(sql1, "create table");
    const char * sql2 = "INSERT INTO USER (ID, NAME, PASSWORD) VALUES(2, 'admin', 123456)";
    m->sqlExec(sql2, "create user");
    const char * sql3 = "SELECT * FROM USER WHERE USER.NAME LIKE 'admin';";
    std::cout << "success : " << m->sqlExec(sql3, "test1") << std::endl;         
	std::cout << "######################" << std::endl;
	std::cout << "success : " << m->sqlExec(sql3, "123456", m_callback) << std::endl; 
	return 0;
}
