#include "attribute-based/PublicKey_ABE.h"
#include <stdlib.h>

void destroyPublicKey_ABE(PublicKey_ABE* publickey) {
	ellipticCurve_destroy(publickey->ellipticCurve);
    affine_destroy(publickey->g);
    affine_destroy(publickey->f);
    affine_destroy(publickey->h);
    mpz_clear(publickey->q);
    complex_destroy(publickey->eggalpha);
    free(publickey);
}