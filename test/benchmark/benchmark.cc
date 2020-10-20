#include <benchmark/benchmark.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryption.h"
#include "identity-based/encryption/formal-language/FormalLanguageIdentityBasedEncryption.h"
#include "identity-based/signature/hess/HessIdentityBasedSignature.h"
#include "util/LogicalExpressionTreeOperators.h"

typedef enum { ONLY_AND, ONLY_OR, RANDOM } TestType;

const int NUMBER_OF_ATTRIBUTES = 9;
const int NUMBER_OF_ATTRIBUTES_TWO_POWER = 3;

char *
buildRandomAuthorizationTree(CryptidLogicalExpressionTree *authorizationFormula,
                             int depth, TestType testType) {

  CryptidLogicalExpressionTree booleanTree = {};

  char *authorizationFormulaString;

  if (depth == 0) {
    char *value = ".*\".*\": \".*\".*";
    booleanTree.value = (char *)malloc(strlen(value) + 1 * sizeof(char));
    strcpy((char *)booleanTree.value, value);

    authorizationFormulaString =
        (char *)calloc(strlen(value) + 1, sizeof(char));
    strcpy(authorizationFormulaString, value);
  } else {

    booleanTree.value = malloc(sizeof(CryptidLogicalExpressionTreeOperators));

    char *operatorString;
    int random = 0;

    switch (testType) {
    case ONLY_AND:
      *(CryptidLogicalExpressionTreeOperators *)booleanTree.value = AND;
      operatorString = " AND ";
      break;
    case ONLY_OR:
      *(CryptidLogicalExpressionTreeOperators *)booleanTree.value = OR;
      operatorString = " OR ";
      break;
    case RANDOM:
      random = rand() % 2;
      *(CryptidLogicalExpressionTreeOperators *)booleanTree.value =
          (CryptidLogicalExpressionTreeOperators)random;
      operatorString = " RND ";
      break;
    }

    booleanTree.rightChild = (CryptidLogicalExpressionTree *)calloc(
        1, sizeof(CryptidLogicalExpressionTree));
    char *rightAuthorizationFormulaString = buildRandomAuthorizationTree(
        booleanTree.rightChild, depth - 1, testType);

    char *leftAuthorizationFormulaString = "";

    if (random != 2) {
      booleanTree.leftChild = (CryptidLogicalExpressionTree *)calloc(
          1, sizeof(CryptidLogicalExpressionTree));
      leftAuthorizationFormulaString = buildRandomAuthorizationTree(
          booleanTree.leftChild, depth - 1, testType);

      authorizationFormulaString = (char *)calloc(
          strlen(leftAuthorizationFormulaString) + strlen(operatorString) +
              strlen(rightAuthorizationFormulaString) + 1,
          sizeof(char));

      strcpy(authorizationFormulaString, leftAuthorizationFormulaString);
      strcat(authorizationFormulaString, operatorString);
      strcat(authorizationFormulaString, rightAuthorizationFormulaString);

      free(leftAuthorizationFormulaString);
    } else {
      authorizationFormulaString = (char *)calloc(
          strlen(operatorString) + strlen(rightAuthorizationFormulaString) + 1,
          sizeof(char));

      strcpy(authorizationFormulaString, operatorString);
      strcat(authorizationFormulaString, rightAuthorizationFormulaString);
    }

    free(rightAuthorizationFormulaString);
  }

  *authorizationFormula = booleanTree;

  return authorizationFormulaString;
}

