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
    
    void UpdateSetData(const SetData& data, int player_level, const vector<int> &champions_added);
    vector<string> ConvertBoard(const Fast_Vector &Fast);
    void AddTrait(int trait_id);
    void RemoveTrait(int trait_id);
    void PushChampion(int champion_id);
    void PopChampion();
    BoardEntry GetBoard();

    // pass through methods
    int ChampionsInSet() const { return set_data.champion_count; }
    int TraitsInSet() const { return set_data.trait_count; }
    int Size() const { return current_board.size(); }
    int GetScore() const { return current_board_score; }
};