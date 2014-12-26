class MibsController < ApplicationController
  before_action :set_mib, only: [:show, :edit, :update, :destroy]
  before_action :check_auth
  before_action :check_edit
  # GET /mibs
  # GET /mibs.json
  def index
    @mibs = Mib.all
  end

  # GET /mibs/1
  # GET /mibs/1.json
  def show
  end

  # GET /mibs/new
  def new
    @mib = Mib.new
  end

  # GET /mibs/1/edit
  def edit
  end

  # POST /mibs
  # POST /mibs.json
  def create
    @mib = Mib.new(mib_params)

    respond_to do |format|
      if @mib.save
        format.html { redirect_to @mib, notice: 'Mib was successfully created.' }
        format.json { render :show, status: :created, location: @mib }
      else
        format.html { render :new }
        format.json { render json: @mib.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /mibs/1
  # PATCH/PUT /mibs/1.json
  def update
    respond_to do |format|
      if @mib.update(mib_params)
        format.html { redirect_to @mib, notice: 'Mib was successfully updated.' }
        format.json { render :show, status: :ok, location: @mib }
      else
        format.html { render :edit }
        format.json { render json: @mib.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /mibs/1
  # DELETE /mibs/1.json
  def destroy
    @mib.destroy
    respond_to do |format|
      format.html { redirect_to mibs_url, notice: 'Mib was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_mib
      @mib = Mib.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def mib_params
      params.require(:mib).permit(:file_name_mib)
    end
    
    def check_edit
      render_error(root_path) unless User.edit?(@current_user)
    end
end
