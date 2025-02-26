#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#define PERMS 0666

int  copy(int ifd, char *ipath, int ofd, char *opath);
void error(char *fmt, ...);

/*
 * Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write, open, and close
 * instead of their standard library equivalents. Perform experiments to determine the relative
 * speeds of the two versions.
 */
int main(int argc, char **argv) {
    int   fd;
    char *path;

    if (argc == 1) {
        fd   = 0;
        path = "<stdin>";
    } else if ((fd = open(path = argv[1], O_RDONLY)) == -1) {
        error("%s: can't open %s", argv[0], path);
    }
    copy(fd, path, 1, "<stdout>");

    if (fd != 0) {
        close(fd);
    }

    return 0;
}

/* error: print an error message and die */
void error(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);

    exit(1);
}

/* copy: copy a file */
int copy(int ifd, char *ipath, int ofd, char *opath) {
    static char buf[BUFSIZ] = {0};
    int         n;
    while ((n = (int)read(ifd, buf, BUFSIZ)) > 0) {
        if (write(ofd, buf, (size_t)n) != n) {
            error("Couldn't write to '%s'", opath);
        }
    }
    if (n == -1) {
        error("Couldn't read from '%s'", ipath);
    }
    return 0;
}
