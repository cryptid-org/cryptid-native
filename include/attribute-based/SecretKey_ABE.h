#ifndef __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#define __CRYPTID_CRYPTID_SECRETKEY_ABE_H
#include "gmp.h"
#include "attribute-based/Defines.h"
#include "attribute-based/PublicKey_ABE.h"
#include "elliptic/AffinePoint.h"

typedef struct SecretKey_ABE
{
	AffinePoint D;
	AffinePoint Dj[MAX_ATTRIBUTES];
	AffinePoint DjA[MAX_ATTRIBUTES];
	char *attributes[MAX_ATTRIBUTES];
	PublicKey_ABE* pubkey;
} SecretKey_ABE;

void destroySecretKey_ABE(SecretKey_ABE* secretkey);

#endif
