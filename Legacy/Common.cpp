#include <Common.hpp>

Trait::Trait(const string &name, const int* value, int num_values){
    this->name = name;
    score = 0;
    quantity = 0;

    // fill the value array with the provided values
    for(int i = 0; i < num_values; i++){
        this->value[i] = value[i];
    }
    // fill the rest of the value array with last value
    for(int i = num_values; i < MAX_TRAIT_TIERS; i++){
        this->value[i] = value[num_values - 1];
    }
}

Trait::Trait(const Trait &a_trait){
    name = a_trait.name;
    score = a_trait.score;
    quantity = a_trait.quantity;

    for(int i = 0; i < MAX_TRAIT_TIERS; i++){
        value[i] = a_trait.value[i];
    }
}

Champion::Champion(const Trait** all_traits, int cost, const string &name, const vector<string> &champion_traits){
    this->cost = cost;
    this->name = name;

    for(int i = 0; i < MAX_CHAMPION_TRAITS; i++){
        traits[i] = nullptr; // initialize all traits to nullptr
    }
    num_traits = 0;
    for(const string &trait_name : champion_traits){
        for(int i = 0; i < MAX_CHAMPION_TRAITS; i++){ // iterate through all traits
            if(all_traits[i]->name == trait_name){
                traits[num_traits++] = all_traits[i]; // assign the trait to the champion
                break;
            }
        }
    }
}