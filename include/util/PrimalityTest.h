#include "gmp.h"

int millerrabin_mpz(const mpz_srcptr n, const int reps);

int millerrabin(const mpz_srcptr n, const mpz_srcptr nm1, const mpz_ptr x, const mpz_ptr y,const  mpz_srcptr q, const unsigned long int k);