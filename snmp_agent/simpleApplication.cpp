//~  Даннаый сорс - минимальный пример SNMP Agenta взятый с http://www.net-snmp.org/wiki/index.php/TUT:Simple_Application

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
//~ g++ -std=gnu++11 -Wall -o "%e" "%f" -lnetsnmp -Ofast -lpthread -pthread -lglut -lGL -lGLU -lOpenCL

//~ Используем SNMP_V_3
#define DEMO_USE_SNMP_VERSION_3

#ifdef DEMO_USE_SNMP_VERSION_3
#include <net-snmp/library/transform_oids.h>
	const char *our_v3_passphrase = "The Net-SNMP Demo Password";
#endif


int main(int argc, char ** argv) 
{


	//~ Структура, которая содержит информацию о том, кто мы собираемся говорить с.
	//~ Мы должны объявить 2 из них, один, чтобы заполнить информацию, и второй, который 
	//~ является указателем вернулся в библиотеку.
	struct snmp_session session, *ss;

	//~ Эта структура будет содержать все сведения, которые мы собираемся отправить на удаленный хост.
	//~ Мы объявим второй за информацию, что они собираются отправить обратно.
	//~ подъязычная: OID собирается провести размещение информации, которую мы хотим получить. Это понадобится размер, а также.
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	
	//~ OID собирается провести размещение информации, которую мы хотим получить
	oid anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;

	//~ This will hold the variables that we want to manipulate via SNMP
	struct variable_list *vars;
	int status;
	
	/*
    * Initialize the SNMP library
    */
   init_snmp("snmpapp");
   
   
    /*
    * Initialize a "session" that defines who we're going to talk to
    */
   snmp_sess_init( &session );
   char peername[] = "test.net-snmp.org";                   /* set up defaults */
   session.peername = peername;
   
   /* set up the authentication parameters for talking to the server */
   
      
   /* Use SNMPv3 to talk to the experimental server */
   
   /* set the SNMP version number */
   session.version=SNMP_VERSION_3;
        
   /* set the SNMPv3 user name */
   session.securityName = strdup("MD5User");
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
       snmp_perror(argv[0]);
       snmp_log(LOG_ERR,
                "Error generating Ku from authentication pass phrase. \n");
       exit(1);
   }
   
      
   /*
    * Open the session
    */
   ss = snmp_open(&session);                     /* establish the session */
   
   if (!ss) {
       snmp_perror("ack");
       snmp_log(LOG_ERR, "something horrible happened!!!\n");
       exit(2);
   }
   
    /*
    * Create the PDU for the data for our request.
    *   1) We're going to GET the system.sysDescr.0 node.
    */
   pdu = snmp_pdu_create(SNMP_MSG_GET);
   
   read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len);
   
#if OTHER_METHODS
   get_node("sysDescr.0", anOID, &anOID_len);
   read_objid("system.sysDescr.0", anOID, &anOID_len);
#endif
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
   
}
