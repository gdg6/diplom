#ifndef __LOG__
#define __LOG__

#include <string>

class Log {
private: 
	int id;
	std::string type;
	std::string context;
public: 

	void setId(int id) {
		this -> id = id;
	}

	int getId() {
		return id;
	}

	void setType() {
		this ->type = type;
	} 

	std::string getType() {
		return type;
	}

	void setContext(std::string context) {
		this -> context = context;
	}

	std::string getContext() {
		return context;
	}
};

#endif