void formal_language_ibe_benchmark_full(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  char *identityAlpha = "{\"name\": \"OGS\"}";
  char *identityBeta = "{\"video title\": \"Probalj meg nem megelégedni\", "
                       "\"megelegedesek\": \"3\"}";

  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
      publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidLogicalExpressionTree *authorizationFormula =
      (CryptidLogicalExpressionTree *)calloc(
          1, sizeof(CryptidLogicalExpressionTree));

  authorizationFormula->value =
      malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)authorizationFormula->value = OR;

  authorizationFormula->leftChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *leftChild = ".*\".*\": \".*\".*";
  authorizationFormula->leftChild->value =
      malloc(strlen(leftChild) + 1 * sizeof(char));
  strcpy((char *)authorizationFormula->leftChild->value, leftChild);

  char *authorizationFormulaLeftString = ".*\".*\": \".*\".* OR ";

  authorizationFormula->rightChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *authorizationFormulaRightString =
      buildRandomAuthorizationTree(authorizationFormula->rightChild,
                                   NUMBER_OF_ATTRIBUTES_TWO_POWER, ONLY_OR);

  char *authorizationFormulaString = (char *)malloc(
      strlen(authorizationFormulaLeftString) +
      strlen(authorizationFormulaRightString) + 1 * sizeof(char));
  strcpy(authorizationFormulaString, authorizationFormulaLeftString);
  strcat(authorizationFormulaString, authorizationFormulaRightString);

  AffinePointAsBinary signatureKey;
  HessIdentityBasedSignatureSignatureAsBinary authorizationFormulaSignature;
  char *encryptionKey;
  BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;
  AffinePointAsBinary privateKey;
  char *plaintext;

  CryptidStatus status;

  for (auto _ : state) {

    status = cryptid_ibe_formalLanguage_setup(
        &masterSecretBF, &publicParametersBF, &masterSecretHess,
        &publicParametersHess, securityLevel);

    status = cryptid_ibs_hess_extract(&signatureKey, identityAlpha,
                                      strlen(identityAlpha), masterSecretHess,
                                      publicParametersHess);

    status = cryptid_ibe_formalLanguage_signFormula(
        &authorizationFormulaSignature, authorizationFormulaString,
        strlen(authorizationFormulaString), identityAlpha,
        strlen(identityAlpha), signatureKey, publicParametersHess);

    status = cryptid_ibe_formalLanguage_generateEncryptionKey(
        &encryptionKey, identityAlpha, strlen(identityAlpha));

    status = cryptid_ibe_formalLanguage_encrypt(
        &ciphertext, message, strlen(message), encryptionKey,
        strlen(encryptionKey), publicParametersBF);

    status = cryptid_ibe_formalLanguage_extract(
        &privateKey, authorizationFormula, authorizationFormulaString,
        strlen(authorizationFormulaString), &authorizationFormulaSignature,
        identityAlpha, strlen(identityAlpha), identityBeta,
        strlen(identityBeta), encryptionKey, strlen(encryptionKey),
        masterSecretBF, publicParametersBF, publicParametersHess);

    status = cryptid_ibe_formalLanguage_decrypt(&plaintext, ciphertext,
                                                privateKey, publicParametersBF);
  }

  bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
      publicParametersBF);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(
      publicParametersHess);
  free(masterSecretBF.masterSecret);
  free(masterSecretHess.masterSecret);
  hessIdentityBasedSignatureSignatureAsBinary_destroy(
      authorizationFormulaSignature);
  affineAsBinary_destroy(privateKey);
  affineAsBinary_destroy(signatureKey);
  free(authorizationFormulaRightString);
  free(authorizationFormulaString);
  free(plaintext);
  free(encryptionKey);
  LogicalExpressionTree_destroy(authorizationFormula);
  bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(ciphertext);
}

