#ifndef __OID__
#define __OID__

#include <string>

class Oid
{
private: 
	int id;
	std::string oid;
	std::string translate;
	int ping_request;
	int device_id;
	bool active; // can be send to device ? 
	time_t last_time_request;
	int status;  // for init db 

public:
	enum {INIT_BAD, INIT_SUCCESS};
	
	Oid() : last_time_request(0), status(INIT_SUCCESS) 
	{

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

	void setPingRequest(int ping_request)
	{
		this -> ping_request = ping_request;
	}

	int getPingRequest()
	{
		return ping_request;
	}

	void setDeviceId(int device_id){
		this -> device_id = device_id;
	}

	int getDeviceId() 
	{
		return device_id;
	}

	void setActive(bool active) {
		this -> active = active;
	}

	bool getActive() {
		return active;
	}

	void setLastTimeRequest(time_t last_time_request)
	{
		this -> last_time_request;
	}

	time_t getLastTimeRequest()
	{
		return last_time_request;
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