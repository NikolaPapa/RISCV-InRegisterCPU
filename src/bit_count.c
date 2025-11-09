#include <stdio.h>

int main() {
    unsigned int test_words[6];
    
    test_words[0] = 0xFFFFFFFF;
    test_words[1] = 0x00000000;
    test_words[2] = 0x12345678;
    test_words[3] = 0xAAAAAAAA;
    test_words[4] = 0x55555555;
    test_words[5] = 0xF0F0F0F0;
    

    unsigned int bit_counts[6] = {0};

    unsigned int *word_ptr = test_words;
    unsigned int *count_ptr = bit_counts;

    for (int i = 0; i < 6; i++) {
        unsigned int word = *word_ptr;
        unsigned int count = 0;

        for (int j = 0; j < 32; j++) {
            count += word & 1;   // Check least significant bit
            word = word >> 1;    // Shift right by 1
        }

        *count_ptr = count;

        word_ptr++;   // Move to next word (no shifting needed)
        count_ptr++;  // Move to next count
    }

    // // Print results
    // word_ptr = test_words; // Reset pointer
    // count_ptr = bit_counts;

    // for (int i = 0; i < 6; i++) {
    //     printf("Word %d (0x%08X) has %u set bits.\n", i, *word_ptr, *count_ptr);
    //     word_ptr++;
    //     count_ptr++;
    // }

    return 0;
}
