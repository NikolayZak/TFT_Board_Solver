-- manual: InsertNewSetTemplate.sql 


-- === Create tables if they don't exist ===
CREATE TABLE IF NOT EXISTS sets (
    set_number INTEGER PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS traits (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    set_number INTEGER,
    name TEXT,
    value TEXT
);

CREATE TABLE IF NOT EXISTS champions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    set_number INTEGER,
    cost INTEGER,
    name TEXT,
    traits TEXT
);

-- === Insert Set if not exists ===
INSERT OR IGNORE INTO sets (set_number) VALUES (10);

-- === Insert Traits for Set 10 ===
INSERT OR IGNORE INTO traits (set_number, name, value) VALUES (10, 'Brawler', '0,0,1,2,4,6');
INSERT OR IGNORE INTO traits (set_number, name, value) VALUES (10, 'Mage', '3,5,7');

-- === Insert Champions for Set 10 ===
INSERT OR IGNORE INTO champions (set_number, cost, name, traits) VALUES (10, 2, 'Garen', 'Brawler');
INSERT OR IGNORE INTO champions (set_number, cost, name, traits) VALUES (10, 3, 'Lux', 'Mage');
