#include "Board.hpp"

// initializes a board class with setdata
Board::Board(const SetData& data){
    if(data.champion_count > MAX_CHAMPIONS) {
        cerr << "Error: SetData exceeds maximum champion count." << endl;
        exit(EXIT_FAILURE);
    }
    current_board_score = 0;
    current_board = FastVector();
    set_data.copySet(data);
}

// deconstructor
Board::~Board() {
    set_data.deallocSet();
}

// optimizes the board class to operate on restricted set data
void Board::UpdateSetData(const SetData& data, int player_level, const vector<string> &traits_added, const vector<string> &champions_added) {
    set_data.deallocSet(); // Free the old set data
    set_data.copySet(data); // Copy the new set data

    current_board = FastVector(); // Reset the current board
    current_board_score = 0; // Reset the score

    for(int i = 0; i < traits_added.size(); i++) {
        AddTrait(data.findTraitIndex(traits_added[i])); // add the traits
    }

    vector<int> champion_indexes; // create and fill a vector with the indexes
    for(int i = 0; i < champions_added.size(); i++) {
        int current = data.findChampionIndex(champions_added[i]);
        PushChampion(current); // Add the champions to the board
        champion_indexes.push_back(current);
    }

    set_data.restrictSet(player_level, champion_indexes);

    // Check if the new set data exceeds the maximum champion count
    if(data.champion_count > MAX_CHAMPIONS) {
        cerr << "Error: SetData exceeds maximum champion count." << endl;
        exit(EXIT_FAILURE);
    }
}

// converts a BoardNode to a BoardResult
BoardResult Board::ConvertBoard(const BoardNode &current) {
    BoardResult answer;
    answer.score = current.board_score;
    for(int i = 0; i < current.board.size(); i++) {
        Champion* champion = set_data.champions[current.board[i]];
        answer.board.push_back(champion->name);
    }
    return answer;
}

// clears the heap and returns the boards
vector<BoardResult> Board::ConvertBoardsAndClearHeap(TopScoringBoards &heap){
    vector<BoardResult> result;
    while(!heap.empty()) {
        BoardNode entry = heap.top();
        heap.pop();
        BoardResult board_data = ConvertBoard(entry);
        result.push_back(board_data);
    }
    reverse(result.begin(), result.end()); // Reverse to get highest scores first
    return result;
}

// adds a trait to the board
void Board::AddTrait(int trait_id) {
    current_board_score += set_data.traits[trait_id]->Increment();
}

// removes a trait from the board
void Board::RemoveTrait(int trait_id) {
    current_board_score += set_data.traits[trait_id]->Decrement();
}

// adds a champion to the board
void Board::PushChampion(int champion_id) {
    current_board.push_back(champion_id);
    current_board_score += set_data.champions[champion_id]->IncrementTraits();
}

// removes the largest champion added
void Board::PopChampion(){
    int champion_id = current_board.pop_back();
    current_board_score += set_data.champions[champion_id]->DecrementTraits();
}

// returns a board node
BoardNode Board::GetBoard() {
    return {current_board, current_board_score};
}

// calculates the largest possible champion increase
int Board::CalculateMaxChampionIncrease() {
    vector<int> max_trait_increases;
    int max_trait_tier_increase = 0;

    // calculate the maximum trait tier increase for each trait
    for(int i = 0; i < set_data.trait_count; i++) {
        max_trait_tier_increase = 0;
        for(int j = 0; j < MAX_TRAIT_TIERS; j++) {
            if(set_data.traits[i]->value[j] > max_trait_tier_increase) {
                max_trait_tier_increase = set_data.traits[i]->value[j];
            }
        }
        max_trait_increases.push_back(max_trait_tier_increase);
    }

    // calculate the maximum champion increase based on traits
    int max_champion_increase = 0;
    for(int i = 0; i < set_data.champion_count; i++) {
        int champion_increase = 0;
        for(int j = 0; j < set_data.champions[i]->num_traits; j++) {
            string current_trait = set_data.champions[i]->traits[j]->name;
            for(int k = 0; k < set_data.trait_count; k++) {
                if(set_data.traits[k]->name == current_trait) {
                    champion_increase += max_trait_increases[k];
                }
            }
        }
        if(champion_increase > max_champion_increase) {
            max_champion_increase = champion_increase;
        }
    }
    return max_champion_increase;
}