#include <stdio.h>

#define MAXLEN 100

/* reverse: reverse string s in place */
void reverse(char s[]) {
  static int i = -1;
  static int j = -1;
  char c;

  if (s[++i] != '\0')
    reverse(s);

  /* stop after all elements are swapped */
  if (j++ > i--)
    return;

  c = s[j];
  s[j] = s[i];
  s[i] = c;
}

int main() {
  char ex[MAXLEN] = "This is a test!";

  reverse(ex);

  printf(">>> %s", ex);

  return 0;
}
