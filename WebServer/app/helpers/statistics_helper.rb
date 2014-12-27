module StatisticsHelper

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
	for i in 1992..Time.now.year
		res << [i, i]
	end	
	return res
  end

end
