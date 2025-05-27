/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-6-18
    Last Modified: 2025-6-18

    Description: LocalDB.hpp is the Reworked Parser.hpp
    Database for local champions and traits

    Notes:
    You must first add the set number to the database
    Then you can add traits and champions to that set

*/
#pragma once
#include "Common.hpp"
#include "Serialisation.hpp"
#include <sqlite3.h>

// TODO: Add method for adding a new set remotely
class LocalDB {
public:
    LocalDB(const string& dbPath);
    ~LocalDB();
    SetData allocSet(int set_number);
    void deallocSet(SetData &set_data);
    vector<int> getSets();

private:
    void execute(const string& sql);
    void prepareSchema();
    int getTraitCount(int set_number);
    int getChampionCount(int set_number);

    Trait** allocTraits(int set_number);
    Champion** allocChampions(int set_number, const Trait** all_traits);
    sqlite3* db;
};
