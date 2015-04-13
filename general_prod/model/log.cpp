#ifndef __LOG__
#define __LOG__

#include <string>

class Log {
private: 
	int id;
	std::string type;
	std::string context;
public: 

	const std::string ERROR = "ERROR"; 
	const std::string WARNING = "WARNING";
	const std::string EVENT = "EVENT";

	void setId(int id) {
		this -> id = id;
	}

	int getId() {
		return id;
	}

	void setType(std::string type) {
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
