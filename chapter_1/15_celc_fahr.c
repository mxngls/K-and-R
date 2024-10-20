#include <stdio.h>

int temp_convert(int fahr) { return (5.0 / 9.0) * (fahr - 32); }

/*
 * Exercise 1-15. Rewrite the temperature conversión program of Section
 * 1.2 to use a function for conversion.
 */
int main() {
    int fahr;
    int celcius;

    celcius = 0;

    for (fahr = 0; fahr <= 300; fahr = fahr + 20) {
        celcius = temp_convert(fahr);
        printf("%3d %6.d\n", fahr, celcius);
    }

    return 0;
}
