#ifndef __MIB__
#define __MIB__

#include <string>

class Mib {
private: 
	int id;
	std::stirng name; // for rails
	std::string path; // for deamon
public: 

	void setId(int id) {
		this -> id = id;
	}

	int getId() {
		return id;
	}

	void setName(std::string name) {
		this -> name = name;
	}

	std::string getName() {
		return name;
	}

	void setPath(std::string path) {
		this -> path = path;
	}

	std::string getPath() {
		return path;
	}
};

#endif