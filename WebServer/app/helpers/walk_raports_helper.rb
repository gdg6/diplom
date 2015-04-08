module WalkRaportsHelper
 
  def snmpwall_type_select()
  	res = []
  	WalkRequest.all.each{|t| res << [t.request+"  (#{t.description})", t.id]}
  	return res
  end
  
end
