#ifndef __SQL
#define __SQL


class SqlService {
public:
    virtual int sqlExec(const char * sql, char * description) = 0;
    virtual int sqlExec(const char * sql, char * description, int (*callback)(void*, int, char **, char **)) = 0;
    virtual int getStatus() = 0;
    //~ virtual	void sqlExec(std::string sql) = 0;
	//~ virtual void sqlExec(std::string sql, int (*callback)(void*, int, char **, char **)) = 0 ;
};

#endif // end __SQL

