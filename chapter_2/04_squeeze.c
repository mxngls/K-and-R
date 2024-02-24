#include <stdio.h>

void squeeze(char s1[], char s2[]) {
  /* TODO: Store unique character values via hash maps*/
  /* This would propably be solved better with a hash map, but right now
   * I have no idea how to implement this myself so we will need to
   * write a less performant version.
   */
  int i, j, n;
  i = 0;
  j = 0;
  n = 0;

  while (s1[i] != '\0') {
    /* TODO: Reduce nested loops. */
    /* Check if the current character of s1 matches one of the
     * characters present in s2
     */

    j = 0;

    /* Increment over s2 */
    while (s2[j] != '\0') {
      if (s1[i] == s2[j]) {
        n = i;
        /* Shift characters of s1 by one character to get rid of the
         * matched character
         */
        while (s1[n] != '\0') {
          s1[n] = s1[n + 1];
          n++;
        }
        s1[n - 1] = '\0';
      }
      j++;
    }
    i++;
  }
}

/*
 * Exercise 2-4. Write an alternative version of squeeze(s1,s2) that
 * deletes each character in s1 that matches any character in the
 * string s2.
 */
int main() {
  char s1[] = "This is a test";
  char s2[] = "ttttt";

  printf("String one: %s\nString two: %s\n", s1, s2);

  squeeze(s1, s2);

  printf("String one modified: %s\n", s1);

  return 0;
}
