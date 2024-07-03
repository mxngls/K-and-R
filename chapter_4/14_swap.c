#include <stdio.h>

#define swap(t,x,y) { \
  t _z;               \
  x = y;              \
  y = _z;             \
}

int main() {
  int x, y;
  
  x = 0;
  y = 1;

  swap(int,x,y);

  printf("Value of x equals: %d\n", x);
  printf("Value of y equals: %d\n", y);

  return 0;
}
