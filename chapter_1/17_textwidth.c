#include <stdio.h>

#define MAX_LEN 72
#define MIN_LEN 34

/*
 * Exercise 1-17. Write a program to print all input lines that are
 * longer than 80 characters.
 */
int main(void) {
    int line_len = 0;
    int c;
    char line[MAX_LEN];

    while ((c = getchar()) != EOF) {
        if (line_len < MAX_LEN) {
            line[line_len] = c;
        }
        ++line_len;

        if (c == '\n') {
            if (line_len >= MIN_LEN) {
                line[line_len] = '\0';
                printf("%s", line);
            };

            line[0] = '\0';
            line_len = 0;
        }
    }

    return 0;
}
