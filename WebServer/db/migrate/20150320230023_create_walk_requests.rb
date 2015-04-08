class CreateWalkRequests < ActiveRecord::Migration
  def change
    create_table :walk_requests do |t|
      t.string :request
      t.string :description

      t.timestamps
    end
  end
end
