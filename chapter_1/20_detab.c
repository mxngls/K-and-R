#include <stdio.h>

#define TABSTOP 4

/*
 * Exercise 1-20. Write a program detab that replaces tabs in the input
 * with the proper number of blanks to space to the next tab stop.
 * Assume a fixed set of tab stops, say every n columns. Should n be a
 * variable or a symbolic parameter?
 */
int main() {
    int c, ns, col; /* current char; number of spaces; current column */

    while ((c = getchar()) != EOF) {
        if (c == '\t') {
            ns = TABSTOP - col % TABSTOP;
            while (ns-- > 0) {
                putchar(' ');
                col++;
            }
        } else {
            c == '\n' ? col = 0 : col++;
            putchar(c);
        }
    }

    return 0;
}
