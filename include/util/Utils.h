#ifndef __CRYPTID_UTILS_H
#define __CRYPTID_UTILS_H

#include "gmp.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Cryptographically hashes a string to an integer in a range.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter storing an integer in the range \f$0\f$ to \f$p-1\f$. Must be mpz_init'd and mpz_clear'd by the caller.
 *   * s
 *     * The string to hash.
 *   * sLength
 *     * The length of the string.
 *   * p
 *     * The upper limit of the range.
 *   * hashFunction
 *     * The hash function to use.
 */
void hashToRange(mpz_t result, const unsigned char *const s, const int sLength,
                 const mpz_t p, const HashFunction hashFunction);

/**
 * ## Description
 *
 * Cryptographically hashes a string to a point on the specified elliptic curve.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter storing a point of order \f$q\f$ in \f$E(F_p)\f$. On CRYPTID_SUCCESS, it must be destroyed by the caller.
 *   * id
 *     * A string.
 *   * idLength
 *     * The length of the id string.
 *   * q
 *     * A prime.
 *   * ellipticCurve
 *     * The curve to operate on.
 *   * hashFunction
 *     * The hash function to use.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus hashToPoint(AffinePoint *result, const char *const id,
                          const int idLength, const mpz_t q,
                          const EllipticCurve ellipticCurve,
                          const HashFunction hashFunction);

/**
 * ## Description
 *
 * Canonically represents elements of an extension field \f$F_p^2\f$.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter storing the resulting string of size \f$2 \cdot \mathrm{Ceiling}(\frac{\log(p)}{8})\f$ octets. SHOULD NOT BE INITIALIZED.
 *   * resultLength
 *     * Out parameter storing the length of the resulting string.
 *   * v
 *     * An element of \f$F_p^2\f$.
 *   * p
 *     * An integer congruent to \f$3\f$ modulo \f$4\f$.
 *   * order
 *     * An ordering parameter that can be {@code 0} or {@code 1}.
 */
void canonical(unsigned char **result, int *const resultLength, const Complex v,
               const mpz_t p, const int order);

/**
 * ## Description
 *
 * Keyed cryptographic pseudo-random bytes generator.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter stroring a {@code b}-octet pseudo-random string.
 *   * b
 *     * The length of the result. Must be less than or equal to the number of bytes in the output of the hash function.
 *   * p
 *     * A string that will key the generator.
 *   * pLength
 *     * The length of the string.
 *   * hashFunction
 *     * The hashFunction to be used.
 */
void hashBytes(unsigned char **result, const int b,
               const unsigned char *const p, const int pLength,
               const HashFunction hashFunction);

#endif
