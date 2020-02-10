#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h"

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_init(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertextAsBinaryOutput, const AffinePointAsBinary cipherU, const unsigned char *const cipherV, const size_t cipherVLength, const unsigned char *const cipherW, const size_t cipherWLength)
{
    affineAsBinary_init(&ciphertextAsBinaryOutput->cipherU, cipherU.x, cipherU.xLength, cipherU.y, cipherU.yLength);

    ciphertextAsBinaryOutput->cipherV = (unsigned char*)malloc(cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsBinaryOutput->cipherV, cipherV, cipherVLength + 1);

    ciphertextAsBinaryOutput->cipherVLength = cipherVLength;

    ciphertextAsBinaryOutput->cipherW = (unsigned char*)malloc(cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsBinaryOutput->cipherW, cipherW, cipherWLength + 1);

    ciphertextAsBinaryOutput->cipherWLength = cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary bonehFranklinIdentityBasedEncryptionCiphertextAsBinary)
{
    affineAsBinary_destroy(bonehFranklinIdentityBasedEncryptionCiphertextAsBinary.cipherU);
    free(bonehFranklinIdentityBasedEncryptionCiphertextAsBinary.cipherV);
    free(bonehFranklinIdentityBasedEncryptionCiphertextAsBinary.cipherW);
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_toBonehFranklinIdentityBasedEncryptionCiphertext(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertextAsBinary)
{
    affineAsBinary_toAffine(&ciphertextOutput->cipherU, ciphertextAsBinary.cipherU);
    
    ciphertextOutput->cipherV = (unsigned char*)malloc(ciphertextAsBinary.cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextOutput->cipherV, ciphertextAsBinary.cipherV, ciphertextAsBinary.cipherVLength + 1);
    
    ciphertextOutput->cipherVLength = ciphertextAsBinary.cipherVLength;
    
    ciphertextOutput->cipherW = (unsigned char*)malloc(ciphertextAsBinary.cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextOutput->cipherW, ciphertextAsBinary.cipherW, ciphertextAsBinary.cipherWLength + 1);
    
    ciphertextOutput->cipherWLength = ciphertextAsBinary.cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertext_toBonehFranklinIdentityBasedEncryptionCiphertextAsBinary(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertextAsBinaryOutput, const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext)
{
    affine_toAffineAsBinary(&ciphertextAsBinaryOutput->cipherU, ciphertext.cipherU);

    ciphertextAsBinaryOutput->cipherV = (unsigned char*)malloc(ciphertext.cipherVLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsBinaryOutput->cipherV, ciphertext.cipherV, ciphertext.cipherVLength + 1);

    ciphertextAsBinaryOutput->cipherVLength = ciphertext.cipherVLength;

    ciphertextAsBinaryOutput->cipherW = (unsigned char*)malloc(ciphertext.cipherWLength * sizeof(unsigned char) + 1);
    memcpy(ciphertextAsBinaryOutput->cipherW, ciphertext.cipherW, ciphertext.cipherWLength + 1);

    ciphertextAsBinaryOutput->cipherWLength = ciphertext.cipherWLength;
}