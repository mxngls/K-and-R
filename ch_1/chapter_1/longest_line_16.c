#include <stdio.h>
#define MAXLINE 10

int getl(char s[], int lim);
void copy(char to[], char from[]);

int main() {
  int len;
  int max;
  char line[MAXLINE];
  char longest[MAXLINE];

  max = 0;

  while ((len = getl(line, MAXLINE)) > 0)
    if (len > max) {
      max = len;
      copy(longest, line);
    }

  if (max > 0)
    printf("%s", longest);

  return 0;
}

int getl(char s[], int lim) {
  int c, i;

  /*
     We increase i until we hit a newline or the end of the
     file. Though we only add characters to the input array
     if the current index is smaller than the current iteration
  */
  for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i)
    if (i < lim - 1) {
      s[i] = c;
    }

  if (c == '\n') {
    if (i <= lim - 1) {
      s[i] = c;
    }
    ++i;
  }
  /*
     The null character marks per definition the end of an
     array
  */
  if (i < lim) {
    s[i] = '\0';
  }

  return i;
}

void copy(char to[], char from[]) {
  int i;

  i = 0;

  while ((to[i] = from[i]) != '\0')
    ++i;
}
