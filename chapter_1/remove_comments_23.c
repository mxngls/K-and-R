#include <stdio.h>

#define MAXLINE 1000
#define IN_STRING 1
#define OUT_STRING 0

/* Exercise 1-23. Write a program to remove all comments from a C program. */
/* Don't forget to handle quoted strings and character constants properly. */
/* C comments don't nest. */

int get_line(char current_line[]);

int main() {

  /* TODO */
  /* 1.  Remove lines that consist of of nothing more than whitespace after */
  /*     comments got removed. */ 
  /* 2.  Add a input file as a test case and validate its output. */

  int l;
  char current_line[MAXLINE];

  while ((l = get_line(current_line)) != -1) {
  }

  return 0;
}

int get_line(char current_line[]) {
  int c;
  int i = 0, s = 0, col = 0;

  while ((c = getchar())) {
    /* Return if we reached the end of a file */
    if (c == EOF)
      return -1;

    current_line[i] = c;

    /* Or the current_line */
    if (c == '\n') {
      current_line[i + 1] = '\0';
      printf("%s", current_line);
      return i;
    }

    /* Check if the current column resides inside a string literal */
    if (c == '"' && col == OUT_STRING)
      col = IN_STRING;
    else if (c == '"' && col == IN_STRING)
      col = OUT_STRING;

    /* Reset the cursor when a valid comment was found */
    if (col == OUT_STRING && i >= 1 && c == '*' && current_line[i - 1] == '/')
      s = i;
    else if (col == OUT_STRING && i >= 1 && c == '/' &&
             current_line[i - 1] == '*') {
      i = s - 2;
    }

    i++;
  }

  return i;
}
