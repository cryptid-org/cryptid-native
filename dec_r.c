#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "CryptID.h"
#include "util/Validation.h"
#include "util/Utils.h"
#include "util/IO.h"


int main()
{
    PublicParameters publicParameters = readPublicParFromFile();
    (validation_isPublicParametersValid(publicParameters)) ? (printf("PublicParameters valid\n")) : (printf("PublicParameters  invalid\n"));

    AffinePoint privateKey;
    privateKey = readPrivateKeyFromFiles();
    (validation_isAffinePointValid(privateKey, publicParameters.ellipticCurve.fieldOrder)) ? (printf("privateKey valid\n")) : (printf("privateKey  invalid\n"));

    CipherTextTuple ciphertext = readCipherTextFromFile();
    (validation_isCipherTextTupleValid(ciphertext, publicParameters.ellipticCurve.fieldOrder)) ? (printf("read cipher valid\n")) : (printf("read cipher invalid\n"));

    char *plaintext;
    if (CRYPTID_SUCCESS != cryptid_decrypt(&plaintext, privateKey, ciphertext, publicParameters))
    {
        printf("Decrypt failed\n");
        return -1;
    }

    printf("Plaintext:\n%s\n", plaintext);

    free(plaintext);
    cipherTextTuple_destroy(ciphertext);
    affine_destroy(privateKey);
    mpz_clear(publicParameters.q);
    affine_destroy(publicParameters.pointP);
    affine_destroy(publicParameters.pointPpublic);
    ellipticCurve_destroy(publicParameters.ellipticCurve);

    return 0;

}
