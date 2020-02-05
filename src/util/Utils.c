#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util/Utils.h"


// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


void hashToRange(mpz_t result, const unsigned char *const s, const int sLength, const mpz_t p, const HashFunction hashFunction)
{
    // Implementation of Algorithm 4.1.1 (HashToRange) in [RFC-5091].

    mpz_t v, a, twoFiveSix, twoFiveSixPow, vxTwoFiveSixPow;
    mpz_inits(v, a, twoFiveSix, twoFiveSixPow, vxTwoFiveSixPow, NULL);

    // Let {@code hashlen} be the number of octets comprising the output of {@code hashfcn}.
    int hashLen = hashFunction_getHashSize(hashFunction);

    // Let \f$v_{0} = 0\f$.
    mpz_set_ui(v, 0);

    // Let \f$h_{0} = 00...00\f$, a string of null octets with a length of {@code hashlen}.
    unsigned char* h = (unsigned char*)calloc(hashLen + 1, sizeof(unsigned char));
    h[hashLen] = '\0';
    unsigned char* t = (unsigned char*)calloc(hashLen + sLength + 1, sizeof(unsigned char));
    unsigned char* hexString = (unsigned char*)calloc(hashLen * 2 + 1, sizeof(unsigned char));
    unsigned char hex[3] = "\0\0\0";

    // {@code For i = 1 to 2, do:}
    for(int i = 1; i < 3; i++)
    {
        // Let \f$t_{i} = h_{(i - 1)} || s\f$, which is the \f$(|s| + {mathrm{hashlen})\f$-octet
        // string concatenation of the strings \f$h_{(i - 1)} and s\f$.
        for(int i = 0; i < hashLen; i++)
        {
            t[i] = h[i];
        }
        for(int i = 0; i < sLength; i++)
        {
            t[hashLen + i] = s[i];
        }
        t[hashLen + sLength] = '\0';

        // Let \f$h_{i} = \mathrm{hashfcn}(t_i)\f$, which is a {@code hashlen}-octet string
        // resulting from the hash algorithm {@code hashfcn} on the input \f$t_i\f$.
        hashFunction_hash(hashFunction, t, hashLen + sLength, h);

        // Let \f$a_i = \mathrm{Value}(h_i)\f$ be the integer in the range \f$0\f$ to
        // \f$256^{\mathrm{hashlen}} - 1\f$ denoted by the raw octet string \f$h_i\f$
        // interpreted in the unsigned big-endian convention.
        for (int j = 0; j < hashLen; j++)
        {
            sprintf((char *)hex, "%02X", h[j]);
            hexString[j * 2] = hex[0];
            hexString[j * 2 + 1] = hex[1];
        }
        hexString[2 * hashLen] = '\0';

        mpz_set_str(a, (char*) hexString, 16);

        // Let \f$v_i = 256^{\mathrm{hashlen}} \cdot v_{(i - 1)} + a_i\f$.
        mpz_set_ui(twoFiveSix, 256);
        mpz_pow_ui(twoFiveSixPow, twoFiveSix, hashLen);
        mpz_mul(vxTwoFiveSixPow, v, twoFiveSixPow);

        mpz_clear(v);
        mpz_init(v);
        mpz_add(v, vxTwoFiveSixPow, a);
    }

    // Let \f$v = v_l \mod n\f$.
    mpz_mod(result, v, p);

    free(h);
    free(t);
    free(hexString);
    mpz_clears(v, a, twoFiveSix, twoFiveSixPow, vxTwoFiveSixPow, NULL);
}

