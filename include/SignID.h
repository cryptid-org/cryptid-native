#ifndef __SIGNID_H
#define __SIGNID_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/Signature.h"
#include "identity-based/PublicParameters.h"
#include "identity-based/SecurityLevel.h"
#include "util/Status.h"

SignidStatus signid_setup(const SecurityLevel securityLevel, PublicParameters* publicParameters, mpz_t masterSecret);

SignidStatus signid_extract(AffinePoint* result, const char *const identity, const size_t identityLength,
                    const PublicParameters publicParameters, const mpz_t masterSecret);

SignidStatus signid_sign(Signature *result, const AffinePoint privateKey, const char *const message, const size_t messageLength,
                    const char *const identity, const size_t identityLength, const PublicParameters publicParameters);

SignidStatus signid_verify(const char *const message, const size_t messageLength, const Signature signature,
                    const char *const identity, const size_t identityLength, const PublicParameters publicParameters);

#endif