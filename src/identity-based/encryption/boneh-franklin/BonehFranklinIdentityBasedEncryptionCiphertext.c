#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h"


void bonehFranklinIdentityBasedEncryptionCiphertext_init(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const AffinePoint cipherU, const unsigned char *const cipherV, const int cipherVLength,
                                     const unsigned char *const cipherW, const int cipherWLength)
{
    affine_init(&ciphertextOutput->cipherU, cipherU.x, cipherU.y);
    
    ciphertextOutput->cipherV = (unsigned char*)malloc(cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextOutput->cipherV, cipherV, cipherVLength + 1);
    
    ciphertextOutput->cipherVLength = cipherVLength;
    
    ciphertextOutput->cipherW = (unsigned char*)malloc(cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextOutput->cipherW, cipherW, cipherWLength + 1);
    
    ciphertextOutput->cipherWLength = cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertext_destroy(BonehFranklinIdentityBasedEncryptionCiphertext ciphertext)
{
    affine_destroy(ciphertext.cipherU);
    free(ciphertext.cipherV);
    free(ciphertext.cipherW);
}
