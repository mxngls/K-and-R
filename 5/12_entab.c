#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define TABSTOP 4   /* default tabstop */
#define MAXSTOP 800 /* maximium number of digits for a valid tabstop */
#define MAXCOL 1000 /* maximum column value */

int parse_inp(int argc, char **argv, int *startc, int *ts) {
    char s[4];
    char *arg;
    int i, flag = 0;

    if (argc > 3) {
        printf("Invalid number of arguments. Usage: entab [-m] [+n]\n");
        return 1;
    }

    while (--argc > 0) {
        arg = *++argv;

        i = 0;
        flag = arg[0];
        while (*++arg) {

            if (isdigit(*arg)) {
                s[i++] = *arg;
            } else {
                printf("Invalid character in argument: %c\n", *arg);
                return 1;
            }
        }

        s[i] = '\0';

        if (flag == '-')
            *startc = atoi(s);
        else if (flag == '+')
            *ts = atoi(s);
    }

    return 0;
}

/* Exercise 5-12. Extend entab and detab to accept the shorthand
 *
 * entab -m +n
 *
 * to mean tab stops every n columns, starting at column m. Choose
 * convenient (for the user) default behavior.
 */
int main(int argc, char **argv) {

    int startc; /* column from which spaces get replaced by tabs */
    int ts;     /* tabstops */

    int c;  /* current char */
    int s;  /* number of spaces */
    int cn; /* current column number */

    startc = 0;
    ts = TABSTOP;

    if (parse_inp(argc, argv, &startc, &ts))
        return 1;

    s = 0;
    cn = 0;

    while ((c = getchar()) != EOF) {

        if (cn++ < startc) {
            putchar(c);
            continue;
        }

        if (c == ' ') {
            if (++s % ts == 0) {
                putchar('\t');
                s = 0;
            }
        } else if (c == '\n') {
            putchar('\n');
            cn = 0;
        } else {
            if (s % ts != 0) {
                while (s-- > 0)
                    putchar(' ');
            }

            putchar(c);

            s = 0;
        }
    }
}