void formal_language_ibe_benchmark_extract(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  char *identityAlpha = "{\"name\": \"OGS\"}";
  char *identityBeta = "{\"video title\": \"Probalj meg nem megelégedni\", "
                       "\"megelegedesek\": \"3\"}";

  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
      publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidLogicalExpressionTree *authorizationFormula =
      (CryptidLogicalExpressionTree *)calloc(
          1, sizeof(CryptidLogicalExpressionTree));

  authorizationFormula->value =
      malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)authorizationFormula->value = OR;

  authorizationFormula->leftChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *leftChild = ".*\".*\": \".*\".*";
  authorizationFormula->leftChild->value =
      malloc(strlen(leftChild) + 1 * sizeof(char));
  strcpy((char *)authorizationFormula->leftChild->value, leftChild);

  char *authorizationFormulaLeftString = ".*\".*\": \".*\".* OR ";

  authorizationFormula->rightChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *authorizationFormulaRightString =
      buildRandomAuthorizationTree(authorizationFormula->rightChild,
                                   NUMBER_OF_ATTRIBUTES_TWO_POWER, ONLY_OR);

  char *authorizationFormulaString = (char *)malloc(
      strlen(authorizationFormulaLeftString) +
      strlen(authorizationFormulaRightString) + 1 * sizeof(char));
  strcpy(authorizationFormulaString, authorizationFormulaLeftString);
  strcat(authorizationFormulaString, authorizationFormulaRightString);

  AffinePointAsBinary signatureKey;
  HessIdentityBasedSignatureSignatureAsBinary authorizationFormulaSignature;
  char *encryptionKey;
  AffinePointAsBinary privateKey;

  CryptidStatus status = cryptid_ibe_formalLanguage_setup(
        &masterSecretBF, &publicParametersBF, &masterSecretHess,
        &publicParametersHess, securityLevel);

    status = cryptid_ibs_hess_extract(&signatureKey, identityAlpha,
                                      strlen(identityAlpha), masterSecretHess,
                                      publicParametersHess);

    status = cryptid_ibe_formalLanguage_signFormula(
        &authorizationFormulaSignature, authorizationFormulaString,
        strlen(authorizationFormulaString), identityAlpha,
        strlen(identityAlpha), signatureKey, publicParametersHess);

    status = cryptid_ibe_formalLanguage_generateEncryptionKey(
        &encryptionKey, identityAlpha, strlen(identityAlpha));

  for (auto _ : state) {
    status = cryptid_ibe_formalLanguage_extract(
        &privateKey, authorizationFormula, authorizationFormulaString,
        strlen(authorizationFormulaString), &authorizationFormulaSignature,
        identityAlpha, strlen(identityAlpha), identityBeta,
        strlen(identityBeta), encryptionKey, strlen(encryptionKey),
        masterSecretBF, publicParametersBF, publicParametersHess);
  }

  bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
      publicParametersBF);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(
      publicParametersHess);
  free(masterSecretBF.masterSecret);
  free(masterSecretHess.masterSecret);
  hessIdentityBasedSignatureSignatureAsBinary_destroy(
      authorizationFormulaSignature);
  affineAsBinary_destroy(privateKey);
  affineAsBinary_destroy(signatureKey);
  free(authorizationFormulaRightString);
  free(authorizationFormulaString);
  free(encryptionKey);
  LogicalExpressionTree_destroy(authorizationFormula);
}

