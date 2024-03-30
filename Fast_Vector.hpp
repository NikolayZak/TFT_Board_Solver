/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2024-03-29
    Last Modified: 2024-03-29

    Description: Fast_Vector.hpp
    Special ADT for condensed fast vector methods
    stores all elements in one integer type
    
    ***** Limitations *****
    the vector can only hold integers 0 to 63
    the vector cannot hold duplicates
    the vector can only pop and push based on the largest integer
    the vector can be seen as a priority queue without duplicates and only ints 0-63
*/
#ifndef FAST_VECTOR
#define FAST_VECTOR
#include <iostream>
#include <stdint.h>
#include <vector>



class Fast_Vector{
    private:
    unsigned long long vector;


    public:
    void push_back(int x);
    int pop_back();
    int back();
    int size();
    unsigned long long num();

    Fast_Vector();
    Fast_Vector(unsigned long long fast_vec_num);
    ~Fast_Vector();
    // operators
    Fast_Vector& operator=(const std::vector<int>& vector);
    int operator[](size_t index);
};

#endif