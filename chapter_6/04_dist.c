#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char.c"

#define MAXWORD 20
#define MAXNODE 50
#define MAXLINE 100
#define MAXCOL 100

char *noise[] = {"a", "an", "and", "the", "s", NULL};
extern int line;
extern int col;

struct tnode {
    char *word;          /* pointer to the word contents */
    int oc;              /* counter of occurences */
    int line[MAXLINE];   /* line number */
    int col[MAXCOL];     /* column number */
    struct tnode *left;  /* left child */
    struct tnode *right; /* right child */
};
int osort = 0;
int nc = 0;

int parseargs(char **argv) {
    if ((*argv)[0] == '-' && (*argv)[1] == 'o')
        return osort = 1;
    return 0;
}

int pushn(struct tnode *p, struct tnode *arr[], int i) {
    if (p == NULL)
        return i;

    arr[i] = p;
    i++;
    if (p->left != NULL)
        i = pushn(p->left, arr, i);
    if (p->right != NULL)
        i = pushn(p->right, arr, i);
    return i;
}

struct tnode *insert(struct tnode *p, int oc) {
    if (p == NULL)
        return p;
    else if (oc <= p->oc)
        p->left = insert(p->left, oc);
    else
        p->right = insert(p->right, oc);
    return p;
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

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;
    if (p == NULL) {  /* a new word has arrived */
        p = talloc(); /* make a new node */
        p->word = _strdup(w);
        p->oc = 1;
        p->line[0] = line;
        p->col[0] = col;
        p->left = p->right = NULL;
        nc++;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->line[p->oc] = line;
        p->col[p->oc] = col;
        p->oc++;
    } else if (cond < 0) /* less than into left subtree */
        p->left = addtree(p->left, w);
    else /* greater than into right subtree */
        p->right = addtree(p->right, w);
    return p;
}

void nprint(struct tnode *p) {
    printf("%-15s", p->word);
    int tmpc = 0;
    do
        printf(" %d - %d:%d", p->oc, p->line[tmpc], p->col[tmpc]);
    while (++tmpc < p->oc);
    printf("\n");
}

void arrprint(struct tnode **arr) {
    int i = 0;
    while (arr[i]) {
        nprint(arr[i++]);
    }
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        nprint(p);
        treeprint(p->right);
    }
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

/* compare word occurrence */
int ocmp(struct tnode *f, struct tnode *s) {
    int foc = f->oc;
    int soc = s->oc;
    return (foc > soc) ? -1 : (foc < soc) ? 1 : 0;
}

void swap(void *v[], int i, int j) {
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/*
 * qsort: sort v[left]...v[right] into increasing order (copied from
 * chapter five)
 */
void _qsort(void *v[], int left, int right,
            int (*comp)(const void *, const void *)) {
    int i, last;
    void swap(void *v[], int, int);
    if (left >= right) /* do nothing if array contains */
        return;        /* fewer than two elements */
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (v[i] != NULL && v[left] != NULL)
            if ((*comp)(v[i], v[left]) < 0)
                swap(v, ++last, i);
    }
    swap(v, left, last);
    _qsort(v, left, last - 1, comp);
    _qsort(v, last + 1, right, comp);
}

/*
 * Exercise 6-3. Write a cross-referencer that prints a list of all words in a
 * document, and for each word, a list of the line numbers on which it occurs.
 * Remove noise words like "the", "and", and so on.
 */
int main(int argc, char **argv) {

    if (argc == 2 && !parseargs(++argv)) {
        printf("error: undefined argument provided.\n");
        return 1;
    } else if (argc > 2) {
        printf("error: too many arguments provided.\n");
        return 1;
    }

    struct tnode *root;
    char word[MAXWORD];
    root = NULL;
    while (nc < MAXNODE && getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0]) && !isnoise(word))
            root = addtree(root, word);
    }

    if (osort) {
        struct tnode **arr = malloc(nc * sizeof(struct tnode *));
        if (arr == NULL)
            return -1;
        pushn(root, arr, 0);
        _qsort((void **)arr, 0, nc, (int (*)(const void *, const void *))ocmp);
        arrprint(arr);
        free(arr);
    } else {
        treeprint(root);
    }
    return 0;
}