CryptidStatus hashToPoint(AffinePoint *result, const EllipticCurve ellipticCurve, const mpz_t p, const mpz_t q,
                   const char *const id, const int idLength, const HashFunction hashFunction)
{
    // Implementation of Algorithm 4.4.2 (HashToPoint1) in [RFC-5091].

    mpz_t y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq;
    mpz_inits(y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq, NULL);

    // Let \f$y = \mathrm{HashToRange}(id, p, \mathrm{hashfcn})\f$, using {@code HashToRange}, an element of \f$F_p\f$.
    hashToRange(y, (unsigned char*) id, idLength, p, hashFunction);

    // Let \f$x = (y^2 - 1)^{\frac{2 \cdot p - 1}{3}} \mod p\f$, an element of \f$F_p\f$.
    mpz_pow_ui(yPowTwo, y, 2);
    mpz_sub_ui(yPowTwo, yPowTwo, 1);

    mpz_mul_ui(pxTwo, p, 2);
    mpz_sub_ui(pxTwoSub, pxTwo, 1);
    mpz_cdiv_q_ui(pxTwoSubQ3, pxTwoSub, 3);

    mpz_powm(x, yPowTwo, pxTwoSubQ3, p);

    // Let \f$Q^{\prime} = (x, y)\f$, a non-zero point in \f$E(F_p)\f$.
    AffinePoint qPrime = affine_init(x, y);

    mpz_add_ui(pAddOne, p, 1);
    mpz_cdiv_q(pAddOneQq, pAddOne, q);

    // Let \f$Q = [(p + 1) / q ]Q^{\prime}\f$, a point of order \f$q\f$ in \f$E(F_p)\f$.
    CryptidStatus status = AFFINE_MULTIPLY_IMPL(result, pAddOneQq, qPrime, ellipticCurve);
    if(status)
    {
        mpz_clears(y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq, NULL);
        affine_destroy(qPrime);
        return status;
    }

    mpz_clears(y, x, pxTwo, pxTwoSub, pxTwoSubQ3, yPowTwo, yPowTwoSub, pAddOne, pAddOneQq, NULL);
    affine_destroy(qPrime);
    return CRYPTID_SUCCESS;
}

