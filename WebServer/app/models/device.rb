class Device < ActiveRecord::Base
	belongs_to :mib
  has_many :wall_raports
  validates :name, :length => {:maximum => 256}, :presence => true
  validates :description, :length => {:maximum => 1024}, :presence => true
  validates :room, :presence => true, :length => {:maximum => 25}
  validates :mac, :presence => true, :uniqueness => true
  validates :model, :presence => true
  validates :peername, :presence => true
  validates :port, :presence => true
  validates :login, :presence => true
  validates :password, :presence => true
end
