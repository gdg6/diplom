class CreateOids < ActiveRecord::Migration
  def change
    create_table :oids do |t|
      t.string :oid
      t.string :translate

      t.timestamps
    end
  end
end
