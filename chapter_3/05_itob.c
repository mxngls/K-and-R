#include <stdio.h>

#define MAX_LEN 4096


/* itob:  convert n to hexadecimal characters in s */
int itob(int n, char s[], int b) {
  int i, r, q;

  i = r = q = 0;

  do {
    r = n % b;
    n = n / b;

    s[i++] = r;
  } while (n > 0);

  /* Hexadecimals simply look nicer that way */
  printf("0x");

  do {
    if (s[--i] < 10) printf("%c", 48 + s[i]);
    else printf("%c", 65 - 10 + s[i]);
  } while (i > 0);

  printf("\n");

  return 0;
}

/*
 * Exercise 3-5. Write the function itob(n,s,b) that converts the
 * integer n into a base b character representation in the string s. In
 * particular, itob(n,s,16) formats s as a hexadecimal integer in s.
 */
int main() {
  int n, b;
  char s[MAX_LEN];

  b = 16;
  n = 978210323; /* -> 0x3A4E4E13 */

  if (itob(n, s, b) != 0)
    return -1;

  return 0;
}
