/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-10-26
    Last Modified: 2023-10-28

    Description: Board.hpp
    ADT for creating, manipulating and calculating scores of a TFT board
*/
#ifndef BOARD
#define BOARD
// Dependencies
#include "Database.hpp"
#include <algorithm>

class Board{
    private:
    // variables
    Database* db;
    int board_score;
    int cost_restriction;
    vector<int> current_board;
    
    //float Average_Trait_Tier_Increase(Trait* current);
    //int Max_Trait_Tier_Increase(Trait* current);
    
    
    public:
    Board(const string &traits_file, const string &champions_file);
    ~Board();
    //vector<string> Get();  Legacy
    int Back();
    int Size();
    vector<int> Get_Compressed();
    vector<string> Uncompress_Champions(const vector<int> &compressed);
    vector<string> Uncompress_Traits(const vector<int> &compressed);
    string Uncompress_Champion(const int &champion);
    string Uncompress_Trait(const int &trait);
    int Score();
    bool Field_Champion(const int &int_champion, const bool &synergy); // housekeeping
    void Shadow_Field_Champion(const int &int_chamption); // shadow
    void Unfield_Champion(); // housekeeping
    void Unfield_Champion(const int &int_champion); // shadow
    void Field_Trait(const int &int_trait);
    void Unfield_Trait(const int &int_trait);
    int Champs_In_Set();
    int Traits_In_Set();
    float Average_Score_Increase();
    int Maximum_Score_Increase();
    vector<string> Get_Champions();
    vector<string> Get_Traits();
    void Set_Cost_Restriction(const int &cost);
    vector<int> Level_Restriction();
    int Compress_Champion(const string &champion);
    int Compress_Trait(const string &trait);

};
#endif