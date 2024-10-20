#include <stdio.h>

char buf; /* buffer for ungetch */

int getch(void) {
    int c;
    if (buf > 0) {
        c = buf;
    } else {
        c = getchar();
    }
    buf = 0;
    return c;
}

void ungetch(int c) {
    if (buf != 0)
        printf("ungetch: too many characters\n");
    else
        buf = c;
}

/*
 * Exercise 4-8. Suppose that there will never be more than one
 * character of pushback. Modify getch and ungetch accordingly.
 */
int main(void) {
    int c;

    while ((c = getch()) != EOF && c != '\n')
        ;

    /*
     * putchar should print an additional new line only if ungetch is
     * called. Otherwise the output will equal the first character
     * of the second input line, as this is the current value of buf.
     * see the following explanation about the inner workings of getchar:
     * https://stackoverflow.com/questions/37100201/
     */
    ungetch(c);
    c = getch();
    putchar(c);

    return 0;
}
