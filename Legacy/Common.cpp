#include "Common.hpp"

Trait::Trait(const string &name, vector<int> &value){
    this->name = name;
    quantity = 0;
    int prev_value = 0;

    // fill the value array with the increments
    for(size_t i = 0; i < value.size(); i++){
        this->value[i] = value[i] - prev_value;
        prev_value = value[i];
    }
    // fill the rest of the value array with 0
    for(int i = value.size(); i < MAX_TRAIT_TIERS; i++){
        this->value[i] = 0;
    }
}

Trait::Trait(const Trait &a_trait){
    name = a_trait.name;
    quantity = a_trait.quantity;

    for(int i = 0; i < MAX_TRAIT_TIERS; i++){
        value[i] = a_trait.value[i];
    }
}

int Trait::Increment(){
    if(quantity < MAX_TRAIT_TIERS){
        quantity++;
    }
    return value[quantity];
}

int Trait::Decrement(){
    if(quantity == 0){
        return 0;
    }
    quantity--;
    return -value[quantity + 1]; // return the lost value
}

Champion::Champion(Trait** all_traits, int cost, const string &name, const vector<string> &champion_traits){
    this->cost = cost;
    this->name = name;

    for(int i = 0; i < MAX_CHAMPION_TRAITS; i++){
        traits[i] = nullptr; // initialize all traits to nullptr
    }
    num_traits = 0;
    for(const string &trait_name : champion_traits){
        int counter = 0;
        while(all_traits[counter]->name != trait_name) { counter ++;} // dangerous, but we assume the trait exists
        traits[num_traits++] = all_traits[counter]; // assign the trait to the champion
    }
}

// internal helper function to find the index of a trait by name
int getTraitIndex(Trait** traits, int trait_count, const string &name) {
    for (int i = 0; i < trait_count; i++) {
        if (traits[i]->name == name) {
            return i;
        }
    }
    return -1; // Trait not found
}

Champion::Champion(const Champion& other, Trait** new_traits, int new_traits_count) {
    cost = other.cost;
    name = other.name;
    num_traits = other.num_traits;

    for(int i = 0; i < MAX_CHAMPION_TRAITS; i++){
        if(i < num_traits) {
            traits[i] = new_traits[getTraitIndex(new_traits, new_traits_count, other.traits[i]->name)];
        } else {
            traits[i] = nullptr; // initialize remaining traits to nullptr
        }
    }
}

int Champion::IncrementTraits() {
    int result = 0;
    for(int i = 0; i < num_traits; i++) {
        result += traits[i]->Increment(); // increment each trait's quantity
    }
    return result;
}

int Champion::DecrementTraits() {
    int result = 0;
    for(int i = 0; i < num_traits; i++) {
        result += traits[i]->Decrement(); // decrement each trait's quantity
    }
    return result;
}

void SetData::copySet(const SetData &a_set_data) {
    set_number = a_set_data.set_number;
    champion_count = a_set_data.champion_count;
    trait_count = a_set_data.trait_count;
    
    for(int i = 0; i < MAX_PLAYER_LEVEL; i++){
        cost_restriction[i] = a_set_data.cost_restriction[i];
    }

    traits = new Trait*[trait_count];
    champions = new Champion*[champion_count];

    for(int i = 0; i < trait_count; i++){
        traits[i] = new Trait(*a_set_data.traits[i]);
    }
    for(int i = 0; i < champion_count; i++){
        champions[i] = new Champion(*a_set_data.champions[i], traits, trait_count);
    }
}

// traits remain unchanged
void SetData::restrictSet(int player_level, const vector<int> &champions_to_remove) {
    // Update the current cost restriction based on the player level
    int current_cost_restriction = cost_restriction[player_level - 1];
    int delete_count = 0;

    // Deallocate specified champions from the set
    for (int champion_id : champions_to_remove) {
        delete champions[champion_id]; // Deallocate the champion
        champions[champion_id] = nullptr; // Set to nullptr to avoid dangling pointer
        delete_count++;
    }

    // Deallocate champions that don't meet the cost restriction
    for (int i = 0; i < champion_count; ++i) {
        if (champions[i] && champions[i]->cost > current_cost_restriction) {
            delete champions[i]; // Deallocate the champion
            champions[i] = nullptr; // Set to nullptr to avoid dangling pointer
            delete_count++;
        }
    }
    // Reallocate champions to remove nullptrs
    Champion** new_champions = new Champion*[champion_count - delete_count];
    int new_index = 0;
    for (int i = 0; i < champion_count; ++i) {
        if (champions[i]) { // Only keep non-nullptr champions
            new_champions[new_index++] = champions[i];
        }
    }
    delete[] champions; // Deallocate old champions array
    champions = new_champions; // Assign the new champions array
    champion_count -= delete_count; // Update the champion count
}

void SetData::deallocSet() {
    // Deallocate champions
    for (int i = 0; i < champion_count; ++i) {
        delete champions[i];
    }
    delete[] champions;

    // Deallocate traits
    for (int i = 0; i < trait_count; ++i) {
        delete traits[i];
    }
    delete[] traits;

    // Reset counts
    trait_count = 0;
    champion_count = 0;
}

int SetData::findTraitIndex(const string &name) const{
    for(int i = 0; i < trait_count; i++){
        if(traits[i]->name == name){
            return i;
        }
    }
    cerr << "Trait Not Found";
    return -1;
}

int SetData::findChampionIndex(const string &name) const{
    for(int i = 0; i < champion_count; i++){
        if(champions[i]->name == name){
            return i;
        }
    }
    cerr << "Champion Not Found";
    return -1;
}

void BoardResult::Print() const{
    cout << "Board: ";
    for (const string &champion : board) {
        cout << champion << " ";
    }
    cout << "\nScore: " << score << "\n";
}