#include "Fast_Vector.hpp"
/*          Time Complexity
    Similar to <vector> all Functions are in O(1)
    The speed ups come from it all being compressed into one integer and bitwise operations being faster
*/

// constructor
Fast_Vector::Fast_Vector(){
    vector = 0;
    element_count = 0;
}

// nothing to deconstruct
Fast_Vector::~Fast_Vector(){

}

// pushes back an integer
void Fast_Vector::push_back(int x){
    vector |= 0x8000000000000000 >> x;
    element_count++;
}

// pops back the highest integer NOT NECESSARILY THE LAST ELEMENT
int Fast_Vector::pop_back(){
    int position = __builtin_ctzll(vector);
    vector ^= (1ULL << position);
    element_count--;
    return 63 - position;
}

// peeks at the highest integer NOT NECESSARILY THE LAST ELEMENT
int Fast_Vector::back(){
    return 63 - __builtin_ctzll(vector);
}