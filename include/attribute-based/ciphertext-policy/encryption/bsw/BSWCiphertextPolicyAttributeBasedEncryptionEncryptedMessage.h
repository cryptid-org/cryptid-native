
#ifndef __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H
#define __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H

#include "gmp.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "complex/Complex.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet
{
	Complex cTilde;
	struct BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* cTildeSet;
	CtildeSetState last;
} BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet;

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage
{
	BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* tree;
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* cTildeSet;
	AffinePoint c;
} BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage;

void BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted);

#endif
