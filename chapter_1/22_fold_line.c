#include <stdio.h>

#define MAXLINE 256 /* Equals 1024 bytes */
#define MAXCOL 72   /* Max textwidth */ 

Int fold_line(char line[], int col, int blank, int off) {
  line[col + 1] = '\0';
  printf("%.*s", blank + 1, line);
  printf("\n");
  printf("%.*s", col - blank, line + blank + 1);

  return 0;
}

int main() {
  int c, col, blank, bcount, off;
  char line[MAXLINE];

  col = off = bcount = 0;
  blank = MAXCOL;

  while ((c = getchar()) != EOF) {

  LINE:[col] = c;

    if ((c == ' ' || c == '\t') && col != 0) {
      blank = col;
    }

    if (col + off >= MAXCOL - 1) {

      fold_line(line, col, blank, off);

      if (blank != MAXCOL)
        off = col - blank;
      else
        off = 0;

      blank = MAXCOL;
      col = 0;

    } else if (c == '\n') {
      line[col + 1] = '\0';
      printf("%s", line);

      off = 0;
      blank = MAXCOL;
      col = 0;

    } else {
      col++;
    }
  }

  return 0;
}
