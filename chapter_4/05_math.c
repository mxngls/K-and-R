#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define ALPHA 'A'  /* signal that a number was found */
#define MAXVAL 100 /* maximum depth of val stack */

int getop(char[]);
void prints(void);
void clears(void);
void swaps(void);
void dups(void);
double parse(char[], double);
void push(double);
double pop(void);

char s[MAXOP];

/* reverse Polish calculator */
int main() {
  int type;
  double op2;

  while ((type = getop(s)) != EOF) {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case ALPHA:
      push(parse(s, pop()));
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

double parse(char s[], double n) {
  if (strncmp(s, "sin", 3) == 0) {
    return sin(n);
  } else if (strncmp(s, "cos", 3) == 0) {
    return cos(n);
  } else if (strncmp(s, "tan", 3) == 0) {
    return tan(n);
  } else {
    printf("error: unrecognized operator\n");
    return 0.0;
  }
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

  /*
   * Keep parsing characters as long as there are some. Then return a
   * value indicating that the main program now has to deal with a
   * sequence of multiple characters. The way we structured our programm
   * before forces us now to account for a couple of edge cases so that
   * we don't skip one of the possible four instructions introduced in
   * exercise 4-3. I saw other solutions that took a different approach
   * and instead mapped the mathematical functions to other characters.
   * I think both are decent approaches. I chose to deal with characters
   * so that users may use the name of library functions as they are
   * provided by math.h
   */

  int f;
  if (isalpha(c)) {
    f = c;
    if (!isalpha(c = getch())) {
      return f;
    } else {
      s[i++] = f;
      s[i++] = c;
    }

    while (isalpha(c = getch())) {
      s[i++] = c;
    }

    ungetch(c);

    s[i] = '\0';

    return ALPHA;
  }

  if (!isdigit(c) && c != '.' && c != '-')
    return c; /* not a number */

  if (c == '-') {
    if (isdigit(c = getch())) { /* the '-' is an algebraic sign */
      s[++i] = c;
    } else { /* the '-' is an operator */
      ungetch(c);
      return '-';
    }
  }

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
