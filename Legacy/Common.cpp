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

void Trait::Increment(){
    quantity++;
    score += value[quantity];
}

void Trait::Decrement(){
    score -= value[quantity];
    quantity--;
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

void Champion::IncrementTraits() {
    for(int i = 0; i < num_traits; i++) {
        traits[i]->Increment(); // increment each trait's quantity
    }
}

void Champion::DecrementTraits() {
    for(int i = 0; i < num_traits; i++) {
        traits[i]->Decrement(); // decrement each trait's quantity
    }
}

void SetData::copySet(const SetData &a_set_data) {
    this->set_number = a_set_data.set_number;
    this->champion_count = a_set_data.champion_count;
    this->trait_count = a_set_data.trait_count;
    this->current_cost_restriction = a_set_data.current_cost_restriction;
    
    for(int i = 0; i < MAX_PLAYER_LEVEL; i++){
        this->cost_restriction[i] = a_set_data.cost_restriction[i];
    }

    this->traits = new Trait*[this->trait_count];
    this->champions = new Champion*[this->champion_count];

    for(int i = 0; i < this->trait_count; i++){
        this->traits[i] = new Trait(*a_set_data.traits[i]);
    }
    for(int i = 0; i < this->champion_count; i++){
        this->champions[i] = new Champion(*a_set_data.champions[i]);
    }
}

void SetData::deallocSet() {
    // Deallocate champions
    for (int i = 0; i < this->champion_count; ++i) {
        delete this->champions[i];
    }
    delete[] this->champions;

    // Deallocate traits
    for (int i = 0; i < this->trait_count; ++i) {
        delete this->traits[i];
    }
    delete[] this->traits;

    // Reset counts
    this->trait_count = 0;
    this->champion_count = 0;
}