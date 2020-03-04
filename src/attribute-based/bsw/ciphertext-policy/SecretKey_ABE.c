#include "attribute-based/bsw/ciphertext-policy/SecretKey_ABE.h"
#include <stdlib.h>

void destroySecretKey_ABE(SecretKey_ABE* secretkey)
{
	for(int i = 0; i < MAX_ATTRIBUTES; i++)
    {
    	if(secretkey->attributes[i] && secretkey->attributes[i][0] != '\0')
        {
        	affine_destroy(secretkey->Dj[i]);
        	affine_destroy(secretkey->DjA[i]);
            free(secretkey->attributes[i]);
        }
        else if(secretkey->attributes[i])
        {
            free(secretkey->attributes[i]);
        }
    }
    affine_destroy(secretkey->D);
    free(secretkey);
}