#include <stdio.h>

int main() {

  float celc, fahr;
  int step, higher;

  fahr = 0.0;
  celc = 0.0;
  step = 10;
  higher = 100;

  /* Print a header */
  printf("%s\t%15s\n", "Celcius", "Fahrenheit");

  /* Convert the temperature from Celcius to Fahrenheit*/
  while (celc <= higher) {
    fahr = (celc / (5.0 / 9.0) + 32);
    printf("%7.0f\t%15.2f\n", celc, fahr);
    celc += step;
  }

  printf("\n");

  return 0;
}

