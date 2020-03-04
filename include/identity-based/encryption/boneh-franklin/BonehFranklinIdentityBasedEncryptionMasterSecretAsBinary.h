#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_MASTER_SECRET_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_MASTER_SECRET_AS_BINARY_H

typedef struct BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
{
    void *masterSecret;

    size_t masterSecretLength;
} BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary;

#endif