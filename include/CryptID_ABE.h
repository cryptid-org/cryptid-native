#ifndef __CRYPTID_CRYPTID_ABE_H
#define __CRYPTID_CRYPTID_ABE_H
#include "gmp.h"


#include "attribute-based/Defines.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/MasterKey_ABE.h"
#include "util/Random.h"
#include "attribute-based/Polynom.h"
#include "attribute-based/AccessTree.h"
#include "attribute-based/EncryptedMessage_ABE.h"
#include "identity-based/SecurityLevel.h"
#include "util/Status.h"

CryptidStatus cryptid_setup_ABE(const SecurityLevel securityLevel, PublicKey_ABE* publickey, MasterKey_ABE* masterkey);

CryptidStatus compute_tree(AccessTree* accessTree, mpz_t s, PublicKey_ABE* publickey);

CryptidStatus cryptid_encrypt_ABE(EncryptedMessage_ABE* encrypted,
                                const char *const message, const size_t messageLength,
                                    PublicKey_ABE* publickey, AccessTree* accessTree);

CryptidStatus cryptid_keygen_ABE(MasterKey_ABE* masterkey, char** attributes, SecretKey_ABE* secretkey);

CryptidStatus cryptid_delegate_ABE(SecretKey_ABE* secretkey, char** attributes, SecretKey_ABE* secretkey_new);

int Lagrange_coefficient(int xi, int* S, int sLength, int x);

CryptidStatus DecryptNode_ABE(EncryptedMessage_ABE* encrypted, SecretKey_ABE* secretkey, AccessTree* node, Complex* result, int* statusCode);

CryptidStatus cryptid_decrypt_ABE(char **result, EncryptedMessage_ABE* encrypted, SecretKey_ABE* secretkey);

#endif
