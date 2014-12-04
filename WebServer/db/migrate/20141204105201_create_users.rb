class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :login, :unique=>true
      t.string :crypted_password
      t.string :salt

      t.timestamps
    end
  end
end
