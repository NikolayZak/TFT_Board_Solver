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

Champion** LocalDB::allocChampions(int set_number, const Trait** all_traits){
    // Allocate memory for champions
    Champion** champions = new Champion*[getChampionCount(set_number)];

    // Fetch champion data
    string sql = "SELECT cost, name, traits FROM champions WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    int counter = 0;

    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cost = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string champion_trait_block = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            vector<string> champion_trait_list = deserializeStrVector(champion_trait_block);
            champions[counter++] = new Champion(all_traits, cost, name, champion_trait_list);
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

SetData LocalDB::allocSet(int set_number) {
    SetData set_data;
    set_data.set_number = set_number;
    set_data.traits = allocTraits(set_number);
    set_data.champions = allocChampions(set_number, set_data.traits);
    set_data.trait_count = getTraitCount(set_number);
    set_data.champion_count = getChampionCount(set_number);
    return set_data;
}

void LocalDB::deallocSet(SetData &set_data) {
    // Deallocate champions
    for (int i = 0; i < set_data.champion_count; ++i) {
        delete set_data.champions[i];
    }
    delete[] set_data.champions;

    // Deallocate traits
    for (int i = 0; i < set_data.trait_count; ++i) {
        delete set_data.traits[i];
    }
    delete[] set_data.traits;

    // Reset counts
    set_data.trait_count = 0;
    set_data.champion_count = 0;
}