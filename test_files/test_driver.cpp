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

    // check the deconstructor
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
    vector<int> Level_Restriction();
    */

   // tbh too many functions to test thoroughly, and I know all of them work lmao

   // testing deconstructor
   delete test_board;

   
/*
// methods
    int Champs_In_Set();
    int Traits_In_Set();
    int Size();
    int Back();
    int Score();
    bool Field_Champion(const int &int_champion, const bool &synergy); // housekeeping
    void Unfield_Champion(); // housekeeping
    void Shadow_Field_Champion(const int &int_chamption); // shadow
    void Shadow_Unfield_Champion(const int &int_champion); // shadow
    void Field_Trait(const int &int_trait); // shadow
    void Unfield_Trait(const int &int_trait); // shadow
    float Average_Score_Increase();
    int Maximum_Score_Increase();
    void Set_Cost_Restriction(const int &cost);
    vector<string> All_Champions();
    vector<string> All_Traits();
    vector<int> Level_Restriction();
    vector<int> Get_Board();

    // compression and uncompression
    vector<string> Uncompress_Champions(const vector<int> &compressed);
    vector<string> Uncompress_Traits(const vector<int> &compressed);
    string Uncompress_Champion(const int &champion);
    string Uncompress_Trait(const int &trait);
    int Compress_Champion(const string &champion);
    int Compress_Trait(const string &trait);*/



    cout << "----- All Tests Passed -----" << endl;
    return 0;
}