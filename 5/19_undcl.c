#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char.c"

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };
enum { OK, ERROR };

int dcl(void);
int dirdcl(void);

int gettoken(void);
int tokentype;           /* type of last token */
char token[MAXTOKEN];    /* last token string */
char name[MAXTOKEN];     /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char out[1000];

/* return next token */
int gettoken() {
    int c;
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t')
        ;

    if (c == '(') {
        if ((c = (char)getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c == '[') {
        for (*p++ = (char)c; (*p++ = (char)getch()) != ']';)
            ;
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++ = (char)c; isalnum(c = getch());) {
            *p++ = (char)c;
        }
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else {
        return tokentype = c;
    }
}

int badd() {
    char c;
    int b = 0;

    while ((c = (char)getch()) == ' ' || c == '\t')
        ;

    if (c == '[' || c == '(')
        b = 1;
    ungetch(c);

    return b;
}

/*
 * Exercise 5-19. Modify undcl so that it does not add redundant
 * parentheses to declarations.
 *
 * Looking at table 2-1 "PRECEDENCE AND ASSOCIATIVITY OF OPERATORS" we
 * see that braches, brackets, arrows and dots have a higher precedence
 * than the asterisk that denotes pointers (see page 53). To account for
 * that add braces only in the case that the following character is part
 * of one of these operators. We only look for braces and brackets for
 * simplicities sake, as arrows and dots have not been intrduced yet.
 */
int main() {
    int type;
    char temp[MAXTOKEN];
    while (gettoken() != EOF) {
        strcpy(out, token);
        while ((type = gettoken()) != '\n')
            if (type == PARENS || type == BRACKETS)
                strcat(out, token);
            else if (type == '*') {
                if (badd()) {
                    sprintf(temp, "(*%s)", out);
                } else
                    sprintf(temp, "*%s", out);
                strcpy(out, temp);
            } else if (type == NAME) {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            } else
                printf("invalid input at %s\n", token);
        printf("%s\n", out);
    }

    return 0;
}
