#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include <stdlib.h>

void BSWCiphertextPolicyAttributeBasedEncryptionPublicKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey)
{
	ellipticCurve_destroy(publickey->ellipticCurve);
    affine_destroy(publickey->g);
    affine_destroy(publickey->f);
    affine_destroy(publickey->h);
    mpz_clear(publickey->q);
    complex_destroy(publickey->eggalpha);
    free(publickey);
}