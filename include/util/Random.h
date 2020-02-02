#ifndef __CRYPTID_RANDOM_H
#define __CRYPTID_RANDOM_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"

/**
 * Generates a cryptographically secure random unsigned integer of the specified bitlength.
 * @param numberOfBits the bitlength of the result
 * @return a random unsigned integer
 */
unsigned int random_unsignedIntOfLength(const unsigned int numberOfBits);

/**
 * Generates a cryptographically secure random unsigned integer between zero and {@code range} inclusive.
 * @param range the upper limit of the generation
 * @return a random unsigned integer
 */
unsigned int random_unsignedIntInRange(const unsigned int range);

/**
 * Generates a cryptographically secure arbitrary precision integer of the specified bitlength.
 * @param result out parameter for the generated value
 * @param numberOfBits the bitlength of the result
 */
void random_mpzOfLength(mpz_t result, const unsigned int numberOfBits);

/**
 * Generates a cryptographically secure random arbitrary precision integer between zero and {@code range} inclusive.
 * @param result out parameter for the generated value
 * @param range the upper limit of the generation
 */
void random_mpzInRange(mpz_t result, const mpz_t range);

/**
 * Generates a cryptographically secure random Solinas prime having the specified bitlength.
 * 
 * A Solinas prime is a prime of the form \f$2^a \pm 2^b \pm 1\f$.
 * @param result out parameter for the generated value
 * @param numberOfBits the bitlength of the result
 * @param attemptLimit the maximum number of attempts before giving up the generation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus random_solinasPrime(mpz_t result, const unsigned int numberOfBits, const unsigned int attemptLimit);

/**
 * Generates a cryptographically secure random AffinePoint on the specified EllipticCurve.
 * @param result out parameter for the generated value
 * @param ellipticCurve the elliptic curve to operate on
 * @param attemptLimit the maximum number of attempts before giving up the generation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus random_affinePoint(AffinePoint* result, const EllipticCurve ellipticCurve, const unsigned int attemptLimit);

#endif
