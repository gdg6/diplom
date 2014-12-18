#ifndef __DEVIE_SERVICE__
#define __DEVIE_SERVICE__


#include "sqlSnmp.cpp"
#include "sqlite3IO.cpp"
#include "pgSql.cpp"
#include "snmpSessionDevice.cpp"
#include <memory>
#include <thread>
#include <string>

#include "./device.cpp"

class DeviceService 
{
	
    int callback( void * data, int argc, char ** argv, char ** azColName)
	{
		   std::cerr << ((const char * ) data) << ":" ;
		  for(int i = 0; i < argc; i++)
		  {
			 std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
		  }
		  std::cout << std::endl;
		  return 0;
	}
public:


	static std::shared_ptr<Device>  getByPk(int id) 
	{
		return std::shared_ptr<Device>(new Device);
	}

};


#endif


int main()
{
	std::shared_ptr<Device>  device  = DeviceService::getByPk(1);
	
	//=== print Device parametres
	std::cout << device->getLogin() << std::endl;
	std::cout << device -> getPassword() << std::endl;
	std::cout << device -> getPassword() << std::endl;
	
	
	
}
