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

/* strindex: return index of t in s, -1 if none */
int _strindex(char *s, char *t) {
  int i, j;

  i = 0;
  j = 0;
  while (*s && *t) {
    if (*s++ == *t) {
      if (!j)
        j = i;
      t++;
    } else if (j) {
      return -1;
    }
    i++;
  }

  return j;
}

int main() {
  char s[SIZE];

  while (_getline(s, SIZE) > 0)
    printf("Input: %s", s);

  int i;

  char a[SIZE] = "Hello, World!";
  char b[SIZE] = "World";
  i = _strindex(a, b);

  /* prints "8" */
  printf("%d \n", i);

  char c[SIZE] = "Hello, World!";
  char d[SIZE] = "Wod";
  i = _strindex(c, d);

  /* prints "-1" */
  printf("%d \n", i);
  return 0;
}
