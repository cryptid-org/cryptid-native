#include "gmp.h"
#include "util/Validation.h"

int primaltyTest_millerrabin_mpz(const mpz_srcptr n, const int reps);

int primaltyTest_millerrabin(const mpz_srcptr n, const mpz_srcptr nm1, const mpz_ptr x, const mpz_ptr y,const  mpz_srcptr q, const unsigned long int k);

/**
 * Check whether \f$p\f$ is a probable prime.
 * @param p the number to check
 * @result CRYPTID_VALIDATION_SUCCESS if \f$p\f$ is a probable prime
 */
CryptidValidationResult primaltyTest_isProbablePrime(const mpz_t p);