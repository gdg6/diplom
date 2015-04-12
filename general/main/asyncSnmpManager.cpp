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

	//FIXME - Надо опрашивать по таймауту каждой комманды
	void requestByTimer()
	{
		struct snmp_pdu *req;
		
		for(int i = 0; i < hosts -> size(); i++) {
			std::shared_ptr<SessSnmpDev> host = hosts -> at(i);
			for(int j = 0; j < ((host -> commands) -> size()); j++) 
			{
				std::shared_ptr<Oid> currentOid = host -> getNextCommand();
				if(currentOid != NULL) 
				{
					if( (time(NULL) -  (currentOid -> getLastTimeRequest()) > (currentOid -> getPingRequest())) && (currentOid -> getOid().length()) > 0 )
					{
						host -> currentOid = currentOid -> getOid();
						req = snmp_pdu_create(SNMP_MSG_GET);
						read_objid(currentOid -> getOid().c_str(), host ->anOID, &(host -> anOID_len) );
						snmp_add_null_var(req, host -> anOID, host-> anOID_len);

						if (snmp_send(host -> ss, req)) {
							currentOid -> setLastTimeRequest(time(NULL));
							std::cout << "отослал: " << currentOid -> getLastTimeRequest() << std::endl;
							break;
						}
						else {
							snmp_perror("snmp_send");
							snmp_free_pdu(req);
							break;
						}
					}	
				}
			}
		}
	}

	static void * checkHosts(void * object)
	{
		for(;;) {
			if(((AsyncSnmpManager *) object) -> getActiveHosts() == 0)
			{
				return NULL;
			}
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
		  
		  // sessSnmpDev -> ping_request = device -> getPingRequest();
		  sessSnmpDev -> commands = oidService -> getActiveOidsByDeviceId(sessSnmpDev -> id);
		  sessSnmpDev -> sqlReportBuffer =  sqlReportBuffer.get();
					 
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
		  sessSnmpDev -> session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV; // надо заменить на SNMP_SEC_LEVEL_AUTHPRIV

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


		   // Надо собрать блок для USM_PRIV_KU_LEN
		   // Пример взять отсюда 
		   //      # Do not generate_Ku, unless we're Auth or AuthPriv
           // unless @sess.securityLevel == Constants::SNMP_SEC_LEVEL_NOAUTH
          //   options[:auth_password] ||= options[:password]  # backward compatability
          //   if options[:username].nil? or options[:auth_password].nil?
          //     raise Net::SNMP::Error.new "SecurityLevel requires username and password"
          //   end
          //   if options[:username]
          //     @sess.securityName = FFI::MemoryPointer.from_string(options[:username])
          //     @sess.securityNameLen = options[:username].length
          //   end

          //   auth_len_ptr = FFI::MemoryPointer.new(:size_t)
          //   auth_len_ptr.write_int(Constants::USM_AUTH_KU_LEN)
          //   auth_key_result = Wrapper.generate_Ku(@sess.securityAuthProto,
          //                                    @sess.securityAuthProtoLen,
          //                                    options[:auth_password],
          //                                    options[:auth_password].length,
          //                                    @sess.securityAuthKey,
          //                                    auth_len_ptr)
          //   @sess.securityAuthKeyLen = auth_len_ptr.read_int

          //   if @sess.securityLevel == Constants::SNMP_SEC_LEVEL_AUTHPRIV
          //     priv_len_ptr = FFI::MemoryPointer.new(:size_t)
          //     priv_len_ptr.write_int(Constants::USM_PRIV_KU_LEN)

          //     # NOTE I know this is handing off the AuthProto, but generates a proper
          //     # key for encryption, and using PrivProto does not.
          //     priv_key_result = Wrapper.generate_Ku(@sess.securityAuthProto,
          //                                      @sess.securityAuthProtoLen,
          //                                      options[:priv_password],
          //                                      options[:priv_password].length,
          //                                      @sess.securityPrivKey,
          //                                      priv_len_ptr)
          //     @sess.securityPrivKeyLen = priv_len_ptr.read_int
          //   end

			// if (generate_Ku(sessSnmpDev -> session.securityAuthProto,
			// 			   sessSnmpDev -> session.securityAuthProtoLen,
			// 			   (u_char *) device -> getPassword().c_str(),device -> getPassword().length(),
			// 			   sessSnmpDev -> session.securityAuthKey,
			// 			   &(sessSnmpDev -> session.securityAuthKeyLen) ) != SNMPERR_SUCCESS) {
			// 	logService -> save(SNMP_LOG, "Error generating Ku from authentication pass phra");
			//     continue; 
		 //   }


		  // Незачем хранить устройства, с которыми нечего делать. Очисткой займуться умные указатели
		  if((sessSnmpDev -> commands) -> size() == 0) {
		  	continue;
		  }
		  
		  sessSnmpDev -> ss = snmp_open(&(sessSnmpDev->session));
		  if (!(sessSnmpDev -> ss) ) {
			logService -> save(SNMP_ASK_LOG, "something horrible happened!!!");
			continue;
		  }

		  std::shared_ptr<Oid>  oid = sessSnmpDev -> getNextCommand();
		  sessSnmpDev -> currentOid = oid -> getOid();
		  sessSnmpDev -> pdu = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
		  read_objid(oid -> getOid().c_str(), sessSnmpDev->anOID, &(sessSnmpDev ->  anOID_len ));
		  snmp_add_null_var(sessSnmpDev->pdu, sessSnmpDev->anOID, sessSnmpDev -> anOID_len);
		  if (snmp_send(sessSnmpDev->ss, sessSnmpDev->pdu)) 
		  {
		  	(sessSnmpDev -> commands) -> at(sessSnmpDev -> getIndexCurrentCommand()) -> setLastTimeRequest(time(NULL));
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

	~AsyncSnmpManager()
	{
		if(active_hosts != 0)
		{
			for(unsigned int i = 0; i < hosts->size(); i++)
			{
				snmp_close((hosts->at(i)) -> ss);
			}
			logService -> save(SNMP_MANAGER_STOP, "async manager is stoped!");
		}
		active_hosts = 0;
	}

};

void * starterAsyncSnmpManager(void * db)
{
	AsyncSnmpManager manager((sqlite3 *)db);
	manager.Run();
}

#endif

