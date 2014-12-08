class Report < ActiveRecord::Base
  validates :device_id, :presence => true
  validates :context, :presence=>true, :length => {:maximum => 1024}, allow_blank: true
end
