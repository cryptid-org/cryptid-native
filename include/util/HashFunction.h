#ifndef __CRYPTID_HASHFUNCTION_H
#define __CRYPTID_HASHFUNCTION_H

#include <stddef.h>

#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * The biggest value in the HashFunction enum.
 */
#define HASHFUNCTION_MAX_VALUE 4

/**
 * ## Description
 *
 * The currently supported hash functions.
 */
typedef enum HashFunction {
  hashFunction_SHA1 = 0,
  hashFunction_SHA224 = 1,
  hashFunction_SHA256 = 2,
  hashFunction_SHA384 = 3,
  hashFunction_SHA512 = 4
} HashFunction;

/**
 * ## Description
 *
 * Gives the length of the hash in bytes.
 *
 * ## Parameters
 *
 *   * hashSizeOutput
 *     * The length of the hash in bytes.
 *   * hashFunction
 *     * The hash function to which the caller wants to know the result's length.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if the function is called to existing hashFunction, CRPYTID_UNKNOWN_HASH_TYPE_ERROR otherwise.
 */
CryptidStatus hashFunction_getHashSize(int *hashSizeOutput,
                                       const HashFunction hashFunction);

/**
 * ## Description
 *
 * Calls the specified hash function to a string.
 *
 * ## Parameters
 *
 *   * hashResult
 *     * The result of the hash.
 *   * message
 *     * The string which needs to be hashed.
 *   * messageLength
 *     * The length of message.
 *   * hashFunction
 *     * The hashfunction to be called.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus hashFunction_hash(unsigned char *hashResult,
                                const unsigned char *const message,
                                const size_t messageLength,
                                const HashFunction hashFunction);

/**
 * ## Description
 *
 * Initializes a hash function for the specified security level of the system.
 *
 * ## Parameters
 *
 *   * hashFunctionOutput
 *     * The hash function matching the requirements of the security level.
 *   * securityLevel
 *     * The security level of the current system.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus
hashFunction_initForSecurityLevel(HashFunction *hashFunctionOutput,
                                  const SecurityLevel securityLevel);

/**
 * ## Description
 *
 * Validates that the passed HashFunction struct is correct.
 *
 * ## Parameters
 *
 *   * hashFunction
 *     * The struct to check.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the struct is valid.
 */
CryptidValidationResult hashFunction_isValid(const HashFunction hashFunction);

#endif
