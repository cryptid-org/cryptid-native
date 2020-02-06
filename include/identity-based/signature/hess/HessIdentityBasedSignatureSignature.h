#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"


typedef struct HessIdentityBasedSignatureSignature
{
    AffinePoint u;
    mpz_t v;
} HessIdentityBasedSignatureSignature;

HessIdentityBasedSignatureSignature hessIdentityBasedSignatureSignature_init(const AffinePoint u, const mpz_t v);

void hessIdentityBasedSignatureSignature_destroy(HessIdentityBasedSignatureSignature hessIdentityBasedSignatureSignature);

#endif
