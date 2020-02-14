#ifndef __CRYPTID_HASHFUNCTION_H
#define __CRYPTID_HASHFUNCTION_H

#include <stddef.h>

#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "util/Validation.h"


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

CryptidStatus hashFunction_getHashSize(int *hashSizeOutput, const HashFunction hashFunction);

CryptidStatus hashFunction_hash(unsigned char* hashResult, const unsigned char *const message, const size_t messageLength, const HashFunction hashFunction);

CryptidStatus hashFunction_initForSecurityLevel(HashFunction *hashFunctionOutput, const SecurityLevel securityLevel);

/**
 * Validates that the passed HashFunction struct is correct.
 * @param hashFunction the struct to check
 * @result CRYPTID_VALIDATION_SUCCESS if the struct is valid
 */
CryptidValidationResult hashFunction_isValid(const HashFunction hashFunction);

#endif
