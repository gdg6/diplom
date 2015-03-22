class OidsController < ApplicationController
  before_action :set_oid, only: [:show, :edit, :update, :destroy]
  before_action :check_auth
  
  # GET /oids
  # GET /oids.json
  def index
    @oids = Oid.all
  end

  # GET /oids/1
  # GET /oids/1.json
  def show
  end

  # GET /oids/new
  def new
    @oid = Oid.new
  end

  # GET /oids/1/edit
  def edit
  end

  # POST /oids
  # POST /oids.json
  def create
    @oid = Oid.new(oid_params)

    respond_to do |format|
      if @oid.save
        format.html { redirect_to @oid, notice: 'Oid was successfully created.' }
        format.json { render :show, status: :created, location: @oid }
      else
        format.html { render :new }
        format.json { render json: @oid.errors, status: :unprocessable_entity }
      end
    end
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
      params.require(:oid).permit(:oid, :translate, :device_id, :active)
    end
end