void formal_language_ibe_benchmark_encrypt(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  char *identityAlpha = "{\"name\": \"OGS\"}";
  char *identityBeta = "{\"video title\": \"Probalj meg nem megelégedni\", "
                       "\"megelegedesek\": \"3\"}";

  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
      publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidLogicalExpressionTree *authorizationFormula =
      (CryptidLogicalExpressionTree *)calloc(
          1, sizeof(CryptidLogicalExpressionTree));

  authorizationFormula->value =
      malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)authorizationFormula->value = OR;

  authorizationFormula->leftChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *leftChild = ".*\".*\": \".*\".*";
  authorizationFormula->leftChild->value =
      malloc(strlen(leftChild) + 1 * sizeof(char));
  strcpy((char *)authorizationFormula->leftChild->value, leftChild);

  char *authorizationFormulaLeftString = ".*\".*\": \".*\".* OR ";

  authorizationFormula->rightChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *authorizationFormulaRightString =
      buildRandomAuthorizationTree(authorizationFormula->rightChild,
                                   NUMBER_OF_ATTRIBUTES_TWO_POWER, ONLY_OR);

  char *authorizationFormulaString = (char *)malloc(
      strlen(authorizationFormulaLeftString) +
      strlen(authorizationFormulaRightString) + 1 * sizeof(char));
  strcpy(authorizationFormulaString, authorizationFormulaLeftString);
  strcat(authorizationFormulaString, authorizationFormulaRightString);

  AffinePointAsBinary signatureKey;
  HessIdentityBasedSignatureSignatureAsBinary authorizationFormulaSignature;
  char *encryptionKey;
  BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;
  AffinePointAsBinary privateKey;

  CryptidStatus status = cryptid_ibe_formalLanguage_setup(
        &masterSecretBF, &publicParametersBF, &masterSecretHess,
        &publicParametersHess, securityLevel);

    status = cryptid_ibs_hess_extract(&signatureKey, identityAlpha,
                                      strlen(identityAlpha), masterSecretHess,
                                      publicParametersHess);

    status = cryptid_ibe_formalLanguage_signFormula(
        &authorizationFormulaSignature, authorizationFormulaString,
        strlen(authorizationFormulaString), identityAlpha,
        strlen(identityAlpha), signatureKey, publicParametersHess);

    status = cryptid_ibe_formalLanguage_generateEncryptionKey(
        &encryptionKey, identityAlpha, strlen(identityAlpha));

  for (auto _ : state) {
    status = cryptid_ibe_formalLanguage_encrypt(
        &ciphertext, message, strlen(message), encryptionKey,
        strlen(encryptionKey), publicParametersBF);
  }

  bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
      publicParametersBF);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(
      publicParametersHess);
  free(masterSecretBF.masterSecret);
  free(masterSecretHess.masterSecret);
  hessIdentityBasedSignatureSignatureAsBinary_destroy(
      authorizationFormulaSignature);
  affineAsBinary_destroy(privateKey);
  affineAsBinary_destroy(signatureKey);
  free(authorizationFormulaRightString);
  free(authorizationFormulaString);
  free(encryptionKey);
  LogicalExpressionTree_destroy(authorizationFormula);
  bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(ciphertext);
}

