#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "greatest.h"

#include "attribute-based/ABE.h"

TEST basic_abe_test(SecurityLevel securityLevel, char* message, AccessTree* accessTree, char** attributes)
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

    char* result;
    status = cryptid_decrypt_ABE(&result, encrypted, secretkey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    ASSERT_EQ(strcmp(result, message), 0);

    free(result);
    destroyPublicKey_ABE(publickey);
    mpz_clear(masterkey->beta);
    affine_destroy(masterkey->g_alpha);
    free(masterkey);
    destroyEncryptedMessage_ABE(encrypted);
    destroySecretKey_ABE(secretkey);

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

    for(int i = 0; i < 4; i++)
    {
        int messageLength = rand() % (1000 - 1) + 1;
        char* message = malloc(messageLength + 1);
        generateRandomString(&message, messageLength + 1, defaultAlphabet, strlen(defaultAlphabet));

        char** attributes = attributeArray();
        int numChilds = 1;
        AccessTree* accessTree = createTree((i%2)*(numChilds-1)+1, NULL, 0);
        char* randomStr = malloc(20 + 1);
        for(int a = 0; a < numChilds; a++)
        {
            generateRandomString(&randomStr, 20, defaultAlphabet, strlen(defaultAlphabet));
            AccessTree* child = createTree(1, randomStr, strlen(randomStr));
            accessTree->children[a] = child;
            attributes[a] = randomStr;
        }

        RUN_TESTp(basic_abe_test, LOWEST, message, accessTree, attributes);

        free(message);
        destroyTree(accessTree);
        free(attributes);

        free(randomStr);
    }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(cryptid_abe_suite);

    GREATEST_MAIN_END();
}