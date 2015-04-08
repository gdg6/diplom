#ifndef __DEVICE__
#define __DEVICE__

#include <string>
#include <memory>
#include <thread>
#include "oid.cpp"

class Device {
private:
	int id;
	std::string name;
	std::string description;
	std::string room;
	std::string mac;
	std::string serial_number;
	std::string model;
	std::string peername;
	int port_number;
		
	//authorization
	std::string login;
	std::string password;
	std::string priv_password;
	
	//helper for deamon
	int status;

	int i; // iterator for command as at(i) 

public: 
	enum {INIT_BAD, INIT_SUCCESS};
	
	Device() : status(INIT_SUCCESS)
	{
	}

	int getId()
	{
		return id;
	}

	void setId(int id)
	{
		this->id = id;
	}
	
	std::string getName()
	{
		return name;
	}
	
	void setName(std::string name)
	{
		this-> name = name;
	}
	
	std::string getDescription()
	{
		return description;
	}
	
	void setDescrition(std::string description)
	{
		this -> description = description;
	}
		
	std::string getRoom()
	{
		return room;
	}
	
	void setRoom(std::string room)
	{
		this -> room = room;
	}
		
	std::string getMac()
	{
		return mac;
	}
	
	void setMac(std::string mac)
	{
		this -> mac = mac; 
	}
	
	std::string getSerialNumber()
	{
		return serial_number;
	}
	
	void setSerialNumber(std::string serial_number)
	{
		this -> serial_number = serial_number;
	}
			
	std::string getModel()
	{
		return model;
	}

	void setModel(std::string model)
	{
		this->model = model;
	}
		
	std::string getPeername()
	{
		return peername;
	}
	
	void setPeername(std::string peername)
	{
		this->peername = peername;
	}

	int getPortNumber()
	{
		return port_number;
	}
	
	void setPortNumber(int port_number)
	{
		this -> port_number = port_number;
	}	

	std::string getLogin()
	{
		return login;
	}

	void setLogin(std::string login)
	{
		this -> login = login;
	}
		
	std::string getPassword()
	{
		return password;
	}

	void setPassword(std::string password) 
	{
		this -> password = password;
	}
	
	void setPrivPassword(std::string priv_password) 
	{
		this -> priv_password = priv_password;
	}

	std::string getPrivPassword()
	{
		return priv_password;
	}

	int getStatus()
	{
		return status;
	}
	
	void setStatus(int status)
	{
		this -> status = status;
	}

};

#endif

