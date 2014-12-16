#ifndef __USER__
#define __USER__

#include <string>
#include <iostream>

class User {
private:
    int id;
	std::string login;
	std::string password;
	int role;
	int active;
public:
    User(int id, char * login, char * password, int role) : active(0)
    {
		this -> id = id;
		this -> login = login;
		this -> password = password;
		this -> role = role;
	} 
	
	int getId()
	{
		return id;
	}
	
	std::string getPassword()
	{
		return password;
	}
	
	std::string getLogin()
	{
		return login;
	}
	
	int getRole()
	{
		return role;
	}
	
	int isActive()
	{
	   return !!active;
	}
	
	inline bool operator == (User &user) const
	{
		if (id != (user.getId())) return false;
		if (login != user.getLogin()) return false;
		if (password != user.getPassword()) return false;
		if (role != user.getRole()) return false;
		if (active != user.isActive()) return false;
		return true;
	}
	
	bool operator!=(User& user) const {
		return !((*this) == user);
	}
};

#endif

