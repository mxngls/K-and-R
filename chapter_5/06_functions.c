#include <stdio.h>

#define SIZE 100

/* getline: get line int s, return length */
int _getline(char *s, int lim) {
  int i;

  i = lim;
  while (--lim && (*s = getchar()) != EOF && *s != '\n')
    s++;

  *++s = '\0';

  return i - lim;
}

int main() {
  char s[SIZE];

  while (_getline(s, SIZE) != EOF)
    printf("Input: %s", s);

  return 0;
}
