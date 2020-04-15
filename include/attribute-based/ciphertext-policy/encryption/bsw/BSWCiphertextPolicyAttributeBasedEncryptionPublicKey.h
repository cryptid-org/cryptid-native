#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_PUBLICKEY_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_PUBLICKEY_ABE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "elliptic/TatePairing.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionPublicKey
{
	EllipticCurve ellipticCurve; // G0
	AffinePoint g; // generator of cyclic group
	AffinePoint h; // g^(beta)
	AffinePoint f; // g^(1/beta)
	Complex eggalpha; // e(g, g)^alpha
	HashFunction hashFunction;
	mpz_t q;
} BSWCiphertextPolicyAttributeBasedEncryptionPublicKey;

void BSWCiphertextPolicyAttributeBasedEncryptionPublicKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

#endif
