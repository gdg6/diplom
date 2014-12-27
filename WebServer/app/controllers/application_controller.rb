class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery with: :exception
  
  before_action :load_current_user
 
  
  private

  def load_current_user
    @current_user=User.where(id: session[:user_id]).take
  end

  def check_auth
    render_error(login_path,"Доступ без авторизации запрещен") unless @current_user
  end
  
  def render_error(url,msg="Доступ запрещен")
    flash[:danger]=msg
    redirect_to url
  end
end
