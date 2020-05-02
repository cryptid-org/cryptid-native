#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "greatest.h"

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryption.h"

TEST basic_abe_test(SecurityLevel securityLevel, char* message, bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary, char** attributes, int numAttributes, int expectedReponse)
{
    bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickey = malloc(sizeof (bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
    bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkey = malloc(sizeof (bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary));

    CryptidStatus status = cryptid_abe_bsw_setup(publickey, masterkey, securityLevel);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encrypted = malloc(sizeof (bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary));
    status = cryptid_abe_bsw_encrypt(encrypted, accessTreeAsBinary, message, strlen(message), publickey);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary = malloc(sizeof (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));

    status = cryptid_abe_bsw_keygen(secretkeyAsBinary, masterkey, attributes, numAttributes);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinaryNew = malloc(sizeof (bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary));

    status = cryptid_abe_bsw_delegate(secretkeyAsBinaryNew, secretkeyAsBinary, attributes, numAttributes);
    ASSERT_EQ(status, CRYPTID_SUCCESS);

    char* resultNew;
    CryptidStatus statusNew = cryptid_abe_bsw_decrypt(&resultNew, encrypted, secretkeyAsBinaryNew);
    char* result;
    status = cryptid_abe_bsw_decrypt(&result, encrypted, secretkeyAsBinary);

    if(expectedReponse == 1)
    {
        ASSERT_EQ(statusNew, CRYPTID_SUCCESS);
        ASSERT_EQ(status, CRYPTID_SUCCESS);
        ASSERT_EQ(strcmp(result, message), 0);
        ASSERT_EQ(strcmp(resultNew, message), 0);

        free(result);
        free(resultNew);
    }
    else
    {
        ASSERT_EQ(statusNew, CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR);
        ASSERT_EQ(status, CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR);
    }

    bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(publickey);
    bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(masterkey);
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(encrypted);
    bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(secretkeyAsBinary);
    bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(secretkeyAsBinaryNew);

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

    for(int i = 0; i < 2; i++)
    {
        int messageLength = rand() % (1000 - 1) + 1;
        char* message = malloc(messageLength + 1);
        generateRandomString(&message, messageLength + 1, defaultAlphabet, strlen(defaultAlphabet));

        int numChilds = 5;
        int numAttributes = 5;
        char** attributes = malloc(numAttributes*(20+1));
        bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init((i%2)*(numChilds-1)+1, NULL, 0, numChilds);
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
            bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* child = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(1, randomStr2, strlen(randomStr2), 0);
            accessTreeAsBinary->children[a] = child;
            attributes[a] = malloc(strlen(randomStr)+1);
            strcpy(attributes[a], randomStr);
        }

        RUN_TESTp(basic_abe_test, LOWEST, message, accessTreeAsBinary, attributes, numAttributes, expectedReponse);

        free(message);
        bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(accessTreeAsBinary);
        for(int a = 0; a < numChilds; a++)
        {
            free(attributes[a]);
        }
        free(attributes);

        free(randomStr);
        free(randomStr2);
    }

    // targeted test

    char* message = "It works!";

    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(2, NULL, 0, 2); // AND
    
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* child0 = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(1, NULL, 0, 2); // OR
    accessTreeAsBinary->children[0] = child0;

    char* attribute0 = "developer";
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* child0_0 = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(1, attribute0, strlen(attribute0), 0);
    child0->children[0] = child0_0;

    char* attribute1 = "reviewer";
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* child0_1 = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(1, attribute1, strlen(attribute1), 0);
    child0->children[1] = child0_1;

    char* attribute2 = "CryptID";
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* child1 = bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(1, attribute2, strlen(attribute2), 0);
    accessTreeAsBinary->children[1] = child1;

    int numAttributes = 2;
    char** attributesGood = malloc(sizeof(char*) * numAttributes);
    attributesGood[0] = attribute0;
    attributesGood[1] = attribute2;

    char** attributesBad = malloc(sizeof(char*) * numAttributes);
    attributesBad[0] = "guest";
    attributesBad[1] = attribute2;

    RUN_TESTp(basic_abe_test, LOWEST, message, accessTreeAsBinary, attributesGood, numAttributes, 1);
    RUN_TESTp(basic_abe_test, LOWEST, message, accessTreeAsBinary, attributesBad, numAttributes, 0);

    free(attributesGood);
    free(attributesBad);
    bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(accessTreeAsBinary);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    srand(time(NULL));

    RUN_SUITE(cryptid_abe_suite);

    GREATEST_MAIN_END();
}