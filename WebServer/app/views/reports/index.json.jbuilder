json.array!(@reports) do |report|
  json.extract! report, :id, :device_id, :context
  json.url report_url(report, format: :json)
end
