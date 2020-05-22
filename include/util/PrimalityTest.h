#ifndef __CRYPTID_PRIMALITY_TEST_H
#define __CRYPTID_PRIMALITY_TEST_H

#include "gmp.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * Check whether \f$p\f$ is a probable prime.
 *
 * ## Parameters
 *
 *   * p
 *     * The number to check.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if \f$p\f$ is a probable prime.
 */
CryptidValidationResult primalityTest_isProbablePrime(const mpz_t p);

#endif
