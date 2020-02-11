#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "CryptID_ABE.h"


int main()
{
    const char *message = "a";

    PublicKey_ABE* publickey = malloc(sizeof (PublicKey_ABE));
    MasterKey_ABE* masterkey = malloc(sizeof (MasterKey_ABE));
    if (CRYPTID_SUCCESS != cryptid_setup_ABE(LOWEST, publickey, masterkey))
    {
        printf("Setup failed\n");
        return -1;
    }

    char *attribute = "test";
    AccessTree* children = childrenArray();
    AccessTree* accessTree = createTree(1, children, attribute, strlen(attribute));

    EncryptedMessage_ABE* encrypted = malloc(sizeof (EncryptedMessage_ABE));
    cryptid_encrypt_ABE(encrypted, message, strlen(message), publickey, accessTree);

    SecretKey_ABE* secretkey = malloc(sizeof (SecretKey_ABE));
    char** attributes = attributeArray();
    attributes[0] = "test";
    cryptid_keygen_ABE(masterkey, attributes, secretkey);

    char* result = malloc(sizeof(char)*100);
    cryptid_decrypt_ABE(result, encrypted, secretkey);

    return 0;
}