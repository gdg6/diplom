class WalkRaport < ActiveRecord::Base
    belongs_to :device
    before_destroy :fileDestroy

private
	def fileDestroy
		if File.exist?(self.path)
      		File.delete(self.path)
    	end
	end
end
