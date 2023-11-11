#include "Multithreading.hpp"

// Constructor
Multithreaded_Solver::Multithreaded_Solver(const int &threads, const string &traits_file, const string &champions_file){
    Solver* worker = new Solver(traits_file, champions_file);
    num_threads = threads;
    Workspace.push_back(worker);
    for(int i = 1; i < threads; i++){
        Workspace.push_back(new Solver(*worker));
    }

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

// helper function for debugging
void Multithreaded_Solver::Print_List(const vector<int> &my_list){
    for(int i = 0; i < (int)my_list.size(); i++){
        cout << my_list[i] << " ";
    }
    cout << endl;
}


// Function that given a sorted list will give x lists later Ex: z = 9; {1,2,3,4} adding 7 = {1,2,4,6}
void Multithreaded_Solver::Add_To_List(vector<int> &my_list, const long long &value){
    long long current = 0;
    int z = champions_in_set - my_list[0];
    int n = board_size;
    int index = 0;
    long long spacer;
    while(current < value){
        spacer = nCr(z-1, n-1);
        if(current + spacer <= value){
            current += spacer;
            z--;
            // increment the list
            my_list[index] += 1;
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
        start.push_back(a_board);
        Add_To_List(a_board, partition);
        end.push_back(a_board);
        Add_To_List(a_board, 1);
    }
}

// Solves the subsets and joins them together in the global optimal
void Multithreaded_Solver::Solve(const int &size){
    board_size = size;
    global_highscore = 0;
    compressed_global_optimal.clear();
    Configure_Subsets();
    // call all the threads on their partitions
    for(auto list : start){
        Print_List(list);
    }

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
            compressed_global_optimal.insert(compressed_global_optimal.end(), Workspace[i]->Compressed_Optimal_Boards().begin(), Workspace[i]->Compressed_Optimal_Boards().end());

        }else if(Workspace[i]->Highscore() == global_highscore){
            compressed_global_optimal.insert(compressed_global_optimal.end(), Workspace[i]->Compressed_Optimal_Boards().begin(), Workspace[i]->Compressed_Optimal_Boards().end());
        }
        return;
    }
}


// returns the compressed optimal boards
vector<vector<int>> Multithreaded_Solver::Compressed_Optimal_Boards(){
    return compressed_global_optimal;
}

// returns the optimal boards
vector<vector<string>> Multithreaded_Solver::Optimal_Boards(){
    return Workspace[0]->Uncompress_Champions(compressed_global_optimal);
}


/*
Example of multithreading

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

// The function you want to call
void myFunction(int condition) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread did task " << condition << std::endl;
    // Your function's work goes here
}

int main() {
    vector<int> initial_conditions = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int numTasks = 100;
    vector<thread> threads;

    for (int i = 0; i < numTasks; i++) {
        threads.emplace_back(myFunction, initial_conditions[i%10]);
    }

    // Join all threads to wait for them to finish
    for (std::thread &t : threads) {
        t.join();
        cout << "#";
    }

    return 0;
}
*/