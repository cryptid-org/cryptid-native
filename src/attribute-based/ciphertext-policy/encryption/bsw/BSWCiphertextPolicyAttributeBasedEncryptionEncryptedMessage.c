#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include <stdlib.h>

void BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted)
{
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* lastSet = encrypted->cTildeSet;
	while(lastSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* prev = lastSet;
		complex_destroy(lastSet->cTilde);
		lastSet = lastSet->cTildeSet;
		free(prev);
	}
	free(lastSet);
	affine_destroy(encrypted->c);
	free(encrypted);
}