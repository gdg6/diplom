json.array!(@wall_raports) do |wall_raport|
  json.extract! wall_raport, :id, :path, :user_id
  json.url wall_raport_url(wall_raport, format: :json)
end
