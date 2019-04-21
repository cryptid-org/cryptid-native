#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "AffinePoint.h"
#include "Complex.h"
#include "EllipticCurve.h"
#include "SignID.h"

int main(int argc, char **argv)
{
    Status status;
    PublicParameters publicParameters;
    mpz_t masterSecret;
    mpz_init(masterSecret);

    status = signid_setup(LOWEST, &publicParameters, masterSecret);

    printf("setup: %d\n", status);

    AffinePoint privateKey;
    char* identity = "adam\0";
    status = signid_extract(&privateKey, identity, strlen(identity), publicParameters, masterSecret);

    printf("extract: %d\n", status);

    Signature signature;
    char* message = "This really needs to be signed.\0";
    status = signid_sign(&signature, privateKey, message, strlen(message), identity, strlen(identity), publicParameters);

    printf("sign: %d\n", status);

    status = signid_verify(message, strlen(message), signature, identity, strlen(identity), publicParameters);

    printf("verify: %d\n", status);

}