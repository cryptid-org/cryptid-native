#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/signature/hess/HessIdentityBasedSignature.h"

const char *LOWEST_QUICK_CHECK_ARGUMENT = "--lowest-quick-check";

int isLowestQuickCheck = 0;
int isVerbose = 0;

TEST fresh_hess_ibs_setup_matching_identities(const SecurityLevel securityLevel,
                                              const char *const message,
                                              const char *const identity) {
  HessIdentityBasedSignaturePublicParametersAsBinary publicParameters;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecret;

  CryptidStatus status =
      cryptid_ibs_hess_setup(&masterSecret, &publicParameters, securityLevel);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  AffinePointAsBinary privateKey;
  status = cryptid_ibs_hess_extract(&privateKey, identity, strlen(identity),
                                    masterSecret, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  HessIdentityBasedSignatureSignatureAsBinary signature;
  status =
      cryptid_ibs_hess_sign(&signature, message, strlen(message), identity,
                            strlen(identity), privateKey, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  status =
      cryptid_ibs_hess_verify(message, strlen(message), signature, identity,
                              strlen(identity), publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  hessIdentityBasedSignatureSignatureAsBinary_destroy(signature);
  affineAsBinary_destroy(privateKey);
  free(masterSecret.masterSecret);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(publicParameters);

  PASS();
}

TEST fresh_hess_ibs_setup_different_identities(
    const SecurityLevel securityLevel, const char *const message,
    const char *const signIdentity, const char *const verifyIdentity) {
  HessIdentityBasedSignaturePublicParametersAsBinary publicParameters;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecret;

  CryptidStatus status =
      cryptid_ibs_hess_setup(&masterSecret, &publicParameters, securityLevel);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  AffinePointAsBinary privateKey;
  status =
      cryptid_ibs_hess_extract(&privateKey, signIdentity, strlen(signIdentity),
                               masterSecret, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  HessIdentityBasedSignatureSignatureAsBinary signature;
  status =
      cryptid_ibs_hess_sign(&signature, message, strlen(message), signIdentity,
                            strlen(signIdentity), privateKey, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  status = cryptid_ibs_hess_verify(message, strlen(message), signature,
                                   verifyIdentity, strlen(verifyIdentity),
                                   publicParameters);

  ASSERT_EQ(status, CRYPTID_VERIFICATION_FAILED_ERROR);

  hessIdentityBasedSignatureSignatureAsBinary_destroy(signature);
  affineAsBinary_destroy(privateKey);
  free(masterSecret.masterSecret);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(publicParameters);

  PASS();
}

TEST fresh_hess_ibs_setup_wrong_signature(const SecurityLevel securityLevel,
                                          const char *const message1,
                                          const char *const message2,
                                          const char *const identity) {
  HessIdentityBasedSignaturePublicParametersAsBinary publicParameters;
  HessIdentityBasedSignatureMasterSecretAsBinary masterSecret;

  CryptidStatus status =
      cryptid_ibs_hess_setup(&masterSecret, &publicParameters, securityLevel);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  AffinePointAsBinary privateKey;
  status = cryptid_ibs_hess_extract(&privateKey, identity, strlen(identity),
                                    masterSecret, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  HessIdentityBasedSignatureSignatureAsBinary signature;
  status =
      cryptid_ibs_hess_sign(&signature, message1, strlen(message1), identity,
                            strlen(identity), privateKey, publicParameters);

  ASSERT_EQ(status, CRYPTID_SUCCESS);

  status =
      cryptid_ibs_hess_verify(message2, strlen(message2), signature, identity,
                              strlen(identity), publicParameters);

  ASSERT_EQ(status, CRYPTID_VERIFICATION_FAILED_ERROR);

  hessIdentityBasedSignatureSignatureAsBinary_destroy(signature);
  affineAsBinary_destroy(privateKey);
  free(masterSecret.masterSecret);
  hessIdentityBasedSignaturePublicParametersAsBinary_destroy(publicParameters);

  PASS();
}

static void generateRandomString(char **output, const size_t outputLength,
                                 const char *const alphabet,
                                 const size_t alphabetSize) {
  memset(*output, '\0', outputLength);

  for (size_t i = 0; i < outputLength - 1; i++) {
    size_t index = rand() % (alphabetSize - 1);

    (*output)[i] = alphabet[index];
  }

  (*output)[outputLength - 1] = '\0';
}

SUITE(cryptid_hess_ibs_suite) {
  {
    char *defaultAlphabet =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    unsigned int testParameters[] = {
        20,    LOWEST, 10,   10,     20,    LOWEST, 100,   100,    20,  LOWEST,
        1000,  1000,   20,   LOWEST, 10000, 10000,  5,     LOW,    10,  10,
        5,     LOW,    100,  100,    5,     LOW,    1000,  1000,   5,   LOW,
        10000, 10000,  1,    MEDIUM, 10,    10,     1,     MEDIUM, 100, 100,
        1,     MEDIUM, 1000, 1000,   1,     MEDIUM, 10000, 10000};

    {
      for (int testSuite = 0; testSuite < 12; testSuite++) {
        int offset = testSuite * 4;
        unsigned int caseCount =
            isLowestQuickCheck ? 1 : testParameters[offset];
        SecurityLevel securityLevel = testParameters[offset + 1];
        unsigned int messageLength = testParameters[offset + 2];
        unsigned int identityLength = testParameters[offset + 3];

        if (isLowestQuickCheck && securityLevel != LOWEST) {
          continue;
        }

        for (unsigned int testCase = 0; testCase < caseCount; testCase++) {
          char *message = malloc(messageLength + 1);
          char *identity = malloc(identityLength + 1);

          generateRandomString(&message, messageLength + 1, defaultAlphabet,
                               strlen(defaultAlphabet));
          generateRandomString(&identity, identityLength + 1, defaultAlphabet,
                               strlen(defaultAlphabet));

          RUN_TESTp(fresh_hess_ibs_setup_matching_identities, securityLevel,
                    message, identity);

          free(message);
          free(identity);
        }
      }
    }

    {
      for (int testSuite = 0; testSuite < 12; testSuite++) {
        int offset = testSuite * 4;
        unsigned int caseCount =
            isLowestQuickCheck ? 1 : testParameters[offset];
        SecurityLevel securityLevel = testParameters[offset + 1];
        unsigned int messageLength = testParameters[offset + 2];
        unsigned int identityLength = testParameters[offset + 3];

        if (isLowestQuickCheck && securityLevel != LOWEST) {
          continue;
        }

        for (unsigned int testCase = 0; testCase < caseCount; testCase++) {
          char *message = malloc(messageLength + 1);
          char *signIdentity = malloc(identityLength + 1);
          char *verifyIdentity = malloc(identityLength + 1);

          generateRandomString(&message, messageLength + 1, defaultAlphabet,
                               strlen(defaultAlphabet));

          do {
            generateRandomString(&signIdentity, identityLength + 1,
                                 defaultAlphabet, strlen(defaultAlphabet));
            generateRandomString(&verifyIdentity, identityLength + 1,
                                 defaultAlphabet, strlen(defaultAlphabet));
          } while (strcmp(signIdentity, verifyIdentity) == 0);

          RUN_TESTp(fresh_hess_ibs_setup_different_identities, securityLevel,
                    message, signIdentity, verifyIdentity);

          free(message);
          free(signIdentity);
          free(verifyIdentity);
        }
      }
    }

    {
      for (int testSuite = 0; testSuite < 12; testSuite++) {
        int offset = testSuite * 4;
        unsigned int caseCount =
            isLowestQuickCheck ? 1 : testParameters[offset];
        SecurityLevel securityLevel = testParameters[offset + 1];
        unsigned int messageLength = testParameters[offset + 2];
        unsigned int identityLength = testParameters[offset + 3];

        if (isLowestQuickCheck && securityLevel != LOWEST) {
          continue;
        }

        for (unsigned int testCase = 0; testCase < caseCount; testCase++) {
          char *message1 = malloc(messageLength + 1);
          char *message2 = malloc(messageLength + 1);
          char *identity = malloc(identityLength + 1);

          do {
            generateRandomString(&message1, messageLength + 1, defaultAlphabet,
                                 strlen(defaultAlphabet));
            generateRandomString(&message2, messageLength + 1, defaultAlphabet,
                                 strlen(defaultAlphabet));
          } while (strcmp(message1, message2) == 0);

          generateRandomString(&identity, identityLength + 1, defaultAlphabet,
                               strlen(defaultAlphabet));

          RUN_TESTp(fresh_hess_ibs_setup_wrong_signature, securityLevel,
                    message1, message2, identity);

          free(message1);
          free(message2);
          free(identity);
        }
      }
    }
  }
}

GREATEST_MAIN_DEFS();

void parse_custom_options(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    if (0 == strncmp(LOWEST_QUICK_CHECK_ARGUMENT, argv[i],
                     strlen(LOWEST_QUICK_CHECK_ARGUMENT))) {
      printf("--lowest-quick-check detected: only the LOWEST level will be "
             "checked and with limited iterations.\n");

      isLowestQuickCheck = 1;
    }

    if (0 == strncmp("-v", argv[i], 2)) {
      isVerbose = 1;
    }
  }
}

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  parse_custom_options(argc, argv);

  srand(time(NULL));

  RUN_SUITE(cryptid_hess_ibs_suite);

  GREATEST_MAIN_END();
}
