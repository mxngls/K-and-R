#include <stdio.h>

int main() {
  
  printf("\a, \r, \m, \z, \s");

  return 0;
}


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

