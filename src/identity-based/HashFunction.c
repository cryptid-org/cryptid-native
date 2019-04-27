#include "sha.h"

#include "identity-based/HashFunction.h"


static const int SHA_TYPE_MAPPING[] = { 1, 224, 256, 384, 512 };

static HashFunction hashFunction_init(const int shaType)
{
    HashFunction hashFunction;
    switch(shaType)
    {
        case 1:
        {
            hashFunction.sha_hash = &SHA1_OneCall;
            hashFunction.hashLength = 20;
            break;
        }
        case 224:
        {
            hashFunction.sha_hash = &SHA224_OneCall;
            hashFunction.hashLength = 28;
            break;
        }
        case 256:
        {
            hashFunction.sha_hash = &SHA256_OneCall;
            hashFunction.hashLength = 32;
            break;
        }
        case 384:
        {
            hashFunction.sha_hash = &SHA384_OneCall;
            hashFunction.hashLength = 48;
            break;
        }
        case 512:
        {
            hashFunction.sha_hash = &SHA512_OneCall;
            hashFunction.hashLength = 64;
            break;
        }
        default :
        {
            hashFunction.sha_hash = &SHA256_OneCall;
            hashFunction.hashLength = 32;
            break;
        }
    }

    return hashFunction;
}

HashFunction hashFunction_initForSecurityLevel(const SecurityLevel securityLevel)
{
    return hashFunction_init(SHA_TYPE_MAPPING[(int) securityLevel]);
}

