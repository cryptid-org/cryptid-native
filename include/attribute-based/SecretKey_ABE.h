#include "gmp.h"

typedef struct SecretKey_ABE
{
	mpz_t D;
	mpz_t* Dattr[MAX_ATTRIBUTES];
} SecretKey_ABE;