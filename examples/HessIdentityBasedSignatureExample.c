//How to build: ./task.sh build && gcc examples/HessIdentityBasedSignatureExample.c build/libcryptid.a -I/mnt/c/Users/vecsi/Documents/CryptID/cryptid-native/include -D__CRYPTID_GMP -D__CRYPTID_HESS_IDENTITY_BASED_SIGNATURE -std=c99 -Wall -Wextra -Werror -lgmp -lm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE
#include "identity-based/signature/hess/HessIdentityBasedSignature.h"

int main() {
  const char *message = "Ironic.";
  const char *identity = "darth.plagueis@sith.com";

  HessIdentityBasedSignaturePublicParametersAsBinary *publicParameters =
      malloc(sizeof(HessIdentityBasedSignaturePublicParametersAsBinary));
  HessIdentityBasedSignatureMasterSecretAsBinary *masterSecret =
      malloc(sizeof(HessIdentityBasedSignatureMasterSecretAsBinary));

  if (CRYPTID_SUCCESS !=
      cryptid_ibs_hess_setup(masterSecret, publicParameters, LOWEST)) {
    printf("Setup failed\n");
    return -1;
  }

  AffinePointAsBinary *privateKey = malloc(sizeof(AffinePointAsBinary));
  if (CRYPTID_SUCCESS !=
      cryptid_ibs_hess_extract(privateKey, identity, strlen(identity),
                               *masterSecret, *publicParameters)) {
    printf("Extract failed\n");
    return -1;
  }

  HessIdentityBasedSignatureSignatureAsBinary *signature =
      malloc(sizeof(HessIdentityBasedSignatureSignatureAsBinary));
  if (CRYPTID_SUCCESS !=
      cryptid_ibs_hess_sign(signature, message, strlen(message), identity,
                            strlen(identity), *privateKey, *publicParameters)) {
    printf("Sign failed\n");
    return -1;
  }

  if (CRYPTID_SUCCESS !=
      cryptid_ibs_hess_verify(message, strlen(message), *signature, identity,
                              strlen(identity), *publicParameters)) {
    printf("Verify failed\n");
    return -1;
  }

  printf("Verification was successful.\n");

  hessIdentityBasedSignatureSignatureAsBinary_destroy(*signature);
  free(signature);
  affineAsBinary_destroy(*privateKey);
  free(privateKey);
  free(masterSecret->masterSecret);
  free(masterSecret);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(*publicParameters);
  free(publicParameters);

  return 0;
}