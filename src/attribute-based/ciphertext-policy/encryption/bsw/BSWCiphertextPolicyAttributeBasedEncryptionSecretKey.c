#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionSecretKey.h"
#include <stdlib.h>

void BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey)
{
	for(int i = 0; i < secretkey->numAttributes; i++)
    {
    	affine_destroy(secretkey->dJ[i]);
    	affine_destroy(secretkey->dJa[i]);
        free(secretkey->attributes[i]);
    }
    free(secretkey->dJ);
    free(secretkey->dJa);
    free(secretkey->attributes);
    affine_destroy(secretkey->d);
    free(secretkey);
}