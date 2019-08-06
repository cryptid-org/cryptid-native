#include "identity-based/Signature.h"


Signature signature_init(AffinePoint u, mpz_t v)
{    
    Signature signature;
    signature.u = affine_init(u.x, u.y);
    
    mpz_init_set(signature.v, v);

    return signature;
}

void signature_destroy(Signature signature)
{
    affine_destroy(signature.u);
    mpz_clear(signature.v);
}
