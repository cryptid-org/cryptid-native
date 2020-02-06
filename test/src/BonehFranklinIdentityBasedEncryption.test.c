#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "greatest.h"

#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryption.h"
#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"

const char *LOWEST_QUICK_CHECK_ARGUMENT = "--lowest-quick-check";

int isLowestQuickCheck = 0;
int isVerbose = 0;

TEST fresh_boneh_franklin_ibe_setup_matching_identities(const SecurityLevel securityLevel, const char *const message, const char *const identity)
{
    BonehFranklinIdentityBasedEncryptionPublicParameters* publicParameters = malloc(sizeof (BonehFranklinIdentityBasedEncryptionPublicParameters));
    mpz_t masterSecret;
    mpz_init(masterSecret);
    mpz_init(publicParameters->q);

    CryptidStatus status = cryptid_ibe_bonehFranklin_setup(securityLevel, publicParameters, masterSecret);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    AffinePoint privateKey;
    status = cryptid_ibe_bonehFranklin_extract(&privateKey, identity, strlen(identity), *publicParameters, masterSecret);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BonehFranklinIdentityBasedEncryptionCipherText* ciphertext = malloc(sizeof (BonehFranklinIdentityBasedEncryptionCipherText));
    status = cryptid_ibe_bonehFranklin_encrypt(ciphertext, message, strlen(message), identity, strlen(identity), *publicParameters);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    char *plaintext;
    status = cryptid_ibe_bonehFranklin_decrypt(&plaintext, privateKey, *ciphertext, *publicParameters);

    ASSERT_EQ(status, CRYPTID_SUCCESS);
    ASSERT_EQ(strcmp(message, plaintext), 0);

    free(plaintext);
    bonehFranklinIdentityBasedEncryptionCipherText_destroy(*ciphertext);
    free(ciphertext);
    affine_destroy(privateKey);
    mpz_clears(publicParameters->q, masterSecret, NULL);
    affine_destroy(publicParameters->pointP);
    affine_destroy(publicParameters->pointPpublic);
    ellipticCurve_destroy(publicParameters->ellipticCurve);
    free(publicParameters);

    PASS();
}

TEST fresh_boneh_franklin_ibe_setup_different_identities(const SecurityLevel securityLevel, const char *const message, const char *const encryptIdentity, const char *const decryptIdentity)
{
    BonehFranklinIdentityBasedEncryptionPublicParameters* publicParameters = malloc(sizeof (BonehFranklinIdentityBasedEncryptionPublicParameters));
    mpz_t masterSecret;
    mpz_init(masterSecret);
    mpz_init(publicParameters->q);

    CryptidStatus status = cryptid_ibe_bonehFranklin_setup(securityLevel, publicParameters, masterSecret);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    AffinePoint privateKey;
    status = cryptid_ibe_bonehFranklin_extract(&privateKey, decryptIdentity, strlen(decryptIdentity), *publicParameters, masterSecret);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BonehFranklinIdentityBasedEncryptionCipherText* ciphertext = malloc(sizeof (BonehFranklinIdentityBasedEncryptionCipherText));
    status = cryptid_ibe_bonehFranklin_encrypt(ciphertext, message, strlen(message), encryptIdentity, strlen(encryptIdentity), *publicParameters);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    char *plaintext;
    status = cryptid_ibe_bonehFranklin_decrypt(&plaintext, privateKey, *ciphertext, *publicParameters);

    ASSERT_EQ(status, CRYPTID_DECRYPTION_FAILED_ERROR);

    bonehFranklinIdentityBasedEncryptionCipherText_destroy(*ciphertext);
    free(ciphertext);
    affine_destroy(privateKey);
    mpz_clears(publicParameters->q, masterSecret, NULL);
    affine_destroy(publicParameters->pointP);
    affine_destroy(publicParameters->pointPpublic);
    ellipticCurve_destroy(publicParameters->ellipticCurve);
    free(publicParameters);

    PASS();
}

