#include <benchmark/benchmark.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "identity-based/encryption/formal-language/FormalLanguageIdentityBasedEncryption.h"
#include "identity-based/signature/hess/HessIdentityBasedSignature.h"
#include "util/LogicalExpressionTreeOperators.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryption.h"


typedef enum { ONLY_AND, ONLY_OR, RANDOM } FLIBETestType;

char *
buildRandomAuthorizationTree(CryptidLogicalExpressionTree *authorizationFormula,
                             int depth, FLIBETestType testType) {

  CryptidLogicalExpressionTree booleanTree =
      *(CryptidLogicalExpressionTree *)calloc(
          1, sizeof(CryptidLogicalExpressionTree));

  char *authorizationFormulaString;

  if (depth == 0) {
    char *value = ".*\".*\": \".*\".*";
    booleanTree.value = malloc(strlen(value) + 1 * sizeof(int));
    strcpy((char*)booleanTree.value, value);

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
      random = rand() % 4;
      *(CryptidLogicalExpressionTreeOperators *)booleanTree.value = (CryptidLogicalExpressionTreeOperators)random;
      operatorString = " RND ";
      break;
    }

    char *leftAuthorizationFormulaString = "";

    if (random != 2) {
      booleanTree.leftChild = (CryptidLogicalExpressionTree *)calloc(1, sizeof(CryptidLogicalExpressionTree));
      leftAuthorizationFormulaString = buildRandomAuthorizationTree(
          booleanTree.leftChild, depth - 1, testType);
    }

    booleanTree.rightChild = (CryptidLogicalExpressionTree *)calloc(1, sizeof(CryptidLogicalExpressionTree));
    char *rightAuthorizationFormulaString = buildRandomAuthorizationTree(
        booleanTree.rightChild, depth - 1, testType);

    authorizationFormulaString = (char *)calloc(
        strlen(leftAuthorizationFormulaString) + strlen(operatorString) +
            strlen(rightAuthorizationFormulaString) + 1,
        sizeof(char));

    strcpy(authorizationFormulaString, leftAuthorizationFormulaString);
    strcat(authorizationFormulaString, operatorString);
    strcat(authorizationFormulaString, rightAuthorizationFormulaString);
  }

  *authorizationFormula = booleanTree;

  return authorizationFormulaString;
}

static void formal_language_ibe_benchmark(benchmark::State& state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  char *identityAlpha = "{\"name\": \"OGS\"}";
  char *identityBeta = "{\"video title\": \"Probalj meg nem megelégedni\", \"megelegedesek\": \"3\"}";
  
  BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
      publicParametersBF;
  HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHess;

  BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBF;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecretHess;

  CryptidLogicalExpressionTree *authorizationFormula =
      (CryptidLogicalExpressionTree *)calloc(1, sizeof(CryptidLogicalExpressionTree));

  authorizationFormula->value =
      malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)authorizationFormula->value = OR;

  authorizationFormula->leftChild =
      (CryptidLogicalExpressionTree *)calloc(1, sizeof(CryptidLogicalExpressionTree));
  char *leftChild = ".*\".*\": \".*\".*";
  authorizationFormula->leftChild->value =
      malloc(strlen(leftChild) + 1 * sizeof(char));
  strcpy((char *)authorizationFormula->leftChild->value, leftChild);

  char *authorizationFormulaLeftString = ".*\".*\": \".*\".* OR ";

  authorizationFormula->rightChild =
      (CryptidLogicalExpressionTree *)calloc(1, sizeof(CryptidLogicalExpressionTree));
  char *authorizationFormulaRightString =
      buildRandomAuthorizationTree(authorizationFormula->rightChild, 25, RANDOM);

  char *authorizationFormulaString =
      (char *)malloc(strlen(authorizationFormulaLeftString) +
             strlen(authorizationFormulaRightString) + 1 * sizeof(char));
  strcpy(authorizationFormulaString, authorizationFormulaLeftString);
  strcat(authorizationFormulaString, authorizationFormulaRightString);

  for (auto _ : state) {

    CryptidStatus status = cryptid_ibe_formalLanguage_setup(
        &masterSecretBF, &publicParametersBF, &masterSecretHess,
        &publicParametersHess, securityLevel);

    AffinePointAsBinary signatureKey;
    status = cryptid_ibs_hess_extract(&signatureKey, identityAlpha,
                                      strlen(identityAlpha), masterSecretHess,
                                      publicParametersHess);

    HessIdentityBasedSignatureSignatureAsBinary authorizationFormulaSignature;
    status = cryptid_ibe_formalLanguage_signFormula(
        &authorizationFormulaSignature, authorizationFormulaString,
        strlen(authorizationFormulaString), identityAlpha, strlen(identityAlpha),
        signatureKey, publicParametersHess);

    char *encryptionKey;
    status = cryptid_ibe_formalLanguage_generateEncryptionKey(
        &encryptionKey, identityAlpha, strlen(identityAlpha));

    BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;
    status = cryptid_ibe_formalLanguage_encrypt(
        &ciphertext, message, strlen(message), encryptionKey,
        strlen(encryptionKey), publicParametersBF);

    AffinePointAsBinary privateKey;
    status = cryptid_ibe_formalLanguage_extract(
        &privateKey, authorizationFormula, authorizationFormulaString,
        strlen(authorizationFormulaString), &authorizationFormulaSignature,
        identityAlpha, strlen(identityAlpha), identityBeta, strlen(identityBeta),
        encryptionKey, strlen(encryptionKey), masterSecretBF, publicParametersBF,
        publicParametersHess);

    char *plaintext;
    status = cryptid_ibe_formalLanguage_decrypt(&plaintext, ciphertext,
                                                privateKey, publicParametersBF);
  }
}

