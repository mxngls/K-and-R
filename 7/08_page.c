#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#define MAXLINE 1000

typedef struct ps {
    const unsigned short cols;
    const unsigned short rows;
} pagesize;

void print_page(const char *filename, const int page, const int pad, const int end) {
    int p;
    for (p = 0; p < pad; p++)
        fputc(' ', stdout);
    printf("(%s) %s PAGE: %d\n\n", filename, end ? "END" : "START", page);
}

int print_file(const char *filename, const pagesize ps) {
    FILE *fp            = NULL;
    char  line[MAXLINE] = {0};
    int   page          = 0;
    int   row           = 0;
    int   pad           = (ps.cols / 2) - 20;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Could not open file \"%s\"\n", filename);
        return 1;
    }

    print_page(filename, ++page, pad, 0);
    while (fgets(line, MAXLINE, fp) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] != '\n') {
            int c;
            while ((c = fgetc(fp)) != EOF && c != '\n')
                ;
        }

        line[ps.cols] = '\0';
        fputs(line, stdout);

        if (row++ > ps.rows) {
            print_page(filename, page, pad, 1);
            print_page(filename, page, pad, 0);
            page++;
            row = 0;
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    struct winsize ws = {0};
    if (ioctl(fileno(stdin), TIOCGWINSZ, &ws) == -1) {
        fprintf(stderr, "Failed to get terminal size: %s\n", strerror(errno));
        return -1;
    }

    pagesize ps = {
        .rows = ws.ws_row - 6,
        .cols = ws.ws_col >= MAXLINE ? MAXLINE - 1 : ws.ws_col - 1,
    };

    int argn = 1;
    while (argn < argc) {
        if (print_file(argv[argn], ps)) {
            exit(1);
        }
        argn++;
    }

    return 0;
}
