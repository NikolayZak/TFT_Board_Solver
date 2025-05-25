#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Trait{
    string name;
    vector<int> value;
    int score;
    int quantity;
    Trait(const string &given_name, const string &value);
    Trait(const Trait &a_trait);
};

struct Champion{
    string name;
    int cost;
    vector<Trait*> traits;
    Champion(const vector<Trait*> &all_traits, const string &name, const int &cost, const string &traits);
};

string serializeVector(const vector<int>& vec);
string serializeVector(const vector<string>& vec);
vector<int> deserializeIntVector(const string& str);
vector<string> deserializeStrVector(const string& str);

void deallocTraits(vector<Trait*> &all_traits);
void deallocChampions(vector<Champion*> &all_champions);
