#include <stdio.h>

/*
 * Exercise 1-3. Modify the temperature conversion program to print a
 * heading above the table.
 */
int main() {

    float celc, fahr;
    int step, higher;

    fahr = 0.0;
    celc = 0.0;
    step = 20;
    higher = 300;

    /* Print a header */
    printf("%-15s\t%s\n", "Fahrenheit", "Celsius");

    /* Convert the temperature from Fahrenheit to Celcius */
    while (fahr <= higher) {
        celc = (5.0 / 9.0) * (fahr - 32);
        printf("%10.0f\t%7.2f\n", fahr, celc);
        fahr += step;
    }

    printf("\n");

    return 0;
}
