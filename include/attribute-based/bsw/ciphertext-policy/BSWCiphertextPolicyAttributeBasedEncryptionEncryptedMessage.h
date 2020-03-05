
#ifndef __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H
#define __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H

#include "gmp.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "complex/Complex.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet
{
	Complex Ctilde;
	struct BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* Ctilde_set;
	int last;
} BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet;

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage
{
	BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* tree;
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* Ctilde_set;
	AffinePoint C;
} BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage;

void BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted);

#endif
