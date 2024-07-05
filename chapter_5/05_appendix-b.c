#include <stddef.h>
#include <stdio.h>

#define SIZE 100

/*
 * Exercise 5-5. Write versions of the library functions strncpy,
 * strncat, and strncmp, which operate on at most the first n
 * characters of their argument strings. For example, strncpy(s,t,n)
 * copies at most n characters of t to s. Full descriptions are in
 * Appendix B
 */

/*
 * copy at most n characters of string ct to s; return s.
 * Pad with '\0's if ct has fewer than n characters.
 */
char *_strncpy(char *s, const char *ct, size_t n) {
  int i;

  i = 0;
  while (i++ < n) {
    if (*ct)
      *s++ = *ct++;
    else
      *s++ = '\0';
  }

  return s;
}

int main() {
  char source[SIZE] = "Hello, World!";
  char target_1[SIZE];
  char target_2[SIZE];

  _strncpy(target_1, source, 6);

  /* prints "Hello," */
  printf("%s\n", target_1);

  _strncpy(target_2, source, 20);

  /* visualize null bytes used as padding */
  int i = -1;
  while (source[++i])
    printf("%c\n", target_1[i]);

  return 0;
}
