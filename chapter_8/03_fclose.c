#include <errno.h>
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

#define feof(p) (((p)->flag & _EOF) != 0))
#define ferror(p) (((p)->flag & _ERR) != 0))
#define fileno(p)  ((p)->fd)
#define getc(p)    (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = x : _flushbuf(x, p))
#define getchar()  getc(_stdin)
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
 * From the appendix:
 * On an output stream, fflush causes any buffered but unwritten data to be written; on
 * an input stream, the effect is undefined. It returns EOF for a write error, and zero
 * otherwise. fflush(NULL) flushes all output streams.
 */
int fflush(_FILE *stream) {
        ssize_t bufsize;
        ssize_t nb;

        if (stream == NULL) {
                /*
                 * NOTE: Would practically iterate over all open files and flush their respecitve
                 * buffers one by one
                 */
                return 0;
        }

        bufsize = (stream->ptr - stream->base) + stream->cnt;
        nb      = stream->ptr - stream->base;

        if ((stream->flag & _WRITE) != _WRITE) {
                /*
                 * No write access, so we treat the stream as an input stream and simply do nothing
                 */
                return 0;
        }

        if (nb > 0 && (write(stream->fd, stream->base, (size_t)nb)) != nb) {
                stream->flag |= _ERR;
                return EOF;
        }

        stream->ptr = stream->base;
        stream->cnt = stream->base == NULL ? 0 : (int)bufsize;

        return 0;
}

/*
 * From the appendix:
 * fclose flushes any unwritten data for stream, discards any unread buffered input,
 * frees any automatically allocated buffer, then closes the stream. It returns EOF if any
 * errors occurred, and zero otherwise.
 */
int fclose(_FILE *stream) {
        if (stream == NULL) {
                return EOF;
        }

        /* flush the stream's buffer if it is an output buffer */
        if ((stream->flag & _WRITE) == _WRITE) {
                if (fflush(stream) == EOF) {
                        return EOF;
                }
        }

        /* free previously allocated buffer */
        if (stream->base != NULL) {
                free(stream->base);
                stream->base = NULL;
        }

        /* close file descriptor */
        if (close(stream->fd) == -1) {
                errno = -1;
                return EOF;
        }

        return 0;
}

/*
 * Exercise 8-3. Design and write _flushbuf, fflush, and fclose.
 */
int main(void) {
        int    i;
        char  *tmp;
        _FILE *tmpf;

        /* test _fluhbuf */
        for (i = 0; i < BUFSIZ; i++) {
                /* cycle through alphabet */
                putc('a' + (i % 26), _stdout);

                /* add newline for readability */
                if ((i + 1) % 26 == 0) {
                        putc('\n', _stdout);
                }
        }

        putc('\n', _stdout);
        putc('\n', _stdout);

        /* test fflush */
        for (i = 0; i < 10; i++) {
                putc('X', _stdout);
        }
        if (fflush(_stdout) == EOF) {
                return 1;
        }

        putc('\n', _stdout);
        putc('\n', _stdout);

        /* test fclose */
        tmp  = "tmp_03_fclose.txt";
        tmpf = _fopen(tmp, "w"); /* create temporary file and write to it */
        if (tmpf == NULL)
                return 1;
        for (i = 0; i < 26; i++) {
                putc('a' + (i % 26), tmpf);
        }
        if (fclose(tmpf) == -1)
                return 1;
        for (i = 0; i < 10; i++) { /* expect a consecutive write attempt to fail */
                putc('0' + (i % 10), tmpf);
        }

        /* cleanup */
        unlink(tmp);

        return 0;
}
