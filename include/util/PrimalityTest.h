#include "gmp.h"
#include "util/Validation.h"

/**
 * Check whether \f$p\f$ is a probable prime.
 * @param p the number to check
 * @result CRYPTID_VALIDATION_SUCCESS if \f$p\f$ is a probable prime
 */
CryptidValidationResult primalityTest_isProbablePrime(const mpz_t p);