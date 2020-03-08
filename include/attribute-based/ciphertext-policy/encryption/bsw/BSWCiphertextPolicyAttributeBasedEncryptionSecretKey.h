#ifndef __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#define __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#include "gmp.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "elliptic/AffinePoint.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionSecretKey
{
	AffinePoint d;
	AffinePoint* dJ;
	AffinePoint* dJa;
	char **attributes;
	int numAttributes;
	BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey;
} BSWCiphertextPolicyAttributeBasedEncryptionSecretKey;

void BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey);

#endif
