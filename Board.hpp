/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-10-26
    Last Modified: 2023-10-28

    Description: Board.hpp
    Stack like board method
    ADT for creating, manipulating and calculating scores of a TFT board
*/
#ifndef BOARD
#define BOARD
// Dependencies
#include "Parser.hpp"
#include "Fast_Vector.hpp"
#include <algorithm>

class Board{
    private:
    // variables
    Database* db;
    int board_score;
    int cost_restriction;
    vector<int> current_board;
    
    int Max_Trait_Tier_Increase(Trait* current);
    float Normalized_trait_increase(Trait* current);
    
    
    public:
    Board(const string &traits_file, const string &champions_file);
    Board(const Board &a_board);
    ~Board();
    // methods
    int Champions_In_Set();
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
    int Compress_Trait(const string &trait);

};
#endif