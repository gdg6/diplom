class StatisticsController < ApplicationController
  #~ before_action :check_auth
  #~ before_action :check_edit
  attr_reader :page, :results
  before_action :load_search_object
  attr_reader :device

  # GET /devices/new
  def new
	@device = nil
  end

  # POST /devices
  # POST /devices.json
  def create
    raise params.to_s
	if((id = params[:device_id]) <= 0 || (@device = Device.where(:id=>id)).nil?) 
		page = nil
		@results = []
		redirect_to statistic_path, notice: 'Нет такого устройства'
	end
	
	r_type = params[:type]
	if(r_type.nil? )
		redirect_to statistic_path, notice: 'Не задан тип ответа'
	end
	start_time = params[:start_time]
	end_time = params[:end_time]
	
	if(start_time.nil? && !end_time.nil?)
		@results = Report.where("device_id = ? AND r_type = ? AND created_at < ? ")
	end
	@results = Report.where("device_id = ? AND r_type = ? AND ").load
  end

  # DELETE /devices/1
  # DELETE /devices/1.json
  def destroy
    @device.destroy
    respond_to do |format|
      format.html { redirect_to devices_url, notice: 'Device was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    
  def check_edit
    render_error(root_path) unless User.edit?(@current_user)
  end
  
  def  load_search_object
	@device = Device.new
  end
end
