#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h"
#include "util/PrimalityTest.h"

void bonehFranklinIdentityBasedEncryptionPublicParameters_init(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const EllipticCurve ellipticCurve, const mpz_t q, const AffinePoint pointP, const AffinePoint pointPpublic, const HashFunction hashFunction)
{
    ellipticCurve_init(&publicParametersOutput->ellipticCurve, ellipticCurve.a, ellipticCurve.b, ellipticCurve.fieldOrder);
    mpz_init_set(publicParametersOutput->q, q);
    affine_init(&publicParametersOutput->pointP, pointP.x, pointP.y);
    affine_init(&publicParametersOutput->pointPpublic, pointPpublic.x, pointPpublic.y);
    publicParametersOutput->hashFunction = hashFunction;
}

void bonehFranklinIdentityBasedEncryptionPublicParameters_destroy(BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters)
{
    ellipticCurve_destroy(publicParameters.ellipticCurve);
    mpz_clear(publicParameters.q);
    affine_destroy(publicParameters.pointP);
    affine_destroy(publicParameters.pointPpublic);
}

CryptidValidationResult bonehFranklinIdentityBasedEncryptionPublicParameters_isValid(const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters)
{
    if(ellipticCurve_isTypeOne(publicParameters.ellipticCurve) 
        && primalityTest_isProbablePrime(publicParameters.q)
        && affine_isValid(publicParameters.pointP, publicParameters.ellipticCurve)  
        && affine_isValid(publicParameters.pointPpublic, publicParameters.ellipticCurve)
        && hashFunction_isValid(publicParameters.hashFunction))
    {
        return CRYPTID_VALIDATION_SUCCESS;
    }

    return CRYPTID_VALIDATION_FAILURE;
}