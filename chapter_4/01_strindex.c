#include <stdio.h>

#define MAX_LINE 100

int strindex(char s[], char t[]) {
    int k, i, j;

    k = i = j = 0;

    while (s[i] != '\n') {

        if (t[j] != '\0') {

            if (s[i] == t[j]) {
                /* Save the index where a possible match might occur */
                if (j++ == 0)
                    k = i;
            } else
                /* Reset j as the current character in s doesn't match */
                j = 0;

        } else
            return k;

        i++;
    }

    return -1;
}

char _getline(char s[], int lim) {
    int i, c;

    i = 0;

    while ((c = getchar()) != EOF && c != '\n' && i < lim)
        s[i++] = c;

    if (c == '\n')
        s[i++] = c;

    s[i] = '\0';

    return i;
}

/*
 * Exercise 4-1. Write the function strindex(s,t) which returns the
 * position of the rightmost occurrence of t in s, or -1 if there is
 * none.
 */
int main() {
    char s[MAX_LINE];
    char t[MAX_LINE] = "test";
    int l, m;

    while ((l = _getline(s, MAX_LINE)) > 0) {
        if ((m = strindex(s, t)) != -1) {
            printf("%d\n", m);
            return 0;
        }
    }

    return -1;
}
