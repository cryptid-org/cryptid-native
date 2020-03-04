#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "util/Validation.h"

// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


/**
 * Struct storing the BF-IBE Public Parameters. Corresponds to {@code BFPublicParameters} in [RFC-5091].
 */
typedef struct BonehFranklinIdentityBasedEncryptionPublicParameters
{
    /**
     * The Type-1 elliptic curve we're operating over. Note, that the {@code p} field of {@code BFPublicParameters}
     * corresponds to {@code ellipticCurve.fieldOrder}.
     */
    EllipticCurve ellipticCurve;

    /**
     * Subgroup order.
     */
    mpz_t q;

    /**
     * A point in \f$E(F_p)\f$.
     */
    AffinePoint pointP;

    /**
     * A point in \f$E(F_p)\f$.
     */
    AffinePoint pointPpublic;

    /**
     * The used hash function.
     */
    HashFunction hashFunction;
} BonehFranklinIdentityBasedEncryptionPublicParameters;

void bonehFranklinIdentityBasedEncryptionPublicParameters_init(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const EllipticCurve ellipticCurve, const mpz_t q, const AffinePoint pointP, const AffinePoint pointPpublic, const HashFunction hashFunction);

void bonehFranklinIdentityBasedEncryptionPublicParameters_destroy(BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

/**
 * Validates that the specified public parameters are correct.
 * @param publicParameters the parameters to check
 * @return CRYPTID_VALIDATION_SUCCESS if the parameters are valid
 */
CryptidValidationResult bonehFranklinIdentityBasedEncryptionPublicParameters_isValid(const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

#endif
