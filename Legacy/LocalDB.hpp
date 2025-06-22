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
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sqlite3.h>
#include "Common.hpp"
#include "StaticVariables.hpp"
#include "Serialisation.hpp"

using std::string;
using std::vector;
using std::unordered_map;
using std::cerr;
using std::to_string;


class LocalDB {
public:
    LocalDB(const string& dbPath);
    ~LocalDB();
    SetData allocSet(int set_number);
    vector<int> getSets();
    void increaseChampionPickCount(int set_number, string champion_name, int increment);
    void incrementAllChampions(int set_number, const vector<BoardResult> &boards);
    bool isChampionInSet(int set_number, const string& champion_name);
    bool isTraitInSet(int set_number, const string& trait_name);

private:
    void execute(const string& sql);
    void prepareSchema();
    vector<int> getCostRestriction(int set_number);
    int getTraitCount(int set_number);
    int getChampionCount(int set_number);
    vector<string> getTraitsForChampion(int champion_id);

    Trait** allocTraits(int set_number);
    Champion** allocChampions(int set_number, Trait** all_traits);
    sqlite3* db;
};
