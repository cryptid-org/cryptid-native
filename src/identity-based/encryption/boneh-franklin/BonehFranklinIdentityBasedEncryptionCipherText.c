#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCipherText.h"


BonehFranklinIdentityBasedEncryptionCipherText bonehFranklinIdentityBasedEncryptionCipherText_init(const AffinePoint cipherU, const unsigned char *const cipherV, const int cipherVLength,
                                     const unsigned char *const cipherW, const int cipherWLength)
{
    BonehFranklinIdentityBasedEncryptionCipherText bonehFranklinIdentityBasedEncryptionCipherText;
    bonehFranklinIdentityBasedEncryptionCipherText.cipherU = affine_init(cipherU.x, cipherU.y);
    
    bonehFranklinIdentityBasedEncryptionCipherText.cipherV = (unsigned char*)malloc(cipherVLength * sizeof(unsigned char) + 1);
    memcpy(bonehFranklinIdentityBasedEncryptionCipherText.cipherV, cipherV, cipherVLength + 1);
    
    bonehFranklinIdentityBasedEncryptionCipherText.cipherVLength = cipherVLength;
    
    bonehFranklinIdentityBasedEncryptionCipherText.cipherW = (unsigned char*)malloc(cipherWLength * sizeof(unsigned char) + 1);
    memcpy(bonehFranklinIdentityBasedEncryptionCipherText.cipherW, cipherW, cipherWLength + 1);
    
    bonehFranklinIdentityBasedEncryptionCipherText.cipherWLength = cipherWLength;

    return bonehFranklinIdentityBasedEncryptionCipherText;
}

void bonehFranklinIdentityBasedEncryptionCipherText_destroy(BonehFranklinIdentityBasedEncryptionCipherText bonehFranklinIdentityBasedEncryptionCipherText)
{
    affine_destroy(bonehFranklinIdentityBasedEncryptionCipherText.cipherU);
    free(bonehFranklinIdentityBasedEncryptionCipherText.cipherV);
    free(bonehFranklinIdentityBasedEncryptionCipherText.cipherW);
}
