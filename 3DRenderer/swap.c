#include "swap.h"

// Swaps the values of two integers.
void int_swap(int* a, int* b) {
    int tmp = *a;  
    *a = *b;       
    *b = tmp;      
}

// Swaps the values of two floating-point numbers.
void float_swap(float* a, float* b) {
    float tmp = *a;  
    *a = *b;         
    *b = tmp;
}
