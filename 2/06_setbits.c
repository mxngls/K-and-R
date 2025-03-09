#include <stdio.h>

int setbits(int x, int p, int n, int y) {
    int mx = 0; /* 0000 0000 */
    int my = 0; /* 0000 0000 */
    int s;

    p++;

    /* Integers have a maximum value of 16 bits which means they are
     * capable of containing at least the [−32,767, +32,767] range.
     */
    if (p > 16) {
        printf("%s", "Positional parameter greater than 15.\n");
        return -1;
    }

    /* Shifting a negative signed value is undefined see the following
     * compiler option: [-Wshift-negative-value]
     */
    if (n < 0) {
        printf("%s", "The number of bits to be set needs to be positive.\n");
        return -1;
    }

    /* Cannot move further than 15 digits max */
    if (p + n > 15) {
        printf(
            "Cannot move digits further than the 15th digits. Setting p equal "
            "to %d.\n",
            p - n);
        p = p - n;
    }

    /* 1. Get the first n bits of x starting at p */

    mx = ~0 << n; /* 1111 0000 */
    mx = ~mx;     /* 0000 1111 */
    mx = mx << p; /* 0011 1100 */
    mx = ~mx;     /* 1100 0011 */
    mx = x & mx;  /* masked x  */

    /* 2. Get the rightmost n bits of y and move these to position p */

    my = ~(~0 << n);
    my = y & my;
    my = my << p;

    /* 3. OR both intermediate values */

    s = mx | my;

    return s;
}

/*
 * Exercise 2-6. Write a function setbits(x,p,n,y) that returns x with
 * the n bits that begin at position p set to the rightmost n bits of y,
 * leaving the other bits unchanged.
 */
int main() {

    /* --- Bits -------------------------------------- */
    /* 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0 */
    /* 0   0  0  0  0  0  0  0  0  1  1  0  0  1  0  0 */
    /* 0   0  0  0  0  0  0  0  0  0  0  0  1  1  0  1 */

    int x = 100;
    int y = 13;

    x = setbits(x, 2, 4, y);

    if (x != -1) {
        printf("%d\n", x);
        return 0;
    } else
        return -1;
}
