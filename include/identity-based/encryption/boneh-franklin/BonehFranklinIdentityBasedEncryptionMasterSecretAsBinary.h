#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_MASTER_SECRET_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_MASTER_SECRET_AS_BINARY_H

/**
 * ## Description
 *
 * Struct holding the master secret in binary form for easier serialization.
 */
typedef struct BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
{
    /**
     * ## Description
     *
     * A binary string representing a part of the cipher.
     */
    void *masterSecret;

    /**
     * ## Description
     *
     * The length of [masterSecret](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h#masterSecret).
     */
    size_t masterSecretLength;
} BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary;

#endif