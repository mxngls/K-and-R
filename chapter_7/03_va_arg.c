#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...) {
    va_list ap; /* points to each unnamed arg in turn */
    char *p, *sval;
    int ival;
    double dval;
    va_start(ap, fmt); /* make ap point to 1st unnamed arg */
    for (p = fmt; *p; p++) {
        if (*p == '\0') {
            break;
        } else if (*p != '%') {
            putchar(*p);
            continue;
        }
        switch (*++p) {
        case 'd':
        case 'i':
            ival = va_arg(ap, int);
            printf("%d", ival);
            break;
        case 'o':
            ival = va_arg(ap, int);
            printf("%o", ival);
            break;
        case 'x':
        case 'X':
            ival = va_arg(ap, int);
            printf("%x", ival);
            break;
        case 'u':
            ival = va_arg(ap, unsigned int);
            printf("%u", ival);
            break;
        case 'c':
            ival = va_arg(ap, int);
            printf("%c", ival);
            break;
        case 's':
            sval = va_arg(ap, char *);
            printf("%s", sval);
            break;
        case 'f':
            dval = va_arg(ap, double);
            printf("%f", dval);
            break;
        default:
            putchar(*p);
            break;
        }
    }
    va_end(ap); /* clean up when done */
}

/*
 * Exercise 7-3. Revise minprintf to handle more of the other facilities
 * of printf.
 */
int main() {
    char c;
    char inp[MAXLINE] = "";

    int i;

    i = 0;
    while ((c = (char)getchar()) != EOF) {
        if (c == '\n') {
            inp[i] = '\0';

            minprintf("Input: %d\n", atoi(inp));
            minprintf("Input: %i\n", atoi(inp));
            minprintf("Input: %o\n", (unsigned)atoi(inp));
            minprintf("Input: 0x%x\n", (unsigned)atoi(inp));
            minprintf("Input: %u\n", (unsigned)atoi(inp));
            minprintf("Input: %c\n", inp[0]);
            minprintf("Input: %s\n", inp);
            minprintf("Input: %f\n", atof(inp));
            printf("\n");

            inp[0] = '\0';
            i = 0;
        } else if (i < MAXLINE - 1) {
            inp[i++] = c;
        }
    }

    return 0;
}
