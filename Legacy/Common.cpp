#include <Common.hpp>

Trait::Trait(const string &given_name, const vector<int> &value){
    name = given_name;
    this->value = value;
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