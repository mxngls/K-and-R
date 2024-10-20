#include <stdio.h>

#define MAX_LEN 72

/*
 * Exercise 1-19. Write a function reverse(s) that reverses the
 * character string s. Use it to write a program that reverses its input
 * a line at a time.
 */
int reverse(char line[], char reversed_line[]) {
    int i, j, l;

    j = 0;
    i = 0;

    while (line[i] != '\0' && i < MAX_LEN) {
        ++i;
    }

    l = i;

    while (j < l) {
        reversed_line[j++] = line[--i];
    }

    reversed_line[j] = '\0';

    return j;
}

int main() {
    int c, i;
    char line[MAX_LEN], reversed_line[MAX_LEN];

    i = 0;

    while ((c = getchar()) != EOF) {
        if (c == '\n' || i == MAX_LEN - 1) {
            if (i > 0) {
                line[i] = '\0';

                reverse(line, reversed_line);

                printf("%s\n", reversed_line);
            }

            i = 0;
        } else {
            line[i] = c;
            i++;
        }
    }

    return 0;
}
