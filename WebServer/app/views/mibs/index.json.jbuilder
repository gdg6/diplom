json.array!(@mibs) do |mib|
  json.extract! mib, :id, :file_name_mib
  json.url mib_url(mib, format: :json)
end
