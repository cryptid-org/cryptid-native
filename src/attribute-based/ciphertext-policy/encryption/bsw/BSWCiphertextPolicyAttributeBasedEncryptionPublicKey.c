#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include <stdlib.h>

void bswCiphertextPolicyAttributeBasedEncryptionPublicKey_destroy(bswCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey)
{
	ellipticCurve_destroy(publickey->ellipticCurve);
    affine_destroy(publickey->g);
    affine_destroy(publickey->f);
    affine_destroy(publickey->h);
    mpz_clear(publickey->q);
    complex_destroy(publickey->eggalpha);
    free(publickey);
}