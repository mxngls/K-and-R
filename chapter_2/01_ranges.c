#include <stdio.h>
#include <limits.h>

int main () {

  printf("UNSIGNED:\n");
  printf("%-10s: %20d ~ %20u\n", "Characters", 0, UCHAR_MAX);
  printf("%-10s: %20d ~ %20d\n", "Shorts", 0, USHRT_MAX);
  printf("%-10s: %20d ~ %20d\n", "Integers", 0, UINT_MAX);
  printf("%-10s: %20d ~ %20lu\n", "Longs", 0, ULONG_MAX);

  printf("\n---------------------------------------------------\n\n");

  printf("SIGNED:\n");
  printf("%-10s: %20d ~ %19d\n", "Characters", CHAR_MIN, CHAR_MAX);
  printf("%-10s: %20d ~ %19d\n", "Shorts", SHRT_MIN, SHRT_MAX);
  printf("%-10s: %20d ~ %19d\n", "Integers", INT_MIN, INT_MAX);
  printf("%-10s: %20ld ~ %19ld\n", "Longs", LONG_MIN, LONG_MAX);

  return 0;
}
