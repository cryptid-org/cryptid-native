#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_MASTER_SECRET_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_MASTER_SECRET_AS_BINARY_H

/**
 * ## Description
 *
 * Struct holding the master secret in binary form for easier serialization.
 */
typedef struct BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary {
  /**
   * ## Description
   *
   * The master secret.
   */
  void *masterSecret;

  /**
   * ## Description
   *
   * The length of
   * [masterSecret](codebase://identity-based/signature/hess/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h#BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.masterSecret).
   */
  size_t masterSecretLength;
} BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary;

#endif