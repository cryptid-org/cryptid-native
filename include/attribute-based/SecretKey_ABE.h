#include "gmp.h"

typedef struct SecretKey_ABE
{
	AffinePoint D;
	AffinePoint* Dj[MAX_ATTRIBUTES];
	AffinePoint* DjA[MAX_ATTRIBUTES];
} SecretKey_ABE;