#ifndef __CRYPTID_CRYPTID_ABE_H
#define __CRYPTID_CRYPTID_ABE_H
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

CryptidStatus cryptid_abe_bsw_setup(const SecurityLevel securityLevel, BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickeyAsBinary,
								    BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkeyAsBinary);

CryptidStatus cryptid_abe_bsw_encrypt(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary,
                                  	  const char *const message, const size_t messageLength,
                                      const BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickeyAsBinary, BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary);

CryptidStatus cryptid_abe_bsw_keygen(const BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkeyAsBinary,
									 char** attributes, const int numAttributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary);

CryptidStatus cryptid_abe_bsw_delegate(const BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary,
									   char** attributes, const int numAttributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinaryNew);

CryptidStatus cryptid_abe_bsw_decrypt(char **result, const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary,
									  const BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary);

#endif
