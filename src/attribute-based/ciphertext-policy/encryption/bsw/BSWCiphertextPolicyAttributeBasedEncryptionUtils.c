#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include <stdlib.h>

int Lagrange_coefficient(const int xi, const int *s, const int sLength,
                         const int x) {
  double result = 1;
  for (int i = 0; i < sLength; i++) {
    if (&s[i] != NULL) {
      if (s[i] != xi) {
        double xD = (double)x;
        double j = (double)s[i];
        double xiD = (double)xi;
        result = result * ((xD - j) / (xiD - j));
      }
    }
  }
  return (int)result;
}

char *concat(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

// Used for generating random numbers in the elliptic curve's fieldOrder of the
// publickey
void bswCiphertextPolicyAttributeBasedEncryptionRandomNumber(
    mpz_t randElement,
    const bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey) {
  mpz_t pMinusOne;
  mpz_init(pMinusOne);
  mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

  random_mpzInRange(randElement, pMinusOne);

  mpz_clear(pMinusOne);
}

// Returning whether an array of attributes contains a specific attribute
int bswCiphertextPolicyAttributeBasedEncryptionHasAttribute(
    char **attributes, const int numAttributes, const char *val) {
  if (val != NULL) {
    for (int i = 0; i < numAttributes; i++) {
      if (attributes[i] && attributes[i][0] != '\0' &&
          strcmp(attributes[i], val) == 0) {
        return 1;
      }
    }
  }
  return 0;
}