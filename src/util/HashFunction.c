#include "sha.h"

#include "util/HashFunction.h"

size_t hashFunction_getHashSize(const HashFunction hashFunction)
{
    switch(hashFunction)
    {
        case hashFunction_SHA1: return 20;
        case hashFunction_SHA224: return 28;
        case hashFunction_SHA256: return 32;
        case hashFunction_SHA384: return 48;
        case hashFunction_SHA512: return 64;
        default: return 0;
    }
}

CryptidStatus hashFunction_hash(const HashFunction hashFunction, const unsigned char *const message, const size_t messageLength, unsigned char* hashResult)
{
    if(hashResult == NULL)
    {
        return HASH_NULLPOINTER_OUTPUT_PARAM_ERROR;
    }

    switch(hashFunction)
    {
        case hashFunction_SHA1:
            SHA1_OneCall(message, messageLength, hashResult);
            return CRYPTID_SUCCESS;
        case hashFunction_SHA224:
            SHA224_OneCall(message, messageLength, hashResult);
            return CRYPTID_SUCCESS;
        case hashFunction_SHA256:
            SHA256_OneCall(message, messageLength, hashResult);
            return CRYPTID_SUCCESS;
        case hashFunction_SHA384:
            SHA384_OneCall(message, messageLength, hashResult);
            return CRYPTID_SUCCESS;
        case hashFunction_SHA512:
            SHA512_OneCall(message, messageLength,hashResult);
            return CRYPTID_SUCCESS;
        default:
            return HASH_UNKNOWN_TYPE_ERROR;
    }
}

HashFunction hashFunction_initForSecurityLevel(const SecurityLevel securityLevel)
{
    switch(securityLevel)
    {
        case LOWEST: return hashFunction_SHA1;
        case LOW: return hashFunction_SHA224;
        case MEDIUM: return hashFunction_SHA256;
        case HIGH: return hashFunction_SHA384;
        case HIGHEST: return hashFunction_SHA512;
        default: return -1;
    }
}