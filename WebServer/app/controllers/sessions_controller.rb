class SessionsController < ApplicationController

  skip_filter :load_current_user

  def new
    return redirect_to devices_path unless session[:user_id].nil?
  end

  def create
    @user=User.where("login=?", params[:login]).take
    if @user && @user.authenticate(params[:password])
      session[:user_id]=@user.id
      redirect_to root_path, notice: 'Вход выполнен'
    else
      flash[:danger]='Неверное имя пользователя или пароль'
      render :new
    end
  end

  def destroy
    session.delete(:user_id)
    redirect_to :login, notice: 'Выход выполнен'
  end
 end
