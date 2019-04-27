#ifndef __CRYPTID_HASHFUNCTION_H
#define __CRYPTID_HASHFUNCTION_H

#include <stddef.h>

#include "identity-based/SecurityLevel.h"


// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


/**
 * Struct holding hash function data.
 */
typedef struct HashFunction
{
    /**
     * Pointer to the actual hash function. The function should take three parameters:
     *   * pointer to the message,
     *   * the length of the message,
     *   * pointer to the result buffer.
     * The return value should be the pointer to the result buffer.
     */
    unsigned char* (*sha_hash)(const unsigned char*, size_t, unsigned char*);

    /**
     * Size of the hash function's output.
     */
    size_t hashLength;
} HashFunction;

/**
 * Returns the HashFunction struct corresponding to the specified security level.
 * Please refer to Section 5.1.2. in [RFC-5091] for hash function and security level pairs.
 * @param securityLevel the used security level
 * @return the corresponding HashFunction struct
 */
HashFunction hashFunction_initForSecurityLevel(const SecurityLevel securityLevel);

#endif
