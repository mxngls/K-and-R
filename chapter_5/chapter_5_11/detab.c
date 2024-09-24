#include <ctype.h>
#include <stdio.h>

#define TABSTOP 4   /* default tabstop */
#define MAXSTOP 800 /* maximium number of digits for a valid tabstop */

int parse_tabs(int argc, char **argv, int ts[], int *tn) {
    char c;
    int i;

    if (argc > 2) {
        printf("To many arguments. Usage: entab -t=[tabstops]\n");
        return 1;
    }

    if (**++argv != '-' || *(*argv + 1) != 't' || *(*argv + 2) != '=') {
        printf("Invalid argument format. Usage: -t=[tabstops]\n");
        return 1;
    }

    *argv += 3;

    i = 0;
    while ((c = *(*argv)++)) {
        if (i >= MAXSTOP) {
            printf("Max tabstop count exceeded. Exiting.\n");
            return 1;
        }

        if (c == ',') {
            *tn = i++;
        } else if (isdigit(c)) {
            ts[i] = (ts[i] * 10) + (c - '0');
        }
    }

    return 0;
}

int main(int argc, char **argv) {

    int tn;                /* number tabstops */
    int ts[MAXSTOP] = {0}; /* tabstops */

    int c;   /* current char */
    int ns;  /* number of spaces to put */
    int col; /* number of spaces */

    int i; /* index of current tabstop */
    int t; /* current tabstop */

    tn = 0;
    if (parse_tabs(argc, argv, ts, &tn))
        return 1;

    ns = 0;
    i = 0;
    col = 0;
    t = ts[i] ? ts[i] : TABSTOP;

    while ((c = getchar()) != EOF) {
        if (c == '\t') {
            ns = t - (col % t);
            t = i <= tn ? ts[i++] : TABSTOP;
            while (ns-- > 0) {
                putchar(' ');
                col++;
            }

        } else {
            putchar(c);

            if (c == '\n')
                col = 0;
            else
                col++;
        }
    }

    return 0;
}
