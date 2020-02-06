#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util/Utils.h"
#include "util/IO.h"

const int BASE = 10;

void writePublicParToFiles(PublicParameters* publicParameters)
{
    FILE * fp;
    struct stat st = {0};

    if (stat("./PP", &st) == -1) {
        mkdir("./PP", 0700);
    }

    fp = fopen ("PP/PP.ellipticCurve.a","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->ellipticCurve.a);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.b","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->ellipticCurve.b);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.fieldOrder","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->ellipticCurve.fieldOrder);
        fclose(fp);
    }

    fp = fopen ("PP/PP.q","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->q);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointP.x","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointP.x);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointP.y","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointP.y);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointPpublic.x","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointPpublic.x);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointPpublic.y","w+");
    if (fp != NULL)
    {
        mpz_out_str(fp, BASE, publicParameters->pointPpublic.y);
        fclose(fp);
    }

    fp = fopen ("PP/PP.hashf","wb");
    if (fp != NULL) {

        fwrite(&publicParameters->hashFunction, sizeof(publicParameters->hashFunction), 1, fp);

        fclose(fp);
    }

}

void writeCipherTextToFiles(CipherTextTuple* ciphertext)
{
    FILE * fp;
    struct stat st = {0};

    if (stat("./CT", &st) == -1) {
        mkdir("./CT", 0700);
    }


    fp = fopen ("CT/cipher","wb");
    if (fp != NULL) {

        //fwrite(&ciphertext->cipherU, sizeof(ciphertext->cipherU), 1, fp);

        fwrite(&ciphertext->cipherVLength, sizeof(ciphertext->cipherVLength), 1, fp);
        fwrite(ciphertext->cipherV, ciphertext->cipherVLength + 1, 1, fp);

        fwrite(&ciphertext->cipherWLength, sizeof(ciphertext->cipherWLength),1, fp);
        fwrite(ciphertext->cipherW, ciphertext->cipherWLength + 1, 1, fp);

        fclose(fp);
    }
    printf("beleirt:\n");
    printf("VLength: %lu\n", ciphertext->cipherVLength);
    printf("%s\n", ciphertext->cipherV);
    printf("%lu\n", ciphertext->cipherWLength);
    printf("%s\n", ciphertext->cipherW);

    fp = fopen ("CT/cipherUx","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, ciphertext->cipherU.x);
        fclose(fp);
    }
    printf ("  wrote  "); mpz_out_str (stdout, BASE, ciphertext->cipherU.x); printf("\n");

    fp = fopen ("CT/cipherUy","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, ciphertext->cipherU.y);
        fclose(fp);
    }
    printf ("  wrote  "); mpz_out_str (stdout, BASE, ciphertext->cipherU.y); printf("\n");
}

void writePrivateKeyToFiles(AffinePoint privateKey)
{
    FILE * fp;
    struct stat st = {0};

    if (stat("./PK", &st) == -1) {
        mkdir("./PK", 0700);
    }


    fp = fopen ("PK/privateX","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, privateKey.x);
        fclose(fp);
    }

    fp = fopen ("PK/privateY","w+");
    if (fp != NULL) {
        mpz_out_str(fp, BASE, privateKey.y);
        fclose(fp);
    }
}

