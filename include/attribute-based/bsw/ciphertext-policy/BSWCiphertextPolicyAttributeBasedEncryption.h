#ifndef __CRYPTID_CRYPTID_ABE_H
#define __CRYPTID_CRYPTID_ABE_H
#include "gmp.h"


#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"
#include "util/Random.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"

CryptidStatus cryptid_abe_bsw_setup(const SecurityLevel securityLevel, BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey,
								    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey);

CryptidStatus cryptid_abe_bsw_encrypt(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted,
                                  	  const char *const message, const size_t messageLength,
                                      const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey, BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree);

CryptidStatus cryptid_abe_bsw_keygen(const BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey,
									 char** attributes, const int num_attributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey);

CryptidStatus cryptid_abe_bsw_delegate(const BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey,
									   char** attributes, const int num_attributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey_new);

CryptidStatus cryptid_abe_bsw_decrypt(char **result, const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted,
									  const BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey);

#endif
