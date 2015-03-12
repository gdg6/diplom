#ifndef __DEVICE_ORM__
#define __DECICE_ORM__

#include "ORM.cpp"
#include "../model/device.cpp"

class DeviceORM  : public ORM
{
    std::string sql_create_table = "CREATE TABLE \'devices\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'name\' varchar(255), \'description\' varchar(1024), \'room\' varchar(25), \'mac\' varchar(255), \'serial_number\' varchar(255), \'model\' varchar(1024), \'port\' integer, \'login\' varchar(255), \'password\' varchar(255), \'mib_id\' integer, \'created_at\' datetime, \'updated_at\' datetime);";
	std::string sql_select_by_id = "SELECT Name, Description, Room, Mac, Serial_number, Model,  Peername, Port, Login, Password, Mib_id FROM devices WHERE Id = ?";
	std::string sql_select_all = "SELECT * FROM devices;";
	
	int rc; //status for db;
	sqlite3 * db;
	
public:

	DeviceORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}
	
	
	std::shared_ptr<std::vector<std::shared_ptr<Device>>> getAll()
	{

		std::shared_ptr<std::vector<std::shared_ptr<Device>>> list(new std::vector<std::shared_ptr<Device>>());
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
			device -> setMibPk( atoi((const char *)sqlite3_column_text(stmt, 10)));
		}

		sqlite3_finalize(stmt);
		
		return device;
	}
	
	~DeviceORM()
	{
		
	}
};

#endif

