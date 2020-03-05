#ifndef __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#define __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#include "gmp.h"
#include "attribute-based/bsw/ciphertext-policy/Defines.h"
#include "attribute-based/bsw/ciphertext-policy/PublicKey_ABE.h"
#include "elliptic/AffinePoint.h"

typedef struct SecretKey_ABE
{
	AffinePoint D;
	AffinePoint* Dj;
	AffinePoint* DjA;
	char **attributes;
	int num_attributes;
	PublicKey_ABE* pubkey;
} SecretKey_ABE;

void destroySecretKey_ABE(SecretKey_ABE* secretkey);

#endif
