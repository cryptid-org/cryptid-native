#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "CryptID.h"
#include "util/Validation.h"
#include "util/Utils.h"


int main()
{
    PublicParameters publicParameters2 = readPublicParFromFile();
    (validation_isTypeOneEllipticCurve(publicParameters2.ellipticCurve)) ? (printf("TypeOneEllipticCurve valid\n")) : (printf("TypeOneEllipticCurve invalid\n"));
    (validation_isProbablePrime(publicParameters2.q)) ? (printf("ProbablePrime q valid\n")) : (printf("ProbablePrime q invalid\n"));
    (validation_isAffinePointValid(publicParameters2.pointP, publicParameters2.ellipticCurve.fieldOrder)) ? (printf("pointP valid\n")) : (printf("pointP invalid\n"));
    (validation_isAffinePointValid(publicParameters2.pointPpublic, publicParameters2.ellipticCurve.fieldOrder)) ? (printf("pointPpublic valid\n")) : (printf("pointPpublic invalid\n"));
    (validation_isPublicParametersValid(publicParameters2)) ? (printf("PublicParameters valid\n")) : (printf("PublicParameters  invalid\n"));

    AffinePoint pk2;
    pk2 = readPrivateKeyFromFiles();

    (validation_isAffinePointValid(pk2, publicParameters2.ellipticCurve.fieldOrder)) ? (printf("privateKey valid\n")) : (printf("privateKey  invalid\n"));

    CipherTextTuple ct = readCipherTextFromFile();

    (validation_isAffinePointValid(ct.cipherU, publicParameters2.ellipticCurve.fieldOrder)) ? (printf("cipherU aff point valid\n")) : (printf("cipherU aff point invalid\n"));
    (ct.cipherV) ? (printf("cipherV valid\n")) : (printf("cipherV invalid\n"));
    (ct.cipherVLength != 0) ? (printf("cipherVlength valid\n")) : (printf("cipherVlength invalid\n"));
    (ct.cipherW) ? (printf("cipherW valid\n")) : (printf("cipherW invalid\n"));
    (ct.cipherWLength != 0) ? (printf("cipherWlength valid\n")) : (printf("cipherWlength invalid\n"));


    (validation_isCipherTextTupleValid(ct, publicParameters2.ellipticCurve.fieldOrder)) ? (printf("read cipher valid\n")) : (printf("read cipher invalid\n"));

    char *plaintext;
    if (CRYPTID_SUCCESS != cryptid_decrypt(&plaintext, pk2, ct, publicParameters2))
    {
        printf("Decrypt failed\n");
        return -1;
    }

    printf("Plaintext:\n%s\n", plaintext);

}
