#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_PUBLICKEY_AS_BINARY_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_PUBLICKEY_AS_BINARY_ABE_H

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "elliptic/AffinePointAsBinary.h"
#include "elliptic/EllipticCurveAsBinary.h"
#include "complex/ComplexAsBinary.h"
#include <stdlib.h>

typedef struct bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary
{
	EllipticCurveAsBinary ellipticCurve; // G0
	AffinePointAsBinary g; // generator of cyclic group
	AffinePointAsBinary h; // g^(beta)
	AffinePointAsBinary f; // g^(1/beta)
	ComplexAsBinary eggalpha; // e(g, g)^alpha
	HashFunction hashFunction;
	void *q;
	size_t qLength;
} bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary;

void bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publicKey);

void bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publicKey, const bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publicKeyAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publicKeyAsBinary, const bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publicKey);

#endif
