class StatisticsController < ApplicationController
  before_action :check_auth
  attr_reader :page, :results
  attr_reader :device, :device_id
  attr_reader :view_id

  def new
    @device_id = params[:device_id].to_i
    if (!(@device = Device.find(@device_id)).nil?)
      return
    end
      redirect_to devices_path
  end

  def create
    if ( !(@device = Device.find(params[:device_id].to_i)).nil? ) 
        r_type  = Oid.where(:id => params[:r_type].to_i, :device_id => @device.id).first
        start_date = Time.new(params[:start_year_time].to_i, params[:start_month_time].to_i, params[:start_day_time].to_i)
        end_date = Time.new(params[:end_year_time].to_i, params[:end_month_time].to_i, params[:end_day_time].to_i, 23, 59, 59)

        if (@view_id = params[:view_id].to_i) == 1
          @results = Report.select(:context, :created_at).where("device_id =? AND r_type = ? AND created_at > ? AND created_at < ?", @device.id, r_type.oid, start_date, end_date).load
        else
          @results = Report.select(:r_type, :context, :created_at).where("device_id =? AND r_type = ? AND created_at > ? AND created_at < ?", @device.id, r_type.oid, start_date, end_date).page(params[:page]).load
        end
        render action: :printResult,  :results=>@results, :view_id => @view_id, :device => @device, :device_id => @device.id
        return
    end
    redirect_to :action => :new, :device_id => @device.id
  end

  def printResult
    @device_id = params[:device_id]
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
