#include "newSolver.hpp"

Solver::Solver(const SetData& data) : B(data) {
    // Constructor initializes the board with the provided set data
}

Solver::~Solver() {
    // Destructor does not need to do anything as Board's destructor will handle cleanup
}

void Solver::UpdateData(const SetData& data, int player_level, const vector<int> &champions_added, const vector<int> &traits_added) {
    B.UpdateSetData(data, player_level, champions_added);
    for (int trait_id : traits_added) {
        B.AddTrait(trait_id);
    }
}

void Solver::SolveBoardsRec() {
    // base case: empty board
    if(B.Size() == 0) {
        for(int i = 0; i < B.ChampionsInSet(); i++) {
            B.PushChampion(i);
            SolveBoardsRec();
            B.PopChampion();
        }
        return;
    }

    // case final board size reached
    if(B.Size() == target_size){}
}

vector<vector<string>> Solver::Solve(int target_size) {
    this->target_size = target_size;
    highscore = 0;
    optimal_boards.clear();
    SolveBoardsRec();
   
}