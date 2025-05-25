/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-6-18
    Last Modified: 2025-6-18

    Description: LocalDB.hpp is the Reworked Parser.hpp
    Database for local champions and traits

*/

#pragma once
#include "Common.hpp"
#include <sqlite3.h>


class Database {
public:
    Database(const std::string& dbPath);
    ~Database();

private:
    void execute(const std::string& sql);
    void prepareSchema();
    sqlite3* db;
};
