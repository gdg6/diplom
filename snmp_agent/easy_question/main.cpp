#include <iostream>
#include "libSqlite3IO.cpp"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include <net-snmp/library/transform_oids.h>


#define INIT_NAME "snmpapp"
#define PEERNAME "test.net-snmp.org"

#define ERR_CREATE -1
#define ERR_OPEN -2


class SnmpAgent
{
private:   
   struct snmp_session session, * ss;
   struct snmp_pdu * pdu;
   struct snmp_pdu * response;
   struct variable_list * vars;
   oid anOID[MAX_OID_LEN];
   size_t  anOID_len = MAX_OID_LEN;
   
   int status;
   int status_obj;
   
public: 
    SnmpAgent(char * address, char * userName, 
              const char * snmp_v3_passphrase,
              const char * snmp_v3_privpass ) : status(0), status_obj(0)
    {
	init_snmp(INIT_NAME);
        snmp_sess_init(&session);
        session.peername = address;
        session.version = SNMP_VERSION_3;
        session.securityName =  strdup(userName);
        session.securityNameLen = strlen(session.securityName);
        session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;
        /* set the authentication method to MD5 */
        session.securityAuthProto = usmHMACMD5AuthProtocol;
        session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)
								/sizeof(oid);
        session.securityAuthKeyLen = USM_AUTH_KU_LEN;

        /* set the authentication key to a MD5 hashed version of our
        passphrase "The Net-SNMP Demo Password" (which must be at least 8
        characters long) */
         if (generate_Ku(session.securityAuthProto,
                session.securityAuthProtoLen,
                (u_char *) snmp_v3_passphrase, strlen(snmp_v3_passphrase),
                session.securityAuthKey,
                &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
                snmp_perror(INIT_NAME);
                snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase. \n");
                status = ERR_OPEN;
        }

        session.securityPrivProto = usmDESPrivProtocol;
        session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
        session.securityPrivKeyLen = USM_PRIV_KU_LEN;
        
        if(!status)
          if (generate_Ku(session.securityAuthProto,
                          session.securityAuthProtoLen,
                          (u_char *) snmp_v3_privpass, strlen(snmp_v3_privpass),
                          session.securityPrivKey,
                          &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
                  snmp_perror(INIT_NAME);
                  snmp_log(LOG_ERR, "Error generating a key (Ku) from the supplied privacy pass phrase. \n");
                 status = ERR_OPEN;
          }


   }

   int openSession() {
	ss = snmp_open(&session);
        if(!ss) {
	   snmp_log(LOG_ERR, "Open session failed");
           status = ERR_OPEN;
           return status;
        }
        return status;
   }

   snmp_pdu *  sendResponce(const char * input)
   {
      pdu = snmp_pdu_create(SNMP_MSG_GET);
      read_objid(input, anOID, &anOID_len);
      snmp_add_null_var(pdu, anOID, anOID_len);
      /*
      * Send the Request out.
      */
     status = snmp_synch_response(ss, pdu, &response);
     return response;
   }

   int getStatus()
   {
     return status;
   }

   void printResponse(struct snmp_pdu * response)
   {
      if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
         for(vars = response->variables; vars; vars = vars->next_variable)
                  print_variable(vars->name, vars->name_length, vars);
                /* manipulate the information ourselves */
          for(vars = response->variables; vars; vars = vars->next_variable) {
            int count=1;
            if (vars->type == ASN_OCTET_STR) {
               char *sp = (char *) malloc(1 + vars->val_len);
               memcpy(sp, vars->val.string, vars->val_len);
               sp[vars->val_len] = '\0';
               printf("value #%d is a string: %s\n", count++, sp);
               free(sp);
            } else
               printf("value #%d is NOT a string! Ack!\n", count++);
        }
      } else {
      /*
       * FAILURE: print what went wrong!
       */
       if (status == STAT_SUCCESS)
          fprintf(stderr, "Error in packet\nReason: %s\n",
      		   snmp_errstring(response->errstat));
       else
          snmp_sess_perror("snmpget", ss);
      }
   }

   ~SnmpAgent()
   {
    if(response)
       snmp_free_pdu(response);
    snmp_close(ss);
    SOCK_CLEANUP; 
   }


};


int main()
{
 SnmpAgent *  agent  = new SnmpAgent("localhost", "snmpuser", "password", "password");
 if(agent->getStatus() != 0) {
  std::cout << "fail status" << std::endl;
  return -2;
 } 
 if(agent->openSession() != 0) {
   std::cout << "fail open" << std::endl;
   return -3;
 }
 struct snmp_pdu * response = NULL;
 response =  agent -> sendResponce(".1.3.6.1.2.1.1.1.0");
 if(!response) {
    std::cout << "ok response" << std::endl;
    agent -> printResponse(response);
 } else {
    std::cout << "response fail" << std::endl;
 }
 delete agent;
return 0;
}
