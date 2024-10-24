#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char.c"

#define MAXWORD 20
#define MAXLINE 100
#define MAXCOL 100

char *noise[] = {"a", "an", "and", "the", "s", NULL};
extern int line;
extern int col;

char *_strdup(char *);
int getword(char *, int);
int wcount(int[]);
int isnoise(char *);

struct tnode {
    char *word;          /* pointer to the word contents */
    int wc;              /* counter of occurences */
    int line[MAXLINE];   /* line number */
    int col[MAXCOL];     /* column number */
    struct tnode *left;  /* left child */
    struct tnode *right; /* right child */
};

struct tnode *talloc(void);
struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);

/*
 * Exercise 6-3. Write a cross-referencer that prints a list of all words in a
 * document, and for each word, a list of the line numbers on which it occurs.
 * Remove noise words like "the", "and", and so on.
 */
int main() {

    struct tnode *root;
    char word[MAXWORD];
    root = NULL;
    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]) && !isnoise(word))
            root = addtree(root, word);

    treeprint(root);

    return 0;
}

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;
    if (p == NULL) {  /* a new word has arrived */
        p = talloc(); /* make a new node */
        p->word = _strdup(w);
        p->wc = 1;
        p->line[0] = line;
        p->col[0] = col;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->line[p->wc] = line;
        p->col[p->wc] = col;
        p->wc++;
    } else if (cond < 0) /* less than into left subtree */
        p->left = addtree(p->left, w);
    else /* greater than into right subtree */
        p->right = addtree(p->right, w);
    return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%-15s", p->word);
        int tmpc = 0;
        do
            printf(" %d:%d", p->line[tmpc], p->col[tmpc]);
        while (++tmpc < p->wc);
        printf("\n");
        treeprint(p->right);
    }
}

/* make a duplicate of s */
char *_strdup(char *s) {
    char *p;
    p = (char *)malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
}

/* talloc: make a tnode */
struct tnode *talloc(void) {
    return (struct tnode *)malloc(sizeof(struct tnode));
}

/* count word occurences (i.e "references") */
int wcount(int r[]) {
    int n = 0;
    while (*r++ > 0)
        n++;
    return n;
}

/* check if a word is noise */
int isnoise(char *w) {
    int i = 0;

    char *tmp_w = malloc(strlen(w) + 1);
    char *s = tmp_w;

    while (*w) {
        *tmp_w = (char)tolower(*w);
        tmp_w++;
        w++;
    }
    *tmp_w = '\0';
    tmp_w = s;

    while (noise[i])
        if (strcmp(tmp_w, noise[i++]) == 0) {
            free(s);
            return 1;
        }

    free(s);
    return 0;
}

/* simplified version of the standard getword function */
int getword(char *word, int lim) {
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;

    if (c != EOF)
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
