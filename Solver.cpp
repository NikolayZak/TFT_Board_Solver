#include "Solver.hpp"
/*          Time Complexity
    let t be the number of traits in the set
    let t` be the number of traits any given champ has
    let t`` be the number of tiers any given trait has
    let c be the number of champions in the set
    let c` be the number of champions on a given board
    let b be the number of boards given
*/


// constructor   O(c*t` + t*t``)
Solver::Solver(const string &traits_file, const string &champions_file){
    B = new Board(traits_file, champions_file);
    highscore = 0;
    target_size = 1;
    cost_restriction = 0;
    blank_score = B->Average_Score_Increase();
    max_increase = B->Maximum_Score_Increase();
    //std::cout << "Blank_Score: " << blank_score << endl;
    //std::cout << "Max_increase: " << max_increase << endl;
}

// deconstructor   O(t+c)
Solver::~Solver(){
    delete B;
    B = nullptr;
    optimal_boards.clear();
    champions_required.clear();
    traits_required.clear();
}

// Recursively generates boards and saves the highest scores   O((c choose c')*(t`*t``))
void Solver::Solve_Boards_Rec(){
    //base case, empty board
    if(B->Size() == 0){
        for(int i = 0; i < B->Champs_In_Set(); i++){
            if(check_vec(champions_required, i) && B->Field_Champion(i, false)){
                Solve_Boards_Rec();
                B->Unfield_Champion();
            }
        }
        return;
    }

    // case final board size reached
    if(B->Size() == target_size - (int)champions_required.size()){
        if(highscore < B->Score()){
            highscore = B->Score();
            optimal_boards.clear();
            optimal_boards.push_back(B->Get_Compressed());

        }else if(highscore == B->Score()){
            optimal_boards.push_back(B->Get_Compressed());
        }
        return;
    }

    // case champion on board & needs to add a champ
    int counter = B->Back() + 1;
    int max_index = B->Champs_In_Set() - (target_size - (int)champions_required.size() - B->Size());
    while(counter <= max_index){
        if(check_vec(champions_required, counter)){ // check if the champ is already added

            if(B->Size() + (int)champions_required.size() >= target_size - 2){ // checks if it's the last 2 champs
                if(B->Field_Champion(counter, true)){ // checks for a required synergy
                    // check if it's within a good score if(current_score + MAX_INCREASE * champs_remaining >= P_highscore)
                    if(B->Score() + (max_increase * (target_size - (int)champions_required.size() - B->Size())) >= highscore){
                        Solve_Boards_Rec();
                    }
                    B->Unfield_Champion();
                }
            }else{ // It's not the last 2 champs check if it's within a good scores
                if(B->Field_Champion(counter, false)){
                    // check if it's within a good score if(current_score + BLANK_SCORE * champs_remaining >= P_highscore)
                    if(B->Score() + (blank_score * (target_size - (int)champions_required.size() - B->Size())) >= highscore){
                        Solve_Boards_Rec();
                    }
                    B->Unfield_Champion();
                }
            }
        }
        counter++;
    }
}


// used to check if an item is in a vector   O(n)
bool Solver::check_vec(const vector<int> &vec, const int &item){
    for(int i = 0; i < (int)vec.size(); i++){
        if(vec[i] == item){
            return false;
        }
    }
    return true;
}

// sets the cost restriction to the correct value
void Solver::Private_Cost_Restriction(){
    if(cost_restriction == 0){
        vector<int> level_restriction = B->Level_Restriction();
        int cost;

        if(target_size > (int)level_restriction.size()){
            cost = level_restriction.back();
        }else{
            cost = level_restriction[target_size - 1];
        }
        B->Set_Cost_Restriction(cost);

    }else{
        B->Set_Cost_Restriction(cost_restriction);
    }
}

// used to reset the variables for the recursive function   O((c choose c')*(t`*t``))
// precondition: target size must be set
void Solver::Compute_Optimal_Boards(const int &size){
    // start timing
    auto start_time = std::chrono::high_resolution_clock::now();

    // clean up
    target_size = size;
    highscore = 0;
    optimal_boards.clear();
    Private_Cost_Restriction();

    // solve
    Solve_Boards_Rec();

    // add in requisites if needed
    for(auto &board : optimal_boards){
        for(const auto &champ : champions_required){
            board.push_back(champ);
        }
    }

    // end time
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    runtime = static_cast<float>(duration.count());
}

// returns the raw solved boards   O(1)
// precondtion: boards must be computed!
vector<vector<int>> Solver::Compressed_Optimal_Boards(){
    return optimal_boards;
}

// adds the required champions and converts to strings   O(c`*b)
// precondtion: boards must be computed!
vector<vector<string>> Solver::Optimal_Boards(){
    vector<vector<string>> boards;
    for(auto &board : optimal_boards){
        boards.push_back(B->Uncompress_Champions(board));
    }
    return boards;
}

// unzips a compressed vector of boards   O(n^2)
vector<vector<string>> Solver::Uncompress_Boards(const vector<vector<int>> &boards){
    vector<vector<string>> uncompressed;
    for(const auto &board : boards){
        uncompressed.push_back(B->Uncompress_Champions(board));
    }
    return uncompressed;
}

// unzips a compressed board   O(n)
vector<string> Solver::Uncompress_Board(const vector<int> &board){
    return B->Uncompress_Champions(board);
}

// unzips a compressed vector of traits   O(n)
vector<string> Solver::Uncompress_Traits(const vector<int> &traits){
    return B->Uncompress_Traits(traits);
}

// Returns a vector of all the champion names   O(c)
vector<string> Solver::Get_All_Champions(){
    return B->Get_Champions();
}

// Returns a vector of all the trait names   O(t)
vector<string> Solver::Get_All_Traits(){
    return B->Get_Traits();
}

// Uses a shadow add and keeps a local account   O(t'*t'')
void Solver::Add_Champion(const int &int_champ){
    B->Shadow_Field_Champion(int_champ);
    champions_required.push_back(int_champ);
}

// adds a trait to the database and keeps a local account   O(t``)
void Solver::Add_Trait(const int &int_trait){
    B->Field_Trait(int_trait);
    traits_required.push_back(int_trait);
}


// sets the max increase   O(1)
void Solver::Max_Increase(const int &value){
    max_increase = value;
}

// sets the average blank score   O(1)
void Solver::Blank_Score(const float &value){
    blank_score = value;
}

// sets the cost restriction   O(1)
// Note: a value of 0 is default for level chance specified
void Solver::Cost_Restriction(const int &cost){
    cost_restriction = cost;
}

// resets the solver's cost restrictions, traits and champions added O(1)
void Solver::Reset(){
    for(const auto &trait : traits_required){
        B->Unfield_Trait(trait);
    }
    for(const auto &champ : champions_required){
        B->Unfield_Champion(champ);
    }
    champions_required.clear();
    traits_required.clear();
    cost_restriction = 0;
}

// returns the current cost restriction
int Solver::Cost_Restriction(){
    return cost_restriction;
}

// returns the Champions added
vector<int> Solver::Champions_Added(){
    return champions_required;
}

// returns the traits added
vector<int> Solver::Traits_Added(){
    return traits_required;
}

// returns the runtime of the solver
float Solver::Runtime(){
    return runtime;
}

// returns the string of the champion
string Solver::Champion_To_String(const int &champion){
    return B->Uncompress_Champion(champion);
}

// returns the string of the trait
string Solver::Trait_To_String(const int &trait){
    return B->Uncompress_Trait(trait);
}