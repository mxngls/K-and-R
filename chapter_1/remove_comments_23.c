#include <stdio.h>

#define MAXLINE 1000
#define TRUE 1
#define FALSE 0

/* Exercise 1-23. Write a program to remove all comments from a C program. */
/* Don't forget to handle quoted strings and character constants properly. */
/* C comments don't nest. */

int get_line(char current_line[]);

int main() {

  int l;
  char line[MAXLINE];

  while ((l = get_line(line)) != -1) {
  }

  return 0;
}

int get_line(char line[]) {
  int c;
  int i = 0, start = 0, string = FALSE, comment = FALSE, whitespace = TRUE;

  while ((c = getchar())) {
    /* Return if we reached the end of a file */
    if (c == EOF)
      return -1;

    line[i] = c;
 
    /* Check if the current column resides inside a string literal */
    if (c == '"' && string == FALSE && comment == FALSE)
      string = TRUE;
    else if (c == '"' && string == TRUE && comment == FALSE)
      string = FALSE;

    /* Reset the cursor when a valid comment was found */
    if (string == FALSE && i >= 1 && c == '*' && line[i - 1] == '/') {
      start = i;
      comment = TRUE;
    } else if (string == FALSE && i >= 1 && c == '/' && line[i - 1] == '*') {
      i = start;
      comment = FALSE;
    }

    /* The *possible* start of a comment can't count as a non-whitespace */
    /* character. So can't empty newlines or any characters inside a comment. */
    if ((c != '/') &&
        ((i == 0 && c == '\n') ||
         (comment == FALSE && c != '\t' && c != ' ' && c != '\n'))) {
      whitespace = FALSE;
    }

    /* Or the line */
    /* "This is a string inside a comment" */
    if (c == '\n') {
      break;
    }

    /* printf("%c | %d %d \n", c, whitespace, comment); */

    i++;
  }

  if (whitespace == FALSE) {
    line[i + 1] = '\0';
    printf("%s", line);
    return i;
  }

  line[0] = '\0';
  return i;
}
