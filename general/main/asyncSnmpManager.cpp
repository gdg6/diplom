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
#include "sessSnmpDev.cpp"

#define MSG_TIMEOUT "Timeout"
#define MSG_SEND "snmp_send"
#define MSG_RECV "snmp_recv"
#define MSG_ERR "snmp_err"


class AsyncSnmpManager {

	std::shared_ptr<ReportService> reportService;
	std::shared_ptr<LogService> logService;
	std::shared_ptr<OidService> oidService;
	std::shared_ptr<SqlReportBuffer> sqlReportBuffer;
	std::mutex mt;
	DeviceService deviceService;
	time_t global_timer;
	std::shared_ptr<std::vector<std::shared_ptr<SessSnmpDev>>> hosts;
	int active_hosts;

	void requestByTimer()
	{
		struct snmp_pdu *req;
		for(int i = 0; i < hosts -> size(); i++) {
			if((time(NULL) - (hosts -> at(i) -> last_request)) > (hosts -> at(i)) -> ping_request )	{
				/* send next GET (if any) */
				hosts -> at(i) -> currentOid = hosts -> at(i) -> getNextCommand();
				if ( (hosts -> at(i)) -> currentOid.length() > 0 ) {
					req = snmp_pdu_create(SNMP_MSG_GET);
		 			read_objid((	hosts -> at(i)) -> currentOid.c_str(), (hosts -> at(i)) ->anOID, &((hosts -> at(i)) -> anOID_len ));
					snmp_add_null_var(req, (hosts -> at(i)) -> anOID, (hosts -> at(i))-> anOID_len);
					if (snmp_send((hosts -> at(i)) -> ss, req)) {
						(hosts -> at(i)) -> last_request = time(NULL);
					}
					else {
						snmp_perror("snmp_send");
						snmp_free_pdu(req);
					}
				}
			}
		}
	}


	static void * checkHosts(void * object)
	{
		for(;;) {
			((AsyncSnmpManager *) object) -> requestByTimer(); 
			sleep(1);
		}
	}

 	static int asynch_response(int operation, struct snmp_session *sp, int reqid,
			struct snmp_pdu *pdu, void *magic)
	{
		SessSnmpDev * host = (SessSnmpDev*)magic;
		struct snmp_pdu *req;

		if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
			print_result(STAT_SUCCESS, host->ss, pdu, host->sqlReportBuffer, host->id, host->currentOid);
		}
		else {
			print_result(STAT_TIMEOUT, host -> ss, pdu, host -> sqlReportBuffer, host -> id, host -> currentOid);
		}

