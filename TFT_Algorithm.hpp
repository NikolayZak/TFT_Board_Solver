#ifndef DATABASE
#define DATABASE
using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include "Embeded_text/Champions.hpp"
#include "Embeded_text/Traits.hpp"
#include "Embeded_text/Prompts.hpp"

#define HASHED_BOARDS "Saved_boards.txt"

/*
    MAX_INCREASE should always be the highest marginal increase of any champion, However, you can get away with lowering it
    to the 3ird highest if you put the first and second highest in the first 2 slots CHAMPIONS_CSV

    BLANK_SCORE is one of the most important variables, it should be initialized between 0 and the
    maximum score increase from any given champion being added
    the best value of Blank_score should be around the average gain of any given champion ~6
    However increasing this value will improve search results finding better boards at the cost of computing time.
    NOTE: BLANK_SCORE can be a float Ex: 5.5 or 6.5


    ***********      Summary     ***********
    when BLANK_SCORE == MAX_INCREASE you have THE best possible boards
    but you sacrifice exponentially LARGE COMPUTING TIME
    try to make BLANK_SCORE >= AVERAGE_MARGINAL_INCREASE_OF_A_CHAMPION
    Update: I have made a function in the class called "average_marginal_increase() which returns the average for the set"
    NOTE: setting BLANK_SCORE to exactly the average does NOT guarentee perfect boards
    NOTE 2: the champions that are most favoured and that have the largest individual increases (3 trait champs)
    should be at the top of CHAMPIONS_CSV as they will make the program faster and produce better solutions.
                

*/
#define BLANK_SCORE 6
#define MAX_INCREASE 7
// **************************************************

//database structures
struct trait_Node {
    int fielded;
    int score;
    string name;
    vector<int> tiers;
    vector<int> tier_values;
};

struct champion_Node {
    string name;
    int cost;
    vector<trait_Node*> traits;
};


// *************************************************     Notes       *************************************************************
// It would be nice to sort the function names
// uses automatic local hashing if the execution took longer then 6 seconds



class tft_database {
    private:
    // Datastructure
    vector<trait_Node*> trait_root;
    vector<champion_Node*> champion_root;

    // Key variables
    vector<int> level_chance = {1, 1, 2, 3, 4, 4, 5};
    int P_champs_in_set;
    int P_traits_in_set;
    int P_score;
    int P_cost_limitation;
    int P_max_size;
    int P_highscore;
    
    // Hashing variables
    string P_barcode;
    string P_hashed;

    // Board variables
    vector<vector<int>> P_computed_boards;
    vector<int> P_current_board;
    vector<int> P_champs_added;
    vector<int> P_emblems_added;

    //private functions
    trait_Node* createTraitNode();
    trait_Node* find_Node(string &name);
    vector<trait_Node*> load_trait_reg();
    vector<champion_Node*> load_champ_reg();
    bool check_vec(vector<int> vec, int item);
    bool feild_champ_with_synergy(int champ);
    bool feild_champ(int champ);
    void unfeild_champion(int champ);
    void compute_boards(vector<int> board); // recursive function that handles solving all the optimal boards in a reasonable runtime
    void add_added_champs_to_computed_boards();
    bool check_hashed();
    string convert_board(vector<int> board);
    string convert_boards_to_string();
    string convert_boards_to_int_string();
    void create_barcode();
    void save_board();
    void print_traits();
    void print_champions();
    int calculateLevenshteinDistance(const string& str1, const string& str2);
    string find_closest_trait(string& misspelled);
    string find_closest_champ(string& misspelled);
    


    public:
    tft_database();
    ~tft_database();

    //public functions
    void set_max_size(int size){ P_max_size = size; }
    string return_optimal_boards(); // this is the big boy function that uses most of the code
    void feild_trait(int trait);
    void feild_champion(int champ);
    void reset();
    void cost_restriction(int x){ P_cost_limitation = x; }
    void default_cost_restriction();
    int get_champ_input();
    int get_trait_input();
    int board_size(){ return P_champs_added.size(); }
    string champ_i_to_s(int value){ return champion_root[value]->name; }
    string trait_i_to_s(int value){ return trait_root[value]->name; }
    void print_custom_settings();
    float average_marginal_increase();
};



#endif