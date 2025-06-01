/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2024-05-25
    Last Modified: 2024-05-26

    Description: Common.hpp
    Common libraries and structures used in the project. Please try to keep it minimal and focused on the core data structures.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "StaticVariables.hpp"

using namespace std;

struct Trait{
    int score;
    int quantity;
    int value[MAX_TRAIT_TIERS];
    string name;
    Trait(const string &name, vector<int> &value);
    Trait(const Trait &a_trait);
};

struct Champion{
    int cost;
    int num_traits;
    const Trait* traits[MAX_CHAMPION_TRAITS];
    string name;
    Champion(Trait** all_traits, int cost, const string &name, const vector<string> &champion_traits);
};

struct SetData{
    Trait** traits; // array of pointers to Trait objects
    Champion** champions; // array of pointers to Champion objects
    int cost_restriction[MAX_PLAYER_LEVEL]; // cost restriction[0] is for level 1
    int trait_count;
    int champion_count;
    int set_number;
    SetData(){}
    SetData(const SetData &a_set_data); // copy constructor
};

void deallocSet(SetData &set_data);