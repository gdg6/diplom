class CreateDevices < ActiveRecord::Migration
  def change
    create_table :devices do |t|
      t.string  :name, :nil => false
      t.string  :description, :limit => 1024
      t.string  :room, :limit => 25, :nil => false
      t.string  :mac, :unique => true, :nil => false
      t.string  :serial_number, :unique => true
      t.string  :model, :limit => 1024
      t.string  :peername, :nil => false;
      t.integer :port, :nil => false
      t.string  :login, :nil => false
      t.string  :password, :nil => false
      t.integer :mib_id, :nil => false

      t.timestamps
    end
  end
end
