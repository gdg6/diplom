#include <iostream>
#include <sqlite3.h>

int main()
{
   sqlite3 * db; 
   char * zErrMsg = 0;
   int rc;

   rc = sqlite3_open("snmp_db", & db);

   if (rc )
   {
   	std::cout << "can not open database: " << sqlite3_errmsg(db);
   	return 1;
   } 
        std::cout << "success open database" << std::endl;
	sqlite3_close(db);
	

return 0;
}
