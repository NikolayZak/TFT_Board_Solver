#!/usr/bin/env python3

# insert a set by running: python3 insertSet.py set#.json

import sqlite3
import json
import sys
import os

# === Step 1: Get JSON filename from command-line ===
if len(sys.argv) != 2:
    print("Usage: python3 insertSet.py <filename.json>")
    sys.exit(1)

filename = sys.argv[1]

if not os.path.exists(filename):
    print(f"❌ File '{filename}' not found.")
    sys.exit(1)

# === Step 2: Load data from JSON ===
with open(filename, "r") as f:
    data = json.load(f)

if "set_number" not in data:
    print("❌ 'set_number' missing in JSON file.")
    sys.exit(1)

set_number = float(data["set_number"])
cost_restriction = data.get("cost_restriction", "")

# === Step 3: Connect to DB and create schema ===
conn = sqlite3.connect("database.db")
cur = conn.cursor()

schema = """
CREATE TABLE IF NOT EXISTS sets (
    set_number FLOAT PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS level_restrictions (
    set_number FLOAT,
    cost_restriction TEXT
);

CREATE TABLE IF NOT EXISTS traits (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    set_number FLOAT,
    name TEXT,
    value TEXT
);

CREATE TABLE IF NOT EXISTS champions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    set_number FLOAT,
    cost INTEGER,
    name TEXT,
    pick_count INTEGER DEFAULT 0
);

CREATE INDEX IF NOT EXISTS idx_champion_pick_count 
ON champions(set_number, pick_count DESC);

CREATE TABLE IF NOT EXISTS champion_traits (
    champion_id INTEGER,
    trait_id INTEGER,
    PRIMARY KEY (champion_id, trait_id),
    FOREIGN KEY (champion_id) REFERENCES champions(id) ON DELETE CASCADE,
    FOREIGN KEY (trait_id) REFERENCES traits(id) ON DELETE CASCADE
);
"""
cur.executescript(schema)

# === Step 4: Insert set and level restriction ===
cur.execute("INSERT OR IGNORE INTO sets (set_number) VALUES (?)", (set_number,))
cur.execute("INSERT OR REPLACE INTO level_restrictions (set_number, cost_restriction) VALUES (?, ?)",
            (set_number, cost_restriction))

# === Step 5: Insert traits ===
trait_id_map = {}
for trait in data.get("traits", []):
    cur.execute("INSERT INTO traits (set_number, name, value) VALUES (?, ?, ?)",
                (set_number, trait["name"], trait["value"]))
    trait_id_map[trait["name"]] = cur.lastrowid

# === Step 6: Insert champions and link traits ===
for champ in data.get("champions", []):
    cur.execute("INSERT INTO champions (set_number, cost, name) VALUES (?, ?, ?)",
                (set_number, champ["cost"], champ["name"]))
    champ_id = cur.lastrowid
    for trait_name in champ.get("traits", []):
        trait_id = trait_id_map.get(trait_name)
        if trait_id is not None:
            cur.execute("INSERT OR IGNORE INTO champion_traits (champion_id, trait_id) VALUES (?, ?)",
                        (champ_id, trait_id))

conn.commit()
conn.close()
print(f"✅ Set {set_number} successfully inserted from '{filename}'.")
