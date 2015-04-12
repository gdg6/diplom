#ifndef __DEVICE_ORM__
#define __DECICE_ORM__

#include "ORM.cpp"
#include "../model/device.cpp"
#include <iostream>

class DeviceORM  : public ORM
{
	std::string sql_select_by_id = "SELECT name, description, room, mac, serial_number, model,  peername, port, login, password, priv_password FROM devices WHERE Id = ?";
	std::string sql_select_all = "SELECT * FROM devices;";
	std::string sql_insert = "INSERT INTO \'devices\' (\'name\', \'description\', \'room\', \'mac\', \'serial_number\', \'model\', \'peername\', \'port\', \'login\', \'password\', \'priv_password\', \'created_at\', \'updated_at\') VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

	int rc; //status for db;
	sqlite3 * db;
	
public:

	DeviceORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}
	
	int insertDevice(Device device) {
		sqlite3_stmt * stmt;
		std::string time  = currentDateTime();
		int rc = sqlite3_prepare_v2(db, sql_insert.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc |= sqlite3_bind_text(stmt, 1, device.getName().c_str(), device.getName().length(), SQLITE_STATIC);    
		rc |= sqlite3_bind_text(stmt, 2, device.getDescription().c_str(), device.getDescription().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 3, device.getRoom().c_str(), device.getRoom().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 4, device.getMac().c_str(), device.getMac().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 5, device.getSerialNumber().c_str(), device.getSerialNumber().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 6, device.getModel().c_str(), device.getModel().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 7, device.getPeername().c_str(), device.getPeername().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_int(stmt, 8, device.getPortNumber()); 
		rc |= sqlite3_bind_text(stmt, 9, device.getLogin().c_str(), device.getLogin().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 10, device.getPassword().c_str(), device.getPassword().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 11, device.getPrivPassword().c_str(), device.getPassword().length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 12, time.c_str(), time.length(), SQLITE_STATIC); 
		rc |= sqlite3_bind_text(stmt, 13, time.c_str(), time.length(), SQLITE_STATIC); 

		std::cout << rc << std::endl;

		if (rc != SQLITE_OK) 
		{                 
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return rc;
		}
 
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}
	
	std::shared_ptr<std::vector<std::shared_ptr<Device>>> getAll()
	{

		std::shared_ptr<std::vector<std::shared_ptr<Device>>> list(new std::vector<std::shared_ptr<Device>>());
		sqlite3_stmt *stmt;
		
		int rc = sqlite3_prepare_v2(db, sql_select_all.c_str(), -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			// throw std::string (sqlite3_errmsg(db));
			return list;
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
			device -> setPrivPassword( std::string((const char *)sqlite3_column_text(stmt, 11)));

			list->push_back(device);
		}

		sqlite3_finalize(stmt);

		return list;
	}
	
	
	std::shared_ptr<Device> getByPK(int id)
	{

		std::shared_ptr<Device> device = std::shared_ptr<Device>(new Device);
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
			device -> setPrivPassword( std::string((const char *)sqlite3_column_text(stmt, 10)));

		}

		sqlite3_finalize(stmt);
		
		return device;
	}
	
	~DeviceORM()
	{
		
	}
};

#endif

// int main()
// {
// 	Device  device;
// 	device . setName("name");
// 	device . setPassword("password");
// 	device . setLogin("login");
// 	device . setPeername("peername");
// 	device . setModel("model");
// 	device . setMac("mac");
// 	device . setPortNumber(123);
// 	device . setDescrition("description");
// 	device . setSerialNumber("serial");
// 	device . setRoom("room");
// 	device . setPrivPassword("password");

	

// 	sqlite3 * db;
// 	sqlite3_open("../snmp_db", &db);
// 	DeviceORM *  deviceORM = new DeviceORM(db);
// 	deviceORM -> insertDevice(device);

// 	std::cout << (deviceORM -> getAll()) -> size();
// 	sqlite3_close(db);
// 	delete deviceORM;
// 	return 0;
// }