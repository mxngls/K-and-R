#include <stdio.h>

/*
 * Exercise 1-5. Modify the temperature conversion program to print the
 * table in reverse order, that is, from 300 degrees to 0.
 */
int main() {

    float celc, fahr;
    int step, higher;

    fahr = 0.0;
    celc = 0.0;
    step = 20;
    higher = 300;

    /* Print a header */
    printf("%-15s\t%s\n", "Fahrenheit", "Celcius");

    /* Convert the temperature from Fahrenheit to Celcius */
    for (fahr = higher; fahr >= 0; fahr -= step) {
        celc = (5.0 / 9.0) * (fahr - 32);
        printf("%10.0f\t%7.2f\n", fahr, celc);
    }

    return 0;
}
