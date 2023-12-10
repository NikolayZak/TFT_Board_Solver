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
    float runtime;
    atomic<int> global_highscore;

    // private functions
    long long nCr(int n, int r);
    //void Print_List(const vector<int> &my_list);
    vector<int> Add_To_List(vector<int> my_list, const long long &value);
    void Configure_Subsets();
    void Combine_Boards(vector<vector<int>> &v1, vector<vector<int>> &v2);



    public:
    // constructors and deconstructors
    Multithreaded_Solver(const int &threads, const string &traits_file, const string &champions_file);
    Multithreaded_Solver(const int &threads, const Solver &main);
    ~Multithreaded_Solver();

    // main functions
    float Runtime();
    void Solve(const int &size);
    void Update(const Solver &main);
    vector<vector<string>> Optimal_Boards();
    vector<vector<int>> Compressed_Optimal_Boards();

    // roping board functions
    void Max_Increase(const int &value);
    int Max_Increase();
    void Blank_Score(const float &value);
    float Blank_Score();
    void Cost_Restriction(const int &value); // setter
    int Cost_Restriction(); // getter
    void Add_Champion(const int &int_champion);
    void Add_Trait(const int &int_trait);
    vector<string> Get_All_Champions();
    vector<string> Get_All_Traits();
    vector<int> Champions_Added();
    vector<int> Traits_Added();
    int Champions_In_Set();
    int Traits_In_Set();
    void Reset();
    int Highscore();
    float Average_Blank_Score();

    // compressions and extraction
    vector<vector<string>> Uncompress_Champions(const vector<vector<int>> &boards);
    vector<string> Uncompress_Champions(const vector<int> &board);
    string Uncompress_Champions(const int &champion);

    vector<string> Uncompress_Traits(const vector<int> &traits);
    string Uncompress_Traits(const int &trait);

    int Compress_Traits(const string &trait);
    int Compress_Champions(const string &champion);
};





#endif