#include "TopScoringBoards.hpp"

TopScoringBoards::TopScoringBoards(int max_size){
    heap.reserve(max_size);
    this->max_size = max_size;
}

TopScoringBoards::~TopScoringBoards() {
    heap.clear();
}

void TopScoringBoards::heapify_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (compare(heap[index], heap[parent])) {
            std::swap(heap[index], heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void TopScoringBoards::heapify_down(int index) {
    int size = heap.size();
    while (index < size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && compare(heap[left], heap[largest])) {
            largest = left;
        }
        if (right < size && compare(heap[right], heap[largest])) {
            largest = right;
        }
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}
// ensures that the board with the lowest score is at the top of the heap
bool TopScoringBoards::compare(const BoardNode& a, const BoardNode& b) const {
    return a.board_score < b.board_score; // Min-heap based on board score
}

void TopScoringBoards::push(const BoardNode& entry) {
    if (heap.size() < max_size) {
        heap.push_back(entry);
        heapify_up(heap.size() - 1);
    } else if (compare(heap[0], entry)) {
        heap[0] = entry;
        heapify_down(0);
    }
}

// return the lowest top board
const BoardNode& TopScoringBoards::top() const {
    return heap[0];
}

void TopScoringBoards::pop() {
    heap[0] = heap.back();
    heap.pop_back();
    heapify_down(0);
}

void TopScoringBoards::clear() {
    heap.clear();
}

bool TopScoringBoards::empty() const {
    return heap.empty();
}

int TopScoringBoards::size() const {
    return heap.size();
}