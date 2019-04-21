#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "Complex.h"
#include "EllipticCurve.h"
#include "HashFunction.h"
#include "CryptID.h"
#include "CryptIDInterop.h"
#include "Status.h"

static const unsigned int CONVERSION_BASE = 10;

PublicParameters* EMSCRIPTEN_KEEPALIVE cryptid_interop_allocatePublicParameters(
    char* fieldOrder,
    char* subgroupOrder,
    char* pointPx,
    char* pointPy,
    char* pointPpublicx,
    char* pointPpublicy,
    int securityLevel
) 
{
    int hashTranslationMap[] = {1, 224, 256, 384, 512};
    PublicParameters* params = (PublicParameters*)malloc(sizeof (PublicParameters));
    mpz_t zero, one, p, px, py, ppx, ppy;
    mpz_init_set_ui(zero, 0);
    mpz_init_set_ui(one, 1);
    mpz_init_set_str(p, fieldOrder, CONVERSION_BASE);
    mpz_init_set_str(px, pointPx, CONVERSION_BASE);
    mpz_init_set_str(py, pointPy, CONVERSION_BASE);
    mpz_init_set_str(ppx, pointPpublicx, CONVERSION_BASE);
    mpz_init_set_str(ppy, pointPpublicy, CONVERSION_BASE);

    params->ellipticCurve = ellipticCurve_init(zero, one, p);
    mpz_init_set_str(params->q, subgroupOrder, CONVERSION_BASE);

    params->pointP = affine_init(px, py);
    params->pointPpublic = affine_init(ppx, ppy);

    params->hashFunction = hashFunction_init(hashTranslationMap[securityLevel]);

    mpz_clears(zero, one, p, px, py, ppx, ppy, NULL);

    return params;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyPublicParameters(PublicParameters* params)
{
    ellipticCurve_destroy(params->ellipticCurve);
    mpz_clear(params->q);
    affine_destroy(params->pointP);
    affine_destroy(params->pointPpublic);
}

EncryptResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_encrypt(char* message, int messageLength, char* identity, int identityLength, PublicParameters* params)
{
    EncryptResult* result = malloc(sizeof (EncryptResult));
    result->ciphertext = malloc(sizeof (CipherTextTuple));

    result->status = cryptid_encrypt(result->ciphertext, message, messageLength, identity, identityLength, *params);

    return result;
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherUX(CipherTextTuple* ciphertext)
{
    return mpz_get_str(NULL, CONVERSION_BASE, ciphertext->cipherU.x);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherUY(CipherTextTuple* ciphertext)
{
    return mpz_get_str(NULL, CONVERSION_BASE, ciphertext->cipherU.y);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherV(CipherTextTuple* ciphertext)
{
    return (char*) ciphertext->cipherV;
}

int EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherVLength(CipherTextTuple* ciphertext)
{
    return ciphertext->cipherVLength;
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherW(CipherTextTuple* ciphertext)
{
    return (char*) ciphertext->cipherW;
}

int EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherWLength(CipherTextTuple* ciphertext)
{
    return ciphertext->cipherWLength;
}

CipherTextTuple* EMSCRIPTEN_KEEPALIVE cryptid_interop_allocateCiphterTextTuple(
    char* cipherUX,
    char* cipherUY,
    unsigned char* cipherV,
    int cipherVLength,
    unsigned char* cipherW,
    int cipherWLength
)
{
    mpz_t ux, uy;
    mpz_init_set_str(ux, cipherUX, CONVERSION_BASE);
    mpz_init_set_str(uy, cipherUY, CONVERSION_BASE);

    CipherTextTuple* ciphertext = malloc(sizeof (CipherTextTuple));
    AffinePoint cipherU = affine_init(ux, uy);
    *ciphertext = cipherTextTuple_init(cipherU, cipherV, cipherVLength, cipherW, cipherWLength);

    affine_destroy(cipherU);
    mpz_clears(ux, uy, NULL);

    return ciphertext;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyCipherTextTuple(CipherTextTuple* ciphertext)
{
    cipherTextTuple_destroy(*ciphertext);
}

AffinePoint* EMSCRIPTEN_KEEPALIVE cryptid_interop_allocatePrivateKey(char* ch_pkx, char* ch_pky)
{
    mpz_t pkx, pky;
    mpz_init_set_str(pkx, ch_pkx, CONVERSION_BASE);
    mpz_init_set_str(pky, ch_pky, CONVERSION_BASE);

    AffinePoint* pk = malloc(sizeof (AffinePoint));

    *pk = affine_init(pkx, pky);
    
    mpz_clears(pkx, pky, NULL);

    return pk;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyPrivateKey(AffinePoint *privateKey)
{
    affine_destroy(*privateKey);
}

DecryptResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_decrypt(CipherTextTuple* ciphertext, AffinePoint* privateKey, PublicParameters* params)
{
    DecryptResult* result = malloc(sizeof (DecryptResult));

    result->status = cryptid_decrypt(&(result->plaintext), *privateKey, *ciphertext, *params);

    return result;
}

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isDecryptSuccess(DecryptResult* result)
{
    return result->status == 0;
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getDecryptPlaintext(DecryptResult* result)
{
    return result->plaintext;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyDecryptResult(DecryptResult* result)
{
    if (result->status == SUCCESS)
    {
        free(result->plaintext);
    }
}

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isEncryptSuccess(EncryptResult* result)
{
    return result->status == 0;
}

CipherTextTuple* EMSCRIPTEN_KEEPALIVE cryptid_interop_getEncryptCiphertext(EncryptResult* result)
{
    return result->ciphertext;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyEncryptResult(EncryptResult* result)
{
    if (result->status == SUCCESS)
    {
        cryptid_interop_destroyCipherTextTuple(result->ciphertext);
    }

    free(result->ciphertext);
}

ExtractResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_extract(char* identity, PublicParameters* publicParameters, char* masterSecretStr)
{
    ExtractResult* result = malloc(sizeof (ExtractResult));

    size_t identityLength = strlen(identity);

    mpz_t masterSecret;
    mpz_init_set_str(masterSecret, masterSecretStr, CONVERSION_BASE);

    result->status = cryptid_extract(&(result->privateKey), identity, identityLength, *publicParameters, masterSecret);

    mpz_clear(masterSecret);

    return result;
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPrivateKeyX(ExtractResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->privateKey.x);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPrivateKeyY(ExtractResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->privateKey.y);
}

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isExtractSuccess(ExtractResult* result)
{
    return result->status == 0;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyExtractResult(ExtractResult* result)
{
    if (result->status == SUCCESS)
    {
        affine_destroy(result->privateKey);
    }
}

SetupResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_setup(int securityLevel)
{
    SetupResult* result = malloc(sizeof (SetupResult));
    result->publicParameters = malloc(sizeof (PublicParameters));
    mpz_t masterSecret;
    mpz_init(masterSecret);
    mpz_init(result->publicParameters->q);

    result->status = cryptid_setup(securityLevel, result->publicParameters, masterSecret);

    if (result->status == SUCCESS)
    {
        result->masterSecret = mpz_get_str(NULL, CONVERSION_BASE, masterSecret);
    }

    mpz_clear(masterSecret);

    return result;
}

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isSetupSuccess(SetupResult* result)
{
    return result->status == 0;
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getMasterSecret(SetupResult* result)
{
    return result->masterSecret;
}

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroySetupResult(SetupResult* result)
{
    if (result->status == SUCCESS)
    {
        cryptid_interop_destroyPublicParameters(result->publicParameters);
        free(result->masterSecret);
    }

    free(result->publicParameters);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersFieldOrder(SetupResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->publicParameters->ellipticCurve.fieldOrder);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersQ(SetupResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->publicParameters->q);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPX(SetupResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->publicParameters->pointP.x);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPY(SetupResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->publicParameters->pointP.y);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPpublicX(SetupResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->publicParameters->pointPpublic.x);
}

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPpublicY(SetupResult* result)
{
    return mpz_get_str(NULL, CONVERSION_BASE, result->publicParameters->pointPpublic.y);
}

