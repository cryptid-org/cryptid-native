#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ABE_H
#include "gmp.h"


#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"
#include "util/Random.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"

CryptidStatus cryptid_abe_bsw_setup(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickeyAsBinary,
									bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkeyAsBinary,
									const SecurityLevel securityLevel);

CryptidStatus cryptid_abe_bsw_encrypt(bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary,
									  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary,
                                  	  const char *const message, const size_t messageLength,
                                      const bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickeyAsBinary);

CryptidStatus cryptid_abe_bsw_keygen(bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary,
									 const bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkeyAsBinary,
									 char** attributes, const int numAttributes);

CryptidStatus cryptid_abe_bsw_delegate(bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinaryNew,
									   const bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary,
									   char** attributes, const int numAttributes);

CryptidStatus cryptid_abe_bsw_decrypt(char **result, const bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary,
									  const bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary);

#endif
