json.array!(@wall_requests) do |wall_request|
  json.extract! wall_request, :id, :request
  json.url wall_request_url(wall_request, format: :json)
end
