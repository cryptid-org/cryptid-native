#ifndef CRYPTID_H
#define CRYPTID_H

#include "gmp.h"

#include "AffinePoint.h"
#include "CipherTextTuple.h"
#include "PublicParameters.h"
#include "SecurityLevel.h"
#include "Status.h"

Status cryptid_setup(SecurityLevel securityLevel, PublicParameters* publicParameters, mpz_t masterSecret);

Status cryptid_extract(AffinePoint* result, char* identity, size_t identityLength, PublicParameters publicParameters, mpz_t masterSecret);

Status cryptid_encrypt(CipherTextTuple *result, char* message, size_t messageLength, char* identity, size_t identityLength, PublicParameters publicParameters);

Status cryptid_decrypt(char **result, AffinePoint privateKey, CipherTextTuple ciphertext, PublicParameters publicParameters);

#endif
