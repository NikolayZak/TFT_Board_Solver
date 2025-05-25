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
    Database(const string& dbPath);
    ~Database();
    void insertSet(int set_number);
    void insertTrait(int set_number, const string& name, const string& value);
    void insertChampion(int set_number, int cost, const string& name, const string& traits);
    vector<Trait*> getTraits(int set_number);
    vector<Champion*> getChampions(int set_number);
    void deallocTraits(vector<Trait*> &all_traits);
    void deallocChampions(vector<Champion*> &all_champions);
    vector<int> getSets();

private:
    void execute(const string& sql);
    void prepareSchema();
    sqlite3* db;
};
