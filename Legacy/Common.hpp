#pragma once
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::endl;
using std::cerr;

struct Trait{
    string name;
    vector<int> value;
    int score;
    int quantity;
    Trait(const string &given_name, const vector<int> &value);
    Trait(const Trait &a_trait);
};

struct Champion{
    string name;
    int cost;
    vector<Trait*> traits;
    Champion(const string &name, const int &cost, const vector<Trait*> &traits);
};