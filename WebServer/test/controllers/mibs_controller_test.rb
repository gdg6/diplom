require 'test_helper'

class MibsControllerTest < ActionController::TestCase
  setup do
    @mib = mibs(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:mibs)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create mib" do
    assert_difference('Mib.count') do
      post :create, mib: { file_name_mib: @mib.file_name_mib }
    end

    assert_redirected_to mib_path(assigns(:mib))
  end

  test "should show mib" do
    get :show, id: @mib
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @mib
    assert_response :success
  end

  test "should update mib" do
    patch :update, id: @mib, mib: { file_name_mib: @mib.file_name_mib }
    assert_redirected_to mib_path(assigns(:mib))
  end

  test "should destroy mib" do
    assert_difference('Mib.count', -1) do
      delete :destroy, id: @mib
    end

    assert_redirected_to mibs_path
  end
end
