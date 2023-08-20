#include <stdio.h>

int temp_convert(int fahr) { return (5.0 / 9.0) * (fahr - 32); }

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
