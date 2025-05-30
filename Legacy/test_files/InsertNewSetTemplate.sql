-- Commands to create a new set in the TFT Board Solver database: 
-- sqlite3 /home/nikolay/Github/TFT_Board_Solver/Legacy/test_files/database.db < InsertNewSetTemplate.sql

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
    name TEXT
);

CREATE TABLE IF NOT EXISTS champion_traits (
    champion_id INTEGER,
    trait_id INTEGER,
    PRIMARY KEY (champion_id, trait_id),
    FOREIGN KEY (champion_id) REFERENCES champions(id) ON DELETE CASCADE,
    FOREIGN KEY (trait_id) REFERENCES traits(id) ON DELETE CASCADE
);

-- Insert sets
INSERT OR IGNORE INTO sets (set_number) VALUES (1);

-- Insert traits (Set#, Name, value per each tier level Example: '0,0,2,2" means value 0 for 0 bruisers, 0 for 1 bruiser, 2 for 2 bruisers, and then last value (2) for the rest (undefined places)))
INSERT OR IGNORE INTO traits (set_number, name, value) VALUES (1, 'Bruiser', '0,0,2,2,4,4,6,6,8');
INSERT OR IGNORE INTO traits (set_number, name, value) VALUES (1, 'Sorcerer', '0,0,2,2,4,4,6,6,8');

-- Insert champions
INSERT OR IGNORE INTO champions (set_number, cost, name) VALUES (1, 4, 'Garen');
INSERT OR IGNORE INTO champions (set_number, cost, name) VALUES (1, 1, 'Lux');

-- Link champion traits (join table)
INSERT OR IGNORE INTO champion_traits (champion_id, trait_id)
SELECT c.id, t.id FROM champions c, traits t
WHERE c.name = 'Garen' AND t.name = 'Bruiser';

INSERT OR IGNORE INTO champion_traits (champion_id, trait_id)
SELECT c.id, t.id FROM champions c, traits t
WHERE c.name = 'Lux' AND t.name = 'Sorcerer';
