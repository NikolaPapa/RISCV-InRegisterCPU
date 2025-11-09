#include <stdio.h>

#define SIZE 6

int main() {
    unsigned int matrix_a[SIZE];
    unsigned int gcd_results[3];

    // Manual initialization
    unsigned int *init_ptr = matrix_a;
    *init_ptr++ = 48;
    *init_ptr++ = 18;
    *init_ptr++ = 56;
    *init_ptr++ = 98;
    *init_ptr++ = 270;
    *init_ptr++ = 192;

    // Reset pointer to beginning
    unsigned int *ptr_a = matrix_a;
    unsigned int *ptr_out = gcd_results;

    for (int pair = 0; pair < 3; pair++) {
        unsigned int a = *ptr_a++;
        unsigned int b = *ptr_a++;
        unsigned int result = 0;

        while (a != b) {
            if (a > b) {
                a = a - b;
            } else {
                b = b - a;
            }
        }

        result = a; // GCD found
        *ptr_out++ = result;
    }

    // // Print results
    // ptr_a = matrix_a;
    // ptr_out = gcd_results;

    // for (int pair = 0; pair < 3; pair++) {
    //     unsigned int val1 = *ptr_a++;
    //     unsigned int val2 = *ptr_a++;
    //     unsigned int res = *ptr_out++;

    //     printf("GCD(%d, %d) = %u\n", val1, val2, res);
    // }

    return 0;
}
