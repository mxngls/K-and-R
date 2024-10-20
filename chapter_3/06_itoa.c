#include <stdio.h>

#define MAX_LEN 4096

int reverse(char s[]) {
    int i, j, tmp;
    char c;

    i = j = 0; /* Get length of input string */
    while ((c = s[i++]) != '\0')
        j++;

    i = 0;
    j--;

    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;

        i++;
        j--;
    }

    return 0;
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[], int w) {
    int i, j, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */

    i = 0;

    do { /* generate digits in reverse order */
        if ((j = (n % 10)) < 0)
            j = -j; /* Set j to it's absolute value */

        s[i++] = j + '0'; /* get next digit */
    } while (n /= 10);    /* delete it */

    if (sign < 0)
        s[i++] = '-';

    while (i < w - 1) {
        /* We use the tilde character instead space for better visibility */
        s[i++] = '~';
    }

    s[i] = '\0';

    reverse(s);
}
/*
 * Exercise 3-6. Write a version of itoa that accepts three arguments
 * instead of two. The third argument is a minimum field width; the
 * converted number must be padded with blanks on the left if necessary
 * to make it wide enough.
 */
int main() {
    int i = 3912;
    int w = 10;
    char o[MAX_LEN];

    itoa(i, o, w);
    printf("%d interpreted as a string evaluates to: %s\n", i, o);

    return 0;
}
