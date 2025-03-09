#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char.c"

#define MAXTOKEN 1000
#define MAXLEN 1000

enum { NAME, PARENS, BRACKETS };
enum { OK, ERROR };
enum { FALSE, TRUE };

int dcl(char *out, char *name);
int dirdcl(char *out, char *name);
int gettoken(void);

int tokentype;        /* type of last token */
char token[MAXTOKEN]; /* last token string */
char store[MAXTOKEN]; /* storage class specifier */

const char *store_t[] = {"extern", "static", NULL};
const char *spec_t[] = {"char", "double", "float", "int", "void", NULL};
const char *mod_t[] = {"long", "short", NULL};
const char *smod_t[] = {"signed", "unsigned", NULL};
const char *qual_t[] = {"const", "volatile", NULL};

int line;
extern int col;

char errmsg[MAXLEN]; /* maximum length of messages describing errors */

/* storage class specifier */
int typestore() {
    int i;
    for (i = 0; store_t[i] != NULL; i++) {
        if (strcmp(store_t[i], token) == 0)
            return TRUE;
    }
    return FALSE;
}

/* type qualifier */
int typequal() {
    int i;
    for (i = 0; qual_t[i] != NULL; i++) {
        if (strcmp(qual_t[i], token) == 0)
            return TRUE;
    }
    return FALSE;
}

/* type modifier */
int typemod() {
    int i;
    for (i = 0; mod_t[i] != NULL; i++) {
        if (strcmp(mod_t[i], token) == 0)
            return -1;
    }
    for (i = 0; smod_t[i] != NULL; i++) {
        if (strcmp(smod_t[i], token) == 0)
            return 1;
    }
    return FALSE;
}

/* type specifier */
int typespec() {
    int i;
    for (i = 0; spec_t[i] != NULL; i++) {
        if (strcmp(spec_t[i], token) == 0)
            return TRUE;
    }
    return FALSE;
}

int pdt(char *dt) {

    int nqual = 0;
    int nmod = 0;
    int nsmod = 0;
    int nspec = 0;

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
            if (nqual) {
                strcpy(errmsg, "error: duplicate type qualifier");
                return ERROR;
            }
            nqual++;
        } else if (typemod() == 1) {
            if (nmod) {
                strcpy(errmsg, "error: duplicate type specifier");
                return ERROR;
            }
            nmod++;
        } else if (typemod() == -1) {
            if (nsmod) {
                strcpy(errmsg, "error: duplicate type specifier");
                return ERROR;
            }
            nsmod++;
        } else if (typespec() == TRUE) {
            if (nspec) {
                strcpy(errmsg, "error: duplicate type specifier");
                return ERROR;
            } else if (nmod && strcmp(token, "void") == 0) {
                strcpy(errmsg,
                       "error:  void cannot be used with type modifiers");
                return ERROR;
            }
            nspec++;
        }
        strcat(dt, token);

    } while (!nspec && (gettoken() == NAME));

    return 0;
}

/* dirdcl: parse a direct declarator */
int dirdcl(char *out, char *name) {

    int type = 0;

    if (tokentype == '(') {
        if (dcl(out, name) == ERROR)
            return ERROR;
        if (tokentype != ')') {
            strcpy(errmsg, "error: missing )");
            return ERROR;
        }
    } else if (tokentype == ')') {
        gettoken();
    } else if (tokentype == NAME) {
        strcat(name, token);
    } else {
        strcpy(errmsg, "error: expected name or (dcl)");
        return ERROR;
    }

    while ((type = gettoken()) == PARENS || type == BRACKETS || type == '(') {

        if (type == PARENS) {
            strcat(out, " function taking no arguments returning");
        } else if (type == '(' || type == ',') {
            strcat(out, " function accepting parameter");

            char argout[MAXLEN];
            char argdt[MAXTOKEN];
            char argname[MAXTOKEN];

            argdt[0] = '\0';
            argout[0] = '\0';
            argname[0] = '\0';

            /* advance another token to skip the opening parentheses */
            gettoken();

            if (pdt(argdt) == ERROR)
                return ERROR;

            if (dcl(argout, argname) == ERROR)
                return ERROR;

            strcat(out, " ");
            strcat(out, argname);
            strcat(out, ":");
            strcat(out, argout);
            strcat(out, " ");
            strcat(out, argdt);
            strcat(out, " returning");

            /* advance another token to skip the closing parentheses */
            if (tokentype == ')')
                gettoken();
            break;
        } else if (type == BRACKETS) {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }

    return OK;
}

/* dcl: parse a declarator */
int dcl(char *out, char *name) {
    int ns;
    for (ns = 0; gettoken() == '*';) /* count *'s */
        ns++;

    if (dirdcl(out, name) == ERROR)
        return ERROR;

    while (ns-- > 0)
        strcat(out, " pointer to");

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

void toend() {
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
        char out[1000];
        char dt[MAXTOKEN];
        char n[MAXTOKEN];

        store[0] = '\0';
        dt[0] = '\0';
        out[0] = '\0';
        n[0] = '\0';

        /* parse the data type */
        if (pdt(dt) == ERROR)
            return ERROR;

        /* then proceed to the rest of the line */
        if (dcl(out, n) == ERROR) {
            printf("%s: line %d column %lu\n", errmsg, line,
                   col - strlen(token) + 1);
            toend();
            continue;
        };

        if (tokentype != '\n') {
            printf("syntax error: line %d; column %d: %c\n", line, col,
                   tokentype);
            toend();
            continue;
        }

        /* reset column counter */
        col = 1;

        printf("%s%s:%s %s\n", store, n, out, dt);
    }
    return 0;
}
