#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/HashFunction.h"
#include "elliptic/TatePairing.h"

typedef struct PublicKey_ABE
{
	mpz_t g;
	mpz_t h;
	mpz_t f;
	mpz_t eggalpha;
	Complex theta;
} PublicKey_ABE;