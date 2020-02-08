#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsString.h"

void bonehFranklinIdentityBasedEncryptionPublicParametersAsString_init(BonehFranklinIdentityBasedEncryptionPublicParametersAsString *publicParametersAsStringOutput, const EllipticCurveAsString ellipticCurve, const char *const q, const int qBase, const AffinePointAsString pointP, const AffinePointAsString pointPpublic, const HashFunction hashFunction)
{
    ellipticCurveAsString_init(&publicParametersAsStringOutput->ellipticCurve, ellipticCurve.a, ellipticCurve.aBase, ellipticCurve.b, ellipticCurve.bBase, ellipticCurve.fieldOrder, ellipticCurve.fieldOrderBase);

    int qLength = strlen(q);
    publicParametersAsStringOutput->q = (char*)malloc(qLength * sizeof(char));
    memcpy(publicParametersAsStringOutput->q, q, qLength);

    publicParametersAsStringOutput->qBase = qBase;

    affineAsString_init(&publicParametersAsStringOutput->pointP, pointP.x, pointP.xBase, pointP.y, pointP.yBase);

    affineAsString_init(&publicParametersAsStringOutput->pointPpublic, pointPpublic.x, pointPpublic.xBase, pointPpublic.y, pointPpublic.yBase);

    publicParametersAsStringOutput->hashFunction = hashFunction;
}

void bonehFranklinIdentityBasedEncryptionPublicParametersAsString_destroy(BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString)
{
    ellipticCurveAsString_destroy(publicParametersAsString.ellipticCurve);
    free(publicParametersAsString.q);
    affineAsString_destroy(publicParametersAsString.pointP);
    affineAsString_destroy(publicParametersAsString.pointPpublic);
}

void bonehFranklinIdentityBasedEncryptionPublicParametersAsString_toBonehFranklinIdentityBasedEncryptionPublicParameters(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString)
{
    ellipticCurveAsString_toEllipticCurve(&publicParametersOutput->ellipticCurve, publicParametersAsString.ellipticCurve);
    mpz_init_set_str(publicParametersOutput->q, publicParametersAsString.q, publicParametersAsString.qBase);
    affineAsString_toAffine(&publicParametersOutput->pointP, publicParametersAsString.pointP);
    affineAsString_toAffine(&publicParametersOutput->pointPpublic, publicParametersAsString.pointPpublic);
    publicParametersOutput->hashFunction = publicParametersAsString.hashFunction;
}

void bonehFranklinIdentityBasedEncryptionPublicParameters_toBonehFranklinIdentityBasedEncryptionPublicParametersAsString(BonehFranklinIdentityBasedEncryptionPublicParametersAsString *publicParametersAsStringOutput, const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters, const int base)
{
    ellipticCurve_toEllipticCurveAsString(&publicParametersAsStringOutput->ellipticCurve, publicParameters.ellipticCurve, base, base, base);
    
    publicParametersAsStringOutput->q = mpz_get_str(NULL, base, publicParameters.q);

    publicParametersAsStringOutput->qBase = base;
    
    affine_toAffineAsString(&publicParametersAsStringOutput->pointP, publicParameters.pointP, base, base);

    affine_toAffineAsString(&publicParametersAsStringOutput->pointPpublic, publicParameters.pointPpublic, base, base);
    
    publicParametersAsStringOutput->hashFunction = publicParameters.hashFunction;
}