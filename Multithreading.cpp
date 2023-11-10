#include "Multithreading.hpp"

// Constructor
Multithreaded_Solver::Multithreaded_Solver(const int &threads, const string &traits_file, const string &champions_file){
    Solver* worker = new Solver(traits_file, champions_file);
    Workspace.push_back(worker);
    for(int i = 1; i < threads; i++){
        Workspace.push_back(new Solver(*worker));
    }

    global_highscore = 0;
}