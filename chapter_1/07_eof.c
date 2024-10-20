#include <stdio.h>

/* Exercise 1-7. Write a program to print the value of EOF. */ 
int main() {

    int c;

    while ((c = getchar()) != EOF) {
        printf("");
    }

    printf("\n\n");
    printf("%-20s%10d\n", "EOF as integer:", EOF);     /* EOF as integer      */
    printf("%-20s%10x\n", "EOF as hexadecimal:", EOF); /* EOF as hexadeicmal  */

    return 0;
}
