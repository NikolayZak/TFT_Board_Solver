/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-5-31
    Last Modified: 2025-5-31

    Description: Board.hpp
    Stack like board method
    ADT for creating, manipulating and calculating scores of a TFT board
*/
#pragma once
// Dependencies
#include "LocalDB.hpp"
#include "Fast_Vector.hpp"

class Board {
    private:
    // variables
    SetData set_data;
    Fast_Vector current_board;
    int current_board_score;

    public:
    // methods
    Board(const SetData& data);
    ~Board();
    void UpdateSetData(const SetData& data, const vector<int> &champions_added);

    void AddTrait(int trait_id);
    void RemoveTrait(int trait_id);
    void PushChampion(int champion_id);
    void PopChampion();
    int GetScore() const { return current_board_score; }
    Fast_Vector GetBoard() const { return current_board; }

};