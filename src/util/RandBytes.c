#include "util/RandBytes.h"


#if defined(_WIN32)

Status randomBytes(unsigned char *buf, int num)
{
    // TODO Implement Windows secure random generation.
    //      Issue with good-first-issue tag?
    return RANDOM_GENERATION_ERROR;
}

#elif defined(__CRYPTID_EXTERN_RANDOM)

extern int cryptoRandom(void *buf, int num);

Status randomBytes(unsigned char *buf, int num)
{
    if (!cryptoRandom(buf, num))
    {
        return SUCCESS;
    }

    return RANDOM_GENERATION_ERROR;
}

#else

#include <stdio.h>

Status randomBytes(unsigned char *buf, int num)
{
    FILE *randomSource = fopen("/dev/urandom", "rb");

    if (!randomSource)
    {
        return RANDOM_GENERATION_ERROR;
    }

    int byteCount = fread(buf, sizeof(unsigned char), num, randomSource);

    if (byteCount < num)
    {
        return RANDOM_GENERATION_ERROR;
    }

    fclose(randomSource);

    return SUCCESS;
}

#endif
