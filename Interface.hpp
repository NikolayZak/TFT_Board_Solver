/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-10-27
    Last Modified: 2023-10-28

    Description: Interface.hpp
    methods to process and display the Solver and Hasher data
*/

#ifndef INTERFACE
#define INTERFACE
// dependacies
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "embeded_text/Prompts.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cin;
using std::left;
using std::setw;
using std::tolower;

// return type for input
struct Input{
    bool Is_Int;
    int name;
    string misspelled;
};



// Independent functions
int Get_Input(const int &lower, const int &higher, const string &prompts);
Input Get_Input(const int &lower, const int &higher, const string &prompts, const vector<string> &data);
void Print_Boards(const vector<vector<string>> &Boards);
void Print_Elements(const vector<string> &elements);
void Print_Settings(const vector<string> &champions_added, const vector<string> &traits_added, const int &cost_restriction);


#endif
