#include "FastVector.hpp"
/*          Time Complexity
    Similar to <vector> all Functions are in O(1)
    The speed ups come from it all being compressed into one integer and bitwise operations being faster
    roughly 4-6 times faster then std vector
*/

// constructor
FastVector::FastVector(){
    vector = 0;
    element_count = 0;
}

FastVector::FastVector(unsigned long long fast_vec_num){
    vector = fast_vec_num;
    element_count = __builtin_popcountll(fast_vec_num);
}

// nothing to deconstruct
FastVector::~FastVector(){

}

// pushes back an integer
void FastVector::push_back(int x){
    vector |= 0x8000000000000000 >> x;
    element_count++;
}

// pops back the highest integer NOT NECESSARILY THE LAST ELEMENT
int FastVector::pop_back(){
    int position = __builtin_ctzll(vector);
    vector ^= (1ULL << position);
    element_count--;
    return 63 - position;
}

// peeks at the highest integer NOT NECESSARILY THE LAST ELEMENT
int FastVector::back() const{
    return 63 - __builtin_ctzll(vector);
}

// peeks at the highest integer NOT NECESSARILY THE LAST ELEMENT
int FastVector::size() const{
    return element_count;
}

// returns the FastVector as a number
unsigned long long FastVector::num() const{
    return vector;
}

// custom assignment
FastVector& FastVector::operator=(const std::vector<int>& v1){
    vector = 0;
    for(int i = 0; i < (int)v1.size(); i++){
        push_back(v1[i]);
    }
    return *this;
}


// custom access
int FastVector::operator[](int index) const{
    int count = 0;
    for (int i = 63; i >= 0; --i) {
        if (vector & (1ULL << i)) {
            if (++count == index + 1) {
                return 63 - i;
            }
        }
    }
    return -1; // error
}