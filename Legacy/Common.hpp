/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2024-05-25
    Last Modified: 2024-05-25

    Description: Common.hpp
    Common libraries and structures used in the project. Please try to keep it minimal and focused on the core data structures.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Trait{
    int score;
    int quantity;
    vector<int> value;
    string name;
    Trait(const string &given_name, const vector<int> &value);
    Trait(const Trait &a_trait);
};

struct Champion{
    int cost;
    string name;
    vector<Trait*> traits;
    Champion(const vector<Trait*> &all_traits, const int &cost, const string &name, const vector<string> &traits);
};

struct SetData{
    int set_number;
    vector<Champion*> champions;
    vector<Trait*> traits;
};
