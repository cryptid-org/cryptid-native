#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_AS_STRING_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_AS_STRING_H

#include "elliptic/AffinePointAsString.h"
#include "elliptic/EllipticCurveAsString.h"
#include "util/HashFunction.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h"


/**
 * Struct storing the Hess-IBS Public Parameters as strings.
 */
typedef struct HessIdentityBasedSignaturePublicParametersAsString
{
    /**
     * The Type-1 elliptic curve we're operating over represented by string values.
     */
    EllipticCurveAsString ellipticCurve;

    /**
     * Subgroup order represented as a string.
     */
    char *q;

    /**
     * The base of the Subgroup order's string representation.
     */
    int qBase;

    /**
     * The string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsString pointP;

    /**
     * The string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsString pointPpublic;

    /**
     * The used hash function.
     */
    HashFunction hashFunction;
} HessIdentityBasedSignaturePublicParametersAsString;

void hessIdentityBasedSignaturePublicParametersAsString_init(HessIdentityBasedSignaturePublicParametersAsString *publicParametersAsStringOutput, const EllipticCurve ellipticCurve, const char *const q, const int qBase, const AffinePoint pointP, const AffinePoint pointPpublic, const HashFunction hashFunction);

void hessIdentityBasedSignaturePublicParametersAsString_destroy(HessIdentityBasedSignaturePublicParametersAsString publicParametersAsString);

void hessIdentityBasedSignaturePublicParametersAsString_toHessIdentityBasedSignaturePublicParameters(HessIdentityBasedSignaturePublicParameters *publicParametersOutput, const HessIdentityBasedSignaturePublicParametersAsString publicParametersAsString);

void hessIdentityBasedSignaturePublicParameters_toHessIdentityBasedSignaturePublicParametersAsString(HessIdentityBasedSignaturePublicParametersAsString *publicParametersAsStringOutput, const HessIdentityBasedSignaturePublicParameters publicParameters);

#endif
