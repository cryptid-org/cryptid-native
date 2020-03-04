#include <stdlib.h>
#include <string.h>

#include "identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h"

void hessIdentityBasedSignatureSignatureAsBinary_init(HessIdentityBasedSignatureSignatureAsBinary *signatureAsBinaryOutput, const AffinePointAsBinary u, const void *const v, const size_t vLength)
{
    affineAsBinary_init(&signatureAsBinaryOutput->u, u.x, u.xLength, u.y, u.yLength);

    signatureAsBinaryOutput->v = malloc(vLength + 1);
    memcpy(signatureAsBinaryOutput->v, v, vLength + 1);

    signatureAsBinaryOutput->vLength = vLength;
}

void hessIdentityBasedSignatureSignatureAsBinary_destroy(HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary)
{
    affineAsBinary_destroy(signatureAsBinary.u);
    free(signatureAsBinary.v);
}

void hessIdentityBasedSignatureSignatureAsBinary_toHessIdentityBasedSignatureSignature(HessIdentityBasedSignatureSignature *signatureOutput, const HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary)
{
    affineAsBinary_toAffine(&signatureOutput->u, signatureAsBinary.u);
    
    mpz_init(signatureOutput->v);
    mpz_import(signatureOutput->v, signatureAsBinary.vLength, 1, 1, 0, 0, signatureAsBinary.v);
}

void hessIdentityBasedSignatureSignatureAsBinary_fromHessIdentityBasedSignatureSignature(HessIdentityBasedSignatureSignatureAsBinary *signatureAsBinaryOutput, const HessIdentityBasedSignatureSignature signature)
{
    affineAsBinary_fromAffine(&signatureAsBinaryOutput->u, signature.u);

    signatureAsBinaryOutput->v = mpz_export(NULL, &signatureAsBinaryOutput->vLength, 1, 1, 0, 0, signature.v);
}