#ifndef __ORM__
#define __ORM__

#include <vector>
#include <sqlite3.h>
#include <memory>
#include <thread>
#include <string>

#define __SQLITE3__

//Создание технологии ORM для работы с sqlite3 and PostgressSQL

class ORM {
public:

    //~ virtual int sqlExec(const char * sql, char * description) = 0;
    //~ virtual int sqlExec(const char * sql, char * description, int (*callback)(void*, int, char **, char **)) = 0;
    //~ virtual int getStatus() = 0;
    
	template <typename T>
	std::shared_ptr<std::vector<std::shared_ptr<T>>> getAll(){};
	
	template <typename T>
	std::shared_ptr<T> getByPK(int id){};
	
	//~ virtual void deleteByPK(int id) = 0;
};

#endif

