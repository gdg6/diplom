#!/bin/bash
sudo snmpd start & 
snmpget -v 3 -a md5 -A password -x des -X password -u snmpuser -l priv localhost .1.3.6.1.2.1.1.3.0 &

