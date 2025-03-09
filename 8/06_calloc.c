#include "header.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static Header  base;         /* start of free list */
static Header *freep = NULL; /* empty list to get started */

/* free: put block ap in free list */
void _free(void *ap) {
        Header *bp, *p;
        bp = (Header *)ap - 1; /* point to block header */

        for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
                if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
                        break; /* freed block at start or end of arena */

        /* join to upper nbr */
        if (bp + bp->s.size == p->s.ptr) {
                bp->s.size += p->s.ptr->s.size;
                bp->s.ptr = p->s.ptr->s.ptr;
        } else {
                bp->s.ptr = p->s.ptr;
        }

        /* join to lower nbr */
        if (p + p->s.size == bp) {
                p->s.size += bp->s.size;
                p->s.ptr = bp->s.ptr;
        } else {
                p->s.ptr = bp;
        }

        freep = p;
}

#define NALLOC 1024 /* minimum #units to request */

/* morecore: ask system for more memory */
static Header *morecore(unsigned nu) {
        char   *cp, *sbrk(int);
        Header *up;

        if (nu < NALLOC)
                nu = NALLOC;

        cp = sbrk((int)(nu * sizeof(Header)));

        if (cp == (char *)-1) /* no space at all */
                return NULL;

        up         = (Header *)cp;
        up->s.size = nu;

        _free((void *)(up + 1));

        return freep;
}

/* malloc: general-purpose storage allocator */
void *_malloc(unsigned nbytes) {
        Header  *p, *prevp;
        Header  *moreroce(unsigned);
        unsigned nunits;

        nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

        if ((prevp = freep) == NULL) {
                base.s.ptr = freep = prevp = &base;
                base.s.size                = 0;
        }

        for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {
                if (p->s.size >= nunits) {
                        if (p->s.size == nunits) {
                                prevp->s.ptr = p->s.ptr;
                        } else {
                                p->s.size -= nunits;
                                p += p->s.size;
                                p->s.size = nunits;
                        }

                        freep = prevp;

                        return (void *)(p + 1);
                }

                if (p == freep)
                        if ((p = morecore(nunits)) == NULL)
                                return NULL;
        }
}

void *_calloc(unsigned n, unsigned nbytes) {
        unsigned *b;
        unsigned *bp;
        size_t    len;

        len = n * nbytes;
        b   = _malloc((unsigned)len);
        bp  = b;

        if (b == NULL)
                return NULL;

        while (--len)
                *bp++ = 0;

        return b;
}

#define HELLO "Hello, World!"

int main(void) {
        char *s;

        /* get some memory */
        s = _calloc(sizeof(HELLO), sizeof(char));
        strcpy(s, HELLO);

        printf("%s\n", s);

        /* cleanup */
        _free(s);

        return 0;
}
