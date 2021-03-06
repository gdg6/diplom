class CreateOids < ActiveRecord::Migration
  def change
    create_table :oids do |t|
      t.belongs_to :device, null: false,  :index=>true
      t.string :oid
      t.string :translate
      t.integer :ping_request, :null => false
      t.boolean :active, null: false

      t.timestamps
    end
  end
end
