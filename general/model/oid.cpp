#ifndef __OID__
#define __OID__

#include <string>

class Oid
{
private: 
	std::string oid;
	std::string synonym;
	bool active; // can be send to device ? 

public:
	Oid() {

	}

	void setOid(std::string oid) {
		this -> oid = oid;
	}

	std::string getOid() {
		return oid;
	}

	vod setSynonym(std::string synonym) {
		this -> synonym = synonym;
	}

	std::string getSynonym() {
		return synonym;
	}

	void setActive(bool active) {
		this -> active -> active;
	}

	bool getActive() {
		return active;
	}

	~Oid() {
	}

};


#endif