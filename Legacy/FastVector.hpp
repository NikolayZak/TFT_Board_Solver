/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2024-03-29
    Last Modified: 2024-03-29

    Description: FastVector.hpp
    Special ADT for condensed fast vector methods
    stores all elements in one integer type
    
    ***** Limitations *****
    the vector can only hold integers 0 to 63
    the vector cannot hold duplicates
    the vector can only pop and push based on the largest integer
    the vector can be seen as a priority queue without duplicates and only ints 0-63
*/
#pragma once
#include <iostream>
#include <stdint.h>
#include <vector>



class FastVector{
    private:
    unsigned long long vector;
    int element_count;


    public:
    void push_back(int x);
    int pop_back();
    int back() const;
    int size() const;
    unsigned long long num() const;

    FastVector();
    FastVector(unsigned long long fast_vec_num);
    ~FastVector();
    // operators
    FastVector& operator=(const std::vector<int>& vector);
    int operator[](int index) const;
};