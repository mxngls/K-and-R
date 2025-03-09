#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 100

/*
 * " [...] scanf stops when it exhausts its format string, or when some
 * input fails to match the control specification. It returns as its
 * value the number of successfully matched and assigned input items.
 * This can be used to decide how many items were found. On the end of
 * file, EOF is returned; note that this is different from 0, which
 * means that the next input character does not match the first
 * specification in the format string. The next call to scanf resumes
 * searching immediately after the last character already converted.":
 *
 * while (getline(line, sizeof(line)) > 0) {
 *     if (sscanf(line, "%d %s %d", &day, monthname, &year) == 3)
 *         printf("valid: %s\n", line);
 *     else if (sscanf(line, "%d/%d/%d", &month, &day, &year) == 3)
 *         printf("valid: %s\n", line);
 *     else
 *         printf("invalid: %s\n", line);
 * }
 *
 * Example: "1957/04/21" -> "%d/%s/%d" (int, int, int)
 */

/* minsscanf: minimal sscanf with variable argument list */
int minsscanf(char *line, char *fmt, ...) {
    va_list ap; /* argument pointer */

    int argn = 0;
    char *linep = line;
    char *p;

    va_start(ap, fmt); /* make ap point to 1st unnamed arg */

    argn = 0;
    for (p = fmt; *p; p++) {
        if (isspace(*p)) {
            while (isspace(*linep++));
            continue;
        }

        if (*p != '%') {
            if (*p != *linep) return 0;
            linep++;
        } else {
            p++;

            switch (*p) {
            case 'd': {
                if (!isdigit(*linep) && *linep != '+' && *linep != '-')
                    return 0;

                /* get current argument pointer and advance to next */
                int *dval = va_arg(ap, int *);
                char *endp;

                /* parse value for current element */
                *dval = (int)strtol(linep, &endp, 10);

                /* if no conversion were performed return */
                if (endp == linep) return 0;

                linep = endp;
                argn++;
                break;
            }
            case 's': {
                /* get current argument pointer and advance to next */
                char *sval = va_arg(ap, char *);

                /* read until whitespace or end of linep */
                int i = 0;
                while (*linep && !isspace(*linep)) sval[i++] = *linep++;
                sval[i] = '\0';

                if (i) argn++;
                break;
            }
            default:
                return 0;
            }
        }
    }

    va_end(ap); /* clean up */

    return argn;
}

int _getline(char line[], int lim) {
    char c;
    int i;
    for (i = 0; i < lim && (c = (char)getchar()) != EOF && c != '\n'; i++)
        line[i] = c;
    if (c == '\n') { line[i++] = c; }
    line[i] = '\0';
    return i;
}

/*
 * Exercise 6-4. Write a private version of scanf analogous to minprintf
 * from the previous section.
 */
int main() {
    char *line = malloc(MAXLINE);

    int day;
    int month;
    int year;
    char *monthname = malloc(10);

    while (_getline(line, MAXLINE) > 0) {
        if (minsscanf(line, "%d %s %d", &day, monthname, &year) == 3)
            printf("valid: %s\n", line);
        else if (minsscanf(line, "%d/%d/%d", &month, &day, &year) == 3)
            printf("valid: %s\n", line);
        else
            printf("invalid: %s\n", line);
    }

    free(line);
    free(monthname);

    return 0;
}
