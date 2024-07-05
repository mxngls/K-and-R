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
 * Pad with '\0''s if ct has fewer than n characters.
 */
char *_strncpy(char *s, const char *ct, size_t n) {
  int i;

  i = 0;
  while (i++ < n && (*s++ = *ct++))
    ;

  return s;
}

int main() {
  char source[SIZE] = "Hello, World!";
  char target[SIZE];

  _strncpy(target, source, 6);

  printf("%s\n", target);

  return 0;
}
