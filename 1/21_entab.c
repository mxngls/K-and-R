#include <stdio.h>

#define TABSTOP 4

/*
 * Exercise 1-21. Write a program entab that replaces strings of blanks
 * by the minimum number of tabs and blanks to achieve the same spacing.
 * Use the same tab stops as for detab. When either a tab or a single
 * blank would suffice to reach a tab stop, which should be given
 * preference?
 */
int main() {
    int c, ns, col;

    ns = 0;  /* number of spaces */
    col = 0; /* current column */

    while ((c = getchar()) != EOF) {

        col++;

        switch (c) {
        case '\t':
            col = col + (TABSTOP - col % TABSTOP) - 1;
            ns = 0;
            putchar(c);
            break;
        case '\n':
            col = 0;
            ns = 0;
            putchar(c);
            break;
        case ' ':
            if (col % TABSTOP == 0) {
                putchar('\t');
                ns = 0;
            } else {
                ns++;
            }
            break;
        default:
            while (ns > 0) {
                putchar(' ');
                ns--;
            }
            putchar(c);
        }
    }
    return 0;
}
