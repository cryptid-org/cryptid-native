#include "gmp.h"

typedef struct SecretKey_ABE
{
	AffinePoint D;
	AffinePoint* Dj[MAX_ATTRIBUTES];
	AffinePoint* DjA[MAX_ATTRIBUTES];
	char** attributes[MAX_ATTRIBUTES];
	PublicKey_ABE* pubkey;
} SecretKey_ABE;