#include <stdio.h>

// We limit the number of characters to
// the ASCII standard of 95 *printable*
// characters (code points 32 until 126)
#define CHAR_START 32
#define CHAR_END 127

int main() {
  int c;
  int char_arr[CHAR_END-CHAR_START];

  for (int i = 0; i < CHAR_END - CHAR_START; i++) {
    char_arr[i] = 0;
  }

  while ((c = getchar()) != EOF) {
    if ((c != ' ' || c != '\n' || c != '\t' || c != '\r') && c < CHAR_END &&  c >= CHAR_START) {
      char_arr[c - CHAR_START]++;
    }
  }

  for (int i = CHAR_START; i < CHAR_END; ++i) {
    if (char_arr[i - CHAR_START] != 0) {
      printf("%c\t%0*d\n", i, char_arr[i - CHAR_START], 0);
    }
  }

  return 0;
}
