#include <stdio.h>

#define MAX_LEN 4095
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

int expand(char s1[], char s2[]) {
  int i, j, r, n, s, e;

  r = 0;
  j = 0;
  s = 0;
  e = 0;

  /* Iterate over string one */
  for (i = 0; s1[i] != '\0'; i++) {
    /*
     * Determine if we currently parse a range sequence. Consequent '-'
     * characters are interpreted as one single one.
     *
     * In addition leading or trailing '-' characters should be 
     * interpreded literally.
     */
    if (s1[i] == '-') {
      if (i > 0 && s1[i + 1] != '\0' && r != IS_RANGE) {
        r = IS_RANGE;
      } else
        i++;
    }

    if (r != IS_RANGE) {
      s = s1[i];
    } else if ((e = s1[i]) > s) {
      /*
       * Make sure we only parse valid ranges (i.e. characters following
       * characters or digits following digits
       */
      if (is_char(s) != is_char(e)) {
        printf("Aborting. Found invalid range sequence: %s\n", s1);
        return -1;
      }

      for (n = 0; s + n <= e; n++, j++) {
        s2[j] = s + n;
      }

      /* Set the new start point */
      if (s1[i + 1] == '-')
        s = (e + 1);
      else
        s = e;

      /* Toggle range indicator */
      r = 0;
    }
  }

  s2[j] = '\0';

  return 0;
}

/*
 * Exercise 3-3. Write a function expand(s1,s2) that expands shorthand
 * notations like a-z in the string s1 into the equivalent complete list
 * abc...xyz in s2. Allow for letters of either case and digits, and be
 * prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that
 * a leading or trailing - is taken literally.
 */
int main() {
  char s1[MAX_LEN] = "a-z";
  char s2[MAX_LEN] = "a---z";
  char s3[MAX_LEN] = "-a-z-";
  char s4[MAX_LEN] = "b-e-y";
  char s5[MAX_LEN] = "a-l0-9m-z";
  char s6[MAX_LEN] = "0-e-y";
  char out[MAX_LEN];

  /* Most simple possible case */
  if (expand(s1, out) != 0)
    return -1;

  printf("String 1: %s\n", s1);
  printf("String 2: %s\n", out);
  printf("\n");

  /* Multiple range characters are interpreded as one */
  if (expand(s2, out) != 0)
    return -1;

  printf("String 1: %s\n", s2);
  printf("String 2: %s\n", out);
  printf("\n");

  /* Leading and trailing range characters are to be ignored */
  if (expand(s3, out) != 0)
    return -1;

  printf("String 1: %s\n", s3);
  printf("String 2: %s\n", out);
  printf("\n");

  /* Multiple character range sequences chained together */
  if (expand(s4, out) != 0)
    return -1;

  printf("String 1: %s\n", s4);
  printf("String 2: %s\n", out);
  printf("\n");

  /*
   * Supplying multiple range sequences consisting of characters and
   * digits
   */
  if (expand(s5, out) != 0)
    return -1;

  printf("String 1: %s\n", s5);
  printf("String 2: %s\n", out);
  printf("\n");

  /*
   * Return an error if the input string consists of an invalid range
   * sequence
   */
  if (expand(s6, out) != 0)
    return -1;

  printf("String 1: %s\n", s6);
  printf("String 2: %s\n", out);

  return 0;
}
