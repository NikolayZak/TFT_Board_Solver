#include "Solver.hpp"

Solver::Solver(const SetData& data, int heap_size) : B(data), optimal_boards(heap_size){
    B.InitialiseMaxChampionIncrease(max_potential_increase);
    target_size = 0;
}

Solver::~Solver() {
    
}

void Solver::UpdateData(const SetData& data, int player_level, const vector<string> &traits_added, const vector<string> &champions_added) {
    B.UpdateSetData(data, player_level, traits_added, champions_added);
    B.InitialiseMaxChampionIncrease(max_potential_increase);
}

// maintains a default board position after complete
void Solver::SolveBoardsRec() {
    // case: empty board
    if(B.Size() == 0) {
        for(int i = 0; i < B.ChampionsInSet(); i++) {
            B.PushChampion(i);
            SolveBoardsRec();
            B.PopChampion();
        }
        return;
    }

    // case: final board size reached
    if(B.Size() == target_size){
        if(B.GetScore() > highscore){
            highscore = B.GetScore();
        }
        optimal_boards.push(B.GetBoard());
        return;
    }

    // case: champion on board & needs to add a champ
    int counter = B.Back() + 1;
    int champions_remaining = target_size - B.Size();
    int max_index = B.ChampionsInSet() - champions_remaining;
    while(counter <= max_index) {
        // calculate the potential increase from this champion and extra champions to add
        if(B.GetScore() + max_potential_increase[champions_remaining - 1][counter] >= highscore) {
            B.PushChampion(counter);
            SolveBoardsRec();
            B.PopChampion();
        }
        counter++;
    }
}

vector<BoardResult> Solver::Solve(int target_size) {
    highscore = 0;
    optimal_boards.clear();

    this->target_size = target_size;

    SolveBoardsRec();

    vector<BoardResult> result = B.ConvertBoardsAndClearHeap(optimal_boards);
    return result;
}