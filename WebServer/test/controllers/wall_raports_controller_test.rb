require 'test_helper'

class WallRaportsControllerTest < ActionController::TestCase
  setup do
    @wall_raport = wall_raports(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:wall_raports)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create wall_raport" do
    assert_difference('WallRaport.count') do
      post :create, wall_raport: { path: @wall_raport.path, user_id: @wall_raport.user_id }
    end

    assert_redirected_to wall_raport_path(assigns(:wall_raport))
  end

  test "should show wall_raport" do
    get :show, id: @wall_raport
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @wall_raport
    assert_response :success
  end

  test "should update wall_raport" do
    patch :update, id: @wall_raport, wall_raport: { path: @wall_raport.path, user_id: @wall_raport.user_id }
    assert_redirected_to wall_raport_path(assigns(:wall_raport))
  end

  test "should destroy wall_raport" do
    assert_difference('WallRaport.count', -1) do
      delete :destroy, id: @wall_raport
    end

    assert_redirected_to wall_raports_path
  end
end
