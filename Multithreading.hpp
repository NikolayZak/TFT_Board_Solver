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

using std::thread;

// Public Function
class Multithreaded_Solver{
    private:
    // variables
    int board_size;
    int champions_in_set;


    vector<Solver*> Workspace;
    vector<vector<int>> compressed_global_optimal;
    vector<vector<int>> start;
    vector<vector<int>> end;
    int num_threads;
    int global_highscore;

    // private functions
    long long nCr(int n, int r);
    void Print_List(const vector<int> &my_list);
    void Add_To_List(vector<int> &my_list, const long long &value);
    void Configure_Subsets();

    

    public:
    Multithreaded_Solver(const int &threads, const string &traits_file, const string &champions_file);
    Multithreaded_Solver(const int &threads, const Solver &main);
    ~Multithreaded_Solver();
    void Solve(const int &size);

    vector<vector<string>> Optimal_Boards();
    vector<vector<int>> Compressed_Optimal_Boards();
};








#endif