#ifndef __CRYPTID_UTILS_H
#define __CRYPTID_UTILS_H

#include "gmp.h"

#include "CryptID.h"
#include "util/Validation.h"
#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/HashFunction.h"
#include "util/Status.h"


/**
 * Cryptographically hashes a string to an integer in a range.
 * @param result Out parameter storing an integer in the range \f$0\f$ to \f$p - 1\f$. Must be mpz_init'd and 
 *               mpz_clear'd by the caller.
 * @param s the string to hash
 * @param sLength the length of the string
 * @param p the upper limit of the range
 * @param hashFunction the hash function to use
 */
void hashToRange(mpz_t result, const unsigned char *const s, const int sLength, const mpz_t p, const HashFunction hashFunction);

/**
 * Cryptographically hashes a string to a point on the specified elliptic curve.
 * @param result Out parameter storing a point of order \f$q\f$ in \f$E(F_p)\f$. On CRYPTID_SUCCESS, it must be
 *               destroyed by the caller.
 * @param ellipticCurve the curve to operate on
 * @param p a prime
 * @param q a prime
 * @param id a string
 * @param idLength the length of the id string
 * @param hashFunction the hash function to use
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus hashToPoint(AffinePoint *result, const EllipticCurve ellipticCurve, const mpz_t p, const mpz_t q, 
                   const char *const id, const int idLength, const HashFunction hashFunction);

/**
 * Canonically represents elements of an extension field \f$F_p^2.\f$
 * @param resultLength out parameter storing the length of the resulting string
 * @param p an integer congruent to \f$3\f$ modulo \f$4\f$
 * @param v an element of \f$F_p^2\f$
 * @param order an ordering parameter that can be {@code 0} or {@code 1}
 * @return a string of size \f$2 \cdot \mathrm{Ceiling}(\frac{\log(p)}{8})\f$ octets
 */
unsigned char* canonical(int *const resultLength, const mpz_t p, const Complex v, const int order);

/**
 * Keyed cryptographic pseudo-random bytes generator.
 * @param b The length of the result. Must be less than or equal to the number of bytes in the output of the hash function.
 * @param p a string that will key the generator
 * @param pLength the length of the string
 * @param hashFunction the hashFunction to be used
 * @return a {@code b}-octet pseudo-random string
 */
unsigned char* hashBytes(const int b, const unsigned char *const p, const int pLength, const HashFunction hashFunction);

/**
 * Writes the public parameters to the following files: PP.ellipticCurve.a  PP.ellipticCurve.b  PP.ellipticCurve.fieldOrder  PP.hashf  PP.pointPpublic.x  PP.pointPpublic.y  PP.pointP.x  PP.pointP.y  PP.q in the folder ./PP.
 * @param publicParameters The parameter storing the public parameters.
 */
void writePublicParToFiles(PublicParameters* publicParameters);

/**
 * Writes the ciphertext tuple to the following files: cipher  cipherUx  cipherUy
 in the folder ./CT.
 * @param publicParameters The parameter storing the public parameters.
 */
void writeCipherTextToFiles(CipherTextTuple* ciphertext);

/**
 * Writes the private key to the following files: privateX  privateY
 in the folder ./PK.
 * @param publicParameters The parameter storing the public parameters.
 */
void writePrivateKeyToFiles(AffinePoint privateKey);

/**
 * Reads public parameters from the files written by writePublicParToFile.
 * @return the public parameters.
 */
PublicParameters readPublicParFromFile();

/**
 * Reads private key from the files written by writePublicParToFiles.
 * @return the private key.
 */
AffinePoint readPrivateKeyFromFiles();

/**
 * Reads ciphertext from the files written by writePublicParToFiles.
 * @return the ciphertext.
 */
CipherTextTuple readCipherTextFromFile();

#endif
