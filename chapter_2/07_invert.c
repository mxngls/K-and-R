#include <stdio.h>

int invert(int x, int p, int n) {
  int s;
  int mx = 0;

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
    printf("Cannot move digits further than the 15th digit. Setting p equal "
           "to %d.\n",
           p - n);
    p = p - n;
  }

  /* 1. Isolate n bits starting at p */

  mx = ~0 << n; /* 1111 0000 */
  mx = ~mx;     /* 0000 1111 */
  mx = mx << p; /* 0011 1100 */
  s = x ^ mx;   /* masked x  */

  return s;
}

/* Exercise 2-7. Write a function invert(x,p,n) that returns x with the
 * n bits that begin at position p inverted (i.e., 1 changed into 0 and
 * vice versa), leaving the others unchanged.
 */
int main() {
  int x, p, n;

  x = 8; /* 0000 1000 -> 0111 0000 (112) */
  p = 2;
  n = 4;

  printf("%d\n", invert(x, p, n));

  return 0;
}
