#include <stdio.h>

#define SIZE 100

/* strcat: concatentate t to end of s; s must be big nough */
void _strcat(char *s, char *t) {
    while (*s) /* find end of s */
        s++;

    while ((*s++ = *t++)) /* copy t */
        ;
}

/*
 * Exercise 5-3. Write a pointer version of the function strcat that we
 * showed in Chapter 2: strcat(s,t) copies the string t to the end of s.
 */
int main() {
    char source[SIZE] = "Hello, ";
    char target[SIZE] = "World!";

    _strcat(source, target);

    printf("concatentated string: %s\n", source);

    return 0;
}
