#include "attribute-based/bsw/ciphertext-policy/MasterKey_ABE.h"
#include <stdlib.h>

void destroyMasterKey_ABE(MasterKey_ABE* masterkey)
{
	mpz_clear(masterkey->beta);
    affine_destroy(masterkey->g_alpha);
    free(masterkey);
}