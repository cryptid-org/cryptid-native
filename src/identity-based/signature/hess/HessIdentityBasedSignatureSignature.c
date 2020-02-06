#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"


void hessIdentityBasedSignatureSignature_init(HessIdentityBasedSignatureSignature *signatureOutput, const AffinePoint u, const mpz_t v)
{
    signatureOutput->u = affine_init(u.x, u.y);
    
    mpz_init_set(signatureOutput->v, v);
}

void hessIdentityBasedSignatureSignature_destroy(HessIdentityBasedSignatureSignature signature)
{
    affine_destroy(signature.u);
    mpz_clear(signature.v);
}