PublicParameters readPublicParFromFile()
{
    FILE * fp;

    PublicParameters publicParameters;

    mpz_t a, b, fieldOrder, q, px, py, ppx, ppy;
    mpz_inits(a, b, fieldOrder, q, px, py, ppx, ppy, NULL);

    fp = fopen ("PP/PP.ellipticCurve.a","r");
    if (fp != NULL)
    {
        mpz_inp_str(a, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.b","r");
    if (fp != NULL)
    {
        mpz_inp_str(b, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.ellipticCurve.fieldOrder","r");
    if (fp != NULL)
    {
        mpz_inp_str(fieldOrder, fp, BASE);
        fclose(fp);
    }
    publicParameters.ellipticCurve = ellipticCurve_init(a, b, fieldOrder);

    fp = fopen ("PP/PP.q","r");
    if (fp != NULL)
    {
        mpz_inp_str(q, fp, BASE);
        fclose(fp);
    }
    mpz_init(publicParameters.q);
    mpz_set(publicParameters.q, q);

    fp = fopen ("PP/PP.pointP.x","r");
    if (fp != NULL)
    {
        mpz_inp_str(px, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointP.y","r");
    if (fp != NULL)
    {
        mpz_inp_str(py, fp, BASE);
        fclose(fp);
    }
    publicParameters.pointP = affine_init(px, py);

    fp = fopen ("PP/PP.pointPpublic.x","r");
    if (fp != NULL)
    {
        mpz_inp_str(ppx, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("PP/PP.pointPpublic.y","r");
    if (fp != NULL)
    {
        mpz_inp_str(ppy, fp, BASE);
        fclose(fp);
    }
    publicParameters.pointPpublic = affine_init(ppx, ppy);

    fp = fopen ("PP/PP.hashf","r");
    if (fp != NULL)
    {
        int i = 0;
        fread(&i, sizeof(int), 1, fp);
        publicParameters.hashFunction = (HashFunction) i;
        fclose(fp);
    }

    return publicParameters;
}

AffinePoint readPrivateKeyFromFiles()
{
    AffinePoint privateKey;
    mpz_t x, y;
    mpz_inits(x, y, NULL);

    FILE * fp;
    fp = fopen ("PK/privateX","r");
    if (fp != NULL)
    {
        mpz_inp_str(x, fp, BASE);
        fclose(fp);
    }
    else printf("Can't open file");

    fp = fopen ("PK/privateY","r");
    if (fp != NULL)
    {
        mpz_inp_str(y, fp, BASE);
        fclose(fp);
    }
    else printf("Can't open file");

    privateKey = affine_init(x, y);

    printf ("privateY  read in  "); mpz_out_str (stdout, BASE, privateKey.y); printf("\n");
    printf ("privateX  read in  "); mpz_out_str (stdout, BASE, privateKey.x); printf("\n");

    return privateKey;
}

CipherTextTuple readCipherTextFromFile()
{
    CipherTextTuple ciphertext;

    FILE * fp;

    fp = fopen("CT/cipher", "rb");
    if (fp != NULL) {

        fread(&ciphertext.cipherVLength, sizeof(ciphertext.cipherVLength), 1, fp);

	ciphertext.cipherV = malloc(ciphertext.cipherVLength + 1);
        fread(ciphertext.cipherV, ciphertext.cipherVLength + 1, 1, fp);
        fread(&ciphertext.cipherWLength, sizeof(ciphertext.cipherWLength), 1, fp);

	ciphertext.cipherW = malloc(ciphertext.cipherWLength + 1);
        fread(ciphertext.cipherW, ciphertext.cipherWLength + 1, 1, fp);

        fclose(fp);
    }

    else printf("Can't open file");
    printf("kiolvasott:\n");
    printf("VLength: %lu\n", ciphertext.cipherVLength);
    printf("%s\n", ciphertext.cipherV);
    printf("%lu\n", ciphertext.cipherWLength);
    printf("%s\n", ciphertext.cipherW);

    mpz_t x, y;
    mpz_inits(x, y, NULL);
    fp = fopen ("CT/cipherUx","r");
    if (fp != NULL) {
        mpz_inp_str(x, fp, BASE);
        fclose(fp);
    }

    fp = fopen ("CT/cipherUy","r");
    if (fp != NULL) {
        mpz_inp_str(y, fp, BASE);
        fclose(fp);
    }

    ciphertext.cipherU = affine_init(x, y);

    printf ("  read in  "); mpz_out_str (stdout, BASE, ciphertext.cipherU.x); printf("\n");
    printf ("  read in  "); mpz_out_str (stdout, BASE, ciphertext.cipherU.y); printf("\n");
    return ciphertext;
}
