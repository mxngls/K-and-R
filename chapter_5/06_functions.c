#include <ctype.h>
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

/* atoi:  convert s to integer; version 2 */
int atoi(char *s) {
  int i, n, sign;

  while (isspace(*s)) /* skip white space */
    s++;

  sign = (*s == '-') ? -1 : 1;
  if (*s == '+' || *s == '-') /* skip sign */
    s++;

  n = 0;
  while (isdigit(*s)) {
    n = 10 * n + (*s - '0');
    s++;
  }

  return sign * n;
}

int main() {
  char s[SIZE];

  _getline(s, SIZE);
  printf("getline: %s", s);

  int i;

  char a[SIZE] = "Hello, World!";
  char b[SIZE] = "World";
  i = _strindex(a, b);

  /* prints "8" */
  printf("strindex: %d \n", i);

  char c[SIZE] = "Hello, World!";
  char d[SIZE] = "Wod";
  i = _strindex(c, d);

  /* prints "-1" */
  printf("strindex: %d \n", i);

  char *str_num = "40";
  int num;
  num = atoi(str_num);

  /* prints 42 */
  printf("atoi: %d\n", num);

  return 0;
}
