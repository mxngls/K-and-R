#include <stdio.h>

#define BUFSIZE 100

/*
 * Answer:
 *
 * As the buffer, buf, is declared with type char it might not handle
 * negative values like EOF (-1) correctly, if the system specific
 * definition of char does not include negative values. On most modern
 * systems char is defined as a signed integer. This was not the case
 * when K&R was written this it is safer to define buf as an integer
 * array instead of an array whose elements are of type char.
 */

int buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;	  /* next free postion in buf */

int getch(void) /* get a posibly pushed back) character */
{
    return (bufp > 0 ? buf[--bufp] : getchar());
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

/*
 * Exercise 4-9. Our getch and ungetch do not handle a pushed-back EOF
 * correctly. Decide what their properties ought to be if an EOF is
 * pushed back, then implement your design.
 */
int main(void)
{
    int c;

    /* Prematurely send EOF. */
    ungetch(EOF);

    while ((c = getch()) != EOF)
        putchar(c);

    return 0;
}
