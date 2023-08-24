#include <stdio.h>
#define MAX_LEN 72

int reverse(char line[], char reversed_line[]) {
  int i;
  int j;
  int l;

  j = 0;
  i = 0;

  while (line[i] != '\0' && i <= MAX_LEN) {
    ++i;
  }

  l = i;

  while (j < l) {
    reversed_line[j++] = line[--i];
  }

  reversed_line[j] = '\0';

  return j;
}

int main() {
  int c;
  int i = 0;
  char line[MAX_LEN];
  char reversed_line[MAX_LEN];

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      line[i] = '\0';

      reverse(line, reversed_line);

      printf("%s\n", reversed_line);

      i = 0;
    } else {
      line[i] = c;
      i++;
    }
  }

  return 0;
}