static void generateRandomString(char** output, const size_t outputLength, const char *const alphabet, const size_t alphabetSize)
{
    memset(*output, '\0', outputLength);
    
    for (size_t i = 0; i < outputLength - 1; i++)
    {
        size_t index = rand() % (alphabetSize - 1);

        (*output)[i] = alphabet[index];
    }

    (*output)[outputLength - 1] = '\0';
}

SUITE(cryptid_boneh_franklin_ibe_suite)
{
    {
        char* defaultAlphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        unsigned int testParameters[] = {
            20, LOWEST, 10, 10,
            20, LOWEST, 100, 100,
            20, LOWEST, 1000, 1000,
            20, LOWEST, 10000, 10000,
            5, LOW, 10, 10,
            5, LOW, 100, 100,
            5, LOW, 1000, 1000,
            5, LOW, 10000, 10000,
            1, MEDIUM, 10, 10,
            1, MEDIUM, 100, 100,
            1, MEDIUM, 1000, 1000,
            1, MEDIUM, 10000, 10000
        };

        {
            for (int testSuite = 0; testSuite < 12; testSuite++)
            {
                int offset = testSuite * 4;
                unsigned int caseCount = isLowestQuickCheck ? 1 : testParameters[offset];
                SecurityLevel securityLevel = testParameters[offset + 1];
                unsigned int messageLength = testParameters[offset + 2];
                unsigned int identityLength = testParameters[offset + 3];

                if (isLowestQuickCheck && securityLevel != LOWEST) {
                    continue;
                }

                for (unsigned int testCase = 0; testCase < caseCount; testCase++)
                {
                    char* message = malloc(messageLength + 1);
                    char* identity = malloc(identityLength + 1);

                    generateRandomString(&message, messageLength + 1, defaultAlphabet, strlen(defaultAlphabet));
                    generateRandomString(&identity, identityLength + 1, defaultAlphabet, strlen(defaultAlphabet));

                    RUN_TESTp(fresh_boneh_franklin_ibe_setup_matching_identities, securityLevel, message, identity);

                    free(message);
                    free(identity);
                }
            }
        }

        {
            for (int testSuite = 0; testSuite < 12; testSuite++)
            {
                int offset = testSuite * 4;
                unsigned int caseCount = isLowestQuickCheck ? 1 : testParameters[offset];
                SecurityLevel securityLevel = testParameters[offset + 1];
                unsigned int messageLength = testParameters[offset + 2];
                unsigned int identityLength = testParameters[offset + 3];

                if (isLowestQuickCheck && securityLevel != LOWEST) {
                    continue;
                }

                for (unsigned int testCase = 0; testCase < caseCount; testCase++)
                {
                    char* message = malloc(messageLength + 1);
                    char* encryptIdentity = malloc(identityLength + 1);
                    char* decryptIdentity = malloc(identityLength + 1);

                    generateRandomString(&message, messageLength + 1, defaultAlphabet, strlen(defaultAlphabet));

                    do {
                        generateRandomString(&encryptIdentity, identityLength + 1, defaultAlphabet, strlen(defaultAlphabet));
                        generateRandomString(&decryptIdentity, identityLength + 1, defaultAlphabet, strlen(defaultAlphabet));
                    } while (strcmp(encryptIdentity, decryptIdentity) == 0);

                    RUN_TESTp(fresh_boneh_franklin_ibe_setup_different_identities, securityLevel, message, encryptIdentity, decryptIdentity);

                    free(message);
                    free(encryptIdentity);
                    free(decryptIdentity);
                }
            }
        }
    }
}

GREATEST_MAIN_DEFS();

void parse_custom_options(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (0 == strncmp(LOWEST_QUICK_CHECK_ARGUMENT, argv[i], strlen(LOWEST_QUICK_CHECK_ARGUMENT))) 
        {
            printf("--lowest-quick-check detected: only the LOWEST level will be checked and with limited iterations.\n");

            isLowestQuickCheck = 1;
        }

        if (0 == strncmp("-v", argv[i], 2))
        {
            isVerbose = 1;
        }
    }
}

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    
    parse_custom_options(argc, argv);

    srand(time(NULL));

    RUN_SUITE(cryptid_boneh_franklin_ibe_suite);

    GREATEST_MAIN_END();
}
