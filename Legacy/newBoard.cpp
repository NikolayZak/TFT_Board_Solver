#include "newBoard.hpp"

Board::Board(const SetData& data){
    current_board_score = 0;
    current_board = Fast_Vector();
    set_data.copySet(data);
}

Board::~Board() {
    set_data.deallocSet();
}

// NOT FINISHED
void Board::UpdateSetData(const SetData& data, const vector<int> &champions_added) {
    set_data.deallocSet(); // Free the old set data
    set_data.copySet(data); // Copy the new set data
    current_board = Fast_Vector(); // Reset the current board
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