#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "greatest.h"

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryption.h"

TEST basic_abe_test(SecurityLevel securityLevel, char* message, BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary, char** attributes, int num_attributes, int expectedReponse)
{
    BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
    BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));

    CryptidStatus status = cryptid_abe_bsw_setup(securityLevel, publickey, masterkey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encrypted = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));
    status = cryptid_abe_bsw_encrypt(encrypted, message, strlen(message), publickey, accessTreeAsBinary);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));

    status = cryptid_abe_bsw_keygen(masterkey, attributes, num_attributes, secretkeyAsBinary);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary_new = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));

    status = cryptid_abe_bsw_delegate(secretkeyAsBinary, attributes, num_attributes, secretkeyAsBinary_new);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    char* result_new;
    CryptidStatus status_new = cryptid_abe_bsw_decrypt(&result_new, encrypted, secretkeyAsBinary_new);
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    char* result;
    status = cryptid_abe_bsw_decrypt(&result, encrypted, secretkeyAsBinary);

    if(expectedReponse == 1)
    {
        ASSERT_EQ(status_new, CRYPTID_SUCCESS);
        ASSERT_EQ(status, CRYPTID_SUCCESS);
        ASSERT_EQ(strcmp(result, message), 0);
        ASSERT_EQ(strcmp(result_new, message), 0);

        free(result);
        free(result_new);
    }
    else
    {
        ASSERT_EQ(status_new, CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR);
        ASSERT_EQ(status, CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR);
    }

    BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(publickey);
    BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(masterkey);
    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(encrypted);
    BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(secretkeyAsBinary);
    BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(secretkeyAsBinary_new);

    PASS();
}

static void generateRandomString(char** output, size_t outputLength, char* alphabet, size_t alphabetSize)
{
    memset(*output, '\0', outputLength);
    
    for (size_t i = 0; i < outputLength - 1; i++)
    {
        size_t index = rand() % (alphabetSize - 1);

        (*output)[i] = alphabet[index];
    }

    (*output)[outputLength - 1] = '\0';
}

SUITE(cryptid_abe_suite)
{
    char* defaultAlphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for(int i = 0; i < 1; i++)
    {
        int messageLength = rand() % (1000 - 1) + 1;
        char* message = malloc(messageLength + 1);
        generateRandomString(&message, messageLength + 1, defaultAlphabet, strlen(defaultAlphabet));

        int numChilds = 5;
        int num_attributes = 5;
        char** attributes = malloc(num_attributes*(20+1));
        BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary = BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init((i%2)*(numChilds-1)+1, NULL, 0, numChilds);
        char* randomStr = malloc(20 + 1);
        char* randomStr2 = malloc(20 + 1);
        int expectedReponse = 1;
        if(i >= 4)
        {
            expectedReponse = 0;
        }
        for(int a = 0; a < numChilds; a++)
        {
            generateRandomString(&randomStr, 20, defaultAlphabet, strlen(defaultAlphabet));
            if(expectedReponse == 0)
            {
                generateRandomString(&randomStr2, 20, defaultAlphabet, strlen(defaultAlphabet));
            }
            else
            {
                strcpy(randomStr2, randomStr);
            }
            BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* child = BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(1, randomStr2, strlen(randomStr2), 0);
            accessTreeAsBinary->children[a] = child;
            attributes[a] = randomStr;
        }

        RUN_TESTp(basic_abe_test, LOWEST, message, accessTreeAsBinary, attributes, num_attributes, expectedReponse);

        free(message);
        bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(accessTreeAsBinary);
        free(attributes);

        free(randomStr);
        free(randomStr2);
    }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    srand(time(NULL));

    RUN_SUITE(cryptid_abe_suite);

    GREATEST_MAIN_END();
}