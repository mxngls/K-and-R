#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]) {
    static int i;

    if (n / 10) {
        itoa(n / 10, s);
    } else {
        i = 0;

        if (n < 0) { /* record sign */
            s[i++] = '-';
            n = -n;
        }
    }

    s[i++] = abs(n) % 10 + '0';
    s[i] = '\0';
}

/*
 * Exercise 4-12. Adapt the ideas of printd to write a recursive version
 * of itoa; that is, convert an integer into a string by calling a
 * recursive routine.
 */
int main() {
    int i;
    char s[MAXLEN];

    i = -123;

    itoa(i, s);
    printf("%s\n", s);

    i = 123;

    itoa(i, s);
    printf("%s\n", s);

    return 0;
}
