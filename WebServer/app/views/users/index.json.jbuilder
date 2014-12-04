json.array!(@users) do |user|
  json.extract! user, :id, :login, :crypted_password, :salt
  json.url user_url(user, format: :json)
end
