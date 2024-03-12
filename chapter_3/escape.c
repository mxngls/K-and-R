#include <stdio.h>

#define MAX_LEN 4095

/*
 * Exercise 3-2. Write a function escape(s,t) that converts characters
 * like newline and tab into visible escape sequences like \n and \t as
 * it copies the string t to s. Use a switch. Write a function for the
 * other direction as well, converting escape sequences into the real
 * characters. */
int main() {
  char from[MAX_LEN] = "This is a \r test string with a newline \n!";
  char to[MAX_LEN];
  char e;
  int i;
  int j;

  i = 0;
  j = 0;

  while (from[i] != '\0') {

    switch (from[i]) {
    case '\n':
      e = 'n';
      to[j++] = '\\';
      to[j] = e;
      break;
    case '\t':
      e = 't';
      to[j++] = '\\';
      to[j] = e;
      break;
    case '\r':
      e = 'r';
      to[j++] = '\\';
      to[j] = e;
      break;
    default:
      to[j] = from[i];
      break;
    }

    ++j;
    ++i;
  }

  /* Account for the null character itself */
  to[j] = from[i];

  printf("%s\n", to);

  return 0;
}
