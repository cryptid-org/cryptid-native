#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_AS_STRING_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_AS_STRING_H

#include "elliptic/AffinePointAsString.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"


typedef struct HessIdentityBasedSignatureSignatureAsString
{
    AffinePointAsString u;

    char *v;

    int vBase;
} HessIdentityBasedSignatureSignatureAsString;

void hessIdentityBasedSignatureSignatureAsString_init(HessIdentityBasedSignatureSignatureAsString *signatureAsStringOutput, const AffinePointAsString u, const char *const v, const int vBase);

void hessIdentityBasedSignatureSignatureAsString_destroy(HessIdentityBasedSignatureSignatureAsString signatureAsString);

void hessIdentityBasedSignatureSignatureAsString_toHessIdentityBasedSignatureSignature(HessIdentityBasedSignatureSignature *signatureOutput, const HessIdentityBasedSignatureSignatureAsString signatureAsString);

void hessIdentityBasedSignatureSignature_toHessIdentityBasedSignatureSignatureAsString(HessIdentityBasedSignatureSignatureAsString *signatureAsStringOutput, const HessIdentityBasedSignatureSignature signature, const int uXBase, const int uYBase, const int vBase);

#endif
