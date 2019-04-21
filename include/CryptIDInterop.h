#ifndef CRYPTID_INTEROP_H
#define CRYPTID_INTEROP_H


#include "AffinePoint.h"
#include "CipherTextTuple.h"
#include "PublicParameters.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

typedef struct
{
    char* plaintext;
    int status;
} DecryptResult;

typedef struct
{
    CipherTextTuple *ciphertext;
    int status;
} EncryptResult;

PublicParameters* EMSCRIPTEN_KEEPALIVE cryptid_interop_allocatePublicParameters(
    char* fieldOrder,
    char* subgroupOrder,
    char* pointPx,
    char* pointPy,
    char* pointPpublicx,
    char* pointPpublicy,
    int securityLevel
);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyPublicParameters(PublicParameters* params);

EncryptResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_encrypt(char* message, int messageLength, char* identity, int identityLength, PublicParameters* params);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherUX(CipherTextTuple* ciphertext);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherUY(CipherTextTuple* ciphertext);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherV(CipherTextTuple* ciphertext);

int EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherVLength(CipherTextTuple* ciphertext);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherW(CipherTextTuple* ciphertext);

int EMSCRIPTEN_KEEPALIVE cryptid_interop_getCipherWLength(CipherTextTuple* ciphertext);

CipherTextTuple* EMSCRIPTEN_KEEPALIVE cryptid_interop_allocateCiphterTextTuple(
    char* cipherUX,
    char* cipherUY,
    unsigned char* cipherV,
    int cipherVLength,
    unsigned char* cipherW,
    int cipherWLength
);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyCipherTextTuple(CipherTextTuple* ciphertext);

AffinePoint* EMSCRIPTEN_KEEPALIVE cryptid_interop_allocatePrivateKey(char* pkxr, char* pkyr);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyPrivateKey(AffinePoint *privateKey);

DecryptResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_decrypt(CipherTextTuple* ciphertext, AffinePoint* privateKey, PublicParameters* params);

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isDecryptSuccess(DecryptResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getDecryptPlaintext(DecryptResult* result);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyDecryptResult(DecryptResult* result);

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isEncryptSuccess(EncryptResult* result);

CipherTextTuple* EMSCRIPTEN_KEEPALIVE cryptid_interop_getEncryptCiphertext(EncryptResult* result);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyEncryptResult(EncryptResult* result);

typedef struct
{
    AffinePoint privateKey;
    int status;
} ExtractResult;

ExtractResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_extract(char* identity, PublicParameters* publicParameters, char* masterSecretStr);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPrivateKeyXR(ExtractResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPrivateKeyYR(ExtractResult* result);

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isExtractSuccess(ExtractResult* result);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroyExtractResult(ExtractResult* result);

typedef struct
{
    PublicParameters* publicParameters;
    char* masterSecret;
    int status;
} SetupResult;

SetupResult* EMSCRIPTEN_KEEPALIVE cryptid_interop_setup(int securityLevel);

int EMSCRIPTEN_KEEPALIVE cryptid_interop_isSetupSuccess(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getMasterSecret(SetupResult* result);

void EMSCRIPTEN_KEEPALIVE cryptid_interop_destroySetupResult(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersFieldOrder(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersQ(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPX(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPY(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPpublicX(SetupResult* result);

char* EMSCRIPTEN_KEEPALIVE cryptid_interop_getPublicParametersPointPpublicY(SetupResult* result);

#endif
