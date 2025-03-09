#include <stdio.h>
#include <stdlib.h>

#define _NULL    0
#define EOF      (-1)
#define BUFSIZ   1024
#define OPEN_MAX 20 /* max #files open at once */

typedef struct {
        unsigned int read;  /* file open for reading */
        unsigned int write; /* file open for writing */
        unsigned int unbuf; /* file is unbuffered */
        unsigned int eof;   /* EOF has occurred on this file */
        unsigned int err;   /* error occurred on this file */
} flags;

typedef struct _iobuf {
        int   cnt;   /* characters left */
        char *ptr;   /* next character position */
        char *base;  /* location of buffer */
        flags flags; /* mode of file access */
        int   fd;    /* file descriptor */
} _FILE;

_FILE _iob[OPEN_MAX] = {
    /* stdin, stdout, stderr */
    {
     0, NULL,
     NULL, {1, 0, 0, 0, 0},
     0, },
    {0, NULL, NULL, {0, 1, 0, 0, 0}, 1},
    {0, NULL, NULL, {0, 1, 1, 1, 0}, 2}
};

#define _stdin  (&_iob[0])
#define _stdout (&_iob[1])
#define _stderr (&_iob[2])

int _fillbuf(_FILE *);
int _flushbuf(int, _FILE *);

#define feof(p) ((p)->flag & _EOF) != 0)
#define ferror(p) ((p)->flag & _ERR) != 0)
#define fileno(p)  ((p)->fd)
#define getc(p)    (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : flushbuf((x), p))
#define getchar()  getc(stdin)
#define putcher(x) putc((x), stdout)

#include <fcntl.h>
#include <unistd.h>

#define PERMS 0666 /* RW for owner, group, others */

_FILE *_fopen(const char *name, const char *mode) {
        int    fd = 0;
        _FILE *fp = NULL;

        if (*mode != 'r' && *mode != 'w' && *mode != 'a')
                return NULL;

        /* obtain open slot */
        for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
                if (!(fp)->flags.read && !(fp)->flags.unbuf)
                        break;

        /* no open slots left */
        if (fp >= _iob + OPEN_MAX)
                return NULL;

        /* open or create file */
        if (*mode == 'w')
                fd = creat(name, PERMS);
        else if (*mode == 'a') {
                if ((fd = open(name, O_WRONLY, 0)) == -1)
                        fd = creat(name, PERMS);
                lseek(fd, 0L, SEEK_END);
        } else
                fd = open(name, O_RDONLY, 0);

        if (fd == -1)
                return NULL;

        fp->fd   = fd;
        fp->cnt  = 0;
        fp->base = NULL;

        if (*mode == 'r')
                fp->flags.read = 1;
        else
                fp->flags.write = 1;

        return fp;
}

int _fillbuf(_FILE *fp) {
        unsigned int bufsize = 0;

        /* check if we're allowed to access the input file */
        if (!fp->flags.read || fp->flags.eof || fp->flags.err)
                return EOF;

        /* get buffer size */
        bufsize = (fp->flags.unbuf) ? 1 : BUFSIZ;

        /* obtain buffer */
        if (fp->base == NULL)
                if ((fp->base = (char *)malloc(bufsize)) == NULL)
                        return EOF;

        /* read contents into buffer */
        fp->ptr = fp->base;
        fp->cnt = (int)read(fp->fd, fp->base, bufsize);

        /* set possible EOF flag */
        if (--fp->cnt < 0) {
                if (fp->cnt == -1)
                        fp->flags.eof = 1;
                else
                        fp->flags.err = 1;
                fp->cnt = 0;
        }

        return (unsigned char)*fp->ptr++;
}

/*
 * Exercise 8-2. Rewrite fopen and _fillbuf with fields instead of explicit bit operations.
 * Compare code size and execution speed.
 */
int main(int argc, char **argv) {
        _FILE *fp;
        char   c;

        if (argc == 1) {
                fprintf(stderr, "Usage: %s [file ...]\n", argv[0]);
                return 1;
        }

        while (--argc) {
                if ((fp = _fopen(*++argv, "r")) == NULL) {
                        return 1;
                } else
                        while ((c = (char)getc(fp)) != EOF)
                                putchar(c);
        }

        return 0;
}
