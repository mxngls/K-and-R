#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int process_stream(FILE *fp, const char *filename, const char *pattern) {
    char line[MAXLINE];
    int  nl    = 0;
    int  found = 0;

    while (fgets(line, MAXLINE, fp) != NULL) {
        nl++;
        if (strstr(line, pattern)) {
            if (filename) {
                printf("%s (%d):%5s%s", filename, nl, "", line);
            } else {
                printf("(%d):%5s%s", nl, "", line);
            }
            found++;
        }
    }

    return found;
}

/*
 * Exercise 7-7. Modify the pattern finding program of Chapter 5 to take its
 * input from a set of named files or, if no files are named as arguments, from
 * the standard input. Should the file name be printed when a matching line is
 * found? */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        return 1;
    }

    char *pattern = argv[argc - 1];
    int   found   = 0;

    if (argc == 2) {
        return process_stream(stdin, NULL, pattern);
    }

    int nf = 0;
    while (++nf < argc - 1) {
        FILE *fp = fopen(argv[nf], "r");

        if (fp == NULL) {
            printf("Error: Couldn't open file \"%s\"\n", argv[nf]);
            return 1;
        }

        found |= process_stream(fp, argv[nf], pattern);
        fclose(fp);

        if (nf != argc - 2)
            putchar('\n');
    }

    return found;
}
