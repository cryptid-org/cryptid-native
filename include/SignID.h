#ifndef SIGNID_H
#define SIGNID_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/PublicParameters.h"
#include "identity-based/Signature.h"
#include "identity-based/SecurityLevel.h"
#include "util/Status.h"


Status signid_setup(SecurityLevel securityLevel, PublicParameters* publicParameters, mpz_t masterSecret);

Status signid_extract(AffinePoint* result, char* identity, size_t identityLength, PublicParameters publicParameters, mpz_t masterSecret);

Status signid_sign(Signature *result, AffinePoint privateKey, char* message, size_t messageLength, char* identity, size_t identityLength, PublicParameters publicParameters);

Status signid_verify(char* message, size_t messageLength, Signature signature, char* identity, size_t identityLength, PublicParameters publicParameters);

#endif
