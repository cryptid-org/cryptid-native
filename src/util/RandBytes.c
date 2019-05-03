#include "util/RandBytes.h"


#if defined(_WIN32)

CryptidStatus cryptid_randomBytes(unsigned char *buf, int num)
{
    // TODO Implement Windows secure random generation.
    //      Issue with good-first-issue tag?
    return CRYPTID_RANDOM_GENERATION_ERROR;
}

#elif defined(__CRYPTID_EXTERN_RANDOM)

extern int __cryptid_cryptoRandom(void *buf, const int num);

CryptidStatus cryptid_randomBytes(unsigned char *buf, const int num)
{
    if (!__cryptid_cryptoRandom(buf, num))
    {
        return CRYPTID_SUCCESS;
    }

    return CRYPTID_RANDOM_GENERATION_ERROR;
}

#else

#include <stdio.h>

CryptidStatus cryptid_randomBytes(unsigned char *buf, const int num)
{
    FILE *randomSource = fopen("/dev/urandom", "rb");

    if (!randomSource)
    {
        return CRYPTID_RANDOM_GENERATION_ERROR;
    }

    int byteCount = fread(buf, sizeof(unsigned char), num, randomSource);

    if (byteCount < num)
    {
        return CRYPTID_RANDOM_GENERATION_ERROR;
    }

    fclose(randomSource);

    return CRYPTID_SUCCESS;
}

#endif
