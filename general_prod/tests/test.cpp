#ifndef __TEST__
#define __TEST__

#include "../serviceImpl/servicePack.h"
#include "../utils/schema.cpp"

#define NAME_TEST_DATABASE "test_db"


class TestORM {

	OidORM oidORM;
	DeviceORM deviceORM;
	sqlite3 * db;
	// int success_test;
	// int fail_test;
	// int count_test;

	int insertTestData()
	{
		//insert devices
		int rc(0);
		Device device;
		device.setName("device_test_1");
		device.setRoom("2204");
		device.setMac("0000000000");
		device.setSerialNumber("0000000000");
		device.setModel("test_model_1");
		device.setPeername("localhost");
		device.setPortNumber(161);
		device.setLogin("snmpuser");
		device.setPassword("password");
		device.setPingRequest(45);
		rc |= deviceORM.insertDevice(device);
		std::cout << "insert Device: ";
		if (rc != SQLITE_DONE ) {
			 std::cout << "Error!!!\n";
			// fail_test++;
			return rc;
		} else {
			// success_test++;
			std::cout << "Ok!\n";
		}

		


	}

	int initDB()
	{
		Schema * schema = new Schema(db);
		if(schema -> getStatus() != SQLITE_OK && schema -> getStatus() != SQLITE_DONE ) {
			std::cout << sqlite3_errmsg(db) << std::endl;
			return schema->getStatus();
		}
		delete schema;
	}

	int testOidORM()
	{
		std::shared_ptr<std::vector<std::shared_ptr<Oid>>> list;
		list = oidORM.getAll();
		std::cout << "Проверка получения всех oids:\n"; 
		std::cout << "count all : " << list-> size() << std::endl;;
		for(int i = 0; i < list -> size(); i ++)
		{
			std::cout << "#############" << std::endl;
			std::cout << "ID: " <<  (list -> at(i)) -> getId() << std::endl;
			std::cout << "Oid: " << (list -> at(i)) -> getOid() << std::endl;
			std::cout << "Translate: " << (list -> at(i)) -> getTranslate() << std::endl;
			std::cout << "Device_id: " << (list -> at(i)) -> getDeviceId() << std::endl;
			std::cout << "Active: " << ((list -> at(i)) -> getActive() ?  "Активен"  : "Неактивен") << std::endl;
		}

		std::shared_ptr<Oid> oid = oidORM.getByPK(8);
		std::cout << "Проверка получения по ID:\n"; 
		if(oid -> getStatus() == Oid::INIT_SUCCESS)
		{
			std::cout << "#############" << std::endl;
			std::cout << "ID: " <<  oid -> getId() << std::endl;
			std::cout << "Oid: " << oid -> getOid() << std::endl;
			std::cout << "Translate: " << oid -> getTranslate() << std::endl;
			std::cout << "Device_id: " << oid -> getDeviceId() << std::endl;
			std::cout << "Active: " << (oid -> getActive() ?  "Активен"  : "Неактивен") << std::endl;		
		}else {
			std::cout << "Произошла ошибка при инициализации\n";
		}

		list = oidORM.getListByDeviceId(1);
		std::cout << "Проверка получения всех oids по id устройства:\n"; 
		std::cout << "size = " << list->size() << std::endl;
		for(int i = 0; i < list -> size(); i ++)
		{
			std::cout << "---------" << std::endl;
			std::cout << "ID: " <<  (list -> at(i)) -> getId() << std::endl;
			std::cout << "Oid: " << (list -> at(i)) -> getOid() << std::endl;
			std::cout << "Translate: " << (list -> at(i)) -> getTranslate() << std::endl;
			std::cout << "Device_id: " << (list -> at(i)) -> getDeviceId() << std::endl;
			std::cout << "Active: " << ((list -> at(i)) -> getActive() ?  "Активен"  : "Неактивен") << std::endl;
		}

	}

public: 
	TestORM(sqlite3 * db) : oidORM(db), deviceORM(db) {
		success_test = 0;
		fail_test = 0;
		count_test = 0;
		this -> db = db;
		initDB();
		insertTestData();
		// testOidORM();
	}	
};

int main()
{
	sqlite3 * db;
	std::cout << "Open database: ";
	if(sqlite3_open(NAME_TEST_DATABASE, & db) == 0){
		std::cout << "SUCCESS\n";
		TestORM testORM(db);
	}
	else{
		std::cout << "FAIL\n";
	}
	// std::cout << "Пройденно тестов: " << count_test << std::endl;
	// std::cout << "Успешно: " << success_test << std::endl;
	// std::cout << "Проваленно: " << fail_test << std::endl;
	std::cout << "Test Finished!!!\n";
	sqlite3_close(db);
}
#endif 