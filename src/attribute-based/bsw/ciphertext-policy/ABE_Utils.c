#include "attribute-based/bsw/ciphertext-policy/ABE_Utils.h"
#include <stdlib.h>

int Lagrange_coefficient(int xi, int* S, int sLength, int x)
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
void ABE_randomNumber(mpz_t randElement, PublicKey_ABE* publickey)
{
    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

    random_mpzInRange(randElement, pMinusOne);

    mpz_clear(pMinusOne);
}

// Simple way of creating an array of attributes (for secretkey)
char** attributeArray() {
    char **attributes = malloc(MAX_ATTRIBUTES*sizeof(char)*(ATTRIBUTE_LENGTH+1));

    for(size_t i = 0; i < MAX_ATTRIBUTES; i++)
    {
       attributes[i] = "";
    }

    return attributes;
}

// Returning whether an array of attributes contains a specific attribute
int hasAttribute(char** attributes, char* val) {
    if(val != NULL)
    {
        for(int i = 0; i < MAX_ATTRIBUTES; i++)
        {
            if(attributes[i] && attributes[i][0] != '\0' && strcmp(attributes[i], val) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}