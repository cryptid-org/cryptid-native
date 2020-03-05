#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include <stdlib.h>

void BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted)
{
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* lastSet = encrypted->Ctilde_set;
	while(lastSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* prev = lastSet;
		complex_destroy(lastSet->Ctilde);
		lastSet = lastSet->Ctilde_set;
		free(prev);
	}
	free(lastSet);
	affine_destroy(encrypted->C);
	free(encrypted);
}