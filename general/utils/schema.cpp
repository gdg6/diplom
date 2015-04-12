#ifndef __SCHEMA__
#define __SCHEMA__

#include <string>
#include <sqlite3.h>

class Schema {
private:

	sqlite3 * db;
	int status;
	const std::string conf_async = "PRAGMA journal_mode=WAL;";

	const std::string table_users = "CREATE TABLE \'users\' ( \
		\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
		\'login\' varchar(255), \
		\'password_digest\' varchar(255), \
		\'role\' integer, \
		\'created_at\' datetime, \
		\'updated_a\' datetime);"
	;

    const std::string table_devices = "CREATE TABLE \'devices\' (\
    	\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
    	\'name\' varchar(255), \
    	\'description\' varchar(1024), \
    	\'room\' varchar(25), \
    	\'mac\' varchar(255), \
    	\'serial_number\' varchar(255), \
    	\'model\' varchar(1024), \
    	\'peername\' varchar(255), \
    	\'port\' integer, \
    	\'login\' varchar(255), \
    	\'password\' varchar(255), \
    	\'priv_password\' varchar(1024), \
    	\'created_at\' datetime, \
    	\'updated_at\' datetime);"
	;


	const std::string table_oids = "CREATE TABLE \'oids\' (\
		\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
		\'device_id\' integer NOT NULL, \
		\'oid\' varchar(255), \
		\'translate\' varchar(255), \
		\'ping_request\' integer NOT NULL, \
		\'active\' boolean NOT NULL, \
		\'created_at\' datetime, \
		\'updated_at\' datetime);"
	;

	const std::string index_oids_on_device_id = "CREATE INDEX \'index_oids_on_device_id\' ON \'oids\' (\'device_id\');";

	const std::string table_reports = "CREATE TABLE \'reports\' (\
		\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
		\'device_id\' integer, \
		\'r_type\' varchar(255), \
		\'context\' text, \
		\'created_at\' datetime, \
		\'updated_at\' datetime);"
	;

	const std::string table_walk_raports = "CREATE TABLE \'walk_raports\' (\
		\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
		\'path\' varchar(255), \
		\'device_id\' integer NOT NULL, \
		\'created_at\' datetime, \
		\'updated_at\' datetime);"
	;

	const std::string create_index_walk_raports_devices = "CREATE INDEX \'index_walk_raports_on_device_id\' ON \'walk_raports\' (\'device_id\');"
	;

	const std::string table_walk_request = "CREATE TABLE \'walk_requests\' (\
		\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
		\'request\' varchar(255), \
		\'translate\' varchar(255), \
		\'created_at\' datetime, \
		\'updated_at\' datetime);"
	;	
	
	const std::string table_logs = "CREATE TABLE \'logs\' (\
		\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
		\'l_type\' integer, \
		\'context\' text, \
		\'created_at\' datetime, \
		\'updated_at\' datetime);"
	;


	int createWalkRaports() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_walk_raports.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createIndexWalkRaportsToDevice() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, create_index_walk_raports_devices.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createWalkRequest() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_walk_request.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createLog() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_logs.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createUser() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_users.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createDevice() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_devices.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createOid() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_oids.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createIndexOidsToDeviceId() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, index_oids_on_device_id.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	int createReport() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, table_reports.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}

	void initTables() {
		status |= createReport();
		status |= createUser();
		status |= createOid();
		status |= createIndexOidsToDeviceId();
		status |= createDevice();
		status |= createLog();
		status |= createWalkRequest();
		status |= createWalkRaports();
		status |= createIndexWalkRaportsToDevice();
	}

	int configure() {
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, conf_async.c_str(), -1, &stmt, NULL);

		if (rc != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			return rc;
		}
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return rc;
	}
public:

	Schema(sqlite3 * db) : status(0) {
		this -> db = db;
		configure();
		initTables();
	}


	int getStatus()
	{
		return status;
	}

	~Schema() {

	}

};

#endif
