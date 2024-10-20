#include <stdio.h>

/* "A rotate does, well, exactly that: it's the same as a logical shift,
 * except when you shift past the end of the number, the digits
 * "wrap around" to the other side. https://stackoverflow.com/a/3928821/13490131
 */
unsigned int rightrot(unsigned int x, unsigned int n) {
    unsigned int ls, rs, rot;

    /* needed so you don't right shift more than int width */
    n %= 32;

    /* right shifted */
    rs = x >> n;

    /* left shifted */
    ls = x << (32 - n);

    /* rotated */
    rot = ls | rs;

    return rot;
}

/*
 * Exercise 2-8. Write a function rightrot(x,n) that returns the value
 * of the integer x rotated to the right by n positions.
 */
int main() {
    unsigned int x = 8; /* 0000 1000 -> 1000 [...] 0000 */
    unsigned int n = 4;

    printf("%u\n", rightrot(x, n));

    return 0;
}
