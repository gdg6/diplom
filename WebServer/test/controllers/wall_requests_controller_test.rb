require 'test_helper'

class WallRequestsControllerTest < ActionController::TestCase
  setup do
    @wall_request = wall_requests(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:wall_requests)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create wall_request" do
    assert_difference('WallRequest.count') do
      post :create, wall_request: { request: @wall_request.request }
    end

    assert_redirected_to wall_request_path(assigns(:wall_request))
  end

  test "should show wall_request" do
    get :show, id: @wall_request
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @wall_request
    assert_response :success
  end

  test "should update wall_request" do
    patch :update, id: @wall_request, wall_request: { request: @wall_request.request }
    assert_redirected_to wall_request_path(assigns(:wall_request))
  end

  test "should destroy wall_request" do
    assert_difference('WallRequest.count', -1) do
      delete :destroy, id: @wall_request
    end

    assert_redirected_to wall_requests_path
  end
end
