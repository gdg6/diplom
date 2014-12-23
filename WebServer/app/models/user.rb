class User < ActiveRecord::Base
  ROLES=["Пользователь", "Администратор"]

  has_secure_password
  validates :login, uniqueness: true
  validates :password, length: {minimum: 6, allow_blank: true}
  validates :role, presence: true, inclusion: {in: 0...ROLES.size}
  
  before_validation :set_default_role

  def set_default_role
    self.role||=0
  end

  def admin?
    role==1
  end

  def self.edit?(u)
    u && u.admin?
    return 1
  end
end
