#include "../LocalDB.hpp"

// compile test with: g++ ../Fast_Vector.cpp ../Common.cpp ../Serialisation.cpp ../LocalDB.cpp ../newBoard.cpp ../newSolver.cpp newMain.cpp -lsqlite3 -o test
int main() {
    // Initialize the local database
    LocalDB db("database.db");
    // Allocate a set (for example, set number 1)
    SetData source_set_data = db.allocSet(11);
    SetData set_data;
    set_data.copySet(source_set_data); // Copy the allocated set data
    //set_data.restrictSet(6, {1}); // Restrict the set for player level 3 with no champions to remove

    // Print the allocated set data
    cout << "Set Number: " << set_data.set_number << endl;
    cout << "Number of Traits: " << set_data.trait_count << endl;
    cout << "Number of Champions: " << set_data.champion_count << endl;

    for(int i = 0; i < set_data.trait_count; ++i) {
        cout << "Trait " << i + 1 << ": " << set_data.traits[i]->name << endl;
        cout << "  Values: ";
        for(int j = 0; j < MAX_TRAIT_TIERS; ++j) {
            cout << set_data.traits[i]->value[j] << " ";
        }
        cout << endl;
    }

    for(int i = 0; i < set_data.champion_count; ++i) {
        cout << "Champion " << i + 1 << ": " << set_data.champions[i]->name 
             << " (Cost: " << set_data.champions[i]->cost << ")" << endl;
        for(int j = 0; j < set_data.champions[i]->num_traits; ++j) {
            cout << "  Trait: " << set_data.champions[i]->traits[j]->name << endl;
        }
    }

    // Deallocate the set data
    set_data.deallocSet();
    source_set_data.deallocSet(); // Deallocate the source set data
    cout << "Set data deallocated." << endl;

    return 0;
}