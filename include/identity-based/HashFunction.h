#ifndef __CRYPTID_HASHFUNCTION_H
#define __CRYPTID_HASHFUNCTION_H

#include <stddef.h>

#include "identity-based/SecurityLevel.h"


typedef struct HashFunction
{
    unsigned char* (*sha_hash)(const unsigned char*, size_t, unsigned char*);
    size_t hashLength;
} HashFunction;

HashFunction hashFunction_initForSecurityLevel(SecurityLevel securityLevel);

HashFunction hashFunction_init(int shaType);

int hashFunction_isValid(HashFunction hashFunction);

#endif