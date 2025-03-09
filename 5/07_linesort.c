#include <stdio.h>
#include <string.h>

#define MAXLINES 5000 /* max #lines to be sorted */
#define MAXLINE 1000  /* max chars per line */

char *lineptr[MAXLINES]; /* pointers to text lines */

int readlines(char *ineptr[], char lines[], int nlines);
void writelines(char *lineptr[], int nlines);

void qsort(char *lnieptr[], int left, int right);

/*
 * Exercise 5-7. Rewrite readlines to store lines in an array supplied by main,
 * rather than calling alloc to maintain storage. How much faster is the
 * program?
 */
int main() {
    int nlines;
    char lines[MAXLINES * MAXLINE];

    if ((nlines = readlines(lineptr, lines, MAXLINES)) >= 0) {
        qsort(lineptr, 0, nlines - 1);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("errorL input too big to sort\n");
        return 1;
    }
}

#define MAXLEN 1000 /* max length of any input line */

/* getline: read a line into s, return length */
int _getline(char *s, int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

/* readlines:   read input lines*/
int readlines(char *lineptr[], char lines[], int maxlines) {
    int len, nlines;
    char line[MAXLEN];
    char *lineend = lines + MAXLINES + MAXLINE;

    nlines = 0;
    while ((len = _getline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || lines + len >= lineend)
            return -1;
        else {
            line[len - 1] = '\0'; /* delete newline */
            strcpy(lines, line);
            lineptr[nlines++] = lines;
            lines += len;
        }
    }
    return nlines;
}

/* writelines:  write output lines */
void writelines(char *lineptr[], int nlines) {
    while (nlines-- > 0) {
        printf("%s\n", *lineptr++);
    }
}

/* qsort: sort v[left]...v[right] into increasing order */
void qsort(char *v[], int left, int right) {
    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right) /* do nothing if array contains */
        return;        /* fewer than two elemeents */

    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);

    swap(v, left, last);

    qsort(v, left, last - 1);
    qsort(v, last + 1, right);
}

/* swap: interchange v[i] and v[j] */
void swap(char *v[], int i, int j) {
    char *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
