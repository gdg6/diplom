#ifndef __ASYNC_SNMP_MANAGER__
#define __ASYNC_SNMP_MANAGER__

#include "../serviceImpl/servicePack.h"
#include "../lib/libSnmpCommand.h"
#include "../utils/sqlReportBuffer.cpp"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/library/transform_oids.h>
#include <iostream>
#include <memory>
#include <thread>
#include <time.h>
#include <unistd.h>

#define MSG_TIMEOUT "Timeout"
#define MSG_SEND "snmp_send"
#define MSG_RECV "snmp_recv"
#define MSG_ERR "snmp_err"


class SessSnmpDev {
public:
    int id;
	struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    const char * command;  //oid
    oid anOID[MAX_OID_LEN];
    size_t  anOID_len = MAX_OID_LEN;
    SqlReportBuffer * sqlReportBuffer;
};

class AsyncSnmpManager {

	std::shared_ptr<ReportService> reportService;
	std::shared_ptr<LogService> logService;
	std::shared_ptr<SqlReportBuffer> sqlReportBuffer;
	std::mutex mt;
	DeviceService deviceService;
	time_t global_timer;

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
			if (print_result(STAT_SUCCESS, host->ss, pdu, host->sqlReportBuffer, host->id, host->command)) {
			  //~ host->current_oid++;			/* send next GET (if any) */
			  if ( host -> command ) {
				req = snmp_pdu_create(SNMP_MSG_GET);
				snmp_add_null_var(req, host -> anOID, host -> anOID_len);
				if (snmp_send(host -> ss, req)) {
					return 1;
				}
				else {
				  snmp_perror("snmp_send");
				  snmp_free_pdu(req);
				}
			  }
			}
		}
		else
		print_result(STAT_TIMEOUT, host -> ss, pdu, host -> sqlReportBuffer, host -> id, host -> command);

		return 1;
	}

	void initSessions()
	{
	   std::string command;
	   std::unique_lock<std::mutex> lock(mt);
	   std::shared_ptr<Device> device;
	   std::shared_ptr<std::vector<std::shared_ptr<Device>>> list = deviceService.getAll();
	   hosts = std::shared_ptr<std::vector<std::shared_ptr<SessSnmpDev>>>(new std::vector<std::shared_ptr<SessSnmpDev>>());
	   sqlReportBuffer = std::shared_ptr<SqlReportBuffer>(new SqlReportBuffer());
		/*
		 * Initialize the SNMP library
		 */
	   init_snmp("snmpapp");
	   for(unsigned int i = 0; i < list->size(); i++) {
		 
		  std::shared_ptr<Device> device = list -> at(i);
		  std::shared_ptr<SessSnmpDev> sessSnmpDev(new SessSnmpDev);
		  sessSnmpDev -> id = device -> getId();

		  sessSnmpDev -> sqlReportBuffer =  sqlReportBuffer.get();
		  command = device -> getNextCommand();
		  sessSnmpDev -> command = command.c_str();
		  // sessSnmpDev -> commandLen = command.length();
					 
		 /*
		  * Initialize a "session" that defines who we're going to talk to
		  */
		  snmp_sess_init( &(sessSnmpDev->session) );                   /* set up defaults */
		  sessSnmpDev -> session.peername = (char*)(device -> getPeername()).c_str();

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
			   snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase. \n");
			   continue; // no crash all programm
		   }
		  
		  sessSnmpDev -> ss = snmp_open(&(sessSnmpDev->session));
		  if (!(sessSnmpDev -> ss) ) {
			snmp_perror("ack");
			snmp_log(LOG_ERR, "something horrible happened!!!\n");
			continue;
		  }

		  sessSnmpDev -> pdu = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
		  read_objid(sessSnmpDev->command, sessSnmpDev->anOID, &(sessSnmpDev ->  anOID_len ));
		  snmp_add_null_var(sessSnmpDev->pdu, sessSnmpDev->anOID, sessSnmpDev -> anOID_len);
		  if (snmp_send(sessSnmpDev->ss, sessSnmpDev->pdu)) 
		  {
		    hosts -> push_back(sessSnmpDev);
		    active_hosts++;
		  }
		   else {
		    reportService->insertReport(device->getId(), MSG_SEND, MSG_TIMEOUT);
		    snmp_free_pdu(sessSnmpDev->pdu);
		  }
	   }

		/* windows32 specific initialization (is a noop on unix) */
		SOCK_STARTUP;
    }

	void selectRun()
	{
		global_timer = clock();
		while (active_hosts) 
		{
		  if(((float)clock() - global_timer) / CLOCKS_PER_SEC > 1 || sqlReportBuffer -> isMustBePush())
		  {
			reportService->sqlExec(sqlReportBuffer->pushSql());
			global_timer = clock();
		  }
		   int fds = 0, block = 1;
		   fd_set fdset;
		   struct timeval timeout;
		   timeout.tv_sec = 15;
		   timeout.tv_usec = 0;
		   FD_ZERO(&fdset);
		   snmp_select_info(&fds, &fdset, &timeout, &block);
		   fds = select(fds+1, &fdset, NULL, NULL, &timeout);
		  
		   if (fds < 0) {
			  //~ perror("select failed");
			  return; //no crash programm
		   }
		   if (fds) {
			 snmp_read(&fdset);
		   }
			else
		  {
		    snmp_timeout();
		  }
	    }
	}

	/*
	 * simple printing of returned data
	 */
	static int print_result (int status, struct snmp_session *sp, struct snmp_pdu *pdu, SqlReportBuffer * sqlReportBuffer, int id, std::string type) 
	{
	  char buf[1024];
	  struct variable_list *vp;
	  
	  switch (status) {
		  case STAT_SUCCESS:
				vp = pdu->variables;
				if (pdu->errstat == SNMP_ERR_NOERROR) {
				  while (vp) {
						snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);
						sqlReportBuffer->addInsert(id, type, std::string(buf));      
					//	fprintf(stdout, "%s: %s\n", sp->peername, buf);
						vp = vp->next_variable;
				  }
				}
				else {
				  for (int ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++);
				  if (vp) snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
				  else strcpy(buf, "(none)");
					sqlReportBuffer->addInsert(id, MSG_ERR, snmp_errstring(pdu->errstat));
				}
				return 1;
		  case STAT_TIMEOUT:
				sqlReportBuffer->addInsert(id, MSG_RECV, "Timeout");
				return 0;
		  case STAT_ERROR:
				sqlReportBuffer->addInsert(id, MSG_ERR, "Timeout");
				snmp_perror(sp->peername);
				return 0;
	  }
	  return 0;
	}

public:

	AsyncSnmpManager(sqlite3 * db, std::shared_ptr<LogService> logService) : deviceService(db), active_hosts(0)	
	{
		reportService = std::shared_ptr<ReportService>(new ReportService(db));
		this -> logService = logService;
	}


	AsyncSnmpManager(sqlite3 * db) : AsyncSnmpManager(db, std::shared_ptr<LogService>(new LogService(db)))
	{
	}
	
	void Run() {
		initSessions();
		//~ std::cout << "init sessions: " << active_hosts << " devices" << std::endl;
		selectRun();
	}
	
	void Stop()
	{
		//std::shared_ptr must auto free objects
		active_hosts = 0;
		for(unsigned int i = 0; i < hosts->size(); i++)
		{
			snmp_close((hosts->at(i)) -> ss);
		}
	}
	
	int getActiveHosts() {
		std::unique_lock<std::mutex> lock(mt);
		return active_hosts;
	}
	~AsyncSnmpManager()
	{
		
	}

};

void * starterAsyncSnmpManager(void * db)
{
	AsyncSnmpManager manager((sqlite3 *)db);
	manager.Run();
	return NULL;
}

#endif

