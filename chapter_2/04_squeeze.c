#include <stdio.h>

void squeeze(char s1[], char s2[]) {
  /* TODO: Store unique character values via hash maps*/
  /* This would propably be solved better with a hash map, but right now
   * I have no idea how to implement this myself so we will need to
   * write a less performant version.
   */
  int i, j, n;
  i = 0;

  while (s2[i] != '\0') {

    j = 0;
    n = 0;

    /* Shift characters of s1 by one character to get rid of the
     * matched character if there is one.
     */
    while (s1[j++] != '\0') {
      if (s1[j] != s2[i])
        s1[n++] = s1[j];
    }

    s1[n] = '\0';

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
  char s2[] = "This";

  printf("String one: %s\nString two: %s\n", s1, s2);

  squeeze(s1, s2);

  printf("String one modified: %s\n", s1);

  return 0;
}
