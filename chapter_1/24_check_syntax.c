#include <stdio.h>

/*
 * Exercise 1-24. Write a program to check a C program for rudimentary
 * syntax errors like unmatched parentheses, brackets and braces. Don't
 * forget about quotes, both single and double, escape sequences, and
 * comments. (This program is hard if you do it in full generality.)
 */

/* 
 * TODO: 
 * - As indicated in the answer book, it might be better to not check if the
 *   result of the application of the modulus operator with the factor two is zero,
 *   but instead simply increment and decrement the values for parentheses, etc.
 *   In doing so, while additionally keeping track of the current line and column
 *   number, we could print more detailed error messages that indicate where
 *   exactly an unmatched number of braces occurred.
*/

/* Get the character immediately following a comment. */
char end_comment() {
  int p = getchar(); /* previous char */
  int c = getchar(); /* current char */

  while (p != '*' || c != '/') {
    p = c;
    c = getchar();
  }

  return c = getchar();
}

/* Get the character immediately following a quote. */
char end_quote(char q) {
  int c;

  while ((c = getchar()) != q) {
    if (c == '\\')
      getchar();
  }

  return c = getchar();
}

int main() {
  int c;
  int parants = 0;
  int braces = 0;
  int brackets = 0;

  while ((c = getchar()) != EOF) {
    if (c == '/') {
      /* Jump to the end of a comment */
      if ((c = getchar()) == '*') {
        c = end_comment();
      }
      /* Jump to the end of a quote */
    } else if (c == '\'' || c == '"') {
      c = end_quote(c);
    }

    if (c == '{') {
      braces++;
    } else if (c == '}') {
      braces--;
    } else if (c == '(') {
      parants--;
    } else if (c == ')') {
      parants--;
    } else if (c == '[') {
      brackets--;
    } else if (c == ']') {
      brackets--;
    }

    printf("%c", c);
  }

  if (parants % 2 != 0) {
    printf("Unmatched number of parantheses!");
  } else if (braces % 2 != 0) {
    printf("Unmatched number of braces!");
  } else if (brackets % 2 != 0) {
    printf("Unmatched number of brackets!");
  }

  return 0;
}
