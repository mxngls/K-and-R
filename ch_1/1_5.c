#include <stdio.h>

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
