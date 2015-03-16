#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>
#include <iostream>

int main(int argc, char ** argv)
{
  struct snmp_session session; 
  struct snmp_session *sess_handle;

  struct snmp_pdu *pdu;                   
  struct snmp_pdu *response;

  struct variable_list *vars;            

  oid id_oid[MAX_OID_LEN];
  oid serial_oid[MAX_OID_LEN];

  size_t id_len = MAX_OID_LEN;
  size_t serial_len = MAX_OID_LEN;

  int status;                             

  struct tree * mib_tree;
  struct tree * current;
	
  /*********************/

  if(argv[1] == NULL){
	printf("Please supply a hostname\n");
	exit(1);
  }

  init_snmp("APC Check");

  // snmp_sess_init( &session );
  //  session.version = SNMP_VERSION_1;
  //  session.community = "public";
  //  session.community_len = strlen(session.community);
  //  session.peername = argv[1];
  // sess_handle = snmp_open(&session);

  // add_mibdir("."); 
  mib_tree = read_mib("RFC2856.mib"); 
  if(mib_tree == NULL) {
  	  std::cout << "null" << std::endl;
  	  return 0;
  }

  current = mib_tree;

  for(;current != NULL; current = current -> next_peer) {
  	  std::cout << "label: " <<  (mib_tree -> label) << std::endl;
  	  std::cout << "status:" << (mib_tree -> status) << std::endl;
  	  std::cout << "type:" << (mib_tree -> type) << std::endl;	

  	  if(mib_tree -> hint != NULL) {
	  	  std::cout << "hint:" << (mib_tree -> hint) << std::endl;
  	  }

  	  if(mib_tree -> enums != NULL) {
  	  	std::cout << "enums -> value: " <<  mib_tree -> enums -> value << std::endl;
  	  }

  	  if(mib_tree -> reference != NULL) {
  	  	std::cout << "reference:" << (mib_tree -> reference) << std::endl;
  	  }
  	  std::cout << "mib_tree -> number_modules:" << (mib_tree -> number_modules) << std::endl; 


  	  if(mib_tree -> module_list != NULL) {
  	  	std::cout << "mib_tree-> module_list:" << *(mib_tree -> module_list) << std::endl;
  	  }

  	  if(mib_tree -> augments != NULL) {
  	  	std::cout << "augments:" << (mib_tree -> augments) << std::endl; 
  	  }
  	  std::cout << "#####################\n";
  }





  // pdu = snmp_pdu_create(SNMP_MSG_GET);


  // read_objid("NET-SNMP-MIB DEFINITIONS::upsBasicIdentModel.0", id_oid, &id_len);
  //  snmp_add_null_var(pdu, id_oid, id_len);

        
 //  status = snmp_synch_response(sess_handle, pdu, &response);
        
 //  for(vars = response->variables; vars; vars = vars->next_variable)
	// print_value(vars->name, vars->name_length, vars);

 //  snmp_free_pdu(response);
 //  snmp_close(sess_handle);
        
  return (0);
}