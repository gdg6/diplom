# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20150325092651) do

  create_table "devices", force: true do |t|
    t.string   "name"
    t.string   "description",   limit: 1024
    t.string   "room",          limit: 25
    t.string   "mac"
    t.string   "serial_number"
    t.string   "model",         limit: 1024
    t.string   "peername"
    t.integer  "port"
    t.string   "login"
    t.string   "password"
    t.string   "priv_password", limit: 1024
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "logs", force: true do |t|
    t.string   "l_type"
    t.text     "context"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "oids", force: true do |t|
    t.integer  "device_id",    null: false
    t.string   "oid"
    t.string   "translate"
    t.integer  "ping_request", null: false
    t.boolean  "active",       null: false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "oids", ["device_id"], name: "index_oids_on_device_id"

  create_table "reports", force: true do |t|
    t.integer  "device_id"
    t.string   "r_type"
    t.text     "context"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "users", force: true do |t|
    t.string   "login"
    t.string   "password_digest"
    t.integer  "role"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "walk_raports", force: true do |t|
    t.string   "path"
    t.integer  "device_id",  null: false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "walk_raports", ["device_id"], name: "index_walk_raports_on_device_id"

  create_table "walk_requests", force: true do |t|
    t.string   "request"
    t.string   "description"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

end
