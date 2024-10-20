#include <stdio.h>

#define swap(t, x, y)                                                          \
    {                                                                          \
        t _z;                                                                  \
        _z = x;                                                                \
        x = y;                                                                 \
        y = _z;                                                                \
    }

int main() {
    int x, y;
    double a, b;

    x = 0;
    y = 1;

    a = 0.00;
    b = 1.11;

    swap(int, x, y);

    printf("Value of x equals: %d\n", x);
    printf("Value of y equals: %d\n", y);

    swap(double, a, b);

    printf("Value of a equals: %f\n", a);
    printf("Value of b equals: %f\n", b);

    return 0;
}
