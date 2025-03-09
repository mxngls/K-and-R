#include <stdio.h>
#include <string.h>

#define BUFSIZE 10
#define MAXLEN 10000

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

int getch(void) /* get a (possibly pushed-back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

int ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE) {
        return EOF;
    } else {
        buf[bufp++] = c;
        return c;
    }
}

/* ungets: push back s onto the input */
int ungets(char s[]) {
    int i, len;

    len = strlen(s);

    for (i = len - 1; i >= 0; --i)
        if (ungetch(s[i]) == EOF) {
            printf("ungetch: too many characters (char: '%c')\n", s[len - i]);
            return EOF;
        }

    return 0;
}

/*
 * exercise 4-7. Write a routine ungets(s) that will push back an entire
 * string onto the input. should ungets know about buf and bufp, or
 * should it just use ungetch?
 *
 * Answer:
 *
 * Since the ungetch routine handles pushback of single characters, it
 * makes sense to simply define ungetc in a way that calls ungetch
 * consecutively, until no more characters to push back are left.
 *
 * Explanation:
 *
 * It took me quite while to wrap my head around how to implement not
 * ungets, but the main loop to nicely illustrate what ungets is doing.
 * The gist of it is simply the following: Only call getch in the main
 * loop. To print back the entire string we simply loop over it until we
 * encounter the null byte.
 *
 */
int main() {
    int c, i;
    char s[MAXLEN];

    printf("Enter string to test ungets function:\n");

    i = 0;
    while ((c = getch()) != EOF && i < MAXLEN - 1) {
        if (c == '\n') {
            s[i] = '\0';
            i = 0;

            printf("Pushing back: %s\n", s);

            /* push back entire string s */
            if (ungets(s) == EOF)
                return 1;

            /* print the whole input string again */
            for (c = 0; s[c] != '\0'; c++)
                putchar(s[c]);

        } else {
            s[i++] = c;
        }
    }

    return 0;
}
