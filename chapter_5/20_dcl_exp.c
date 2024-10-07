#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char.c"

#define MAXTOKEN 100
#define MAXLEN 100

enum { NAME, PARENS, BRACKETS };
enum { OK, ERROR };
enum { FALSE, TRUE };

int dcl(void);
int dirdcl(void);
int gettoken(void);

int tokentype;           /* type of last token */
char token[MAXTOKEN];    /* last token string */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char name[MAXTOKEN];     /* variable name */
char store[MAXTOKEN];    /* storage class specifier */
char out[1000];

int line;
extern int col;

char errmsg[MAXLEN]; /* maximum length of messages describing errors */

/* dcl: parse a declarator */
int dcl(void) {
    int ns;
    for (ns = 0; gettoken() == '*';) /* count *'s */
        ns++;

    if (dirdcl() == ERROR)
        return ERROR;

    while (ns-- > 0)
        strcat(out, " pointer to");

    return OK;
}

/* storage class specifier */
int typestore() {
    const char *st[] = {"extern", "static", NULL};
    int i;
    for (i = 0; st[i] != NULL; i++) {
        if (strcmp(st[i], token) == 0)
            return TRUE;
    }
    return FALSE;
}

/* type qualifier */
int typequal() {
    char *q = "const";
    if (strcmp(q, token) == 0)
        return TRUE;
    else
        return FALSE;
}

/* type modifier */
int typemod() {
    char *m[] = {"long", "short", "signed", "unsigned", NULL};
    int i;
    for (i = 0; m[i] != NULL; i++) {
        if (strcmp(m[i], token) == 0)
            return TRUE;
    }
    return FALSE;
}

/* type specifier */
int typespec() {
    char *s[] = {"char", "double", "float", "int", "void", NULL};
    int i;
    for (i = 0; s[i] != NULL; i++) {
        if (strcmp(s[i], token) == 0)
            return TRUE;
    }
    return FALSE;
}

int parsedt() {

    int qual = 0;
    int mod = 0;
    int spec = 0;

    do {
        if (typestore() == TRUE) {
            if (*store) {
                strcpy(errmsg, "error: duplicate storage class specifier");
                return ERROR;
            }
            strcpy(store, token);
            strcat(store, " ");
            continue;
        } else if (typequal() == TRUE) {
            if (qual) {
                strcpy(errmsg, "error: duplicate type qualifier");
                return ERROR;
            }
            qual = TRUE;
        } else if (typemod() == TRUE) {
            if (mod) {
                strcpy(errmsg, "error: duplicate type specifier");
                return ERROR;
            }
            mod = TRUE;
        } else if (typespec() == TRUE) {
            if (spec) {
                strcpy(errmsg, "error: duplicate type specifier");
                return ERROR;
            } else if (mod && strcmp(token, "void") == 0) {
                strcpy(errmsg,
                       "error:  void cannot be used with type modifiers");
                return ERROR;
            }
            spec = TRUE;
        }
        strcat(datatype, token);
        strcat(datatype, " ");
    } while (!spec && (gettoken() == NAME));

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
        if (type == PARENS) {
            strcat(out, " function returning");
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }

    return OK;
}

/* return next token */
int gettoken(void) {
    int c;
    char *p = token;

    while ((c = getch()) == ' ')
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
        for (*p++ = (char)c; isalnum(c = getch());)
            *p++ = (char)c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else {
        return tokentype = c;
    }
}

void skip2end() {
    while (tokentype != '\n' && tokentype != EOF) {
        gettoken();
    }
}

/*
 * Exercise 5-20. Expand dcl to handle declarations with function argument
 * types, qualifiers like const, and so on.
 *
 * (Based upon our solution for exercise 5-18)
 */
int main() {
    while (++line && (gettoken()) != EOF) { /* 1st token on line */
        if (tokentype == '\n') {            /* preserve empty lines */
            printf("\n");
            continue;
        }

        store[0] = '\0';
        datatype[0] = '\0';
        out[0] = '\0';

        /* parse the data type and proceed to the rest of the line */
        if (parsedt() == ERROR || dcl() == ERROR) {
            printf("%s: line %d column %lu\n", errmsg, line,
                   col - strlen(token) + 1);
            skip2end();
            continue;
        };

        if (tokentype != '\n') {
            printf("syntax error: line %d; column %d: %c\n", line, col,
                   tokentype);
            skip2end();
            continue;
        }

        /* reset column counter */
        col = 1;

        printf("%s%s: %s %s\n", store, name, out, datatype);
    }
    return 0;
}
