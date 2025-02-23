#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000

/*
 * Exercise 7-6. Write a program to compare two files, printing the first line
 * where they differ.
 */
int main(int argc, char *argv[]) {
    FILE *fp1;
    FILE *fp2;
    char *prog = argv[0];
    int nl = 1;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", prog);
        return 1;
    }
    if (argc > 3) {
        fprintf(stderr, "Only two files can be compared at a time\n");
        return 1;
    }

    if ((fp1 = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open %s\n", argv[1]);
        return 1;
    }

    if ((fp2 = fopen(argv[2], "r")) == NULL) {
        fprintf(stderr, "Can't open %s\n", argv[2]);
        fclose(fp1);
        return 1;
    }

    int c1, c2;
    char line1[MAXLINE], line2[MAXLINE];
    while ((c1 = getc(fp1)) != EOF && (c2 = getc(fp2)) != EOF) {
        if (c1 != c2) {
            ungetc(c1, fp1);
            ungetc(c2, fp2);

            if (fgets(line1, MAXLINE, fp1) && fgets(line2, MAXLINE, fp2)) {
                printf("Files differ at line %d:\n", nl);
                printf(">>> %s", line1);
                printf(">>> %s", line2);
            }
            break;
        }
        if (c1 == '\n') nl++;
    }

    if (c1 == EOF && c2 != EOF)
        printf("EOF on %s\n", argv[1]);
    else if (c1 != EOF && c2 == EOF)
        printf("EOF on %s\n", argv[2]);

    fclose(fp1);
    fclose(fp2);

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    return 0;
}
