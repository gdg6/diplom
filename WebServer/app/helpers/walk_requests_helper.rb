module WalkRequestsHelper

  def type_select()
  	res = []
  	WalkRequest.all.each{|t| res << [t.id, t.request]}
  	return res
  end

end
