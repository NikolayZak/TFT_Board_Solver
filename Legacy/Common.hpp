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
#include "Fast_Vector.hpp"
#include "StaticVariables.hpp"

using namespace std;

struct Trait{
    int quantity;
    int value[MAX_TRAIT_TIERS];
    string name;

    Trait(const string &name, vector<int> &value);
    Trait(const Trait &a_trait);
    int Increment(); // returns the positive value of the increment
    int Decrement(); // returns the negative value of the decrement
};

struct Champion{
    int cost;
    int num_traits;
    Trait* traits[MAX_CHAMPION_TRAITS];
    string name;

    Champion(Trait** all_traits, int cost, const string &name, const vector<string> &champion_traits);
    int IncrementTraits(); // returns the total positive value of all traits
    int DecrementTraits(); // returns the total negative value of all traits
};

struct SetData{
    Trait** traits; // array of pointers to Trait objects
    Champion** champions; // array of pointers to Champion objects
    int cost_restriction[MAX_PLAYER_LEVEL]; // cost restriction[0] is for level 1
    int trait_count;
    int champion_count;
    int set_number;

    void copySet(const SetData &data); // copy the set data from another SetData object
    void restrictSet(int player_level, const vector<int> &champions_to_remove); // restrict the set data based on player level and champions to remove
    void deallocSet(); // deallocate all traits and champions
};

struct BoardEntry {
    Fast_Vector board;
    int board_score;
};

class TopScoringBoards {
private:
    vector<BoardEntry> heap;
    int max_size;

    void heapify_up(int index);
    void heapify_down(int index);
    bool compare(const BoardEntry& a, const BoardEntry& b) const;

public:
    TopScoringBoards(int max_size); // constructor
    void push(const BoardEntry& entry);
    void pop();
    const BoardEntry& top() const;
    bool empty() const;
    int size() const;
};
