#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include <stdlib.h>

void bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted)
{
	bswCiphertextPolicyAttributeBasedEncryptionCtildeSet* lastSet = encrypted->cTildeSet;
	while(lastSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		bswCiphertextPolicyAttributeBasedEncryptionCtildeSet* prev = lastSet;
		complex_destroy(lastSet->cTilde);
		lastSet = lastSet->cTildeSet;
		free(prev);
	}
	free(lastSet);
	affine_destroy(encrypted->c);
	free(encrypted);
}