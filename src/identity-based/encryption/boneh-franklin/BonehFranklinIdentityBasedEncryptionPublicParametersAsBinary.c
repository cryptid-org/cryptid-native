#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h"

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_init(
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersAsBinaryOutput,
    const EllipticCurveAsBinary ellipticCurve, const void *const q,
    const size_t qLength, const AffinePointAsBinary pointP,
    const AffinePointAsBinary pointPpublic, const HashFunction hashFunction) {
  ellipticCurveAsBinary_init(
      &publicParametersAsBinaryOutput->ellipticCurve, ellipticCurve.a,
      ellipticCurve.aLength, ellipticCurve.b, ellipticCurve.bLength,
      ellipticCurve.fieldOrder, ellipticCurve.fieldOrderLength);

  publicParametersAsBinaryOutput->q = malloc(qLength + 1);
  memcpy(publicParametersAsBinaryOutput->q, q, qLength + 1);

  publicParametersAsBinaryOutput->qLength = qLength;

  affineAsBinary_init(&publicParametersAsBinaryOutput->pointP, pointP.x,
                      pointP.xLength, pointP.y, pointP.yLength);

  affineAsBinary_init(&publicParametersAsBinaryOutput->pointPpublic,
                      pointPpublic.x, pointPpublic.xLength, pointPpublic.y,
                      pointPpublic.yLength);

  publicParametersAsBinaryOutput->hashFunction = hashFunction;
}

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary) {
  ellipticCurveAsBinary_destroy(publicParametersAsBinary.ellipticCurve);
  free(publicParametersAsBinary.q);
  affineAsBinary_destroy(publicParametersAsBinary.pointP);
  affineAsBinary_destroy(publicParametersAsBinary.pointPpublic);
}

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_toBonehFranklinIdentityBasedEncryptionPublicParameters(
    BonehFranklinIdentityBasedEncryptionPublicParameters
        *publicParametersOutput,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary) {
  ellipticCurveAsBinary_toEllipticCurve(&publicParametersOutput->ellipticCurve,
                                        publicParametersAsBinary.ellipticCurve);

  mpz_init(publicParametersOutput->q);
  mpz_import(publicParametersOutput->q, publicParametersAsBinary.qLength, 1, 1,
             0, 0, publicParametersAsBinary.q);

  affineAsBinary_toAffine(&publicParametersOutput->pointP,
                          publicParametersAsBinary.pointP);

  affineAsBinary_toAffine(&publicParametersOutput->pointPpublic,
                          publicParametersAsBinary.pointPpublic);

  publicParametersOutput->hashFunction = publicParametersAsBinary.hashFunction;
}

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_fromBonehFranklinIdentityBasedEncryptionPublicParameters(
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersAsBinaryOutput,
    const BonehFranklinIdentityBasedEncryptionPublicParameters
        publicParameters) {
  ellipticCurveAsBinary_fromEllipticCurve(
      &publicParametersAsBinaryOutput->ellipticCurve,
      publicParameters.ellipticCurve);

  publicParametersAsBinaryOutput->q =
      mpz_export(NULL, &publicParametersAsBinaryOutput->qLength, 1, 1, 0, 0,
                 publicParameters.q);

  affineAsBinary_fromAffine(&publicParametersAsBinaryOutput->pointP,
                            publicParameters.pointP);

  affineAsBinary_fromAffine(&publicParametersAsBinaryOutput->pointPpublic,
                            publicParameters.pointPpublic);

  publicParametersAsBinaryOutput->hashFunction = publicParameters.hashFunction;
}