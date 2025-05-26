/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-5-25
    Last Modified: 2025-5-25

    Description: Serialization.hpp is a method used to serialize and deserialize vectors of integers and strings.

*/
#pragma once
#include "Common.hpp"

string serializeVector(const vector<int>& vec);
string serializeVector(const vector<string>& vec);
vector<int> deserializeIntVector(const string& str);
vector<string> deserializeStrVector(const string& str);