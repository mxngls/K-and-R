#include <stdio.h>

/* 
 * Exercsise 1-6. Verify that the expression getchar() != EOF is 0 or 1.
 */
int main() {

    int c;

    while (((c = getchar()) != EOF) != 0)
        ;

    return 0;
}
