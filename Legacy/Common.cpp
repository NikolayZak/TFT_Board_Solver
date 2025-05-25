#include <Common.hpp>

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

Trait::Trait(const string &given_name, const string &value){
    name = given_name;
    this->value = deserializeIntVector(value);
    score = 0;
    quantity = 0;
}
Trait::Trait(const Trait &a_trait){
    name = a_trait.name;
    value = a_trait.value;
    score = a_trait.score;
    quantity = a_trait.quantity;
}

Champion::Champion(const string &name, const int &cost, const vector<Trait*> &traits){
    this->name = name;
    this->cost = cost;
    this->traits = traits;
}