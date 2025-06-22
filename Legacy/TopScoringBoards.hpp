/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-06-7
    Last Modified: 2025-06-7

    Description: TopScoringBoards.hpp
    Heap Based ADT for keeping and sorting the top n boards
*/

#pragma once
#include <vector>
#include "Common.hpp"

using std::vector;

class TopScoringBoards {
private:
    vector<BoardNode> heap;
    int max_size;

    void heapify_up(int index);
    void heapify_down(int index);
    bool compare(const BoardNode& a, const BoardNode& b) const;

public:
    TopScoringBoards(int max_size); // constructor
    ~TopScoringBoards(); // destructor
    void push(const BoardNode& entry);
    void pop();
    void clear();
    const BoardNode& top() const;
    bool empty() const;
    int size() const;
};