#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define ALLOCSIZE 10000
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

/* return pointer to n characters */
char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else
        return 0;
}

void afree(char *p) /* free storage pointed to by p */
{
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p;
}

#define MAXLEN 1000 /* max length of any input line */

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
            printf("%s\n", lineptr[i]);
    } else {
        for (i = nlines - 1; i >= 0; i--)
            printf("%s\n", lineptr[i]);
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

void swap(void *v[], int i, int j) {
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* qsort: sort v[left]...v[right] into increasing order */
void _qsort(void *v[], int left, int right,
            int (*comp)(const void *, const void *)) {
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

#define MAXLINES 5000 /* max #lines to be sorted */

char *lineptr[MAXLINES]; /* pointers to text lines */

/*
 * Exercise 5-14. Modify the sort program to handle a -r flag, which
 * indicates sorting in reverse (decreasing) order. Be sure that -r
 * works with -n.
 */
int main(int argc, char *argv[]) {
    int nlines;      /* number of input lines read */
    int numeric = 0; /* 1 if numeric sort */
    int reverse = 0; /* 1 if reverse sort */

    if (argc > 1) {
        while (--argc) {
            if (strcmp(argv[argc], "-n") == 0)
                numeric = 1;
            if (strcmp(argv[argc], "-r") == 0)
                reverse = 1;
        }
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        _qsort(
            (void **)lineptr, 0, nlines - 1,
            (int (*)(const void *, const void *))(numeric ? numcmp : strcmp));
        writelines(lineptr, nlines, reverse);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}
