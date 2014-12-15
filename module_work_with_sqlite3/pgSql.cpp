#ifndef __PGSQL
#define __PGSQL

#include "sqlService.cpp"

class PgSql : public SqlService {
public: 
	int sqlExec(const char * sql, char * description) {
	  return 0;
	}
    
    int sqlExec(const char * sql, char * description, int (*callback)(void*, int, char **, char **)) {
	  return 0;
	}
    int getStatus(){
	  return 0;
	}
};

#endif
