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

ActiveRecord::Schema.define(version: 20141208135403) do

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
    t.integer  "mib_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "mibs", force: true do |t|
    t.string   "file_name_mib"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "reports", force: true do |t|
    t.integer  "device_id"
    t.string   "type"
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

end
