#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char.c"

#define MAXTOKEN 100
#define MAXLEN 100

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

int line;

char errmsg[MAXLEN]; /* maximum length of messages describing errors */

/* dcl: parse a declarator */
int dcl(void) {
    int ns;
    for (ns = 0; gettoken() == '*';) /* count *'s */
        ns++;
    if (!dirdcl())
        return ERROR;
    while (ns-- > 0)
        strcat(out, " pointer to");
    return 0;
}

/* dirdcl: parse a direct declarator */
int dirdcl(void) {
    int type;
    if (tokentype == '(') { /* ( dcl ) */
        dcl();
        if (tokentype != ')') {
            strcpy(errmsg, "error: missing )");
            return ERROR;
        }
    } else if (tokentype == NAME) {
        strcpy(name, token);
    } else {
        strcpy(errmsg, "error: expected name or (dcl)");
        return ERROR;
    }
    while ((type = gettoken()) == PARENS || type == BRACKETS)
        if (type == PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    return 0;
}

/* return next token */
int gettoken(void) {
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

void skip_line() {
    while (tokentype != '\n' && tokentype != EOF) {
        gettoken();
    }
}

/* Exercise 5-18. Make dcl recover from input errors. */
int main() {
    while (++line && (gettoken()) != EOF) { /* 1st token on line */
        if (tokentype != '\n') {            /* is the datatype */
            strcpy(datatype, token);        /* preserve empty lines */
            out[0] = '\0';
        } else {
            printf("\n");
            continue;
        }

        /* reset column counter */
        col = 0;

        /* parse rest of line */
        if (!dcl()) {
            printf("%s: line %d column %d\n", errmsg, line, col);
            skip_line();
            continue;
        };

        if (tokentype != '\n') {
            printf("syntax error: line %d; column %d: %c\n", line, col,
                   tokentype);
            skip_line();
            continue;
        }

        printf("%s: %s %s\n", name, out, datatype);
    }
    return 0;
}
