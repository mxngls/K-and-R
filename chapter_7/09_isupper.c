#include <stdio.h>

#define cisupper(c) ((c) >= 'A' && (c) <= 'Z' ? 1 : 0)

int _isupper(const unsigned char c) { return c >= 'A' && c <= 'Z' ? 1 : 0; }

/*
 *  Exercise 7-9. Functions like isupper can be implemented to save space or to save time.
 * Explore both possibilities.
 */
int main(void) {
    /* implemented as function */
    printf("\"C\" is%svalid upper case according to our simple custom isupper function.\n",
           _isupper('C') ? " " : " not ");

    /* implemented as macro */
    printf("\"C\" is%svalid upper case even when implemented as a macro.\n",
           cisupper('C') == 1 ? " " : " not ");

    return 0;
}
