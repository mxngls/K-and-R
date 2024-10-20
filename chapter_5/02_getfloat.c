#include <ctype.h>
#include <stdio.h>

int getch(void);
void ungetch(int);

#define LEN 10
#define BUFSIZE 10

char buf[BUFSIZE];
int bufp = 0;

int getch(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }

void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

/* getfloat:  get next integer from input into *pn */
double getfloat(double *pn) {
    char c;
    int sign, d;

    while (isspace(c = getch()))
        ; /* skip whit space */

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c); /* it's not a number */
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-') {
        if (!isdigit(c = getch())) {
            if (c != EOF)
                ungetch(c);
            ungetch(sign == 1 ? '+' : '-');
            return 0;
        }
    }

    d = 0;
    for (*pn = 0; isdigit(c) || c == '.'; c = getch()) {
        if (c == '.') {
            d++;
            continue;
        }

        if (!d)
            *pn = 10 * *pn + (c - '0');
        else {
            int i;
            float o;

            i = 0;
            o = 1;
            while (i++ < d) {
                o *= 0.1;
            }

            *pn = *pn + (c - '0') * o;

            d++;
        }
    }

    *pn *= sign;

    if (c != EOF)
        ungetch(c);

    return c;
}

/*
 * Exercise 5-1. As written, getfloat treats a + or - not followed by a
 * digit as a valid representation of zero. Fix it to push such a
 * character back on the input.
 *
 * see:
 *
 * https://stackoverflow.com/questions/25611409/the-wording-of-exercise-5-1-in-kr-c
 *
 * for a helpful explanation and behavior that a possible caller of
 * getfloat might expect.
 */
int main() {
    int n, s;
    double getfloat(double *);

    double array[LEN] = {0};

    for (n = 0; n < LEN && getfloat(&array[n]) != EOF; n++) {
        printf("inserting %f (pos %d)\n", array[n], n + 1);
    }

    for (s = 0; s < n; s++)
        printf("%f, ", array[s]);

    return 0;
}
