#include <stdio.h>

#define MAX_LEN 4095
#define IS_CHAR 1
#define IS_DIGIT 2
#define IS_RANGE 1

int is_char(int x) {
  if ((x >= 65 && x <= 90) || (x >= 97 && x <= 122))
    return 1;
  else
    return 0;
}

int is_digit(int x) {
  if (x >= 48 && x <= 56)
    return 1;
  else
    return 0;
}

void expand(char s1[], char s2[]) {
  int i, j, m, r, n, s, e;

  /* r = m = j = s = e = 0; */

  r = 0;
  m = 0;
  j = 0;
  s = 0;
  e = 0;

  /* Iterate over string one */
  for (i = 0; s1[i] != '\0'; i++) {
    /* printf("Current char: %c\n", s1[i]); */
    /* printf("Current char as digit: %d\n", s1[i]); */

    /*
     * TODO: Make sure we only parse a range as such if it is valid
     * (i.e. a digit follows a previous digit and not a character).
     */

    /* Check if the current character is a valid character */
    if (is_char(s1[i])) {
      m = IS_CHAR;
    } else if (is_digit(s1[i])) {
      m = IS_DIGIT;
    } else if ((s1[i] == '-' && r != IS_RANGE)) {
      r = IS_RANGE;
    }

    if (r != IS_RANGE) {

      s = s1[i];

    } else if ((e = s1[i]) > s) {

      for (n = 0; s + n < e; n++, j++) {
        /* printf("Current range char: %c\n", s + n); */
        s2[j] = s + n;
      }

      s2[j] = s + n; /* Get the last char as well */
      s = e;         /* Set the new start point */
      r = 0;         /* Toggle range indicator */
    }

    /*
     * If the current character is not a character check if it indicates
     * a range sequence
     */
  }
  s2[++j] = '\0';
}

/*
 * Exercise 3-3. Write a function expand(s1,s2) that expands shorthand
 * notations like a-z in the string s1 into the equivalent complete list
 * abc...xyz in s2. Allow for letters of either case and digits, and be
 * prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that
 * a leading or trailing - is taken literally.
 */
int main() {
  char s1[MAX_LEN] = "a-e-k-m-x-z";
  char s2[MAX_LEN];

  expand(s1, s2);

  printf("String 1: %s\n", s1);
  printf("String 2: %s\n", s2);

  return 0;
}