void formal_language_ibe_benchmark_decrypt(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  char *identityAlpha = "{\"name\": \"OGS\"}";
  char *identityBeta = "{\"video title\": \"Probalj meg nem megelégedni\", "
                       "\"megelegedesek\": \"3\"}";

  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
      publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidLogicalExpressionTree *authorizationFormula =
      (CryptidLogicalExpressionTree *)calloc(
          1, sizeof(CryptidLogicalExpressionTree));

  authorizationFormula->value =
      malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)authorizationFormula->value = OR;

  authorizationFormula->leftChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *leftChild = ".*\".*\": \".*\".*";
  authorizationFormula->leftChild->value =
      malloc(strlen(leftChild) + 1 * sizeof(char));
  strcpy((char *)authorizationFormula->leftChild->value, leftChild);

  char *authorizationFormulaLeftString = ".*\".*\": \".*\".* OR ";

  authorizationFormula->rightChild = (CryptidLogicalExpressionTree *)calloc(
      1, sizeof(CryptidLogicalExpressionTree));
  char *authorizationFormulaRightString =
      buildRandomAuthorizationTree(authorizationFormula->rightChild,
                                   NUMBER_OF_ATTRIBUTES_TWO_POWER, ONLY_OR);

  char *authorizationFormulaString = (char *)malloc(
      strlen(authorizationFormulaLeftString) +
      strlen(authorizationFormulaRightString) + 1 * sizeof(char));
  strcpy(authorizationFormulaString, authorizationFormulaLeftString);
  strcat(authorizationFormulaString, authorizationFormulaRightString);

  AffinePointAsBinary signatureKey;
  HessIdentityBasedSignatureSignatureAsBinary authorizationFormulaSignature;
  char *encryptionKey;
  BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;
  AffinePointAsBinary privateKey;
  char *plaintext;

  CryptidStatus status = cryptid_ibe_formalLanguage_setup(
        &masterSecretBF, &publicParametersBF, &masterSecretHess,
        &publicParametersHess, securityLevel);

    status = cryptid_ibs_hess_extract(&signatureKey, identityAlpha,
                                      strlen(identityAlpha), masterSecretHess,
                                      publicParametersHess);

    status = cryptid_ibe_formalLanguage_signFormula(
        &authorizationFormulaSignature, authorizationFormulaString,
        strlen(authorizationFormulaString), identityAlpha,
        strlen(identityAlpha), signatureKey, publicParametersHess);

    status = cryptid_ibe_formalLanguage_generateEncryptionKey(
        &encryptionKey, identityAlpha, strlen(identityAlpha));

    status = cryptid_ibe_formalLanguage_encrypt(
        &ciphertext, message, strlen(message), encryptionKey,
        strlen(encryptionKey), publicParametersBF);

    status = cryptid_ibe_formalLanguage_extract(
        &privateKey, authorizationFormula, authorizationFormulaString,
        strlen(authorizationFormulaString), &authorizationFormulaSignature,
        identityAlpha, strlen(identityAlpha), identityBeta,
        strlen(identityBeta), encryptionKey, strlen(encryptionKey),
        masterSecretBF, publicParametersBF, publicParametersHess);

  for (auto _ : state) {
    status = cryptid_ibe_formalLanguage_decrypt(&plaintext, ciphertext,
                                                privateKey, publicParametersBF);
  }

  bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
      publicParametersBF);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(
      publicParametersHess);
  free(masterSecretBF.masterSecret);
  free(masterSecretHess.masterSecret);
  hessIdentityBasedSignatureSignatureAsBinary_destroy(
      authorizationFormulaSignature);
  affineAsBinary_destroy(privateKey);
  affineAsBinary_destroy(signatureKey);
  free(authorizationFormulaRightString);
  free(authorizationFormulaString);
  free(plaintext);
  free(encryptionKey);
  LogicalExpressionTree_destroy(authorizationFormula);
  bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(ciphertext);
}

void bsw_abe_benchmark_full(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  int expectedReponse = 1;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
      *accessTreeAsBinary =
          bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
              1, NULL, 0, NUMBER_OF_ATTRIBUTES); // AND

  for (int i = 0; i < NUMBER_OF_ATTRIBUTES; i++) {
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child =
        bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
            1, ".*\".*\": \".*\".*", strlen(".*\".*\": \".*\".*"), 0);
    accessTreeAsBinary->children[i] = child;
  }

  int numAttributes = 1;
  char **attributes = (char **)malloc(sizeof(char *) * numAttributes);
  attributes[0] = ".*\".*\": \".*\".*";

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey =
      (bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *encrypted =
      (bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *)
          malloc(sizeof(
              bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
      *secretkeyAsBinary =
          (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *)
              malloc(sizeof(
                  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));
  char *result;

  CryptidStatus status;

  for (auto _ : state) {

    status = cryptid_abe_bsw_setup(publickey, masterkey, securityLevel);

    status = cryptid_abe_bsw_encrypt(encrypted, accessTreeAsBinary, message,
                                     strlen(message), publickey);

    status = cryptid_abe_bsw_keygen(secretkeyAsBinary, masterkey, attributes,
                                    numAttributes);

    status = cryptid_abe_bsw_decrypt(&result, encrypted, secretkeyAsBinary);
  }

  free(result);
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      publickey);
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
      masterkey);
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
      encrypted);
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
      secretkeyAsBinary);
}

