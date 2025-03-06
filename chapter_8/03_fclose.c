#include <stdlib.h>

#undef NULL
#define NULL     0
#define EOF      (-1)
#define BUFSIZ   1024
#define OPEN_MAX 20 /* max #files open at once */

typedef struct _iobuf {
        int   cnt;  /* characters left */
        char *ptr;  /* next character position */
        char *base; /* location of buffer */
        int   flag; /* mode of file access */
        int   fd;   /* file descriptor */
} _FILE;

enum _flags {
        _READ  = 01,  /* file open for reading */
        _WRITE = 02,  /* file open for writing */
        _UNBUF = 04,  /* file is unbuffered */
        _EOF   = 010, /* EOF has occurred on this file */
        _ERR   = 020  /* error occurred on this file */
};

_FILE _iob[OPEN_MAX] = {
    /* stdin, stdout, stderr */
    {0, (char *)0, (char *)0, _READ,           0},
    {0, (char *)0, (char *)0, _WRITE,          1},
    {0, (char *)0, (char *)0, _WRITE | _UNBUF, 2}
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
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = x : _flushbuf(x, p))
#define getchar()  getc(stdin)
#define putchar(x) putc((x), stdout)

#include <fcntl.h>
#include <unistd.h>

#define PERMS 0666 /* RW for owner, group, others */

_FILE *_fopen(char *name, char *mode) {
        int    fd;
        _FILE *fp;
        if (*mode != 'r' && *mode != 'w' && *mode != 'a')
                return NULL;
        for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
                if ((fp->flag & (_READ | _WRITE)) == 0)
                        break;     /* found free slot */
        if (fp >= _iob + OPEN_MAX) /* no free slots */
                return NULL;
        if (*mode == 'w')
                fd = creat(name, PERMS);
        else if (*mode == 'a') {
                if ((fd = open(name, O_WRONLY, 0)) == -1)
                        fd = creat(name, PERMS);
                lseek(fd, 0L, 2);
        } else
                fd = open(name, O_RDONLY, 0);
        if (fd == -1) /* couldn't access name */
                return NULL;
        fp->fd   = fd;
        fp->cnt  = 0;
        fp->base = NULL;
        fp->flag = (*mode == 'r') ? _READ : _WRITE;
        return fp;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(_FILE *fp) {
        unsigned int bufsize;
        if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
                return EOF;
        bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
        if (fp->base == NULL) /* no buffer yet */
                if ((fp->base = (char *)malloc(bufsize)) == NULL)
                        return EOF; /* can't get buffer */
        fp->ptr = fp->base;
        fp->cnt = (int)read(fp->fd, fp->ptr, bufsize);
        if (--fp->cnt < 0) {
                if (fp->cnt == -1)
                        fp->flag |= _EOF;

                else
                        fp->flag |= _ERR;
                fp->cnt = 0;
                return EOF;
        }
        return (unsigned char)*fp->ptr++;
}

/* _flushbuf: empty the contents of the input buffer */
int _flushbuf(int c, _FILE *fp) {
        int     bufsize = BUFSIZ;
        ssize_t nb      = fp->ptr - fp->base;

        if ((fp->flag & _WRITE) != _WRITE) {
                return EOF;
        }

        /* in case no buffer has been allocated yet */
        if (fp->base == NULL) {
                if ((fp->base = malloc(BUFSIZ)) == NULL) {
                        fp->flag |= _ERR;
                        return EOF;
                }
                fp->ptr    = fp->base;
                fp->cnt    = bufsize - 1;
                *fp->ptr++ = (char)c;

                return c;
        }

        if (nb > 0 && (write(fp->fd, fp->base, (size_t)nb)) != nb) {
                fp->flag |= _ERR;
                return EOF;
        }

        fp->ptr    = fp->base;
        fp->cnt    = bufsize - 1;
        *fp->ptr++ = (char)c;

        return c;
}

/*
 * Exercise 8-3. Design and write _flushbuf, fflush, and fclose.
 */
int main(void) {
        int  i;
        char c = (char)getc(_stdin);
        putc(c, _stdout);
        for (i = 0; i < BUFSIZ + 10; i++)
                putc('\n', _stdout);
        putc('\n', _stdout);
        return 0;
}
