#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define ALPHA 'A'  /* signal that a number was found */

/* ####### DEBUG ####### */
void printvars(void);
void printval(void);

int getop(char[]);
double math(char[], double n);
int cmd(char[]);
void push(double);
double pop(void);

char s[MAXOP];
double vars[26] = {0};
char var = 0;

/*
 * Exercise 4-6. Add commands for handling variables. (It's easy to
 * provide twenty-six variables with single-letter names.) Add a
 * variable for the most recently printed value.
 */
int main() {
  int type;
  double op2;

  while ((type = getop(s)) != EOF) {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case ALPHA:
      if (strlen(s) == 1) {
        if (var == 0) {
          /* Only set if variable already was assigned before */
          var = s[0];
          push(vars[var - 'a']);
        } else
          push(vars[s[0] - 'a']);
      } else {
        if (cmd(s)) {
          /*
           * Add zero to the stack as the execution of a command
           * triggers the removal of the top element of the stack
           */
          push(0);
          break;
        }
        push(math(s, pop()));
      }
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
    case '=':

      /* Get value that is to be assigned */
      op2 = pop();

      /* Assign to variable */
      vars[(int)var - 'a'] = op2;

      /* Resets our variable */
      var = 0;

      /*
       * Remove operator one from the stack and replace it with the
       * result from our variable assignment
       */
      pop();
      push(op2);

      break;
    default:
      printf("error: unknown command %s\n", s);
      break;
    }
  }
  return 0;
}

#define MAXVAL 100  /* maximum depth of val stack */
double val[MAXVAL]; /* value stack */
int sp = 0;         /* next free stack position */

/* ############################# DEBUG ############################# */

/* Inspect the value stack */
void printval(void) {
  int i;
  for (i = 0; i < sp; ++i) {
    printf("%.2f ", val[i]);
  }
  printf("\n");
}

/* Inspect the value stack */
void printvars(void) {
  int i;

  printf("\n");

  for (i = 0; i < 26; ++i) {
    if (vars[i] > 0)
      printf("%.2f ", vars[i]);
    else
      printf("_ ");
  }
  printf("\n\n");
}

/* ################################################################# */

void prints(void) {
  if (sp == 0)
    printf("error: stack empty\n");
  else if (sp == 2)
    printf("Only one element in stack: %g\n", val[sp - 1]);
  else
    printf("Top of stack: %g, %g\n", val[sp - 1], val[sp - 2]);
}

void clears(void) {
  while (sp > 0)
    sp--;
  printf("cleared stack\n");
}

void swaps(void) {
  int tmp;
  if (sp > 1) {
    tmp = val[sp - 1];
    val[sp - 1] = val[sp - 2];
    val[sp - 2] = tmp;
  } else
    printf("error: stack empty\n");
}

void dups(void) {
  if (sp > 1) {
    push(val[sp - 2]);
    push(val[sp - 2]);
  } else
    printf("error: stack empty\n");
}

double math(char s[], double n) {
  if (strncmp(s, "sin", 3) == 0) {
    return sin(n);
  } else if (strncmp(s, "cos", 3) == 0) {
    return cos(n);
  } else if (strncmp(s, "tan", 3) == 0) {
    return tan(n);
  } else
    printf("error: unknown command %s\n", s);

  return n;
}

void prints(void);
void clears(void);
void swaps(void);
void dups(void);

int cmd(char s[]) {
  if (strncmp(s, "prints", 6) == 0) {
    prints();
    return 1;
  } else if (strncmp(s, "clears", 6) == 0) {
    clears();
    return 1;
  } else if (strncmp(s, "swaps", 5) == 0) {
    swaps();
    return 1;
  } else if (strncmp(s, "dups", 4) == 0) {
    dups();
    return 1;
  }
  return 0;
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

  if (isalpha(c)) {
    s[i++] = c;

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
