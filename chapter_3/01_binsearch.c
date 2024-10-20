#include <stdio.h>

/* Look for an integer inside an integer array using binary search */
int binsearch(int x, int v[], int n) {
    int low, mid, high;

    low = 0;
    high = n - 1;

    while (low < high) {
        /*
         * Reminders are simply discarded (instead of simply beeing rounded)
         * if using input parameters of type int
         */
        mid = (low + high) / 2;

        if (x <= v[mid])
            high = mid;
        else
            low = mid + 1;
    }
    return v[low] == x ? low : -1;
}

/*
 * Exercise 3.1 Our binary search makes two tests inside the loop, when
 * one would suffice (at the price of more tests outside). Write a
 * version with only one test inside the loop and measure the difference
 * in run-time.
 */
int main() {

    int x, n;

    /*         0  1  2  3   4   5 */
    int v[] = {3, 6, 7, 9, 11, 15};

    x = 11;
    n = 6;

    // TODO: Compare execution time of the two different implementations.
    // Possible ways can be found at
    // https://clc-wiki.net/wiki/K&R2_solutions:Chapter_3:Exercise_1
    printf("%d\n", binsearch(x, v, n));

    return 0;
}
