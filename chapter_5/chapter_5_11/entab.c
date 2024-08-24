#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define TABSTOP 8    /* default tabstop */
#define MAXSTOP 800  /* maximium number of digits for a valid tabstop */

void parse_ts(char *arg, int ts[]) {

  char d[MAXSTOP];

  int i, n, c;

  i = n = c = 0;
  while (*arg && c++ < MAXSTOPS) {
    if (*arg == ',') {
      ts[n++] = atoi(d);
      i = 0;
    }

    if (isdigit(*arg) && i < MAXSTOP) {
      d[i++] = *arg;
      d[i] = '\0';
    }
    ++arg;
  }

  if (i > 0) {
    ts[n++] = atoi(d);
  }
}

int main(int argc, char **argv) {
  int ts[MAXSTOP];

  if (argc != 2) {
    printf("Usage: entab -t[tabstops]\n");
    return 1;
  }

  parse_ts((++argv)[0], ts);

  /* DEBUG *
  int i = 0;
  while (ts[i] != '\0') {
    printf("Current value of ts: %d\n", ts[i]);
    i++;
  }
  */

  /* int c; /1* current char *1/ */
  /* int s; /1* number of spaces *1/ */
  /* s = 0; */

  /* while ((c = getchar()) != EOF) { */
  /*   if (c == ' ') { */
  /*     if (++s % TABSTOP == 0) { */
  /*       putchar('\t'); */
  /*       s = 0; */
  /*     } */
  /*   } else { */
  /*     if (s % TABSTOP != 0) { */
  /*       while (s-- > 0) */
  /*         putchar(' '); */
  /*     } */

  /*     putchar(c); */

  /*     s = 0; */
  /*   } */
  /* } */

  return 0;
}
