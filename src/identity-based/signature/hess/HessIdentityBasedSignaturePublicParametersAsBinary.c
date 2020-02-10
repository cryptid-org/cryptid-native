#include <stdlib.h>
#include <string.h>

#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h"

void hessIdentityBasedSignaturePublicParametersAsBinary_init(HessIdentityBasedSignaturePublicParametersAsBinary *publicParametersAsBinaryOutput, const EllipticCurveAsBinary ellipticCurve, const char *const q, const size_t qLength, const AffinePointAsBinary pointP, const AffinePointAsBinary pointPpublic, const HashFunction hashFunction)
{
    ellipticCurveAsBinary_init(&publicParametersAsBinaryOutput->ellipticCurve, ellipticCurve.a, ellipticCurve.aLength, ellipticCurve.b, ellipticCurve.bLength, ellipticCurve.fieldOrder, ellipticCurve.fieldOrderLength);

    publicParametersAsBinaryOutput->q = (char*)malloc(qLength * sizeof(char) + 1);
    memcpy(publicParametersAsBinaryOutput->q, q, qLength + 1);

    publicParametersAsBinaryOutput->qLength = qLength;

    affineAsBinary_init(&publicParametersAsBinaryOutput->pointP, pointP.x, pointP.xLength, pointP.y, pointP.yLength);

    affineAsBinary_init(&publicParametersAsBinaryOutput->pointPpublic, pointPpublic.x, pointPpublic.xLength, pointPpublic.y, pointPpublic.yLength);

    publicParametersAsBinaryOutput->hashFunction = hashFunction;
}

void hessIdentityBasedSignaturePublicParametersAsBinary_destroy(HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary)
{
    ellipticCurveAsBinary_destroy(publicParametersAsBinary.ellipticCurve);
    free(publicParametersAsBinary.q);
    affineAsBinary_destroy(publicParametersAsBinary.pointP);
    affineAsBinary_destroy(publicParametersAsBinary.pointPpublic);
}

void hessIdentityBasedSignaturePublicParametersAsBinary_toHessIdentityBasedSignaturePublicParameters(HessIdentityBasedSignaturePublicParameters *publicParametersOutput, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary)
{
    ellipticCurveAsBinary_toEllipticCurve(&publicParametersOutput->ellipticCurve, publicParametersAsBinary.ellipticCurve);

    mpz_init(publicParametersOutput->q);
    mpz_import(publicParametersOutput->q, publicParametersAsBinary.qLength, 1, 1, 0, 0, publicParametersAsBinary.q);
    
    affineAsBinary_toAffine(&publicParametersOutput->pointP, publicParametersAsBinary.pointP);

    affineAsBinary_toAffine(&publicParametersOutput->pointPpublic, publicParametersAsBinary.pointPpublic);

    publicParametersOutput->hashFunction = publicParametersAsBinary.hashFunction;
}

void hessIdentityBasedSignaturePublicParameters_toHessIdentityBasedSignaturePublicParametersAsBinary(HessIdentityBasedSignaturePublicParametersAsBinary *publicParametersAsBinaryOutput, const HessIdentityBasedSignaturePublicParameters publicParameters)
{
    ellipticCurve_toEllipticCurveAsBinary(&publicParametersAsBinaryOutput->ellipticCurve, publicParameters.ellipticCurve);

    publicParametersAsBinaryOutput->q = mpz_export(NULL, &publicParametersAsBinaryOutput->qLength, 1, 1, 0, 0, publicParameters.q);
    
    affine_toAffineAsBinary(&publicParametersAsBinaryOutput->pointP, publicParameters.pointP);

    affine_toAffineAsBinary(&publicParametersAsBinaryOutput->pointPpublic, publicParameters.pointPpublic);
    
    publicParametersAsBinaryOutput->hashFunction = publicParameters.hashFunction;
}
