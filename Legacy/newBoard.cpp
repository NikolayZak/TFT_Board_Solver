#include "newBoard.hpp"

Board::Board(const SetData& data){
    current_board_score = 0;
    current_board = Fast_Vector();
    set_data.copySet(data);
}

Board::~Board() {
    set_data.deallocSet();
}

void Board::UpdateSetData(const SetData& data, int player_level, const vector<int> &champions_added) {
    set_data.deallocSet(); // Free the old set data
    set_data.copySet(data); // Copy the new set data
    for(int i = 0; i < champions_added.size(); i++) {
        PushChampion(champions_added[i]); // Add the champions to the board
    }
    set_data.restrictSet(player_level, champions_added);
    current_board = Fast_Vector(); // Reset the current board
    current_board_score = 0; // Reset the score
}

void Board::AddTrait(int trait_id) {
    set_data.traits[trait_id]->Increment();
}

void Board::RemoveTrait(int trait_id) {
    set_data.traits[trait_id]->Decrement();
}

void Board::PushChampion(int champion_id) {
    current_board.push_back(champion_id);
    set_data.champions[champion_id]->IncrementTraits();
}

void Board::PopChampion(){
    int champion_id = current_board.pop_back();
    set_data.champions[champion_id]->DecrementTraits();
}