static void bsw_abe_benchmark(benchmark::State& state) {

  SecurityLevel securityLevel = LOW;
  char *message = "Online Games Studios";
  int expectedReponse = 1;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
      *accessTreeAsBinary =
          bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
              2, NULL, 0, 100); // AND

  for(int i = 0; i < 100; i++) {
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, ".*\".*\": \".*\".*", strlen(".*\".*\": \".*\".*"), 0);
    accessTreeAsBinary->children[i] = child;
  }

  /*bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child0 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, NULL, 0, 2); // OR
  accessTreeAsBinary->children[0] = child0;

  char *attribute0 = ".*\".*\": \".*\".*";
  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child0_0 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, attribute0, strlen(attribute0), 0);
  child0->children[0] = child0_0;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child0_1 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, attribute0, strlen(attribute0), 0);
  child0->children[1] = child0_1;

  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *child1 =
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
          1, attribute0, strlen(attribute0), 0);
  accessTreeAsBinary->children[1] = child1;*/

  int numAttributes = 1;
  char **attributes = (char **)malloc(sizeof(char *) * numAttributes);
  attributes[0] = ".*\".*\": \".*\".*";

  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey =
      (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey =
        (bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *)malloc(
            sizeof(bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encrypted = (bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *)malloc(sizeof(
            bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
        *secretkeyAsBinary = (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *)malloc(
            sizeof(bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
        *secretkeyAsBinaryNew = (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *)malloc(
            sizeof(bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));
  char *resultNew;
  char *result;

  for (auto _ : state) {

    CryptidStatus status =
        cryptid_abe_bsw_setup(publickey, masterkey, securityLevel);

    status = cryptid_abe_bsw_encrypt(encrypted, accessTreeAsBinary, message,
                                    strlen(message), publickey);

    status = cryptid_abe_bsw_keygen(secretkeyAsBinary, masterkey, attributes,
                                    numAttributes);

    status = cryptid_abe_bsw_delegate(secretkeyAsBinaryNew, secretkeyAsBinary,
                                      attributes, numAttributes);

    status = cryptid_abe_bsw_decrypt(&resultNew, encrypted, secretkeyAsBinaryNew);
    
    status = cryptid_abe_bsw_decrypt(&result, encrypted, secretkeyAsBinary);
  }
  free(result);
  free(resultNew);
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(
      publickey);
  bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
      masterkey);
  bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
      encrypted);
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
      secretkeyAsBinary);
  bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
      secretkeyAsBinaryNew);
}

/*static void BM_SomeFunction(benchmark::State& state) {

  for(int i = 0; i < 1000; i++) {
    printf("asd\n");
  }
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
  }
}*/
// Register the function as a benchmark
//BENCHMARK(formal_language_ibe_benchmark);

BENCHMARK(bsw_abe_benchmark);
// Run the benchmark
BENCHMARK_MAIN();