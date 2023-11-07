#include "../Solver.hpp" // includes parser.hpp and board.hpp
#include "../Hasher.hpp"
#include "../Interface.hpp"

#define CHAMPION_FILE "test_files/test_champs.csv"
#define TRAIT_FILE "test_files/test_traits.csv"

int main(){
    cout << "----- Testing Parser.hpp -----" << endl;
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

    // testing the deconstructor
    cout << "deconstructing" << endl;
    delete test_db;

    cout << "----- Parser.hpp Successful -----" << endl;

    // **************************************************************

    cout << "----- Testing Board.hpp -----" << endl;

    // constructs a board class
    Board* test_board = new Board(TRAIT_FILE, CHAMPION_FILE);

    /* wrapper methods that won't be tested:
    int Champs_In_Set();
    int Traits_In_Set();
    int Back();
    vector<string> All_Champions();
    vector<string> All_Traits();
    vector<int> Level_Restriction();
    */

    cout << "Average score increase = " << test_board->Average_Score_Increase() << endl;
    cout << "Maximum score increase = " << test_board->Maximum_Score_Increase() << endl;

    cout << "Fielding 5 (Taric)" << endl;
    if(test_board->Field_Champion(5, true)){
        cerr << "Field with synergy failed" << endl;
    }

    if(test_board->Field_Champion(5, false)){
        cout << "Board: ";
        cout << test_board->Uncompress_Champion(test_board->Get_Board()[test_board->Size() - 1]) << endl;
    }
    cout << "Unfielding from last added" << endl;
    test_board->Unfield_Champion();

    cout << "Shadow fielding 2 (Naafiri)" << endl;
    test_board->Shadow_Field_Champion(2);
    cout << "Board Size (should be 0) = " << test_board->Size() << endl;

    cout << "Testing Field with synergy" << endl;
    if(test_board->Field_Champion(6, true)){
        cout << "Success" << endl;
        test_board->Unfield_Champion();
    }else{
        cerr << "Field with synergy failed" << endl;
    }

    cout << "Testing Shadow trait field" << endl;
    test_board->Field_Trait(0);

    cout << "Score (should be 2) = " << test_board->Score() << endl;

    cout << "Testing shadow unfield" << endl;
    test_board->Shadow_Unfield_Champion(2);
    if(test_board->Field_Champion(6, true)){
        cerr << "Shadow unfield failed" << endl;
    }else{
        cout << "Success" << endl;
    }
    cout << "Testing cost restriction" << endl;
    test_board->Set_Cost_Restriction(4);
    if(test_board->Field_Champion(1, false)){
        cerr << "Cost Restriction Failed" << endl;
    }else{
        cout << "Cost Restriction works" << endl;
    }

    // testing the deconstructor
    cout << "deconstructing" << endl;
    delete test_board;
    cout << "----- Board.hpp Successful -----" << endl;


    cout << "----- All Tests Passed -----" << endl;
    return 0;
}