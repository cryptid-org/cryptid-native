// How to build: ./task.sh build && gcc examples/BSWCiphertextPolicyAttributeBasedEncryptionExample.c build/libcryptid.a -I/mnt/c/Users/vecsi/Documents/CryptID/cryptid-native/include -std=c99 -Wall -Wextra -Werror -lgmp -lm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryption.h"

int main() {
  const char *message = "Ironic.";

  /*      --- "Sith"
   *   AND
   *               --- "wise"
   *       --- OR
   *               --- "powerful"
   */

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
      *accessTreeAsBinary =
          bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
              2, NULL, 0, 2); // AND

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child0 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, NULL, 0, 2); // OR
  accessTreeAsBinary->children[0] = child0;

  char *attribute0 = "powerful";
  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child0_0 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, attribute0, strlen(attribute0), 0);
  child0->children[0] = child0_0;

  char *attribute1 = "wise";
  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child0_1 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, attribute1, strlen(attribute1), 0);
  child0->children[1] = child0_1;

  char *attribute2 = "Sith";
  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child1 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, attribute2, strlen(attribute2), 0);
  accessTreeAsBinary->children[1] = child1;

  int numOfAttributes = 2;
  char **attributes = malloc(sizeof(char *) * numOfAttributes);
  attributes[0] = attribute0;
  attributes[1] = attribute2;

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publicKey =
      malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterKey =
      malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));

  if (CRYPTID_SUCCESS != cryptid_abe_bsw_setup(publicKey, masterKey, LOWEST)) {
    printf("Setup failed\n");
    return -1;
  }

  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
      *ciphertext = malloc(sizeof(
          bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));
  if (CRYPTID_SUCCESS != cryptid_abe_bsw_encrypt(ciphertext, accessTreeAsBinary,
                                                 message, strlen(message),
                                                 publicKey)) {
    printf("Encrypt failed\n");
    return -1;
  }

  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *privateKey =
      malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));
  if (CRYPTID_SUCCESS != cryptid_abe_bsw_keygen(privateKey, masterKey,
                                                attributes, numOfAttributes)) {
    printf("Keygen failed\n");
    return -1;
  }

  char *plaintext;
  if (CRYPTID_SUCCESS !=
      cryptid_abe_bsw_decrypt(&plaintext, ciphertext, privateKey)) {
    printf("Decrypt failed\n");
    return -1;
  }

  printf("Plaintext:\n%s\n", plaintext);

  free(plaintext);
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
      ciphertext);
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
      privateKey);
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
      masterKey);
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      publicKey);

  return 0;
}