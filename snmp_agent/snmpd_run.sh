#!/bin/bash
#sudo snmpd start  

sudo net-snmp-config --create-snmpv3-user -a password -x password -X DES snmpuser
