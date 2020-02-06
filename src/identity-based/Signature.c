#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"


HessIdentityBasedSignatureSignature hessIdentityBasedSignatureSignature_init(const AffinePoint u, const mpz_t v)
{    
    HessIdentityBasedSignatureSignature signature;
    signature.u = affine_init(u.x, u.y);
    
    mpz_init_set(signature.v, v);

    return signature;
}

void hessIdentityBasedSignatureSignature_destroy(HessIdentityBasedSignatureSignature signature)
{
    affine_destroy(signature.u);
    mpz_clear(signature.v);
}
