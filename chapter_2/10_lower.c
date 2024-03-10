#include <stdio.h>

/* lower:  convert c to lower case; ASCII only */
int lower(int c) {
  return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c;
}

/*
 * Exercise 2-10. Rewrite the function lower, which converts upper case 
 * letters to lower case, with a conditional expression instead of 
 * if-else.
 */
int main() { 
  char c;
  char lc;

  c = 'A';

  lc = lower(c);

  printf("Uppercase \"%c\" transformed to lowercase \"%c\".\n", c, lc);

  return 0;
}
