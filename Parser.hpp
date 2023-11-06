/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-10-25
    Last Modified: 2023-10-27

    Description: Database.hpp
    ADT for extracting this set's data on champs, traits, level chance, and trait tiers/tier values
    Formatting must be precise!

    Champion_file must be formatted as such:

    Level Chance:
    1,1,2,3,4,4,5
    Cassiopeia,1,Noxus,Shurima,Invoker
    Aatrox,5,Darkin,Slayer,Juggernaut
    Naafiri,2,Darkin,Shurima,Challenger
    ...

    NOTE: the second line of integers in Traits_file is for editing the values of any given tier
    Traits_file must be formatted as such:
    Darkin
    0,2
    0,2
    Slayer
    0,2,4,6
    0,2,4,6
    Juggernaut
    0,2,4,6
    0,2,4,6

*/

#ifndef DATABASE
#define DATABASE
// Dependencies
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


// std abbreviations
using std::string;
using std::vector;
using std::endl;
using std::cerr;


class Trait{
    public:
    string name;
    vector<int> tiers;
    vector<int> tier_values;
    int score;
    int fielded;
    Trait(const string &name, const vector<int> &tiers, const vector<int> &tier_values);
    ~Trait();
};

class Champion{
    public:
    string name;
    int cost;
    vector<Trait*> traits;
    Champion(const string &name, const int &cost, const vector<Trait*> &traits);
    ~Champion();
};

// used to parse traits and champions from a file
class Database{
    private:
    // functions used to create the database
    Trait* Find_Trait(const string &name);
    void Parse_Traits(const string &traits_file);
    void Parse_Champions(const string &champions_file);
    vector<int> Parse_Int_Line(const string &line);
    vector<string> Parse_String_Line(const string& line);
    
    public:
    // variables
    vector<Trait*> all_traits;
    vector<Champion*> all_champions;
    int traits_in_set;
    int champions_in_set;
    vector<int> level_restriction;
    

    Database(const string &traits_file, const string &champions_file);
    ~Database();

};
#endif