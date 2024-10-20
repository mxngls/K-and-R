#include <stdio.h>

/*
 * Exercise 1-2. Experiment to find out what happens when prints's
 * argument string contains \c, where c is some character not listed
 * above.
 *
 * A full list of all available escape sequences can be found at
 * https://en.cppreference.com/w/c/language/escape
 */
int main() {

    printf("\a, \r, \e, \v, \s");

    return 0;
}

/* clang-format off */
/*   max@max-mbpro: .../c_programming_lang/ch_1 $ ./run.sh 1_2                             main */
/*   1_2.c:5:19: warning: unknown escape sequence '\m' [-Wunknown-escape-sequence]              */
/*     printf("\a, \r, \m, \z, \s");                                                            */
/*                     ^~                                                                       */
/*   1_2.c:5:23: warning: unknown escape sequence '\z' [-Wunknown-escape-sequence]              */
/*     printf("\a, \r, \m, \z, \s");                                                            */
/*                         ^~                                                                   */
/*   1_2.c:5:27: warning: unknown escape sequence '\s' [-Wunknown-escape-sequence]              */
/*     printf("\a, \r, \m, \z, \s");                                                            */
/*                             ^~                                                               */
/*   3 warnings generated.                                                                      */
/*   , m, z, s%                                                                                 */
/* clang-format on */
