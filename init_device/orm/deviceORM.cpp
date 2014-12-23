#ifndef __DEVICE_ORM__
#define __DECICE_ORM__

#include "ORM.cpp"
#include "../model/device.cpp"

class DeviceORM  : public ORM
{
	std::string sql_select_by_id = "SELECT Name, Description, Room, Mac, Serial_number, Model,  Peername, Port, Login, Password, Mib_id FROM devices WHERE Id = ?";
	std::string sql_select_all = "SELECT * FROM devices;";
	int rc; //status for db;
	
#ifdef __SQLITE3__
	  sqlite3 * db;
#else

#endif
	
public:


#ifdef __SQLITE3__
	DeviceORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}
#else
    //pg
#endif
	
	
	std::shared_ptr<std::vector<std::shared_ptr<Device>>> getAll()
	{

		std::shared_ptr<std::vector<std::shared_ptr<Device>>> list(new std::vector<std::shared_ptr<Device>>());
#ifdef __SQLITE3__
		sqlite3_stmt *stmt;
		
		int rc = sqlite3_prepare_v2(db, sql_select_all.c_str(), -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			throw std::string (sqlite3_errmsg(db));
		}
		std::shared_ptr<Device> device;
		while((rc = sqlite3_step(stmt) ==  100)) { // до тех пор пока запрос возращяет результат
			device = std::shared_ptr<Device>(new Device);
			device -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			device -> setName( std::string((const char *)sqlite3_column_text(stmt, 1)));
			device -> setDescrition( std::string((const char *)sqlite3_column_text(stmt, 2)));
			device -> setRoom( std::string((const char *)sqlite3_column_text(stmt, 3)));
			device -> setMac( std::string((const char *)sqlite3_column_text(stmt, 4)));
			device -> setSerialNumber( std::string((const char *)sqlite3_column_text(stmt, 5)));
			device -> setModel(std::string((const char *)sqlite3_column_text(stmt, 6)));
			device -> setPeername( std::string((const char *)sqlite3_column_text(stmt, 7)));
			device -> setPortNumber( atoi((const char *)sqlite3_column_text(stmt, 8)));
			device -> setLogin( std::string((const char *)sqlite3_column_text(stmt, 9)));
			device -> setPassword( std::string((const char *)sqlite3_column_text(stmt, 10)));
			device -> setMibPk( atoi((const char *)sqlite3_column_text(stmt, 11)));
			list->push_back(device);
		}
#else

#endif
		return list;
	}
	
	
	std::shared_ptr<Device> getByPK(int id)
	{

		std::shared_ptr<Device> device = std::shared_ptr<Device>(new Device);
#ifdef __SQLITE3__		
		sqlite3_stmt *stmt;

		int rc = sqlite3_prepare_v2(db, sql_select_by_id.c_str(), -1, &stmt, NULL);
		if (rc != SQLITE_OK){
			device->setStatus(device ->INIT_BAD);
			return device;
		}
		rc = sqlite3_bind_int(stmt, 1, id);    // Using parameters ("?") is not
		if (rc != SQLITE_OK) {                 // really necessary, but recommended
			sqlite3_finalize(stmt);            // formatting problems and SQL
			device->setStatus(device ->INIT_BAD);
			return device;            // injection attacks.
		}

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			sqlite3_finalize(stmt);
			device->setStatus(device ->INIT_BAD);
			return device;
		}
		if (rc == SQLITE_DONE) {
			sqlite3_finalize(stmt);
		    device->setStatus(device ->INIT_BAD);
			return device;
		}
	
		if(device->getStatus() == device->INIT_SUCCESS) 
		{
			device -> setId(id);
			device -> setName( std::string((const char *)sqlite3_column_text(stmt, 0)));
			device -> setDescrition( std::string((const char *)sqlite3_column_text(stmt, 1)));
			device -> setRoom( std::string((const char *)sqlite3_column_text(stmt, 2)));
			device -> setMac( std::string((const char *)sqlite3_column_text(stmt, 3)));
			device -> setSerialNumber( std::string((const char *)sqlite3_column_text(stmt, 4)));
			device -> setModel(std::string((const char *)sqlite3_column_text(stmt, 5)));
			device -> setPeername( std::string((const char *)sqlite3_column_text(stmt, 6)));
			device -> setPortNumber( atoi((const char *)sqlite3_column_text(stmt, 7)));
			device -> setLogin( std::string((const char *)sqlite3_column_text(stmt, 8)));
			device -> setPassword( std::string((const char *)sqlite3_column_text(stmt, 9)));
			device -> setMibPk( atoi((const char *)sqlite3_column_text(stmt, 10)));
		}

		sqlite3_finalize(stmt);
#else

#endif
		return device;
	}
	~DeviceORM()
	{
		
	}
};

#endif

#include <iostream>

int main()
{
	sqlite3 * db;
	sqlite3_open("../../snmp_db", &db);
	DeviceORM d(db);
	std::shared_ptr<std::vector<std::shared_ptr<Device>>> devices = d.getAll();
	for(unsigned int i = 0; i < devices->size(); i ++) {
		std::shared_ptr<Device> device = devices->at(i);
		std::cout << "Id: " <<  device->getId() << std::endl;
		std::cout << "Name: " << device->getName() << std::endl;
		std::cout << "Description: " << device->getDescription() << std::endl;
		std::cout << "Room: " << device->getRoom() << std::endl;
		std::cout << "Mac: " << device->getMac() << std::endl;
		std::cout << "SerialNumber: " << device->getSerialNumber() << std::endl;
		std::cout << "Peername: " << device->getPeername() << std::endl;
		std::cout << "PortNumber: " << device->getPortNumber() << std::endl;
		std::cout << "Login: " << device->getLogin() << std::endl;
		std::cout << "Password: " << device->getPassword() << std::endl;
		std::cout << "Mib_id: " << device->getMibPk() << std::endl;
		std::cout << "status " << (device->getStatus() == Device::INIT_SUCCESS) << std::endl;
		std::cout << "###########################################" << std::endl;
	}
	return 0;
}
