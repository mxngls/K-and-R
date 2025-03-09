#include <errno.h>
#include <stdlib.h>

#undef NULL
#define NULL     0
#define EOF      (-1)
#define BUFSIZ   1024 /* choose small value on purpose */
#define OPEN_MAX 20   /* max #files open at once */

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

#define feof(p)    (((p)->flag & _EOF) != 0)
#define ferror(p)  (((p)->flag & _ERR) != 0)
#define fileno(p)  ((p)->fd)
#define getc(p)    (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))
#define getchar()  getc(_stdin)
#define putchar(x) putc((x), _stdout)

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
                if ((fp->base = malloc((unsigned long)bufsize)) == NULL) {
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
int _fflush(_FILE *stream) {
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
int _fclose(_FILE *stream) {
        if (stream == NULL) {
                return EOF;
        }

        /* flush the stream's buffer if it is an output buffer */
        if ((stream->flag & _WRITE) == _WRITE) {
                if (_fflush(stream) == EOF) {
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
 * From the appendix:
 * fseek sets the file position for stream; a subsequent read or write will access data
 * beginning at the new position. For a binary file, the position is set to offset characters
 * from origin, which may be SEEK_SET (beginning), SEEK_CUR (current position), or
 * SEEK_END (end of file). For a text stream, offset must be zero, or a value returned by
 * ftell (in which case origin must be SEEK_SET). fseek returns non-zero on error.
 */
long _fseek(_FILE *fp, long offset, int origin) {
        /* stale file pointer or not opened yet */
        if (fp == NULL || !(fp->flag & (_READ | _WRITE))) {
                return -1;
        }

        /* no buffer allocated yet */
        if (fp->base == NULL) {
                return -1;
        }

        /* write buffered data before seeking */
        if (fp->flag & _WRITE) {
                if (_fflush(fp)) {
                        fp->flag |= _ERR;
                        return -1;
                }
        } else {
                /*
                 * adjust for a possible difference between the actual current position and the
                 * possition specified by SEEK_CUR
                 */
                offset -= (origin == SEEK_CUR) ? fp->cnt : 0L;

                /* invalidate the current state of the given file pointer */
                fp->cnt = 0;
                fp->ptr = fp->base;
        }

        if (lseek(fp->fd, offset, origin) == -1) {
                fp->flag |= _ERR;
                return -1;
        }

        /* remove indicator of EOF */
        fp->flag &= ~_EOF;

        return 0;
}

/*
 * Exercise 8-4. The standard library function int fseek(FILE *fp, long offset, int origin) is
 * identical to lseek except that fp is a file pointer instead of a file descriptor and return value
 * is an int status, not a position. Write fseek. Make sure that your fseek coordinates properly
 * with the buffering done for the other functions of the library.
 */
int main(void) {
        int    i;
        char   c;
        char  *tmp;
        _FILE *tmpf;

        tmp  = "tmp_04_fseek.txt";
        tmpf = _fopen(tmp, "w");
        if (tmpf == NULL) {
                write(_stdout->fd, "error: could not open or create file\n", 37);
                return 1;
        }

        for (i = 0; i < 26; i++) {
                putc('a' + (i % 26), tmpf);
        }

        if (_fclose(tmpf)) {
                write(_stdout->fd, "error: could not close file\n", 29);
                return 1;
        }

        /* re-open and manually allocate a adequate buffer */
        tmpf = _fopen(tmp, "r");
        if (tmpf == NULL) {
                write(_stdout->fd, "error: could not open file for reading\n", 40);
                return 1;
        }
        _fillbuf(tmpf);

        /* seek until "d" and then print from there onwards */
        _fseek(tmpf, 3L, SEEK_SET);

        while ((c = (char)getc(tmpf)) != EOF) {
                putc(c, _stdout);
        }
        _fflush(_stdout);

        putc('\n', _stdout);
        putc('\n', _stdout);

        /* seek until "w" and then print from there onwards */
        _fseek(tmpf, -3L, SEEK_END);
        while ((c = (char)getc(tmpf)) != EOF) {
                putc(c, _stdout);
        }
        _fflush(_stdout);

        if (_fclose(tmpf) == -1) {
                write(_stdout->fd, "error: could not close file\n", 29);
                return 1;
        }

        /* cleanup */
        unlink(tmp);

        return 0;
}
