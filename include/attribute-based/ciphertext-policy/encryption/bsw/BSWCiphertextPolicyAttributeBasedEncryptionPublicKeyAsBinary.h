#ifndef __CRYPTID_CRYPTID_PUBLICKEY_AS_BINARY_ABE_H
#define __CRYPTID_CRYPTID_PUBLICKEY_AS_BINARY_ABE_H

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "elliptic/AffinePointAsBinary.h"
#include "elliptic/EllipticCurveAsBinary.h"
#include "complex/ComplexAsBinary.h"
#include <stdlib.h>

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary
{
	EllipticCurveAsBinary ellipticCurve; // G0
	AffinePointAsBinary g; // generator of cyclic group
	AffinePointAsBinary h; // g^(beta)
	AffinePointAsBinary f; // g^(1/beta)
	ComplexAsBinary eggalpha; // e(g, g)^alpha
	HashFunction hashFunction;
	void *q;
	size_t qLength;
} BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary;

void BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publicKey);

void bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(BSWCiphertextPolicyAttributeBasedEncryptionPublicKey *publicKey, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publicKeyAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publicKeyAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey *publicKey);

#endif
