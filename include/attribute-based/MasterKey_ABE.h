#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/HashFunction.h"
#include "elliptic/TatePairing.h"

#include "attribute-based/PublicKey_ABE.h"

typedef struct MasterKey_ABE
{
	mpz_t beta;
	mpz_t galpha;
	PublicKey_ABE* pubkey;
} MasterKey_ABE;