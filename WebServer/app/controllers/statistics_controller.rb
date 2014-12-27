class StatisticsController < ApplicationController
  #~ before_action :check_auth
  #~ before_action :check_edit
  attr_reader :page, :results
  # before_action :load_search_object
  attr_reader :device

  def new
	  @device = nil
    if(!params[:r_type].nil? )
      if(!params[:device_id].nil?)
          @view_id = params[:view_id].to_i # if nil then 0
          @device = Device.where(:id=>params[:device_id].to_i).first
          @results = [] if @view_id.nil?
          @results = Report.where("device_id = ? AND r_type = ?", params[:device_id].to_i, params[:r_type].to_s).load if @view_id.to_i == 1
          @results = Report.where("device_id = ? AND r_type = ?", params[:device_id].to_i, params[:r_type].to_s).page(params[:page]).load if @view_id.to_i == 2
      end
    end
  end

  # я не дообрабатывал все исключения
  def create
      id = 0
      r_type = "" 
  	if((id = params[:device_id]).to_i <= 0 || (@device = Device.where(:id=>id).load).nil?) 
  		page = nil
  		@results = []
  		redirect_to statistic_path, notice: 'Нет такого устройства'
  	end
  	
  	r_type = params[:type]
  	if(r_type.nil? )
  		redirect_to statistic_path, notice: 'Не задан тип ответа'
  	end

    redirect_to  :action=>'new',  device_id: id, r_type: r_type, view_id: params[:view_id]
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
  
  def statistic_params
     params.require(:statistic).permit(:device_id, :r_type)
  end

  def  load_search_object
	  @device = Device.new
  end
end
