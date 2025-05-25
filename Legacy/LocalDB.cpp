#include "LocalDB.hpp"

void Database::execute(const string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "SQL Error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void Database::prepareSchema() {
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

Database::Database(const string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db)) {
        cerr << "Can't open DB: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
    } else {
        prepareSchema();
    }
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

void Database::insertSet(int set_number) {
    string sql = "INSERT OR IGNORE INTO sets (set_number) VALUES (" + to_string(set_number) + ");";
    execute(sql);
}

void Database::insertTrait(int set_number, const string& name, const string& value) {
    string sql = "INSERT INTO traits (set_number, name, value) VALUES (" +
                 to_string(set_number) + ", '" + name + "', '" + value + "');";
    execute(sql);
}

void Database::insertChampion(int set_number, int cost, const string& name, const string& traits) {
    string sql = "INSERT INTO champions (set_number, cost, name, traits) VALUES (" +
                 to_string(set_number) + ", '" + to_string(cost) + "', " + name + ", '" + traits + "');";
    execute(sql);
}

vector<Trait*> Database::getTraits(int set_number){
    vector<Trait*> traits;
    string sql = "SELECT name, value FROM traits WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            traits.push_back(new Trait(name, value));
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch traits: " << sqlite3_errmsg(db) << "\n";
    }
    return traits;
}

vector<Champion*> Database::getChampions(int set_number) {
    vector<Champion*> champions;
    string sql = "SELECT cost, name, traits FROM champions WHERE set_number = " + to_string(set_number) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v3(db, sql.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cost = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string traits = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            champions.push_back(new Champion(getTraits(set_number), cost, name, traits));
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to fetch champions: " << sqlite3_errmsg(db) << "\n";
    }
    return champions;
}

vector<int> Database::getSets() {
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

void Database::deallocTraits(vector<Trait*> &all_traits) {
    for (Trait* trait : all_traits) {
        delete trait;
    }
    all_traits.clear();
}

void Database::deallocChampions(vector<Champion*> &all_champions) {
    for (Champion* champion : all_champions) {
        delete champion;
    }
    all_champions.clear();
}