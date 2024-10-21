#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char.c"

#define MAXWORD 10000
#define NKEYS (int)(sizeof keytab / sizeof(struct key))

/* clang-format off */
struct key {
    char *word;
    int count;
} keytab[] = {
    {"auto", 0},
	{"case", 0},
	{"char", 0},
	{"const", 0},
	{"continue", 0},
	{"default", 0},
	{"do", 0},
	{"double", 0},
	{"else", 0},
	{"enum", 0},
	{"extern", 0},
	{"float", 0},
	{"goto", 0},
	{"if", 0},
	{"int", 0},
	{"long", 0},
	{"register", 0},
	{"return", 0},
	{"short", 0},
	{"signed", 0},
	{"sizeof", 0},
	{"static", 0},
	{"struct", 0},
	{"switch", 0},
	{"typeof", 0},
	{"union", 0},
	{"unsigned", 0},
	{"void", 0},
	{"volatile", 0},
	{"while", 0},
};
/* clang-format on */

int getword(char *, int);
int binsearch(char *, struct key *, int);

/*
 * Exercise 6-1. Our version of getword does not properly handle underscores,
 * string constants, comments, or preprocessor control lines. Write a better
 * version.
 *
 * Some restrictions applied:
 *
 * - We make our lifes easier as we only proclaim to give correct numbers
 *   for C source files that compile succesfully.
 *
 * - We skip all preprocessor control lines and comments
 *
 */
int main() {
    int n;
    char word[MAXWORD];
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0]))
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
    }
    for (n = 0; n < NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
    return 0;
}

/* skipc: skip comments */
void skipc() {
    char c;
    while ((c = (char)getch()) != EOF)
        if (c == '*' && (c = (char)getch()) == '/')
            return;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim) {
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;

    if (c == '\n' && (c = getch()) == '#') { /* preprocessor control lines */
        while ((c = getch()) != '\n')
            ;
        getch();
    } else if (c == '/' && (c = getch()) == '*') { /* comments */
        skipc();
    } else if (c == '\'') { /* string literals */
        while ((c = getch()) != '\'')
            ;
    } else if (c == '"') { /* string constants */
        while ((c = getch()) != '"')
            ;
    } else if (c != EOF)
        *w++ = (char)c;

    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++)
        if (!isalnum(*w = (char)getch()) && *w != '_') {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low, high, mid;
    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (tab[mid].word == NULL || (cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}
