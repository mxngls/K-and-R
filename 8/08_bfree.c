#include "header.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static Header  base;         /* start of free list */
static Header *freep = NULL; /* empty list to get started */

/* free: put block ap in free list */
void _free(void *ap) {
        Header *bp, *p;

        if (ap == NULL)
                return;

        bp = (Header *)ap - 1; /* point to block header and */
        if (bp->s.size <= 1)   /* verify we deal with a minimum amount of valid units */
                return;

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

#define NALLOC    1024 /* minimum #units to request */
#define MAXNALLOC 4096 /* maximum #units to request */

/* morecore: ask system for more memory */
static Header *morecore(unsigned nu) {
        char   *cp, *sbrk(int);
        Header *up;

        if (nu < NALLOC)
                nu = NALLOC;
        if (nu > MAXNALLOC)
                return NULL;

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

        p = prevp->s.ptr;
        for (;;) {
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

                prevp = p;
                p     = p->s.ptr;
        }
}

void _bfree(void *p, unsigned n) {
        unsigned nunits;
        Header  *hp;

        nunits = (n + sizeof(Header) - 1) / sizeof(Header) + 1;

        if (nunits < 2)
                return;

        /* clear */
        memset(p, 0, n);

        hp         = (Header *)p;
        hp->s.size = nunits;

        _free(hp + 1);
}

int main(void) {
        size_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        _bfree(arr, sizeof(arr));

        return 0;
}
