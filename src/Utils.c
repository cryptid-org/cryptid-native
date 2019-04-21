#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

void hashToRange(mpz_t result, unsigned char* s, int sLength, mpz_t p, HashFunction hashFunction)
{
    mpz_t v, a, twoFiveSix, twoFiveSixPow, vxTwoFiveSixPow;
    mpz_inits(v, a, twoFiveSix, twoFiveSixPow, vxTwoFiveSixPow, NULL);

    int hashLen = hashFunction.hashLength;

    mpz_set_ui(v, 0);

    unsigned char* h = (unsigned char*)calloc(hashLen + 1, sizeof(unsigned char));
    h[hashLen] = '\0';
    unsigned char* t = (unsigned char*)calloc(hashLen + sLength + 1, sizeof(unsigned char));
    unsigned char* hexString = (unsigned char*)calloc(hashLen * 2 + 1, sizeof(unsigned char));
    unsigned char hex[2];
    for(int i = 1; i < 3; i++)
    {
        for(int i = 0; i < hashLen; i++)
        {
            t[i] = h[i];
        }
        for(int i = 0; i < sLength; i++)
        {
            t[hashLen + i] = s[i];
        }
        t[hashLen + sLength] = '\0';

        (*(hashFunction.sha_hash))(t, hashLen + sLength, h);

        for (int j = 0; j < hashLen; j++)
        {
            sprintf((char *)hex, "%02X", h[j]);
            hexString[j * 2] = hex[0];
            hexString[j * 2 + 1] = hex[1];
        }
        hexString[2 * hashLen] = '\0';

        mpz_set_str(a, (char*) hexString, 16);

        mpz_set_ui(twoFiveSix, 256);
        mpz_pow_ui(twoFiveSixPow, twoFiveSix, hashLen);
        mpz_mul(vxTwoFiveSixPow, v, twoFiveSixPow);

        mpz_clear(v);
        mpz_init(v);
        mpz_add(v, vxTwoFiveSixPow, a);
    }

    mpz_mod(result, v, p);

    free(h);
    free(t);
    free(hexString);
    mpz_clears(v, a, twoFiveSix, twoFiveSixPow, vxTwoFiveSixPow, NULL);
}

Status hashToPoint(AffinePoint *result, EllipticCurve ellipticCurve, mpz_t p, mpz_t q, char* id, int idLength, HashFunction hashFunction)
{
    mpz_t y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq;
    mpz_inits(y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq, NULL);

    hashToRange(y, (unsigned char*) id, idLength, p, hashFunction);

    mpz_pow_ui(yPowTwo, y, 2);
    mpz_sub_ui(yPowTwo, yPowTwo, 1);

    mpz_mul_ui(pxTwo, p, 2);
    mpz_sub_ui(pxTwoSub, pxTwo, 1);
    mpz_cdiv_q_ui(pxTwoSubQ3, pxTwoSub, 3);

    mpz_powm(x, yPowTwo, pxTwoSubQ3, p);

    AffinePoint qPrime = affine_init(x, y);

    mpz_add_ui(pAddOne, p, 1);
    mpz_cdiv_q(pAddOneQq, pAddOne, q);

    Status status = affine_wNAFMultiply(result, pAddOneQq, qPrime, ellipticCurve);
    if(status)
    {
        mpz_clears(y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq, NULL);
        affine_destroy(qPrime);
        return status;
    }

    mpz_clears(y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq, NULL);
    affine_destroy(qPrime);
    return SUCCESS;
}

unsigned char* canonical(int* resultLength, mpz_t p, Complex v, int order)
{
    char* bytes = mpz_get_str(NULL, 16, p);
    int outputSize = strlen(bytes);

    char * realPartHexString = mpz_get_str(NULL, 16, v.real);
    char * imagPartHexString = mpz_get_str(NULL, 16, v.imaginary);

    unsigned char* result = (unsigned char*)calloc(outputSize, sizeof(unsigned char));
    unsigned char* resultHexString = (unsigned char*)calloc(2 * outputSize + 1,sizeof(unsigned char));
    int index = 0;
    if(order == 0)
    {
        for(int i = 0; i < outputSize; i++)
        {
            if(strlen(realPartHexString) + i < outputSize)
            {
                resultHexString[i] = 0;
                index++;
            }
            else
            {
                resultHexString[i] = realPartHexString[i - index];
            }
        }
        index = 0;
        for(int i = 0; i < outputSize; i++)
        {
            if(strlen(imagPartHexString) + i < outputSize)
            {
                resultHexString[outputSize + i] = 0;
                index++;
            }
            else
            {
                resultHexString[outputSize + i] = imagPartHexString[i - index];
            }
        }
    }
    else
    {
        for(int i = 0; i < outputSize; i++)
        {
            if(strlen(imagPartHexString) + i < outputSize)
            {
                resultHexString[i] = 0;
                index++;
            }
            else
            {
                resultHexString[i] = imagPartHexString[i - index];
            }
        }
        index = 0;
        for(int i = 0; i < outputSize; i++)
        {
            if(strlen(realPartHexString) + i < outputSize)
            {
                resultHexString[outputSize + i] = 0;
                index++;
            }
            else
            {
                resultHexString[outputSize + i] = realPartHexString[i - index];
            }
        }
    }
    

    resultHexString[2*outputSize] = '\0';

    unsigned char* pos = resultHexString;
    for(size_t i = 0; i < outputSize; i++)
    {
        sscanf((char*) pos, "%2hhx", result + i);
        pos += 2;
    }
    
    free(bytes);
    free(realPartHexString);
    free(imagPartHexString);
    free(resultHexString);
    *resultLength = outputSize;
    return result;
}

unsigned char* hashBytes(int b, unsigned char* p, int pLength, HashFunction hashFunction)
{
    unsigned char* result = (unsigned char*)calloc(b + 1, sizeof(unsigned char));

    int hashLen = hashFunction.hashLength;

    unsigned char* k = (*(hashFunction.sha_hash))(p, pLength, NULL);

    unsigned char* h = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    for(int i = 0; i < hashLen; i++)
    {
        h[i] = 0;
    }

    int l = (int)ceil((double)b / (double)hashLen);

    int generatedOctets = 0;
    int didGenerateEnough = 0;
    unsigned char* concat = (unsigned char*)calloc(2 * hashLen, sizeof(unsigned char));
    unsigned char* resultPart = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    for(int i = 1; i <= l && !didGenerateEnough; i++)
    {
        (*(hashFunction.sha_hash))(h, hashLen, h);

        for(int j = 0; j < hashLen; j++)
        {
            concat[j] = h[j];
        }
        for(int j = 0; j < hashLen; j++)
        {
            concat[hashLen + j] = k[j];
        }

        (*(hashFunction.sha_hash))(concat, 2 * hashLen, resultPart);
        
        for(int j = 0; j < hashLen; j++)
        {
            if(generatedOctets + j < b)
            {
                result[generatedOctets + j] = resultPart[j];
            }
            else
            {
                didGenerateEnough = 1;
                break;
            }
        }
        generatedOctets += hashLen;
    }

    result[b] = '\0';

    free(h);
    free(concat);
    free(resultPart);
    return result;
}
