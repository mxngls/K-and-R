#include <limits.h>
#include <stdio.h>

int _getline(char s[]) {
    int i;
    char c;

    for (i = 0; i < MAX_INPUT; i++) {

        c = getchar();

        if (c == '\n') {
            s[i] = c;
            i++;
            break;
        } else if (c == EOF) {
            return 0;
        } else {
            s[i] = c;
        }
    }

    s[i] = '\0';

    return i;
}

/* 
 * Exercise 2-2. Write a loop equivalent to the for loop above without
 * using && or ||.
 */
int main() { int l; char s[MAX_INPUT];

    while ((l = _getline(s)) > 0) {
        printf("%s", s);
    }

    return 0;
}
