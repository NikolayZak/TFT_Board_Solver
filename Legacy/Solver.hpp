/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-6-1
    Last Modified: 2025-6-1

    Description: Solver.hpp
    Algorithm using tft Board methods to calculate the optimal tft board
*/
#pragma once
#include "Board.hpp"
#include "TopScoringBoards.hpp"
#include <chrono>

class Solver {
    private:
    // variables
    Board B;
    TopScoringBoards optimal_boards;
    int highscore;
    int target_size;
    int max_champion_increase;
    float runtime;

    void SolveBoardsRec();

    public:
    Solver(const SetData& data, int heap_size);
    ~Solver();
    void UpdateData(const SetData& data, int player_level, const vector<int> &champions_added, const vector<int> &traits_added);
    vector<BoardResult> Solve(int target_size);
    float Runtime() const { return runtime; }


};