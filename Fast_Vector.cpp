#include "Fast_Vector.hpp"
/*          Time Complexity
    Similar to <vector> all Functions are in O(1)
    The speed ups come from it all being compressed into one integer and bitwise operations being faster
    roughly 4-6 times faster then std vector
*/

// constructor
Fast_Vector::Fast_Vector(){
    vector = 0;
}

Fast_Vector::Fast_Vector(unsigned long long fast_vec_num){
    vector = fast_vec_num;
}

// nothing to deconstruct
Fast_Vector::~Fast_Vector(){

}

// pushes back an integer
void Fast_Vector::push_back(int x){
    vector |= 0x8000000000000000 >> x;
}

// pops back the highest integer NOT NECESSARILY THE LAST ELEMENT
int Fast_Vector::pop_back(){
    int position = __builtin_ctzll(vector);
    vector ^= (1ULL << position);
    return 63 - position;
}

// peeks at the highest integer NOT NECESSARILY THE LAST ELEMENT
int Fast_Vector::back(){
    return 63 - __builtin_ctzll(vector);
}

// peeks at the highest integer NOT NECESSARILY THE LAST ELEMENT
int Fast_Vector::size(){
    return __builtin_popcountll(vector);
}

// returns the Fast_Vector as a number
unsigned long long Fast_Vector::num(){
    return vector;
}

// custom assignment
Fast_Vector& Fast_Vector::operator=(const std::vector<int>& v1){
    for(int i = 0; i < v1.size(); i++){
        push_back(v1[i]);
    }
}

// custom access
int Fast_Vector::operator[](size_t index){
    int count = 0;
    for (int i = 63; i >= 0; --i) {
        if (vector & (1ULL << i)) {
            if (++count == index + 1) {
                return 63 - i;
            }
        }
    }
}