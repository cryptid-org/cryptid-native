#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * Struct storing the BF-IBE public parameters. Refer to [BFPublicParameters](https://tools.ietf.org/html/rfc5091#page-55) in [RFC-5091](https://tools.ietf.org/html/rfc5091).
 */
typedef struct BonehFranklinIdentityBasedEncryptionPublicParameters
{
    /**
     * ## Description
     *
     * The Type-1 elliptic curve we're operating over. Note, that the p field of [BFPublicParameters](https://tools.ietf.org/html/rfc5091#page-55) corresponds to [ellipticCurve.fieldOrder](elliptic/EllipticCurve.h#fieldOrder).
     */
    EllipticCurve ellipticCurve;

    /**
     * ## Description
     *
     * Subgroup order.
     */
    mpz_t q;

    /**
     * ## Description
     *
     * A generator point of group order [q](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#q) on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve).
     */
    AffinePoint pointP;

    /**
     * ## Description
     *
     * A point on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve) which is [pointP](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#pointP) multiplied with the master secret.
     */
    AffinePoint pointPpublic;

    /**
     * ## Description
     *
     * The hash function, used by the system.
     */
    HashFunction hashFunction;
} BonehFranklinIdentityBasedEncryptionPublicParameters;

/**
 * ## Description
 *
 * Initializes a new [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters) with the specified values.
 *
 * ## Parameters
 *
 *   * publicParametersOutput
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters) to be initialized.
 *   * ellipticCurve
 *     * The [EllipticCurve](elliptic/EllipticCurve.h#EllipticCurve), the system operates on.
 *   * q
 *     * The field order of the subgroup, the system operates on.
 *   * pointP
 *     * A generator point of group order [q](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#q) on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve).
 *   * pointPpublic
 *     * A point on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve) which is [pointP](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#pointP) multiplied with the master secret.
 *   * hashFunction
 *     * The hash function, the system operates with.
 */
void bonehFranklinIdentityBasedEncryptionPublicParameters_init(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const EllipticCurve ellipticCurve, const mpz_t q, const AffinePoint pointP, const AffinePoint pointPpublic, const HashFunction hashFunction);

/**
 * ## Description
 *
 * Frees a [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 *
 * ## Parameters
 *
 *   * publicParameters
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters) to be destroyed.
 */
void bonehFranklinIdentityBasedEncryptionPublicParameters_destroy(BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

/**
 * ## Description
 *
 * Validates that the specified public parameters are correct.
 *
 * ## Parameters
 *
 *   * publicParameters
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters) to check.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the parameters are valid.
 */
CryptidValidationResult bonehFranklinIdentityBasedEncryptionPublicParameters_isValid(const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

#endif
