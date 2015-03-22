class WalkRequestsController < ApplicationController
  before_action :set_walk_request, only: [:show, :edit, :update, :destroy]
  before_action :check_auth
  # GET /walk_requests
  # GET /walk_requests.json
  def index
    @walk_requests = WalkRequest.all
  end

  # GET /walk_requests/1
  # GET /walk_requests/1.json
  def show
  end

  # GET /walk_requests/new
  def new
    @walk_request = WalkRequest.new
  end

  # GET /walk_requests/1/edit
  def edit
  end

  # POST /walk_requests
  # POST /walk_requests.json
  def create
    @walk_request = WalkRequest.new(walk_request_params)

    respond_to do |format|
      if @walk_request.save
        format.html { redirect_to @walk_request, notice: 'walk request was successfully created.' }
        format.json { render :show, status: :created, location: @walk_request }
      else
        format.html { render :new }
        format.json { render json: @walk_request.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /walk_requests/1
  # PATCH/PUT /walk_requests/1.json
  def update
    respond_to do |format|
      if @walk_request.update(walk_request_params)
        format.html { redirect_to @walk_request, notice: 'walk request was successfully updated.' }
        format.json { render :show, status: :ok, location: @walk_request }
      else
        format.html { render :edit }
        format.json { render json: @walk_request.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /walk_requests/1
  # DELETE /walk_requests/1.json
  def destroy
    @walk_request.destroy
    respond_to do |format|
      format.html { redirect_to walk_requests_url, notice: 'walk request was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_walk_request
      @walk_request = WalkRequest.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def walk_request_params
      params.require(:walk_request).permit(:request)
    end
end
