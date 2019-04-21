#include "identity-based/Signature.h"


Signature signature_init(AffinePoint cipherU, mpz_t cipherV)
{    
    Signature signature;
    signature.cipherU = affine_init(cipherU.x, cipherU.y);
    
    mpz_init_set(signature.cipherV, cipherV);

    return signature;
}

void signature_destroy(Signature signature)
{
    affine_destroy(signature.cipherU);
    mpz_clear(signature.cipherV);
}
