#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsString.h"

void bonehFranklinIdentityBasedEncryptionCiphertextAsString_init(BonehFranklinIdentityBasedEncryptionCiphertextAsString *ciphertextAsStringOutput, const AffinePointAsString cipherU, const unsigned char *const cipherV, const int cipherVLength, const unsigned char *const cipherW, const int cipherWLength)
{
    affineAsString_init(&ciphertextAsStringOutput->cipherU, cipherU.x, cipherU.xBase, cipherU.y, cipherU.yBase);

    ciphertextAsStringOutput->cipherV = (unsigned char*)malloc(cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsStringOutput->cipherV, cipherV, cipherVLength + 1);

    ciphertextAsStringOutput->cipherVLength = cipherVLength;

    ciphertextAsStringOutput->cipherW = (unsigned char*)malloc(cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsStringOutput->cipherW, cipherW, cipherWLength + 1);

    ciphertextAsStringOutput->cipherWLength = cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsString_destroy(BonehFranklinIdentityBasedEncryptionCiphertextAsString bonehFranklinIdentityBasedEncryptionCiphertextAsString)
{
    affineAsString_destroy(bonehFranklinIdentityBasedEncryptionCiphertextAsString.cipherU);
    free(bonehFranklinIdentityBasedEncryptionCiphertextAsString.cipherV);
    free(bonehFranklinIdentityBasedEncryptionCiphertextAsString.cipherW);
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsString_toBonehFranklinIdentityBasedEncryptionCiphertext(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const BonehFranklinIdentityBasedEncryptionCiphertextAsString ciphertextAsString)
{
    affineAsString_toAffine(&ciphertextOutput->cipherU, ciphertextAsString.cipherU);
    
    ciphertextOutput->cipherV = (unsigned char*)malloc(ciphertextAsString.cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextOutput->cipherV, ciphertextAsString.cipherV, ciphertextAsString.cipherVLength + 1);
    
    ciphertextOutput->cipherVLength = ciphertextAsString.cipherVLength;
    
    ciphertextOutput->cipherW = (unsigned char*)malloc(ciphertextAsString.cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextOutput->cipherW, ciphertextAsString.cipherW, ciphertextAsString.cipherWLength + 1);
    
    ciphertextOutput->cipherWLength = ciphertextAsString.cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertext_toBonehFranklinIdentityBasedEncryptionCiphertextAsString(BonehFranklinIdentityBasedEncryptionCiphertextAsString *ciphertextAsStringOutput, const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext, const int cipherUXBase, const int cipherUYBase)
{
    affine_toAffineAsString(&ciphertextAsStringOutput->cipherU, ciphertext.cipherU, cipherUXBase, cipherUYBase);

    ciphertextAsStringOutput->cipherV = (unsigned char*)malloc(ciphertext.cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsStringOutput->cipherV, ciphertext.cipherV, ciphertext.cipherVLength + 1);

    ciphertextAsStringOutput->cipherVLength = ciphertext.cipherVLength;

    ciphertextAsStringOutput->cipherW = (unsigned char*)malloc(ciphertext.cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsStringOutput->cipherW, ciphertext.cipherW, ciphertext.cipherWLength + 1);

    ciphertextAsStringOutput->cipherWLength = ciphertext.cipherWLength;
}