void bsw_abe_benchmark_extract(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  int expectedReponse = 1;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
      *accessTreeAsBinary =
          bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
              1, NULL, 0, NUMBER_OF_ATTRIBUTES); // AND

  for (int i = 0; i < NUMBER_OF_ATTRIBUTES; i++) {
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child =
        bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
            1, ".*\".*\": \".*\".*", strlen(".*\".*\": \".*\".*"), 0);
    accessTreeAsBinary->children[i] = child;
  }

  int numAttributes = 1;
  char **attributes = (char **)malloc(sizeof(char *) * numAttributes);
  attributes[0] = ".*\".*\": \".*\".*";

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey =
      (bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
      *secretkeyAsBinary =
          (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *)
              malloc(sizeof(
                  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));

  CryptidStatus status = cryptid_abe_bsw_setup(publickey, masterkey, securityLevel);

  for (auto _ : state) {
    status = cryptid_abe_bsw_keygen(secretkeyAsBinary, masterkey, attributes,
                                    numAttributes);
  }

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      publickey);
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
      masterkey);
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
      secretkeyAsBinary);
}

void bsw_abe_benchmark_encrypt(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  int expectedReponse = 1;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
      *accessTreeAsBinary =
          bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
              1, NULL, 0, NUMBER_OF_ATTRIBUTES); // AND

  for (int i = 0; i < NUMBER_OF_ATTRIBUTES; i++) {
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child =
        bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
            1, ".*\".*\": \".*\".*", strlen(".*\".*\": \".*\".*"), 0);
    accessTreeAsBinary->children[i] = child;
  }

  int numAttributes = 1;
  char **attributes = (char **)malloc(sizeof(char *) * numAttributes);
  attributes[0] = ".*\".*\": \".*\".*";

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey =
      (bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *encrypted =
      (bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *)
          malloc(sizeof(
              bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));

  CryptidStatus status = cryptid_abe_bsw_setup(publickey, masterkey, securityLevel);

  for (auto _ : state) {
    status = cryptid_abe_bsw_encrypt(encrypted, accessTreeAsBinary, message,
                                     strlen(message), publickey);
  }

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      publickey);
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
      masterkey);
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
      encrypted);
}

void bsw_abe_benchmark_decrypt(benchmark::State &state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  int expectedReponse = 1;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
      *accessTreeAsBinary =
          bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
              1, NULL, 0, NUMBER_OF_ATTRIBUTES); // AND

  for (int i = 0; i < NUMBER_OF_ATTRIBUTES; i++) {
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child =
        bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
            1, ".*\".*\": \".*\".*", strlen(".*\".*\": \".*\".*"), 0);
    accessTreeAsBinary->children[i] = child;
  }

  int numAttributes = 1;
  char **attributes = (char **)malloc(sizeof(char *) * numAttributes);
  attributes[0] = ".*\".*\": \".*\".*";

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey =
      (bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *encrypted =
      (bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *)
          malloc(sizeof(
              bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
      *secretkeyAsBinary =
          (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *)
              malloc(sizeof(
                  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));
  char *result;

  CryptidStatus status = cryptid_abe_bsw_setup(publickey, masterkey, securityLevel);

    status = cryptid_abe_bsw_encrypt(encrypted, accessTreeAsBinary, message,
                                     strlen(message), publickey);

    status = cryptid_abe_bsw_keygen(secretkeyAsBinary, masterkey, attributes,
                                    numAttributes);

  for (auto _ : state) {
    status = cryptid_abe_bsw_decrypt(&result, encrypted, secretkeyAsBinary);
  }

  free(result);
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      publickey);
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
      masterkey);
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
      encrypted);
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
      secretkeyAsBinary);
}

BENCHMARK(formal_language_ibe_benchmark_full)->Iterations(20);

BENCHMARK(formal_language_ibe_benchmark_extract)->Iterations(20);

BENCHMARK(formal_language_ibe_benchmark_encrypt)->Iterations(20);

BENCHMARK(formal_language_ibe_benchmark_decrypt)->Iterations(20);

BENCHMARK(bsw_abe_benchmark_full)->Iterations(20);

BENCHMARK(bsw_abe_benchmark_extract)->Iterations(20);

BENCHMARK(bsw_abe_benchmark_encrypt)->Iterations(20);

BENCHMARK(bsw_abe_benchmark_decrypt)->Iterations(20);

BENCHMARK_MAIN();