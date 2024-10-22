#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char.c"

#define MAXWORD 100
#define MAXWORDS 100
#define MAXDIGIT 10
#define DEFAULT_DIGIT 6

struct tnode *talloc(void);
struct tnode *addtree(struct tnode *, char *);

struct tnode {            /* the tree node: */
    char *word[MAXWORDS]; /* points to the text */
    int nword;            /* number of words stored */
    struct tnode *left;   /* left child */
    struct tnode *right;  /* right child */
};

void treeprint(struct tnode *);
int getword(char *, int);
char *_strdup(char *s);
int getnum(char *s, int *pnum);
int isdup(char **s, char *w);

int num;

/*
 * Exercise 6-2. Write a program that reads a C program and prints in
 * alphabetical order each group of variable names that are identical in
 * the first 6 characters, but different somewhere thereafter. Don't
 * count words within strings and comments. Make 6 a parameter that can
 * be set from the command line.
 *
 * NOTE:
 *
 * The term "variable" is not strictly defined as per the standard so we
 * are a bit lazy and do not restrict our matching to any particular
 * definition of a word that is more restrective than the initial
 * implementation of `getword` suggests. For a more in-depth discussion
 * see "https://stackoverflow.com/questions/52991889/what-is-the-definition-of-variable".
 */
int main(int argc, char **argv) {

    num = 0;
    if (argc > 1) {
        if (argc != 3 || strcmp(*++argv, "-d")) {
            printf("error: expected command line option [-d]\n");
            return 1;
        }

        int *pnum = &num;
        if (getnum(*++argv, pnum) != 0)
            return 1;
    }

    struct tnode *root;
    char word[MAXWORD];
    root = NULL;
    while (getword(word, MAXWORD) != EOF) {
        if ((int)strlen(word) < num)
            continue;

        if (isalpha(word[0]))
            root = addtree(root, word);
    }

    treeprint(root);

    return 0;
}

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;
    if (p == NULL) {  /* a new word has arrived */
        p = talloc(); /* make a new node */
        p->word[p->nword] = _strdup(w);
        p->left = p->right = NULL;
    } else if ((cond = strncmp(w, p->word[p->nword], num)) == 0) {
        if (isdup(p->word, w))
            return p;
        p->nword++;
        p->word[p->nword] = _strdup(w);
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
        int i = 0;
        while (p->word[i])
            printf("%s\n", p->word[i++]);
        printf("\n");
        treeprint(p->right);
    }
}

/* talloc: make a tnode */
struct tnode *talloc(void) {
    return (struct tnode *)malloc(sizeof(struct tnode));
}

/* make a duplicate of s */
char *_strdup(char *s) {
    char *p;
    p = (char *)malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
}

/* skipc: skip comments */
void skipc() {
    char c;
    while ((c = (char)getch()) != EOF)
        if (c == '*' && (c = (char)getch()) == '/')
            return;
}

/*
 * getword: get next word or character from input (taken from our
 * solution to problem 6-1)
 */
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

/* parse a provided number */
int getnum(char *s, int *pnum) {
    char c, numbuf[MAXDIGIT];
    int i = 0;
    while ((c = s[i])) {
        if (i >= MAXDIGIT) {
            printf("error: only arguments up to %d digits are allowed\n",
                   MAXDIGIT);
            return 1;
        }
        if (!isdigit(c)) {
            printf("error: only digits are allowed as valid arguments\n");
            return 1;
        }
        numbuf[i++] = c;
        *pnum = atoi(numbuf);
    }
    return 0;
}

/* check if the _exact_ word is already present */
int isdup(char **s, char *w) {
    while (*s) {
        if (strcmp(*s, w) == 0)
            return 1;
        s++;
    }
    return 0;
}
