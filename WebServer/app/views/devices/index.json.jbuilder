json.array!(@devices) do |device|
  json.extract! device, :id, :name, :description, :room, :mac, :serial_number, :model, :port, :mib_id
  json.url device_url(device, format: :json)
end
