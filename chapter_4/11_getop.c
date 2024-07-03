#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> /* for atof() */

#define MAXOP 100   /* max size of operand or operator */
#define MAXLINE 100 /* max line size */
#define NUMBER '0'  /* signal that a number was found */

/* function prototypes */
int getop(char p[]);
void push(double);
double pop(void);

/*
 * Exercise 4-11. Modify getop so that it doesn't need to use ungetch.
 * Hint: use an internal static variable.

 * Instead of revising our solution for 4-6 we instead took sample
 * implementation presented in K&R and adjusted the getop function to
 * utilize getline to read from an input stream.
 */
int main(void) {

  int type;
  double op2;
  char s[MAXOP];

  while ((type = getop(s)) != '\0') {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      push(pop() + pop());
      break;
    case '*':
      push(pop() * pop());
      break;
    case '-':
      op2 = pop();
      push(pop() - op2);
      break;
    case '/':
      op2 = pop();
      if (op2 != 0.0)
        push(pop() / op2);
      else
        printf("error: zero dvisor\n");
      break;
    case '\n':
      printf("\t%.8g\n", pop());
      break;
    default:
      printf("error: unknown command %s\n", s);
      break;
    }
  }
  return 0;
}

#define MAXVAL 100 /* maximum depth of val stack */

int sp = 0;         /* next free position */
double val[MAXVAL]; /* value stack */

/* push:  push f onto value stack */
void push(double f) {
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}

/* pop:   pop and reutrn top value from stack */
double pop(void) {
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

/* getop: get next operator or numeric operand */
int getop(char s[]) {
  int i;
  static int c = ' '; /* Initialize to space to trigger first read */

  if (c == EOF)
    return '\0';

  /* Skip whitespace, except newline */
  while (c == ' ' || c == '\t')
    c = getchar();

  s[0] = c;
  s[1] = '\0';
  if (!isdigit(c) && c != '.') {
    /*
     * reset c to a space (otherwise we enter an infinite loop as c will 
     * never get updated, just returned over and over)
     */
    int res = c;
    c = ' ';
    return res;
  }

  i = 0;
  if (isdigit(c)) /* collect integer part */
    while (isdigit(s[++i] = c = getchar()))
      ;

  if (c == '.') /* collect fraction part */
    while (isdigit(s[++i] = c = getchar()))
      ;

  s[i] = '\0';

  return NUMBER;
}