		return 1;
	}

	void initSessions()
	{
	   std::unique_lock<std::mutex> lock(mt);
	   std::shared_ptr<Device> device;
	   std::shared_ptr<std::vector<std::shared_ptr<Device>>> list = deviceService.getAll();
	   hosts = std::shared_ptr<std::vector<std::shared_ptr<SessSnmpDev>>>(new std::vector<std::shared_ptr<SessSnmpDev>>());
	   sqlReportBuffer = std::shared_ptr<SqlReportBuffer>(new SqlReportBuffer());

	   init_snmp("snmpapp");
	   for(unsigned int i = 0; i < list->size(); i++) {
		 
		  std::shared_ptr<SessSnmpDev> sessSnmpDev(new SessSnmpDev);
		  std::shared_ptr<Device> device = list -> at(i);

		  sessSnmpDev -> id = device -> getId();
		  sessSnmpDev -> ping_request = device -> getPingRequest();
		  sessSnmpDev -> commands = oidService -> getActiveOidsByDeviceId(sessSnmpDev -> id);
		  sessSnmpDev -> sqlReportBuffer =  sqlReportBuffer.get();
		  sessSnmpDev -> currentOid = sessSnmpDev -> getNextCommand();
					 
		 /*
		  * Initialize a "session" that defines who we're going to talk to
		  */
		  snmp_sess_init( &(sessSnmpDev->session) );                   /* set up defaults */
		  sessSnmpDev -> session.peername = (char*)(device -> getPeername()).c_str();
		  sessSnmpDev -> session.remote_port = (u_short)device -> getPortNumber();
		  sessSnmpDev -> session.version = SNMP_VERSION_3;
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
				logService -> save(SNMP_LOG, "Error generating Ku from authentication pass phra");
			    continue; 
		   }
		  
		  sessSnmpDev -> ss = snmp_open(&(sessSnmpDev->session));
		  if (!(sessSnmpDev -> ss) ) {
			logService -> save(SNMP_ASK_LOG, "something horrible happened!!!");
			continue;
		  }

		  sessSnmpDev -> pdu = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
		  read_objid(sessSnmpDev->currentOid.c_str(), sessSnmpDev->anOID, &(sessSnmpDev ->  anOID_len ));
		  snmp_add_null_var(sessSnmpDev->pdu, sessSnmpDev->anOID, sessSnmpDev -> anOID_len);
		  if (snmp_send(sessSnmpDev->ss, sessSnmpDev->pdu)) 
		  {
		  	sessSnmpDev -> last_request = time(NULL);
		    hosts -> push_back(sessSnmpDev);
		    active_hosts++;
		  }
		   else {
		    reportService->insertReport(device->getId(), MSG_SEND, MSG_TIMEOUT);
		    snmp_free_pdu(sessSnmpDev->pdu);
		  }
	   }

		SOCK_STARTUP;
    }

	void selectRun()
	{
		global_timer = time(NULL);
		pthread_t t2;
		pthread_create(&t2, NULL, AsyncSnmpManager::checkHosts, this);
		while (active_hosts) 
		{
		  if((time(NULL) - global_timer) > 1 || sqlReportBuffer -> isMustBePush())
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
		   	logService -> save(SELECT_LOG, "select failed");
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
		pthread_join(t2, NULL);
	}

	static void print_result (int status, struct snmp_session *sp, struct snmp_pdu *pdu, SqlReportBuffer * sqlReportBuffer, int id, std::string type) 
	{
	  char buf[1024];
	  struct variable_list *vp;
	  
	  switch (status) {
		  case STAT_SUCCESS:
				vp = pdu->variables;
				if (pdu->errstat == SNMP_ERR_NOERROR) {
				  while (vp) {
						snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);
						sqlReportBuffer -> addInsert(id, type, std::string(buf));     
						// fprintf(stdout, "%s: %s\n", sp->peername, buf);
						vp = vp->next_variable;
				  }
				}
				else {
				    for (int ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++);
				    if (vp) snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
				    else strcpy(buf, "(none)");
				    sqlReportBuffer->addInsert(id, MSG_ERR, snmp_errstring(pdu->errstat));
				}
				break;
		  case STAT_TIMEOUT:
				sqlReportBuffer->addInsert(id, MSG_RECV, "Timeout");
				break;
		  case STAT_ERROR:
				sqlReportBuffer->addInsert(id, MSG_ERR, "Error");
	  }
	}

public:

	AsyncSnmpManager(sqlite3 * db, std::shared_ptr<LogService> logService) : deviceService(db), active_hosts(0)	
	{
		reportService = std::shared_ptr<ReportService>(new ReportService(db));
		oidService = std::shared_ptr<OidService>(new OidService(db));
		this -> logService = logService;
	}

	AsyncSnmpManager(sqlite3 * db) : AsyncSnmpManager(db, std::shared_ptr<LogService>(new LogService(db))){}
	
	void Run() {
		initSessions();
		logService -> save(SNMP_INIT_SESSION, "active hosts: " + std::to_string(active_hosts) + " devices.");
		selectRun();
	}
	
	void Restart()
	{
		Stop();
		initSessions();
		logService -> save(SNMP_MANAGER_RESTART, "active hosts: " + std::to_string(active_hosts) + " devices.");
		selectRun();
	}

	void Stop()
	{
		std::unique_lock<std::mutex> lock(mt);
		active_hosts = 0;
		for(unsigned int i = 0; i < hosts->size(); i++)
		{
			snmp_close((hosts->at(i)) -> ss);
		}
		logService -> save(SNMP_MANAGER_STOP, "async manager is stoped!");
	}
	
	int getActiveHosts() {
		std::unique_lock<std::mutex> lock(mt);
		return active_hosts;
	}

};

void * starterAsyncSnmpManager(void * db)
{
	AsyncSnmpManager manager((sqlite3 *)db);
	manager.Run();
}

#endif

