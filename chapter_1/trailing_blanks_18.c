#include <stdio.h>
#define MAX_LEN 80

int main(void) {
  int c;
  char line[MAX_LEN];
  int line_len;
  int start;

  line_len = 0;
  start = 0;

  while ((c = getchar()) != EOF) {
    if (start == 0) {
      if (c != ' ' && c != '\t' && c != '\n') {
        line[line_len] = c;
        ++line_len;
        start = 1;
      }
    } else {
      line[line_len] = c;
      ++line_len;
    }

    if (c == '\n') {
      line[line_len] = '\0';
      printf("%s", line);

      line[0] = '\0';
      line_len = 0;
      start = 0;
    }
  }
  return 0;
}
