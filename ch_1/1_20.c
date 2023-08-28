#include <stdio.h>
#define TABSTOP 8

int main() {
  int c, i;

  while ((c = getchar()) != EOF) {
    if (c == '\t') {
      i = TABSTOP;
      while (i-- > 0) {
        putchar(' ');
      }
    } else {
      putchar(c);
    }
  }

  return 0;
}
