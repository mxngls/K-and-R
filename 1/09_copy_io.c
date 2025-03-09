#include <stdio.h>

/*
 * Exercise 1-9. Write a program to copy its input to its output,
 * replacing each string of one or more blanks by a single blank.
 */
int main() {

    int last, c;

    last = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            if (last != ' ') {
                putchar(c);
            };
            last = ' ';
        } else {
            last = 0;
            putchar(c);
        };
    };

    return 0;
}
