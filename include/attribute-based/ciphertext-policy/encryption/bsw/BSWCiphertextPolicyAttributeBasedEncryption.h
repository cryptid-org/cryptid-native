#ifndef __CRYPTID_CRYPTID_ABE_H
#define __CRYPTID_CRYPTID_ABE_H
#include "gmp.h"


#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"
#include "util/Random.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"

CryptidStatus cryptid_abe_bsw_setup(const SecurityLevel securityLevel, BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey,
								    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey);

CryptidStatus cryptid_abe_bsw_encrypt(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted,
                                  	  const char *const message, const size_t messageLength,
                                      const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey, BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree);

CryptidStatus cryptid_abe_bsw_keygen(const BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey,
									 char** attributes, const int numAttributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey);

CryptidStatus cryptid_abe_bsw_delegate(const BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey,
									   char** attributes, const int numAttributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkeyNew);

CryptidStatus cryptid_abe_bsw_decrypt(char **result, const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted,
									  const BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey);

#endif
