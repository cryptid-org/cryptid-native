#include "gmp.h"

int millerrabin_mpz(mpz_srcptr n, int reps);

int millerrabin(mpz_srcptr n, mpz_srcptr nm1, mpz_ptr x, mpz_ptr y, mpz_srcptr q, unsigned long int k);