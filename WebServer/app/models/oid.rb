class Oid < ActiveRecord::Base
    belongs_to :device
	validates :ping_request, :presence => true
end
