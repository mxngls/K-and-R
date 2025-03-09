#include <stddef.h>
#include <stdio.h>

#define SIZE 100

/*
 * Exercise 5-5. Write versions of the library functions strncpy,
 * strncat, and strncmp, which operate on at most the first n
 * characters of their argument strings. For example, strncpy(s,t,n)
 * copies at most n characters of t to s. Full descriptions are in
 * Appendix B
 */

/*
 * copy at most n characters of string ct to s; return s.
 * Pad with '\0's if ct has fewer than n characters.
 */
char *_strncpy(char *s, const char *ct, size_t n) {
    int i;

    i = 0;
    while (i++ < n) {
        if (*ct)
            *s++ = *ct++;
        else
            *s++ = '\0';
    }

    return s;
}

/*
 * concatentate at most n characters of string ct to string s, terminate
 * s with '\0'; return s.
 */
char *_strncat(char *s, const char *ct, size_t n) {
    int i;

    while (*s)
        s++;

    i = 0;
    while (++i < n && (*s++ = *ct++))
        ;

    *s = '\0';

    return s;
}

/*
 * compare at most n characters of string cs to string ct;
 * return <0 if cs<ct, 0 if cs==ct, or >0 if cs>ct.
 */
int _strncmp(const char *cs, const char *ct, size_t n) {
    int i;

    i = 0;
    while (i++ < n && (*cs++ == *ct++)) {
        if (!*cs)
            return 0;
    }

    return *cs - *ct;
}

int main() {
    char source[SIZE] = "Hello, World!";
    char target_1[SIZE];
    char target_2[SIZE];

    _strncpy(target_1, source, 6);

    /* prints "Hello," */
    printf("%s\n", target_1);

    _strncpy(target_2, source, 20);

    /* visualize null bytes used as padding */
    int i = -1;
    while (source[++i])
        printf("%c\n", target_1[i]);

    char head[SIZE] = "Hello, ";
    char tail[SIZE] = "World!";

    _strncat(head, tail, 6);

    /* prints "Hello, World" (missing '!') */
    printf("%s\n", head);

    char part[SIZE] = "Hello,";
    int r;
    r = _strncmp(source, part, 10);

    /* prints "87" */
    printf("%d\n", r);

    return 0;
}
