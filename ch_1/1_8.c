#include <stdio.h>

int main() {
  long whitespace;
  int c;

  whitespace = 0;

  while ((c = getchar()) != EOF) {
    if (c == '\n' || c == '\t' || c == ' ') {
      ++whitespace;
    };
  };

  printf("%ld\n", whitespace);

  return 0;
}
