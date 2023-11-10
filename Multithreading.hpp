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
void Multithreaded_Compute(const int &size, vector<Solver*> &solvers);






#endif