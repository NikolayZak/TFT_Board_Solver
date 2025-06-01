#include "Common.hpp"

Trait::Trait(const string &name, vector<int> &value){
    this->name = name;
    quantity = 0;
    int prev_value = 0;

    // fill the value array with the increments
    for(int i = 0; i < value.size(); i++){
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
    quantity++;
    return value[quantity];
}

int Trait::Decrement(){
    quantity--;
    return -value[quantity + 1];
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
    this->set_number = a_set_data.set_number;
    this->champion_count = a_set_data.champion_count;
    this->trait_count = a_set_data.trait_count;
    
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

void SetData::restrictSet(int player_level, const vector<int> &champions_to_remove) {
    // Update the current cost restriction based on the player level
    int current_cost_restriction = this->cost_restriction[player_level - 1];
    int delete_count = 0;

    // Deallocate specified champions from the set
    for (int champion_id : champions_to_remove) {
        delete this->champions[champion_id]; // Deallocate the champion
        this->champions[champion_id] = nullptr; // Set to nullptr to avoid dangling pointer
        delete_count++;
    }

    // Deallocate champions that don't meet the cost restriction
    for (int i = 0; i < this->champion_count; ++i) {
        if (this->champions[i] && this->champions[i]->cost > current_cost_restriction) {
            delete this->champions[i]; // Deallocate the champion
            this->champions[i] = nullptr; // Set to nullptr to avoid dangling pointer
            delete_count++;
        }
    }
    // Reallocate champions to remove nullptrs
    Champion** new_champions = new Champion*[this->champion_count - delete_count];
    int new_index = 0;
    for (int i = 0; i < this->champion_count; ++i) {
        if (this->champions[i]) { // Only keep non-nullptr champions
            new_champions[new_index++] = this->champions[i];
        }
    }
    delete[] this->champions; // Deallocate old champions array
    this->champions = new_champions; // Assign the new champions array
    this->champion_count -= delete_count; // Update the champion count
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

TopScoringBoards::TopScoringBoards(int max_size){
    heap.reserve(max_size);
    this->max_size = max_size;
}

TopScoringBoards::~TopScoringBoards() {
    heap.clear();
}

void TopScoringBoards::heapify_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (compare(heap[index], heap[parent])) {
            swap(heap[index], heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void TopScoringBoards::heapify_down(int index) {
    int size = heap.size();
    while (index < size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && compare(heap[left], heap[largest])) {
            largest = left;
        }
        if (right < size && compare(heap[right], heap[largest])) {
            largest = right;
        }
        if (largest != index) {
            swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}
// ensures that the board with the lowest score is at the top of the heap
bool TopScoringBoards::compare(const BoardEntry& a, const BoardEntry& b) const {
    return a.board_score < b.board_score; // Min-heap based on board score
}

void TopScoringBoards::push(const BoardEntry& entry) {
    if (heap.size() < max_size) {
        heap.push_back(entry);
        heapify_up(heap.size() - 1);
    } else if (compare(heap[0], entry)) {
        heap[0] = entry;
        heapify_down(0);
    }
}

const BoardEntry& TopScoringBoards::top() const {
    return heap[0];
}

void TopScoringBoards::pop() {
    heap[0] = heap.back();
    heap.pop_back();
    heapify_down(0);
}

bool TopScoringBoards::empty() const {
    return heap.empty();
}

int TopScoringBoards::size() const {
    return heap.size();
}