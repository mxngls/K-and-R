#include <stdio.h>

int main() {

  int c;

  while (((c = getchar()) != EOF) != 0) {
    putchar(c);
  }

  return 0;
}
