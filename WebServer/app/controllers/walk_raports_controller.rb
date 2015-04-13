require 'net-snmp'

class WalkRaportsController < ApplicationController
  before_action :set_walk_raport, only: [:show, :edit, :update, :destroy]
  before_action :check_auth

  attr_reader :device, :device_id, :snmpwalk_request, :snmpwalk_type_request, :result
  attr_reader :errors
  # GET /walk_raports
  # GET /walk_raports.json

  def index
    @walk_raports = WalkRaport.all
  end

  # GET /walk_raports/1
  # GET /walk_raports/1.json
  def show
    @result = ""
    if (!params[:id].nil?)
      @walk_raport = WalkRaport.find(params[:id].to_i)
      @result = File.read(@walk_raport.path)
    end
  end

  # GET /walk_raports/new
  def new
    @device_id = params[:device_id].to_i
    @device = nil
    begin
      device = Device.find(@device_id).take
    rescue

    end
  end

  # GET /walk_raports/1/edit
  def edit
  end

  # POST /walk_raports
  def create
    @device_id = params[:device_id].to_i
    @device = Device.find(@device_id)
    @snmpwalk_type_request = params[:walk_request_id].nil? ? nil :  WalkRequest.find(params[:walk_request_id].to_i)
    @result = ""
    user = User.find(session[:user_id].to_i)
    t = Time.new
    if (@device != nil && @snmpwalk_type_request != nil)
      @result = "####\nCreate by #{user.login} #{t} \nsnmpwalk -v 3 -a md5 -A #{@device.password} -x des -X #{@device.password} -u #{@device.login} -l priv #{@device.peername}:#{@device.port} #{@snmpwalk_type_request.request}\n####\n"
      begin
        session = Net::SNMP::Session.open(
          :peername => @device.peername, 
          :port=>@device.port, 
          :version => '3', 
          :security_level => Net::SNMP::Constants::SNMP_SEC_LEVEL_AUTHPRIV,
          :auth_protocol => :md5,
          :priv_protocol => :des, 
          :username => @device.login, 
          :auth_password => @device.password, 
          :priv_password => @device.priv_password) do |session|  
          session.walk(@snmpwalk_type_request.request.to_s) do |status, pdu|

            if !status.nil?
              status.each{|key, value|  @result += "===\nkey: #{key.to_s}\nvalue: #{value.to_s}\n" }
            else
              @result = "something went wrong.  status is #{status} pdu #{pdu.to_s}"
            end
          end
        end

        session.close
        # must be create dir for save raports
        file_path = "raports/#{@device.mac}_walk_#{@snmpwalk_type_request.request}_#{t.year.to_s}-#{t.month.to_s}-#{t.day.to_s}_#{t.hour.to_s}:#{t.min.to_s}:#{t.sec.to_s}";
        file = File.new(file_path, "w+")
        file.puts(@result)
        file.close
        @walk_raport = WalkRaport.new({"path"=>file_path, "device_id"=>@device.id})
        if @walk_raport.save
          return render :action=>:show, :device_id=>@device_id, :result=>@result
        end
        rescue     
          return render :action=>:show, :device_id=>@device_id, :result=>"При генерации произошла ошибка"
      end
    end
    redirect_to devices_path
  end

  # PATCH/PUT /walk_raports/1
  # PATCH/PUT /walk_raports/1.json
  def update
    respond_to do |format|
      if @walk_raport.update(walk_raport_params)
        format.html { redirect_to @walk_raport, notice: 'Walk raport was successfully updated.' }
        format.json { render :show, status: :ok, location: @walk_raport }
      else
        format.html { render :edit }
        format.json { render json: @walk_raport.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /walk_raports/1
  def destroy
    @walk_raport.destroy
    respond_to do |format|
      format.html { redirect_to "/devices/#{@walk_raport.device_id}", notice: 'Walk raport was successfully destroyed.', errors: @errors }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_walk_raport
      @walk_raport = WalkRaport.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def walk_raport_params
      params.require(:walk_raport).permit(:path, :device_id)
    end

end
