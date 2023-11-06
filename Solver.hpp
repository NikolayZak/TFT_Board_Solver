/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-10-26
    Last Modified: 2023-10-27

    Description: Solver.hpp
    Algorithm using tft Board methods to calculate the optimal tft board
*/
#ifndef SOLVER
#define SOLVER
// Dependencies
#include "Board.hpp"
#include <chrono>

class Solver{
    private:
    // variables
    Board* B;
    vector<vector<int>> optimal_boards;
    vector<int> champions_required;
    vector<int> traits_required;
    int highscore;
    int target_size;
    int max_increase;
    float blank_score;
    int cost_restriction;
    float runtime;


    void Solve_Boards_Rec();
    bool check_vec(const vector<int> &vec, const int &item);
    void Private_Cost_Restriction();

    public:
    // functions
    Solver(const string &traits_file, const string &champions_file);
    ~Solver();

    void Max_Increase(const int &value);
    void Blank_Score(const float &value);
    void Cost_Restriction(const int &value); // setter
    int Cost_Restriction(); // getter
    void Add_Champion(const int &int_champion);
    void Add_Trait(const int &int_trait);
    void Compute_Optimal_Boards(const int &size);
    vector<string> Get_All_Champions();
    vector<string> Get_All_Traits();
    vector<int> Champions_Added();
    vector<int> Traits_Added();
    vector<vector<string>> Optimal_Boards();
    vector<vector<int>> Compressed_Optimal_Boards();
    void Reset();
    float Runtime();

    // compression and extraction
    vector<vector<string>> Uncompress_Champions(const vector<vector<int>> &boards);
    vector<string> Uncompress_Champions(const vector<int> &board);
    string Uncompress_Champions(const int &champion);

    vector<string> Uncompress_Traits(const vector<int> &traits);
    string Uncompress_Traits(const int &trait);

    int Compress_Traits(const string &trait);
    int Compress_Champions(const string &champion);

};


#endif