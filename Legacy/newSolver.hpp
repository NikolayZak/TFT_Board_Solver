/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-6-1
    Last Modified: 2025-6-1

    Description: Solver.hpp
    Algorithm using tft Board methods to calculate the optimal tft board
*/
#pragma once
#include "newBoard.hpp"
#include <chrono>

class Solver {
    private:
    // variables
    Board B;
    vector<vector<string>> optimal_boards;
    int highscore;
    int target_size;
    int max_increase;
    float blank_score;
    float runtime;

    void SolveBoardsRec();

    public:
    Solver(const SetData& data);
    ~Solver();
    void UpdateData(const SetData& data, int player_level, const vector<int> &champions_added, const vector<int> &traits_added);
    vector<vector<string>> Solve(int target_size);


};