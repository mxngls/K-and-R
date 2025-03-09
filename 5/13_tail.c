#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXDIGITS 4
#define MAXLINES 10000
#define DEFAULT_NLINE 10

int parse_args(int argc, char **argv, int *ln) {
    int i, j;
    char c;
    char s[4];

    if (argc > 2) {
        printf("Invalid number of arguments\n");
        return 1;
    }

    while (--argc > 0) {
        if ((*++argv)[0] == '-') {
            i = 0;
            j = 0;

            while ((c = (*argv)[++j])) {
                if (isdigit(c))
                    s[i++] = c;
            }
            *ln = atoi(s);
        }
    }

    return 0;
}

/* getline:  read a line into s, return length (see page 29, modified) */
int _getline(char *s, int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        s[i] = (char)c;
    }

    if (c == '\n') {
        s[i] = (char)c;
        ++i;
    }

    s[i] = '\0';

    return i;
}

#define MAXLEN 1000

/* readlines:  read input lines (see page 109; modified) */
int readlines(char *lineptr[], int maxlines) {
    int len, nlines;
    char *p, line[MAXLEN];
    nlines = 0;

    while ((len = _getline(line, MAXLEN)) > 0) {

        /*
         * Just allocate and do not free as memory get's automatically
         * freed on program termination
         */
        if (nlines >= maxlines || ((p = malloc(len)) == NULL))
            return -1;
        else {
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }

    return nlines;
}

/* writelines:  write output lines (see page 109) */
void writelines(char *lineptr[], int nlines) {
    int i;

    for (i = 0; i < nlines && lineptr[i]; i++) {
        printf("%s\n", lineptr[i]);
    }
}

/*
 * Exercise 5-13. Write the program tail, which prints the last n lines
 * of its input. By default, n is set to 10, let us say, but it can be
 * changed by an optional argument so that
 *
 * tail -n
 *
 * prints the last n lines. The program should behave rationally no
 * matter how unreasonable the input or the value of n. Write the
 * program so it makes the best use of available storage; lines should
 * be stored as in the sorting program of Section 5.6, not in a
 * two-dimensional array of fixed size.
 */
int main(int argc, char **argv) {
    int ln = DEFAULT_NLINE;

    char *lineptr[MAXLINES];

    if (parse_args(argc, argv, &ln))
        return 1;

    readlines(lineptr, MAXLINES);
    writelines(lineptr, ln);

    return 0;
}
