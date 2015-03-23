class OidsController < ApplicationController
  before_action :set_oid, only: [:show, :edit, :update, :destroy]
  before_action :check_auth
  attr_reader :device_id
  # GET /oids
  # GET /oids.json
  def index
    if (! (@device_id = params[:device_id]).nil?)
      @oids = Oid.where(:device_id=>@device_id.to_i).load
      return 
    end
    @oids = Oid.all
  end

  # GET /oids/1
  # GET /oids/1.json
  def show
  end

  # GET /oids/new
  # Can't create oid with empty device
  def new
    if (! (@device_id = params[:device_id]).nil?)
      @oid = Oid.new
      return
    end
    redirect_to root_path
  end

  # GET /oids/1/edit
  def edit
    @device_id = @oid.device_id
  end

  # POST /oids
  # POST /oids.json
  def create
    if(!(device = Device.find(params[:device_id].to_i)).nil? && !params[:oid].nil?)
      @oid = Oid.new
      @oid.oid = params[:oid][:oid].to_s
      @oid.translate = params[:oid][:translate].to_s
      @oid.active = params[:oid][:active] == true ? true : false
      @oid.device_id = device.id
      respond_to do |format|
        if @oid.save
          format.html { redirect_to @oid, notice: 'Oid was successfully created.' }
          format.json { render :show, status: :created, location: @oid }
        else
          format.html { render :new }
          format.json { render json: @oid.errors, status: :unprocessable_entity }
        end
      end
      return 
    end
    redirect_to :new, :device_id=>params[:device_id]
  end

  # PATCH/PUT /oids/1
  # PATCH/PUT /oids/1.json
  def update
    respond_to do |format|
      if @oid.update(oid_params)
        format.html { redirect_to @oid, notice: 'Oid was successfully updated.' }
        format.json { render :show, status: :ok, location: @oid }
      else
        format.html { render :edit }
        format.json { render json: @oid.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /oids/1
  # DELETE /oids/1.json
  def destroy
    @oid.destroy
    respond_to do |format|
      format.html { redirect_to oids_url, notice: 'Oid was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_oid
      @oid = Oid.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def oid_params
      params.require(:oid).permit(:oid, :translate, :active, :device_id)
    end
end
