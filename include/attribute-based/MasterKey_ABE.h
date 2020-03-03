#ifndef __CRYPTID_CRYPTID_MASTERKEY_ABE_H
#define __CRYPTID_CRYPTID_MASTERKEY_ABE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "elliptic/TatePairing.h"

#include "attribute-based/PublicKey_ABE.h"
#include "attribute-based/SecretKey_ABE.h"

typedef struct MasterKey_ABE
{
	mpz_t beta;
	AffinePoint g_alpha;
	PublicKey_ABE* pubkey;
} MasterKey_ABE;

void destroyMasterKey_ABE(MasterKey_ABE* masterkey);

#endif
