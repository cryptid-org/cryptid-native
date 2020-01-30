#include "sha.h"

#include "identity-based/HashFunction.h"

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

unsigned char* hashFunction_hash(const HashFunction hashFunction, const unsigned char* message, const size_t messageLength)
{
    switch(hashFunction)
    {
        case hashFunction_SHA1: return SHA1_OneCall(message, messageLength);
        case hashFunction_SHA224: return SHA224_OneCall(message, messageLength);
        case hashFunction_SHA256: return SHA256_OneCall(message, messageLength);
        case hashFunction_SHA384: return SHA384_OneCall(message, messageLength);
        case hashFunction_SHA512: return SHA512_OneCall(message, messageLength);
        default: return NULL;
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