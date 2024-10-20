#include <stdio.h>

/*
 * Count the number of 1-bits in a given integer.
 *
 * For the why: As x &= (x-1) removes the rightmost 1-bit we simply need
 * to apply it until no 1-bits are leaved to remove. Adding a counter
 * then gives us the exact number of 1-bits. The implementation is
 * simply faster for we do not need to check every single digit, but
 * only repeat the operation exactly as many times as there is still a
 * 1-bit left to be removed.
 */
int bitcount(int x) {
    int c = 0;

    while ((x &= (x - 1)) > 0) {
        c++;
    }

    /*
     * Add one for the last 1-bit will not be properly counted with the
     * above expression
     */
    ++c;

    return c;
}

/*
 * Exercise 2-9. In a two's complement number system, x &= (x-1) deletes
 * the rightmost 1-bit in x. Explain why. Use this observation to write
 * a faster version of bitcount.
 */
int main() {

    /*
     * x &= (x-1);
     *
     * First we clear the question why the above operation results in the
     * removal of the rightmost 1-bit of a given integer:
     *
     * A given integer can either be even or uneven.
     *
     * In the latter case the last 1-bit will always be one. Substracting
     * one from an uneven integer will always result in an even one. Which
     * essentially will lead to the substraction of said 1-bit in the
     * first position. The rightmost 1-bit that was present before will
     * instead be interchanged with a zero.
     *
     * In the case that our integer is a even number instead the removal
     * of the rightmost 1-bit gets slightly complex. If we remove the
     * rightmost 1-bit from an even integer we need to first switch this
     * bit to a zero and all following bits that were previously zero's
     * need to be transformed into ones.
     */

    int x;
    int c;

    x = 15; /* 1111 */
    c = bitcount(x);

    printf("1-bits in %d equal to %d.\n", x, c);

    return 0;
}
