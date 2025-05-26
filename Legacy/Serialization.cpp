#include "Serialization.hpp"

string serializeVector(const vector<int>& vec) {
    ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) oss << ",";
    }
    return oss.str();
}

string serializeVector(const vector<string>& vec) {
    ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) oss << ",";
    }
    return oss.str();
}

vector<int> deserializeIntVector(const string& str) {
    vector<int> result;
    istringstream iss(str);
    string token;
    while (getline(iss, token, ',')) {
        result.push_back(stoi(token));
    }
    return result;
}

vector<string> deserializeStrVector(const string& str) {
    vector<string> result;
    istringstream iss(str);
    string token;
    while (getline(iss, token, ',')) {
        result.push_back(token);
    }
    return result;
}