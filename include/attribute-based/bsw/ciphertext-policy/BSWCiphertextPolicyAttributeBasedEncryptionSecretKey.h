#ifndef __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#define __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#include "gmp.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "elliptic/AffinePoint.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionSecretKey
{
	AffinePoint D;
	AffinePoint* Dj;
	AffinePoint* DjA;
	char **attributes;
	int num_attributes;
	BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* pubkey;
} BSWCiphertextPolicyAttributeBasedEncryptionSecretKey;

void BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey);

#endif
