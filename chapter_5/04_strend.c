#include <stdio.h>

#define SIZE 100

int strend(char *s, char *t) {
    char *c = t;
    int f; /* indicate if a match for the first char of t was found */

    f = 0;
    while (*s) {
        if (*s == *t) {
            *c++ = *t++;
            f = 1;
        } else if (f) /* non-matching char found. return */
            return 0;
        s++;
    }

    return 1;
}

/*
 * Exercise 5-4. Write the function strend(s,t), which returns 1 if the
 * string t occurs at the end of the string s, and zero otherwise.
 */
int main() {
    char s[SIZE] = "Hello, World!";
    char t[SIZE] = "World!";

    /* match */
    printf("%d\n", strend(s, t));

    char d[SIZE] = "Hello, World!";
    char c[SIZE] = "Wrld!";

    /* match */
    printf("%d\n", strend(d, c));

    return 0;
}
