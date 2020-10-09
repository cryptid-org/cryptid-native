#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "greatest.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/encryption/formal-language/FormalLanguageIdentityBasedEncryption.h"

const char *LOWEST_QUICK_CHECK_ARGUMENT = "--lowest-quick-check";

int isLowestQuickCheck = 0;
int isVerbose = 0;

TEST fresh_formal_language_ibe_setup_matching_identities(
    const SecurityLevel securityLevel, const char *const message,
    const char *const identity) {
  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidStatus status = cryptid_ibe_formalLanguage_setup(
      &masterSecretBF, &publicParametersBF, &masterSecretHess, &publicParametersHess, securityLevel);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  /*AffinePointAsBinary privateKey;
  status = cryptid_ibe_formalLanguage_extract(
      &privateKey, identity, strlen(identity), masterSecret, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;
  status = cryptid_ibe_bonehFranklin_encrypt(
      &ciphertext, message, strlen(message), identity, strlen(identity),
      publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  char *plaintext;
  status = cryptid_ibe_bonehFranklin_decrypt(&plaintext, ciphertext, privateKey,
                                             publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);
  ASSERT_EQ(strcmp(message, plaintext), 0);

  free(plaintext);
  bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(ciphertext);
  affineAsBinary_destroy(privateKey);
  free(masterSecret.masterSecret);
  bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
      publicParameters);*/

  PASS();
}

SUITE(cryptid_formal_language_ibe_suite) {
  {
    RUN_TESTp(fresh_formal_language_ibe_setup_matching_identities,
                    1, "Online Games Studios", "{\"video title\": \"Próbálj meg nem megelégedni\", \"megelégedések\": \"3\"}");
        
  }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(cryptid_formal_language_ibe_suite);

  GREATEST_MAIN_END();
}
