#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char.c"

#define MAXWORD 20
#define MAXLINE 100
#define MAXCOL 100

char *noise[] = {"a", "an", "and", "the"};
extern int line;
extern int col;

int getword(char *, int);
char *_strdup(char *s);
int getnum(char *s, int *pnum);
int isdup(char **s, char *w);

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
        if (isalpha(word[0]))
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

    if (strlen(word) < 2) {
        *w = '\0';
        return c;
    }

    *w = '\0';
    return word[0];
}
