#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char.c"

#define MAXWORD 10000
#define HASHSIZE 101
#define MAXCOM 10

/* skipc: skip comments */
void skipc() {
    char c;
    while ((c = (char)getch()) != EOF)
        if (c == '*' && (c = (char)getch()) == '/')
            return;
}

/*
 * getword: get next word or character from input (adapted from exercise 6-1)
 */
int getword(char *word, int lim) {
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    if ((c = getch()) == '#' ||
        (c == '\n' && (c = getch()) == '#')) { /* preprocessor
                                               control lines */
        *w++ = (char)c;
    } else {
        ungetch(c);
    }

    while ((c = getch()) != '\n' && isspace(c))
        ;

    if (c == '/' && (c = getch()) == '*') { /* comments */
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

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

struct nlist {          /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *name;         /* defined name */
    char *defn;         /* replacement text */
};

/* hash: form hash value for string s */
unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s) {
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL;       /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct nlist *)malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else                      /* already there */
        free((void *)np->defn); /*free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

/* uninstall: remove (name, defn) from hashtab */
int uninstall(char *name) {
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* not found */
        return 1;
    } else {
        hashval = hash(name);
        if (np->next)
            hashtab[hashval] = np->next; /* replace */
        else
            hashtab[hashval] = NULL; /* remove */
        free(np);
    }
    return 0;
}

void tabprint(struct nlist *l[], int size) {
    int tmpsize = size;
    while (size && l[tmpsize - size]) {
        /* clang-format off */
        printf(">>> name: %s, val: %s\n", 
               l[tmpsize - size]->name,
               l[tmpsize - size]->defn);
        /* clang-format on */
        size--;
    }
}

/*
 * Exercise 6-6. Implement a simple version of the #define processor (i.e., no
 * arguments) suitable for use with C programs, based on the routines of this
 * section. You may also find getch and ungetch helpful.
 *
 * - We limit our implementation to the first step the pre-processor
 *   takes, the collection of names to replaced.
 */
int main() {
    int tabsize = 0;
    char word[MAXWORD];
    while (getword(word, MAXWORD) != EOF) {
        if (strcmp(word, "#define") == 0) {
            char name[MAXWORD];
            char defn[MAXWORD];
            getword(name, MAXWORD);
            getword(defn, MAXWORD);
            printf(">>> name: %s, val: %s\n", name, defn);
            install(name, defn);
            tabsize++;
        }
    }
    tabprint(hashtab, tabsize);
    return 0;
}
