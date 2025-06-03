#include "LocalDB.hpp"

void LocalDB::execute(const string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "SQL Error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void LocalDB::prepareSchema() {
    const char* schema = R"(
        CREATE TABLE IF NOT EXISTS sets (
            set_number INTEGER PRIMARY KEY
        );

        CREATE TABLE IF NOT EXISTS level_restrictions (
            set_number INTEGER,
            cost_restriction TEXT
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
    )";
    execute(schema);
}

LocalDB::LocalDB(const string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db)) {
        cerr << "Can't open DB: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
    } else {
        prepareSchema();
    }
}

LocalDB::~LocalDB() {
    if (db) sqlite3_close(db);
}

int LocalDB::getTraitCount(int set_number) {
    string sql = "SELECT COUNT(*) FROM traits WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    int count = 0;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to count traits: " << sqlite3_errmsg(db) << "\n";
    }

    return count;
}

int LocalDB::getChampionCount(int set_number) {
    string sql = "SELECT COUNT(*) FROM champions WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    int count = 0;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to count champions: " << sqlite3_errmsg(db) << "\n";
    }

    return count;
}

Trait** LocalDB::allocTraits(int set_number) {
    // Allocate memory for traits
    Trait** traits = new Trait*[getTraitCount(set_number)];

    // Fetch trait data
    string sql = "SELECT name, value FROM traits WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    int counter = 0;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            vector<int> value_list = deserializeIntVector(value);
            traits[counter++] = new Trait(name, value_list);
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch traits: " << sqlite3_errmsg(db) << "\n";
    }

    return traits;
}

vector<string> LocalDB::getTraitsForChampion(int champion_id) {
    vector<string> trait_names;
    string sql = 
        "SELECT t.name FROM traits t "
        "JOIN champion_traits ct ON t.id = ct.trait_id "
        "WHERE ct.champion_id = " + to_string(champion_id) + ";";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* name = sqlite3_column_text(stmt, 0);
            if (name) trait_names.emplace_back(reinterpret_cast<const char*>(name));
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch traits for champion " << champion_id << ": " << sqlite3_errmsg(db) << "\n";
    }

    return trait_names;
}

Champion** LocalDB::allocChampions(int set_number, Trait** all_traits) {
    Champion** champions = new Champion*[getChampionCount(set_number)];

    string sql = "SELECT id, cost, name FROM champions WHERE set_number = " + to_string(set_number) + " ORDER BY pick_count DESC;";
    sqlite3_stmt* stmt;
    int counter = 0;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int champion_id = sqlite3_column_int(stmt, 0);
            int cost = sqlite3_column_int(stmt, 1);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            vector<string> trait_names = getTraitsForChampion(champion_id);
            champions[counter++] = new Champion(all_traits, cost, name, trait_names);
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch champions: " << sqlite3_errmsg(db) << "\n";
    }

    return champions;
}

vector<int> LocalDB::getSets() {
    vector<int> sets;
    string sql = "SELECT set_number FROM sets;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sets.push_back(sqlite3_column_int(stmt, 0));
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch sets: " << sqlite3_errmsg(db) << "\n";
    }
    return sets;
}

void LocalDB::increaseChampionPickCount(int set_number, string champion_name, int increment) {
    string sql = "UPDATE champions SET pick_count = pick_count + " + to_string(increment) +
                 " WHERE set_number = " + to_string(set_number) + " AND name = '" + champion_name + "';";
    execute(sql);
}

vector<int> LocalDB::getCostRestriction(int set_number) {
    vector<int> restrictions;
    string sql = "SELECT cost_restriction FROM level_restrictions WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* restriction_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            restrictions = deserializeIntVector(restriction_text);
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch cost restrictions: " << sqlite3_errmsg(db) << "\n";
    }

    return restrictions;
}

SetData LocalDB::allocSet(int set_number) {
    SetData set_data;
    set_data.set_number = set_number;
    set_data.traits = allocTraits(set_number);
    set_data.champions = allocChampions(set_number, set_data.traits);
    set_data.trait_count = getTraitCount(set_number);
    set_data.champion_count = getChampionCount(set_number);

    // Get cost restrictions
    vector<int> cost_restrictions = getCostRestriction(set_number);
    for (int i = 0; i < cost_restrictions.size(); ++i) {
        set_data.cost_restriction[i] = cost_restrictions[i];
    }
    for (int i = cost_restrictions.size(); i < MAX_PLAYER_LEVEL; ++i) {
        set_data.cost_restriction[i] = cost_restrictions.back();
    }

    return set_data;
}
