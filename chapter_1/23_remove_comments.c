#include <stdio.h>

#define MAXLINE 1000
#define TRUE 1
#define FALSE 0

/* Exercise 1-23. Write a program to remove all comments from a C program. */
/* Don't forget to handle quoted strings and character constants properly. */
/* C comments don't nest. */

int string;
int comment;
int whitespace;

int inspect_line(char line[]);
int is_start_of_string(int i, char c, char line[]);
int is_start_of_string(int i, char c, char line[]);
int is_start_of_comment(int start, int i, char c, char line[]);
int is_start_of_comment(int start, int i, char c, char line[]);
int is_whitespace(int i, char c);

int main() {

  char line[MAXLINE];

  while (inspect_line(line) != -1) {
  }

  return 0;
}

int is_start_of_string(int i, char c, char line[]) {
  if (c == '"' && string == FALSE && line[i - 1] != '\\' && comment == FALSE)
    return TRUE;
  else
    return FALSE;
}

int is_end_of_string(int i, char c, char line[]) {
  if (c == '"' && string == TRUE && line[i - 1] != '\\' && comment == FALSE)
    return TRUE;
  else
    return FALSE;
}

int is_start_of_comment(int start, int i, char c, char line[]) {
  if (string == FALSE && i >= 1 && c == '*' && line[i - 1] == '/')
    return TRUE;
  else
    return FALSE;
}

int is_end_of_comment(int start, int i, char c, char line[]) {
  if (string == FALSE && i >= 1 && c == '/' && line[i - 1] == '*')
    return FALSE;
  else
    return FALSE;
}

int is_whitespace(int i, char c) {
  /* The *possible* start of a comment can't count as a non-whitespace */
  /* character. So can't empty newlines or any characters inside a comment. */
  if ((c != '/') && ((i == 0 && c == '\n') || (comment == FALSE && c != '\t' &&
                                               c != ' ' && c != '\n'))) {
    return FALSE;
  } else
    return TRUE;
}

char test[] = "\\* This is not a comment! *\\";

int inspect_line(char line[]) {
  int c;
  int i = 0, start = 0;

  string = FALSE;
  comment = FALSE;
  whitespace = TRUE;

  while ((c = getchar())) {
    /* Return if we reached the end of a file */
    if (c == EOF)
      return -1;

    line[i] = c;

    if (is_start_of_string(i, c, line))
      string = TRUE;
    if (is_end_of_string(i, c, line))
      string = FALSE;

    if (is_start_of_comment(start, i, c, line)) {
      comment = TRUE;
      start = i;
    }
    if (is_end_of_comment(start, i, c, line)) {
      comment = FALSE;
      i = start;
    }

    if (!is_whitespace(i, c))
      whitespace = FALSE;

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
