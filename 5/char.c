#include <stdio.h>

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */
int col = 1;       /* current column position */
int line = 1;      /* current line */

/* get a (possibly pushed-back) character */
int getch(void) {
    int c;
    if (bufp > 0)
        return buf[--bufp];
    else {
        col++;
        if ((c = getchar()) == '\n') {
            col = 0;
            line++;
        }
        return c;
    }
}

/* push character back on input */
void ungetch(int c) {
    col--;
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = (char)c;
}
