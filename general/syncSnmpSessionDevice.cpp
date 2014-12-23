#ifndef __SNMP_SESSION_DEVICE__
#define __SNMP_SESSION_DEVICE__

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <iostream>
#include <memory>
#include <thread>

 /* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
 #define DEMO_USE_SNMP_VERSION_3
 
 #ifdef DEMO_USE_SNMP_VERSION_3
 #include <net-snmp/library/transform_oids.h>
 const char *our_v3_passphrase = "password";
 #endif

char peername[] = "localhost";

 class SnmpSessionDevice
 {
 	struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    struct snmp_pdu *response;
    oid anOID[MAX_OID_LEN];
    size_t anOID_len = MAX_OID_LEN;
    struct variable_list *vars;
    int status;

    std::string currentAnswer;


 public:
 	 SnmpSessionDevice(){
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

   void openSession(){
  	  ss = snmp_open(&session);

  	  if (!ss) {
  	    snmp_perror("ack");
  	    snmp_log(LOG_ERR, "something horrible happened!!!\n");
  	    exit(2);
  	  }
    }

   std::shared_ptr<std::string>  responseToDevice(char * oid) {
   currentAnswer = "";
   /*
    * Create the PDU for the data for our request.
    *   1) We're going to GET the system.sysDescr.0 node.
    */
   pdu = snmp_pdu_create(SNMP_MSG_GET);

   read_objid(oid, anOID, &anOID_len);
   
// #if OTHER_METHODS
//    get_node("sysDescr.0", anOID, &anOID_len);
//    read_objid("system.sysDescr.0", anOID, &anOID_len);
// #endif

   snmp_add_null_var(pdu, anOID, anOID_len);

  /*
    * Send the Request out.
    */
   status = snmp_synch_response(ss, pdu, &response);
   

      /*
    * Process the response.
    */
   if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
     /*
      * SUCCESS: Print the result variables
      */

    for(vars = response->variables; vars; vars = vars->next_variable)
     print_variable(vars->name, vars->name_length, vars);
       

        /* manipulate the information ourselves */
     for(vars = response->variables; vars; vars = vars->next_variable) {
       int count=1;
       if (vars->type == ASN_OCTET_STR) {
         currentAnswer += (char*)vars->val.string;
         std::cout << currentAnswer;
       }
       else
         printf("value #%d is NOT a string! Ack!\n", count++);
     }
       
       } else {
     /*
      * FAILURE: print what went wrong!
      */
    
     if (status == STAT_SUCCESS)
       fprintf(stderr, "Error in packet\nReason: %s\n", snmp_errstring(response->errstat));
     else
       snmp_sess_perror("snmpget", ss);
   }
   	   if (response) {
	      snmp_free_pdu(response);
   	   	response = NULL;
   	   }

    return std::shared_ptr<std::string>(new std::string(currentAnswer.c_str()));
 }


 	~ SnmpSessionDevice()
 	{
       /*
	    * Clean up:
	    *  1) free the response.
	    *  2) close the session.
	    */
	   if (response)
	     snmp_free_pdu(response);
	   snmp_close(ss);
	    
	   /* windows32 specific cleanup (is a noop on unix) */
	   SOCK_CLEANUP;
 	}
 	
 };

#endif

int main()
{
SnmpSessionDevice snmp;
snmp.openSession();
snmp.responseToDevice(".1.3.6.1.4.1.2021.10.1.3.1");
}

