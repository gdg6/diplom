#ifndef __ASYNC_SNMP_MANAGER__
#define __ASYNC_SNMP_MANAGER__

#include "../serviceImpl/servicePack.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/library/transform_oids.h>
#include <iostream>
#include <memory>
#include <thread>

const char *our_v3_passphrase = "password";

class sessSnmpDev {
public: 
	struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    struct snmp_pdu *response;
    oid anOID[MAX_OID_LEN];
    size_t anOID_len = MAX_OID_LEN;
    struct variable_list *vars;
    int status;
};

class AsyncSnmpManager{
	
	ReportService reportService;
	DeviceService deviceService;
	
	std::shared_ptr<std::vector<std::shared_ptr<sessSnmpDev>>> hosts;	
	int active_hosts;
	
/*
 * simple printing of returned data
 */
int print_result (int status, struct snmp_session *sp, struct snmp_pdu *pdu)
{
  char buf[1024];
  struct variable_list *vp;
  int ix;
  struct timeval now;
  struct timezone tz;
  struct tm *tm;

  gettimeofday(&now, &tz);
  tm = localtime(&now.tv_sec);
  fprintf(stdout, "%.2d:%.2d:%.2d.%.6d ", tm->tm_hour, tm->tm_min, tm->tm_sec,
            (int)now.tv_usec);
  switch (status) {
	  case STAT_SUCCESS:
			vp = pdu->variables;
			if (pdu->errstat == SNMP_ERR_NOERROR) {
			  while (vp) {
					snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);
					fprintf(stdout, "%s: %s\n", sp->peername, buf);
					vp = vp->next_variable;
			  }
			}
			else {
			  for (ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++)
				;
			  if (vp) snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
			  else strcpy(buf, "(none)");
			  fprintf(stdout, "%s: %s: %s\n",
				sp->peername, buf, snmp_errstring(pdu->errstat));
			}
			return 1;
	  case STAT_TIMEOUT:
			fprintf(stdout, "%s: Timeout\n", sp->peername);
			return 0;
	  case STAT_ERROR:
			snmp_perror(sp->peername);
			return 0;
  }
  return 0;
}
	
	


 void initSessions()
 {
	 std::shared_ptr<Device> device;
	 std::shared_ptr<std::vector<std::shared_ptr<Device>>> list = deviceService.getAll();
	 /*
	  * Initialize the SNMP library
	  */
	 init_snmp("snmpapp");

    /*
    * Initialize a "session" that defines who we're going to talk to
    */
   snmp_sess_init( &session );                   /* set up defaults */
   session.peername = peername;
   
   /* set up the authentication parameters for talking to the server */
   
   
   /* Use SNMPv3 to talk to the experimental server */
   
   /* set the SNMP version number */
   session.version=SNMP_VERSION_3;
        
   /* set the SNMPv3 user name */
   session.securityName = strdup("snmpuser");
   session.securityNameLen = strlen(session.securityName);
   
   /* set the security level to authenticated, but not encrypted */
   session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;
   
   /* set the authentication method to MD5 */
   session.securityAuthProto = usmHMACMD5AuthProtocol;
   session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
   session.securityAuthKeyLen = USM_AUTH_KU_LEN;
    
   /* set the authentication key to a MD5 hashed version of our
      passphrase "The Net-SNMP Demo Password" (which must be at least 8
      characters long) */
   if (generate_Ku(session.securityAuthProto,
                   session.securityAuthProtoLen,
                   (u_char *) our_v3_passphrase, strlen(our_v3_passphrase),
                   session.securityAuthKey,
                   &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
       snmp_log(LOG_ERR,
                "Error generating Ku from authentication pass phrase. \n");
       exit(1);
   }
   

    /* windows32 specific initialization (is a noop on unix) */
    SOCK_STARTUP;
 }
 
 
public: 

	AsyncSnmpManager(sqlite3 * db) : reportService(db), deviceService(db), active_hosts(0)
	{
		
	}

	int getActiveHosts()
	{
		return active_hosts;
	}
	
};


#endif

int main()
{
	return 0;
}
