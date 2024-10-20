#include <limits.h>
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
void itoa(int n, char s[]) {
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

    s[i] = '\0';

    reverse(s);
}

/*
 * Exercise 3-4. In a two's complement number representation, our
 * version of itoa does not handle the largest negative number, that
 * is, the value of n equal to -(2^(wordsize-1)). Explain why not.
 * Modify it to print that value correctly, regardless of the machine on
 * which it runs.
 */
int main() {
    int i = INT_MIN;
    char o[MAX_LEN];

    itoa(i, o);
    printf("%d interpreted as a string evaluates to: %s\n", i, o);

    return 0;
}

/*
 * Explanation:
 *
 * In a two's complement number representation, negative numbers are
 * represented by inverting all bits and adding one additional bit, known
 * as the sign bit. The bug in the original implementation of `itoa` lies
 * in the first if-statement, where the input parameter `n` is reversed. In
 * two's complement, the absolute value of the largest negative number is
 * exactly one greater than its positive counterpart. Attempting to
 * calculate the two's complement of this number leads us back to the
 * original negative number.
 *
 * This can be easily illustrated by assuming signed integers are
 * represented using 8 bits. The largest negative number that can be
 * represented this way is -2 to the power of 7. In binary, it looks like
 * this: `10000000`. Reversing all the bits: `01111111`, and then adding
 * one to get its inverted value leads to: `10000000`. Thus, we end up with
 * our initial value again. Observing these results, it's apparent that in
 * two's complement, the largest negative number is in absolute terms
 * greater than its counterpart by one. In our 8-bit example, this means
 * that the largest negative number is -2 to the power of 7, which equals
 * -128. The largest positive number is therefore 2 to the power of 7 minus
 * 1, which equals 127.
 */
