-- Insert a a new set (example for Set #10)
-- INSERT INTO sets (set_number) VALUES (Set #);
INSERT INTO sets (set_number) VALUES (10);

-- Insert traits for set 10
INSERT INTO traits (set_number, name, value) VALUES (10, 'Brawler', '0,0,1,2,4,6'); -- (Set #, Trait Name, Trait Value for EACH tier)
INSERT INTO traits (set_number, name, value) VALUES (10, 'Mage', '3,5,7');

-- Insert champions for set 10
INSERT INTO champions (set_number, cost, name, traits) VALUES (10, 2, 'Garen', 'Brawler'); -- (Set #, Cost, Champion Name, Traits)
INSERT INTO champions (set_number, cost, name, traits) VALUES (10, 3, 'Lux', 'Mage');

-- Add more rows as needed...
