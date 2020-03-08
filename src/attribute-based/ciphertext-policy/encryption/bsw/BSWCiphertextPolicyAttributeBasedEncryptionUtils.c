#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include <stdlib.h>

int Lagrange_coefficient(const int xi, const int* S, const int sLength, const int x)
{
    double result = 1;
    for(int i = 0; i < sLength; i++)
    {
        if(&S[i] != NULL)
        {
            if(S[i] != xi)
            {
                double xD = (double) x;
                double j = (double) S[i];
                double xiD = (double) xi;
                result = result*((xD-j)/(xiD-j));
            }
        }
    }
    return (int) result;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// Used for generating random numbers in the elliptic curve's fieldOrder of the publickey 
void BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(mpz_t randElement, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey)
{
    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

    random_mpzInRange(randElement, pMinusOne);

    mpz_clear(pMinusOne);
}

// Returning whether an array of attributes contains a specific attribute
int BSWCiphertextPolicyAttributeBasedEncryptionHasAttribute(char** attributes, const int num_attributes, const char* val) {
    if(val != NULL)
    {
        for(int i = 0; i < num_attributes; i++)
        {
            if(attributes[i] && attributes[i][0] != '\0' && strcmp(attributes[i], val) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}