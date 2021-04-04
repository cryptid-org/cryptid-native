#include <stdlib.h>
#include <string.h>

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h"

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_init(
    BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        *ciphertextAsBinaryOutput,
    const AffinePointAsBinary cipherU, const void *const cipherV,
    const size_t cipherVLength, const void *const cipherW,
    const size_t cipherWLength) {
  affineAsBinary_init(&ciphertextAsBinaryOutput->cipherU, cipherU.x,
                      cipherU.xLength, cipherU.y, cipherU.yLength);

  ciphertextAsBinaryOutput->cipherV = malloc(cipherVLength + 1);
  memcpy(ciphertextAsBinaryOutput->cipherV, cipherV, cipherVLength + 1);

  ciphertextAsBinaryOutput->cipherVLength = cipherVLength;

  ciphertextAsBinaryOutput->cipherW = malloc(cipherWLength + 1);
  memcpy(ciphertextAsBinaryOutput->cipherW, cipherW, cipherWLength + 1);

  ciphertextAsBinaryOutput->cipherWLength = cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(
    BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        bonehFranklinIdentityBasedEncryptionCiphertextAsBinary) {
  affineAsBinary_destroy(
      bonehFranklinIdentityBasedEncryptionCiphertextAsBinary.cipherU);
  free(bonehFranklinIdentityBasedEncryptionCiphertextAsBinary.cipherV);
  free(bonehFranklinIdentityBasedEncryptionCiphertextAsBinary.cipherW);
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_toBonehFranklinIdentityBasedEncryptionCiphertext(
    BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput,
    const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        ciphertextAsBinary) {
  affineAsBinary_toAffine(&ciphertextOutput->cipherU,
                          ciphertextAsBinary.cipherU);

  ciphertextOutput->cipherV =
        (unsigned char *)malloc(ciphertextAsBinary.cipherVLength + 1);
  memcpy(ciphertextOutput->cipherV, ciphertextAsBinary.cipherV,
         ciphertextAsBinary.cipherVLength + 1);

  ciphertextOutput->cipherVLength = ciphertextAsBinary.cipherVLength;

  ciphertextOutput->cipherW =
        (unsigned char *)malloc(ciphertextAsBinary.cipherWLength + 1);
  memcpy(ciphertextOutput->cipherW, ciphertextAsBinary.cipherW,
         ciphertextAsBinary.cipherWLength + 1);

  ciphertextOutput->cipherWLength = ciphertextAsBinary.cipherWLength;
}

void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_fromBonehFranklinIdentityBasedEncryptionCiphertext(
    BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        *ciphertextAsBinaryOutput,
    const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext) {
  affineAsBinary_fromAffine(&ciphertextAsBinaryOutput->cipherU,
                            ciphertext.cipherU);

  ciphertextAsBinaryOutput->cipherV = malloc(ciphertext.cipherVLength + 1);
  memcpy(ciphertextAsBinaryOutput->cipherV, ciphertext.cipherV,
         ciphertext.cipherVLength + 1);

  ciphertextAsBinaryOutput->cipherVLength = ciphertext.cipherVLength;

  ciphertextAsBinaryOutput->cipherW = malloc(ciphertext.cipherWLength + 1);
  memcpy(ciphertextAsBinaryOutput->cipherW, ciphertext.cipherW,
         ciphertext.cipherWLength + 1);

  ciphertextAsBinaryOutput->cipherWLength = ciphertext.cipherWLength;
}