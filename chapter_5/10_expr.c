#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define OPERATOR 'O'
#define NUMBER 'N'

char getop(char *op, int *num, char *argv);
void push(int);
int pop(void);

/*
 * Exercise 5-10. Write the program expr, which evaluates a reverse
 * Polish expression from the command line, where each operator or
 * operand is a separate argument. For example,
 *
 *      expr 2 3 4 + *
 *
 *   evaluates 2 * (3+4).
 */
int main(int argc, char **argv) {
    char type;
    char op;
    int num;
    int num_2;

    ++argv;
    while (--argc && *++argv) {
        num = 0;
        num_2 = 0;
        op = '\0';
        type = getop(&op, &num, *argv);
        if (type == NUMBER)
            push(num);
        else {
            switch (op) {
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                num_2 = pop();
                push(pop() - num_2);
                break;
            case '/':
                num_2 = pop();
                if (num_2 != 0.0)
                    push(pop() / num_2);
                else
                    printf("error: zero divisor\n");
                break;
            default:
                printf("error: unknown command %s\n", *argv);
                break;
            }
        }
    }

    printf("%d\n", pop());

    return 0;
}

char getop(char *op, int *num, char *argv) {
    char c;

    while ((*op = c = *argv++)) {

        if (!isdigit(c)) {
            return OPERATOR;
        }
        *num = 10 * *num + atoi(&c);
    }

    return NUMBER;
}

#define MAXVAL 100 /* maximum depth of val stack */

int sp = 0;      /* next free stack position */
int val[MAXVAL]; /* value stack */

/* push:  push f onto value stack */
void push(int n) {
    if (sp < MAXVAL)
        val[sp++] = n;
    else
        printf("error: stack full, can't push %d\n", n);
}

/* pop:  pop and return top value from stack */
int pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0;
    }
}
