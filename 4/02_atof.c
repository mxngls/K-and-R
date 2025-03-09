#include <ctype.h>
#include <stdio.h>

#define MAX_LEN 4096

/* atof:  convert string s to double */
double atof(char s[]) {
    double val, power, exp_val;
    int i, j, sign, exp_sign;

    for (i = 0; isspace(s[i]); i++) /* skip white space */
        ;

    sign = (s[i] == '-') ? -1 : 1;

    if (s[i] == '+' || s[i] == '-')
        i++;

    for (val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');

    if (s[i] == '.')
        i++;

    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10;
    }

    if (s[i] == 'e' || s[i] == 'E')
        i++;

    exp_sign = (s[i] == '-') ? -1 : 1;

    if (s[i] == '+' || s[i] == '-')
        i++;

    for (exp_val = 0; isdigit(s[i]); i++)
        exp_val = 10.0 * exp_val + (s[i] - '0');

    double exp_factor = 1.0;
    for (j = 0; j < exp_val; j++)
        exp_factor *= 10.0;

    if (exp_sign == -1)
        exp_factor = 1.0 / exp_factor;

    return (sign * val / power) * exp_factor;
}

/* Exercise 4-2. Extend atof to handle scientific notation of the form */
int main() {
    char s[MAX_LEN] = "123.45e-6";
    double d;

    d = atof(s);

    printf("%.8f\n", d);

    return 0;
}
