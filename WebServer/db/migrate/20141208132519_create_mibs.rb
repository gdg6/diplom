class CreateMibs < ActiveRecord::Migration
  def change
    create_table :mibs do |t|
      t.string :file_name_mib, :nil=>false

      t.timestamps
    end
  end
end
