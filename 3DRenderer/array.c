#include <stdio.h>
#include <stdlib.h>
#include "array.h"

// Retrieves a pointer to the raw allocated memory.
#define ARRAY_RAW_DATA(array) ((int*)(array) - 2)

// Gets the total allocated capacity of the array.
#define ARRAY_CAPACITY(array) (ARRAY_RAW_DATA(array)[0])

// Gets the number of elements currently stored in the array.
#define ARRAY_OCCUPIED(array) (ARRAY_RAW_DATA(array)[1])

void* array_hold(void* array, int count, int item_size) {
    if (array == NULL) {
        // Allocate memory for metadata (2 integers) and the requested elements.
        int raw_size = (sizeof(int) * 2) + (item_size * count);
        int* base = (int*)malloc(raw_size);
        base[0] = count;  // Set capacity.
        base[1] = count;  // Set occupied count.
        return base + 2;  // Return pointer to data.
    }
    else if (ARRAY_OCCUPIED(array) + count <= ARRAY_CAPACITY(array)) {
        // If enough space is available, just increase the occupied count.
        ARRAY_OCCUPIED(array) += count;
        return array;
    }
    else {
        // Calculate new required size.
        int needed_size = ARRAY_OCCUPIED(array) + count;
        int float_curr = ARRAY_CAPACITY(array) * 2;
        int capacity = needed_size > float_curr ? needed_size : float_curr;
        int occupied = needed_size;
        int raw_size = sizeof(int) * 2 + item_size * capacity;

        // Reallocate memory with new size.
        int* base = (int*)realloc(ARRAY_RAW_DATA(array), raw_size);
        base[0] = capacity;  // Update capacity.
        base[1] = occupied;  // Update occupied count.
        return base + 2;  // Return pointer to data.
    }
}

int array_length(void* array) {
    // Return the number of occupied elements or 0 if the array is NULL.
    return (array != NULL) ? ARRAY_OCCUPIED(array) : 0;
}

void array_free(void* array) {
    if (array != NULL) {
        // Free the allocated memory, including metadata.
        free(ARRAY_RAW_DATA(array));
    }
}
