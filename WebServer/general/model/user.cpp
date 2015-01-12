#ifndef __USER__
#define __USER__

#include <string>

class User {
private:
    int id;
	std::string login;
	std::string password;
	int role;
	int active;
	int status; //for ORM
public:

	enum {AUTHORIZED_USER, NO_AUTHORIZED_USER_ACTIVE, BAD_SOCKET};
    User(int id, char * login, char * password, int role) : active(NO_AUTHORIZED_USER_ACTIVE)
    {
		this -> id = id;
		this -> login = login;
		this -> password = password;
		this -> role = role;
	} 
	
	User() : active(NO_AUTHORIZED_USER_ACTIVE)
	{
	
	}
	
	int getId()
	{
		return id;
	}
	
	void setId(int id)
	{
		this -> id  = id;
	}
	
	std::string getPassword()
	{
		return password;
	}
	
	void setPassword(std::string password)
	{
		this ->password = password;
	}
	
	std::string getLogin()
	{
		return login;
	}
	
	void setLogin(std::string login)
	{
		this -> login = login;
	}
	
	int getRole()
	{
		return role;
	}
	
	void setRole(int role) 
	{
		this -> role = role;
	}
	
	int isActive()
	{
	   return active == User::AUTHORIZED_USER;
	}
	
	void setActive(int active)
	{
		this -> active = active;
	}
	
	void setStatus(int status)
	{
		this -> status = status;
	}
	
	int getStatus()
	{
		return status;
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

