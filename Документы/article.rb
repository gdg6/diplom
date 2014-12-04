#пример сериализованного поля. 

class PresenceCheck
  def check(value)
    !value.blank?
  end
end

class YearCheck
  def check(value)
    year = value.to_i
    year > 0 &&  year <= Date.today.year
  end
end

class BooleanCheck
  def check(value)
    [true, false].include?(value)
  end
end

class SourceValidator < ActiveModel::Validator
  def initialize(value)
    @checks = {
      name: [PresenceCheck.new],
      year: [PresenceCheck.new, YearCheck.new],
      output: [PresenceCheck.new],
      is_russian: [BooleanCheck.new], 
      in_rinc: [BooleanCheck.new],
	   type: []
    }
  end

  def validate(record)
    result = ""
	if record.source.present?
	   	if !record.source.has_key?(:type) 
			record.errors[:source] << "Тип докумнта не определен"
		else
			case record.source[:type]
			when 'журнал'
				  result = validates_magazine(record.source) 
				if !result.blank?
					record.errors[:source] << "Неверное значение аттрибута #{result}"	
				end	  		  
			when 'сборник трудов'
				result = validates_papers(record.source) 
				if !result.blank?
					record.errors[:source] << "Неверное значение аттрибута #{result}"
		    end 	
			else
			  record.errors[:source] << 'Неверное значение типа документа'	
			end	
		end
	else
		record.errors[:source] << 'Отсутствует источник'			 
	end 
  end
  
  private
  
  def validates_magazine(hash)
    required_keys=[:name, :year, :type, :output, :is_russian, :in_rinc]
    
    required_keys.each do |key|
    	if !hash.has_key?(key)
    		return "#{key} (missed attribute)"
      end			
    end
  	hash.each do |key, value|
  	  if key.kind_of?(String)
  		  value = value.strip 
  		  hash[key] = value.gsub(/\s+/,' ')
  		end
		  @checks[key].each do |check| 
  		  return key unless check.check(value)

  		end
  	end
 	  return ''
  end
  
  def validates_papers(hash)
    required_keys=[:name, :year, :type]
    required_keys.each do |key|
    	if !hash.has_key?(key)
    		return "#{key} (пропущенный атрибут)"
      end			
    end
  	hash.each do |key, value|
  	  if key.kind_of?(String)
  		  value = value.strip 
  		  hash[key] = value.gsub(/\s+/,' ')
  	  end  
	    @checks[key].each do |check| 
  		  return key unless check.check(value)
  	  end
  	end
  	return ''
  end
   
end

class Article < ActiveRecord::Base
	include StringStrip
	
	validates_with SourceValidator
	before_validation :copy_year
	before_validation :convert_value_in_rinc_is_russian

	has_many :documents, :as=> :owner
	has_one :report	
	has_many :article_authors
	has_paper_trail
	serialize :source
	serialize :creator_data
	validates :name, presence: true, uniqueness: true
	validates :start_page, :finish_page, :year,
	      numericality: { only_integer: true, greater_than: 0, allow_nil: false }
	validates :year, numericality: {less_than_or_equal_to: Date.today.year} 
	   
	private 
	
	def copy_year
		if self.source.present?
			self.year=self.source[:year]
		else
			self.errors[:source] << 'Missed source value!'		
		end
	end

    def convert_value_in_rinc_is_russian
		if self.source.present?
            
			if self.source[:in_rinc]!=false && !self.source[:in_rinc].nil?
				self.source[:in_rinc] = true 
            else
				self.source[:in_rinc] = false
			end
			if self.source[:is_russian]!=false && !self.source[:is_russian].nil?
            	self.source[:is_russian] = true 
			else
				self.source[:is_russian] = false
			end
		else
			self.errors[:source] << 'Missed source value!'		
		end
    end
end
