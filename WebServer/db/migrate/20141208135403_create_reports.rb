class CreateReports < ActiveRecord::Migration
  def change
    create_table :reports do |t|
      t.integer :device_id, :nil=>false
      t.text :context

      t.timestamps
    end
  end
end
