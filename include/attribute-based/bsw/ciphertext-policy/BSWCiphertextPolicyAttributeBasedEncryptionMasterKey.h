#ifndef __CRYPTID_CRYPTID_MASTERKEY_ABE_H
#define __CRYPTID_CRYPTID_MASTERKEY_ABE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "elliptic/TatePairing.h"

#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionSecretKey.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionMasterKey
{
	mpz_t beta;
	AffinePoint g_alpha;
	BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* pubkey;
} BSWCiphertextPolicyAttributeBasedEncryptionMasterKey;

void BSWCiphertextPolicyAttributeBasedEncryptionMasterKey_destroy(BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey);

#endif
