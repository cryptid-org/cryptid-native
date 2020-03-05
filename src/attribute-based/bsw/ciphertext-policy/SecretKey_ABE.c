#include "attribute-based/bsw/ciphertext-policy/SecretKey_ABE.h"
#include <stdlib.h>

void destroySecretKey_ABE(SecretKey_ABE* secretkey)
{
	for(int i = 0; i < secretkey->num_attributes; i++)
    {
    	affine_destroy(secretkey->Dj[i]);
    	affine_destroy(secretkey->DjA[i]);
        free(secretkey->attributes[i]);
    }
    free(secretkey->Dj);
    free(secretkey->DjA);
    free(secretkey->attributes);
    affine_destroy(secretkey->D);
    free(secretkey);
}