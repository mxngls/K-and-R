#include <stdio.h>

#define SIZE 100

/* getline: get line int s, return length */
int _getline(char *s, int lim) {
  int i;

  i = 0;
  while (--lim && (*s = getchar()) != EOF && *s != '\n') {
    s++;
    i++;
  }

  *++s = '\0';

  return i;
}
}

int main() {
  char s[SIZE];

  while (_getline(s, SIZE) > 0)
    printf("Input: %s", s);

  return 0;
}
