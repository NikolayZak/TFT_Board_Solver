#include "Multithreading.hpp"

// Constructor
Multithreaded_Solver::Multithreaded_Solver(const int &threads, const string &traits_file, const string &champions_file){
    num_threads = threads;
    Solver* worker = new Solver(traits_file, champions_file);
    worker->Blank_Score(worker->Max_Increase());
    Workspace.push_back(worker);
    for(int i = 1; i < threads; i++){
        Workspace.push_back(new Solver(*worker));
    }
    champions_in_set = Workspace[0]->Champions_In_Set();
    global_highscore = 0;
}

// copy constructor
Multithreaded_Solver::Multithreaded_Solver(const int &threads, const Solver &main){
    num_threads = threads;
    for(int i = 0; i < threads; i++){
        Workspace.push_back(new Solver(main));
    }
    champions_in_set = Workspace[0]->Champions_In_Set();
    global_highscore = 0;
}

// Deconstructor
Multithreaded_Solver::~Multithreaded_Solver(){
    for(int i = 0; i < (int)Workspace.size(); i++){
        delete Workspace[i];
    }
    compressed_global_optimal.clear();
}

// Updates the solver settings
void Multithreaded_Solver::Update(const Solver &main){
    // clears the old memory
    for(int i = 0; i < (int)Workspace.size(); i++){
        delete Workspace[i];
    }
    Workspace.clear();
    compressed_global_optimal.clear();
    start.clear();
    end.clear();

    // loads in new solvers
    for(int i = 0; i < num_threads; i++){
        Workspace.push_back(new Solver(main));
    }
    champions_in_set = Workspace[0]->Champions_In_Set();
    global_highscore = 0;
}

