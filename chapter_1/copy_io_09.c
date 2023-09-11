#include <stdio.h>

int main() {

  int last, c;

  last = 0;

  while ((c = getchar()) != EOF) {
    if (c == ' ') {
      if (last != ' ') {
        putchar(c);
      };
      last = ' ';
    } else {
      last = 0;
      putchar(c);
    };
  };

  return 0;
}
