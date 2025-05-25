#include "LocalDB.hpp"

Database::Database(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db)) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
    } else {
        prepareSchema();
    }
}

Database::~Database() {
    if (db) sqlite3_close(db);
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
            name TEXT,
            cost INTEGER
        );

        CREATE TABLE IF NOT EXISTS champion_traits (
            champion_id INTEGER,
            trait_id INTEGER
        );
    )";
    execute(schema);
}

void Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}