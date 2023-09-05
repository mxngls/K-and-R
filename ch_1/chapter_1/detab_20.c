#include <stdio.h>
#define TABSTOP 5

int main() {
  int c, i, off;

  while ((c = getchar()) != EOF) {
    if (c == '\t') {
      i = TABSTOP - (off % TABSTOP);
      while (i-- > 0) {
        putchar(' ');
        off++;
      }

    } else {
      putchar(c);

      if (c == '\n')
        off = 0;
      else
        off++;
    }
  }

  return 0;
}
