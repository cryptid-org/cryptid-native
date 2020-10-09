#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "greatest.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/encryption/formal-language/FormalLanguageIdentityBasedEncryption.h"
#include "identity-based/signature/hess/HessIdentityBasedSignature.h"
#include "util/LogicalExpressionTreeOperators.h"

const char *LOWEST_QUICK_CHECK_ARGUMENT = "--lowest-quick-check";

int isLowestQuickCheck = 0;
int isVerbose = 0;

TEST fresh_formal_language_ibe_setup_matching_identities(
    const SecurityLevel securityLevel, const char *const message,
    const char *const identityAlpha, const char *const identityBeta) {
  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidStatus status = cryptid_ibe_formalLanguage_setup(
      &masterSecretBF, &publicParametersBF, &masterSecretHess, &publicParametersHess, securityLevel);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  char* authorizationFormulaString = ".*\"video title\": \"Probalj meg nem megelégedni\".* AND .*\"megelegedesek\": \"3\".*";

  CryptidLogicalExpressionTree *authorizationFormula = calloc(1, sizeof(CryptidLogicalExpressionTree));
  authorizationFormula->value = malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators*)authorizationFormula->value = AND;

  authorizationFormula->leftChild = calloc(1, sizeof(CryptidLogicalExpressionTree));
  char* leftChild = ".*\"video title\": \"Probalj meg nem megelégedni\".*";
  authorizationFormula->leftChild->value = malloc(strlen(leftChild) + 1 * sizeof(char));
  strcpy(authorizationFormula->leftChild->value, leftChild);

  authorizationFormula->rightChild = calloc(1, sizeof(CryptidLogicalExpressionTree));
  char *rightChild = ".*\"megelegedesek\": \"3\".*";
  authorizationFormula->rightChild->value = malloc(strlen(rightChild) + 1 * sizeof(char));
  strcpy(authorizationFormula->rightChild->value, rightChild);

  AffinePointAsBinary signatureKey;
  status = cryptid_ibs_hess_extract(&signatureKey, identityAlpha, strlen(identityAlpha),
                                    masterSecretHess, publicParametersHess);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  HessIdentityBasedSignatureSignatureAsBinary authorizationFormulaSignature;
  status = cryptid_ibe_formalLanguage_signFormula(&authorizationFormulaSignature, authorizationFormulaString, strlen(authorizationFormulaString), identityAlpha, strlen(identityAlpha), signatureKey, publicParametersHess);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  char* encryptionKey;
  status = cryptid_ibe_formalLanguage_generateEncryptionKey(&encryptionKey, identityAlpha, strlen(identityAlpha));

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;
  status = cryptid_ibe_formalLanguage_encrypt(&ciphertext, message, strlen(message), encryptionKey, strlen(encryptionKey), publicParametersBF);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  AffinePointAsBinary privateKey;
  status = cryptid_ibe_formalLanguage_extract(
      &privateKey, authorizationFormula, authorizationFormulaString, strlen(authorizationFormulaString), &authorizationFormulaSignature, identityAlpha, strlen(identityAlpha), identityBeta, strlen(identityBeta), encryptionKey, strlen(encryptionKey), masterSecretBF, publicParametersBF, publicParametersHess);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  char *plaintext;
  status = cryptid_ibe_formalLanguage_decrypt(&plaintext, ciphertext, privateKey,
                                             publicParametersBF);

  ASSERT_EQ(status, CRYPTID_SUCCESS);
  ASSERT_EQ(strcmp(message, plaintext), 0);

  PASS();
}

SUITE(cryptid_formal_language_ibe_suite) {
  {
    RUN_TESTp(fresh_formal_language_ibe_setup_matching_identities,
                    1, "Online Games Studios", "{\"name\": \"OGS\"}", "{\"video title\": \"Probalj meg nem megelégedni\", \"megelegedesek\": \"3\"}");
        
  }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(cryptid_formal_language_ibe_suite);

  GREATEST_MAIN_END();
}
