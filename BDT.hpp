/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2024-03-29
    Last Modified: 2024-03-29

    Description: BDT.hpp
    Special ADT for condensed fast vector like methods
    stored everything in an integer type
    ADT for creating, manipulating and calculating scores of a TFT board
*/
#ifndef BDT
#define BDT
#include <iostream>
#include <stdint.h>

// Limitations
#define INT_TYPE uint64_t // 64 elements in vector max, no duplicates

class Board_Vector{
    private:
    INT_TYPE Vector;
    int size;


    public:
    void push_back(int x);
    int pop_back();
    int back();
    int size(){ return size; }
    Board_Vector();
    ~Board_Vector();
};

#endif