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

class SessSnmpDev {
public:
	struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    struct snmp_pdu *response;
    const char * command;  //oid
    size_t commandLen;  //oid
    oid anOID[MAX_OID_LEN];
    size_t anOID_len = MAX_OID_LEN;
    struct variable_list *vars;
    int status;
    int id;
    ReportService * reportService; 
};

class AsyncSnmpManager{

	ReportService reportService;
	DeviceService deviceService;

	std::shared_ptr<std::vector<std::shared_ptr<SessSnmpDev>>> hosts;
	int active_hosts;

	/*
	* response handler
	*/
	static int asynch_response(int operation, struct snmp_session *sp, int reqid,
			struct snmp_pdu *pdu, void *magic)
	{
		SessSnmpDev * host = (SessSnmpDev*)magic;
		struct snmp_pdu *req;
		if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
		if (print_result(STAT_SUCCESS, host->ss, pdu, host->reportService, host->id, host->command)) {
			  //~ host->current_oid++;			/* send next GET (if any) */
			  if (host->command) {
				req = snmp_pdu_create(SNMP_MSG_GET);
				snmp_add_null_var(host->pdu, host->anOID, host->commandLen);
				if (snmp_send(host->ss, req))
					return 1;
				else {
				  snmp_perror("snmp_send");
				  snmp_free_pdu(req);
				}
			  }
		}
		}
		else
		print_result(STAT_TIMEOUT, host->ss, pdu, host -> reportService, host->id, host->command);
//~ 
		//~ /* something went wrong (or end of variables) 
		//~ * this host not active any more
		//~ */
		//~ active_hosts--;
	return 1;
	}

 void initSessions()
 {

	 std::shared_ptr<Device> device;
	 std::shared_ptr<std::vector<std::shared_ptr<Device>>> list = deviceService.getAll();

	 hosts = std::shared_ptr<std::vector<std::shared_ptr<SessSnmpDev>>>(new std::vector<std::shared_ptr<SessSnmpDev>>());
	 
	 /*
	  * Initialize the SNMP library
	  */
	 init_snmp("snmpapp");
 	 for(unsigned int i = 0; i < list->size(); i++)
	 {
		 
		std::shared_ptr<Device> device = list -> at(i);
		std::shared_ptr<SessSnmpDev> sessSnmpDev(new SessSnmpDev);
		sessSnmpDev -> id = device -> getId();
		sessSnmpDev -> reportService = & reportService;
		sessSnmpDev -> command = ".1.3.6.1.4.1.2021.10.1.3.1";
		sessSnmpDev -> commandLen = strlen(".1.3.6.1.4.1.2021.10.1.3.1");
				 
		/*
		* Initialize a "session" that defines who we're going to talk to
		*/
	   snmp_sess_init( &(sessSnmpDev->session) );                   /* set up defaults */
	   sessSnmpDev -> session.peername = (char*)(device -> getPeername()).c_str();

	   /* Use SNMPv3 to talk to the experimental server */

	   /* set the SNMP version number */
	   sessSnmpDev -> session.version = SNMP_VERSION_3;

	   /* set the SNMPv3 user name */
	   sessSnmpDev -> session.securityName = strdup(device->getLogin().c_str());
	   sessSnmpDev -> session.securityNameLen = device->getLogin().length();
	   sessSnmpDev -> session.callback = asynch_response;		/* default callback */
	   sessSnmpDev -> session.callback_magic = sessSnmpDev.get();

	   /* set the security level to authenticated, but not encrypted */
	   sessSnmpDev -> session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;

	   /* set the authentication method to MD5 */
	   sessSnmpDev -> session.securityAuthProto = usmHMACMD5AuthProtocol;
	   sessSnmpDev -> session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
	   sessSnmpDev -> session.securityAuthKeyLen = USM_AUTH_KU_LEN;

	   /* set the authentication key to a MD5 hashed version of our
		  passphrase "The Net-SNMP Demo Password" (which must be at least 8
		  characters long) */
	   if (generate_Ku(sessSnmpDev -> session.securityAuthProto,
					   sessSnmpDev -> session.securityAuthProtoLen,
					   (u_char *) device -> getPassword().c_str(),device -> getPassword().length(),
					   sessSnmpDev -> session.securityAuthKey,
					   &(sessSnmpDev -> session.securityAuthKeyLen) ) != SNMPERR_SUCCESS) {
		   snmp_log(LOG_ERR,
					"Error generating Ku from authentication pass phrase. \n");
		   continue; // no crash all programm
	   }
	  
	  sessSnmpDev -> ss = snmp_open(&(sessSnmpDev->session));
  	  if (!(sessSnmpDev -> ss) ) {
  	    snmp_perror("ack");
  	    snmp_log(LOG_ERR, "something horrible happened!!!\n");
  	    continue;
  	  }
		hosts -> push_back(sessSnmpDev);
		active_hosts++;
		
		
		sessSnmpDev -> pdu = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
		snmp_add_null_var(sessSnmpDev->pdu, sessSnmpDev->anOID, sessSnmpDev->commandLen);
		if (snmp_send(sessSnmpDev->ss, sessSnmpDev->pdu))
		  active_hosts++;
		else {
		  snmp_perror("snmp_send");
		  snmp_free_pdu(sessSnmpDev->pdu);
		}
	 }
	
    /* windows32 specific initialization (is a noop on unix) */
    SOCK_STARTUP;
 }

 /*
 * simple printing of returned data
 */
static int print_result (int status, struct snmp_session *sp, struct snmp_pdu *pdu, ReportService * reportService, int id, std::string type)
{
  char buf[1024];
  struct variable_list *vp;
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
					reportService->insertReport(id, type, std::string(buf));       fprintf(stdout, "%s: %s\n", sp->peername, buf);
					
					vp = vp->next_variable;
			  }
			}
			else {
			  for (int ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++)
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


public:

	AsyncSnmpManager(sqlite3 * db) : reportService(db), deviceService(db), active_hosts(0)
	{
		
	}
	
	int Run()
	{
		initSessions();
		return 0;
	}
	
	int getActiveHosts()
	{
		return active_hosts;
	}

};


#endif

int main()
{
	sqlite3 * db;
	sqlite3_open("../snmp_db", &db);
	AsyncSnmpManager manager(db);
	manager.Run();
	std::cout << "active hosts: " << manager.getActiveHosts();
	return 0;
}
