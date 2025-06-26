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

bool LocalDB::isChampionInSet(float set_number, const string& champion_name) {
    string sql = "SELECT COUNT(*) FROM champions WHERE set_number = " + to_string(set_number) +
                 " AND name = '" + champion_name + "';";
    sqlite3_stmt* stmt;
    bool exists = false;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to check champion existence: " << sqlite3_errmsg(db) << "\n";
    }

    return exists;
}

bool LocalDB::isTraitInSet(float set_number, const string& trait_name) {
    string sql = "SELECT COUNT(*) FROM traits WHERE set_number = " + to_string(set_number) +
                 " AND name = '" + trait_name + "';";
    sqlite3_stmt* stmt;
    bool exists = false;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to check trait existence: " << sqlite3_errmsg(db) << "\n";
    }

    return exists;
}

int LocalDB::getTraitCount(float set_number) {
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

int LocalDB::getChampionCount(float set_number) {
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

Trait** LocalDB::allocTraits(float set_number) {
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

Champion** LocalDB::allocChampions(float set_number, Trait** all_traits) {
    vector<Champion*> champion_vector;

    // Step 1: Fetch all traits in one query
    unordered_map<int, vector<string>> traits_by_champion;
    string trait_sql =
        "SELECT ct.champion_id, t.name "
        "FROM champion_traits ct "
        "JOIN traits t ON ct.trait_id = t.id "
        "WHERE ct.champion_id IN (SELECT id FROM champions WHERE set_number = " + to_string(set_number) + ");";

    sqlite3_stmt* trait_stmt;
    if (sqlite3_prepare_v2(db, trait_sql.c_str(), -1, &trait_stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(trait_stmt) == SQLITE_ROW) {
            int champ_id = sqlite3_column_int(trait_stmt, 0);
            const unsigned char* name = sqlite3_column_text(trait_stmt, 1);
            if (name) {
                traits_by_champion[champ_id].emplace_back(reinterpret_cast<const char*>(name));
            }
        }
        sqlite3_finalize(trait_stmt);
    } else {
        cerr << "Failed to fetch traits: " << sqlite3_errmsg(db) << "\n";
    }

    // Step 2: Fetch all champions
    string champ_sql =
        "SELECT id, cost, name FROM champions "
        "WHERE set_number = " + to_string(set_number) + " "
        "ORDER BY pick_count DESC;";

    sqlite3_stmt* champ_stmt;
    if (sqlite3_prepare_v2(db, champ_sql.c_str(), -1, &champ_stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(champ_stmt) == SQLITE_ROW) {
            int champion_id = sqlite3_column_int(champ_stmt, 0);
            int cost = sqlite3_column_int(champ_stmt, 1);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(champ_stmt, 2));

            const vector<string>& trait_names = traits_by_champion[champion_id];
            champion_vector.push_back(new Champion(all_traits, cost, name, trait_names));
        }
        sqlite3_finalize(champ_stmt);
    } else {
        cerr << "Failed to fetch champions: " << sqlite3_errmsg(db) << "\n";
    }

    // Step 3: Convert vector to raw pointer array
    Champion** champions = new Champion*[champion_vector.size()];
    std::copy(champion_vector.begin(), champion_vector.end(), champions);

    return champions;
}

vector<float> LocalDB::getSets() {
    vector<float> sets;
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

void LocalDB::incrementAllChampions(float set_number, const vector<BoardResult>& boards) {
    unordered_map<string, int> champion_counts;

    // Count champions
    for (const auto& board_result : boards) {
        for (const auto& champ_name : board_result.board) {
            champion_counts[champ_name]++;
        }
    }

    // Prepare one SQL statement and reuse it
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE champions SET pick_count = pick_count + ? WHERE set_number = ? AND name = ?;";

    // Begin transaction
    execute("BEGIN TRANSACTION;");

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        for (const auto& [name, count] : champion_counts) {
            sqlite3_bind_int(stmt, 1, count);
            sqlite3_bind_int(stmt, 2, set_number);
            sqlite3_bind_text(stmt, 3, name.c_str(), -1, SQLITE_STATIC);

            sqlite3_step(stmt);
            sqlite3_reset(stmt); // Ready the statement for next bind
        }
        sqlite3_finalize(stmt);
    } else {
        // handle prepare error
    }

    // End transaction
    execute("COMMIT;");
}


vector<int> LocalDB::getCostRestriction(float set_number) {
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

SetData LocalDB::allocSet(float set_number) {
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
