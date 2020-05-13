//How to build: ./task.sh build && gcc examples/BonehFranklinIdentityBasedEncryptionExample.c build/libcryptid.a -I/mnt/c/Users/vecsi/Documents/CryptID/cryptid-native/include -D__CRYPTID_GMP -D__CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION -std=c99 -Wall -Wextra -Werror -lgmp -lm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryption.h"

int main() {
  const char *message = "Ironic.";
  const char *identity = "darth.plagueis@sith.com";

  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
      *publicParameters = malloc(
          sizeof(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary));
  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary *masterSecret =
      malloc(sizeof(BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary));

  if (CRYPTID_SUCCESS !=
      cryptid_ibe_bonehFranklin_setup(masterSecret, publicParameters, LOWEST)) {
    printf("Setup failed\n");
    return -1;
  }

  BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertext =
      malloc(sizeof(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary));
  if (CRYPTID_SUCCESS != cryptid_ibe_bonehFranklin_encrypt(
                             ciphertext, message, strlen(message), identity,
                             strlen(identity), *publicParameters)) {
    printf("Encrypt failed\n");
    return -1;
  }

  AffinePointAsBinary *privateKey = malloc(sizeof(AffinePointAsBinary));
  if (CRYPTID_SUCCESS !=
      cryptid_ibe_bonehFranklin_extract(privateKey, identity, strlen(identity),
                                        *masterSecret, *publicParameters)) {
    printf("Extract failed\n");
    return -1;
  }

  char *plaintext;
  if (CRYPTID_SUCCESS !=
      cryptid_ibe_bonehFranklin_decrypt(&plaintext, *ciphertext, *privateKey,
                                        *publicParameters)) {
    printf("Decrypt failed\n");
    return -1;
  }

  printf("Plaintext:\n%s\n", plaintext);

  free(plaintext);
  bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(*ciphertext);
  free(ciphertext);
  affineAsBinary_destroy(*privateKey);
  free(privateKey);
  free(masterSecret->masterSecret);
  free(masterSecret);
  bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
      *publicParameters);
  free(publicParameters);

  return 0;
}