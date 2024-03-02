#include <stdio.h>

int any(char s1[], char s2[]) {
  int i, j;

  /* TODO: Sorting both arrays might significantly speed up the
   * execution time. The implementation as it is right now is overly
   * naive. Getting then rid of duplicates via storing unique values
   * from s2 in a hash map might further improve things.
   */
  for (i = 0; s1[i] != '\0'; ++i)
    for (j = 0; s2[j] != '\0'; ++j)
      if (s1[i] == s2[j]) {
        return i;
      }

  return -1;
}

/* Exercise 2-5. Write the function any(s1,s2), which returns the first
 * location in a string s1 where any character from the string s2
 * occurs, or -1 if s1 contains no characters from s2. (The standard
 * library function strpbrk does the same job but returns a pointer to
 * the location.)
 */
int main() {
  char s1[] = "This is a test";
  char s2[] = "a test";

  printf("%d\n", any(s1, s2));

  return 0;
}
