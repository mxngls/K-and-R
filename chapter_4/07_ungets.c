#include <stdio.h>
#include <string.h>

#define BUFSIZE 100
#define MAXLEN 10000

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

int getch(void) /* get a (possibly pushed-back) character */
{
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters (current char: %c)\n", c);
  else
    buf[bufp++] = c;
}

/* ungets: push back s onto the input */
void ungets(char s[]) {
  int i;

  for (i = strlen(s) - 1; i >= 0; --i)
    ungetch(s[i]);
}

/*
 * exercise 4-7. write a routine ungets(s) that will push back an entire
 * string onto the input. should ungets know about buf and bufp, or
 * should it just use ungetch?
 *
 * answer:
 *
 * since the ungetch routine handles pushback of single characters, it
 * makes sense to simply define ungetc in a way that calls ungetch
 * consecutively until no more characters to push back are left.
 *
 */
int main() {
  int c, i;
  char s[MAXLEN];

  printf("Enter string to test ungets function:\n");

  i = 0;
  while ((c = getch()) != EOF && i < MAXLEN - 1) {
    if (c == '\n') {
      s[i] = '\0';
      i = 0;

      printf("Pushing back: %s\n", s);
      ungets(s);
      printf("Pushed back string. Now reading it back:\n");

	   /* print the whole input string again */
      for (c = 0; s[c] != '\0'; c++)
        putchar(s[c]);

    } else {
      s[i++] = c;
    }
  }

  return 0;
}
