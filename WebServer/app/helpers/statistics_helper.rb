module StatisticsHelper

  def listExecuteOidForDevice(device_id)
  	result = []
		Oid.where(:device_id => device_id).each{|x| result << ["#{x.oid} (#{x.translate})" , x.id]}
  	return result
  end

  def search_select()
	  res = []
	  Device.all.each{|d| res << [d.name, d.id]}	
    return res
  end

  def type_select()
  	res = []
  	Oid.all.each{|t| res << [t.translate, t.oid]}
  	return res
  end
  
  def day_select()
  	res = []
    	for i in 1..31
    		res << [i, i]
    	end	
  	return res
  end
  
  def month_select()
  	res = []
    	for i in 1..12
    		res << [i, i]
    	end	
  	return res
  end

  def year_select()
  	res = []
    	for i in (Time.now.year - 10) ..Time.now.year
    		res << [i, i]
    	end	
  	return res
  end

end
