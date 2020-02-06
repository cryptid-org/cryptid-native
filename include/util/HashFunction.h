#ifndef __CRYPTID_HASHFUNCTION_H
#define __CRYPTID_HASHFUNCTION_H

#include <stddef.h>

#include "identity-based/SecurityLevel.h"
#include "util/Status.h"


// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems

typedef enum HashFunction
{
    hashFunction_SHA1 = 0,
    hashFunction_SHA224 = 1,
    hashFunction_SHA256 = 2,
    hashFunction_SHA384 = 3,
    hashFunction_SHA512 = 4
} HashFunction;

size_t hashFunction_getHashSize(const HashFunction hashFunction);

CryptidStatus hashFunction_hash(const HashFunction hashFunction, const unsigned char *const message, const size_t messageLength, unsigned char* hashResult);

HashFunction hashFunction_initForSecurityLevel(const SecurityLevel securityLevel);

#endif
