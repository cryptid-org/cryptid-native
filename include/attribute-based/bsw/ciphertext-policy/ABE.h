#ifndef __CRYPTID_CRYPTID_ABE_H
#define __CRYPTID_CRYPTID_ABE_H
#include "gmp.h"


#include "attribute-based/bsw/ciphertext-policy/Defines.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/bsw/ciphertext-policy/MasterKey_ABE.h"
#include "util/Random.h"
#include "attribute-based/bsw/ciphertext-policy/Polynom.h"
#include "attribute-based/bsw/ciphertext-policy/AccessTree.h"
#include "attribute-based/bsw/ciphertext-policy/EncryptedMessage_ABE.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "attribute-based/bsw/ciphertext-policy/ABE_Utils.h"

CryptidStatus cryptid_setup_ABE(const SecurityLevel securityLevel, PublicKey_ABE* publickey, MasterKey_ABE* masterkey);

CryptidStatus cryptid_encrypt_ABE(EncryptedMessage_ABE* encrypted,
                                  const char *const message, const size_t messageLength,
                                  const PublicKey_ABE* publickey, AccessTree* accessTree);

CryptidStatus cryptid_keygen_ABE(const MasterKey_ABE* masterkey, char** attributes, const int num_attributes, SecretKey_ABE* secretkey);

CryptidStatus cryptid_delegate_ABE(const SecretKey_ABE* secretkey, char** attributes, const int num_attributes, SecretKey_ABE* secretkey_new);

CryptidStatus cryptid_decrypt_ABE(char **result, const EncryptedMessage_ABE* encrypted, const SecretKey_ABE* secretkey);

#endif
