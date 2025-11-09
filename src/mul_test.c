#include <stdio.h>


int multiply(int a, int b) {
    int result = 0;

    // Handle negative values
    int negative = 0;
    if (a < 0) {
        a = -a;
        negative = !negative;
    } 
    if (b < 0) {
        b = -b;
        negative = !negative;
    }

    while (b > 0) {
        // If the lowest bit of b is 1, add a to result
        if (b & 1) {
            result += a;
        }

        a = a << 1;    // Multiply a by 2
        b = b >> 1;    // Divide b by 2
    }

    // If b was negative, flip the result sign
    return negative ? -result : result;
}

int main() {
    int x = 6, y = -3;
    int result = multiply(x, y);
    // printf("%d * %d = %d\n", x, y, result);

    x=12;
    y=4;
    int result2 = multiply(x,y);
    // printf("%d * %d = %d\n", x, y, result2);

    x=-3;
    y=-5;
    int result3 = multiply(x,y);
    // printf("%d * %d = %d\n", x, y, result3);

    x=0;
    y=-5;
    int result4 = multiply(x,y);
    // printf("%d * %d = %d\n", x, y, result4);

    x=3;
    y=0;
    int result5 = multiply(x,y);
    // printf("%d * %d = %d\n", x, y, result5);

    return 0;
}
