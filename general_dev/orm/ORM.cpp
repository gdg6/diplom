#ifndef __ORM__
#define __ORM__

#include <vector>
#include <sqlite3.h>
#include <memory>
#include <thread>
#include <string>

#define __SQLITE3__

//Создание технологии ORM для работы с sqlite3

class ORM {
public:
	enum{BAD_INIT, SUCCESS_INIT, NOT_FOUND, BAD_SOCKET};
    
	template <typename T>
	std::shared_ptr<std::vector<std::shared_ptr<T>>> getAll(){};
	
	template <typename T>
	std::shared_ptr<T> getByPK(int id){};
	
	static const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		return buf;
	}

};

#endif

