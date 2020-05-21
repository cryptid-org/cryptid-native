#ifndef __CRYPTID_RANDOM_H
#define __CRYPTID_RANDOM_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Generates a cryptographically secure random unsigned integer of the specified
 * bitlength.
 *
 * ## Parameters
 *
 *   * randomOutput
 *     * The generated random unsigned integer.
 *   * numberOfBits
 *     * The bitlength of the result.
 */
void random_unsignedIntOfLength(unsigned int *randomOutput,
                                const unsigned int numberOfBits);

/**
 * ## Description
 *
 * Generates a cryptographically secure random unsigned integer between zero and
 * {@code range} inclusive.
 *
 * ## Parameters
 *
 *   * randomOutput
 *     * The generated random unsigned integer.
 *   * range
 *     * The upper limit of the generation.
 */
void random_unsignedIntInRange(unsigned int *randomOutput,
                               const unsigned int range);

/**
 * ## Description
 *
 * Generates a cryptographically secure arbitrary precision integer of the
 * specified bitlength.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter for the generated value.
 *   * numberOfBits
 *     * The bitlength of the result.
 */
void random_mpzOfLength(mpz_t result, const unsigned int numberOfBits);

/**
 * ## Description
 *
 * Generates a cryptographically secure random arbitrary precision integer
 * between zero and {@code range} inclusive.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter for the generated value.
 *   * range
 *     * The upper limit of the generation.
 */
void random_mpzInRange(mpz_t result, const mpz_t range);

/**
 * ## Description
 *
 * enerates a cryptographically secure random Solinas prime having the
 * specified bitlength. A Solinas prime is a prime of the form \f$2^a \pm 2^b \pm 1\f$, where \f$a > b\f$.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter for the generated value.
 *   * numberOfBits
 *     * The bitlength of the result.
 *   * attemptLimit
 *     * The maximum number of attempts before giving up the generation.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus random_solinasPrime(mpz_t result, const unsigned int numberOfBits,
                                  const unsigned int attemptLimit);

/**
 * ## Description
 *
 * Generates a cryptographically secure random AffinePoint on the specified
 * EllipticCurve.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter for the generated value.
 *   * ellipticCurve
 *     * The elliptic curve to operate on.
 *   * attemptLimit
 *     * The maximum number of attempts before giving up the generation.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus random_affinePoint(AffinePoint *result,
                                 const EllipticCurve ellipticCurve,
                                 const unsigned int attemptLimit);

#endif
