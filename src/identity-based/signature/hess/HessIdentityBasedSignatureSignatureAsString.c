#include <stdlib.h>
#include <string.h>

#include "identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsString.h"

void hessIdentityBasedSignatureSignatureAsString_init(HessIdentityBasedSignatureSignatureAsString *signatureAsStringOutput, const AffinePointAsString u, const char *const v, const int vBase)
{
    affineAsString_init(&signatureAsStringOutput->u, u.x, u.xBase, u.y, u.yBase);

    int vLength = strlen(v);
    signatureAsStringOutput->v = (char*)malloc(vLength * sizeof(char) + 1);
    memcpy(signatureAsStringOutput->v, v, vLength + 1);

    signatureAsStringOutput->vBase = vBase;
}

void hessIdentityBasedSignatureSignatureAsString_destroy(HessIdentityBasedSignatureSignatureAsString signatureAsString)
{
    affineAsString_destroy(signatureAsString.u);
    free(signatureAsString.v);
}

void hessIdentityBasedSignatureSignatureAsString_toHessIdentityBasedSignatureSignature(HessIdentityBasedSignatureSignature *signatureOutput, const HessIdentityBasedSignatureSignatureAsString signatureAsString)
{
    affineAsString_toAffine(&signatureOutput->u, signatureAsString.u);
    
    mpz_init_set_str(signatureOutput->v, signatureAsString.v, signatureAsString.vBase);
}

void hessIdentityBasedSignatureSignature_toHessIdentityBasedSignatureSignatureAsString(HessIdentityBasedSignatureSignatureAsString *signatureAsStringOutput, const HessIdentityBasedSignatureSignature signature, const int uXBase, const int uYBase, const int vBase)
{
    affine_toAffineAsString(&signatureAsStringOutput->u, signature.u, uXBase, uYBase);

    signatureAsStringOutput->v = mpz_get_str(NULL, vBase, signature.v);

    signatureAsStringOutput->vBase = vBase;
}