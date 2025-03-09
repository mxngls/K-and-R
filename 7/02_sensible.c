#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXWORD 1000
#define BUFSIZE 100
#define LINEBREAK 72
#define HEXLEN 7

int sanitize();
int breakline();

/* global state */
int line;
int col;
int inword;
int buf[BUFSIZE];
int ncpbuf;

/* get a (possibly pushed-back) character */
int getch() {
    if (ncpbuf)
        return buf[--ncpbuf];

    col++;
    return getchar();
}

/* push character back on input */
void ungetch(int c) {
    col--;
    buf[ncpbuf++] = c;
}

/*
 * Exercise 7-2. Write a program that will print arbitrary input in a sensible
 * way. As a minimum, it should print non-graphic characters in octal or
 * hexadecimal according to local custom, and break long text lines.
 */
int main() {
    int c;
    int i;
    char word[MAXWORD];

    line = 0;
    col = 0;

    i = 0;
    word[i] = '\0';

    while ((c = getch()) != EOF) {
        /* printf("c: %c\n", c); */
        if (c == ' ' || c == '\n') {
            if (col > LINEBREAK) {
                printf("\n");
                col = (int)strlen(word);
            }

            printf("%s", word);
            printf("%c", c);

            /* line and word reset */
            if (c == '\n')
                col = 0;
            i = 0;
            word[0] = '\0';
        } else if (isprint(c)) {
            word[i++] = (char)c;
            word[i] = '\0';
        } else {
            char hex[7]; /* padding (2):    '<' and '>' */
                         /* prefix (2):     "0x"        */
                         /* hex digits (2): "##"        */
                         /* null byte (1):  '\0'        */
            sprintf(hex, "<0x%X>", c);
            strcat(word, hex);
            i += HEXLEN - 1;
            col += HEXLEN - 1;
            word[i] = '\0';
        }
    }

    return 0;
}
