#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char.c"

#define MAXTOKEN 100
#define MAXLEN 100

enum { NAME, PARENS, BRACKETS };
enum { OK, ERROR };
enum { FALSE, TRUE };

int dcl(int);
int dirdcl(int);
int gettoken(void);

int tokentype;              /* type of last token */
char token[MAXTOKEN];       /* last token string */
char datatype[MAXTOKEN];    /* data type = char, int, etc. */
char name[MAXTOKEN];        /* variable name */
char argdatatype[MAXTOKEN]; /* data type = char, int, etc. */
char argname[MAXTOKEN];     /* argument name */
char store[MAXTOKEN];       /* storage class specifier */
char out[1000];

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

int parsedt(int args) {

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
        if (args == TRUE) {
            argname[0] = '\0';
            argdatatype[0] = '\0';
            strcat(argdatatype, token);
        } else {
            strcat(datatype, token);
            strcat(datatype, " ");
        }

    } while (!nspec && (gettoken() == NAME));

    return 0;
}

/* dcl: parse a declarator */
int dcl(int args) {
    parsedt(args);

    int ns;
    for (ns = 0; gettoken() == '*';) /* count *'s */
        ns++;

    if (dirdcl(args) == ERROR)
        return ERROR;

    while (ns-- > 0)
        strcat(out, " pointer to");

    return OK;
}

/* dirdcl: parse a direct declarator */
int dirdcl(int args) {

    int type = 0;


    if (tokentype == '(') {
        dcl(FALSE);
        if (tokentype != ')') {
            strcpy(errmsg, "error: missing )");
            return ERROR;
        }
    } else if (tokentype == NAME) {
        if (args == TRUE) {
            strcpy(argname, " ");
            strcat(argname, token);

            strcat(out, argname);
            strcat(out, ": ");
            strcat(out, argdatatype);

            argname[0] = '\0';
            argdatatype[0] = '\0';
        } else {
            strcpy(name, token);
        }
    } else {
        strcpy(errmsg, "error: expected name or (dcl)");
        return ERROR;
    }


    while ((type = gettoken()) == PARENS || type == BRACKETS || type == '(' ||
           type == ')' || type == ',') {

        if (type == PARENS) {
            strcat(out, " function taking no arguments returning");
        } else if (type == '(') {
            strcat(out, " function accepting parameter");
            dcl(TRUE);
        } else if (type == ')') {
            if (args)
                strcat(out, " returning");
            break;
        } else if (type == BRACKETS) {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        } else if (type == ',') {
            strcat(out, " and parameter");
            dcl(TRUE);
            break;
        }
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
        if (dcl(FALSE) == ERROR) {
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
