#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> /* for atof() */

#define MAXOP 100   /* max size of operand or operator */
#define MAXLINE 100 /* max line size */
#define NUMBER '0'  /* signal that a number was found */

/* function prototypes */
int getop(char p[]);
int getline_(char l[], int limit);
void push(double);
double pop(void);

/* global variables */
char line[MAXLINE];
int col;

/* getline:  get line into s, return length */
int getline_(char s[], int lim) {
  int c, i;

  i = 0;
  while (i < lim && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';

  return i;
}

/*
 * Exercise 4-10. An alternate organization uses getline to read an
 * entire input line; this makes getch and ungetch unnecessary.
 * Revise the calculator to use this approach.
 *
 * Instead of revising our solution for 4-6 we instead took sample
 * implementation presented in K&R and adjusted the getop function to
 * utilize getline to read from an input stream.
 */
int main(void) {

  while (getline_(line, MAXLINE) > 0) {
    int type;
    double op2;
    char s[MAXOP];

    col = 0;
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
  int i, c;

  while ((s[0] = c = line[col++]) == ' ' || c == '\t')
    ;

  s[1] = '\0';
  if (!isdigit(c) && c != '.')
    return c; /* not a number */
  i = 0;

  if (isdigit(c)) /* collect integer part */
    while (isdigit(s[++i] = c = line[col++]))
      ;

  if (c == '.') /* collect fraction part */
    while (isdigit(s[++i] = c = line[col++]))
      ;

  s[i] = '\0';

  return NUMBER;
}
