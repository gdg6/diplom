#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

 /*
  * a list of hosts to query
  */
 struct host {
   char *name;
   char *community;
 };
 host  hosts[] = {
   { "test1", "public" },
   { "test2", "public" },
   { "test3", "public" },
   { "test4", "public" },
   { NULL, NULL }
 };
 
 /*
  * a list of variables to query for
  */
 struct oid {
   char *Name;
   oid Oid[MAX_OID_LEN];
   int OidLen;
 } oids[] = {
   { "system.sysDescr.0" },
   { "interfaces.ifNumber.1" },
   { "interfaces.ifNumber.0" },
   { NULL }
 };


int main()
{
return 0;
}
