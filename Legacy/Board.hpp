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
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include "StaticVariables.hpp"
#include "Common.hpp"
#include "LocalDB.hpp"
#include "FastVector.hpp"
#include "TopScoringBoards.hpp"

using std::string;
using std::vector;
using std::cerr;
using std::endl;

class Board {
    private:
    // variables
    SetData set_data;
    FastVector current_board;
    int current_board_score;

    public:
    // methods
    Board(const SetData& data);
    ~Board();
    
    void UpdateSetData(const SetData& data, int player_level, const vector<string> &traits_added, const vector<string> &champions_added);
    BoardResult ConvertBoard(const BoardNode &current);
    vector<BoardResult> ConvertBoardsAndClearHeap(TopScoringBoards &heap);
    void AddTrait(int trait_id);
    void RemoveTrait(int trait_id);
    void PushChampion(int champion_id);
    void PopChampion();
    BoardNode GetBoard();
    void InitialiseMaxChampionIncrease(int (&table)[MAX_PLAYER_LEVEL][MAX_CHAMPIONS]);

    // pass through methods
    int ChampionsInSet() const { return set_data.champion_count; }
    int TraitsInSet() const { return set_data.trait_count; }
    int Back() const { return current_board.back(); }
    int Size() const { return current_board.size(); }
    int GetScore() const { return current_board_score; }
};