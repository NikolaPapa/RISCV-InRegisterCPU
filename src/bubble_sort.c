#include <stdio.h>

// // Function to perform Bubble Sort without shifting or multiplication
// void bubbleSort(int arr[], int n) {
//     int i, j, temp;
//     for (i = 0; i < n - 1; i++) {
//         for (j = 0; j < n - i - 1; j++) {
//             // Manual array indexing using addition instead of multiplication
//             int *a = arr; // Pointer to array base
//             int *b = arr; // Another pointer

//             // Calculate addresses without shifts
//             for (int k = 0; k < j; k++) a++;
//             for (int k = 0; k < j + 1; k++) b++;

//             // Comparison and swap
//             if (*a > *b) {
//                 temp = *a;
//                 *a = *b;
//                 *b = temp;
//             }
//         }
//     }
// }

//bb_sort2.dmp
void bubbleSort(int arr[], int n) {
    int i, j, temp;

    char *base = (char *)arr; // base address as byte pointer

    for (i = 0; i < n - 1; i++) {
        int byteOffset = 0;

        for (j = 0; j < n - i - 1; j++) {
            // Access arr[j] and arr[j+1] using byte offsets
            int *a = (int *)(base + byteOffset);
            int *b = (int *)(base + byteOffset + 4);

            if (*a > *b) {
                temp = *a;
                *a = *b;
                *b = temp;
            }

            // Move to next element: +4 bytes
            byteOffset += 4;
        }
    }
}

// //Function to print array
// void printArray(int arr[], int n) {
//     for (int i = 0; i < n; i++) {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");
// }

// Main function to test Bubble Sort
int main() {
    int arr[6];

    //initialization that don't needs a bootstrapinp
    arr[0] = 5;
    arr[1] = 2;
    arr[2] = 9;
    arr[3] = 1;
    arr[4] = 5;
    arr[5] = 6;

    int n = 6;

    // printf("Original array: ");
    // printArray(arr, n);

    bubbleSort(arr, n);

    // printf("Sorted array: ");
    // printArray(arr, n);

    return 0;
}
