/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-10-27
    Last Modified: 2023-10-28

    Description: Hasher.hpp
    Uses a txt file to save calculated boards, if they take more then 5 seconds to compute
*/
#ifndef HASHER
#define HASHER
// dependancies
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

// std abbreviations
using std::string;
using std::endl;
using std::to_string;
using std::ifstream;
using std::vector;
using std::stringstream;
using std::ofstream;

class Hasher{
    private:
    // variables
    string filename;
    vector<vector<int>> hashed_boards;
    vector<int> champions;
    vector<int> traits;
    int board_size;
    int cost_limit;
    string hash_file;
    string barcode;

    //functions
    string Vector_To_String(const vector<vector<int>> &vector_boards);
    void Barcode();

    public:
    Hasher(const string &hash_file);
    ~Hasher();
    void Configure(
        const int &cost_restriction,
        const vector<int> &required_champions,
        const vector<int> &required_traits,
        const int &size);
    bool Check_Hashed();
    vector<vector<int>> Fetch();
    void Hash(const vector<vector<int>> &compressed_boards);

};

#endif