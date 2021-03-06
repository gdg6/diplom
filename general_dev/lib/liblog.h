#ifndef __LIB_LOG__
#define __LIB_LOG__

// #define SMI_INTEGER     ASN_INTEGER
// #define SMI_STRING      ASN_OCTET_STR
// #define SMI_OBJID       ASN_OBJECT_ID
// #define SMI_NULLOBJ     ASN_NULL
// #define SMI_IPADDRESS   ASN_IPADDRESS
// #define SMI_COUNTER32	    ASN_COUNTER
// #define SMI_GAUGE32	    ASN_GAUGE
// #define SMI_UNSIGNED32 SMI_GAUGE32
// #define SMI_TIMETICKS   ASN_TIMETICKS
// #define SMI_OPAQUE ASN_OPAQUE
// #define SMI_COUNTER64   ASN_COUNTER64


// For app code
#define APP_OK 0
#define BAD_ALLOC -1


// Type of logs
#define RUN_SNMP_DEAMON_LOG "RUN SNMP MANAGER"
#define BAD_ALLOC_LOG "BAD_ALLOC"
#define SNMP_LOG "SNMP LOG" 
#define SNMP_ASK_LOG "ASK SNMP"
#define SELECT_LOG "SELECT"
#define SNMP_INIT_SESSION "SNMP INIT SESSION"
#define SNMP_MANAGER_STOP "MANAGER STOP"
#define SNMP_MANAGER_RESTART "MANAGER RESTART"

#endif