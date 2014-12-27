require 'test_helper'

class OidsControllerTest < ActionController::TestCase
  setup do
    @oid = oids(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:oids)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create oid" do
    assert_difference('Oid.count') do
      post :create, oid: { oid: @oid.oid, translate: @oid.translate }
    end

    assert_redirected_to oid_path(assigns(:oid))
  end

  test "should show oid" do
    get :show, id: @oid
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @oid
    assert_response :success
  end

  test "should update oid" do
    patch :update, id: @oid, oid: { oid: @oid.oid, translate: @oid.translate }
    assert_redirected_to oid_path(assigns(:oid))
  end

  test "should destroy oid" do
    assert_difference('Oid.count', -1) do
      delete :destroy, id: @oid
    end

    assert_redirected_to oids_path
  end
end
