#ifndef __CRYPTID_CIPHERTEXTTUPLE_H
#define __CRYPTID_CIPHERTEXTTUPLE_H

#include <stddef.h>

#include "elliptic/AffinePoint.h"


typedef struct CipherTextTuple
{
    AffinePoint cipherU;
    unsigned char* cipherV;
    size_t cipherVLength;
    unsigned char* cipherW;
    size_t cipherWLength;
} CipherTextTuple;

CipherTextTuple cipherTextTuple_init(AffinePoint cipherU, unsigned char* cipherV, int cipherVLength, unsigned char* cipherW, int cipherWLength);

void cipherTextTuple_destroy(CipherTextTuple cipherTextTuple);

#endif
