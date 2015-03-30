#ifndef __WALK_REQUEST__
#define __WALK_REQUEST__

#include <string>

class WalkRequest{

private: 
	int id;
	std::string request;
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

};

#endif