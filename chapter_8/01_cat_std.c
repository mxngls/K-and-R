#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
 * Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write, open, and close
 * instead of their standard library equivalents. Perform experiments to determine the relative
 * speeds of the two versions.
 */
int main(int argc, char **argv) {
    int   fn = 1;
    FILE *fp = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage %s file1 [file2 ...]\n", argv[0]);
        return 1;
    }

    while (--argc) {

        char   buf[BUFSIZ] = {0};
        size_t nb;

        if ((fp = fopen(argv[fn], "r")) == NULL) {
            fprintf(stderr, "Error opening \"%s\": %s\n", argv[fn], strerror(errno));
            return 1;
        }
        while ((nb = fread(buf, sizeof buf[0], BUFSIZ, fp))) {
            if ((nb != fwrite(buf, sizeof buf[0], nb, stdout))) {
                fprintf(stderr, "Error writing to stdout: %s\n", strerror(errno));
            }
        }

        if (ferror(fp)) {
            fprintf(stderr, "Error reading from %s: %s\n", argv[fn], strerror(errno));
        }

        fclose(fp);
        fn++;
    }

    return 0;
}
