json.array!(@logs) do |log|
  json.extract! log, :id, :l_type, :context
  json.url log_url(log, format: :json)
end
