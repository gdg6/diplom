class CreateWalkRaports < ActiveRecord::Migration
  def change
    create_table :walk_raports do |t|
      t.string :path
      t.belongs_to :device, null: false,  :index=>true

      t.timestamps
    end
  end
end
