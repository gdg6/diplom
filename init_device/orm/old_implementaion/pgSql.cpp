#ifndef __PGSQL
#define __PGSQL

#include "sqlService.cpp"

class PgSql : public SqlService {
public: 

	PgSql(char * db_name) {
	}
	
	int sqlExec(const char * sql, char * description) {
	  return 0;
	}
    
    int sqlExec(const char * sql, char * description, int (*callback)(void*, int, char **, char **)) {
	  return 0;
	}

    int getStatus(){
	  return 0;
	}

	// в базовом классе необходимо пименить стратегию что бы возращять указатель на разные типа дб
	sqlite3 * getDb()
	{
		return NULL;
	}
};

#endif
