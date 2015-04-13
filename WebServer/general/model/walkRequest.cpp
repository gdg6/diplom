#ifndef __WALK_REQUEST__
#define __WALK_REQUEST__

#include <string>

class WalkRequest{

private: 
	int id;
	std::string request;
	std::string description;
public:

	void setId(int id)
	{
		this -> id = id;
	}

	int getId()
	{
		return id;
	}

	void setRequest(std::string request)
	{
		this -> request = request;
	}

	std::string getRequest()
	{
		return request;
	}

	void setDescription(std::string description)
	{
		this -> description = description;
	}

	std::string getDescription()
	{
		return description;
	}

};

#endif