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
    
    current_board = FastVector(); // eliminate dangling champions added

    set_data.restrictSet(player_level, champion_indexes);

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

// makes sure the finish is not bounds and if so, does not add them
// adds [start,finish) exclusive
int PedanticSum(vector<int> A, int start, int finish){
    if(A.size() < finish){
        finish = A.size();
    }
    int ans = 0;
    for(int i = start; i < finish; i++){
        ans += A[i];
    }
    return ans;
}

// calculates the largest possible champion increase
void Board::InitialiseMaxChampionIncrease(int (&table)[MAX_BOARD_SIZE][MAX_CHAMPIONS]){
    vector<int> max_trait_increases;
    vector<int> max_champion_increase;

    // calculate the maximum trait tier increase for each trait
    for(int i = 0; i < set_data.trait_count; i++) {
        int max_trait_tier_increase = 0;
        for(int j = 0; j < MAX_TRAIT_TIERS; j++) {
            if(set_data.traits[i]->value[j] > max_trait_tier_increase) {
                max_trait_tier_increase = set_data.traits[i]->value[j];
            }
        }
        max_trait_increases.push_back(max_trait_tier_increase);
    }

    // calculate the maximum champion increase based on traits
    for(int i = 0; i < set_data.champion_count; i++) { // for every champion
        int champion_increase = 0;
        for(int j = 0; j < set_data.champions[i]->num_traits; j++) { // for every champ trait
            string current_trait = set_data.champions[i]->traits[j]->name;
            for(int k = 0; k < set_data.trait_count; k++) { // find the trait increase
                if(set_data.traits[k]->name == current_trait) {
                    champion_increase += max_trait_increases[k];
                }
            }
        }
        max_champion_increase.push_back(champion_increase);
    }

    // initialise to 0
    for(int i = 0; i < MAX_BOARD_SIZE; i++){
        for(int j = 0; j < MAX_CHAMPIONS; j++){
            table[i][j] = 0;
        }
    }

    // accumulate valid indexes
    for(int i = 0; i < MAX_BOARD_SIZE; i++){
        for(int j = 0; j < set_data.champion_count; j++){
            if(i + j >= set_data.champion_count){
                continue; // goes out of bounds
            }
            
            // add the first champion
            table[i][j] = max_champion_increase[j];

            if(i == 0){
                continue; // top k not needed
            }
            
            // Select top k values
            vector<int> top_k(i);
            std::partial_sort_copy(max_champion_increase.begin() + j + 1, max_champion_increase.end(), top_k.begin(), top_k.end(), std::greater<int>());

            // Store their sum
            table[i][j] += std::accumulate(top_k.begin(), top_k.end(), 0);
        }
    }
}