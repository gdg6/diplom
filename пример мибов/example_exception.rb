class RetryException < StandardError
  attr :can_retry
  def initialize(rtr = true)
    @can_retry = rtr
  end
end

def exception_handling
  begin # Если бы не было else, то возвращалось бы последнее значение begin
    yield if block_given? 
  rescue ZeroDivisionError # Если програму запускает не Чак Норрис
    "На ноль делить нельзя"
  rescue RetryException => re # Возврат перезапускает блок begin
    puts "Не получилось, но мы попробуем снова"
    if re.can_retry
      retry
    else
      "Теперь точно не получилось"
    end
  rescue # Здесь ловится всё остальное
    puts "Случилось непредвиденное: #{$!.inspect}"
    raise
  else # Если всё прошло без ошибок
    "Всё прошло без ошибок"
  ensure # Этот блок выполняется в любом случае
    puts "Процесс окончен, но эта часть ничего не возвращает"
    "В любом случае" # Этой строки мы нигде не увидим
  end
end

blocks = [] # Массив с блоками

blocks.push(lambda {})
blocks.push(lambda { 1/0 })
blocks.push(lambda do
  @retry ||= 3 # Пробуем ещё раз не более трёх раз
  @retry -= 1
  raise RetryException.new(@retry > 0), "Временная ошибка"
end)
blocks.push(lambda { raise "Неведомая ошибка" })

blocks.each do |block|
  puts "Возвратилось: #{exception_handling(&block) rescue "Ошибка!"}"
  puts "------------------------"
end