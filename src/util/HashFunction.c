#include "sha.h"

#include "util/HashFunction.h"

CryptidStatus hashFunction_getHashSize(int *hashSizeOutput, const HashFunction hashFunction)
{
    switch(hashFunction)
    {
        case hashFunction_SHA1:
            *hashSizeOutput = 20;
            break;
        case hashFunction_SHA224:
            *hashSizeOutput = 28;
            break;
        case hashFunction_SHA256:
            *hashSizeOutput = 32;
            break;
        case hashFunction_SHA384:
            *hashSizeOutput = 48;
            break;
        case hashFunction_SHA512:
            *hashSizeOutput = 64;
            break;
        default:
            return CRPYTID_UNKNOWN_HASH_TYPE_ERROR;
    }

    return CRYPTID_SUCCESS;
}

CryptidStatus hashFunction_hash(unsigned char* hashResult, const unsigned char *const message, const size_t messageLength, const HashFunction hashFunction)
{
    if(hashResult == NULL)
    {
        return CRYPTID_HASH_NULLPOINTER_OUTPUT_PARAM_ERROR;
    }

    switch(hashFunction)
    {
        case hashFunction_SHA1:
            SHA1_OneCall(message, messageLength, hashResult);
            break;
        case hashFunction_SHA224:
            SHA224_OneCall(message, messageLength, hashResult);
            break;
        case hashFunction_SHA256:
            SHA256_OneCall(message, messageLength, hashResult);
            break;
        case hashFunction_SHA384:
            SHA384_OneCall(message, messageLength, hashResult);
            break;
        case hashFunction_SHA512:
            SHA512_OneCall(message, messageLength,hashResult);
            break;
        default:
            return CRPYTID_UNKNOWN_HASH_TYPE_ERROR;
    }

    return CRYPTID_SUCCESS;
}

CryptidStatus hashFunction_initForSecurityLevel(HashFunction *hashFunctionOutput, const SecurityLevel securityLevel)
{
    switch(securityLevel)
    {
        case LOWEST:
            *hashFunctionOutput = hashFunction_SHA1;
            break;
        case LOW:
            *hashFunctionOutput = hashFunction_SHA224;
            break;
        case MEDIUM:
            *hashFunctionOutput = hashFunction_SHA256;
            break;
        case HIGH:
            *hashFunctionOutput = hashFunction_SHA384;
            break;
        case HIGHEST:
            *hashFunctionOutput = hashFunction_SHA512;
            break;
        default:
            return CRPYTID_UNKNOWN_HASH_TYPE_ERROR;
    }

    return CRYPTID_SUCCESS;
}