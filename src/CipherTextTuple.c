#include <stdlib.h>
#include <string.h>

#include "CipherTextTuple.h"

CipherTextTuple cipherTextTuple_init(AffinePoint cipherU, unsigned char* cipherV, int cipherVLength, unsigned char* cipherW, int cipherWLength)
{    
    CipherTextTuple cipherTextTuple;
    cipherTextTuple.cipherU = affine_init(cipherU.x, cipherU.y);
    
    cipherTextTuple.cipherV = (unsigned char*)malloc(cipherVLength*sizeof(unsigned char) + 1);
    memcpy(cipherTextTuple.cipherV, cipherV, cipherVLength + 1);
    
    cipherTextTuple.cipherVLength = cipherVLength;
    
    cipherTextTuple.cipherW = (unsigned char*)malloc(cipherWLength*sizeof(unsigned char) + 1);
    memcpy(cipherTextTuple.cipherW, cipherW, cipherWLength + 1);
    
    cipherTextTuple.cipherWLength = cipherWLength;

    return cipherTextTuple;
}

void cipherTextTuple_destroy(CipherTextTuple cipherTextTuple)
{
    affine_destroy(cipherTextTuple.cipherU);
    free(cipherTextTuple.cipherV);
    free(cipherTextTuple.cipherW);
}
