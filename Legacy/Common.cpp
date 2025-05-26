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

Champion::Champion(const vector<Trait*> &all_traits, const int &cost, const string &name, const vector<string> &champion_traits){
    this->cost = cost;
    this->name = name;
    
    for(const string &current_trait : champion_traits){
        for(Trait* trait : all_traits){
            if(trait->name == current_trait){
                traits.push_back(trait);
                break;
            }
        }
    }
}