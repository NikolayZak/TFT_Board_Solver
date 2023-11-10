/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2023-11-10
    Last Modified: 2023-11-10

    Description: Multithreading.hpp
    Uses Threads we split the work among equal subsets to compute the optimal boards.
*/
#ifndef MULTITHREADING
#define MULTITHREADING

// Dependencies
#include "Solver.hpp"
#include <thread>
#include <cmath>

// Public Function
class Multithreaded_Solver{
    private:
    vector<Solver*> Workspace;
    vector<vector<int>> compressed_global_optimal;
    int global_highscore;

    public:
    Multithreaded_Solver(const int &threads, const string &traits_file, const string &champions_file);
    ~Multithreaded_Solver();

    
};








#endif