#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h"

void hessIdentityBasedSignaturePublicParameters_init(HessIdentityBasedSignaturePublicParameters *publicParametersOutput, const EllipticCurve ellipticCurve, const mpz_t q, const AffinePoint pointP, const AffinePoint pointPpublic, const HashFunction hashFunction)
{
    ellipticCurve_init(&publicParametersOutput->ellipticCurve, ellipticCurve.a, ellipticCurve.b, ellipticCurve.fieldOrder);
    mpz_init_set(publicParametersOutput->q, q);
    affine_init(&publicParametersOutput->pointP, pointP.x, pointP.y);
    affine_init(&publicParametersOutput->pointPpublic, pointPpublic.x, pointPpublic.y);
    publicParametersOutput->hashFunction = hashFunction;
}

void hessIdentityBasedSignaturePublicParameters_destroy(HessIdentityBasedSignaturePublicParameters publicParameters)
{
    ellipticCurve_destroy(publicParameters.ellipticCurve);
    mpz_clear(publicParameters.q);
    affine_destroy(publicParameters.pointP);
    affine_destroy(publicParameters.pointPpublic);
}
