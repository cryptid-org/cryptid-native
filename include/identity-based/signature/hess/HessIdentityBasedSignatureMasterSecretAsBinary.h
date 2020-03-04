#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_MASTER_SECRET_AS_BINARY_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_MASTER_SECRET_AS_BINARY_H

typedef struct HessIdentityBasedSignatureMasterSecretAsBinary
{
    void *masterSecret;

    size_t masterSecretLength;
} HessIdentityBasedSignatureMasterSecretAsBinary;

#endif