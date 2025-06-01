#include "Common.hpp"

Trait::Trait(const string &name, vector<int> &value){
    this->name = name;
    score = 0;
    quantity = 0;

    // fill the value array with the provided values
    for(int i = 0; i < value.size(); i++){
        this->value[i] = value[i];
    }
    // fill the rest of the value array with last value
    for(int i = value.size(); i < MAX_TRAIT_TIERS; i++){
        this->value[i] = value[value.size() - 1];
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

Champion::Champion(Trait** all_traits, int cost, const string &name, const vector<string> &champion_traits){
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

SetData::SetData(const SetData &a_set_data) {
    SetData new_set_data;
    
    new_set_data.set_number = a_set_data.set_number;
    new_set_data.champion_count = a_set_data.champion_count;
    new_set_data.trait_count = a_set_data.trait_count;
    
    for(int i = 0; i < MAX_PLAYER_LEVEL; i++){
        new_set_data.cost_restriction[i] = a_set_data.cost_restriction[i];
    }

    new_set_data.traits = new Trait*[new_set_data.trait_count];
    new_set_data.champions = new Champion*[new_set_data.champion_count];

    for(int i = 0; i < new_set_data.trait_count; i++){
        new_set_data.traits[i] = new Trait(*a_set_data.traits[i]);
    }
    for(int i = 0; i < new_set_data.champion_count; i++){
        new_set_data.champions[i] = new Champion(*a_set_data.champions[i]);
    }
}

void deallocSet(SetData &set_data) {
    // Deallocate champions
    for (int i = 0; i < set_data.champion_count; ++i) {
        delete set_data.champions[i];
    }
    delete[] set_data.champions;

    // Deallocate traits
    for (int i = 0; i < set_data.trait_count; ++i) {
        delete set_data.traits[i];
    }
    delete[] set_data.traits;

    // Reset counts
    set_data.trait_count = 0;
    set_data.champion_count = 0;
}