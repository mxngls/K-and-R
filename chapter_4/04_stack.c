#include <math.h>
#include <stdio.h>
#include <stdlib.h> /* for  atof() */

#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define MAXVAL 100 /* maximum depth of val stack */

int getop(char[]);
void prints(void);
void clears(void);
void swaps(void);
void dups(void);
void push(double);
double pop(void);

char s[MAXOP];

/* reverse Polish calculator */
int main() {
  int type, tmp;
  double op2;

  while ((type = getop(s)) != EOF) {
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
        printf("error: zero divisor\n");
      break;
    case '%':
      op2 = pop();
      if (op2 != 0.0)
        push(fmod(pop(), op2));
      else
        printf("error: zero modulo\n");
      break;
    case '\n':
      printf("\t%.8g\n", pop());
      break;
    case 'p':
      prints();
      break;
    case 'c':
      clears();
      break;
    case 's':
      swaps();
      break;
    case 'd':
      dups();
      break;
    default:
      printf("error: unknown command %s\n", s);
      break;
    }
  }
  return 0;
}

double val[MAXVAL]; /* value stack */
int sp = 0;         /* next free stack position */

void prints(void) {
  if (sp == 0)
    return;
  else if (sp > 1) {
    printf("Current top of stack: %g %g\n", val[sp - 1], val[sp - 2]);
  } else
    printf("Only one element in stack: %g\n", val[sp - 1]);
  getchar();
}

void clears(void) {
  if (sp > 0) {
    while (--sp > 0)
      ;
    printf("cleared stack\n");
    getchar();
  }
}

void swaps(void) {
  int tmp;
  if (sp > 1) {
    tmp = val[sp - 1];
    val[sp - 1] = val[sp - 2];
    val[sp - 2] = tmp;
    getchar();
  }
}

void dups(void) {
  if (sp > 1) {
    push(val[sp - 2]);
    push(val[sp - 2]);
    getchar();
  } else
    printf("not enough elements in stack");
}

/* push:  push f onto value stack */
void push(double f) {
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}

/* pop:  pop and return top value from stack */
double pop(void) {
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop:  get next character or numeric operand */
int getop(char s[]) {
  int i, c;

  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;

  s[1] = '\0';

  i = 0;

  if (c == '-') {
    if (isdigit(c = getch())) { /* the '-' is an algebraic sign */
      s[++i] = c;
    } else { /* the '-' is an operator */
      ungetch(c);
      return '-';
    }
  } else if (!isdigit(c) && c != '.')
    return c; /* not a number */

  if (isdigit(c))
    while (isdigit(s[++i] = c = getch()))
      ;

  if (c == '.') /* collect fraction part */
    while (isdigit(s[++i] = c = getch()))
      ;

  s[i] = '\0';

  if (c != EOF)
    ungetch(c);

  return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

int getch(void) /* get a (possibly pushed-back) character */
{
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}
