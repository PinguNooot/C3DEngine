#ifndef ARRAY_H
#define ARRAY_H

// Macro to append a value to a dynamic array.
#define array_push(array, value)                                              \
    do {                                                                      \
        (array) = array_hold((array), 1, sizeof(*(array)));                   \
        (array)[array_length(array) - 1] = (value);                           \
    } while (0);


// Ensures the dynamic array has enough space for a given number of elements.
void* array_hold(void* array, int count, int item_size);

// Returns the number of elements currently stored in the array.
int array_length(void* array);

// Frees the allocated memory for the dynamic array.
void array_free(void* array);

#endif // ARRAY_H
