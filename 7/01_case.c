#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define ERR -1
#define LOWER 1
#define UPPER 2

int argparse(int argc, char **argv) {
    if (argc == 1) {
        return 0;
    }

    if (argc > 2) {
        printf("error: too many arguments\n");
        return ERR;
    }
    if (strcmp("-l", *argv) == 0) {
        return LOWER;
    }
    if (strcmp("-u", *argv) == 0) {
        return UPPER;
    }

    printf("error: unrecognized argument\n");
    return ERR;
}
int main(int argc, char **argv) {
    int c;
    int tocase;

    if ((tocase = argparse(argc, ++argv)) == ERR)
        return 1;

    while ((c = getchar()) != EOF)
        tocase == LOWER   ? putchar(tolower(c))
        : tocase == UPPER ? putchar(toupper(c))
                          : putchar(c);

    return 0;
}
