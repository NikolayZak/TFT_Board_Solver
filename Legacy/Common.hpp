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
    Trait(const string &given_name, const string &value);
    Trait(const Trait &a_trait);
};

struct Champion{
    int cost;
    string name;
    vector<Trait*> traits;
    Champion(const vector<Trait*> &all_traits, const int &cost, const string &name, const string &traits);
};

string serializeVector(const vector<int>& vec);
string serializeVector(const vector<string>& vec);
vector<int> deserializeIntVector(const string& str);
vector<string> deserializeStrVector(const string& str);