unsigned char* canonical(int *const resultLength, const mpz_t p, const Complex v, const int order)
{
    // Implementation of Algorithm 4.3.2 (Canonical1) in [RFC-5091].

    // Let \f$l = \mathrm{Ceiling}(\frac{\log(p)}{8})\f$, the number of octets needed to represent integers in \f$Z_p\f$.
    char* bytes = mpz_get_str(NULL, 16, p);
    size_t outputSize = strlen(bytes);

    // Let \f$v = a + b \cdot i\f$, where \f$i^2 = -1\f$.
    // Let \f$a_{256^l}\f$ be the big-endian zero-padded fixed-length octet string representation of \f$a\f$ in \f$Z_p\f$.
    // Let \f$b_{256^l}\f$ be the big-endian zero-padded fixed-length octet string representation of \f$b\f$ in \f$Z_p\f$.
    char * realPartHexString = mpz_get_str(NULL, 16, v.real);
    char * imagPartHexString = mpz_get_str(NULL, 16, v.imaginary);

    unsigned char* result = (unsigned char*)calloc(outputSize, sizeof(unsigned char));
    unsigned char* resultHexString = (unsigned char*)calloc(2 * outputSize + 1,sizeof(unsigned char));
    int index = 0;

    // If the {@code order} is {@code 0}, then let \f$s = a_{256^l} || b_{256^l}\f$,
    // which is the concatenation of \f$a_{256^l}\f$ followed by \f$b_{256^l}\f$.
    if(order == 0)
    {
        for(size_t i = 0; i < outputSize; i++)
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
        for(size_t i = 0; i < outputSize; i++)
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
    // If the {@code order} is {@code 1}, then let \f$s = b_{256^l} || a_{256^l}\f$,
    // which is the concatenation of \f$a_{256^l}\f$ followed by \f$b_{256^l}\f$.
    else
    {
        for(size_t i = 0; i < outputSize; i++)
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
        for(size_t i = 0; i < outputSize; i++)
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

unsigned char* hashBytes(const int b, const unsigned char *const p, const int pLength, const HashFunction hashFunction)
{
    // Implementation of Algorithm 4.2.1 (HashBytes) in [RFC-5091].

    unsigned char* result = (unsigned char*)calloc(b + 1, sizeof(unsigned char));

    // Let {@code hashlen{} be the number of octets comprising the output of {@code hashfcn}.
    int hashLen = hashFunction_getHashSize(hashFunction);

    // Let \f$k = \mathrm{hashfcn}(p)\f$.
    unsigned char* k = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    hashFunction_hash(hashFunction, p, pLength, k);

    // Let \f$h_0 = 00...00\f$, a string of null octets with a length of {@code hashlen}.
    unsigned char* h = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    for(int i = 0; i < hashLen; i++)
    {
        h[i] = 0;
    }

    // Let \f$l = \mathrm{Ceiling}(\frac{b}{\mathrm{hashlen}}).
    int l = (int)ceil((double)b / (double)hashLen);

    int generatedOctets = 0;
    int didGenerateEnough = 0;
    unsigned char* concat = (unsigned char*)calloc(2 * hashLen, sizeof(unsigned char));
    unsigned char* resultPart = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    // {@code For each i in 1 to l, do:}
    for(int i = 1; i <= l && !didGenerateEnough; i++)
    {
        // Let \f$h_i = \mathrm{hashfcn}(h_{i - 1}).
        hashFunction_hash(hashFunction, h, hashLen, h);

        // Let \f$r_i = \mathrm{hashfcn}(h_i || k)\f$, where \f$h_i || k\f$ is the
        // \f$(2 \cdot \mathrm{hashlen})\f$-octet concatenation of \f$h_i\f$ and \f$k\f$.
        for(int j = 0; j < hashLen; j++)
        {
            concat[j] = h[j];
        }
        for(int j = 0; j < hashLen; j++)
        {
            concat[hashLen + j] = k[j];
        }

        hashFunction_hash(hashFunction, concat, 2 * hashLen, resultPart);

        // Let \f$r = \mathrm{LeftmostOctets}(b, r_1 || ... || r_l)\f$, i.e., \f$r\f$ is formed as
        // the concatenation of the \f$r_i\f$, truncated to the desired number of
        // octets.
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

    free(k);
    free(h);
    free(concat);
    free(resultPart);
    return result;
}

const int BASE = 10;

void writePublicParToFiles(PublicParameters* publicParameters)
{
    FILE * fp;
    struct stat st = {0};

    if (stat("./PP", &st) == -1) {
        mkdir("./PP", 0700);
    }

    fp = fopen ("PP/PP.ellipticCurve.a","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->ellipticCurve.a);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.b","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->ellipticCurve.b);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.fieldOrder","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->ellipticCurve.fieldOrder);
        fclose(fp);
    }

    fp = fopen ("PP/PP.q","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->q);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointP.x","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointP.x);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointP.y","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointP.y);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointPpublic.x","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointPpublic.x);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointPpublic.y","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointPpublic.y);
        fclose(fp);
    }

    fp = fopen ("PP/PP.hashf","wb");
    if (fp != NULL) {

        fwrite(&publicParameters->hashFunction, sizeof(publicParameters->hashFunction), 1, fp);

        fclose(fp);
    }

}

void writeCipherTextToFiles(CipherTextTuple* ciphertext)
{
    FILE * fp;
    struct stat st = {0};

    if (stat("./CT", &st) == -1) {
        mkdir("./CT", 0700);
    }


    fp = fopen ("CT/cipher","wb");
    if (fp != NULL) {

        //fwrite(&ciphertext->cipherU, sizeof(ciphertext->cipherU), 1, fp);

        fwrite(&ciphertext->cipherVLength, sizeof(ciphertext->cipherVLength), 1, fp);
        fwrite(ciphertext->cipherV, ciphertext->cipherVLength + 1, 1, fp);

        fwrite(&ciphertext->cipherWLength, sizeof(ciphertext->cipherWLength),1, fp);
        fwrite(ciphertext->cipherW, ciphertext->cipherWLength + 1, 1, fp);

        fclose(fp);
    }
    printf("beleirt:\n");
    printf("VLength: %lu\n", ciphertext->cipherVLength);
    printf("%s\n", ciphertext->cipherV);
    printf("%lu\n", ciphertext->cipherWLength);
    printf("%s\n", ciphertext->cipherW);

    fp = fopen ("CT/cipherUx","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, ciphertext->cipherU.x);
        fclose(fp);
    }
    printf ("  wrote  "); mpz_out_str (stdout, BASE, ciphertext->cipherU.x); printf("\n");

    fp = fopen ("CT/cipherUy","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, ciphertext->cipherU.y);
        fclose(fp);
    }
    printf ("  wrote  "); mpz_out_str (stdout, BASE, ciphertext->cipherU.y); printf("\n");
}

void writePrivateKeyToFiles(AffinePoint privateKey)
{
    FILE * fp;
    struct stat st = {0};

    if (stat("./PK", &st) == -1) {
        mkdir("./PK", 0700);
    }


    fp = fopen ("PK/privateX","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, privateKey.x);
        fclose(fp);
    }

    fp = fopen ("PK/privateY","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, privateKey.y);
        fclose(fp);
    }
}

PublicParameters readPublicParFromFile()
{
    FILE * fp;

    PublicParameters publicParameters;

    mpz_t a, b, fieldOrder, q, px, py, ppx, ppy;
    mpz_inits(a, b, fieldOrder, q, px, py, ppx, ppy, NULL);

    fp = fopen ("PP/PP.ellipticCurve.a","r");
    if (fp != NULL)
    {
        mpz_inp_str(a, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.b","r");
    if (fp != NULL)
    {
        mpz_inp_str(b, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.fieldOrder","r");
    if (fp != NULL)
    {
        mpz_inp_str(fieldOrder, fp, BASE);
        fclose(fp);
    }
    publicParameters.ellipticCurve = ellipticCurve_init(a, b, fieldOrder);

    fp = fopen ("PP/PP.q","r");
    if (fp != NULL)
    {
        mpz_inp_str(q, fp, BASE);
        fclose(fp);
    }
    mpz_init(publicParameters.q);
    mpz_set(publicParameters.q, q);

    fp = fopen ("PP/PP.pointP.x","r");
    if (fp != NULL)
    {
        mpz_inp_str(px, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointP.y","r");
    if (fp != NULL)
    {
        mpz_inp_str(py, fp, BASE);
        fclose(fp);
    }
    publicParameters.pointP = affine_init(px, py);

    fp = fopen ("PP/PP.pointPpublic.x","r");
    if (fp != NULL)
    {
        mpz_inp_str(ppx, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointPpublic.y","r");
    if (fp != NULL)
    {
        mpz_inp_str(ppy, fp, BASE);
        fclose(fp);
    }
    publicParameters.pointPpublic = affine_init(ppx, ppy);

    fp = fopen ("PP/PP.hashf","r");
    if (fp != NULL)
    {
        int i = 0;
        fread(&i, sizeof(int), 1, fp);
        publicParameters.hashFunction = (HashFunction) i;
        fclose(fp);
    }

    return publicParameters;
}

AffinePoint readPrivateKeyFromFiles()
{
    AffinePoint privateKey;
    mpz_t x, y;
    mpz_inits(x, y, NULL);

    FILE * fp;
    fp = fopen ("PK/privateX","r");
    if (fp != NULL)
    {
        mpz_inp_str(x, fp, BASE);
        fclose(fp);
    }
    else printf("Can't open file");

    fp = fopen ("PK/privateY","r");
    if (fp != NULL)
    {
        mpz_inp_str(y, fp, BASE);
        fclose(fp);
    }
    else printf("Can't open file");

    privateKey = affine_init(x, y);

    printf ("privateY  read in  "); mpz_out_str (stdout, BASE, privateKey.y); printf("\n");
    printf ("privateX  read in  "); mpz_out_str (stdout, BASE, privateKey.x); printf("\n");

    return privateKey;
}

CipherTextTuple readCipherTextFromFile()
{
    CipherTextTuple ciphertext;

    FILE * fp;

    fp = fopen("CT/cipher", "rb");
    if (fp != NULL) {

        //fread(&ciphertext.cipherU, sizeof(ciphertext.cipherU), 1, fp);
        fread(&ciphertext.cipherVLength, sizeof(ciphertext.cipherVLength), 1, fp);

	ciphertext.cipherV = malloc(ciphertext.cipherVLength + 1);
        fread(ciphertext.cipherV, ciphertext.cipherVLength + 1, 1, fp);
        fread(&ciphertext.cipherWLength, sizeof(ciphertext.cipherWLength), 1, fp);

	ciphertext.cipherW = malloc(ciphertext.cipherWLength + 1);
        fread(ciphertext.cipherW, ciphertext.cipherWLength + 1, 1, fp);

        fclose(fp);
    }

    else printf("Can't open file");
    printf("kiolvasott:\n");
    printf("VLength: %lu\n", ciphertext.cipherVLength);
    printf("%s\n", ciphertext.cipherV);
    printf("%lu\n", ciphertext.cipherWLength);
    printf("%s\n", ciphertext.cipherW);

    mpz_t x, y;
    mpz_inits(x, y, NULL);
    fp = fopen ("CT/cipherUx","r");
    if (fp != NULL) {
        mpz_inp_str(x, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("CT/cipherUy","r");
    if (fp != NULL) {
        mpz_inp_str(y, fp, BASE);
        fclose(fp);
    }

    ciphertext.cipherU = affine_init(x, y);

    printf ("  read in  "); mpz_out_str (stdout, BASE, ciphertext.cipherU.x); printf("\n");
    printf ("  read in  "); mpz_out_str (stdout, BASE, ciphertext.cipherU.y); printf("\n");
    return ciphertext;
}
