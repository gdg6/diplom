class CreateLogs < ActiveRecord::Migration
  def change
    create_table :logs do |t|
      t.integer :l_type
      t.text :context

      t.timestamps
    end
  end
end
