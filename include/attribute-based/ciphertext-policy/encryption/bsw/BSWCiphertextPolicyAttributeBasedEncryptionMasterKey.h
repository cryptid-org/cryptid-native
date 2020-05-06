#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_MASTERKEY_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_MASTERKEY_ABE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "elliptic/TatePairing.h"
#include "util/HashFunction.h"

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionSecretKey.h"

typedef struct bswCiphertextPolicyAttributeBasedEncryptionMasterKey {
  mpz_t beta;
  AffinePoint g_alpha;
  bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey;
} bswCiphertextPolicyAttributeBasedEncryptionMasterKey;

void bswCiphertextPolicyAttributeBasedEncryptionMasterKey_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKey *masterkey);

#endif
