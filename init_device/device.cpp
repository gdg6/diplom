#ifndef __DEVICE__
#define __DEVICE__

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
	// mib
	int mib_id;
	
	//helper for deamon
	int status;

public: 
	Device() 
	{
	}

	int getId()
	{
		return id;
	}

	void setid(int id)
	{
		this->id = id;
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


