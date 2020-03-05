#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "greatest.h"

#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryption.h"

TEST basic_abe_test(SecurityLevel securityLevel, char* message, BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, char** attributes, int num_attributes, int expectedReponse)
{
    BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKey));
    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionMasterKey));
    CryptidStatus status = cryptid_abe_bsw_setup(securityLevel, publickey, masterkey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);


    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage));
    status = cryptid_abe_bsw_encrypt(encrypted, message, strlen(message), publickey, accessTree);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKey));

    status = cryptid_abe_bsw_keygen(masterkey, attributes, num_attributes, secretkey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey_new = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKey));

    status = cryptid_abe_bsw_delegate(secretkey, attributes, num_attributes, secretkey_new);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    char* result_new;
    CryptidStatus status_new = cryptid_abe_bsw_decrypt(&result_new, encrypted, secretkey_new);
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    char* result;
    status = cryptid_abe_bsw_decrypt(&result, encrypted, secretkey);

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

    BSWCiphertextPolicyAttributeBasedEncryptionPublicKey_destroy(publickey);
    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey_destroy(masterkey);
    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(encrypted);
    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(secretkey);
    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(secretkey_new);

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

    for(int i = 0; i < 6; i++)
    {
        int messageLength = rand() % (1000 - 1) + 1;
        char* message = malloc(messageLength + 1);
        generateRandomString(&message, messageLength + 1, defaultAlphabet, strlen(defaultAlphabet));

        int numChilds = 5;
        int num_attributes = 5;
        char** attributes = malloc(num_attributes*(20+1));
        BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree = BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_init((i%2)*(numChilds-1)+1, NULL, 0, numChilds);
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
            BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* child = BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_init(1, randomStr2, strlen(randomStr2), 0);
            accessTree->children[a] = child;
            attributes[a] = randomStr;
        }

        RUN_TESTp(basic_abe_test, LOWEST, message, accessTree, attributes, num_attributes, expectedReponse);

        free(message);
        BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(accessTree);
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