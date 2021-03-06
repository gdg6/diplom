#ifndef __SESS_SNMP_DEV__
#define __SESS_SNMP_DEV__

class SessSnmpDev {
public:
    int id;
	struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    std::string currentOid;
	std::shared_ptr<std::vector<std::shared_ptr<Oid>>> commands; // list oids
    oid anOID[MAX_OID_LEN];
    size_t  anOID_len = MAX_OID_LEN;
    SqlReportBuffer * sqlReportBuffer;
    int i;
    
    SessSnmpDev() : i(0)
    {
    	commands = NULL;
    }

	// FIXME must be use iterator
	std::shared_ptr<Oid> getNextCommand()
	{
		if (commands != NULL && commands -> size() != 0)
		{
			if(i >= commands -> size())
			{
				i = 0;
			}
			return (commands -> at(i++));
		}
		return NULL;
	}

	int getIndexCurrentCommand()
	{
		return (i == 0 ? 0 : (i - 1));
	}
};

#endif