#include "newSolver.hpp"

Solver::Solver(const SetData& data, int heap_size) : B(data), optimal_boards(heap_size){
    max_champion_increase = B.CalculateMaxChampionIncrease();
}

Solver::~Solver() {
}

void Solver::UpdateData(const SetData& data, int player_level, const vector<int> &champions_added, const vector<int> &traits_added) {
    B.UpdateSetData(data, player_level, champions_added);
    for (int trait_id : traits_added) {
        B.AddTrait(trait_id);
    }
    max_champion_increase = B.CalculateMaxChampionIncrease();
}

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
    int max_increase = max_champion_increase * (champions_remaining - 1);
    while(counter <= max_index) {
        B.PushChampion(counter);
        if(B.GetScore() + max_increase >= highscore) {
            SolveBoardsRec();
        }
        B.PopChampion();
        counter++;
    }
}

vector<BoardResult> Solver::Solve(int target_size) {
    this->target_size = target_size;
    highscore = 0;
    optimal_boards.clear();

    auto start = chrono::high_resolution_clock::now();
    SolveBoardsRec();
    auto end = chrono::high_resolution_clock::now();
    runtime = chrono::duration<float, milli>(end - start).count() / 1000.0f; // Convert to seconds
    
    vector<BoardResult> result;
    while(!optimal_boards.empty()) {
        BoardNode entry = optimal_boards.top();
        optimal_boards.pop();
        vector<string> board_strings = B.ConvertBoard(entry.board);
        sort(board_strings.begin(), board_strings.end());
        result.push_back({board_strings, entry.board_score});
    }
    reverse(result.begin(), result.end()); // Reverse to get highest scores first
    return result;
}