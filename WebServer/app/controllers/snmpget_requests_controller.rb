class SnmpgetRequestsController < ApplicationController
  before_action :check_auth
  attr_reader :result, :device_id, :snmp_request

  def new
  	@device_id = params[:device_id].to_i
  end

  def create

  	oid = params[:oid].nil? ? "" : params[:oid][0].to_s 
  	@device_id = params[:device_id].to_i
  	@snmp_request = ""
  	@result = ""
  	device = nil
    device = Device.find(@device_id) if @device_id > 0

  	if(device != nil)
  		@snmp_request = "snmpget -v 3 -a md5 -A #{device.password} des -X #{device.password} -u #{device.login} -l priv #{device.peername} #{oid}"
		  session = Net::SNMP::Session.open(:peername => device.peername, :version => '3', :security_level => Net::SNMP::Constants::SNMP_SEC_LEVEL_AUTHPRIV,
		    :auth_protocol => :md5, :priv_protocol => :des, :username => device.login, :auth_password => device.password, :priv_password => device.password) do |session|
		  session.get([oid]) do |status, pdu|

		      if status == :success
		        @result = pdu.varbinds[0].value
		      else
		        @result = "something went wrong.  status is #{status} pdu #{pdu.to_s}"
		      end
		    end
		  end
    
      Net::SNMP::Dispatcher.select(false) 
		  session.close
      return render :action=>:show, :result=>@result, :request=>@snmp_request, :device_id=>@device_id
    end
    redirect_to :controller=>"devices", :action=>"index", :error=>"ошибка при генерации запроса"
  end

  def show
  end

end