#include <stdio.h>

 /* We allow for no more than ten different percentiles */
#define PERCENTILES 10

/* 
 * Exercise 1-13. Write a program to print a histogram of the lengths of
 * words in its input. It is easy to draw the histogram with the bars
 * horizontal; a vertical orientation is more challenging. 
 */
int main() {

    int c = 0;
    int word_c[PERCENTILES];
    int cur_word_len = 0;

    for (int i = 0; i < PERCENTILES; i++) {
        word_c[i] = 0;
    }

    while ((c = getchar()) != EOF) {
        switch (c) {
        case '\n':
        case '\t':
        case '\r':
        case ' ':
            if (cur_word_len >= 9) {
                ++word_c[9];
            } else {
                ++word_c[cur_word_len];
            }

            cur_word_len = 0;

            break;
        default:
            cur_word_len++;
            break;
        }
    }

    for (int l = 0; l < PERCENTILES; l++) {
        if (l < 9)
            printf("%3d: %0*d\n", l + 1, word_c[l], 0);
        else
            printf(">%2d: %0*d\n", l + 1, word_c[l], 0);
    }

    return 0;
}
