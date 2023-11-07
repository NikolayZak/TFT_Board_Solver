#include "Parser.hpp"
#include "Board.hpp"
#include "Solver.hpp"
#include "Hasher.hpp"
#include "Interface.hpp"

#define CHAMPION_FILE "champs.csv"
#define TRAIT_FILE "traits.csv"

int main(){
    cout << "----- Testing Parser.cpp -----" << endl;
    // initialize the database
    Database* test_db = new Database(TRAIT_FILE, CHAMPION_FILE);

    // check the champions in set
    cout << "Champions in Set: " << test_db->champions_in_set << endl;

    // check the traits in set
    cout << "Traits in Set: " << test_db->traits_in_set << endl;

    // check the level restriction
    vector<int> vec = test_db->level_restriction;
    cout << "Level Restriction = {";
    for(int i = 0; i < (int)vec.size(); i++){
        if(i+1 == (int)vec.size()){
            cout << vec[i];
        }else{
            cout << vec[i] << ",";
        }
    }
    cout << "}\n";

    // prints the traits
    cout << "Traits:" << endl;
    for(int i = 0; i < test_db->traits_in_set; i++){
        cout << test_db->all_traits[i]->name << ", fielded = ";
        cout << test_db->all_traits[i]->fielded << ", score = ";
        cout << test_db->all_traits[i]->score << "\ntier : value\n";
        for(int j = 0; j < (int)test_db->all_traits[i]->tiers.size(); j++){
            cout << test_db->all_traits[i]->tiers[j] << " : ";
            cout << test_db->all_traits[i]->tier_values[j] << endl;
        }
    }

    // prints the database
    cout << "Champions:" << endl;
    for(int i = 0; i < test_db->champions_in_set; i++){
        //name
        cout << test_db->all_champions[i]->name << ", ";
        //cost
        cout << test_db->all_champions[i]->cost << ", ";
        //traits
        for(int j = 0; j < (int)test_db->all_champions[i]->traits.size(); j++){
            if(j+1 == (int)test_db->all_champions[i]->traits.size()){
                cout << test_db->all_champions[i]->traits[j]->name;
            }else{
                cout << test_db->all_champions[i]->traits[j]->name << ", ";
            }
        }
        cout << endl;
    }

    // check the deconstructor
    cout << "deconstructing" << endl;
    delete test_db;
    cout << "----- Parser.hpp Successful -----" << endl;
    // **************************************************************
    cout << "----- Testing Board.cpp -----" << endl;
    








    return 0;
}