#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_MASTER_SECRET_AS_BINARY_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_MASTER_SECRET_AS_BINARY_H

#include <stddef.h>

/**
 * ## Description
 *
 * Struct holding the master secret in binary form for easier serialization.
 */
typedef struct HessIdentityBasedSignatureMasterSecretAsBinary {
  /**
   * ## Description
   *
   * The master secret.
   */
  void *masterSecret;

  /**
   * ## Description
   *
   * The length of masterSecret.
   */
  size_t masterSecretLength;
} HessIdentityBasedSignatureMasterSecretAsBinary;

#endif