#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/library/transform_oids.h>

const char *snmp_v3_passphrase = "password";
const char *snmp_v3_privpass = "password";
// config_api.h по всей видимости работатет с mib-aми  - configure

//mib_api.h - init mib - read and parsing :

// net-snmp-config.h = legacy defines. различные define для работы snmp

// net-snmp-features.h - в зависимости от выставленных defines подрубает различгые библиотеки

// net-snmp-includes.h - подрубает как стандартные библиотеки так и snmp

// output_api.h - содержит процедуры логирования(loggin) и вывода сообщений с дебагом

//pdu_api.h   - содержит процедуры связанные с SNMP PDU (protocol data unit)

// session_api.h - содержит процедуры связанные с сессией - создание сессии, отправка и чтение ответов и много другое. в библиотеке все прокомментировано.

//snmpv3_api.h - подключение различных библеотек + библеотека для шифровантя md5

//types.h - содержит описание таких структур как snmp_pdu, snmp_session  и т.п.

//utilites.h - содержит только подключаемые библиотеки (утилиты из net-snmp/librry)

// varbind.h - cодержит такие функции как snmp_free_var print_value . Содержит процедуры имеющие отношение к связанным переменным и значениям к примеру: 
//netsnmp_variable_list *  snmp_pdu_add_variable(....);


//version.h - просто содержит метод для возрата версии. в примеру char * netsnmp_get_version(void);

//~ gcc -Wall -o "%e" "%f" -lnetsnmp

int main(int argc, char* argv[]) {
	struct snmp_session session, *ss;
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
           
	oid anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
   
	struct variable_list *vars;
	int status;

	init_snmp(argv[0]);
	snmp_sess_init(&session);
	session.peername = "localhost";
	session.version = SNMP_VERSION_3;
	session.securityName = strdup("snmpuser");
	session.securityNameLen = strlen(session.securityName);
	session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;
	session.securityAuthProto = usmHMACMD5AuthProtocol;
	session.securityAuthProtoLen = 
			sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
	session.securityAuthKeyLen = USM_AUTH_KU_LEN;

	if (generate_Ku(session.securityAuthProto, 
		session.securityAuthProtoLen, 
		(u_char *) snmp_v3_passphrase, strlen(snmp_v3_passphrase),
		session.securityAuthKey,
		&session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
		snmp_perror(argv[0]);
		snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase. \n");
		exit(1);
	}

	session.securityPrivProto = usmDESPrivProtocol;
	session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
	session.securityPrivKeyLen = USM_PRIV_KU_LEN;

	if (generate_Ku(session.securityAuthProto,
                        session.securityAuthProtoLen,
                        (u_char *) snmp_v3_privpass, strlen(snmp_v3_privpass),
                        session.securityPrivKey,
                        &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
		snmp_perror(argv[0]);
		snmp_log(LOG_ERR, "Error generating a key (Ku) from the supplied privacy pass phrase. \n");
		exit(1);
        }

	ss = snmp_open(&session);
	
	if (!ss) {
		snmp_perror("ack");
		snmp_log(LOG_ERR, "something horrible happened!!!\n");
		exit(2);
	}

	pdu = snmp_pdu_create(SNMP_MSG_GET);
	read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len);
	snmp_add_null_var(pdu, anOID, anOID_len);
	status = snmp_synch_response(ss, pdu, &response);

	if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {

		for(vars = response->variables; vars; vars = vars->next_variable)
			print_variable(vars->name, vars->name_length, vars);
		/* manipulate the information ourselves */
		for(vars = response->variables; vars; vars = vars->next_variable) {
			int count=1;
			if (vars->type == ASN_OCTET_STR) {
				char *sp = malloc(1 + vars->val_len);
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
	
	if (response)
		snmp_free_pdu(response);
	snmp_close(ss);

	return 0;
}

