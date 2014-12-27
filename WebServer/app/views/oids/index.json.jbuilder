json.array!(@oids) do |oid|
  json.extract! oid, :id, :oid, :translate
  json.url oid_url(oid, format: :json)
end
