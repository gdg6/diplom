Rails.application.routes.draw do
  resources :walk_raports

  resources :walk_requests

  resources :oids

  resources :reports

  resources :mibs

  resources :devices

  resources :users

  root :to => "devices#index"

  
  get 'login'=>'sessions#new', as: :login
  post 'login'=>'sessions#create'
  get 'logout'=>'sessions#destroy', as: :logout
  
  get 'search_statistic' => 'statistics#new', as: :search
  post 'search_statistic' => 'statistics#create'

  get 'snmpget_request' => 'snmpget_requests#new', as: :snmp_get
  post 'snmpget_request' => 'snmpget_requests#create'

  
end
