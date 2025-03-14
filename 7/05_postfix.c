#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */

int getop(char[]);
void push(double);
double pop(void);

/* reverse Polish calculator */
int main() {

        int type;
        double op2;
        char s[MAXOP];

        while ((type = getop(s)) != EOF) {
                /* printf("s: %s\n", s); */
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

int sp = 0;         /* next free stack position */
double val[MAXVAL]; /* value stack */

/* push: push f onto value stack */
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

/* getop: get next character or numeric operand */
int getop(char s[]) {
        char c;
        float f;

        /* skip any leading whitespace */
        while ((c = (char)getchar()) == ' ' || c == '\t') {}
        if (!isdigit(c)) {
                s[0] = c;
                s[1] = '\0';
                return c;
        }

        /* try to read as a number */
        ungetc(c, stdin);
        if (scanf("%f", &f) == 1) {
                sprintf(s, "%g", f);
                return NUMBER;
        }

        return 0;
}
