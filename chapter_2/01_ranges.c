#include <limits.h>
#include <stdio.h>

int main() {

  printf("Unsigned:\n");
  printf("%-10s: %20d ~ %20u\n", "Characters", 0, UCHAR_MAX);
  printf("%-10s: %20d ~ %20d\n", "Shorts", 0, USHRT_MAX);
  printf("%-10s: %20d ~ %20d\n", "Integers", 0, UINT_MAX);
  printf("%-10s: %20d ~ %20lu\n", "Longs", 0, ULONG_MAX);

  printf("\n-------------------------------------------------------\n\n");

  printf("Signed:\n");
  printf("%-10s: %20d ~ %19d\n", "Characters", CHAR_MIN, CHAR_MAX);
  printf("%-10s: %20d ~ %19d\n", "Shorts", SHRT_MIN, SHRT_MAX);
  printf("%-10s: %20d ~ %19d\n", "Integers", INT_MIN, INT_MAX);
  printf("%-10s: %20ld ~ %19ld\n", "Longs", LONG_MIN, LONG_MAX);

  printf("\n-------------------------------------------------------\n\n");

  /* The calculation is not too straighforward without any prior knowledge but
   * not too complicated once one understands that we simply need to apply type
   * casting to set and convert between types
   *
   * ~0 will give bits in 1s (i.e. maximum values)
   * (unsigned <type>) will cast it unsigned
   * >> 1 right shifts 1 bit to remove the sign bit
   * (<type>) casting it the required type again
   *
   */
  printf("CALCULATED Unsigned:\n");
  printf("%-10s: %20d ~ %20u\n", "Characters", 0,
         ((unsigned char)~0));
  printf("%-10s: %20d ~ %20u\n", "Shorts", 0,
         ((unsigned short)~0));
  printf("%-10s: %20d ~ %20u\n", "Integers", 0,
         ((unsigned int)~0));
  printf("%-10s: %20d ~ %20lu\n", "Longs", 1,
         ((unsigned long)~0));

  printf("\n-------------------------------------------------------\n\n");

  printf("CALCULATED Signed:\n");
  printf("%-10s: %20d ~ %20u\n", "Characters", -(char)((unsigned char)~0 >> 1) - 1,
         (char)((unsigned char)~0 >> 1));
  printf("%-10s: %20d ~ %20u\n", "Shorts", -(short)((unsigned short)~0 >> 1) - 1,
         (short)((unsigned short)~0 >> 1));
  printf("%-10s: %20d ~ %20u\n", "Ints", -(int)((unsigned int)~0 >> 1) - 1,
         (int)((unsigned int)~0 >> 1));
  printf("%-10s: %20lu ~ %20lu\n", "Longs", -(long)((unsigned long)~0 >> 1) - 1,
         (long)((unsigned long)~0 >> 1));

  return 0;
}
