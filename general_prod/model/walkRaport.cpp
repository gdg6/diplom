#ifndef __WALK_RAPORT__
#define __WALK_RAPORT__

#include <string>

class WalkRaport
{
private: 
	int id;
	std::string path;
	int device_id;

public: 

	void setId(int id)
	{
		this -> id = id;
	}

	int getId()
	{
		return id;
	}

	void setPath(std::string path)
	{
		this -> path = path;
	}

	std::string getPath()
	{
		return path;
	}

	void setDeviceId(int device_id)
	{
		this -> device_id = device_id;
	}

	int getDeviceId()
	{
		return device_id;
	}

};


#endif