require 'net-snmp'     
       session = Net::SNMP::Session.open(:peername => "localhost", :version => '3', :security_level => Net::SNMP::Constants::SNMP_SEC_LEVEL_AUTHPRIV,
       :auth_protocol => :md5, :priv_protocol => :des, :username => "snmpuser", :auth_password => "password", :priv_password => "password") do |session|
      
        session.walk("sysORDescr") do |status, pdu|

          if status != nil
              status.each do |key, value|
                p key
                p value
              end
          else
            p  "something went wrong.  status is #{status} pdu #{pdu.to_s}"
          end
        end
      end
      # Net::SNMP::Dispatcher.select(false)  #Setting timeout to false causes dispatcher to block until data is ready
      session.close
