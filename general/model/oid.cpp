#ifndef __OID__
#define __OID__

#include <string>

class Oid
{
private: 
	int id;
	std::string oid;
	std::string translate;
	bool active; // can be send to device ? 
	int status;  // for init db 

public:
	enum {INIT_BAD, INIT_SUCCESS};
	
	Oid() {

	}

	void setId(int id) {
		this -> id = id;
	}

	int getId() {
		return id;
	}

	void setOid(std::string oid) {
		this -> oid = oid;
	}

	std::string getOid() {
		return oid;
	}

	void setTranslate(std::string translate) {
		this -> translate = translate;
	}

	std::string getTranslate() {
		return translate;
	}

	void setActive(bool active) {
		this -> active = active;
	}

	bool getActive() {
		return active;
	}

	void setStatus(int status) {
		this -> status = status;
	}

	int getStatus() {
		return status;
	}

	~Oid() {
	}

};


#endif