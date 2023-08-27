#include <stdio.h>
#define BUFFER 1000
#define TABSTOP 2

int main() {
	int c, i;

  while ((c = getchar()) != EOF) {
    if (c == '\t') {
      i = 0;
      while (++i <= TABSTOP) {
        putchar(' ');
      }
      i = 0;
    } else {
      putchar(c);
    }
  }

  return 0;
}
