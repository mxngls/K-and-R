#include <stdio.h>

#define TABSTOP 8

int main() {
  int c;  /* current char */
  int s;  /* number of spaces */

  s = 0;

  while ((c = getchar()) != EOF) {
    if (c == ' ') {
      if (++s % TABSTOP == 0) {
        putchar('\t');
        s = 0;
      }
    } else {
      if (s % TABSTOP != 0) {
        while (s-- > 0)
          putchar(' ');
      }

      putchar(c);

      s = 0;
    }
  }

  return 0;
}
