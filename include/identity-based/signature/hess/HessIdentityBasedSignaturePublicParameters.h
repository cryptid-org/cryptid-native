#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "util/Validation.h"

/**
 * Struct storing the Hess-IBS Public Parameters.
 */
typedef struct HessIdentityBasedSignaturePublicParameters
{
    /**
     * The Type-1 elliptic curve we're operating over.
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
} HessIdentityBasedSignaturePublicParameters;

void hessIdentityBasedSignaturePublicParameters_init(HessIdentityBasedSignaturePublicParameters *publicParametersOutput, const EllipticCurve ellipticCurve, const mpz_t q, const AffinePoint pointP, const AffinePoint pointPpublic, const HashFunction hashFunction);

void hessIdentityBasedSignaturePublicParameters_destroy(HessIdentityBasedSignaturePublicParameters publicParameters);

/**
 * Validates that the specified public parameters are correct.
 * @param publicParameters the parameters to check
 * @return CRYPTID_VALIDATION_SUCCESS if the parameters are valid
 */
CryptidValidationResult hessIdentityBasedSignaturePublicParameters_isValid(const HessIdentityBasedSignaturePublicParameters publicParameters);

#endif
