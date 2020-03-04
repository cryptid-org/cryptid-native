#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "greatest.h"

#include "attribute-based/ABE.h"

TEST basic_abe_test(SecurityLevel securityLevel, char* message, AccessTree* accessTree, char** attributes, int expectedReponse)
{
    PublicKey_ABE* publickey = malloc(sizeof (PublicKey_ABE));
    MasterKey_ABE* masterkey = malloc(sizeof (MasterKey_ABE));
    CryptidStatus status = cryptid_setup_ABE(securityLevel, publickey, masterkey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);


    EncryptedMessage_ABE* encrypted = malloc(sizeof (EncryptedMessage_ABE));
    status = cryptid_encrypt_ABE(encrypted, message, strlen(message), publickey, accessTree);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    SecretKey_ABE* secretkey = malloc(sizeof (SecretKey_ABE));

    status = cryptid_keygen_ABE(masterkey, attributes, secretkey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    SecretKey_ABE* secretkey_new = malloc(sizeof (SecretKey_ABE));

    status = cryptid_delegate_ABE(secretkey, attributes, secretkey_new);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    char* result_new;
    CryptidStatus status_new = cryptid_decrypt_ABE(&result_new, encrypted, secretkey_new);
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    char* result;
    status = cryptid_decrypt_ABE(&result, encrypted, secretkey);

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

    destroyPublicKey_ABE(publickey);
    destroyMasterKey_ABE(masterkey);
    destroyEncryptedMessage_ABE(encrypted);
    destroySecretKey_ABE(secretkey);
    destroySecretKey_ABE(secretkey_new);

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

        char** attributes = attributeArray();
        int numChilds = 5;
        AccessTree* accessTree = createTree((i%2)*(numChilds-1)+1, NULL, 0);
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
            AccessTree* child = createTree(1, randomStr2, strlen(randomStr2));
            accessTree->children[a] = child;
            attributes[a] = randomStr;
        }

        RUN_TESTp(basic_abe_test, LOWEST, message, accessTree, attributes, expectedReponse);

        free(message);
        destroyTree(accessTree);
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