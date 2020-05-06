#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_AS_BINARY_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_AS_BINARY_H

#include "elliptic/AffinePointAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"

typedef struct HessIdentityBasedSignatureSignatureAsBinary {
  AffinePointAsBinary u;

  void *v;

  size_t vLength;
} HessIdentityBasedSignatureSignatureAsBinary;

void hessIdentityBasedSignatureSignatureAsBinary_init(
    HessIdentityBasedSignatureSignatureAsBinary *signatureAsBinaryOutput,
    const AffinePointAsBinary u, const void *const v, const size_t vLength);

void hessIdentityBasedSignatureSignatureAsBinary_destroy(
    HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary);

void hessIdentityBasedSignatureSignatureAsBinary_toHessIdentityBasedSignatureSignature(
    HessIdentityBasedSignatureSignature *signatureOutput,
    const HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary);

void hessIdentityBasedSignatureSignatureAsBinary_fromHessIdentityBasedSignatureSignature(
    HessIdentityBasedSignatureSignatureAsBinary *signatureAsBinaryOutput,
    const HessIdentityBasedSignatureSignature signature);

#endif