// nCr function taken from online
long long Multithreaded_Solver::nCr(int n, int r) {
    if(r > n - r) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++) {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

/*
// helper function for debugging
void Multithreaded_Solver::Print_List(const vector<int> &my_list){
    for(int i = 0; i < (int)my_list.size(); i++){
        cout << my_list[i] << " ";
    }
    cout << endl;
}
*/

// Function that given a sorted list will give x lists later Ex: z = 9; {1,2,3,4} adding 7 = {1,2,4,6}
vector<int> Multithreaded_Solver::Add_To_List(vector<int> my_list, const long long &value){
    int index = 0;
    long long current = 0;
    int z = champions_in_set;
    int n = board_size;
    long long tester;
    while(current < value){
        tester = nCr(z-1, n-1);
        if(current + tester <= value){
            // increment
            current += tester;
            z--;
            // increment the list
            for(int i = index; i < (int)my_list.size(); i++){
                my_list[i] += 1;
            }
        }else{
            // go deeper in the list
            index++;
            z = champions_in_set - my_list[index];
            n--;
        }
    }
    return my_list;
}

// Configure subsets
void Multithreaded_Solver::Configure_Subsets(){
    long long total = nCr(champions_in_set, board_size);
    long long partition = total/num_threads;

    // create the default list
    vector<int> a_board;
    for(int i = 0; i < board_size; i++){
        a_board.push_back(i);
    }
    
    // put in the partitions
    for(int i = 0; i < num_threads; i++){
        // adding in the starts
        if(i != 0){
            start.push_back(Add_To_List(a_board, (partition * i) + 1));
        }else{
            start.push_back(a_board);
        }

        // adding in the ends
        if(i != num_threads - 1){
            end.push_back(Add_To_List(a_board, partition * (i + 1) ));
        }else{
            end.push_back(Add_To_List(a_board, total));
        }
    }
}

// helper function for combining vector<vector<int>> with a vector<vector<int>>
void Multithreaded_Solver::Combine_Boards(vector<vector<int>> &v1, vector<vector<int>> &v2){
    for(int i = 0; i < (int)v2.size(); i++){
        v1.push_back(v2[i]);
    }
}

// Solves the subsets and joins them together in the global optimal
void Multithreaded_Solver::Solve(const int &size){
    auto start_time = std::chrono::high_resolution_clock::now();
    board_size = size;
    global_highscore = -1;
    compressed_global_optimal.clear();
    start.clear();
    end.clear();
    Configure_Subsets();
    // call all the threads on their partitions
    vector<thread> threads;
    for(int i = 0; i < num_threads; i++){
        threads.emplace_back(&Solver::Subset_Optimal_Boards, Workspace[i], size, start[i], end[i]);
    }

    // Join all threads to wait for them to finish
    for (thread &t : threads) {
        t.join();
    }

    // combines all the worker threads
    for(int i = 0; i < num_threads; i++){
        if(Workspace[i]->Highscore() > global_highscore){
            global_highscore = Workspace[i]->Highscore();
            compressed_global_optimal.clear();
            compressed_global_optimal = Workspace[i]->Compressed_Optimal_Boards();

        }else if(Workspace[i]->Highscore() == global_highscore){
            vector<vector<int>> tmp = Workspace[i]->Compressed_Optimal_Boards();
            compressed_global_optimal.insert(compressed_global_optimal.end(), tmp.begin(), tmp.end());
        }
    }
    // end time
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    runtime = static_cast<float>(duration.count()) / 1000;
}


// returns the compressed optimal boards
vector<vector<int>> Multithreaded_Solver::Compressed_Optimal_Boards(){
    return compressed_global_optimal;
}

// returns the optimal boards
vector<vector<string>> Multithreaded_Solver::Optimal_Boards(){
    return Workspace[0]->Uncompress_Champions(compressed_global_optimal);
}

// returns the runtime;
float Multithreaded_Solver::Runtime(){
    return runtime;
}

// returns the max_increase
int Multithreaded_Solver::Max_Increase(){
    return Workspace[0]->Max_Increase();
}

// sets the max_increase
void Multithreaded_Solver::Max_Increase(const int &value){
    for(int i = 0; i < (int)Workspace.size(); i++){
        Workspace[i]->Max_Increase(value);
    }
}

// returns the blank_score
float Multithreaded_Solver::Blank_Score(){
    return Workspace[0]->Blank_Score();
}

// sets the blank_score
void Multithreaded_Solver::Blank_Score(const float &value){
    for(int i = 0; i < (int)Workspace.size(); i++){
        Workspace[i]->Blank_Score(value);
    }
}

// returns the cost_restriction
int Multithreaded_Solver::Cost_Restriction(){
    return Workspace[0]->Cost_Restriction();
}

// sets the cost_restriction
void Multithreaded_Solver::Cost_Restriction(const int &value){
    for(int i = 0; i < (int)Workspace.size(); i++){
        Workspace[i]->Cost_Restriction(value);
    }
}

// adds a champion
void Multithreaded_Solver::Add_Champion(const int &champion){
    for(int i = 0; i < (int)Workspace.size(); i++){
        Workspace[i]->Add_Champion(champion);
    }
}

// adds a trait
void Multithreaded_Solver::Add_Trait(const int &trait){
    for(int i = 0; i < (int)Workspace.size(); i++){
        Workspace[i]->Add_Trait(trait);
    }
}

// returns the string on all champions
vector<string> Multithreaded_Solver::Get_All_Champions(){
    return Workspace[0]->Get_All_Champions();
}

// returns the string on all traits
vector<string> Multithreaded_Solver::Get_All_Traits(){
    return Workspace[0]->Get_All_Traits();
}

// returns a vector of int of all the champs added
vector<int> Multithreaded_Solver::Champions_Added(){
    return Workspace[0]->Champions_Added();
}

// returns a vector of int of all the traits added
vector<int> Multithreaded_Solver::Traits_Added(){
    return Workspace[0]->Traits_Added();
}

// returns the number of champions in the set
int Multithreaded_Solver::Champions_In_Set(){
    return Workspace[0]->Champions_In_Set();
}

// returns the number of traits in the set
int Multithreaded_Solver::Traits_In_Set(){
    return Workspace[0]->Traits_In_Set();
}

// resets the database
void Multithreaded_Solver::Reset(){
    for(int i = 0; i < (int)Workspace.size(); i++){
        Workspace[i]->Reset();
    }
}

// returns the highscore
int Multithreaded_Solver::Highscore(){
    return global_highscore;
}

// returns the average blank_score
float Multithreaded_Solver::Average_Blank_Score(){
    return Workspace[0]->Average_Blank_Score();
}

// returns the uncompressed champions
vector<vector<string>> Multithreaded_Solver::Uncompress_Champions(const vector<vector<int>> &boards){
    return Workspace[0]->Uncompress_Champions(boards);
}

// returns the uncompressed champions
vector<string> Multithreaded_Solver::Uncompress_Champions(const vector<int> &boards){
    return Workspace[0]->Uncompress_Champions(boards);
}

// returns the uncompressed champion
string Multithreaded_Solver::Uncompress_Champions(const int &boards){
    return Workspace[0]->Uncompress_Champions(boards);
}

//returns the uncompressed traits
vector<string> Multithreaded_Solver::Uncompress_Traits(const vector<int> &traits){
    return Workspace[0]->Uncompress_Traits(traits);
}

//returns the uncompressed trait
string Multithreaded_Solver::Uncompress_Traits(const int &trait){
    return Workspace[0]->Uncompress_Traits(trait);
}

// returns the compressed champions
int Multithreaded_Solver::Compress_Champions(const string &champion){
    return Workspace[0]->Compress_Champions(champion);
}

// returns the compressed traits
int Multithreaded_Solver::Compress_Traits(const string &trait){
    return Workspace[0]->Compress_Traits(trait);
}