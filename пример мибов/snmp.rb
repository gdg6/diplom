require 'net-snmp'
# snmpget -v 3 -a md5 -A password -x des -X password -u snmpuser -l priv localhost .1.3.6.1.2.1.1.3.0
session = Net::SNMP::Session.open(:peername => 'localhost', :version => '3', :security_level => Net::SNMP::Constants::SNMP_SEC_LEVEL_AUTHPRIV,
 :auth_protocol => :md5, :priv_protocol => :des, :username => "snmpuser", :auth_password => "password", :priv_password => "password") do |session|
  session.walk("system") do |status, pdu|

    if status != nil
        status.each do |key, value|
    			p "==="
    			puts key
    			puts value
  		  end
    else
      puts "something went wrong.  status is #{status} pdu #{pdu.to_s}"
    end
  end
end
session.close


