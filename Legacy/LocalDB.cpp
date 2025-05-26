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

void LocalDB::insertSet(int set_number) {
    string sql = "INSERT OR IGNORE INTO sets (set_number) VALUES (" + to_string(set_number) + ");";
    execute(sql);
}

void LocalDB::insertTrait(int set_number, const string& name, const string& value) {
    string sql = "INSERT INTO traits (set_number, name, value) VALUES (" +
                 to_string(set_number) + ", '" + name + "', '" + value + "');";
    execute(sql);
}

void LocalDB::insertChampion(int set_number, int cost, const string& name, const string& traits) {
    string sql = "INSERT INTO champions (set_number, cost, name, traits) VALUES (" +
                 to_string(set_number) + ", '" + to_string(cost) + "', " + name + ", '" + traits + "');";
    execute(sql);
}

vector<Trait*> LocalDB::allocTraits(int set_number){
    vector<Trait*> traits;
    string sql = "SELECT name, value FROM traits WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            traits.push_back(new Trait(name, deserializeIntVector(value)));
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch traits: " << sqlite3_errmsg(db) << "\n";
    }
    return traits;
}

vector<Champion*> LocalDB::allocChampions(int set_number, const vector<Trait*> &all_traits){
    vector<Champion*> champions;
    string sql = "SELECT cost, name, traits FROM champions WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cost = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string trait_block = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            vector<string> trait_list = deserializeStrVector(trait_block);
            champions.push_back(new Champion(all_traits, cost, name, trait_list));
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

void LocalDB::deallocTraits(vector<Trait*> &all_traits) {
    for (Trait* trait : all_traits) {
        delete trait;
    }
    all_traits.clear();
}

void LocalDB::deallocChampions(vector<Champion*> &all_champions) {
    for (Champion* champion : all_champions) {
        delete champion;
    }
    all_champions.clear();
}

SetData LocalDB::allocSet(int set_number) {
    SetData set_data;
    set_data.set_number = set_number;
    set_data.traits = allocTraits(set_number);
    set_data.champions = allocChampions(set_number, set_data.traits);
    return set_data;
}

void LocalDB::deallocSet(SetData &set_data) {
    deallocChampions(set_data.champions);
    deallocTraits(set_data.traits);
}