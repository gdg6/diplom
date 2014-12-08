class Mib < ActiveRecord::Base
  validates :file_name_mib, :presence => true, :length => {:maximum => 1024}
end
