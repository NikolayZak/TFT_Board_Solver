#include "newBoard.hpp"

Board::Board(const SetData& data){
    if(data.champion_count > MAX_CHAMPIONS) {
        cerr << "Error: SetData exceeds maximum champion count." << endl;
        exit(EXIT_FAILURE);
    }
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

    // Check if the new set data exceeds the maximum champion count
    if(data.champion_count > MAX_CHAMPIONS) {
        cerr << "Error: SetData exceeds maximum champion count." << endl;
        exit(EXIT_FAILURE);
    }
    // Reset the current board and score
    current_board = Fast_Vector(); // Reset the current board
    current_board_score = 0; // Reset the score
}

vector<string> Board::ConvertBoard(const Fast_Vector &Fast) {
    vector<string> result;
    for(int i = 0; i < Fast.size(); i++) {
        result.push_back(set_data.champions[Fast[i]]->name);
    }
    return result;
}

void Board::AddTrait(int trait_id) {
    current_board_score += set_data.traits[trait_id]->Increment();
}

void Board::RemoveTrait(int trait_id) {
    current_board_score += set_data.traits[trait_id]->Decrement();
}

void Board::PushChampion(int champion_id) {
    current_board.push_back(champion_id);
    current_board_score += set_data.champions[champion_id]->IncrementTraits();
}

void Board::PopChampion(){
    int champion_id = current_board.pop_back();
    current_board_score += set_data.champions[champion_id]->DecrementTraits();
}

BoardEntry Board::GetBoard() {
    return {current_board, current_board_score};
}