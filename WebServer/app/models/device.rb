class Device < ActiveRecord::Base
	belongs_to :mib
  validates :name, :length => {:maximum => 256}, :presence => true
  validates :description, :length => {:maximum => 1024}, :presence => true
  validates :room, :presence => true, :length => {:maximum => 25}
  validates :mac, :presence => true, :uniqueness => true
  validates :model, :presence => true
  validates :port, :presence => true
end
