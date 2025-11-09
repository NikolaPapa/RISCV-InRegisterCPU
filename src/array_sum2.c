#include <stdio.h>

// Function to compute the sum of an array
int array_sum(int *arr, int size) {
    int sum = 0;
    for (int j = 0; j < size; j++) {
        int offset = j;
        offset = offset << 1;  // First shift by 1
        offset = offset << 1;  // Second shift by 1 → total of shift by 2

        // Now use offset in terms of bytes — cast to char* to index via bytes
        int *element_ptr = (int *)((char *)arr + offset);
        sum += *element_ptr;
    }
    return sum;
}


int main() {
    int numbers[10];

    // Manually assigning values to the array, including negatives
    numbers[0] = 5;
    numbers[1] = -3;
    numbers[2] = 12;
    numbers[3] = -7;
    numbers[4] = 4;
    numbers[5] = -1;
    numbers[6] = 9;
    numbers[7] = -6;
    numbers[8] = 2;
    numbers[9] = -8;

    // Call the summation function
    int total = array_sum(numbers, 10);

    // Print the result
    printf("Sum of array elements: %d\n", total);

    return 0;
}