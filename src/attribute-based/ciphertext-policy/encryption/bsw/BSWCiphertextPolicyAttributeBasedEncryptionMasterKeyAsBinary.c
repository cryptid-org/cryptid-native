#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary.h"

void bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey) {
  free(masterkey->beta);
  affineAsBinary_destroy(masterkey->g_alpha);
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      masterkey->publickey);
  free(masterkey);
}

void bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionMasterKey(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKey *masterKey,
    const bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary
        *masterKeyAsBinary) {
  mpz_init(masterKey->beta);
  mpz_import(masterKey->beta, masterKeyAsBinary->betaLength, 1, 1, 0, 0,
             masterKeyAsBinary->beta);
  affineAsBinary_toAffine(&(masterKey->g_alpha), masterKeyAsBinary->g_alpha);
  masterKey->publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKey *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKey));
  bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(
      masterKey->publickey, masterKeyAsBinary->publickey);
}

void bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionMasterKey(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary
        *masterKeyAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionMasterKey *masterKey) {
  masterKeyAsBinary->beta = mpz_export(NULL, &masterKeyAsBinary->betaLength, 1,
                                       1, 0, 0, masterKey->beta);
  affineAsBinary_fromAffine(&(masterKeyAsBinary->g_alpha), masterKey->g_alpha);
  masterKeyAsBinary->publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(
      masterKeyAsBinary->publickey, masterKey->publickey);
}