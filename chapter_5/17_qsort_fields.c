#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 10000
#define MAXLINES 5000
#define MAXFIELD 100
#define MAXFIELDLEN 1000
#define VOIDCMP int (*)(void *, void *)

int nlines;                    /* number of input lines read */
int nfields = 0;               /* number of total fields */
int field = 0;                 /* current field index */
int reverse = 0;               /* sort in reverse */
int fields[MAXFIELD] = {0};    /* store fields */
int numeric[MAXFIELD] = {0};   /* stack of field indexes to sort numerically */
int fold[MAXFIELD] = {0};      /* stack of field indexes to fold */
int directory[MAXFIELD] = {0}; /* stack of field indexes to sort in directory
                                  order */

static char allocbuf[MAXLEN];
static char *allocp = allocbuf;

int (*compares)(const char *, const char *); /* comparison function for each
                                                sort field */

/* getline: read a line into s, return length */
int _getline(char s[], int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = (char)c;
    if (c == '\n') {
        s[i] = (char)c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

/* return pointer to n characters */
char *alloc(int n) {
    if (allocbuf + MAXLEN - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else
        return 0;
}

/* free storage pointed to by p */
void afree(char *p) {
    if (p >= allocbuf && p < allocbuf + MAXLEN)
#define ALLOCSIZE MAXLEN
        allocp = p;
}

/* readlines: read input lines */
int readlines(char *lineptr[], int maxlines) {
    int len, nlines;
    char *p, line[MAXLEN];
    nlines = 0;
    while ((len = _getline(line, MAXLEN)) > 0)
        if (nlines >= maxlines || (p = alloc(len)) == NULL)
            return -1;
        else {
            line[len - 1] = '\0'; /* delete newline */
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    return nlines;
}

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines, int reverse) {
    int i;
    if (!reverse) {
        for (i = 0; i < nlines; i++)
            printf("%3d:\t%s\n", i, lineptr[i]);
    } else {
        for (i = nlines - 1; i >= 0; i--)
            printf("%3d:\t%s\n", i, lineptr[i]);
    }
}

/* numcmp: compare s1 and s2 numerically */
int numcmp(const char *s1, const char *s2) {
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

void skipc(const char *s) {
    while (!(isalnum(*s) || isspace(*s)) && (*s++))
        ;
}

char foldchar(char c) { return c <= 'Z' && c >= 'A' ? c + 'a' - 'A' : c; }

/* foldcmp: compare s1 and s2 ignoring case differences */
int foldcmp(const char *s1, const char *s2) {
    char c1, c2;
    while (*s1 && *s2) {

        if (directory[field]) {
            skipc(s1);
            skipc(s2);
        }

        c1 = foldchar(*s1);
        c2 = foldchar(*s2);

        if (c1 != c2) {
            return c1 - c2;
        }

        s1++;
        s2++;
    }

    return c1 - c2;
}

/* custom strcmp */
int _strcmp(const char *s1, const char *s2) {
    char c1, c2;
    while (*s1 && *s2) {

        if (directory[field]) {
            skipc(s1);
            skipc(s2);
        }

        c1 = *s1;
        c2 = *s2;

        if (c1 != c2) {
            return c1 - c2;
        }

        s1++;
        s2++;
    }

    return c1 - c2;
}

/* swap: interchange v[i] and v[j] */
void swap(void *v[], int i, int j) {
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* split lines into fields */
void splitf(const char *s, char fields[MAXFIELD][MAXFIELDLEN]) {
    int i, fn, c;
    i = fn = c = 0;

    for (i = 0; s[i] && fn < MAXFIELD; i++) {
        if (s[i] == '\t' || s[i] == '\n') {
            fields[fn][c] = '\0';
            fn++;
            c = 0;
        } else if (c < MAXFIELDLEN - 1) {
            fields[fn][c++] = s[i];
        }
    }

    if (c > 0 && fn < MAXFIELD) {
        fields[fn][c] = '\0';
    }
}

/* qsort: sort v[left]...v[right] into increasing order */
void _qsort(void *v[], int left, int right, int (*comp)(void *, void *)) {
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right) /* do nothing if array contains */
        return;        /* fewer than two elements */

    swap(v, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++)
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);

    swap(v, left, last);
    _qsort(v, left, last - 1, comp);
    _qsort(v, last + 1, right, comp);
}

/* parse the number of the field the list of options belongs to */
int parsefn(char **s) {
    char field[MAXFIELD];
    int i = 0;

    do {
        if (!isdigit(**s))
            break;
        field[i++] = **s;
    } while (++*s && i < MAXFIELD);

    return atoi(field);
}

/* compare strings in order of provided fields */
int compare(char *s1, char *s2) {
    int i, rslt = 0;

    char tmpleft[MAXFIELD][MAXFIELDLEN] = {0};
    char tmpi[MAXFIELD][MAXFIELDLEN] = {0};

    splitf(s1, tmpi);
    splitf(s2, tmpleft);

    for (i = 0; i <= nfields && rslt == 0; i++) {
        compares = numeric[i] ? numcmp : fold[i] ? foldcmp : _strcmp;
        rslt = (compares)(tmpi[fields[i]], tmpleft[fields[i]]);
    }

    return rslt;
}

/* parse command line arguments */
int parseargs(int argc, char *argv[]) {
    while (--argc && *++argv) {
        if (strcmp(*argv, "-r") == 0) {
            reverse = 1;
        } else if (**argv == '-') {
            char *opt = ++*argv;
            field = parsefn(&opt) - 1;
            fields[nfields] = field;

            while (*opt) {
                char o = *opt++;

                switch (o) {
                case 'n':
                    numeric[nfields] = 1;
                    break;
                case 'f':
                    fold[nfields] = 1;
                    break;
                case 'd':
                    directory[nfields] = 1;
                    break;
                default:
                    printf("Invalid option supplied. Aborting.\n");
                    return 1;
                }

                if (directory[nfields] && numeric[nfields]) {
                    printf("Options '-dn' are incompatible. Aborting\n");
                    return -1;
                }
                if (fold[nfields] && numeric[nfields]) {
                    printf("Options '-nf' are incompatible. Aborting\n");
                    return -1;
                }
            }

            nfields++;

        } else {
            printf("Only options are allowed. Aborting.");
            return 1;
        }
    }

    return 0;
}

/*
 * Exercise 5-16. Add the -d ("directory order") option, which makes
 * comparisons only on letters, numbers and blanks. Make sure it works
 * in conjunction with -f.
 *
 * Similar solution to the one provided by John Donnellan, but
 * implemented differently (see
 * https://clc-wiki.net/wiki/K%26R2_solutions:Chapter_5:Exercise_17#Solution_by_codybartfast):
 *
 * Fields are tab delimited, one argument is provided for each field to
 * sort by:
 *
 * sort [-r] [-NN*M*]*
 *
 * Where NN* is the index of field to sort by and M* is any combination
 * of the modifiers n, f or d. For convinience we only allow sorting by
 * reverse after sorting by fields. Piority is given to fields in the
 * order that they are listed. E.g.:
 *
 * sort -10 -1n -5
 *
 * Will first sort by the Tenth field, then by the First field (numeric)
 * and then by the Fifth field.
 */
int main(int argc, char *argv[]) {
    if (parseargs(argc, argv))
        return 1;

    char *lineptr[MAXLINES];

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        _qsort((void **)lineptr, 0, nlines - 1, (VOIDCMP)compare);
        writelines(lineptr, nlines, reverse);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}
