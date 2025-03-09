#include <math.h>
#include <stdio.h>

/* Check if the current char is a valid as of the exercise description */
int valid(char c) {

    if (c >= 48 && c <= 57)
        return 1;
    else if (c >= 65 && c <= 70)
        return 1;
    else if (c >= 97 && c <= 102)
        return 1;
    else if (c == 88 || c == 120)
        return 1;

    return 0;
};

/* Convert a single hexadecimal value to it's integer representation */
int toint(char c) {
    int i = 0;

    if (c >= 48 && c <= 57)
        i = c - 48;
    else if (c >= 65 && c <= 70)
        i = 10 + c - 65;
    else if (c >= 97 && c <= 102)
        i = 10 + c - 97;

    printf("%d\n", i);

    return i;
}

/*
 * Exercise 2-3. Write the function htoi(s), which converts a string of
 * hexadecimal digits (including an optional 0x or 0X) into its
 * equivalent integer value. The allowable digits are O through 9, a
 * through f, and A through F.
 */
int main() {
    int c, d;
    int p = 0;
    int sum = 1;

    while ((c = getchar()) != EOF && c != 10) {
        if (!valid(c)) {
            printf("Error: Invalid character found '%c' (decimal %d).\n", c, c);
            return 1;
        }
        if (c == 48 && ((d = getchar()) == 88 || d == 120)) {
            continue;
        }

        /*
         * The current implementation assumes small-endianness. In order to
         * convert to an integer and assumme the input string conforms to
         * big-endianness we would need to first determine the length of the
         * string provided, then set p to said value and decrement instead
         * of increment with each iteration.
         */
        sum += (toint(c) * pow(16, p));
        p++;
    }

    printf("%d", sum);

    return 0;
}
