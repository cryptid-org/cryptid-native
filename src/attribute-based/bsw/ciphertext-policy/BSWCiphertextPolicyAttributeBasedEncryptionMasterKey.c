#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"
#include <stdlib.h>

void BSWCiphertextPolicyAttributeBasedEncryptionMasterKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey)
{
	mpz_clear(masterkey->beta);
    affine_destroy(masterkey->g_alpha);
    free(masterkey);
}