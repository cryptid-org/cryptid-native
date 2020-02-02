#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/HashFunction.h"
#include "elliptic/TatePairing.h"

typedef struct PublicKey_ABE
{
	EllipticCurve ellipticCurve; // G0
	AffinePoint g; // generator of cyclic group
	AffinePoint h; // g^(beta)
	AffinePoint f; // g^(1/beta)
	Complex pairValue; // e(g, g)^alpha
} PublicKey_ABE;