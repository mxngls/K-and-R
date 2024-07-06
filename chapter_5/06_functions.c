#include <stdio.h>

#define SIZE 100

/* getline: get line int s, return length */
int _getline(char *s, int lim) {
  int i;
  int c;

  i = 0;
  c = 0;
  while (--lim && (c = getchar()) != EOF && c != '\n') {
    i++;
    *s++ = c;
  }

  if (c == '\n')
    *s++ = '\n';

  *s = '\0';

  return i;
}

int main() {
  char s[SIZE];

  while (_getline(s, SIZE) != EOF)
    printf("Input: %s", s);

  return 0;
}
