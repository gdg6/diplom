
#include "App.cpp"

//~ PRAGMA journal_mode=WAL - for async SQLITE

int main(int argc, char *argv[])
{
	sqlite3 * db;
	sqlite3_open("../../snmp_db", &db);
	App app(db);
	app.Run();
	
	return 0;
}
