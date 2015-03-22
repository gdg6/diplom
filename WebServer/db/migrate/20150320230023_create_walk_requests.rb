class CreateWalkRequests < ActiveRecord::Migration
  def change
    create_table :walk_requests do |t|
      t.string :request

      t.timestamps
    end
  end
end
